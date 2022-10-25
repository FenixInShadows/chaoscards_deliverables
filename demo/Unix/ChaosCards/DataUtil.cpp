#include "DataUtil.h"

#include <GiglDefaultInclude.h>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <queue>

using namespace std;

/* Card structure representation */

NodeRep::NodeRep()
	: choice(0u), term_info()
{
}

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

#define REAL_RANGE_ADJUST 0.499999 // adjust the range for normalizing/denormalizing when the given range is in integers, to mitigate artificial biases

double NormalizeCode(double val, int min_int, int max_int)
{
	double min_val = min_int - REAL_RANGE_ADJUST;
	double max_val = max_int + REAL_RANGE_ADJUST;
	return (2 * val - max_val - min_val) / (max_val - min_val);
}

int DenormalizeCode(double code, int min_int, int max_int)
{
	double min_val = min_int - REAL_RANGE_ADJUST;
	double max_val = max_int + REAL_RANGE_ADJUST;
	return (int)round(0.5 * ((1 + code) * max_val + (1 - code) * min_val));
}

double DenormalizeCodeReal(double code, int min_int, int max_int)
{
	double min_val = min_int - REAL_RANGE_ADJUST;
	double max_val = max_int + REAL_RANGE_ADJUST;
	return 0.5 * ((1 + code) * max_val + (1 - code) * min_val);
}


int SampleDefaultStat()
{
	double roll = GetRandFloat();
	if (roll < 0.40) // 0 ~ 2
		return SampleRangeUpperConcentratedLinear(0.0, 2.5); // we don't use -REAL_RANGE_ADJUST as we want 0 to be weighted extra low
	else if (roll < 0.80) // 3 ~ 5
		return SampleRangeLowerConcentratedLinear(2.5, 5.5);
	else // 6 ~ 10
		return SampleRangeLowerConcentratedLinear(5.5, 10 + REAL_RANGE_ADJUST);
}

int SampleDefaultStatClamped(int min_int, int max_int) // Perform a clamp on top of the version above
{
	int num = SampleDefaultStat();
	if (num < min_int)
		return min_int;
	if (num > max_int)
		return max_int;
	return num;
}

int SampleNonLeaderCost()
{
	double roll = GetRandFloat();
	if (roll < 0.35) // 0 ~ 2
		return SampleRangeUpperConcentratedLinear(0.0, 2.5); // we don't use -REAL_RANGE_ADJUST as we want 0 to be weighted extra low
	else // 3 ~ 7
		return SampleRangeLowerConcentratedLinear(2.5, 7 + REAL_RANGE_ADJUST);
}

int SampleNonLeaderCostClamped(int min_int, int max_int) // Perform a clamp on top of the version above
{
	int num = SampleNonLeaderCost();
	if (num < min_int)
		return min_int;
	if (num > max_int)
		return max_int;
	return num;
}

int SampleRangeLowerConcentratedLinear(double min_val, double max_val)
{
	return SampleRangeUpperConcentratedLinear(max_val, min_val);
}

int SampleRangeLowerConcentratedLinearFromInt(int min_int, int max_int)
{
	return SampleRangeLowerConcentratedLinear(min_int - REAL_RANGE_ADJUST, max_int + REAL_RANGE_ADJUST);
}

int SampleRangeUpperConcentratedLinear(double min_val, double max_val)
{
	double x = sqrt(GetRandFloat());
	return (int)round(min_val + (max_val - min_val) * x);
}

int SampleRangeUpperConcentratedLinearFromInt(int min_int, int max_int)
{
	return SampleRangeUpperConcentratedLinear(min_int - REAL_RANGE_ADJUST, max_int + REAL_RANGE_ADJUST);
}

int SampleRangeCenterConcentratedLinear(double min_val, double max_val)
{
	double mid_val = (min_val + max_val) * 0.5;
	if (RandomRoll(0.5))
		return SampleRangeUpperConcentratedLinear(min_val, mid_val);
	else
		return SampleRangeLowerConcentratedLinear(mid_val, max_val);
}

int SampleRangeCenterConcentratedLinearFromInt(int min_int, int max_int)
{
	return SampleRangeCenterConcentratedLinear(min_int - REAL_RANGE_ADJUST, max_int + REAL_RANGE_ADJUST);
}

int SampleRangeLowerConcentratedPolynomial(double min_val, double max_val, double exponent)
{
	return SampleRangeUpperConcentratedPolynomial(max_val, min_val, exponent);
}

int SampleRangeLowerConcentratedPolynomialFromInt(int min_int, int max_int, double exponent)
{
	return SampleRangeLowerConcentratedPolynomial(min_int - REAL_RANGE_ADJUST, max_int + REAL_RANGE_ADJUST, exponent);
}

int SampleRangeUpperConcentratedPolynomial(double min_val, double max_val, double exponent)
{
	double x = pow(GetRandFloat(), 1.0/(exponent + 1.0));
	return (int)round(min_val + (max_val - min_val) * x);
}

int SampleRangeUpperConcentratedPolynomialFromInt(int min_int, int max_int, double exponent)
{
	return SampleRangeUpperConcentratedPolynomial(min_int - REAL_RANGE_ADJUST, max_int + REAL_RANGE_ADJUST, exponent);
}

int SampleRangeCenterConcentratedPolynomial(double min_val, double max_val, double exponent)
{
	double mid_val = (min_val + max_val) * 0.5;
	if (RandomRoll(0.5))
		return SampleRangeUpperConcentratedPolynomial(min_val, mid_val, exponent);
	else
		return SampleRangeLowerConcentratedPolynomial(mid_val, max_val, exponent);
}

int SampleRangeCenterConcentratedPolynomialFromInt(int min_int, int max_int, double exponent)
{
	return SampleRangeCenterConcentratedPolynomial(min_int - REAL_RANGE_ADJUST, max_int + REAL_RANGE_ADJUST, exponent);
}


int ClassifyDiff(unsigned choice_a, unsigned choice_b, int& entry_count)
{
	entry_count++;
	return (int)(choice_a != choice_b);
}

double ValueDiff(double val_a, double val_b, int& entry_count)
{
	entry_count++;
	double scaled_val_delta = 0.5 * (val_a - val_b); // this 0.5 is because we want to keep the metric be in 0 ~ 1 (and vals are in -1 ~ 1)
	return scaled_val_delta * scaled_val_delta;
}


double DiffCardRep(CardRep& rep_a, CardRep& rep_b)
{
	NodeRep* start_rep_a = rep_a.data();
	NodeRep* start_rep_b = rep_b.data();
	int entry_count = 0;
	double summed_diff = DiffCardRoot(start_rep_a, start_rep_b, entry_count);
	return summed_diff / (double)entry_count;
}

