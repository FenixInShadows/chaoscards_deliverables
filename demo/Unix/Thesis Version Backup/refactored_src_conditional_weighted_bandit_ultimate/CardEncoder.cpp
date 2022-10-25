#include "CardEncoder.h"

#include <GiglDefaultInclude.h>

#include <fstream>
#include <ctime>

using namespace std;

torch::Tensor UnitForward(const torch::Tensor& input, torch::nn::Linear& layer, bool is_training)
{
	torch::Tensor before_dropout = torch::sin(layer->forward(input));
	return torch::dropout(before_dropout, Dropout_Rate, is_training);
}

torch::Tensor UnitForwardWithIntermediate(const torch::Tensor& input, torch::nn::Linear& to_intermediate, torch::nn::Linear& to_final, bool is_training)
{
	return UnitForward(UnitForward(input, to_intermediate, is_training), to_final, is_training);
}

torch::Tensor ForwardClassifyOut(const torch::Tensor& input, torch::nn::Linear& layer)
{
	return layer->forward(input); // no activation function as log_softmax already has non-linearity, and it accepts any real numbers as inputs 
}

torch::Tensor ForwardBinaryClassifyOut(const torch::Tensor& input, torch::nn::Linear& layer)
{
	return torch::sigmoid(layer->forward(input)); // 0 ~ 1
}

torch::Tensor ForwardValueOut(const torch::Tensor& input, torch::nn::Linear& layer)
{
	return torch::tanh(layer->forward(input)); // -1 ~ 1
}

torch::Tensor ClassifyLoss(const torch::Tensor& pred_class_vals, int target_class, int& correct_count, int& classify_count)
{
	classify_count++;
	size_t class_num = pred_class_vals.size(0);
	torch::Tensor class_code = CreateSingleLongIntTensor(target_class);
	correct_count += (int)(pred_class_vals.argmax(0).template item<long>() == target_class);
	return torch::nll_loss(pred_class_vals.log_softmax(0).view({ 1, class_num }), class_code);
}

torch::Tensor BinaryClassifyLoss(const torch::Tensor& pred_class_val, int target_bool, int& correct_count, int& classify_count)
{
	classify_count++;
	torch::Tensor class_code = CreateSingleValTensor(target_bool);
	correct_count += (int)((GetSingleValFromTensor(pred_class_val) < 0.5) ^ target_bool);
	return torch::binary_cross_entropy(pred_class_val, class_code);
}

torch::Tensor ValueLoss(const torch::Tensor& pred_val, double target_val, double& sum_sqr_err, int& value_count)
{
	value_count++;
	torch::Tensor val_code = CreateSingleValTensor(target_val);
	torch::Tensor loss = mse_loss(pred_val, val_code);
	sum_sqr_err += loss.template item<double>();
	return loss;
}

CardEncoderNet::CardEncoderNet() :
	root_to_mu_intermediate(Card_Root_Size, Card_Root_Size + Card_Root_Size),
	root_to_mu_final(Card_Root_Size + Card_Root_Size, Card_Root_Size),
	root_to_log_std_intermediate(Card_Root_Size, Card_Root_Size + Card_Root_Size),
	root_to_log_std_final(Card_Root_Size + Card_Root_Size, Card_Root_Size),
	condition_layer_to_intermediate(1 + Card_Root_Size, 1 + Card_Root_Size + Card_Root_Size),
	condition_layer_to_final(1 + Card_Root_Size + Card_Root_Size, Card_Root_Size),
	card_root_encoder_to_intermediate(3 + 3 + Attack_Times_Size + Minion_Type_Size + Ability_Size + Special_Effects_Size, 3 + 3 + Attack_Times_Size + Minion_Type_Size + Ability_Size + Special_Effects_Size + Card_Root_Size),
	card_root_encoder_to_final(3 + 3 + Attack_Times_Size + Minion_Type_Size + Ability_Size + Special_Effects_Size + Card_Root_Size, Card_Root_Size),
	card_root_decoder_to_intermediate(Card_Root_Size, 3 + 3 + Attack_Times_Size + Minion_Type_Size + Ability_Size + Special_Effects_Size + Card_Root_Size),
	card_root_decoder_classify(3 + 3 + Attack_Times_Size + Minion_Type_Size + Ability_Size + Special_Effects_Size + Card_Root_Size, 3 + Minion_Type_Size),
	card_root_decoder_binary_classify(3 + 3 + Attack_Times_Size + Minion_Type_Size + Ability_Size + Special_Effects_Size + Card_Root_Size, Ability_Size),
	card_root_decoder_value(3 + 3 + Attack_Times_Size + Minion_Type_Size + Ability_Size + Special_Effects_Size + Card_Root_Size, 3 + Attack_Times_Size),
	card_root_decoder_hidden(3 + 3 + Attack_Times_Size + Minion_Type_Size + Ability_Size + Special_Effects_Size + Card_Root_Size, Special_Effects_Size),
	special_effects_encoder_to_intermediate(Targeted_Play_Eff_Size + Effect_Seq_Size, Targeted_Play_Eff_Size + Effect_Seq_Size + Special_Effects_Size),
	special_effects_encoder_to_final(Targeted_Play_Eff_Size + Effect_Seq_Size + Special_Effects_Size, Special_Effects_Size),
	special_effects_decoder_to_intermediate(Special_Effects_Size, Targeted_Play_Eff_Size + Effect_Seq_Size + Special_Effects_Size),
	special_effects_decoder_classify(Targeted_Play_Eff_Size + Effect_Seq_Size + Special_Effects_Size, 3),
	special_effects_decoder_hidden(Targeted_Play_Eff_Size + Effect_Seq_Size + Special_Effects_Size, Single_Effect_Size + Effect_Seq_Size),
	other_effs_encoder_to_intermediate(1 + Other_Eff_Size + Effect_Seq_Size, 1 + Other_Eff_Size + Effect_Seq_Size + Effect_Seq_Size),
	other_effs_encoder_to_final(1 + Other_Eff_Size + Effect_Seq_Size + Effect_Seq_Size, Effect_Seq_Size),
	other_effs_decoder_to_intermediate(Effect_Seq_Size, 1 + Other_Eff_Size + Effect_Seq_Size + Effect_Seq_Size),
	other_effs_decoder_classify(1 + Other_Eff_Size + Effect_Seq_Size + Effect_Seq_Size, 6 + Alle_Cond_Size),
	other_effs_decoder_binary_classify(1 + Other_Eff_Size + Effect_Seq_Size + Effect_Seq_Size, 1),
	other_effs_decoder_hidden(1 + Other_Eff_Size + Effect_Seq_Size + Effect_Seq_Size, Single_Effect_Size + Effect_Seq_Size),
	targeted_eff_encoder_to_intermediate(3 + Base_Targeted_Eff_Size + 2 * Cond_Size, 3 + Base_Targeted_Eff_Size + 2 * Cond_Size + Single_Effect_Size),
	targeted_eff_encoder_to_final(3 + Base_Targeted_Eff_Size + 2 * Cond_Size + Single_Effect_Size, Single_Effect_Size),
	targeted_eff_decoder_to_intermediate(Single_Effect_Size, 3 + Base_Targeted_Eff_Size + 2 * Cond_Size + Single_Effect_Size),
	targeted_eff_decoder_classify(3 + Base_Targeted_Eff_Size + 2 * Cond_Size + Single_Effect_Size, 3),
	targeted_eff_decoder_hidden(3 + Base_Targeted_Eff_Size + 2 * Cond_Size + Single_Effect_Size, Base_Targeted_Eff_Size + 2 * Cond_Size),
	untargeted_eff_encoder_to_intermediate(3 + Base_Untargeted_Eff_Size + Cond_Size, 3 + Base_Untargeted_Eff_Size + Cond_Size + Single_Effect_Size),
	untargeted_eff_encoder_to_final(3 + Base_Untargeted_Eff_Size + Cond_Size + Single_Effect_Size, Single_Effect_Size),
	untargeted_eff_decoder_to_intermediate(Single_Effect_Size, 3 + Base_Untargeted_Eff_Size + Cond_Size + Single_Effect_Size),
	untargeted_eff_decoder_classify(3 + Base_Untargeted_Eff_Size + Cond_Size + Single_Effect_Size, 3),
	untargeted_eff_decoder_hidden(3 + Base_Untargeted_Eff_Size + Cond_Size + Single_Effect_Size, Base_Untargeted_Eff_Size + Cond_Size),
	target_cond_encoder_to_intermediate(2 + Sub_Cond_Size, 2 + Sub_Cond_Size + Cond_Size),
	target_cond_encoder_to_final(2 + Sub_Cond_Size + Cond_Size, Cond_Size),
	target_cond_decoder_to_intermediate(Cond_Size, 2 + Sub_Cond_Size + Cond_Size),
	target_cond_decoder_classify(2 + Sub_Cond_Size + Cond_Size, 2),
	target_cond_decoder_hidden(2 + Sub_Cond_Size + Cond_Size, Sub_Cond_Size),
	char_target_cond_encoder_to_intermediate(Alle_Cond_Size + Char_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size, Alle_Cond_Size + Char_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size),
	char_target_cond_encoder_to_final(Alle_Cond_Size + Char_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size, Sub_Cond_Size),
	char_target_cond_decoder_to_intermediate(Sub_Cond_Size, Alle_Cond_Size + Char_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size),
	char_target_cond_decoder_classify(Alle_Cond_Size + Char_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size, Alle_Cond_Size + Char_Type_Cond_Size + Abil_Cond_Size + 7),
	char_target_cond_decoder_value(Alle_Cond_Size + Char_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size, 1),
	card_target_cond_encoder_to_intermediate(Card_Pos_Cond_Size + Alle_Cond_Size + Card_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size, Card_Pos_Cond_Size + Alle_Cond_Size + Card_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size),
	card_target_cond_encoder_to_final(Card_Pos_Cond_Size + Alle_Cond_Size + Card_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size, Sub_Cond_Size),
	card_target_cond_decoder_to_intermediate(Sub_Cond_Size, Card_Pos_Cond_Size + Alle_Cond_Size + Card_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size),
	card_target_cond_decoder_classify(Card_Pos_Cond_Size + Alle_Cond_Size + Card_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size, Card_Pos_Cond_Size + Alle_Cond_Size + Card_Type_Cond_Size + Abil_Cond_Size + 7),
	card_target_cond_decoder_value(Card_Pos_Cond_Size + Alle_Cond_Size + Card_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size, 1),
	inde_cond_encoder_to_intermediate(5 + Sub_Cond_Size, 5 + Sub_Cond_Size + Cond_Size),
	inde_cond_encoder_to_final(5 + Sub_Cond_Size + Cond_Size, Cond_Size),
	inde_cond_decoder_to_intermediate(Cond_Size, 5 + Sub_Cond_Size + Cond_Size),
	inde_cond_decoder_classify(5 + Sub_Cond_Size + Cond_Size, 5),
	inde_cond_decoder_hidden(5 + Sub_Cond_Size + Cond_Size, Sub_Cond_Size),
	leader_cond_encoder_to_intermediate(Alle_Cond_Size + Abil_Cond_Size + Stat_Cond_Size, Alle_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size),
	leader_cond_encoder_to_final(Alle_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size, Sub_Cond_Size),
	leader_cond_decoder_to_intermediate(Sub_Cond_Size, Alle_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size),
	leader_cond_decoder_classify(Alle_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size, Alle_Cond_Size + Abil_Cond_Size + 7),
	leader_cond_decoder_value(Alle_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size, 1),
	mp_or_max_cond_encoder_to_intermediate(Alle_Cond_Size + Stat_Cond_Variant_Size, Alle_Cond_Size + Stat_Cond_Variant_Size + Sub_Cond_Size),
	mp_or_max_cond_encoder_to_final(Alle_Cond_Size + Stat_Cond_Variant_Size + Sub_Cond_Size, Sub_Cond_Size),
	mp_or_max_cond_decoder_to_intermediate(Sub_Cond_Size, Alle_Cond_Size + Stat_Cond_Variant_Size + Sub_Cond_Size),
	mp_or_max_cond_decoder_classify(Alle_Cond_Size + Stat_Cond_Variant_Size + Sub_Cond_Size, Alle_Cond_Size + 2),
	mp_or_max_cond_decoder_value(Alle_Cond_Size + Stat_Cond_Variant_Size + Sub_Cond_Size, 1),
	base_targeted_eff_encoder_to_intermediate(26 + 3 + Eff_Utility_Size, 26 + 3 + Eff_Utility_Size + Base_Targeted_Eff_Size),
	base_targeted_eff_encoder_to_final(26 + 3 + Eff_Utility_Size + Base_Targeted_Eff_Size, Base_Targeted_Eff_Size),
	base_targeted_eff_decoder_to_intermediate(Base_Targeted_Eff_Size, 26 + 3 + Eff_Utility_Size + Base_Targeted_Eff_Size),
	base_targeted_eff_decoder_classify(26 + 3 + Eff_Utility_Size + Base_Targeted_Eff_Size, 26),
	base_targeted_eff_decoder_value(26 + 3 + Eff_Utility_Size + Base_Targeted_Eff_Size, 3),
	base_targeted_eff_decoder_hidden(26 + 3 + Eff_Utility_Size + Base_Targeted_Eff_Size, Eff_Utility_Size),
	damage_eff_encoder_to_intermediate(Damage_Ability_Size, Damage_Ability_Size + Eff_Utility_Size),
	damage_eff_encoder_to_final(Damage_Ability_Size + Eff_Utility_Size, Eff_Utility_Size),
	damage_eff_decoder_to_intermediate(Eff_Utility_Size, Damage_Ability_Size + Eff_Utility_Size),
	damage_eff_decoder_binary_classify(Damage_Ability_Size + Eff_Utility_Size, Damage_Ability_Size),
	move_or_copy_eff_encoder_to_intermediate(Destination_Size, Destination_Size + Eff_Utility_Size),
	move_or_copy_eff_encoder_to_final(Destination_Size + Eff_Utility_Size, Eff_Utility_Size),
	move_or_copy_eff_decoder_to_intermediate(Eff_Utility_Size, Destination_Size + Eff_Utility_Size),
	move_or_copy_eff_decoder_classify(Destination_Size + Eff_Utility_Size, Destination_Size),
	transform_eff_encoder_to_intermediate(New_Card_Variant_Size, New_Card_Variant_Size + Eff_Utility_Size),
	transform_eff_encoder_to_final(New_Card_Variant_Size + Eff_Utility_Size, Eff_Utility_Size),
	transform_eff_decoder_to_intermediate(Eff_Utility_Size, New_Card_Variant_Size + Eff_Utility_Size),
	transform_eff_decoder_classify(New_Card_Variant_Size + Eff_Utility_Size, 6),
	transform_eff_decoder_value(New_Card_Variant_Size + Eff_Utility_Size, 1),
	transform_eff_decoder_hidden(New_Card_Variant_Size + Eff_Utility_Size, Card_Root_Size),
	give_effects_eff_encoder_to_intermediate(Special_Effects_Size, Special_Effects_Size + Eff_Utility_Size),
	give_effects_eff_encoder_to_final(Special_Effects_Size + Eff_Utility_Size, Eff_Utility_Size),
	give_effects_eff_decoder_to_intermediate(Eff_Utility_Size, Special_Effects_Size + Eff_Utility_Size),
	give_effects_eff_decoder_hidden(Special_Effects_Size + Eff_Utility_Size, Special_Effects_Size),
	base_untargeted_eff_encoder_to_intermediate(9 + 1 + Base_Targeted_Eff_Size + Cond_Size, 9 + 1 + Base_Targeted_Eff_Size + Cond_Size + Base_Untargeted_Eff_Size),
	base_untargeted_eff_encoder_to_final(9 + 1 + Base_Targeted_Eff_Size + Cond_Size + Base_Untargeted_Eff_Size, Base_Untargeted_Eff_Size),
	base_untargeted_eff_decoder_to_intermediate(Base_Untargeted_Eff_Size, 9 + 1 + Base_Targeted_Eff_Size + Cond_Size + Base_Untargeted_Eff_Size),
	base_untargeted_eff_decoder_classify(9 + 1 + Base_Targeted_Eff_Size + Cond_Size + Base_Untargeted_Eff_Size, 9),
	base_untargeted_eff_decoder_value(9 + 1 + Base_Targeted_Eff_Size + Cond_Size + Base_Untargeted_Eff_Size, 1),
	base_untargeted_eff_decoder_hidden(9 + 1 + Base_Targeted_Eff_Size + Cond_Size + Base_Untargeted_Eff_Size, Base_Targeted_Eff_Size + Cond_Size),
	alle_to_cond_encoder_to_intermediate(Alle_Cond_Size, Alle_Cond_Size + Cond_Size),
	alle_to_cond_encoder_to_final(Alle_Cond_Size + Cond_Size, Cond_Size),
	alle_to_cond_decoder_to_intermediate(Cond_Size, Alle_Cond_Size + Cond_Size),
	alle_to_cond_decoder_classify(Alle_Cond_Size + Cond_Size, Alle_Cond_Size),
	new_eff_encoder_to_intermediate(Destination_Size + New_Card_Variant_Size, Destination_Size + New_Card_Variant_Size + Base_Targeted_Eff_Size),
	new_eff_encoder_to_final(Destination_Size + New_Card_Variant_Size + Base_Targeted_Eff_Size, Base_Targeted_Eff_Size),
	new_eff_decoder_to_intermediate(Base_Targeted_Eff_Size, Destination_Size + New_Card_Variant_Size + Base_Targeted_Eff_Size),
	new_eff_decoder_classify(Destination_Size + New_Card_Variant_Size + Base_Targeted_Eff_Size, Destination_Size + 6),
	new_eff_decoder_value(Destination_Size + New_Card_Variant_Size + Base_Targeted_Eff_Size, 1),
	new_eff_decoder_hidden(Destination_Size + New_Card_Variant_Size + Base_Targeted_Eff_Size, Card_Root_Size)
{
	register_params();
}

