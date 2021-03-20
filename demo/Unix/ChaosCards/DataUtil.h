#pragma once

#include <vector>
#include <string>

using namespace std;


/* Card structure representation */

struct NodeRep
{
	NodeRep(unsigned _choice, const vector<double>& _term_info);
	string GetPrintVersion() const;
	unsigned choice;
	vector<double> term_info;
};

typedef vector<NodeRep> CardRep;

NodeRep mkNodeRep(unsigned choice, const vector<double>& term_info);
NodeRep mkNodeRep(unsigned choice);

double NormalizeCode(double val, double min_val, double max_val); // normalize to -1.0 ~ 1.0
int DenormalizeCode(double code, double min_val, double max_val); // de-normalize from -1.0 ~ 1.0 to an integer


/* Card generation info representation*/

struct ExtraCardGenConfig
{
	ExtraCardGenConfig(const string& _name, NodeRep*& _rep);
	string name;
	NodeRep*& rep;
};

ExtraCardGenConfig* MkExtraCardGenConfig(const string& name, NodeRep*& rep);


/* Card/Deck Performance */

struct BalanceStat // can be for a deck or a card
{
	BalanceStat();
	void WinUpdate(double participation);
	void LoseUpdate(double participation);
	void DrawUpdate(double participation);
	void UpdateEval();
	int num_wins;
	int num_losses;
	int total_num;
	double win_contribution;
	double total_participation;
	double eval;
};

void UpdateStatEvals(vector<BalanceStat>& stats);
bool StatIndexComparator(const vector<BalanceStat>& stats, int i, int j);
void SortStatInIndices(const vector<BalanceStat>& stats, vector<int>& indices);


/* Match records */

struct MatchRec // records for matches between two specific decks
{
	MatchRec();
	MatchRec(const vector<int>& _deck_a_indices, const vector<int>& _deck_b_indices);
	void WinUpdate(const vector<double>& contribution_a, const vector<double>& contribution_b); // b wins
	void LoseUpdate(const vector<double>& contribution_a, const vector<double>& contribution_b); // b losses
	void DrawUpdate(const vector<double>& contribution_a, const vector<double>& contribution_b); // draw
	void UpdateStats();
	vector<int> deck_a_indices;
	vector<int> deck_b_indices;
	double weight; // number of matches
	double win_weight; // wins for deck b over a (one draw counts as 0.5 wins for b)
	double win_rate; // win rate for deck b over a (one draw counts as 0.5 wins for b)
	vector<double> sum_win_contrib_a; // accumlated card win contribution for cards in deck a
	vector<double> ave_win_contrib_a; // average over matches
	vector<double> sum_total_partic_a; // accumulated card total participation for cards in deck a
	vector<double> ave_total_partic_a; // average over matches
	vector<double> sum_win_contrib_b;
	vector<double> ave_win_contrib_b;
	vector<double> sum_total_partic_b;
	vector<double> ave_total_partic_b;
};