void SkipCardRoot(NodeRep*& rep)
{
	unsigned choice = (rep++)->choice;

	switch (choice)
	{
	case CHOICE_LEADER_CARD:
		SkipAttackTimes(rep);
		SkipAbilities(rep);
		SkipSpecialEffects(rep);
		break;
	case CHOICE_MINION_CARD:
		SkipAttackTimes(rep);
		SkipMinionType(rep);
		SkipAbilities(rep);
		SkipSpecialEffects(rep);
		break;
	case CHOICE_SPELL_CARD:
		SkipAbilities(rep);
		SkipSpecialEffects(rep);
		break;
	}
}
double DiffCardRoot(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	const double* term_info_a = rep_a->term_info.data();
	unsigned choice_a = rep_a->choice;
	const double* term_info_b = rep_b->term_info.data();
	unsigned choice_b = rep_b->choice;

	int classify_diff = ClassifyDiff(choice_a, choice_b, entry_count);
	if (classify_diff) // skip the rest of the subtree if the choice is wrong
	{
		SkipCardRoot(rep_a);
		SkipCardRoot(rep_b);
		return 1.0;
	}
	rep_a++;
	rep_b++;

	double cost_diff = 0.0;
	double attack_diff = 0.0;
	double hp_diff = 0.0;
	double attack_times_diff = 0.0;
	double minion_type_diff = 0.0;
	double abilities_diff = 0.0;
	double effects_diff = 0.0;

	switch (choice_a)
	{
	case CHOICE_LEADER_CARD:
		cost_diff = ValueDiff(term_info_a[0], term_info_b[0], entry_count);
		attack_diff = ValueDiff(term_info_a[1], term_info_b[1], entry_count);
		hp_diff = ValueDiff(term_info_a[2], term_info_b[2], entry_count);
		attack_times_diff = DiffAttackTimes(rep_a, rep_b, entry_count); // note: the pointers will be incremented inside (similar below)
		minion_type_diff = 0.0; // ignore parts that does not actually exist (similar below)
		abilities_diff = DiffAbilities(rep_a, rep_b, entry_count);
		effects_diff = DiffSpecialEffects(rep_a, rep_b, entry_count);
		break;
	case CHOICE_MINION_CARD:
		cost_diff = ValueDiff(term_info_a[0], term_info_b[0], entry_count);
		attack_diff = ValueDiff(term_info_a[1], term_info_b[1], entry_count);
		hp_diff = ValueDiff(term_info_a[2], term_info_b[2], entry_count);
		attack_times_diff = DiffAttackTimes(rep_a, rep_b, entry_count);
		minion_type_diff = DiffMinionType(rep_a, rep_b, entry_count);
		abilities_diff = DiffAbilities(rep_a, rep_b, entry_count);
		effects_diff = DiffSpecialEffects(rep_a, rep_b, entry_count);
		break;
	case CHOICE_SPELL_CARD:
		cost_diff = ValueDiff(term_info_a[0], term_info_b[0], entry_count);
		attack_diff = 0.0;
		hp_diff = 0.0;
		attack_times_diff = 0.0;
		minion_type_diff = 0.0;
		abilities_diff = DiffAbilities(rep_a, rep_b, entry_count);
		effects_diff = DiffSpecialEffects(rep_a, rep_b, entry_count);
		break;
	}

	return cost_diff + attack_diff + hp_diff + attack_times_diff + minion_type_diff + abilities_diff + effects_diff;
}

void SkipAttackTimes(NodeRep*& rep)
{
	// no subtrees no matter the choice
	rep++;
}
double DiffAttackTimes(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	const double* term_info_a = rep_a->term_info.data();
	unsigned choice_a = (rep_a++)->choice;
	const double* term_info_b = rep_b->term_info.data();
	unsigned choice_b = (rep_b++)->choice;

	double attack_times_val_a = 1.0;
	switch (choice_a)
	{
	case CHOICE_ZERO_ATTACK:
		attack_times_val_a = NormalizeCode(0, 0, 5);
		break;
	case CHOICE_SINGLE_ATTACK:
		attack_times_val_a = NormalizeCode(1, 0, 5);
		break;
	case CHOICE_MULTIPLE_ATTACK:
		attack_times_val_a = term_info_a[0];
		break;
	}

	double attack_times_val_b = 1.0;
	switch (choice_b)
	{
	case CHOICE_ZERO_ATTACK:
		attack_times_val_b = NormalizeCode(0, 0, 5);
		break;
	case CHOICE_SINGLE_ATTACK:
		attack_times_val_b = NormalizeCode(1, 0, 5);
		break;
	case CHOICE_MULTIPLE_ATTACK:
		attack_times_val_b = term_info_b[0];
		break;
	}

	return ValueDiff(attack_times_val_a, attack_times_val_b, entry_count);
}

void SkipMinionType(NodeRep*& rep)
{
	rep++;
}
double DiffMinionType(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	return (double)ClassifyDiff((rep_a++)->choice, (rep_b++)->choice, entry_count);
}

void SkipAbilities(NodeRep*& rep)
{	
	for (int i = 0; i < 7; i++)
		SkipAnyAbil(rep);
}
double DiffAbilities(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	double charge_diff = DiffAnyAbil(rep_a, rep_b, entry_count);
	double taunt_diff = DiffAnyAbil(rep_a, rep_b, entry_count);
	double stealth_diff = DiffAnyAbil(rep_a, rep_b, entry_count);
	double untargetable_diff = DiffAnyAbil(rep_a, rep_b, entry_count);
	double shield_diff = DiffAnyAbil(rep_a, rep_b, entry_count);
	double poisonous_diff = DiffAnyAbil(rep_a, rep_b, entry_count);
	double lifesteal_diff = DiffAnyAbil(rep_a, rep_b, entry_count);
	return charge_diff + taunt_diff + stealth_diff + untargetable_diff + shield_diff + poisonous_diff + lifesteal_diff;
}

void SkipDamageAbilities(NodeRep*& rep)
{
	for (int i = 0; i < 2; i++)
		SkipAnyAbil(rep);
}
double DiffDamageAbilities(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	double poisonous_diff = DiffAnyAbil(rep_a, rep_b, entry_count);
	double lifesteal_diff = DiffAnyAbil(rep_a, rep_b, entry_count);
	return poisonous_diff + lifesteal_diff;
}

void SkipAnyAbil(NodeRep*& rep)
{
	rep++;
}
double DiffAnyAbil(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	return (double)ClassifyDiff((rep_a++)->choice, (rep_b++)->choice, entry_count);
}