void CardEncoderNet::reset() // note: it is important to both re-constructing/re-allocating the submodules and re-registering them, otherwise clone would not be clean (reset is called by clone)
{
	// re-construct/re-allocate submodules
	root_to_mu_intermediate = torch::nn::Linear(Card_Root_Size, Card_Root_Size + Card_Root_Size);
	root_to_mu_final = torch::nn::Linear(Card_Root_Size + Card_Root_Size, Card_Root_Size);
	root_to_log_std_intermediate = torch::nn::Linear(Card_Root_Size, Card_Root_Size + Card_Root_Size);
	root_to_log_std_final = torch::nn::Linear(Card_Root_Size + Card_Root_Size, Card_Root_Size);
	condition_layer_to_intermediate = torch::nn::Linear(1 + Card_Root_Size, 1 + Card_Root_Size + Card_Root_Size);
	condition_layer_to_final = torch::nn::Linear(1 + Card_Root_Size + Card_Root_Size, Card_Root_Size);
	card_root_encoder_to_intermediate = torch::nn::Linear(3 + 3 + Attack_Times_Size + Minion_Type_Size + Ability_Size + Special_Effects_Size, 3 + 3 + Attack_Times_Size + Minion_Type_Size + Ability_Size + Special_Effects_Size + Card_Root_Size);
	card_root_encoder_to_final = torch::nn::Linear(3 + 3 + Attack_Times_Size + Minion_Type_Size + Ability_Size + Special_Effects_Size + Card_Root_Size, Card_Root_Size);
	card_root_decoder_to_intermediate = torch::nn::Linear(Card_Root_Size, 3 + 3 + Attack_Times_Size + Minion_Type_Size + Ability_Size + Special_Effects_Size + Card_Root_Size);
	card_root_decoder_classify = torch::nn::Linear(3 + 3 + Attack_Times_Size + Minion_Type_Size + Ability_Size + Special_Effects_Size + Card_Root_Size, 3 + Minion_Type_Size);
	card_root_decoder_binary_classify = torch::nn::Linear(3 + 3 + Attack_Times_Size + Minion_Type_Size + Ability_Size + Special_Effects_Size + Card_Root_Size, Ability_Size);
	card_root_decoder_value = torch::nn::Linear(3 + 3 + Attack_Times_Size + Minion_Type_Size + Ability_Size + Special_Effects_Size + Card_Root_Size, 3 + Attack_Times_Size);
	card_root_decoder_hidden = torch::nn::Linear(3 + 3 + Attack_Times_Size + Minion_Type_Size + Ability_Size + Special_Effects_Size + Card_Root_Size, Special_Effects_Size);
	special_effects_encoder_to_intermediate = torch::nn::Linear(Targeted_Play_Eff_Size + Effect_Seq_Size, Targeted_Play_Eff_Size + Effect_Seq_Size + Special_Effects_Size);
	special_effects_encoder_to_final = torch::nn::Linear(Targeted_Play_Eff_Size + Effect_Seq_Size + Special_Effects_Size, Special_Effects_Size);
	special_effects_decoder_to_intermediate = torch::nn::Linear(Special_Effects_Size, Targeted_Play_Eff_Size + Effect_Seq_Size + Special_Effects_Size);
	special_effects_decoder_classify = torch::nn::Linear(Targeted_Play_Eff_Size + Effect_Seq_Size + Special_Effects_Size, 3);
	special_effects_decoder_hidden = torch::nn::Linear(Targeted_Play_Eff_Size + Effect_Seq_Size + Special_Effects_Size, Single_Effect_Size + Effect_Seq_Size);
	other_effs_encoder_to_intermediate = torch::nn::Linear(1 + Other_Eff_Size + Effect_Seq_Size, 1 + Other_Eff_Size + Effect_Seq_Size + Effect_Seq_Size);
	other_effs_encoder_to_final = torch::nn::Linear(1 + Other_Eff_Size + Effect_Seq_Size + Effect_Seq_Size, Effect_Seq_Size);
	other_effs_decoder_to_intermediate = torch::nn::Linear(Effect_Seq_Size, 1 + Other_Eff_Size + Effect_Seq_Size + Effect_Seq_Size);
	other_effs_decoder_classify = torch::nn::Linear(1 + Other_Eff_Size + Effect_Seq_Size + Effect_Seq_Size, 6 + Alle_Cond_Size);
	other_effs_decoder_binary_classify = torch::nn::Linear(1 + Other_Eff_Size + Effect_Seq_Size + Effect_Seq_Size, 1);
	other_effs_decoder_hidden = torch::nn::Linear(1 + Other_Eff_Size + Effect_Seq_Size + Effect_Seq_Size, Single_Effect_Size + Effect_Seq_Size);
	targeted_eff_encoder_to_intermediate = torch::nn::Linear(3 + Base_Targeted_Eff_Size + 2 * Cond_Size, 3 + Base_Targeted_Eff_Size + 2 * Cond_Size + Single_Effect_Size);
	targeted_eff_encoder_to_final = torch::nn::Linear(3 + Base_Targeted_Eff_Size + 2 * Cond_Size + Single_Effect_Size, Single_Effect_Size);
	targeted_eff_decoder_to_intermediate = torch::nn::Linear(Single_Effect_Size, 3 + Base_Targeted_Eff_Size + 2 * Cond_Size + Single_Effect_Size);
	targeted_eff_decoder_classify = torch::nn::Linear(3 + Base_Targeted_Eff_Size + 2 * Cond_Size + Single_Effect_Size, 3);
	targeted_eff_decoder_hidden = torch::nn::Linear(3 + Base_Targeted_Eff_Size + 2 * Cond_Size + Single_Effect_Size, Base_Targeted_Eff_Size + 2 * Cond_Size);
	untargeted_eff_encoder_to_intermediate = torch::nn::Linear(3 + Base_Untargeted_Eff_Size + Cond_Size, 3 + Base_Untargeted_Eff_Size + Cond_Size + Single_Effect_Size);
	untargeted_eff_encoder_to_final = torch::nn::Linear(3 + Base_Untargeted_Eff_Size + Cond_Size + Single_Effect_Size, Single_Effect_Size);
	untargeted_eff_decoder_to_intermediate = torch::nn::Linear(Single_Effect_Size, 3 + Base_Untargeted_Eff_Size + Cond_Size + Single_Effect_Size);
	untargeted_eff_decoder_classify = torch::nn::Linear(3 + Base_Untargeted_Eff_Size + Cond_Size + Single_Effect_Size, 3);
	untargeted_eff_decoder_hidden = torch::nn::Linear(3 + Base_Untargeted_Eff_Size + Cond_Size + Single_Effect_Size, Base_Untargeted_Eff_Size + Cond_Size);
	target_cond_encoder_to_intermediate = torch::nn::Linear(2 + Sub_Cond_Size, 2 + Sub_Cond_Size + Cond_Size);
	target_cond_encoder_to_final = torch::nn::Linear(2 + Sub_Cond_Size + Cond_Size, Cond_Size);
	target_cond_decoder_to_intermediate = torch::nn::Linear(Cond_Size, 2 + Sub_Cond_Size + Cond_Size);
	target_cond_decoder_classify = torch::nn::Linear(2 + Sub_Cond_Size + Cond_Size, 2);
	target_cond_decoder_hidden = torch::nn::Linear(2 + Sub_Cond_Size + Cond_Size, Sub_Cond_Size);
	char_target_cond_encoder_to_intermediate = torch::nn::Linear(Alle_Cond_Size + Char_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size, Alle_Cond_Size + Char_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size);
	char_target_cond_encoder_to_final = torch::nn::Linear(Alle_Cond_Size + Char_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size, Sub_Cond_Size);
	char_target_cond_decoder_to_intermediate = torch::nn::Linear(Sub_Cond_Size, Alle_Cond_Size + Char_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size);
	char_target_cond_decoder_classify = torch::nn::Linear(Alle_Cond_Size + Char_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size, Alle_Cond_Size + Char_Type_Cond_Size + Abil_Cond_Size + 7);
	char_target_cond_decoder_value = torch::nn::Linear(Alle_Cond_Size + Char_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size, 1);
	card_target_cond_encoder_to_intermediate = torch::nn::Linear(Card_Pos_Cond_Size + Alle_Cond_Size + Card_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size, Card_Pos_Cond_Size + Alle_Cond_Size + Card_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size);
	card_target_cond_encoder_to_final = torch::nn::Linear(Card_Pos_Cond_Size + Alle_Cond_Size + Card_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size, Sub_Cond_Size);
	card_target_cond_decoder_to_intermediate = torch::nn::Linear(Sub_Cond_Size, Card_Pos_Cond_Size + Alle_Cond_Size + Card_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size);
	card_target_cond_decoder_classify = torch::nn::Linear(Card_Pos_Cond_Size + Alle_Cond_Size + Card_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size, Card_Pos_Cond_Size + Alle_Cond_Size + Card_Type_Cond_Size + Abil_Cond_Size + 7);
	card_target_cond_decoder_value = torch::nn::Linear(Card_Pos_Cond_Size + Alle_Cond_Size + Card_Type_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size, 1);
	inde_cond_encoder_to_intermediate = torch::nn::Linear(5 + Sub_Cond_Size, 5 + Sub_Cond_Size + Cond_Size);
	inde_cond_encoder_to_final = torch::nn::Linear(5 + Sub_Cond_Size + Cond_Size, Cond_Size);
	inde_cond_decoder_to_intermediate = torch::nn::Linear(Cond_Size, 5 + Sub_Cond_Size + Cond_Size);
	inde_cond_decoder_classify = torch::nn::Linear(5 + Sub_Cond_Size + Cond_Size, 5);
	inde_cond_decoder_hidden = torch::nn::Linear(5 + Sub_Cond_Size + Cond_Size, Sub_Cond_Size);
	leader_cond_encoder_to_intermediate = torch::nn::Linear(Alle_Cond_Size + Abil_Cond_Size + Stat_Cond_Size, Alle_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size);
	leader_cond_encoder_to_final = torch::nn::Linear(Alle_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size, Sub_Cond_Size);
	leader_cond_decoder_to_intermediate = torch::nn::Linear(Sub_Cond_Size, Alle_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size);
	leader_cond_decoder_classify = torch::nn::Linear(Alle_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size, Alle_Cond_Size + Abil_Cond_Size + 7);
	leader_cond_decoder_value = torch::nn::Linear(Alle_Cond_Size + Abil_Cond_Size + Stat_Cond_Size + Sub_Cond_Size, 1);
	mp_or_max_cond_encoder_to_intermediate = torch::nn::Linear(Alle_Cond_Size + Stat_Cond_Variant_Size, Alle_Cond_Size + Stat_Cond_Variant_Size + Sub_Cond_Size);
	mp_or_max_cond_encoder_to_final = torch::nn::Linear(Alle_Cond_Size + Stat_Cond_Variant_Size + Sub_Cond_Size, Sub_Cond_Size);
	mp_or_max_cond_decoder_to_intermediate = torch::nn::Linear(Sub_Cond_Size, Alle_Cond_Size + Stat_Cond_Variant_Size + Sub_Cond_Size);
	mp_or_max_cond_decoder_classify = torch::nn::Linear(Alle_Cond_Size + Stat_Cond_Variant_Size + Sub_Cond_Size, Alle_Cond_Size + 2);
	mp_or_max_cond_decoder_value = torch::nn::Linear(Alle_Cond_Size + Stat_Cond_Variant_Size + Sub_Cond_Size, 1);
	base_targeted_eff_encoder_to_intermediate = torch::nn::Linear(26 + 3 + Eff_Utility_Size, 26 + 3 + Eff_Utility_Size + Base_Targeted_Eff_Size);
	base_targeted_eff_encoder_to_final = torch::nn::Linear(26 + 3 + Eff_Utility_Size + Base_Targeted_Eff_Size, Base_Targeted_Eff_Size);
	base_targeted_eff_decoder_to_intermediate = torch::nn::Linear(Base_Targeted_Eff_Size, 26 + 3 + Eff_Utility_Size + Base_Targeted_Eff_Size);
	base_targeted_eff_decoder_classify = torch::nn::Linear(26 + 3 + Eff_Utility_Size + Base_Targeted_Eff_Size, 26);
	base_targeted_eff_decoder_value = torch::nn::Linear(26 + 3 + Eff_Utility_Size + Base_Targeted_Eff_Size, 3);
	base_targeted_eff_decoder_hidden = torch::nn::Linear(26 + 3 + Eff_Utility_Size + Base_Targeted_Eff_Size, Eff_Utility_Size);
	damage_eff_encoder_to_intermediate = torch::nn::Linear(Damage_Ability_Size, Damage_Ability_Size + Eff_Utility_Size);
	damage_eff_encoder_to_final = torch::nn::Linear(Damage_Ability_Size + Eff_Utility_Size, Eff_Utility_Size);
	damage_eff_decoder_to_intermediate = torch::nn::Linear(Eff_Utility_Size, Damage_Ability_Size + Eff_Utility_Size);
	damage_eff_decoder_binary_classify = torch::nn::Linear(Damage_Ability_Size + Eff_Utility_Size, Damage_Ability_Size);
	move_or_copy_eff_encoder_to_intermediate = torch::nn::Linear(Destination_Size, Destination_Size + Eff_Utility_Size);
	move_or_copy_eff_encoder_to_final = torch::nn::Linear(Destination_Size + Eff_Utility_Size, Eff_Utility_Size);
	move_or_copy_eff_decoder_to_intermediate = torch::nn::Linear(Eff_Utility_Size, Destination_Size + Eff_Utility_Size);
	move_or_copy_eff_decoder_classify = torch::nn::Linear(Destination_Size + Eff_Utility_Size, Destination_Size);
	transform_eff_encoder_to_intermediate = torch::nn::Linear(New_Card_Variant_Size, New_Card_Variant_Size + Eff_Utility_Size);
	transform_eff_encoder_to_final = torch::nn::Linear(New_Card_Variant_Size + Eff_Utility_Size, Eff_Utility_Size);
	transform_eff_decoder_to_intermediate = torch::nn::Linear(Eff_Utility_Size, New_Card_Variant_Size + Eff_Utility_Size);
	transform_eff_decoder_classify = torch::nn::Linear(New_Card_Variant_Size + Eff_Utility_Size, 6);
	transform_eff_decoder_value = torch::nn::Linear(New_Card_Variant_Size + Eff_Utility_Size, 1);
	transform_eff_decoder_hidden = torch::nn::Linear(New_Card_Variant_Size + Eff_Utility_Size, Card_Root_Size);
	give_effects_eff_encoder_to_intermediate = torch::nn::Linear(Special_Effects_Size, Special_Effects_Size + Eff_Utility_Size);
	give_effects_eff_encoder_to_final = torch::nn::Linear(Special_Effects_Size + Eff_Utility_Size, Eff_Utility_Size);
	give_effects_eff_decoder_to_intermediate = torch::nn::Linear(Eff_Utility_Size, Special_Effects_Size + Eff_Utility_Size);
	give_effects_eff_decoder_hidden = torch::nn::Linear(Special_Effects_Size + Eff_Utility_Size, Special_Effects_Size);
	base_untargeted_eff_encoder_to_intermediate = torch::nn::Linear(9 + 1 + Base_Targeted_Eff_Size + Cond_Size, 9 + 1 + Base_Targeted_Eff_Size + Cond_Size + Base_Untargeted_Eff_Size);
	base_untargeted_eff_encoder_to_final = torch::nn::Linear(9 + 1 + Base_Targeted_Eff_Size + Cond_Size + Base_Untargeted_Eff_Size, Base_Untargeted_Eff_Size);
	base_untargeted_eff_decoder_to_intermediate = torch::nn::Linear(Base_Untargeted_Eff_Size, 9 + 1 + Base_Targeted_Eff_Size + Cond_Size + Base_Untargeted_Eff_Size);
	base_untargeted_eff_decoder_classify = torch::nn::Linear(9 + 1 + Base_Targeted_Eff_Size + Cond_Size + Base_Untargeted_Eff_Size, 9);
	base_untargeted_eff_decoder_value = torch::nn::Linear(9 + 1 + Base_Targeted_Eff_Size + Cond_Size + Base_Untargeted_Eff_Size, 1);
	base_untargeted_eff_decoder_hidden = torch::nn::Linear(9 + 1 + Base_Targeted_Eff_Size + Cond_Size + Base_Untargeted_Eff_Size, Base_Targeted_Eff_Size + Cond_Size);
	alle_to_cond_encoder_to_intermediate = torch::nn::Linear(Alle_Cond_Size, Alle_Cond_Size + Cond_Size);
	alle_to_cond_encoder_to_final = torch::nn::Linear(Alle_Cond_Size + Cond_Size, Cond_Size);
	alle_to_cond_decoder_to_intermediate = torch::nn::Linear(Cond_Size, Alle_Cond_Size + Cond_Size);
	alle_to_cond_decoder_classify = torch::nn::Linear(Alle_Cond_Size + Cond_Size, Alle_Cond_Size);
	new_eff_encoder_to_intermediate = torch::nn::Linear(Destination_Size + New_Card_Variant_Size, Destination_Size + New_Card_Variant_Size + Base_Targeted_Eff_Size);
	new_eff_encoder_to_final = torch::nn::Linear(Destination_Size + New_Card_Variant_Size + Base_Targeted_Eff_Size, Base_Targeted_Eff_Size);
	new_eff_decoder_to_intermediate = torch::nn::Linear(Base_Targeted_Eff_Size, Destination_Size + New_Card_Variant_Size + Base_Targeted_Eff_Size);
	new_eff_decoder_classify = torch::nn::Linear(Destination_Size + New_Card_Variant_Size + Base_Targeted_Eff_Size, Destination_Size + 6);
	new_eff_decoder_value = torch::nn::Linear(Destination_Size + New_Card_Variant_Size + Base_Targeted_Eff_Size, 1);
	new_eff_decoder_hidden = torch::nn::Linear(Destination_Size + New_Card_Variant_Size + Base_Targeted_Eff_Size, Card_Root_Size);
	
	// re-register parameters using the newly constructed/allocated modules
	register_params();
}

