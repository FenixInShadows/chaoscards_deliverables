#pragma once

#include <vector>
#include <string>

#include <torch/torch.h>

/* Card structure representation */

struct NodeRep
{
	NodeRep();
	NodeRep(unsigned _choice, const std::vector<double>& _term_info);
	std::string GetPrintVersion() const;
	unsigned choice;
	std::vector<double> term_info;
};

typedef std::vector<NodeRep> CardRep;

NodeRep mkNodeRep(unsigned choice, const std::vector<double>& term_info);
NodeRep mkNodeRep(unsigned choice);

double NormalizeCode(double val, int min_int, int max_int); // normalize to -1.0 ~ 1.0
int DenormalizeCode(double code, int min_int, int max_int); // de-normalize from -1.0 ~ 1.0 to an integer
double DenormalizeCodeReal(double code, int min_int, int max_int); // return floating point result

int SampleDefaultStat(); // Sample 0 ~ 10 with weight concentrated more around 2 ~ 3
int SampleDefaultStatClamped(int min_int, int max_int); // Perform a clamp on top of the version above
int SampleNonLeaderCost(); // Sample 0 ~ 7 with weight concentrated more around 2 ~ 3
int SampleNonLeaderCostClamped(int min_int, int max_int); // Perform a clamp on top of the version above
int SampleRangeLowerConcentratedLinear(double min_val, double max_val);
int SampleRangeLowerConcentratedLinearFromInt(int min_int, int max_int);
int SampleRangeUpperConcentratedLinear(double min_val, double max_val);
int SampleRangeUpperConcentratedLinearFromInt(int min_int, int max_int);
int SampleRangeCenterConcentratedLinear(double min_val, double max_val);
int SampleRangeCenterConcentratedLinearFromInt(int min_int, int max_int);
int SampleRangeLowerConcentratedPolynomial(double min_val, double max_val, double exponent);
int SampleRangeLowerConcentratedPolynomialFromInt(int min_int, int max_int, double exponent);
int SampleRangeUpperConcentratedPolynomial(double min_val, double max_val, double exponent);
int SampleRangeUpperConcentratedPolynomialFromInt(int min_int, int max_int, double exponent);
int SampleRangeCenterConcentratedPolynomial(double min_val, double max_val, double exponent);
int SampleRangeCenterConcentratedPolynomialFromInt(int min_int, int max_int, double exponent);


double DiffCardRep(CardRep& rep_a, CardRep& rep_b); // well, should be const ref but the argument type in subsequent function call is a little nasty to deal with...(didn't get reference to const pointer work), return average difference on each entry