void SkipSpecialEffects(NodeRep*& rep)
{
	SkipTargetedPlayEff(rep);
	SkipOtherEffs(rep);
}
double DiffSpecialEffects(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	double targeted_play_eff_diff = DiffTargetedPlayEff(rep_a, rep_b, entry_count);
	double other_effs_diff = DiffOtherEffs(rep_a, rep_b, entry_count);
	return targeted_play_eff_diff + other_effs_diff;
}

void SkipTargetedPlayEff(NodeRep*& rep)
{
	unsigned choice = (rep++)->choice;

	switch (choice)
	{
	case CHOICE_TARGETED_BATTLECRY_EFF:
	case CHOICE_TARGETED_CAST_EFF:
		SkipTargetedEff(rep);
		break;
	}
}
double DiffTargetedPlayEff(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	unsigned choice_a = rep_a->choice;
	unsigned choice_b = rep_b->choice;

	int classify_diff = ClassifyDiff(choice_a, choice_b, entry_count);
	if (classify_diff)
	{
		SkipTargetedPlayEff(rep_a);
		SkipTargetedPlayEff(rep_b);
		return 1.0;
	}
	rep_a++;
	rep_b++;

	double effect_diff = 0.0;

	switch (choice_a)
	{
	case CHOICE_NO_TARGETED_PLAY_EFF:
		effect_diff = 0.0;
		break;
	case CHOICE_TARGETED_BATTLECRY_EFF:
	case CHOICE_TARGETED_CAST_EFF:
		effect_diff = DiffTargetedEff(rep_a, rep_b, entry_count);
		break;
	}
	return effect_diff;
}

void SkipOtherEffs(NodeRep*& rep)
{
	unsigned choice = (rep++)->choice;

	switch (choice)
	{
	case CHOICE_CONS_OTHER_EFFS:
		SkipOtherEff(rep);
		SkipOtherEffs(rep);
		break;
	}
}
double DiffOtherEffs(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	unsigned choice_a = rep_a->choice;
	unsigned choice_b = rep_b->choice;

	int classify_diff = ClassifyDiff(choice_a, choice_b, entry_count);
	if (classify_diff)
	{
		SkipOtherEffs(rep_a);
		SkipOtherEffs(rep_b);
		return 1.0;
	}
	rep_a++;
	rep_b++;

	double effects_diff = 0.0;

	switch (choice_a)
	{
	default: // fall through
	case CHOICE_NO_OTHER_EFFS:
		effects_diff = 0.0;
		break;
	case CHOICE_CONS_OTHER_EFFS:
		{
			double head_eff_diff = DiffOtherEff(rep_a, rep_b, entry_count);
			double tail_effs_diff = DiffOtherEffs(rep_a, rep_b, entry_count);
			effects_diff = head_eff_diff + tail_effs_diff;
		}
		break;
	}
	return effects_diff;
}

void SkipOtherEff(NodeRep*& rep)
{
	unsigned choice = (rep++)->choice;

	SkipUntargetedEff(rep); // all rules share this part as the next node
	switch (choice)
	{
	case CHOICE_TURN_START_EFF:
	case CHOICE_TURN_END_EFF:
		SkipAllegianceCond(rep);
		break;
	}
}
double DiffOtherEff(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	unsigned choice_a = rep_a->choice;
	unsigned choice_b = rep_b->choice;

	int classify_diff = ClassifyDiff(choice_a, choice_b, entry_count);
	if (classify_diff)
	{
		SkipOtherEff(rep_a);
		SkipOtherEff(rep_b);
		return 1.0;
	}
	rep_a++;
	rep_b++;

	double effect_diff = DiffUntargetedEff(rep_a, rep_b, entry_count); // all rules share this part as the next node
	double alle_diff = 0.0;

	switch (choice_a)
	{
	case CHOICE_UNTARGETED_BATTLECRY_EFF:
	case CHOICE_UNTARGETED_CAST_EFF:
	case CHOICE_DEATHRATTLE_EFF:
	case CHOICE_ONDISCARD_EFF:
		alle_diff = 0.0;
		break;
	case CHOICE_TURN_START_EFF:
	case CHOICE_TURN_END_EFF:
		alle_diff = DiffAllegianceCond(rep_a, rep_b, entry_count);
		break;
	}
	return effect_diff + alle_diff;
}

void SkipTargetedEff(NodeRep*& rep)
{
	unsigned choice = (rep++)->choice;

	SkipBaseTargetedEff(rep); // all rules share this part as the next node
	SkipTargetCond(rep); // all rules share this part and at the next node
	switch (choice)
	{
	case CHOICE_INDE_COND_EFF:
		SkipIndeCond(rep);
		break;
	case CHOICE_SRC_COND_EFF:
		SkipTargetCond(rep);
		break;
	}
}
double DiffTargetedEff(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	unsigned choice_a = rep_a->choice;
	unsigned choice_b = rep_b->choice;

	int classify_diff = ClassifyDiff(choice_a, choice_b, entry_count);
	if (classify_diff)
	{
		SkipTargetedEff(rep_a);
		SkipTargetedEff(rep_b);
		return 1.0;
	}
	rep_a++;
	rep_b++;

	double effect_diff = DiffBaseTargetedEff(rep_a, rep_b, entry_count); // all rules share this part as the next node
	double constr_diff = DiffTargetCond(rep_a, rep_b, entry_count); // all rules share this part and at the next node
	double cond_diff = 0.0;

	switch (choice_a)
	{
	case CHOICE_NO_COND_EFF:
		cond_diff = 0.0;
		break;
	case CHOICE_INDE_COND_EFF:
		cond_diff = DiffIndeCond(rep_a, rep_b, entry_count);
		break;
	case CHOICE_SRC_COND_EFF:
		cond_diff = DiffTargetCond(rep_a, rep_b, entry_count);
		break;
	}
	return effect_diff + constr_diff + cond_diff;
}

void SkipUntargetedEff(NodeRep*& rep)
{
	unsigned choice = (rep++)->choice;

	SkipBaseUntargetedEff(rep); // all rules share this part as the next node
	switch (choice)
	{
	case CHOICE_INDE_COND_EFF:
		SkipIndeCond(rep);
		break;
	case CHOICE_SRC_COND_EFF:
		SkipTargetCond(rep);
		break;
	}
}
double DiffUntargetedEff(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	unsigned choice_a = rep_a->choice;
	unsigned choice_b = rep_b->choice;

	int classify_diff = ClassifyDiff(choice_a, choice_b, entry_count);
	if (classify_diff)
	{
		SkipUntargetedEff(rep_a);
		SkipUntargetedEff(rep_b);
		return 1.0;
	}
	rep_a++;
	rep_b++;

	double effect_diff = DiffBaseUntargetedEff(rep_a, rep_b, entry_count); // all rules share this part as the next node
	double cond_diff = 0.0;

	switch (choice_a)
	{
	case CHOICE_NO_COND_EFF:
		cond_diff = 0.0;
		break;
	case CHOICE_INDE_COND_EFF:
		cond_diff = DiffIndeCond(rep_a, rep_b, entry_count);
		break;
	case CHOICE_SRC_COND_EFF:
		cond_diff = DiffTargetCond(rep_a, rep_b, entry_count);
		break;
	}
	return effect_diff + cond_diff;
}