void CardEncoderNet::register_params()
{
	register_module("root_to_mu_intermediate", root_to_mu_intermediate);
	register_module("root_to_mu_final", root_to_mu_final);
	register_module("root_to_log_std_intermediate", root_to_log_std_intermediate);
	register_module("root_to_log_std_final", root_to_log_std_final);
	register_module("condition_layer_to_intermediate", condition_layer_to_intermediate);
	register_module("condition_layer_to_final", condition_layer_to_final);
	register_module("card_root_encoder_to_intermediate", card_root_encoder_to_intermediate);
	register_module("card_root_encoder_to_final", card_root_encoder_to_final);
	register_module("card_root_decoder_to_intermediate", card_root_decoder_to_intermediate);
	register_module("card_root_decoder_classify", card_root_decoder_classify);
	register_module("card_root_decoder_binary_classify", card_root_decoder_binary_classify);
	register_module("card_root_decoder_value", card_root_decoder_value);
	register_module("card_root_decoder_hidden", card_root_decoder_hidden);
	register_module("special_effects_encoder_to_intermediate", special_effects_encoder_to_intermediate);
	register_module("special_effects_encoder_to_final", special_effects_encoder_to_final);
	register_module("special_effects_decoder_to_intermediate", special_effects_decoder_to_intermediate);
	register_module("special_effects_decoder_classify", special_effects_decoder_classify);
	register_module("special_effects_decoder_hidden", special_effects_decoder_hidden);
	register_module("other_effs_encoder_to_intermediate", other_effs_encoder_to_intermediate);
	register_module("other_effs_encoder_to_final", other_effs_encoder_to_final);
	register_module("other_effs_decoder_to_intermediate", other_effs_decoder_to_intermediate);
	register_module("other_effs_decoder_classify", other_effs_decoder_classify);
	register_module("other_effs_decoder_binary_classify", other_effs_decoder_binary_classify);
	register_module("other_effs_decoder_hidden", other_effs_decoder_hidden);
	register_module("targeted_eff_encoder_to_intermediate", targeted_eff_encoder_to_intermediate);
	register_module("targeted_eff_encoder_to_final", targeted_eff_encoder_to_final);
	register_module("targeted_eff_decoder_to_intermediate", targeted_eff_decoder_to_intermediate);
	register_module("targeted_eff_decoder_classify", targeted_eff_decoder_classify);
	register_module("targeted_eff_decoder_hidden", targeted_eff_decoder_hidden);
	register_module("untargeted_eff_encoder_to_intermediate", untargeted_eff_encoder_to_intermediate);
	register_module("untargeted_eff_encoder_to_final", untargeted_eff_encoder_to_final);
	register_module("untargeted_eff_decoder_to_intermediate", untargeted_eff_decoder_to_intermediate);
	register_module("untargeted_eff_decoder_classify", untargeted_eff_decoder_classify);
	register_module("untargeted_eff_decoder_hidden", untargeted_eff_decoder_hidden);
	register_module("target_cond_encoder_to_intermediate", target_cond_encoder_to_intermediate);
	register_module("target_cond_encoder_to_final", target_cond_encoder_to_final);
	register_module("target_cond_decoder_to_intermediate", target_cond_decoder_to_intermediate);
	register_module("target_cond_decoder_classify", target_cond_decoder_classify);
	register_module("target_cond_decoder_hidden", target_cond_decoder_hidden);
	register_module("char_target_cond_encoder_to_intermediate", char_target_cond_encoder_to_intermediate);
	register_module("char_target_cond_encoder_to_final", char_target_cond_encoder_to_final);
	register_module("char_target_cond_decoder_to_intermediate", char_target_cond_decoder_to_intermediate);
	register_module("char_target_cond_decoder_classify", char_target_cond_decoder_classify);
	register_module("char_target_cond_decoder_value", char_target_cond_decoder_value);
	register_module("card_target_cond_encoder_to_intermediate", card_target_cond_encoder_to_intermediate);
	register_module("card_target_cond_encoder_to_final", card_target_cond_encoder_to_final);
	register_module("card_target_cond_decoder_to_intermediate", card_target_cond_decoder_to_intermediate);
	register_module("card_target_cond_decoder_classify", card_target_cond_decoder_classify);
	register_module("card_target_cond_decoder_value", card_target_cond_decoder_value);
	register_module("inde_cond_encoder_to_intermediate", inde_cond_encoder_to_intermediate);
	register_module("inde_cond_encoder_to_final", inde_cond_encoder_to_final);
	register_module("inde_cond_decoder_to_intermediate", inde_cond_decoder_to_intermediate);
	register_module("inde_cond_decoder_classify", inde_cond_decoder_classify);
	register_module("inde_cond_decoder_hidden", inde_cond_decoder_hidden);
	register_module("leader_cond_encoder_to_intermediate", leader_cond_encoder_to_intermediate);
	register_module("leader_cond_encoder_to_final", leader_cond_encoder_to_final);
	register_module("leader_cond_decoder_to_intermediate", leader_cond_decoder_to_intermediate);
	register_module("leader_cond_decoder_classify", leader_cond_decoder_classify);
	register_module("leader_cond_decoder_value", leader_cond_decoder_value);
	register_module("mp_or_max_cond_encoder_to_intermediate", mp_or_max_cond_encoder_to_intermediate);
	register_module("mp_or_max_cond_encoder_to_final", mp_or_max_cond_encoder_to_final);
	register_module("mp_or_max_cond_decoder_to_intermediate", mp_or_max_cond_decoder_to_intermediate);
	register_module("mp_or_max_cond_decoder_classify", mp_or_max_cond_decoder_classify);
	register_module("mp_or_max_cond_decoder_value", mp_or_max_cond_decoder_value);
	register_module("base_targeted_eff_encoder_to_intermediate", base_targeted_eff_encoder_to_intermediate);
	register_module("base_targeted_eff_encoder_to_final", base_targeted_eff_encoder_to_final);
	register_module("base_targeted_eff_decoder_to_intermediate", base_targeted_eff_decoder_to_intermediate);
	register_module("base_targeted_eff_decoder_classify", base_targeted_eff_decoder_classify);
	register_module("base_targeted_eff_decoder_value", base_targeted_eff_decoder_value);
	register_module("base_targeted_eff_decoder_hidden", base_targeted_eff_decoder_hidden);
	register_module("damage_eff_encoder_to_intermediate", damage_eff_encoder_to_intermediate);
	register_module("damage_eff_encoder_to_final", damage_eff_encoder_to_final);
	register_module("damage_eff_decoder_to_intermediate", damage_eff_decoder_to_intermediate);
	register_module("damage_eff_decoder_binary_classify", damage_eff_decoder_binary_classify);
	register_module("move_or_copy_eff_encoder_to_intermediate", move_or_copy_eff_encoder_to_intermediate);
	register_module("move_or_copy_eff_encoder_to_final", move_or_copy_eff_encoder_to_final);
	register_module("move_or_copy_eff_decoder_to_intermediate", move_or_copy_eff_decoder_to_intermediate);
	register_module("move_or_copy_eff_decoder_classify", move_or_copy_eff_decoder_classify);
	register_module("transform_eff_encoder_to_intermediate", transform_eff_encoder_to_intermediate);
	register_module("transform_eff_encoder_to_final", transform_eff_encoder_to_final);
	register_module("transform_eff_decoder_to_intermediate", transform_eff_decoder_to_intermediate);
	register_module("transform_eff_decoder_classify", transform_eff_decoder_classify);
	register_module("transform_eff_decoder_value", transform_eff_decoder_value);
	register_module("transform_eff_decoder_hidden", transform_eff_decoder_hidden);
	register_module("give_effects_eff_encoder_to_intermediate", give_effects_eff_encoder_to_intermediate);
	register_module("give_effects_eff_encoder_to_final", give_effects_eff_encoder_to_final);
	register_module("give_effects_eff_decoder_to_intermediate", give_effects_eff_decoder_to_intermediate);
	register_module("give_effects_eff_decoder_hidden", give_effects_eff_decoder_hidden);
	register_module("base_untargeted_eff_encoder_to_intermediate", base_untargeted_eff_encoder_to_intermediate);
	register_module("base_untargeted_eff_encoder_to_final", base_untargeted_eff_encoder_to_final);
	register_module("base_untargeted_eff_decoder_to_intermediate", base_untargeted_eff_decoder_to_intermediate);
	register_module("base_untargeted_eff_decoder_classify", base_untargeted_eff_decoder_classify);
	register_module("base_untargeted_eff_decoder_value", base_untargeted_eff_decoder_value);
	register_module("base_untargeted_eff_decoder_hidden", base_untargeted_eff_decoder_hidden);
	register_module("alle_to_cond_encoder_to_intermediate", alle_to_cond_encoder_to_intermediate);
	register_module("alle_to_cond_encoder_to_final", alle_to_cond_encoder_to_final);
	register_module("alle_to_cond_decoder_to_intermediate", alle_to_cond_decoder_to_intermediate);
	register_module("alle_to_cond_decoder_classify", alle_to_cond_decoder_classify);
	register_module("new_eff_encoder_to_intermediate", new_eff_encoder_to_intermediate);
	register_module("new_eff_encoder_to_final", new_eff_encoder_to_final);
	register_module("new_eff_decoder_to_intermediate", new_eff_decoder_to_intermediate);
	register_module("new_eff_decoder_classify", new_eff_decoder_classify);
	register_module("new_eff_decoder_value", new_eff_decoder_value);
	register_module("new_eff_decoder_hidden", new_eff_decoder_hidden);
}

torch::Tensor CardEncoderNet::forward_mu(const torch::Tensor& code)
{
	torch::Tensor intermediate_code = torch::sin(root_to_mu_intermediate->forward(code)); // no dropout on this layer (not sure if it matters though)
	return torch::sin(root_to_mu_final->forward(intermediate_code)); // no dropout on this layer (not sure if it matters though)
}

torch::Tensor CardEncoderNet::forward_log_std(const torch::Tensor& code)
{
	torch::Tensor intermediate_code = torch::sin(root_to_log_std_intermediate->forward(code)); // no dropout on this layer (not sure if it matters though)
	return torch::sin(root_to_log_std_final->forward(intermediate_code)); // no dropout on this layer (not sure if it matters though)
}

torch::Tensor CardEncoderNet::decode_conditioned_root(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep)
{
	int cost = (rep->choice == CHOICE_LEADER_CARD ? DenormalizeCode(rep->term_info[0], 8, 10) : DenormalizeCode(rep->term_info[0], 0, 7));
	torch::Tensor pre_selection_code = torch::cat({ CreateSingleValTensor(NormalizeCode(cost, 0, 10)), code }); // this uses 0 ~ 10 as normalization range for all, because at this point the network does not know the card type yet
	torch::Tensor root_code = UnitForwardWithIntermediate(pre_selection_code, condition_layer_to_intermediate, condition_layer_to_final, is_training());
	return decode_card_root(root_code, correct_count, classify_count, sum_sqr_err, value_count, rep);
}

