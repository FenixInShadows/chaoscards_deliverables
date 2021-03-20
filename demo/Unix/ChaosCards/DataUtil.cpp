#include "DataUtil.h"

#include <GiglDefaultInclude.h>

#include <algorithm>
#include <numeric>
#include <functional>


/* Card structure representation */

NodeRep::NodeRep(unsigned _choice, const vector<double>& _term_info)
	: choice(_choice), term_info(_term_info)
{
}

string NodeRep::GetPrintVersion() const
{
	int term_len = term_info.size();
	string tmp_string = IntToStr(choice) + ", " + IntToStr(term_len) + ": ";
	for (int i = 0; i < term_len; i++)
		tmp_string += FloatToStr(term_info[i]) + " ";
	tmp_string += "; ";

	return tmp_string;
}

NodeRep mkNodeRep(unsigned choice, const vector<double>& term_info)
{
	return NodeRep(choice, term_info);
}

NodeRep mkNodeRep(unsigned choice)
{
	return NodeRep(choice, vector<double>{});
}

double NormalizeCode(double val, double min_val, double max_val)
{
	return (2 * val - max_val - min_val) / (max_val - min_val);
}

int DenormalizeCode(double code, double min_val, double max_val)
{
	return round(0.5 * ((1 + code) * max_val + (1 - code) * min_val));
}


/* Card generation info representation*/

ExtraCardGenConfig::ExtraCardGenConfig(const string& _name, NodeRep*& _rep)
	: name(_name), rep(_rep)
{
}

ExtraCardGenConfig* MkExtraCardGenConfig(const string& name, NodeRep*& rep)
{
	return new ExtraCardGenConfig(name, rep);
}


/* Card/Deck performance */

BalanceStat::BalanceStat()
	: num_wins(0), num_losses(0), total_num(0), win_contribution(0.0), total_participation(0.0), eval(0.5)
{
}

void BalanceStat::WinUpdate(double participation)
{
	num_wins++;
	total_num++;
	win_contribution += participation * 1.0;
	total_participation += participation;
}

void BalanceStat::LoseUpdate(double participation)
{
	num_losses++;
	total_num++;
	total_participation += participation;
}

void BalanceStat::DrawUpdate(double participation)
{
	total_num++;
	win_contribution += participation * 0.5;
	total_participation += participation;
}

void BalanceStat::UpdateEval()
{
	if (total_participation > 0)
		eval = win_contribution / total_participation;
}

void UpdateStatEvals(vector<BalanceStat>& stats)
{
	for (auto& stat : stats) // note the & is very important otherwise it won't update the original copies
		stat.UpdateEval();
}

bool StatIndexComparator(const vector<BalanceStat>& stats, int i, int j)
{
	return stats[i].eval > stats[j].eval;
}

void SortStatInIndices(const vector<BalanceStat>& stats, vector<int>& indices)
{
	iota(indices.begin(), indices.end(), 0); // fill index with {0,1,2,...}
	sort(indices.begin(), indices.end(), bind(StatIndexComparator, stats, placeholders::_1, placeholders::_2));
}


/* Match records */

MatchRec::MatchRec()
	: deck_a_indices(), deck_b_indices(), win_weight(0.0), weight(0.0), win_rate(0.5),
	sum_win_contrib_a(), ave_win_contrib_a(),
	sum_total_partic_a(), ave_total_partic_a(),
	sum_win_contrib_b(), ave_win_contrib_b(),
	sum_total_partic_b(), ave_total_partic_b()
{
}

MatchRec::MatchRec(const vector<int>& _deck_a_indices, const vector<int>& _deck_b_indices)
	: deck_a_indices(_deck_a_indices), deck_b_indices(_deck_b_indices), win_weight(0.0), weight(0.0), win_rate(0.5),
	sum_win_contrib_a(_deck_a_indices.size(), 0.0), ave_win_contrib_a(_deck_a_indices.size(), 0.5 / (double)deck_a_indices.size()),
	sum_total_partic_a(_deck_a_indices.size(), 0.0), ave_total_partic_a(_deck_a_indices.size(), 1.0 / (double)deck_a_indices.size()),
	sum_win_contrib_b(_deck_b_indices.size(), 0.0), ave_win_contrib_b(_deck_b_indices.size(), 0.5 / (double)deck_b_indices.size()),
	sum_total_partic_b(_deck_b_indices.size(), 0.0), ave_total_partic_b(_deck_b_indices.size(), 1.0 / (double)deck_b_indices.size())
{
}

void MatchRec::WinUpdate(const vector<double>& contribution_a, const vector<double>& contribution_b)
{
	win_weight += 1.0;
	weight += 1.0;
	for (int i = 0; i < contribution_a.size(); i++)
		sum_total_partic_a[i] += contribution_a[i];
	for (int i = 0; i < contribution_b.size(); i++)
	{
		sum_win_contrib_b[i] += contribution_b[i];
		sum_total_partic_b[i] += contribution_b[i];
	}
}

void MatchRec::LoseUpdate(const vector<double>& contribution_a, const vector<double>& contribution_b)
{
	weight += 1.0;
	for (int i = 0; i < contribution_a.size(); i++)
	{
		sum_win_contrib_a[i] += contribution_a[i];
		sum_total_partic_a[i] += contribution_a[i];
	}
	for (int i = 0; i < contribution_b.size(); i++)
		sum_total_partic_b[i] += contribution_b[i];
}


void MatchRec::DrawUpdate(const vector<double>& contribution_a, const vector<double>& contribution_b)
{
	win_weight += 0.5;
	weight += 1.0;
	for (int i = 0; i < contribution_a.size(); i++)
	{
		sum_win_contrib_a[i] += 0.5 * contribution_a[i];
		sum_total_partic_a[i] += contribution_a[i];
	}
	for (int i = 0; i < contribution_b.size(); i++)
	{
		sum_win_contrib_b[i] += 0.5 * contribution_b[i];
		sum_total_partic_b[i] += contribution_b[i];
	}
}

void MatchRec::UpdateStats()
{
	if (weight > 0)
	{
		win_rate = win_weight / weight;
		for (int i = 0; i < sum_win_contrib_a.size(); i++)
		{
			ave_win_contrib_a[i] = sum_win_contrib_a[i] / weight;
			ave_total_partic_a[i] = sum_total_partic_a[i] / weight;
		}
		for (int i = 0; i < sum_win_contrib_b.size(); i++)
		{
			ave_win_contrib_b[i] = sum_win_contrib_b[i] / weight;
			ave_total_partic_b[i] = sum_total_partic_b[i] / weight;
		}
	}
}