void SkipTargetCond(NodeRep*& rep)
{
	unsigned choice = (rep++)->choice;

	switch (choice)
	{
	case CHOICE_CHAR_TARGET_COND:
		SkipCharTargetCond(rep);
		break;
	case CHOICE_CARD_TARGET_COND:
		SkipCardTargetCond(rep);
		break;
	}
}
double DiffTargetCond(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	unsigned choice_a = rep_a->choice;
	unsigned choice_b = rep_b->choice;

	int classify_diff = ClassifyDiff(choice_a, choice_b, entry_count);
	if (classify_diff)
	{
		SkipTargetCond(rep_a);
		SkipTargetCond(rep_b);
		return 1.0;
	}
	rep_a++;
	rep_b++;

	double cond_diff = 0.0;

	switch (choice_a)
	{
	case CHOICE_CHAR_TARGET_COND:
		cond_diff = DiffCharTargetCond(rep_a, rep_b, entry_count);
		break;
	case CHOICE_CARD_TARGET_COND:
		cond_diff = DiffCardTargetCond(rep_a, rep_b, entry_count);
		break;
	}
	return cond_diff;
}

void SkipCharTargetCond(NodeRep*& rep)
{
	SkipAllegianceCond(rep);
	SkipCharTypeCond(rep);
	SkipAbilCond(rep);
	SkipStatCond(rep);
}
double DiffCharTargetCond(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	double alle_diff = DiffAllegianceCond(rep_a, rep_b, entry_count);
	double type_diff = DiffCharTypeCond(rep_a, rep_b, entry_count);
	double abil_diff = DiffAbilCond(rep_a, rep_b, entry_count);
	double stat_diff = DiffStatCond(rep_a, rep_b, entry_count);
	return alle_diff + type_diff + abil_diff + stat_diff;
}

void SkipCharTypeCond(NodeRep*& rep)
{
	rep++;
}
double DiffCharTypeCond(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	return (double)ClassifyDiff((rep_a++)->choice, (rep_b++)->choice, entry_count);
}

void SkipCardTargetCond(NodeRep*& rep)
{
	SkipCardPosCond(rep);
	SkipAllegianceCond(rep);
	SkipCardTypeCond(rep);
	SkipAbilCond(rep);
	SkipStatCond(rep);
}
double DiffCardTargetCond(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	double pos_diff = DiffCardPosCond(rep_a, rep_b, entry_count);
	double alle_diff = DiffAllegianceCond(rep_a, rep_b, entry_count);
	double type_diff = DiffCardTypeCond(rep_a, rep_b, entry_count);
	double abil_diff = DiffAbilCond(rep_a, rep_b, entry_count);
	double stat_diff = DiffStatCond(rep_a, rep_b, entry_count);
	return pos_diff + alle_diff + type_diff + abil_diff + stat_diff;
}

void SkipCardTypeCond(NodeRep*& rep)
{
	rep++;
}
double DiffCardTypeCond(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	return (double)ClassifyDiff((rep_a++)->choice, (rep_b++)->choice, entry_count);
}

void SkipCardPosCond(NodeRep*& rep)
{
	rep++;
}
double DiffCardPosCond(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	return (double)ClassifyDiff((rep_a++)->choice, (rep_b++)->choice, entry_count);
}

void SkipAllegianceCond(NodeRep*& rep)
{
	rep++;
}
double DiffAllegianceCond(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	return (double)ClassifyDiff((rep_a++)->choice, (rep_b++)->choice, entry_count);
}

void SkipAbilCond(NodeRep*& rep)
{
	rep++;
}
double DiffAbilCond(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	return (double)ClassifyDiff((rep_a++)->choice, (rep_b++)->choice, entry_count);
}

void SkipStatCond(NodeRep*& rep)
{
	unsigned choice = (rep++)->choice;

	switch (choice)
	{
	case CHOICE_NO_STAT_COND:
		break;
	default:
		SkipStatCondVariant(rep);
		break;
	}
}
double DiffStatCond(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	unsigned choice_a = rep_a->choice;
	unsigned choice_b = rep_b->choice;


	int classify_diff = ClassifyDiff(choice_a, choice_b, entry_count);
	if (classify_diff)
	{
		SkipStatCond(rep_a);
		SkipStatCond(rep_b);
		return 1.0;
	}
	rep_a++;
	rep_b++;

	double variant_diff = 0.0;

	switch (choice_a)
	{
	case CHOICE_NO_STAT_COND:
		variant_diff = 0.0;
		break;
	default:
		variant_diff = DiffStatCondVariant(rep_a, rep_b, entry_count);
		break;
	}
	return variant_diff;
}

void SkipStatCondVariant(NodeRep*& rep)
{
	rep++;
}
double DiffStatCondVariant(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	const double* term_info_a = rep_a->term_info.data();
	unsigned choice_a = rep_a->choice;
	const double* term_info_b = rep_b->term_info.data();
	unsigned choice_b = rep_b->choice;

	int classify_diff = ClassifyDiff(choice_a, choice_b, entry_count);
	if (classify_diff)
	{
		SkipStatCondVariant(rep_a);
		SkipStatCondVariant(rep_b);
		return 1.0;
	}
	rep_a++;
	rep_b++;

	double val_diff = ValueDiff(term_info_a[0], term_info_b[0], entry_count);
	return val_diff;
}