torch::Tensor CardEncoderNet::encode_card_root(NodeRep*& rep)
{
	const double* term_info = rep->term_info.data();
	unsigned choice = (rep++)->choice;
	torch::Tensor choice_code = CreateOnehotTensor(choice, 3);
	torch::Tensor cost_code;
	torch::Tensor attack_code;
	torch::Tensor hp_code;
	torch::Tensor attack_times_code;
	torch::Tensor minion_type_code;
	torch::Tensor abilities_code;
	torch::Tensor effects_code;
	switch (choice)
	{
	case CHOICE_LEADER_CARD:
		{
			cost_code = CreateSingleValTensor(term_info[0]);
			attack_code = CreateSingleValTensor(term_info[1]);
			hp_code = CreateSingleValTensor(term_info[2]);
			attack_times_code = encode_attack_times(rep); // note: the pointer will be incremented inside (similar below)
			minion_type_code = CreateZerosTensor(Minion_Type_Size);
			abilities_code = encode_abilities(rep);
			effects_code = encode_special_effects(rep);
		}
		break;
	case CHOICE_MINION_CARD:
		{
			cost_code = CreateSingleValTensor(term_info[0]);
			attack_code = CreateSingleValTensor(term_info[1]);
			hp_code = CreateSingleValTensor(term_info[2]);
			attack_times_code = encode_attack_times(rep);
			minion_type_code = encode_minion_type(rep);
			abilities_code = encode_abilities(rep);
			effects_code = encode_special_effects(rep);
		}
		break;
	case CHOICE_SPELL_CARD:
		{
			cost_code = CreateSingleValTensor(term_info[0]);
			attack_code = CreateZerosTensor(1);
			hp_code = CreateZerosTensor(1);
			attack_times_code = CreateZerosTensor(Attack_Times_Size);
			minion_type_code = CreateZerosTensor(Minion_Type_Size);
			abilities_code = encode_abilities(rep);
			effects_code = encode_special_effects(rep);
		}
		break;
	}
	return UnitForwardWithIntermediate(torch::cat({ choice_code, cost_code, attack_code, hp_code, attack_times_code, minion_type_code, abilities_code, effects_code }), card_root_encoder_to_intermediate, card_root_encoder_to_final, is_training());
}
torch::Tensor CardEncoderNet::decode_card_root(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep)
{
	const double* term_info = rep->term_info.data();
	unsigned choice = rep->choice;

	torch::Tensor intermediate_code = UnitForward(code, card_root_decoder_to_intermediate, is_training());
	torch::Tensor pred_classify = ForwardClassifyOut(intermediate_code, card_root_decoder_classify);
	torch::Tensor pred_binary_classify = ForwardBinaryClassifyOut(intermediate_code, card_root_decoder_binary_classify);
	torch::Tensor pred_value = ForwardValueOut(intermediate_code, card_root_decoder_value);
	torch::Tensor pred_hidden = UnitForward(intermediate_code, card_root_decoder_hidden, is_training());

	torch::Tensor pred_choice = pred_classify.narrow(0, 0, 3);
	torch::Tensor pred_cost = pred_value.narrow(0, 0, 1);
	torch::Tensor pred_attack = pred_value.narrow(0, 1, 1);
	torch::Tensor pred_hp = pred_value.narrow(0, 2, 1);
	torch::Tensor pred_attack_times = pred_value.narrow(0, 3, Attack_Times_Size);
	torch::Tensor pred_minion_type = pred_classify.narrow(0, 3, Minion_Type_Size);
	torch::Tensor pred_abilities = pred_binary_classify;
	torch::Tensor pred_effects = pred_hidden;

	int correctness = 0;
	torch::Tensor choice_loss = ClassifyLoss(pred_choice, choice, correctness, classify_count);
	if (!correctness) // skip the rest of the subtree if the choice is wrong
	{
		SkipCardRoot(rep);
		return choice_loss;
	}
	rep++;
	correct_count++;

	torch::Tensor cost_loss;
	torch::Tensor attack_loss;
	torch::Tensor hp_loss;
	torch::Tensor attack_times_loss;
	torch::Tensor minion_type_loss;
	torch::Tensor abilities_loss;
	torch::Tensor effects_loss;

	switch (choice)
	{
	case CHOICE_LEADER_CARD:
		cost_loss = ValueLoss(pred_cost, term_info[0], sum_sqr_err, value_count); // we still want to keep this even the mana cost is generally conditioned, because for certain types (tokens not dynamically randomized) derived card the cost is not an input
		attack_loss = ValueLoss(pred_attack, term_info[1], sum_sqr_err, value_count);
		hp_loss = ValueLoss(pred_hp, term_info[2], sum_sqr_err, value_count);
		attack_times_loss = decode_attack_times(pred_attack_times, correct_count, classify_count, sum_sqr_err, value_count, rep); // note: the pointer will be incremented inside (similar below)
		minion_type_loss = CreateZerosTensor(1); // ignore losses on parts that does not actually exist (similar below)
		abilities_loss = decode_abilities(pred_abilities, correct_count, classify_count, sum_sqr_err, value_count, rep);
		effects_loss = decode_special_effects(pred_effects, correct_count, classify_count, sum_sqr_err, value_count, rep);
		break;
	case CHOICE_MINION_CARD:
		cost_loss = ValueLoss(pred_cost, term_info[0], sum_sqr_err, value_count); // we still want to keep this even the mana cost is generally conditioned, because for certain types (tokens not dynamically randomized) derived card the cost is not an input
		attack_loss = ValueLoss(pred_attack, term_info[1], sum_sqr_err, value_count);
		hp_loss = ValueLoss(pred_hp, term_info[2], sum_sqr_err, value_count);
		attack_times_loss = decode_attack_times(pred_attack_times, correct_count, classify_count, sum_sqr_err, value_count, rep);
		minion_type_loss = decode_minion_type(pred_minion_type, correct_count, classify_count, sum_sqr_err, value_count, rep);
		abilities_loss = decode_abilities(pred_abilities, correct_count, classify_count, sum_sqr_err, value_count, rep);
		effects_loss = decode_special_effects(pred_effects, correct_count, classify_count, sum_sqr_err, value_count, rep);
		break;
	case CHOICE_SPELL_CARD:
		cost_loss = ValueLoss(pred_cost, term_info[0], sum_sqr_err, value_count); // we still want to keep this even the mana cost is generally conditioned, because for certain types (tokens not dynamically randomized) derived card the cost is not an input
		attack_loss = CreateZerosTensor(1);
		hp_loss = CreateZerosTensor(1);
		attack_times_loss = CreateZerosTensor(1);
		minion_type_loss = CreateZerosTensor(1);
		abilities_loss = decode_abilities(pred_abilities, correct_count, classify_count, sum_sqr_err, value_count, rep);
		effects_loss = decode_special_effects(pred_effects, correct_count, classify_count, sum_sqr_err, value_count, rep);
		break;
	}

	return choice_loss + cost_loss + attack_loss + hp_loss + attack_times_loss + minion_type_loss + abilities_loss + effects_loss;
}

torch::Tensor CardEncoderNet::encode_attack_times(NodeRep*& rep) // no network parameters involved
{
	// don't need the choice code as we use will use the actual attack times as code
	const double* term_info = rep->term_info.data();
	unsigned choice = (rep++)->choice;
	double attack_times_val = 1.0;
	switch (choice)
	{
	case CHOICE_ZERO_ATTACK:
		attack_times_val = NormalizeCode(0, 0, 5);
		break;
	case CHOICE_SINGLE_ATTACK:
		attack_times_val = NormalizeCode(1, 0, 5);
		break;
	case CHOICE_MULTIPLE_ATTACK:
		attack_times_val = term_info[0];
		break;
	}
	return CreateSingleValTensor(attack_times_val);
}
torch::Tensor CardEncoderNet::decode_attack_times(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep) // no network parameters involved
{
	// don't need the choice code as we use will use the actual attack times as code
	const double* term_info = rep->term_info.data();
	unsigned choice = (rep++)->choice;
	double attack_times_val = 1.0;
	switch (choice)
	{
	case CHOICE_ZERO_ATTACK:
		attack_times_val = NormalizeCode(0, 0, 5);
		break;
	case CHOICE_SINGLE_ATTACK:
		attack_times_val = NormalizeCode(1, 0, 5);
		break;
	case CHOICE_MULTIPLE_ATTACK:
		attack_times_val = term_info[0];
		break;
	}
	return ValueLoss(code, attack_times_val, sum_sqr_err, value_count);
}

torch::Tensor CardEncoderNet::encode_minion_type(NodeRep*& rep) // no network parameters involved
{
	return CreateOnehotTensor((rep++)->choice, 3);
}
torch::Tensor CardEncoderNet::decode_minion_type(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep) // no network parameters involved
{
	return ClassifyLoss(code, (rep++)->choice, correct_count, classify_count);
}

torch::Tensor CardEncoderNet::encode_abilities(NodeRep*& rep) // no network parameters involved
{
	torch::Tensor charge_code = encode_any_abil(rep);
	torch::Tensor taunt_code = encode_any_abil(rep);
	torch::Tensor stealth_code = encode_any_abil(rep);
	torch::Tensor untargetable_code = encode_any_abil(rep);
	torch::Tensor shield_code = encode_any_abil(rep);
	torch::Tensor poisonous_code = encode_any_abil(rep);
	torch::Tensor lifesteal_code = encode_any_abil(rep);
	return torch::cat({ charge_code, taunt_code, stealth_code, untargetable_code, shield_code, poisonous_code, lifesteal_code });
}
torch::Tensor CardEncoderNet::decode_abilities(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep)  // no network parameters involved
{
	torch::Tensor charge_loss = decode_any_abil(code.narrow(0, 0, 1), correct_count, classify_count, sum_sqr_err, value_count, rep);
	torch::Tensor taunt_loss = decode_any_abil(code.narrow(0, 1, 1), correct_count, classify_count, sum_sqr_err, value_count, rep);
	torch::Tensor stealth_loss = decode_any_abil(code.narrow(0, 2, 1), correct_count, classify_count, sum_sqr_err, value_count, rep);
	torch::Tensor untargetable_loss = decode_any_abil(code.narrow(0, 3, 1), correct_count, classify_count, sum_sqr_err, value_count, rep);
	torch::Tensor shield_loss = decode_any_abil(code.narrow(0, 4, 1), correct_count, classify_count, sum_sqr_err, value_count, rep);
	torch::Tensor poisonous_loss = decode_any_abil(code.narrow(0, 5, 1), correct_count, classify_count, sum_sqr_err, value_count, rep);
	torch::Tensor lifesteal_loss = decode_any_abil(code.narrow(0, 6, 1), correct_count, classify_count, sum_sqr_err, value_count, rep);
	return charge_loss + taunt_loss + stealth_loss + untargetable_loss + shield_loss + poisonous_loss + lifesteal_loss;
}

torch::Tensor CardEncoderNet::encode_damage_abilities(NodeRep*& rep) // no network parameters involved
{
	torch::Tensor poisonous_code = encode_any_abil(rep);
	torch::Tensor lifesteal_code = encode_any_abil(rep);
	return torch::cat({ poisonous_code, lifesteal_code });
}
torch::Tensor CardEncoderNet::decode_damage_abilities(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep)  // no network parameters involved
{
	torch::Tensor poisonous_loss = decode_any_abil(code.narrow(0, 0, 1), correct_count, classify_count, sum_sqr_err, value_count, rep);
	torch::Tensor lifesteal_loss = decode_any_abil(code.narrow(0, 1, 1), correct_count, classify_count, sum_sqr_err, value_count, rep);
	return poisonous_loss + lifesteal_loss;
}

torch::Tensor CardEncoderNet::encode_any_abil(NodeRep*& rep) // no network parameters involved
{
	return CreateSingleValTensor((rep++)->choice); // as the choice is binary we only need one bit for this
}
torch::Tensor CardEncoderNet::decode_any_abil(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep) // no network parameters involved
{
	return BinaryClassifyLoss(code, (rep++)->choice, correct_count, classify_count); // as the choice is binary we only need one bit for this
}

torch::Tensor CardEncoderNet::encode_special_effects(NodeRep*& rep)
{
	torch::Tensor targeted_play_eff_code = encode_targeted_play_eff(rep);
	torch::Tensor other_effs_code = encode_other_effs(rep);
	return UnitForwardWithIntermediate(torch::cat({ targeted_play_eff_code, other_effs_code }), special_effects_encoder_to_intermediate, special_effects_encoder_to_final, is_training());
}
torch::Tensor CardEncoderNet::decode_special_effects(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep)
{
	torch::Tensor intermediate_code = UnitForward(code, special_effects_decoder_to_intermediate, is_training());
	torch::Tensor pred_classify = ForwardClassifyOut(intermediate_code, special_effects_decoder_classify);
	torch::Tensor pred_hidden = UnitForward(intermediate_code, special_effects_decoder_hidden, is_training());

	torch::Tensor pred_targeted_play_eff = torch::cat({ pred_classify, pred_hidden.narrow(0, 0, Single_Effect_Size) });
	torch::Tensor pred_other_effs = pred_hidden.narrow(0, Single_Effect_Size, Effect_Seq_Size);

	torch::Tensor targeted_play_eff_loss = decode_targeted_play_eff(pred_targeted_play_eff, correct_count, classify_count, sum_sqr_err, value_count, rep);
	torch::Tensor other_effs_loss = decode_other_effs(pred_other_effs, correct_count, classify_count, sum_sqr_err, value_count, rep);
	return targeted_play_eff_loss + other_effs_loss;
}

torch::Tensor CardEncoderNet::encode_targeted_play_eff(NodeRep*& rep) // no network paramters involved
{
	unsigned choice = (rep++)->choice;
	torch::Tensor choice_code = CreateOnehotTensor(choice, 3);
	torch::Tensor effect_code;
	switch (choice)
	{
	case CHOICE_NO_TARGETED_PLAY_EFF:
		effect_code = CreateZerosTensor(Single_Effect_Size);
		break;
	case CHOICE_TARGETED_BATTLECRY_EFF:
	case CHOICE_TARGETED_CAST_EFF:
		effect_code = encode_targeted_eff(rep);
		break;
	}
	return torch::cat({ choice_code, effect_code });
}
torch::Tensor CardEncoderNet::decode_targeted_play_eff(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep) // no network paramters involved
{
	unsigned choice = rep->choice;

	torch::Tensor pred_choice = code.narrow(0, 0, 3);
	torch::Tensor pred_effect = code.narrow(0, 3, Single_Effect_Size);

	int correctness = 0;
	torch::Tensor choice_loss = ClassifyLoss(pred_choice, choice, correctness, classify_count);
	if (!correctness)
	{
		SkipTargetedPlayEff(rep);
		return choice_loss;
	}
	rep++;
	correct_count++;

	torch::Tensor effect_loss;

	switch (choice)
	{
	case CHOICE_NO_TARGETED_PLAY_EFF:
		effect_loss = CreateZerosTensor(1);
		break;
	case CHOICE_TARGETED_BATTLECRY_EFF:
	case CHOICE_TARGETED_CAST_EFF:
		effect_loss = decode_targeted_eff(pred_effect, correct_count, classify_count, sum_sqr_err, value_count, rep);
		break;
	}
	return choice_loss + effect_loss;
}