void SkipCardRoot(NodeRep*& rep);
double DiffCardRoot(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count); // return the summed difference on all entries in the subtree
void SkipAttackTimes(NodeRep*& rep);
double DiffAttackTimes(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipMinionType(NodeRep*& rep);
double DiffMinionType(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipAbilities(NodeRep*& rep);
double DiffAbilities(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipDamageAbilities(NodeRep*& rep);
double DiffDamageAbilities(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipAnyAbil(NodeRep*& rep);
double DiffAnyAbil(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipSpecialEffects(NodeRep*& rep);
double DiffSpecialEffects(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipTargetedPlayEff(NodeRep*& rep);
double DiffTargetedPlayEff(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipOtherEffs(NodeRep*& rep);
double DiffOtherEffs(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipOtherEff(NodeRep*& rep);
double DiffOtherEff(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipTargetedEff(NodeRep*& rep);
double DiffTargetedEff(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipUntargetedEff(NodeRep*& rep);
double DiffUntargetedEff(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipTargetCond(NodeRep*& rep);
double DiffTargetCond(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipCharTargetCond(NodeRep*& rep);
double DiffCharTargetCond(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipCharTypeCond(NodeRep*& rep);
double DiffCharTypeCond(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipCardTargetCond(NodeRep*& rep);
double DiffCardTargetCond(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipCardTypeCond(NodeRep*& rep);
double DiffCardTypeCond(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipCardPosCond(NodeRep*& rep);
double DiffCardPosCond(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipAllegianceCond(NodeRep*& rep);
double DiffAllegianceCond(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipAbilCond(NodeRep*& rep);
double DiffAbilCond(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipStatCond(NodeRep*& rep);
double DiffStatCond(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipStatCondVariant(NodeRep*& rep);
double DiffStatCondVariant(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipIndeCond(NodeRep*& rep);
double DiffIndeCond(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipBaseTargetedEff(NodeRep*& rep);
double DiffBaseTargetedEff(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipBaseUntargetedEff(NodeRep*& rep);
double DiffBaseUntargetedEff(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipDestination(NodeRep*& rep);
double DiffDestination(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);
void SkipNewCardVariant(NodeRep*& rep);
double DiffNewCardVariant(NodeRep*& rep_a, NodeRep*& rep_b, int& entry_count);

/* Card generation info representation*/

class CardEncoderNet;

struct ExtraCardGenConfig
{
	ExtraCardGenConfig(const std::string& _name, NodeRep*& _rep);
	ExtraCardGenConfig(const std::string& _name, NodeRep*& _filler_rep, int _min_cost, int _max_cost, bool _forcing_minion = false);
	ExtraCardGenConfig(const std::string& _name, NodeRep*& _filler_rep, CardEncoderNet* _encoder, const torch::Tensor& _code, int _min_cost = 0, int _max_cost = 10, bool _forcing_minion = false); // rep has to appear here as it is a reference which needs to be initialized (will be initialized with the refererence to a nullptr variable)
	std::string name;
	NodeRep*& rep;
	CardEncoderNet* encoder;
	torch::Tensor code;
	int min_cost;
	int max_cost;
	bool forcing_minion;
};

ExtraCardGenConfig* MkExtraCardGenConfigWithRep(const std::string& name, NodeRep*& rep);
ExtraCardGenConfig* MkExtraCardGenConfigWithCost(const std::string& name, NodeRep*& filler_rep, int cost); // note, this version does not use the encoder; filler_rep should be the reference to a variable that lives until the use of this config is done
ExtraCardGenConfig* MkExtraCardGenConfigWithLatentCode(const std::string& name, NodeRep*& filler_rep, CardEncoderNet* encoder, const torch::Tensor& code); // filler_rep should be the reference to a variable that lives until the use of this config is done
ExtraCardGenConfig* MkExtraCardGenConfigWithLatentCode(const std::string& name, NodeRep*& filler_rep, CardEncoderNet* encoder, const torch::Tensor& code, int cost, bool forcing_minion = false); // filler_rep should be the reference to a variable that lives until the use of this config is done


/* Card/Deck Performance */

struct BalanceStat // can be for a deck or a card
{
	BalanceStat();
	void SetCardCost(int cost);
	void ResetStat();
	void WinUpdate(double participation, bool is_snd_player);
	void WinUpdate(double participation, double cost_constr_partic, int num_turns, bool is_snd_player);
	void LoseUpdate(double participation, bool is_snd_player);
	void LoseUpdate(double participation, double cost_constr_partic, int num_turns, bool is_snd_player);
	void DrawUpdate(double participation, bool is_snd_player);
	void DrawUpdate(double participation, double cost_constr_partic, int num_turns, bool is_snd_player);
	void UpdateEval(); // update eval, and update adjusted participation as well (if related info is recorded)
	void AppendStat(const BalanceStat& stat);
	int num_wins;
	int num_losses;
	int total_num;
	double effective_win_contribution;
	double draw_contribution;
	double snd_player_eff_win_contrib;
	double total_participation;
	double eval; // derived stat: effective_win_contribution/total_participation
	int card_cost;
	double sum_weighted_num_turns; // total number of turns on the match side it participates, weighted by its participation in the corresponding match side
	double cost_constrained_participation; // participation mostly relying the card being played
	double adjusted_participation; // derived stat: participation with cost constrainted part adjusted, used for deciding which card is more desired for balancing purpose
};

void UpdateStatEvals(std::vector<BalanceStat>& stats);
void AppendBalanceStats(std::vector<BalanceStat>& stats, const std::vector<BalanceStat>& new_stats); // assumes the two vectors has the same size
void SortStatInIndices(const std::vector<BalanceStat>& stats, std::vector<int>& indices); // decreasing with eval, note, the vector indices is required to be allocated to the right size already
void SortStatInIndicesByAdjustedParticipation(const std::vector<BalanceStat>& stats, std::vector<int>& indices); // decreasing with participation, note, the vector indices is requried to be allocated to the right size already
std::vector<int> SortInIndicesInc(const std::vector<double>& vals);

/* Match records */

extern int Deck_Size;
extern int Double_Deck_Size;
extern int Quadruple_Deck_Size;
extern int Square_Deck_Size;
extern int Square_Double_Deck_Size;

struct MatchRec // records for matches between two specific decks
{
	MatchRec();
	MatchRec(const std::vector<int>& _deck_a_indices, const std::vector<int>& _deck_b_indices);
	void WinUpdate(const std::vector<double>& contribution_a, const std::vector<double>& contribution_b); // b wins
	void LoseUpdate(const std::vector<double>& contribution_a, const std::vector<double>& contribution_b); // b losses
	void DrawUpdate(const std::vector<double>& contribution_a, const std::vector<double>& contribution_b); // draw
	void UpdateStats();
	std::vector<int> deck_a_indices;
	std::vector<int> deck_b_indices;
	double weight; // number of matches
	double win_weight; // wins for deck b over a (one draw counts as 0.5 wins for b)
	double win_rate; // win rate for deck b over a (one draw counts as 0.5 wins for b)
	std::vector<double> sum_win_contrib_a; // accumlated card win contribution for cards in deck a
	std::vector<double> ave_win_contrib_a; // average over matches
	std::vector<double> sum_total_partic_a; // accumulated card total participation for cards in deck a
	std::vector<double> ave_total_partic_a; // average over matches
	std::vector<double> sum_win_contrib_b;
	std::vector<double> ave_win_contrib_b;
	std::vector<double> sum_total_partic_b;
	std::vector<double> ave_total_partic_b;
};

/* NN/Torch helpers */

extern double Dropout_Rate;

torch::Tensor CreateEmptyTensor();
torch::Tensor CreateZerosTensor(size_t size);
torch::Tensor CreateSingleValTensor(double val);
torch::Tensor CreateSingleLongIntTensor(int val);
torch::Tensor CreateVectorTensor(const std::vector<double>& v);
torch::Tensor CreateVectorTensorFromArray(const double* a, size_t size);
torch::Tensor CreateOnehotTensor(unsigned choice, size_t size);
double GetSingleValFromTensor(const torch::Tensor& code); // assume the tensor has only one element with double type
std::vector<double> ExtractVectorFromTensor(const torch::Tensor& t); // assume 1-D tensor
std::vector<double> ExtractVectorFromTensor(const torch::Tensor& t, size_t start_index, size_t end_index_plus_one);
unsigned GetMaxIndex(const torch::Tensor& code); // assume 1-D tensor
unsigned GetBinaryIndex(const torch::Tensor& code); // assume 1-element tensor

int GetModelSize(const torch::nn::Module& model);
torch::Device* InitModel(torch::nn::Module& model);  // return the pointer to device (the device isn't necessarily used in this implementation but just in case)
void SaveModel(const torch::nn::Module& model, const char* file_path);
torch::Device* LoadModel(torch::nn::Module& model, const char* file_path); // return the pointer to device (the device isn't necessarily used in this implementation but just in case)

#define torch_Tensor torch::Tensor // get around GIGL artifacts
#define torch_cat torch::cat // get around GIGL artifacts

/* Misc algorithms */

void HungarianAssignment(const std::vector<std::vector<double>>& costs, std::vector<int>& matched_indices); // assumes the input costs is a square matrix, i.e. balanced assignment; also it is mininum cost assignment


/* Constants and macros */

extern int Card_Root_Size;
extern int Attack_Times_Size;
extern int Minion_Type_Size;
extern int Ability_Size;
extern int Damage_Ability_Size;
extern int Special_Effects_Size;
extern int Single_Effect_Size;
extern int Alle_Cond_Size;
extern int Targeted_Play_Eff_Size;
extern int Other_Eff_Size;
extern int Effect_Seq_Size;
extern int Base_Targeted_Eff_Size;
extern int Base_Untargeted_Eff_Size;
extern int Cond_Size;
extern int Sub_Cond_Size;
extern int Char_Type_Cond_Size;
extern int Card_Type_Cond_Size;
extern int Card_Pos_Cond_Size;
extern int Abil_Cond_Size;
extern int Stat_Cond_Variant_Size;
extern int Stat_Cond_Size;
extern int Eff_Utility_Size;
extern int Destination_Size;
extern int New_Card_Variant_Size;

extern int Hidden_Total_Partic_Size;
extern int Hidden_Win_Contrib_Size;
extern int Transformer_Unit_Size;
extern int Hidden_Transformer_Size;

#define CHOICE_LEADER_CARD 0u
#define CHOICE_MINION_CARD 1u
#define CHOICE_SPELL_CARD 2u

#define CHOICE_ZERO_ATTACK 0u
#define CHOICE_SINGLE_ATTACK 1u
#define CHOICE_MULTIPLE_ATTACK 2u

#define CHOICE_NO_TARGETED_PLAY_EFF 0u
#define CHOICE_TARGETED_BATTLECRY_EFF 1u
#define CHOICE_TARGETED_CAST_EFF 2u

#define CHOICE_NO_OTHER_EFFS 0u
#define CHOICE_CONS_OTHER_EFFS 1u

#define CHOICE_UNTARGETED_BATTLECRY_EFF 0u
#define CHOICE_UNTARGETED_CAST_EFF 1u
#define CHOICE_DEATHRATTLE_EFF 2u
#define CHOICE_ONDISCARD_EFF 3u
#define CHOICE_TURN_START_EFF 4u
#define CHOICE_TURN_END_EFF 5u

#define CHOICE_NO_COND_EFF 0u
#define CHOICE_INDE_COND_EFF 1u
#define CHOICE_SRC_COND_EFF 2u

#define CHOICE_CHAR_TARGET_COND 0u
#define CHOICE_CARD_TARGET_COND 1u

#define CHOICE_IS_CHARACTER 0u
#define CHOICE_IS_MINION 1u
#define CHOICE_IS_BEAST 2u
#define CHOICE_IS_DRAGON 3u
#define CHOICE_IS_DEMON 4u

#define CHOICE_IS_CARD 0u
#define CHOICE_IS_LEADER_CARD 1u
#define CHOICE_IS_MINION_CARD 2u
#define CHOICE_IS_SPELL_CARD 3u
#define CHOICE_IS_BEAST_CARD 4u
#define CHOICE_IS_DRAGON_CARD 5u
#define CHOICE_IS_DEMON_CARD 6u

#define CHOICE_ANY_ALLE 0u
#define CHOICE_ALLY_ALLE 1u
#define CHOICE_OPPO_ALLE 2u

#define CHOICE_NO_ABIL_COND 0u
#define CHOICE_CHARGE_COND 1u
#define CHOICE_TAUNT_COND 2u
#define CHOICE_STEALTH_COND 3u
#define CHOICE_UNTARGETABLE_COND 4u
#define CHOICE_SHIELD_COND 5u
#define CHOICE_POISONOUS_COND 6u
#define CHOICE_LIFESTEAL_COND 7u

#define CHOICE_NO_STAT_COND 0u
#define CHOICE_STAT_COST_COND 1u
#define CHOICE_STAT_ATK_COND 2u
#define CHOICE_STAT_HP_COND 3u
#define CHOICE_STAT_ATK_TIMES_COND 4u

#define CHOICE_FIELD_EXISTENCE_COND 0u
#define CHOICE_CARD_EXISTENCE_COND 1u
#define CHOICE_LEADER_COND 2u
#define CHOICE_MP_COND 3u
#define CHOICE_MAX_MP_COND 4u

#define CHOICE_DAMAGE_EFF 0u
#define CHOICE_HEAL_EFF 1u
#define CHOICE_RES_ATK_TIMES_EFF 2u
#define CHOICE_COST_MOD_EFF 3u
#define CHOICE_ATK_HP_MOD_EFF 4u
#define CHOICE_ATK_TIMES_MOD_EFF 5u
#define CHOICE_DESTROY_EFF 6u
#define CHOICE_DISCARD_EFF 7u
#define CHOICE_MOVE_EFF 8u
#define CHOICE_COPY_EFF 9u
#define CHOICE_TRANSFORM_EFF 10u
#define CHOICE_CHANGE_TO_BEAST_EFF 11u
#define CHOICE_CHANGE_TO_DRAGON_EFF 12u
#define CHOICE_CHANGE_TO_DEMON_EFF 13u
#define CHOICE_GIVE_CHARGE_EFF 14u
#define CHOICE_GIVE_TAUNT_EFF 15u
#define CHOICE_GIVE_STEALTH_EFF 16u
#define CHOICE_GIVE_UNTARGETABLE_EFF 17u
#define CHOICE_GIVE_SHIELD_EFF 18u
#define CHOICE_GIVE_POISONOUS_EFF 19u
#define CHOICE_GIVE_LIEFSTEAL_EFF 20u
#define CHOICE_RM_ABILITIES_EFF 21u
#define CHOICE_SET_OVERHEAT_EFF 22u
#define CHOICE_DEC_OH_THRESHOLD_EFF 23u
#define CHOICE_RESET_STATE_EFF 24u
#define CHOICE_GIVE_EFFECTS_EFF 25u

#define CHOICE_AOE_EFF 0u
#define CHOICE_RAND_EFF 1u
#define CHOICE_LEADER_EFF 2u
#define CHOICE_SELF_EFF 3u
#define CHOICE_DRAW_CARD_EFF 4u
#define CHOICE_MP_EFF 5u
#define CHOICE_MAX_MP_EFF 6u
#define CHOICE_NEW_EFF 7u
#define CHOICE_WIN_GAME_EFF 8u

#define CHOICE_FIELD_DEST 0u
#define CHOICE_OWN_FIELD_DEST 1u
#define CHOICE_HAND_DEST 2u
#define CHOICE_OWN_HAND_DEST 3u
#define CHOICE_DECK_DEST 4u
#define CHOICE_OWN_DECK_DEST 5u

#define CHOICE_PLAIN_MINION_CARD 0u
#define CHOICE_PLAIN_MINION 1u
#define CHOICE_FIXED_CARD 2u
#define CHOICE_FIXED_MINION 3u
#define CHOICE_RANDOM_CARD 4u
#define CHOICE_RANDOM_MINION 5u