void SkipIndeCond(NodeRep*& rep)
{
	unsigned choice = (rep++)->choice;

	switch (choice)
	{
	case CHOICE_FIELD_EXISTENCE_COND:
		SkipCharTargetCond(rep);
		break;
	case CHOICE_CARD_EXISTENCE_COND:
		SkipCardTargetCond(rep);
		break;
	case CHOICE_LEADER_COND:
		SkipAllegianceCond(rep);
		SkipAbilCond(rep);
		SkipStatCond(rep);
		break;
	case CHOICE_MP_COND:
	case CHOICE_MAX_MP_COND:
		SkipAllegianceCond(rep);
		SkipStatCondVariant(rep);
		break;
	}
}
double DiffIndeCond(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	unsigned choice_a = rep_a->choice;
	unsigned choice_b = rep_b->choice;

	int classify_diff = ClassifyDiff(choice_a, choice_b, entry_count);
	if (classify_diff)
	{
		SkipIndeCond(rep_a);
		SkipIndeCond(rep_b);
		return 1.0;
	}
	rep_a++;
	rep_b++;

	double cond_diff = 0.0;

	switch (choice_a)
	{
	case CHOICE_FIELD_EXISTENCE_COND:
		cond_diff = DiffCharTargetCond(rep_a, rep_b, entry_count);
		break;
	case CHOICE_CARD_EXISTENCE_COND:
		cond_diff = DiffCardTargetCond(rep_a, rep_b, entry_count);
		break;
	case CHOICE_LEADER_COND:
		{
			double alle_diff = DiffAllegianceCond(rep_a, rep_b, entry_count);
			double abil_diff = DiffAbilCond(rep_a, rep_b, entry_count);
			double stat_diff = DiffStatCond(rep_a, rep_b, entry_count);
			cond_diff = alle_diff + abil_diff + stat_diff;
		}
		break;
	case CHOICE_MP_COND:
	case CHOICE_MAX_MP_COND:
		{
			double alle_diff = DiffAllegianceCond(rep_a, rep_b, entry_count);
			double variant_diff = DiffStatCondVariant(rep_a, rep_b, entry_count);
			cond_diff = alle_diff + variant_diff;
		}
		break;
	}
	return cond_diff;
}

void SkipBaseTargetedEff(NodeRep*& rep)
{
	unsigned choice = (rep++)->choice;

	switch (choice)
	{
	case CHOICE_DAMAGE_EFF:
		SkipDamageAbilities(rep);
		break;
	case CHOICE_MOVE_EFF:
	case CHOICE_COPY_EFF:
		SkipDestination(rep);
		break;
	case CHOICE_TRANSFORM_EFF:
		SkipNewCardVariant(rep);
		break;
	case CHOICE_GIVE_EFFECTS_EFF:
		SkipSpecialEffects(rep);
		break;
	}
}
double DiffBaseTargetedEff(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	const double* term_info_a = rep_a->term_info.data();
	unsigned choice_a = rep_a->choice;
	const double* term_info_b = rep_b->term_info.data();
	unsigned choice_b = rep_b->choice;

	int classify_diff = ClassifyDiff(choice_a, choice_b, entry_count);
	if (classify_diff)
	{
		SkipBaseTargetedEff(rep_a);
		SkipBaseTargetedEff(rep_b);
		return 1.0;
	}
	rep_a++;
	rep_b++;

	// the categorization there doesn't quite matter, but the number of terminal values has to match
	double offensive_diff = 0.0;
	double defensive_diff = 0.0;
	double resource_diff = 0.0;
	double utility_diff = 0.0;

	switch (choice_a)
	{
	case CHOICE_RES_ATK_TIMES_EFF:
	case CHOICE_ATK_TIMES_MOD_EFF:
		offensive_diff = ValueDiff(term_info_a[0], term_info_b[0], entry_count);
		defensive_diff = 0.0;
		resource_diff = 0.0;
		utility_diff = 0.0;
		break;
	case CHOICE_HEAL_EFF:
		offensive_diff = 0.0;
		defensive_diff = ValueDiff(term_info_a[0], term_info_b[0], entry_count);
		resource_diff = 0.0;
		utility_diff = 0.0;
		break;
	case CHOICE_ATK_HP_MOD_EFF:
		offensive_diff = ValueDiff(term_info_a[0], term_info_b[0], entry_count);
		defensive_diff = ValueDiff(term_info_a[1], term_info_b[1], entry_count);
		resource_diff = 0.0;
		utility_diff = 0.0;
		break;
	case CHOICE_COST_MOD_EFF:
	case CHOICE_DEC_OH_THRESHOLD_EFF:
		offensive_diff = 0.0;
		defensive_diff = 0.0;
		resource_diff = ValueDiff(term_info_a[0], term_info_b[0], entry_count);
		utility_diff = 0.0;
		break;
	case CHOICE_DAMAGE_EFF:
		offensive_diff = ValueDiff(term_info_a[0], term_info_b[0], entry_count);
		defensive_diff = 0.0;
		resource_diff = 0.0;
		utility_diff = DiffDamageAbilities(rep_a, rep_b, entry_count);
		break;
	case CHOICE_MOVE_EFF:
	case CHOICE_COPY_EFF:
		offensive_diff = 0.0;
		defensive_diff = 0.0;
		resource_diff = 0.0;
		utility_diff = DiffDestination(rep_a, rep_b, entry_count);
		break;
	case CHOICE_TRANSFORM_EFF:
		offensive_diff = 0.0;
		defensive_diff = 0.0;
		resource_diff = 0.0;
		utility_diff = DiffNewCardVariant(rep_a, rep_b, entry_count);
		break;
	case CHOICE_GIVE_EFFECTS_EFF:
		offensive_diff = 0.0;
		defensive_diff = 0.0;
		resource_diff = 0.0;
		utility_diff = DiffSpecialEffects(rep_a, rep_b, entry_count);
		break;
	default:
		offensive_diff = 0.0;
		defensive_diff = 0.0;
		resource_diff = 0.0;
		utility_diff = 0.0;
		break;
	}
	return offensive_diff + defensive_diff + resource_diff + utility_diff;
}