torch::Tensor CardEncoderNet::encode_other_effs(NodeRep*& rep)
{
	unsigned choice = (rep++)->choice;
	torch::Tensor choice_code = CreateSingleValTensor(choice); // as the choice is binary we only need one bit for this
	torch::Tensor effs_code;
	switch (choice)
	{
	default: // fall through
	case CHOICE_NO_OTHER_EFFS:
		{
			effs_code = CreateZerosTensor(Other_Eff_Size + Effect_Seq_Size);
		}
		break;
	case CHOICE_CONS_OTHER_EFFS:
		{	
			torch::Tensor head_eff_code = encode_other_eff(rep);
			torch::Tensor tail_effs_code = encode_other_effs(rep);
			effs_code = torch::cat({ head_eff_code, tail_effs_code });
		}
		break;
	}
	return UnitForwardWithIntermediate(torch::cat({ choice_code, effs_code }), other_effs_encoder_to_intermediate, other_effs_encoder_to_final, is_training());
}
torch::Tensor CardEncoderNet::decode_other_effs(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep)
{
	unsigned choice = rep->choice;

	torch::Tensor intermediate_code = UnitForward(code, other_effs_decoder_to_intermediate, is_training());
	torch::Tensor pred_classify = ForwardClassifyOut(intermediate_code, other_effs_decoder_classify);
	torch::Tensor pred_binary_classify = ForwardBinaryClassifyOut(intermediate_code, other_effs_decoder_binary_classify);
	torch::Tensor pred_hidden = UnitForward(intermediate_code, other_effs_decoder_hidden, is_training());

	torch::Tensor pred_choice = pred_binary_classify;

	int correctness = 0;
	torch::Tensor choice_loss = BinaryClassifyLoss(pred_choice, choice, correctness, classify_count);
	if (!correctness)
	{
		SkipOtherEffs(rep);
		return choice_loss;
	}
	rep++;
	correct_count++;

	torch::Tensor effects_loss;

	switch (choice)
	{
	default: // fall through
	case CHOICE_NO_OTHER_EFFS:
		{
			effects_loss = CreateZerosTensor(1);
		}
		break;
	case CHOICE_CONS_OTHER_EFFS:
		{
			torch::Tensor pred_head_eff = torch::cat({ pred_classify.narrow(0, 0, 6), pred_hidden.narrow(0, 0, Single_Effect_Size), pred_classify.narrow(0, 6, Alle_Cond_Size) });
			torch::Tensor pred_tail_effs = pred_hidden.narrow(0, Single_Effect_Size, Effect_Seq_Size);

			torch::Tensor head_eff_loss = decode_other_eff(pred_head_eff, correct_count, classify_count, sum_sqr_err, value_count, rep);
			torch::Tensor tail_effs_loss = decode_other_effs(pred_tail_effs, correct_count, classify_count, sum_sqr_err, value_count, rep);
			effects_loss = head_eff_loss + tail_effs_loss;
		}
		break;
	}
	return choice_loss + effects_loss;
}

torch::Tensor CardEncoderNet::encode_other_eff(NodeRep*& rep) // no network parameters involved
{
	unsigned choice = (rep++)->choice;
	torch::Tensor choice_code = CreateOnehotTensor(choice, 6);
	torch::Tensor effect_code = encode_untargeted_eff(rep); // all rules share this part as the next node
	torch::Tensor alle_code;
	switch (choice)
	{
	case CHOICE_UNTARGETED_BATTLECRY_EFF:
	case CHOICE_UNTARGETED_CAST_EFF:
	case CHOICE_DEATHRATTLE_EFF:
	case CHOICE_ONDISCARD_EFF:
		alle_code = CreateZerosTensor(Alle_Cond_Size);
		break;
	case CHOICE_TURN_START_EFF:
	case CHOICE_TURN_END_EFF:
		alle_code = encode_allegiance_cond(rep);
		break;
	}
	return torch::cat({ choice_code, effect_code, alle_code });
}
torch::Tensor CardEncoderNet::decode_other_eff(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep) // no network parameters involved
{
	unsigned choice = rep->choice;

	torch::Tensor pred_choice = code.narrow(0, 0, 6);
	torch::Tensor pred_effect = code.narrow(0, 6, Single_Effect_Size);
	torch::Tensor pred_alle = code.narrow(0, 6 + Single_Effect_Size, Alle_Cond_Size);

	int correctness = 0;
	torch::Tensor choice_loss = ClassifyLoss(pred_choice, choice, correctness, classify_count);
	if (!correctness)
	{
		SkipOtherEff(rep);
		return choice_loss;
	}
	rep++;
	correct_count++;

	torch::Tensor effect_loss = decode_untargeted_eff(pred_effect, correct_count, classify_count, sum_sqr_err, value_count, rep); // all rules share this part as the next node
	torch::Tensor alle_loss;

	switch (choice)
	{
	case CHOICE_UNTARGETED_BATTLECRY_EFF:
	case CHOICE_UNTARGETED_CAST_EFF:
	case CHOICE_DEATHRATTLE_EFF:
	case CHOICE_ONDISCARD_EFF:
		alle_loss = CreateZerosTensor(1);
		break;
	case CHOICE_TURN_START_EFF:
	case CHOICE_TURN_END_EFF:
		alle_loss = decode_allegiance_cond(pred_alle, correct_count, classify_count, sum_sqr_err, value_count, rep);
		break;
	}
	return choice_loss + effect_loss + alle_loss;
}

torch::Tensor CardEncoderNet::encode_targeted_eff(NodeRep*& rep)
{
	unsigned choice = (rep++)->choice;
	torch::Tensor choice_code = CreateOnehotTensor(choice, 3);
	torch::Tensor effect_code = encode_base_targeted_eff(rep); // all rules share this part as the next node
	torch::Tensor constr_code = encode_target_cond(rep); // all rules share this part and at the next node
	torch::Tensor cond_code;
	switch (choice)
	{
	case CHOICE_NO_COND_EFF:
		cond_code = CreateZerosTensor(Cond_Size);
		break;
	case CHOICE_INDE_COND_EFF:
		cond_code = encode_inde_cond(rep);
		break;
	case CHOICE_SRC_COND_EFF:
		cond_code = encode_target_cond(rep);
		break;
	}
	return UnitForwardWithIntermediate(torch::cat({ choice_code, effect_code, constr_code, cond_code }), targeted_eff_encoder_to_intermediate, targeted_eff_encoder_to_final, is_training());
}
torch::Tensor CardEncoderNet::decode_targeted_eff(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep)
{
	unsigned choice = rep->choice;

	torch::Tensor intermediate_code = UnitForward(code, targeted_eff_decoder_to_intermediate, is_training());
	torch::Tensor pred_classify = ForwardClassifyOut(intermediate_code, targeted_eff_decoder_classify);
	torch::Tensor pred_hidden = UnitForward(intermediate_code, targeted_eff_decoder_hidden, is_training());

	torch::Tensor pred_choice = pred_classify;
	torch::Tensor pred_effect = pred_hidden.narrow(0, 0, Base_Targeted_Eff_Size);
	torch::Tensor pred_constr = pred_hidden.narrow(0, Base_Targeted_Eff_Size, Cond_Size);
	torch::Tensor pred_cond = pred_hidden.narrow(0, Base_Targeted_Eff_Size + Cond_Size, Cond_Size);

	int correctness = 0;
	torch::Tensor choice_loss = ClassifyLoss(pred_choice, choice, correctness, classify_count);
	if (!correctness)
	{
		SkipTargetedEff(rep);
		return choice_loss;
	}
	rep++;
	correct_count++;

	torch::Tensor effect_loss = decode_base_targeted_eff(pred_effect, correct_count, classify_count, sum_sqr_err, value_count, rep); // all rules share this part as the next node
	torch::Tensor constr_loss = decode_target_cond(pred_constr, correct_count, classify_count, sum_sqr_err, value_count, rep); // all rules share this part and at the next node
	torch::Tensor cond_loss;

	switch (choice)
	{
	case CHOICE_NO_COND_EFF:
		cond_loss = CreateZerosTensor(1);
		break;
	case CHOICE_INDE_COND_EFF:
		cond_loss = decode_inde_cond(pred_cond, correct_count, classify_count, sum_sqr_err, value_count, rep);
		break;
	case CHOICE_SRC_COND_EFF:
		cond_loss = decode_target_cond(pred_cond, correct_count, classify_count, sum_sqr_err, value_count, rep);
		break;
	}
	return choice_loss + effect_loss + constr_loss + cond_loss;
}

torch::Tensor CardEncoderNet::encode_untargeted_eff(NodeRep*& rep)
{
	unsigned choice = (rep++)->choice;
	torch::Tensor choice_code = CreateOnehotTensor(choice, 3);
	torch::Tensor effect_code = encode_base_untargeted_eff(rep); // all rules share this part as the next node
	torch::Tensor cond_code;
	switch (choice)
	{
	case CHOICE_NO_COND_EFF:
		cond_code = CreateZerosTensor(Cond_Size);
		break;
	case CHOICE_INDE_COND_EFF:
		cond_code = encode_inde_cond(rep);
		break;
	case CHOICE_SRC_COND_EFF:
		cond_code = encode_target_cond(rep);
		break;
	}
	return UnitForwardWithIntermediate(torch::cat({ choice_code, effect_code, cond_code }), untargeted_eff_encoder_to_intermediate, untargeted_eff_encoder_to_final, is_training());
}
torch::Tensor CardEncoderNet::decode_untargeted_eff(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep)
{
	unsigned choice = rep->choice;

	torch::Tensor intermediate_code = UnitForward(code, untargeted_eff_decoder_to_intermediate, is_training());
	torch::Tensor pred_classify = ForwardClassifyOut(intermediate_code, untargeted_eff_decoder_classify);
	torch::Tensor pred_hidden = UnitForward(intermediate_code, untargeted_eff_decoder_hidden, is_training());

	torch::Tensor pred_choice = pred_classify;
	torch::Tensor pred_effect = pred_hidden.narrow(0, 0, Base_Untargeted_Eff_Size);
	torch::Tensor pred_cond = pred_hidden.narrow(0, Base_Untargeted_Eff_Size, Cond_Size);

	int correctness = 0;
	torch::Tensor choice_loss = ClassifyLoss(pred_choice, choice, correctness, classify_count);
	if (!correctness)
	{
		SkipUntargetedEff(rep);
		return choice_loss;
	}
	rep++;
	correct_count++;

	torch::Tensor effect_loss = decode_base_untargeted_eff(pred_effect, correct_count, classify_count, sum_sqr_err, value_count, rep); // all rules share this part as the next node
	torch::Tensor cond_loss;

	switch (choice)
	{
	case CHOICE_NO_COND_EFF:
		cond_loss = CreateZerosTensor(1);
		break;
	case CHOICE_INDE_COND_EFF:
		cond_loss = decode_inde_cond(pred_cond, correct_count, classify_count, sum_sqr_err, value_count, rep);
		break;
	case CHOICE_SRC_COND_EFF:
		cond_loss = decode_target_cond(pred_cond, correct_count, classify_count, sum_sqr_err, value_count, rep);
		break;
	}
	return choice_loss + effect_loss + cond_loss;
}

torch::Tensor CardEncoderNet::encode_target_cond(NodeRep*& rep)
{
	unsigned choice = (rep++)->choice;
	torch::Tensor choice_code = CreateOnehotTensor(choice, 2); // we could use just one bit but just doing it in a more generalizable fashion (e.g. in case we have more types there)
	torch::Tensor cond_code;
	switch (choice)
	{
	case CHOICE_CHAR_TARGET_COND:
		cond_code = encode_char_target_cond(rep);
		break;
	case CHOICE_CARD_TARGET_COND:
		cond_code = encode_card_target_cond(rep);
		break;
	}
	return UnitForwardWithIntermediate(torch::cat({ choice_code, cond_code }), target_cond_encoder_to_intermediate, target_cond_encoder_to_final, is_training()); // needed for keep equal size as inde_cond
}
torch::Tensor CardEncoderNet::decode_target_cond(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep)
{
	unsigned choice = rep->choice;

	torch::Tensor intermediate_code = UnitForward(code, target_cond_decoder_to_intermediate, is_training());
	torch::Tensor pred_choice = ForwardClassifyOut(intermediate_code, target_cond_decoder_classify);
	torch::Tensor pred_cond = UnitForward(intermediate_code, target_cond_decoder_hidden, is_training());

	int correctness = 0;
	torch::Tensor choice_loss = ClassifyLoss(pred_choice, choice, correctness, classify_count); // we could use just one bit but just doing it in a more generalizable fashion (e.g. in case we have more types there)
	if (!correctness)
	{
		SkipTargetCond(rep);
		return choice_loss;
	}
	rep++;
	correct_count++;

	torch::Tensor cond_loss;

	switch (choice)
	{
	case CHOICE_CHAR_TARGET_COND:
		cond_loss = decode_char_target_cond(pred_cond, correct_count, classify_count, sum_sqr_err, value_count, rep);
		break;
	case CHOICE_CARD_TARGET_COND:
		cond_loss = decode_card_target_cond(pred_cond, correct_count, classify_count, sum_sqr_err, value_count, rep);
		break;
	}
	return choice_loss + cond_loss;
}

torch::Tensor CardEncoderNet::encode_char_target_cond(NodeRep*& rep)
{
	torch::Tensor alle_code = encode_allegiance_cond(rep);
	torch::Tensor type_code = encode_char_type_cond(rep);
	torch::Tensor abil_code = encode_abil_cond(rep);
	torch::Tensor stat_code = encode_stat_cond(rep);
	return UnitForwardWithIntermediate(torch::cat({ alle_code, type_code, abil_code, stat_code }), char_target_cond_encoder_to_intermediate, char_target_cond_encoder_to_final, is_training());
}
torch::Tensor CardEncoderNet::decode_char_target_cond(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep)
{
	torch::Tensor intermediate_code = UnitForward(code, char_target_cond_decoder_to_intermediate, is_training());
	torch::Tensor pred_classify = ForwardClassifyOut(intermediate_code, char_target_cond_decoder_classify);
	torch::Tensor pred_value = ForwardValueOut(intermediate_code, char_target_cond_decoder_value);

	torch::Tensor pred_alle = pred_classify.narrow(0, 0, Alle_Cond_Size);
	torch::Tensor pred_type = pred_classify.narrow(0, Alle_Cond_Size, Char_Type_Cond_Size);
	torch::Tensor pred_abil = pred_classify.narrow(0, Alle_Cond_Size + Char_Type_Cond_Size, Abil_Cond_Size);
	torch::Tensor pred_stat = torch::cat({ pred_classify.narrow(0, Alle_Cond_Size + Char_Type_Cond_Size + Abil_Cond_Size, 7), pred_value }); // 7 is from the 5 classes in StatCond and 2 classes in StatCondVariant 

	torch::Tensor alle_loss = decode_allegiance_cond(pred_alle, correct_count, classify_count, sum_sqr_err, value_count, rep);
	torch::Tensor type_loss = decode_char_type_cond(pred_type, correct_count, classify_count, sum_sqr_err, value_count, rep);
	torch::Tensor abil_loss = decode_abil_cond(pred_abil, correct_count, classify_count, sum_sqr_err, value_count, rep);
	torch::Tensor stat_loss = decode_stat_cond(pred_stat, correct_count, classify_count, sum_sqr_err, value_count, rep);
	return alle_loss + type_loss + abil_loss + stat_loss;
}

torch::Tensor CardEncoderNet::encode_char_type_cond(NodeRep*& rep) // no network parameters involved
{
	return CreateOnehotTensor((rep++)->choice, 5);
}
torch::Tensor CardEncoderNet::decode_char_type_cond(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep) // no network parameters involved
{
	return ClassifyLoss(code, (rep++)->choice, correct_count, classify_count);
}

torch::Tensor CardEncoderNet::encode_card_target_cond(NodeRep*& rep)
{
	torch::Tensor pos_code = encode_card_pos_cond(rep);
	torch::Tensor alle_code = encode_allegiance_cond(rep);
	torch::Tensor type_code = encode_card_type_cond(rep);
	torch::Tensor abil_code = encode_abil_cond(rep);
	torch::Tensor stat_code = encode_stat_cond(rep);
	return UnitForwardWithIntermediate(torch::cat({ pos_code, alle_code, type_code, abil_code, stat_code }), card_target_cond_encoder_to_intermediate, card_target_cond_encoder_to_final, is_training());
}
torch::Tensor CardEncoderNet::decode_card_target_cond(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep)
{
	torch::Tensor intermediate_code = UnitForward(code, card_target_cond_decoder_to_intermediate, is_training());
	torch::Tensor pred_classify = ForwardClassifyOut(intermediate_code, card_target_cond_decoder_classify);
	torch::Tensor pred_value = ForwardValueOut(intermediate_code, card_target_cond_decoder_value);

	torch::Tensor pred_pos = pred_classify.narrow(0, 0, Card_Pos_Cond_Size);
	torch::Tensor pred_alle = pred_classify.narrow(0, Card_Pos_Cond_Size, Alle_Cond_Size);
	torch::Tensor pred_type = pred_classify.narrow(0, Card_Pos_Cond_Size + Alle_Cond_Size, Card_Type_Cond_Size);
	torch::Tensor pred_abil = pred_classify.narrow(0, Card_Pos_Cond_Size + Alle_Cond_Size + Card_Type_Cond_Size, Abil_Cond_Size);
	torch::Tensor pred_stat = torch::cat({ pred_classify.narrow(0, Card_Pos_Cond_Size + Alle_Cond_Size + Card_Type_Cond_Size + Abil_Cond_Size, 7), pred_value }); // 7 is from the 5 classes in StatCond and 2 classes in StatCondVariant

	torch::Tensor pos_loss = decode_card_pos_cond(pred_pos, correct_count, classify_count, sum_sqr_err, value_count, rep);
	torch::Tensor alle_loss = decode_allegiance_cond(pred_alle, correct_count, classify_count, sum_sqr_err, value_count, rep);
	torch::Tensor type_loss = decode_card_type_cond(pred_type, correct_count, classify_count, sum_sqr_err, value_count, rep);
	torch::Tensor abil_loss = decode_abil_cond(pred_abil, correct_count, classify_count, sum_sqr_err, value_count, rep);
	torch::Tensor stat_loss = decode_stat_cond(pred_stat, correct_count, classify_count, sum_sqr_err, value_count, rep);
	return pos_loss + alle_loss + type_loss + abil_loss + stat_loss;
}

torch::Tensor CardEncoderNet::encode_card_type_cond(NodeRep*& rep) // no network parameters involved
{
	return CreateOnehotTensor((rep++)->choice, 7);
}
torch::Tensor CardEncoderNet::decode_card_type_cond(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep) // no network parameters involved
{
	return ClassifyLoss(code, (rep++)->choice, correct_count, classify_count);
}

torch::Tensor CardEncoderNet::encode_card_pos_cond(NodeRep*& rep) // no network parameters involved
{
	return CreateOnehotTensor((rep++)->choice, 3);
}
torch::Tensor CardEncoderNet::decode_card_pos_cond(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep) // no network parameters involved
{
	return ClassifyLoss(code, (rep++)->choice, correct_count, classify_count);
}

torch::Tensor CardEncoderNet::encode_allegiance_cond(NodeRep*& rep) // no network parameters involved
{
	return CreateOnehotTensor((rep++)->choice, 3);
}
torch::Tensor CardEncoderNet::decode_allegiance_cond(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep) // no network parameters involved
{
	return ClassifyLoss(code, (rep++)->choice, correct_count, classify_count);
}

torch::Tensor CardEncoderNet::encode_abil_cond(NodeRep*& rep) // no network parameters involved
{
	return CreateOnehotTensor((rep++)->choice, 8);
}
torch::Tensor CardEncoderNet::decode_abil_cond(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep) // no network parameters involved
{
	return ClassifyLoss(code, (rep++)->choice, correct_count, classify_count);
}

torch::Tensor CardEncoderNet::encode_stat_cond(NodeRep*& rep) // no network parameters involved
{
	unsigned choice = (rep++)->choice;
	torch::Tensor choice_code = CreateOnehotTensor(choice, 5);
	torch::Tensor variant_code;
	switch (choice)
	{
	case CHOICE_NO_STAT_COND:
		variant_code = CreateZerosTensor(Stat_Cond_Variant_Size);
		break;
	default:
		variant_code = encode_stat_cond_variant(rep);
		break;
	}
	return torch::cat({ choice_code, variant_code });
}
torch::Tensor CardEncoderNet::decode_stat_cond(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep) // no network parameters involved
{
	unsigned choice = rep->choice;

	torch::Tensor pred_choice = code.narrow(0, 0, 5);
	torch::Tensor pred_variant = code.narrow(0, 5, Stat_Cond_Variant_Size);

	int correctness = 0;
	torch::Tensor choice_loss = ClassifyLoss(pred_choice, choice, correctness, classify_count);
	if (!correctness)
	{
		SkipStatCond(rep);
		return choice_loss;
	}
	rep++;
	correct_count++;

	torch::Tensor variant_loss;

	switch (choice)
	{
	case CHOICE_NO_STAT_COND:
		variant_loss = CreateZerosTensor(1);
		break;
	default:
		variant_loss = decode_stat_cond_variant(pred_variant, correct_count, classify_count, sum_sqr_err, value_count, rep);
		break;
	}
	return choice_loss + variant_loss;
}

torch::Tensor CardEncoderNet::encode_stat_cond_variant(NodeRep*& rep) // no network parameters involved
{
	const double* term_info = rep->term_info.data();
	torch::Tensor choice_code = CreateOnehotTensor((rep++)->choice, 2); // we could use just one bit but just doing it in a more generalizable fashion (e.g. in case we have more types there)
	torch::Tensor val_code = CreateSingleValTensor(term_info[0]);
	return torch::cat({ choice_code, val_code });
}
torch::Tensor CardEncoderNet::decode_stat_cond_variant(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep) // no network parameters involved
{
	const double* term_info = rep->term_info.data();

	int correctness = 0;
	torch::Tensor choice_loss = ClassifyLoss(code.narrow(0, 0, 2), rep->choice, correctness, classify_count); // we could use just one bit but just doing it in a more generalizable fashion (e.g. in case we have more types there)
	if (!correctness)
	{
		SkipStatCondVariant(rep);
		return choice_loss;
	}
	rep++;
	correct_count++;

	torch::Tensor val_loss = ValueLoss(code.narrow(0, 2, 1), term_info[0], sum_sqr_err, value_count);
	return choice_loss + val_loss;
}

torch::Tensor CardEncoderNet::encode_inde_cond(NodeRep*& rep) // involving multiple sets of network parameters
{
	unsigned choice = (rep++)->choice;
	torch::Tensor choice_code = CreateOnehotTensor(choice, 5);
	torch::Tensor cond_code;
	switch (choice)
	{
	case CHOICE_FIELD_EXISTENCE_COND:
		cond_code = encode_char_target_cond(rep);
		break;
	case CHOICE_CARD_EXISTENCE_COND:
		cond_code = encode_card_target_cond(rep);
		break;
	case CHOICE_LEADER_COND:
		{
			torch::Tensor alle_code = encode_allegiance_cond(rep);
			torch::Tensor abil_code = encode_abil_cond(rep);
			torch::Tensor stat_code = encode_stat_cond(rep);
			cond_code = UnitForwardWithIntermediate(torch::cat({ alle_code, abil_code, stat_code }), leader_cond_encoder_to_intermediate, leader_cond_encoder_to_final, is_training());
		}
		break;
	case CHOICE_MP_COND:
	case CHOICE_MAX_MP_COND:
		{
			torch::Tensor alle_code = encode_allegiance_cond(rep);
			torch::Tensor variant_code = encode_stat_cond_variant(rep);
			cond_code = UnitForwardWithIntermediate(torch::cat({ alle_code, variant_code }), mp_or_max_cond_encoder_to_intermediate, mp_or_max_cond_encoder_to_final, is_training());
		}
		break;
	}
	return UnitForwardWithIntermediate(torch::cat({ choice_code, cond_code }), inde_cond_encoder_to_intermediate, inde_cond_encoder_to_final, is_training()); // needed for keep equal size as target_cond
}
torch::Tensor CardEncoderNet::decode_inde_cond(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep) // involving multiple sets of network parameters
{
	unsigned choice = rep->choice;

	torch::Tensor intermediate_code = UnitForward(code, inde_cond_decoder_to_intermediate, is_training());
	torch::Tensor pred_choice = ForwardClassifyOut(intermediate_code, inde_cond_decoder_classify);
	torch::Tensor pred_cond = UnitForward(intermediate_code, inde_cond_decoder_hidden, is_training());

	int correctness = 0;
	torch::Tensor choice_loss = ClassifyLoss(pred_choice, choice, correctness, classify_count);
	if (!correctness)
	{
		SkipIndeCond(rep);
		return choice_loss;
	}
	rep++;
	correct_count++;

	torch::Tensor cond_loss;

	switch (choice)
	{
	case CHOICE_FIELD_EXISTENCE_COND:
		cond_loss = decode_char_target_cond(pred_cond, correct_count, classify_count, sum_sqr_err, value_count, rep);
		break;
	case CHOICE_CARD_EXISTENCE_COND:
		cond_loss = decode_card_target_cond(pred_cond, correct_count, classify_count, sum_sqr_err, value_count, rep);
		break;
	case CHOICE_LEADER_COND:
		{
			torch::Tensor sub_intermediate_code = UnitForward(pred_cond, leader_cond_decoder_to_intermediate, is_training());
			torch::Tensor pred_leader_cond_classify = ForwardClassifyOut(sub_intermediate_code, leader_cond_decoder_classify);
			torch::Tensor pred_leader_cond_value = ForwardValueOut(sub_intermediate_code, leader_cond_decoder_value);

			torch::Tensor pred_alle = pred_leader_cond_classify.narrow(0, 0, Alle_Cond_Size);
			torch::Tensor pred_abil = pred_leader_cond_classify.narrow(0, Alle_Cond_Size, Abil_Cond_Size);
			torch::Tensor pred_stat = torch::cat({ pred_leader_cond_classify.narrow(0, Alle_Cond_Size + Abil_Cond_Size, 7), pred_leader_cond_value });

			torch::Tensor alle_loss = decode_allegiance_cond(pred_alle, correct_count, classify_count, sum_sqr_err, value_count, rep);
			torch::Tensor abil_loss = decode_abil_cond(pred_abil, correct_count, classify_count, sum_sqr_err, value_count, rep);
			torch::Tensor stat_loss = decode_stat_cond(pred_stat, correct_count, classify_count, sum_sqr_err, value_count, rep);
			cond_loss = alle_loss + abil_loss + stat_loss;
		}
		break;
	case CHOICE_MP_COND:
	case CHOICE_MAX_MP_COND:
		{
			torch::Tensor sub_intermediate_code = UnitForward(pred_cond, mp_or_max_cond_decoder_to_intermediate, is_training());
			torch::Tensor pred_mp_or_max_cond_classify = ForwardClassifyOut(sub_intermediate_code, mp_or_max_cond_decoder_classify);
			torch::Tensor pred_mp_or_max_cond_value = ForwardValueOut(sub_intermediate_code, mp_or_max_cond_decoder_value);

			torch::Tensor pred_alle = pred_mp_or_max_cond_classify.narrow(0, 0, Alle_Cond_Size);
			torch::Tensor pred_variant = torch::cat({ pred_mp_or_max_cond_classify.narrow(0, Alle_Cond_Size, 2), pred_mp_or_max_cond_value });

			torch::Tensor alle_loss = decode_allegiance_cond(pred_alle, correct_count, classify_count, sum_sqr_err, value_count, rep);
			torch::Tensor variant_loss = decode_stat_cond_variant(pred_variant, correct_count, classify_count, sum_sqr_err, value_count, rep);
			cond_loss = alle_loss + variant_loss;
		}
		break;
	}
	return choice_loss + cond_loss;
}