void SkipBaseUntargetedEff(NodeRep*& rep)
{
	unsigned choice = (rep++)->choice;

	switch (choice)
	{
	case CHOICE_AOE_EFF:
	case CHOICE_RAND_EFF:
		SkipBaseTargetedEff(rep);
		SkipTargetCond(rep);
		break;
	case CHOICE_LEADER_EFF:
		SkipBaseTargetedEff(rep);
		SkipAllegianceCond(rep);
		break;
	case CHOICE_SELF_EFF:
		SkipBaseTargetedEff(rep);
		break;
	case CHOICE_DRAW_CARD_EFF:
	case CHOICE_MP_EFF:
	case CHOICE_MAX_MP_EFF:
		SkipAllegianceCond(rep);
		break;
	case CHOICE_NEW_EFF:
		SkipDestination(rep);
		SkipNewCardVariant(rep);
		break;
	case CHOICE_WIN_GAME_EFF:
		SkipAllegianceCond(rep);
		break;
	}
}
double DiffBaseUntargetedEff(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	const double* term_info_a = rep_a->term_info.data();
	unsigned choice_a = rep_a->choice;
	const double* term_info_b = rep_b->term_info.data();
	unsigned choice_b = rep_b->choice;

	int classify_diff = ClassifyDiff(choice_a, choice_b, entry_count);
	if (classify_diff)
	{
		SkipBaseUntargetedEff(rep_a);
		SkipBaseUntargetedEff(rep_b);
		return 1.0;
	}
	rep_a++;
	rep_b++;

	double effect_diff = 0.0;
	double cond_diff = 0.0;
	double val_diff = 0.0;

	switch (choice_a)
	{
	case CHOICE_AOE_EFF:
	case CHOICE_RAND_EFF:
		effect_diff = DiffBaseTargetedEff(rep_a, rep_b, entry_count);
		cond_diff = DiffTargetCond(rep_a, rep_b, entry_count);
		val_diff = 0.0;
		break;
	case CHOICE_LEADER_EFF:
		effect_diff = DiffBaseTargetedEff(rep_a, rep_b, entry_count);
		cond_diff = DiffAllegianceCond(rep_a, rep_b, entry_count);
		val_diff = 0.0;
		break;
	case CHOICE_SELF_EFF:
		effect_diff = DiffBaseTargetedEff(rep_a, rep_b, entry_count);
		cond_diff = 0.0;
		val_diff = 0.0;
		break;
	case CHOICE_DRAW_CARD_EFF:
	case CHOICE_MP_EFF:
	case CHOICE_MAX_MP_EFF:
		effect_diff = 0.0;
		cond_diff = DiffAllegianceCond(rep_a, rep_b, entry_count);
		val_diff = ValueDiff(term_info_a[0], term_info_b[0], entry_count);
		break;
	case CHOICE_NEW_EFF:
		{
			double dest_diff = DiffDestination(rep_a, rep_b, entry_count);
			double variant_diff = DiffNewCardVariant(rep_a, rep_b, entry_count);
			effect_diff = dest_diff + variant_diff;
		}
		cond_diff = 0.0;
		val_diff = ValueDiff(term_info_a[0], term_info_b[0], entry_count);
		break;
	case CHOICE_WIN_GAME_EFF:
		effect_diff = 0.0;
		cond_diff = DiffAllegianceCond(rep_a, rep_b, entry_count);
		val_diff = 0.0;
		break;
	}
	return effect_diff + cond_diff + val_diff;
}

void SkipDestination(NodeRep*& rep)
{
	unsigned choice = (rep++)->choice;

	switch (choice)
	{
	case CHOICE_FIELD_DEST:
	case CHOICE_HAND_DEST:
	case CHOICE_DECK_DEST:
		SkipAllegianceCond(rep);
		break;
	}
}
double DiffDestination(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	unsigned choice_a = rep_a->choice;
	unsigned choice_b = rep_b->choice;

	int classify_diff = ClassifyDiff(choice_a, choice_b, entry_count);
	if (classify_diff)
	{
		SkipDestination(rep_a);
		SkipDestination(rep_b);
		return 1.0;
	}
	rep_a++;
	rep_b++;

	double alle_diff = 0.0;

	switch (choice_a)
	{
	case CHOICE_FIELD_DEST:
	case CHOICE_HAND_DEST:
	case CHOICE_DECK_DEST:
		alle_diff = DiffAllegianceCond(rep_a, rep_b, entry_count);
		break;
	case CHOICE_OWN_FIELD_DEST:
	case CHOICE_OWN_HAND_DEST:
	case CHOICE_OWN_DECK_DEST:
		alle_diff = 0.0;
		break;
	}
	return alle_diff;
}

void SkipNewCardVariant(NodeRep*& rep)
{
	unsigned choice = (rep++)->choice;

	switch (choice)
	{
	case CHOICE_PLAIN_MINION_CARD:
	case CHOICE_PLAIN_MINION:
	case CHOICE_FIXED_CARD:
	case CHOICE_FIXED_MINION:
		SkipCardRoot(rep);
		break;
	}
}
double DiffNewCardVariant(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count)
{
	const double* term_info_a = rep_a->term_info.data();
	unsigned choice_a = rep_a->choice;
	const double* term_info_b = rep_b->term_info.data();
	unsigned choice_b = rep_b->choice;

	int classify_diff = ClassifyDiff(choice_a, choice_b, entry_count);
	if (classify_diff)
	{
		SkipNewCardVariant(rep_a);
		SkipNewCardVariant(rep_b);
		return 1.0;
	}
	rep_a++;
	rep_b++;

	double card_diff = 0.0;
	double cost_diff = 0.0;

	switch (choice_a)
	{
	case CHOICE_PLAIN_MINION_CARD:
	case CHOICE_PLAIN_MINION:
	case CHOICE_FIXED_CARD:
	case CHOICE_FIXED_MINION:
		card_diff = DiffCardRoot(rep_a, rep_b, entry_count);
		cost_diff = 0.0;
		break;
	case CHOICE_RANDOM_CARD:
	case CHOICE_RANDOM_MINION:
		card_diff = 0.0;
		cost_diff = ValueDiff(term_info_a[0], term_info_b[0], entry_count);
		break;
	}
	return card_diff + cost_diff;
}


/* Card generation info representation*/

ExtraCardGenConfig::ExtraCardGenConfig(const string& _name, NodeRep*& _rep)
	: name(_name), rep(_rep), min_cost(0), max_cost(10), forcing_minion(false)
{
}

ExtraCardGenConfig::ExtraCardGenConfig(const string& _name, NodeRep*& _rep, int _min_cost, int _max_cost, bool _forcing_minion)
	: name(_name), rep(_rep), min_cost(_min_cost), max_cost(_max_cost), forcing_minion(_forcing_minion)
{
}

ExtraCardGenConfig* MkExtraCardGenConfig(const string& name, NodeRep*& rep)
{
	return new ExtraCardGenConfig(name, rep);
}

ExtraCardGenConfig* MkExtraCardGenConfigWithCost(const std::string& name, NodeRep*& rep, int cost, bool forcing_minion)
{
	return new ExtraCardGenConfig(name, rep, cost, cost);
}


/* Card/Deck performance */

BalanceStat::BalanceStat()
	: num_wins(0), num_losses(0), total_num(0), effective_win_contribution(0.0), draw_contribution(0.0), snd_player_eff_win_contrib(0.0), total_participation(0.0), eval(0.5), card_cost(-1), sum_weighted_num_turns(0.0), cost_constrained_participation(0.0), adjusted_participation(0.0)
{
}

void BalanceStat::SetCardCost(int cost)
{
	card_cost = cost;
}

void BalanceStat::ResetStat()
{
	num_wins = 0;
	num_losses = 0;
	total_num = 0;
	effective_win_contribution = 0.0;
	draw_contribution = 0.0;
	snd_player_eff_win_contrib = 0.0;
	total_participation = 0.0;
	eval = 0.5;
	sum_weighted_num_turns = 0.0;
	cost_constrained_participation = 0.0;
	adjusted_participation = 0.0;
}

void BalanceStat::WinUpdate(double participation, bool is_snd_player)
{
	num_wins++;
	total_num++;
	effective_win_contribution += participation;
	if (is_snd_player)
		snd_player_eff_win_contrib += participation;
	total_participation += participation;
}

void BalanceStat::WinUpdate(double participation, double cost_constr_partic, int num_turns, bool is_snd_player)
{
	WinUpdate(participation, is_snd_player);
	sum_weighted_num_turns += participation * (double)num_turns;
	cost_constrained_participation += cost_constr_partic;
}

void BalanceStat::LoseUpdate(double participation, bool is_snd_player)
{
	num_losses++;
	total_num++;
	total_participation += participation;
}

void BalanceStat::LoseUpdate(double participation, double cost_constr_partic, int num_turns, bool is_snd_player)
{
	LoseUpdate(participation, is_snd_player);
	sum_weighted_num_turns += participation * (double)num_turns;
	cost_constrained_participation += cost_constr_partic;
}

void BalanceStat::DrawUpdate(double participation, bool is_snd_player)
{
	total_num++;
	effective_win_contribution += participation * 0.5;
	draw_contribution += participation;
	if (is_snd_player)
		snd_player_eff_win_contrib += participation * 0.5;
	total_participation += participation;
}

void BalanceStat::DrawUpdate(double participation, double cost_constr_partic, int num_turns, bool is_snd_player)
{
	DrawUpdate(participation, is_snd_player);
	sum_weighted_num_turns += participation * (double)num_turns;
	cost_constrained_participation += cost_constr_partic;
}

void BalanceStat::UpdateEval()
{
	if (total_participation > 0)
		eval = effective_win_contribution / total_participation;
	if (card_cost >= 0) // card_cost < 0 means we don't care about adjusted participation, thus not recording related stats
	{
		double other_partic = total_participation - cost_constrained_participation;
		double ave_match_len = (double)sum_weighted_num_turns / (double)total_participation;
        	double factor = 30.0; // upper bound (maybe good to set to max turn num, not sure though)
        	double diff = ave_match_len - (double)card_cost;
        	if (diff > ave_match_len / factor)
                	factor = ave_match_len / diff;
        	adjusted_participation = other_partic + cost_constrained_participation * factor;
	}
}

void BalanceStat::AppendStat(const BalanceStat& stat)
{
	num_wins += stat.num_wins;
	num_losses += stat.num_losses;
	total_num += stat.total_num;
	effective_win_contribution += stat.effective_win_contribution;
	draw_contribution += stat.draw_contribution;
	snd_player_eff_win_contrib += stat.snd_player_eff_win_contrib;
	total_participation += stat.total_participation;
	sum_weighted_num_turns += stat.sum_weighted_num_turns;
	cost_constrained_participation += stat.cost_constrained_participation;
	UpdateEval();
}

void UpdateStatEvals(vector<BalanceStat>& stats)
{
	for (auto& stat : stats) // note the & is very important otherwise it won't update the original copies
		stat.UpdateEval();
}