torch::Tensor CardEncoderNet::encode_base_targeted_eff(NodeRep*& rep) // involving multiple sets of net work parameters
{
	const double* term_info = rep->term_info.data();
	unsigned choice = (rep++)->choice;
	torch::Tensor choice_code = CreateOnehotTensor(choice, 26);
	torch::Tensor offensive_code;
	torch::Tensor defensive_code;
	torch::Tensor resource_code;
	torch::Tensor utility_code;
	switch (choice)
	{
	case CHOICE_RES_ATK_TIMES_EFF:
	case CHOICE_ATK_TIMES_MOD_EFF:
		offensive_code = CreateSingleValTensor(term_info[0]);
		defensive_code = CreateZerosTensor(1);
		resource_code = CreateZerosTensor(1);
		utility_code = CreateZerosTensor(Eff_Utility_Size);
		break;
	case CHOICE_HEAL_EFF:
		offensive_code = CreateZerosTensor(1);
		defensive_code = CreateSingleValTensor(term_info[0]);
		resource_code = CreateZerosTensor(1);
		utility_code = CreateZerosTensor(Eff_Utility_Size);
		break;
	case CHOICE_ATK_HP_MOD_EFF:
		offensive_code = CreateSingleValTensor(term_info[0]);
		defensive_code = CreateSingleValTensor(term_info[1]);
		resource_code = CreateZerosTensor(1);
		utility_code = CreateZerosTensor(Eff_Utility_Size);
		break;
	case CHOICE_COST_MOD_EFF:
	case CHOICE_DEC_OH_THRESHOLD_EFF:
		offensive_code = CreateZerosTensor(1);
		defensive_code = CreateZerosTensor(1);
		resource_code = CreateSingleValTensor(term_info[0]);
		utility_code = CreateZerosTensor(Eff_Utility_Size);
		break;
	case CHOICE_DAMAGE_EFF:
		offensive_code = CreateSingleValTensor(term_info[0]);
		defensive_code = CreateZerosTensor(1);
		resource_code = CreateZerosTensor(1);
		utility_code = UnitForwardWithIntermediate(encode_damage_abilities(rep), damage_eff_encoder_to_intermediate, damage_eff_encoder_to_final, is_training());
		break;
	case CHOICE_MOVE_EFF:
	case CHOICE_COPY_EFF:
		offensive_code = CreateZerosTensor(1);
		defensive_code = CreateZerosTensor(1);
		resource_code = CreateZerosTensor(1);
		utility_code = UnitForwardWithIntermediate(encode_destination(rep), move_or_copy_eff_encoder_to_intermediate, move_or_copy_eff_encoder_to_final, is_training());
		break;
	case CHOICE_TRANSFORM_EFF:
		offensive_code = CreateZerosTensor(1);
		defensive_code = CreateZerosTensor(1);
		resource_code = CreateZerosTensor(1);
		utility_code = UnitForwardWithIntermediate(encode_new_card_variant(rep), transform_eff_encoder_to_intermediate, transform_eff_encoder_to_final, is_training());
		break;
	case CHOICE_GIVE_EFFECTS_EFF:
		offensive_code = CreateZerosTensor(1);
		defensive_code = CreateZerosTensor(1);
		resource_code = CreateZerosTensor(1);
		utility_code = UnitForwardWithIntermediate(encode_special_effects(rep), give_effects_eff_encoder_to_intermediate, give_effects_eff_encoder_to_final, is_training());
		break;
	default:
		offensive_code = CreateZerosTensor(1);
		defensive_code = CreateZerosTensor(1);
		resource_code = CreateZerosTensor(1);
		utility_code = CreateZerosTensor(Eff_Utility_Size);
		break;
	}
	return UnitForwardWithIntermediate(torch::cat({ choice_code, offensive_code, defensive_code, resource_code, utility_code }), base_targeted_eff_encoder_to_intermediate, base_targeted_eff_encoder_to_final, is_training());
}
torch::Tensor CardEncoderNet::decode_base_targeted_eff(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep) // involving multiple sets of net work parameters
{
	const double* term_info = rep->term_info.data();
	unsigned choice = rep->choice;

	torch::Tensor intermediate_code = UnitForward(code, base_targeted_eff_decoder_to_intermediate, is_training());
	torch::Tensor pred_classify = ForwardClassifyOut(intermediate_code, base_targeted_eff_decoder_classify);
	torch::Tensor pred_value = ForwardValueOut(intermediate_code, base_targeted_eff_decoder_value);
	torch::Tensor pred_hidden = UnitForward(intermediate_code, base_targeted_eff_decoder_hidden, is_training());

	torch::Tensor pred_choice = pred_classify;
	torch::Tensor pred_offensive = pred_value.narrow(0, 0, 1);
	torch::Tensor pred_defensive = pred_value.narrow(0, 1, 1);
	torch::Tensor pred_resource = pred_value.narrow(0, 2, 1);
	torch::Tensor pred_utility = pred_hidden;

	int correctness = 0;
	torch::Tensor choice_loss = ClassifyLoss(pred_choice, choice, correctness, classify_count);
	if (!correctness)
	{
		SkipBaseTargetedEff(rep);
		return choice_loss;
	}
	rep++;
	correct_count++;

	torch::Tensor offensive_loss;
	torch::Tensor defensive_loss;
	torch::Tensor resource_loss;
	torch::Tensor utility_loss;

	switch (choice)
	{
	case CHOICE_RES_ATK_TIMES_EFF:
	case CHOICE_ATK_TIMES_MOD_EFF:
		offensive_loss = ValueLoss(pred_offensive, term_info[0], sum_sqr_err, value_count);
		defensive_loss = CreateZerosTensor(1);
		resource_loss = CreateZerosTensor(1);
		utility_loss = CreateZerosTensor(1);
		break;
	case CHOICE_HEAL_EFF:
		offensive_loss = CreateZerosTensor(1);
		defensive_loss = ValueLoss(pred_defensive, term_info[0], sum_sqr_err, value_count);
		resource_loss = CreateZerosTensor(1);
		utility_loss = CreateZerosTensor(1);
		break;
	case CHOICE_ATK_HP_MOD_EFF:
		offensive_loss = ValueLoss(pred_offensive, term_info[0], sum_sqr_err, value_count);
		defensive_loss = ValueLoss(pred_defensive, term_info[1], sum_sqr_err, value_count);
		resource_loss = CreateZerosTensor(1);
		utility_loss = CreateZerosTensor(1);
		break;
	case CHOICE_COST_MOD_EFF:
	case CHOICE_DEC_OH_THRESHOLD_EFF:
		offensive_loss = CreateZerosTensor(1);
		defensive_loss = CreateZerosTensor(1);
		resource_loss = ValueLoss(pred_resource, term_info[0], sum_sqr_err, value_count);
		utility_loss = CreateZerosTensor(1);
		break;
	case CHOICE_DAMAGE_EFF:
		offensive_loss = ValueLoss(pred_offensive, term_info[0], sum_sqr_err, value_count);
		defensive_loss = CreateZerosTensor(1);
		resource_loss = CreateZerosTensor(1);
		{
			torch::Tensor sub_intermediate_code = UnitForward(pred_utility, damage_eff_decoder_to_intermediate, is_training());
			torch::Tensor pred_damage_abilities = ForwardBinaryClassifyOut(sub_intermediate_code, damage_eff_decoder_binary_classify);
			utility_loss = decode_damage_abilities(pred_damage_abilities, correct_count, classify_count, sum_sqr_err, value_count, rep);
		}
		break;
	case CHOICE_MOVE_EFF:
	case CHOICE_COPY_EFF:
		offensive_loss = CreateZerosTensor(1);
		defensive_loss = CreateZerosTensor(1);
		resource_loss = CreateZerosTensor(1);
		{
			torch::Tensor sub_intermediate_code = UnitForward(pred_utility, move_or_copy_eff_decoder_to_intermediate, is_training());
			torch::Tensor pred_destination = ForwardClassifyOut(sub_intermediate_code, move_or_copy_eff_decoder_classify);
			utility_loss = decode_destination(pred_destination, correct_count, classify_count, sum_sqr_err, value_count, rep);
		}
		break;
	case CHOICE_TRANSFORM_EFF:
		offensive_loss = CreateZerosTensor(1);
		defensive_loss = CreateZerosTensor(1);
		resource_loss = CreateZerosTensor(1);
		{
			torch::Tensor sub_intermediate_code = UnitForward(pred_utility, transform_eff_decoder_to_intermediate, is_training());
			torch::Tensor pred_transform_eff_classify = ForwardClassifyOut(sub_intermediate_code, transform_eff_decoder_classify);
			torch::Tensor pred_transform_eff_value = ForwardValueOut(sub_intermediate_code, transform_eff_decoder_value);
			torch::Tensor pred_transform_eff_hidden = UnitForward(sub_intermediate_code, transform_eff_decoder_hidden, is_training());

			torch::Tensor pred_new_card_variant = torch::cat({ pred_transform_eff_classify, pred_transform_eff_hidden, pred_transform_eff_value });

			utility_loss = decode_new_card_variant(pred_new_card_variant, correct_count, classify_count, sum_sqr_err, value_count, rep);
		}
		break;
	case CHOICE_GIVE_EFFECTS_EFF:
		offensive_loss = CreateZerosTensor(1);
		defensive_loss = CreateZerosTensor(1);
		resource_loss = CreateZerosTensor(1);
		{
			torch::Tensor sub_intermediate_code = UnitForward(pred_utility, give_effects_eff_decoder_to_intermediate, is_training());
			torch::Tensor pred_special_effects = UnitForward(sub_intermediate_code, give_effects_eff_decoder_hidden, is_training());
			utility_loss = decode_special_effects(pred_special_effects, correct_count, classify_count, sum_sqr_err, value_count, rep);
		}
		break;
	default:
		offensive_loss = CreateZerosTensor(1);
		defensive_loss = CreateZerosTensor(1);
		resource_loss = CreateZerosTensor(1);
		utility_loss = CreateZerosTensor(1);
		break;
	}
	return choice_loss + offensive_loss + defensive_loss + resource_loss + utility_loss;
}

torch::Tensor CardEncoderNet::encode_base_untargeted_eff(NodeRep*& rep) // involving multiple sets of network parameters
{
	const double* term_info = rep->term_info.data();
	unsigned choice = (rep++)->choice;
	torch::Tensor choice_code = CreateOnehotTensor(choice, 9);
	torch::Tensor effect_code;
	torch::Tensor cond_code;
	torch::Tensor val_code;
	switch (choice)
	{
	case CHOICE_AOE_EFF:
	case CHOICE_RAND_EFF:
		effect_code = encode_base_targeted_eff(rep);
		cond_code = encode_target_cond(rep);
		val_code = CreateZerosTensor(1);
		break;
	case CHOICE_LEADER_EFF:
		effect_code = encode_base_targeted_eff(rep);
		cond_code = UnitForwardWithIntermediate(encode_allegiance_cond(rep), alle_to_cond_encoder_to_intermediate, alle_to_cond_encoder_to_final, is_training());
		val_code = CreateZerosTensor(1);
		break;
	case CHOICE_SELF_EFF:
		effect_code = encode_base_targeted_eff(rep);
		cond_code = CreateZerosTensor(Cond_Size);
		val_code = CreateZerosTensor(1);
		break;
	case CHOICE_DRAW_CARD_EFF:
	case CHOICE_MP_EFF:
	case CHOICE_MAX_MP_EFF:
		effect_code = CreateZerosTensor(Base_Targeted_Eff_Size);
		cond_code = UnitForwardWithIntermediate(encode_allegiance_cond(rep), alle_to_cond_encoder_to_intermediate, alle_to_cond_encoder_to_final, is_training());
		val_code = CreateSingleValTensor(term_info[0]);
		break;
	case CHOICE_NEW_EFF:
		{
			torch::Tensor dest_code = encode_destination(rep);
			torch::Tensor variant_code = encode_new_card_variant(rep);
			effect_code = UnitForwardWithIntermediate(torch::cat({ dest_code, variant_code }), new_eff_encoder_to_intermediate, new_eff_encoder_to_final, is_training());
		}
		cond_code = CreateZerosTensor(Cond_Size);
		val_code = CreateSingleValTensor(term_info[0]);
		break;
	case CHOICE_WIN_GAME_EFF:
		effect_code = CreateZerosTensor(Base_Targeted_Eff_Size);
		cond_code = UnitForwardWithIntermediate(encode_allegiance_cond(rep), alle_to_cond_encoder_to_intermediate, alle_to_cond_encoder_to_final, is_training());
		val_code = CreateZerosTensor(1);
		break;
	}
	return UnitForwardWithIntermediate(torch::cat({ choice_code, effect_code, cond_code, val_code }), base_untargeted_eff_encoder_to_intermediate, base_untargeted_eff_encoder_to_final, is_training());
}
torch::Tensor CardEncoderNet::decode_base_untargeted_eff(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep) // involving multiple sets of network parameters
{
	const double* term_info = rep->term_info.data();
	unsigned choice = rep->choice;

	torch::Tensor intermediate_code = UnitForward(code, base_untargeted_eff_decoder_to_intermediate, is_training());
	torch::Tensor pred_classify = ForwardClassifyOut(intermediate_code, base_untargeted_eff_decoder_classify);
	torch::Tensor pred_value = ForwardValueOut(intermediate_code, base_untargeted_eff_decoder_value);
	torch::Tensor pred_hidden = UnitForward(intermediate_code, base_untargeted_eff_decoder_hidden, is_training());

	torch::Tensor pred_choice = pred_classify;
	torch::Tensor pred_effect = pred_hidden.narrow(0, 0, Base_Targeted_Eff_Size);
	torch::Tensor pred_cond = pred_hidden.narrow(0, Base_Targeted_Eff_Size, Cond_Size);
	torch::Tensor pred_val = pred_value;

	int correctness = 0;
	torch::Tensor choice_loss = ClassifyLoss(pred_choice, choice, correctness, classify_count);
	if (!correctness)
	{
		SkipBaseUntargetedEff(rep);
		return choice_loss;
	}
	rep++;
	correct_count++;

	torch::Tensor effect_loss;
	torch::Tensor cond_loss;
	torch::Tensor val_loss;

	switch (choice)
	{
	case CHOICE_AOE_EFF:
	case CHOICE_RAND_EFF:
		effect_loss = decode_base_targeted_eff(pred_effect, correct_count, classify_count, sum_sqr_err, value_count, rep);
		cond_loss = decode_target_cond(pred_cond, correct_count, classify_count, sum_sqr_err, value_count, rep);
		val_loss = CreateZerosTensor(1);
		break;
	case CHOICE_LEADER_EFF:
		effect_loss = decode_base_targeted_eff(pred_effect, correct_count, classify_count, sum_sqr_err, value_count, rep);
		{
			torch::Tensor sub_intermediate_code = UnitForward(pred_cond, alle_to_cond_decoder_to_intermediate, is_training());
			torch::Tensor pred_alle_cond = ForwardClassifyOut(sub_intermediate_code, alle_to_cond_decoder_classify);
			cond_loss = decode_allegiance_cond(pred_alle_cond, correct_count, classify_count, sum_sqr_err, value_count, rep);
		}
		val_loss = CreateZerosTensor(1);
		break;
	case CHOICE_SELF_EFF:
		effect_loss = decode_base_targeted_eff(pred_effect, correct_count, classify_count, sum_sqr_err, value_count, rep);
		cond_loss = CreateZerosTensor(1);
		val_loss = CreateZerosTensor(1);
		break;
	case CHOICE_DRAW_CARD_EFF:
	case CHOICE_MP_EFF:
	case CHOICE_MAX_MP_EFF:
		effect_loss = CreateZerosTensor(1);
		{
			torch::Tensor sub_intermediate_code = UnitForward(pred_cond, alle_to_cond_decoder_to_intermediate, is_training());
			torch::Tensor pred_alle_cond = ForwardClassifyOut(sub_intermediate_code, alle_to_cond_decoder_classify);
			cond_loss = decode_allegiance_cond(pred_alle_cond, correct_count, classify_count, sum_sqr_err, value_count, rep);
		}
		val_loss = ValueLoss(pred_val, term_info[0], sum_sqr_err, value_count);
		break;
	case CHOICE_NEW_EFF:
		{
			torch::Tensor sub_intermediate_code = UnitForward(pred_effect, new_eff_decoder_to_intermediate, is_training());
			torch::Tensor pred_new_eff_classify = ForwardClassifyOut(sub_intermediate_code, new_eff_decoder_classify);
			torch::Tensor pred_new_eff_value = ForwardValueOut(sub_intermediate_code, new_eff_decoder_value);
			torch::Tensor pred_new_eff_hidden = UnitForward(sub_intermediate_code, new_eff_decoder_hidden, is_training());

			torch::Tensor pred_dest = pred_new_eff_classify.narrow(0, 0, Destination_Size);
			torch::Tensor pred_variant = torch::cat({ pred_new_eff_classify.narrow(0, Destination_Size, 6), pred_new_eff_hidden, pred_new_eff_value });

			torch::Tensor dest_loss = decode_destination(pred_dest, correct_count, classify_count, sum_sqr_err, value_count, rep);
			torch::Tensor variant_loss = decode_new_card_variant(pred_variant, correct_count, classify_count, sum_sqr_err, value_count, rep);
			effect_loss = dest_loss + variant_loss;
		}
		cond_loss = CreateZerosTensor(1);
		val_loss = ValueLoss(pred_val, term_info[0], sum_sqr_err, value_count);
		break;
	case CHOICE_WIN_GAME_EFF:
		effect_loss = CreateZerosTensor(1);
		{
			torch::Tensor sub_intermediate_code = UnitForward(pred_cond, alle_to_cond_decoder_to_intermediate, is_training());
			torch::Tensor pred_alle_cond = ForwardClassifyOut(sub_intermediate_code, alle_to_cond_decoder_classify);
			cond_loss = decode_allegiance_cond(pred_alle_cond, correct_count, classify_count, sum_sqr_err, value_count, rep);
		}
		val_loss = CreateZerosTensor(1);
		break;
	}
	return choice_loss + effect_loss + cond_loss + val_loss;
}

torch::Tensor CardEncoderNet::encode_destination(NodeRep*& rep) // no network parameters involved
{
	unsigned choice = (rep++)->choice;
	torch::Tensor choice_code = CreateOnehotTensor(choice, 6);
	torch::Tensor alle_code;
	switch (choice)
	{
	case CHOICE_FIELD_DEST:
	case CHOICE_HAND_DEST:
	case CHOICE_DECK_DEST:
		alle_code = encode_allegiance_cond(rep);
		break;
	case CHOICE_OWN_FIELD_DEST:
	case CHOICE_OWN_HAND_DEST:
	case CHOICE_OWN_DECK_DEST:
		alle_code = CreateZerosTensor(Alle_Cond_Size);
		break;
	}
	return torch::cat({ choice_code, alle_code });
}
torch::Tensor CardEncoderNet::decode_destination(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep) // no network parameters involved
{
	unsigned choice = rep->choice;

	torch::Tensor pred_choice = code.narrow(0, 0, 6);
	torch::Tensor pred_alle = code.narrow(0, 6, Alle_Cond_Size);

	int correctness = 0;
	torch::Tensor choice_loss = ClassifyLoss(pred_choice, choice, correctness, classify_count);
	if (!correctness)
	{
		SkipDestination(rep);
		return choice_loss;
	}
	rep++;
	correct_count++;

	torch::Tensor alle_loss;

	switch (choice)
	{
	case CHOICE_FIELD_DEST:
	case CHOICE_HAND_DEST:
	case CHOICE_DECK_DEST:
		alle_loss = decode_allegiance_cond(pred_alle, correct_count, classify_count, sum_sqr_err, value_count, rep);
		break;
	case CHOICE_OWN_FIELD_DEST:
	case CHOICE_OWN_HAND_DEST:
	case CHOICE_OWN_DECK_DEST:
		alle_loss = CreateZerosTensor(1);
		break;
	}
	return choice_loss + alle_loss;
}