void AppendBalanceStats(std::vector<BalanceStat>& stats, const std::vector<BalanceStat>& new_stats)
{
	for (int i = 0; i < stats.size(); i++)
		stats[i].AppendStat(new_stats[i]);
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

bool StatIndexComparatorByAdjustedParticipation(const vector<BalanceStat>& stats, int i, int j)
{
	return stats[i].adjusted_participation > stats[j].adjusted_participation;
}

void SortStatInIndicesByAdjustedParticipation(const vector<BalanceStat>& stats, vector<int>& indices)
{
	iota(indices.begin(), indices.end(), 0); // fill index with {0,1,2,...}
	sort(indices.begin(), indices.end(), bind(StatIndexComparatorByAdjustedParticipation, stats, placeholders::_1, placeholders::_2));
}

vector<int> SortInIndicesInc(const std::vector<double>& vals)
{
	vector<int> indices(vals.size());
	iota(indices.begin(), indices.end(), 0); // fill index with {0, 1, 2,...}
	sort(indices.begin(), indices.end(), [&vals](int i, int j) { return vals[i] < vals[j]; });
	return indices;
}


/* Match records */

int Deck_Size = 20u;
int Double_Deck_Size = 2u * Deck_Size;
int Quadruple_Deck_Size = 2u * Double_Deck_Size;
int Square_Deck_Size = Deck_Size * Deck_Size;
int Square_Double_Deck_Size = Double_Deck_Size * Double_Deck_Size;

MatchRec::MatchRec()
	: deck_a_indices(), deck_b_indices(), weight(0.0), win_weight(0.0), win_rate(0.5),
	sum_win_contrib_a(), ave_win_contrib_a(),
	sum_total_partic_a(), ave_total_partic_a(),
	sum_win_contrib_b(), ave_win_contrib_b(),
	sum_total_partic_b(), ave_total_partic_b()
{
}

MatchRec::MatchRec(const vector<int>& _deck_a_indices, const vector<int>& _deck_b_indices)
	: deck_a_indices(_deck_a_indices), deck_b_indices(_deck_b_indices), weight(0.0), win_weight(0.0), win_rate(0.5),
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


/* Misc algorithms */

struct RowNode // this struct is only used in this file so don't need to be put in .h file
{
	RowNode(int _index) : parent_row(nullptr), index(_index) {}
	RowNode* parent_row; // the child-to-parent relation for a row-column node pair in the alternating tree is always a matched pair, so the parent column is unique and recorded in other places
	int index;
};

// This is an implementation mainly based on the graph interpretation of Hugarian algorithm. The problem setting is a minimum cost balanced assignement problem.
// It does use matrices as the data structures though, trying to make some connections between graph and the matrix interpretation.
// The main difference from the more common matrix interpretation is that it does maintain a temperary match thoughout the whole process, and certain matched pairs may be cancelled (when adding more matched pairs) during the process.
// The logic behind the graph interpretation makes it easier to argue about certain properties (e.g. it always terminate), and does not contain mysterious/unexplained parts like find the min #covering-lines (rows/columns), as opposed to the common matrix interpretation.
// The main connection between the two interpretation is that the modification of vertex labels in the graph interpretation corresponds to the shift of row/column values; and the edges in the equality graph corresponds to zeros in the matrix. 
// Assuming the input costs is a square matrix.
void HungarianAssignment(const vector<vector<double>>& costs, vector<int>& matched_indices) 
{
	int n_elements = costs.size();
	vector<vector<double>> m = costs;

	// Reduce each row by subtracting each row's minimum from every element in the corresponding row; shifting values in any row not change the relative optimality between any choices.
	// This corresponds to the initial labeling in the graph interpretation, guaranteeing the initial equality graph is non-empty.
	for (int i = 0; i < n_elements; i++)
	{
		double min_val = 1000.0;
		for (int j = 0; j < n_elements; j++)
			if (m[i][j] < min_val)
				min_val = m[i][j];
		for (int j = 0; j < n_elements; j++)
			m[i][j] -= min_val;
	}
	// Reduce each column in similar fashion; shifting values in any column does not change the relative optimality between any choices.
	// This has no counterparts in the common graph interpretation, but as it doesn't affect the result and can help make the initial equality graph larger, it's not bad to do it.
	// It could be viewed as part of the initialization process in the graph interpretation, where we additionally make the labels on certain column node being negative, so that when adding labels of all row nodes, the min is equal to the min edge weight (addition to the equality graph).
	for (int i = 0; i < n_elements; i++)
	{
		double min_val = 1000.0;
		for (int j = 0; j < n_elements; j++)
			if (m[j][i] < min_val)
				min_val = m[j][i];
		for (int j = 0; j < n_elements; j++)
			m[j][i] -= min_val;
	}

	// Generate an initial matching.
	int num_matched_pairs = 0;
	matched_indices.resize(n_elements);
	for (int i = 0; i < n_elements; i++)
		matched_indices[i] = -1;
	vector<int> matched_rows_for_columns(n_elements, -1); // -1 means the column is unmatched (free)
	for (int i = 0; i < n_elements; i++)
		for (int j = 0; j < n_elements; j++)
			if (matched_rows_for_columns[j] < 0 && m[i][j] == 0.0)
			{
				num_matched_pairs++;
				matched_indices[i] = j;
				matched_rows_for_columns[j] = i;
				break;
			}

	// Iteratively augment the matching with augmenting paths, each of which is an alternating path that starts and ends with a free vertex (row/column) respectively.
	while (num_matched_pairs < n_elements)
	{
		vector<bool> is_row_in_tree(n_elements, false);
		vector<bool> is_column_in_tree(n_elements, false);
		vector<RowNode*> row_nodes(n_elements);
		for (int i = 0; i < n_elements; i++)
			row_nodes[i] = new RowNode(i);
		queue<RowNode*> row_node_queue; // used for BFS style exploring
		
		// find an unmatched row as the initial node, i.e. root of the alternating tree
		for (int i = 0; i < n_elements; i++)
			if (matched_indices[i] < 0) // this must exist because otherwise the matching should already ended
			{
				is_row_in_tree[i] = true;
				row_node_queue.push(row_nodes[i]);
				break;
			}

		while (true)
		{
			// get the next node and try expanding the alternating tree from there
			RowNode* curr_node = row_node_queue.front();
			row_node_queue.pop();
			int curr_row_index = curr_node->index;
			int path_end_column_index = -1; // if a complete augmenting path is found this would be set to the index of the ending column node; otherwise it would remain -1
			for (int j = 0; j < n_elements; j++)
				if (!is_column_in_tree[j] && m[curr_row_index][j] == 0.0)
				{
					int matched_row = matched_rows_for_columns[j];

					// the column is unmatched, so a complete augmenting path is found
					if (matched_row < 0)
					{
						path_end_column_index = j;
						break;
					}

					// otherwise just continue expanding
					is_column_in_tree[j] = true;
					is_row_in_tree[matched_row] = true;
					RowNode* new_row_node = row_nodes[matched_row];
					new_row_node->parent_row = curr_node;
					row_node_queue.push(new_row_node);
				}

			// found a complete augmenting path
			if (path_end_column_index >= 0)
			{
				num_matched_pairs++;
				int next_column_index = matched_indices[curr_row_index]; // need to store this first before it is overwritten
				matched_indices[curr_row_index] = path_end_column_index;
				matched_rows_for_columns[path_end_column_index] = curr_row_index;
				curr_node = curr_node->parent_row;
				while (curr_node)
				{
					curr_row_index = curr_node->index;
					path_end_column_index = next_column_index;
					next_column_index = matched_indices[curr_row_index]; // need to store this first before it is overwritten
					matched_indices[curr_row_index] = path_end_column_index;
					matched_rows_for_columns[path_end_column_index] = curr_row_index;
					curr_node = curr_node->parent_row;
				}
				break;
			}
			
			// If suck on extending the alternating tree, then adjust the labels,
			// which corresponds to the step (not an exact counterpart) in the matrix interpretation where it uses the lowest non-covered value to shift the matrix values,
			// i.e. subtract all elements of the matrix by the min of non-covered elements, then add back the same amount to each of the covering lines (to maintain non-negative).
			// This process is still basically row shifts and column shifts, which preserve relative optimalities.
			// The process is tantamount to reduce the non-covered elements by the min of them, then keep the non-intersection covered elements unchanged, but increase the intersection covered elements by that same amount.
			// The latter implementation is probably more numerically accurate (we want floating point zeros to be accurate).
			// The covering rows in the matrix interpretation corresponds to row nodes NOT in the current alternating tree in the graph interpretation, and the covering columns corresponds to column nodes in the current alternating tree.
			// Not that the graph interpretation does not create the exact correspondence as the "min-cover" (fewest lines) as the matrix interpretation often does, as it only starts from one of the non-matched nodes, not all.
			// This still works, because the graph interpretation doesn't rely on the "min-cover" to check for success/termination (as opposed to the matrix interpretation), and this step guarantees to create at least one new step for expanding the alternating tree.
			if (row_node_queue.empty())
			{
				double non_covered_min = 1000.0;
				for (int i = 0; i < n_elements; i++)
					for (int j = 0; j < n_elements; j++)
						if (is_row_in_tree[i] && !is_column_in_tree[j] && m[i][j] < non_covered_min)
							non_covered_min = m[i][j];
				for (int i = 0; i < n_elements; i++)
					for (int j = 0; j < n_elements; j++)
						if (is_row_in_tree[i] && !is_column_in_tree[j]) // non-covered
						{
							m[i][j] -= non_covered_min;
							// add the new candidate row node needs (among ones already visited, but with new extension edges)
							if (m[i][j] == 0.0)
								row_node_queue.push(row_nodes[i]);
						}
						else if (!is_row_in_tree[i] && is_column_in_tree[j]) // intersection covered
						{
							m[i][j] += non_covered_min;
						}
			}
		}

		for (int i = 0; i < n_elements; i++)
			delete row_nodes[i];
	}
}