torch::Tensor CardEncoderNet::encode_new_card_variant(NodeRep*& rep) // no network parameters involved
{
	const double* term_info = rep->term_info.data();
	unsigned choice = (rep++)->choice;
	torch::Tensor choice_code = CreateOnehotTensor(choice, 6);
	torch::Tensor card_code;
	torch::Tensor cost_code;
	switch (choice)
	{
	case CHOICE_PLAIN_MINION_CARD:
	case CHOICE_PLAIN_MINION:
	case CHOICE_FIXED_CARD:
	case CHOICE_FIXED_MINION:
		card_code = encode_card_root(rep);
		cost_code = CreateZerosTensor(1);
		break;
	case CHOICE_RANDOM_CARD:
	case CHOICE_RANDOM_MINION:
		card_code = CreateZerosTensor(Card_Root_Size);
		cost_code = CreateSingleValTensor(term_info[0]);
		break;
	}
	return torch::cat({ choice_code, card_code, cost_code });
}
torch::Tensor CardEncoderNet::decode_new_card_variant(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep) // no network parameters involved
{
	const double* term_info = rep->term_info.data();
	unsigned choice = rep->choice;

	torch::Tensor pred_choice = code.narrow(0, 0, 6);
	torch::Tensor pred_card = code.narrow(0, 6, Card_Root_Size);
	torch::Tensor pred_cost = code.narrow(0, 6 + Card_Root_Size, 1);

	int correctness = 0;
	torch::Tensor choice_loss = ClassifyLoss(pred_choice, choice, correctness, classify_count);
	if (!correctness)
	{
		SkipNewCardVariant(rep);
		return choice_loss;
	}
	rep++;
	correct_count++;

	torch::Tensor card_loss;
	torch::Tensor cost_loss;

	switch (choice)
	{
	case CHOICE_PLAIN_MINION_CARD:
	case CHOICE_PLAIN_MINION:
	case CHOICE_FIXED_CARD:
	case CHOICE_FIXED_MINION:
		card_loss = decode_card_root(pred_card, correct_count, classify_count, sum_sqr_err, value_count, rep);
		cost_loss = CreateZerosTensor(1);
		break;
	case CHOICE_RANDOM_CARD:
	case CHOICE_RANDOM_MINION:
		card_loss = CreateZerosTensor(1);
		cost_loss = ValueLoss(pred_cost, term_info[0], sum_sqr_err, value_count);
		break;
	}
	return choice_loss + card_loss + cost_loss;
}


// kl_divergence wants to evaluate how far our distribution q is from the target distribution p (choosen as standard normal), by integrate overal all real (tensor) z on q(z)*log(q(z)/p(z))
// the derviation can be seen at: https://leenashekhar.github.io/2019-01-30-KL-Divergence/
// though there are some type in the "Cross Entropy" derivation part,
// it is supposed to be negative integral of q(z)log(p(z))dz, and the RHS of the first line there should replace mu of 0 and sigma of 1 (not the same mu and sigma and the "Entropy" part)
torch::Tensor kl_divergence(const torch::Tensor& mu, const torch::Tensor& log_std)
{
	torch::Tensor std = torch::exp(log_std);
	torch::Tensor kl_tensor = 0.5 * (mu * mu + std * std - 2.0 * log_std - 1.0);
	return torch::sum(kl_tensor, -1).view({ 1 });
}


torch::Tensor EncodeCard(const CardRep& rep, CardEncoderNet& model)
{
	NodeRep* node_rep_ptr = (NodeRep*)(rep.data());
	return model.encode_card_root(node_rep_ptr);
}

torch::Tensor DecodeCard(const torch::Tensor& code, const CardRep& rep, double& class_acc, double& value_mse, CardEncoderNet& model)
{
	NodeRep* node_rep_ptr = (NodeRep*)(rep.data());
	int correct_count = 0;
	int classify_count = 0;
	double sum_sqr_err = 0.0;
	int value_count = 0;
	torch::Tensor loss = model.decode_conditioned_root(code, correct_count, classify_count, sum_sqr_err, value_count, node_rep_ptr);
	
	class_acc = (double)correct_count / (double)classify_count;
	if (value_count < 1e-7)
		value_mse = 1.0; // only used if the card type is wrong
	else
		value_mse = sum_sqr_err / (double)value_count;
	return loss / (double)(classify_count + value_count);
}

torch::Tensor SingleCardAutoEncoderForward(const CardRep& rep, double& kl_val, double& class_acc, double& value_mse, CardEncoderNet& model)
{
	torch::Tensor latent_code = EncodeCard(rep, model);
	torch::Tensor mu = model.forward_mu(latent_code);
	torch::Tensor log_std = model.forward_log_std(latent_code);
	torch::Tensor std = torch::exp(log_std);
	torch::Tensor sample = torch::randn({ Card_Root_Size }, torch::kDouble) * std + mu;
	torch::Tensor kl_loss = kl_divergence(mu, log_std);

	kl_val = GetSingleValFromTensor(kl_loss);

	torch::Tensor recon_loss = DecodeCard(sample, rep, class_acc, value_mse, model);

	return 0.01 * kl_loss + recon_loss;
}

torch::Tensor GetCardMeanCode(const CardRep& rep, CardEncoderNet& model)
{
	torch::Tensor latent_code = EncodeCard(rep, model);
	return model.forward_mu(latent_code);
}

vector<torch::Tensor> GetCardsMeanCodes(const std::vector<CardRep>& reps, CardEncoderNet& model)
{
	vector<torch::Tensor> codes;
	for (const CardRep& rep : reps)
		codes.push_back(GetCardMeanCode(rep, model));
	return codes;
}


const int Encoder_Train_Batch_Size = 100;

double TrainCardEncoder(const vector<CardRep>& inputs, CardEncoderNet& model, torch::optim::Adam& optimizer, double& kl_val, double& class_acc, double& value_mse)
{
	int num_samples = inputs.size();

	// randomly shuffle for each training epoch 
	int* index_list = new int[num_samples];
	for (int i = 0; i < num_samples; i++)
		index_list[i] = i;
	RandShuffle(index_list, num_samples);

	model.train();
	optimizer.zero_grad();
	torch::Tensor sum_loss = CreateZerosTensor(1);
	double loss_val = 0.0;
	double kl_val_sum = 0.0;
	double correct_count = 0.0; // it is double as there are partial correctness inside a card
	double sqr_err_sum = 0.0;
	for (int i = 1; i <= num_samples; i++)
	{
		int index = index_list[i - 1];
		double kl_val, class_acc, value_mse;
		auto loss = SingleCardAutoEncoderForward(inputs[index], kl_val, class_acc, value_mse, model);
		kl_val_sum += kl_val;
		correct_count += class_acc;
		sqr_err_sum += value_mse;
		sum_loss += loss; // accumulating the loss and do the backward after a batch (as opposed to not using sum_loss but do a loss.backward here) is a little more efficient (does not affect the result)
		loss_val += GetSingleValFromTensor(loss);
		if (i % Encoder_Train_Batch_Size == 0)
		{
			sum_loss.backward();
			optimizer.step();
			optimizer.zero_grad();
			sum_loss = CreateZerosTensor(1);
		}
	}
	if (num_samples % Encoder_Train_Batch_Size != 0)
	{
		sum_loss.backward();
		optimizer.step();
	}

	delete[] index_list;

	loss_val /= (double)num_samples;
	kl_val = kl_val_sum / (double)num_samples;
	class_acc = correct_count / (double)num_samples;
	value_mse = sqr_err_sum / (double)num_samples;

	return loss_val;
}

double TrainCardEncoderWeighted(const vector<CardRep>& inputs, const vector<double>& weights, CardEncoderNet& model, torch::optim::Adam& optimizer, double& kl_val, double& class_acc, double& value_mse)
{
	int num_samples = inputs.size();
	double sum_weight = 0.0;
	for (int k = 0; k < num_samples; k++)
		sum_weight += weights[k];
	double weight_factor = num_samples / sum_weight;

	// randomly shuffle for each training epoch 
	int* index_list = new int[num_samples];
	for (int i = 0; i < num_samples; i++)
		index_list[i] = i;
	RandShuffle(index_list, num_samples);

	model.train();
	optimizer.zero_grad();
	torch::Tensor sum_loss = CreateZerosTensor(1);
	double loss_val = 0.0;
	double kl_val_sum = 0.0;
	double correct_count = 0.0; // it is double as there are partial correctness inside a card
	double sqr_err_sum = 0.0;
	for (int i = 1; i <= num_samples; i++)
	{
		int index = index_list[i - 1];
		double relative_weight = weight_factor * weights[index];
		double kl_val, class_acc, value_mse;		
		auto loss = SingleCardAutoEncoderForward(inputs[index], kl_val, class_acc, value_mse, model);
		kl_val_sum += relative_weight * kl_val;
		correct_count += relative_weight * class_acc;
		sqr_err_sum += relative_weight * value_mse;
		sum_loss += relative_weight * loss; // accumulating the loss and do the backward after a batch (as opposed to not using sum_loss but do a loss.backward here) is a little more efficient (does not affect the result)
		loss_val += relative_weight * GetSingleValFromTensor(loss);
		if (i % Encoder_Train_Batch_Size == 0)
		{
			sum_loss.backward();
			optimizer.step();
			optimizer.zero_grad();
			sum_loss = CreateZerosTensor(1);
		}
	}
	if (num_samples % Encoder_Train_Batch_Size != 0)
	{
		sum_loss.backward();
		optimizer.step();
	}

	delete[] index_list;

	loss_val /= (double)num_samples;
	kl_val = kl_val_sum / (double)num_samples;
	class_acc = correct_count / (double)num_samples;
	value_mse = sqr_err_sum / (double)num_samples;

	return loss_val;
}

double ValidateCardEncoder(const vector<CardRep>& inputs, CardEncoderNet& model, double& kl_val, double& class_acc, double& value_mse)
{
	int num_samples = inputs.size();

	torch::NoGradGuard no_grad;
	model.eval();
	double loss_val = 0.0;
	double kl_val_sum = 0.0;
	double correct_count = 0.0; // it is double as there are partial correctness inside a card
	double sqr_err_sum = 0.0;
	for (int index = 0; index < num_samples; index++)
	{
		double kl_val, class_acc, value_mse;
		auto loss = SingleCardAutoEncoderForward(inputs[index], kl_val, class_acc, value_mse, model);
		kl_val_sum += kl_val;
		correct_count += class_acc;
		sqr_err_sum += value_mse;
		loss_val += GetSingleValFromTensor(loss);
	}
	loss_val /= (double)num_samples;
	kl_val = kl_val_sum / (double)num_samples;
	class_acc = correct_count / (double)num_samples;
	value_mse = sqr_err_sum / (double)num_samples;

	return loss_val;
}

double ValidateCardEncoderWeighted(const vector<CardRep>& inputs, const vector<double>& weights, CardEncoderNet& model, double& kl_val, double& class_acc, double& value_mse)
{
	int num_samples = inputs.size();
	double sum_weight = 0.0;
	for (int k = 0; k < num_samples; k++)
		sum_weight += weights[k];
	double weight_factor = num_samples / sum_weight;

	torch::NoGradGuard no_grad;
	model.eval();
	double loss_val = 0.0;
	double kl_val_sum = 0.0;
	double correct_count = 0.0; // it is double as there are partial correctness inside a card
	double sqr_err_sum = 0.0;
	for (int index = 0; index < num_samples; index++)
	{
		double relative_weight = weight_factor * weights[index];
		double kl_val, class_acc, value_mse;
		auto loss = SingleCardAutoEncoderForward(inputs[index], kl_val, class_acc, value_mse, model);
		kl_val_sum += relative_weight * kl_val;
		correct_count += relative_weight * class_acc;
		sqr_err_sum += relative_weight * value_mse;
		loss_val += relative_weight * GetSingleValFromTensor(loss);
	}
	loss_val /= (double)num_samples;
	kl_val = kl_val_sum / (double)num_samples;
	class_acc = correct_count / (double)num_samples;
	value_mse = sqr_err_sum / (double)num_samples;

	return loss_val;
}

double TrainCardEncoderNoValidationSet(const vector<CardRep>& reps, int num_epochs, CardEncoderNet& model, double& kl_val, double& class_acc, double& value_mse)
{
	// Do a validation before training and return the stats (this is supposed to be run repeatedly for card sets that gets updated over iterations)
	double validate_loss_val = ValidateCardEncoder(reps, model, kl_val, class_acc, value_mse);
	cout << "Pre-validate Loss: " << validate_loss_val << "; KL Divergence: " << kl_val << "; Classify Accuracy: " << class_acc << "; Value MSE: " << value_mse << endl;

	// set up the optimizer
	torch::optim::Adam optimizer(model.parameters(), torch::optim::AdamOptions(0.001));

	// Training
	for (int epoch = 1; epoch <= num_epochs; epoch++)
	{
		double tmp_kl_val, tmp_class_acc, tmp_value_mse; // not used
		TrainCardEncoder(reps, model, optimizer, tmp_kl_val, tmp_class_acc, tmp_value_mse);
	}

	return validate_loss_val;
}

double TrainCardEncoderNoValidationSetWeighted(const vector<CardRep>& reps, const vector<double>& weights, int num_epochs, CardEncoderNet& model, double& kl_val, double& class_acc, double& value_mse)
{
	// Do a validation before training and return the stats (this is supposed to be run repeatedly for card sets that gets updated over iterations)
	double validate_loss_val = ValidateCardEncoderWeighted(reps, weights, model, kl_val, class_acc, value_mse);
	cout << "Pre-validate Loss: " << validate_loss_val << "; KL Divergence: " << kl_val << "; Classify Accuracy: " << class_acc << "; Value MSE: " << value_mse << endl;

	// set up the optimizer
	torch::optim::Adam optimizer(model.parameters(), torch::optim::AdamOptions(0.001));

	// Training
	for (int epoch = 1; epoch <= num_epochs; epoch++)
	{
		double tmp_kl_val, tmp_class_acc, tmp_value_mse; // not used
		TrainCardEncoderWeighted(reps, weights, model, optimizer, tmp_kl_val, tmp_class_acc, tmp_value_mse);
	}

	return validate_loss_val;
}

void TrainCardEncoderWithValidationSet(const vector<CardRep>& train_reps, const vector<CardRep>& validate_reps, int num_epochs, CardEncoderNet& model, int seed, const char* model_file_path, const char* log_file_path)
{
	RandInit(seed);
	seed = GetRandInt();
	srand(seed);
	torch::manual_seed(seed);

	// set up the optimizer
	torch::optim::Adam optimizer(model.parameters(), torch::optim::AdamOptions(0.001));
	
	// train/validate
	ofstream log_fs(log_file_path);
	time_t start_timer = time(NULL);
	int epoch;
	for (epoch = 1; epoch <= num_epochs; epoch++)
	{
		cout << endl << "Epoch " << epoch << ":" << endl;
		double kl_val, class_acc, value_mse;
		double train_loss_val = TrainCardEncoder(train_reps, model, optimizer, kl_val, class_acc, value_mse);
		cout << "Train Loss: " << train_loss_val << "; KL Divergence: " << kl_val << "; Classify Accuracy: " << class_acc << "; Value MSE: " << value_mse << endl;
		double validate_loss_val = ValidateCardEncoder(validate_reps, model, kl_val, class_acc, value_mse);
		cout << "Validate Loss: " << validate_loss_val << "; KL Divergence: " << kl_val << "; Classify Accuracy: " << class_acc << "; Value MSE: " << value_mse << endl;
		time_t current_timer = time(NULL);
		double time_elapsed = difftime(current_timer, start_timer);
		log_fs << epoch << " " << train_loss_val << " " << validate_loss_val << " " << time_elapsed << endl;
	}
	time_t end_timer = time(NULL);
	double total_time = difftime(end_timer, start_timer);
	int train_size = train_reps.size();
	int validate_size = validate_reps.size();
	cout << "Summary:" << endl;
	cout << "Number of parameters in the model: " << GetModelSize(model) << endl;
	cout << "Number of epochs: " << epoch - 1 << endl;
	cout << "Train set size: " << train_size << endl;
	cout << "Validate set size: " << validate_size << endl;
	cout << "Total training time: " << total_time << endl;

	// save the model
	SaveModel(model, model_file_path);
}

