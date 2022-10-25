#pragma once

#include "DataUtil.h"

#include <memory>

torch::Tensor UnitForward(const torch::Tensor& input, torch::nn::Linear& layer, bool is_training);
torch::Tensor UnitForwardWithIntermediate(const torch::Tensor& input, torch::nn::Linear& to_intermediate, torch::nn::Linear& to_final, bool is_training);
torch::Tensor ForwardClassifyOut(const torch::Tensor& input, torch::nn::Linear& layer);
torch::Tensor ForwardBinaryClassifyOut(const torch::Tensor& input, torch::nn::Linear& layer);
torch::Tensor ForwardValueOut(const torch::Tensor& input, torch::nn::Linear& layer);

struct CardEncoderNet : torch::nn::Cloneable<CardEncoderNet>
{
	CardEncoderNet();
	void reset() override;
	void register_params(); // register the parameters using the submodules (network layers), assuming they are already property constructed/allocated
	// encoding/decoding methods
	torch::Tensor forward_mu(const torch::Tensor& code);
	torch::Tensor forward_log_std(const torch::Tensor& code);
	torch::Tensor decode_conditioned_root(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_card_root(NodeRep*& rep);
	torch::Tensor decode_card_root(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_attack_times(NodeRep*& rep);
	torch::Tensor decode_attack_times(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_minion_type(NodeRep*& rep);
	torch::Tensor decode_minion_type(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_abilities(NodeRep*& rep);
	torch::Tensor decode_abilities(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_damage_abilities(NodeRep*& rep);
	torch::Tensor decode_damage_abilities(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_any_abil(NodeRep*& rep);
	torch::Tensor decode_any_abil(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_special_effects(NodeRep*& rep);
	torch::Tensor decode_special_effects(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_targeted_play_eff(NodeRep*& rep);
	torch::Tensor decode_targeted_play_eff(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_other_effs(NodeRep*& rep);
	torch::Tensor decode_other_effs(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_other_eff(NodeRep*& rep);
	torch::Tensor decode_other_eff(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_targeted_eff(NodeRep*& rep);
	torch::Tensor decode_targeted_eff(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_untargeted_eff(NodeRep*& rep);
	torch::Tensor decode_untargeted_eff(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_target_cond(NodeRep*& rep);
	torch::Tensor decode_target_cond(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_char_target_cond(NodeRep*& rep);
	torch::Tensor decode_char_target_cond(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_char_type_cond(NodeRep*& rep);
	torch::Tensor decode_char_type_cond(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_card_target_cond(NodeRep*& rep);
	torch::Tensor decode_card_target_cond(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_card_type_cond(NodeRep*& rep);
	torch::Tensor decode_card_type_cond(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_card_pos_cond(NodeRep*& rep);
	torch::Tensor decode_card_pos_cond(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_allegiance_cond(NodeRep*& rep);
	torch::Tensor decode_allegiance_cond(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_abil_cond(NodeRep*& rep);
	torch::Tensor decode_abil_cond(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_stat_cond(NodeRep*& rep);
	torch::Tensor decode_stat_cond(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_stat_cond_variant(NodeRep*& rep);
	torch::Tensor decode_stat_cond_variant(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_inde_cond(NodeRep*& rep);
	torch::Tensor decode_inde_cond(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_base_targeted_eff(NodeRep*& rep);
	torch::Tensor decode_base_targeted_eff(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_base_untargeted_eff(NodeRep*& rep);
	torch::Tensor decode_base_untargeted_eff(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_destination(NodeRep*& rep);
	torch::Tensor decode_destination(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	torch::Tensor encode_new_card_variant(NodeRep*& rep);
	torch::Tensor decode_new_card_variant(const torch::Tensor& code, int& correct_count, int& classify_count, double& sum_sqr_err, int& value_count, NodeRep*& rep);
	// layers/parameters
	torch::nn::Linear root_to_mu_intermediate;
	torch::nn::Linear root_to_mu_final;
	torch::nn::Linear root_to_log_std_intermediate;
	torch::nn::Linear root_to_log_std_final;
	torch::nn::Linear condition_layer_to_intermediate;
	torch::nn::Linear condition_layer_to_final;
	torch::nn::Linear card_root_encoder_to_intermediate;
	torch::nn::Linear card_root_encoder_to_final;
	torch::nn::Linear card_root_decoder_to_intermediate;
	torch::nn::Linear card_root_decoder_classify; // because we cannot apply dropout at the output, i.e. classify & value (otherwise the loss will be problematic), we need to separate these out (similar below)
	torch::nn::Linear card_root_decoder_binary_classify; // binary classify is occasionally used which requires a different range (0 ~ 1) for input values
	torch::nn::Linear card_root_decoder_value;
	torch::nn::Linear card_root_decoder_hidden;
	torch::nn::Linear special_effects_encoder_to_intermediate;
	torch::nn::Linear special_effects_encoder_to_final;
	torch::nn::Linear special_effects_decoder_to_intermediate;
	torch::nn::Linear special_effects_decoder_classify;
	torch::nn::Linear special_effects_decoder_hidden;
	torch::nn::Linear other_effs_encoder_to_intermediate;
	torch::nn::Linear other_effs_encoder_to_final;
	torch::nn::Linear other_effs_decoder_to_intermediate;
	torch::nn::Linear other_effs_decoder_classify;
	torch::nn::Linear other_effs_decoder_binary_classify;
	torch::nn::Linear other_effs_decoder_hidden;
	torch::nn::Linear targeted_eff_encoder_to_intermediate;
	torch::nn::Linear targeted_eff_encoder_to_final;
	torch::nn::Linear targeted_eff_decoder_to_intermediate;
	torch::nn::Linear targeted_eff_decoder_classify;
	torch::nn::Linear targeted_eff_decoder_hidden;
	torch::nn::Linear untargeted_eff_encoder_to_intermediate;
	torch::nn::Linear untargeted_eff_encoder_to_final;
	torch::nn::Linear untargeted_eff_decoder_to_intermediate;
	torch::nn::Linear untargeted_eff_decoder_classify;
	torch::nn::Linear untargeted_eff_decoder_hidden;
	torch::nn::Linear target_cond_encoder_to_intermediate;
	torch::nn::Linear target_cond_encoder_to_final;
	torch::nn::Linear target_cond_decoder_to_intermediate;
	torch::nn::Linear target_cond_decoder_classify;
	torch::nn::Linear target_cond_decoder_hidden;
	torch::nn::Linear char_target_cond_encoder_to_intermediate; // this set and the other "card" version are basically functioning as layers to unify the representations for the children of a rule expansion upper on the tree, similar to those sub layers inside base_untargeted_eff, base_untargeted_eff, and inde_cond forwarding
	torch::nn::Linear char_target_cond_encoder_to_final;
	torch::nn::Linear char_target_cond_decoder_to_intermediate;
	torch::nn::Linear char_target_cond_decoder_classify;
	torch::nn::Linear char_target_cond_decoder_value;
	torch::nn::Linear card_target_cond_encoder_to_intermediate;
	torch::nn::Linear card_target_cond_encoder_to_final;
	torch::nn::Linear card_target_cond_decoder_to_intermediate;
	torch::nn::Linear card_target_cond_decoder_classify;
	torch::nn::Linear card_target_cond_decoder_value;
	torch::nn::Linear inde_cond_encoder_to_intermediate;
	torch::nn::Linear inde_cond_encoder_to_final;
	torch::nn::Linear inde_cond_decoder_to_intermediate;
	torch::nn::Linear inde_cond_decoder_classify;
	torch::nn::Linear inde_cond_decoder_hidden;
	torch::nn::Linear leader_cond_encoder_to_intermediate;
	torch::nn::Linear leader_cond_encoder_to_final;
	torch::nn::Linear leader_cond_decoder_to_intermediate;
	torch::nn::Linear leader_cond_decoder_classify;
	torch::nn::Linear leader_cond_decoder_value;
	torch::nn::Linear mp_or_max_cond_encoder_to_intermediate;
	torch::nn::Linear mp_or_max_cond_encoder_to_final;
	torch::nn::Linear mp_or_max_cond_decoder_to_intermediate;
	torch::nn::Linear mp_or_max_cond_decoder_classify;
	torch::nn::Linear mp_or_max_cond_decoder_value;
	torch::nn::Linear base_targeted_eff_encoder_to_intermediate;
	torch::nn::Linear base_targeted_eff_encoder_to_final;
	torch::nn::Linear base_targeted_eff_decoder_to_intermediate;
	torch::nn::Linear base_targeted_eff_decoder_classify;
	torch::nn::Linear base_targeted_eff_decoder_value;
	torch::nn::Linear base_targeted_eff_decoder_hidden;
	torch::nn::Linear damage_eff_encoder_to_intermediate;
	torch::nn::Linear damage_eff_encoder_to_final;
	torch::nn::Linear damage_eff_decoder_to_intermediate;
	torch::nn::Linear damage_eff_decoder_binary_classify;
	torch::nn::Linear move_or_copy_eff_encoder_to_intermediate;
	torch::nn::Linear move_or_copy_eff_encoder_to_final;
	torch::nn::Linear move_or_copy_eff_decoder_to_intermediate;
	torch::nn::Linear move_or_copy_eff_decoder_classify;
	torch::nn::Linear transform_eff_encoder_to_intermediate;
	torch::nn::Linear transform_eff_encoder_to_final;
	torch::nn::Linear transform_eff_decoder_to_intermediate;
	torch::nn::Linear transform_eff_decoder_classify;
	torch::nn::Linear transform_eff_decoder_value;
	torch::nn::Linear transform_eff_decoder_hidden;
	torch::nn::Linear give_effects_eff_encoder_to_intermediate;
	torch::nn::Linear give_effects_eff_encoder_to_final;
	torch::nn::Linear give_effects_eff_decoder_to_intermediate;
	torch::nn::Linear give_effects_eff_decoder_hidden;
	torch::nn::Linear base_untargeted_eff_encoder_to_intermediate;
	torch::nn::Linear base_untargeted_eff_encoder_to_final;
	torch::nn::Linear base_untargeted_eff_decoder_to_intermediate;
	torch::nn::Linear base_untargeted_eff_decoder_classify;
	torch::nn::Linear base_untargeted_eff_decoder_value;
	torch::nn::Linear base_untargeted_eff_decoder_hidden;
	torch::nn::Linear alle_to_cond_encoder_to_intermediate;
	torch::nn::Linear alle_to_cond_encoder_to_final;
	torch::nn::Linear alle_to_cond_decoder_to_intermediate;
	torch::nn::Linear alle_to_cond_decoder_classify;
	torch::nn::Linear new_eff_encoder_to_intermediate;
	torch::nn::Linear new_eff_encoder_to_final;
	torch::nn::Linear new_eff_decoder_to_intermediate;
	torch::nn::Linear new_eff_decoder_classify;
	torch::nn::Linear new_eff_decoder_value;
	torch::nn::Linear new_eff_decoder_hidden;
};

typedef std::shared_ptr<CardEncoderNet> CardEncoderWrapper;
#define ENCODER_WRAPPER_CAST std::dynamic_pointer_cast<CardEncoderNet> // used to cast a shared pointer of other types (torch::nn::Module) to one of CardEncoderNet type

torch::Tensor EncodeCard(const CardRep& rep, CardEncoderNet& model);
torch::Tensor DecodeCard(const torch::Tensor& code, const CardRep& rep, double& class_acc, double& value_mse, CardEncoderNet& model); // return reconstruction loss (no kl loss) as a single element tensor
torch::Tensor SingleCardAutoEncoderForward(const CardRep& rep, double& kl_val, double& class_acc, double& value_mse, CardEncoderNet& model); // return total loss (including kl loss) as a single element tensor
torch::Tensor GetCardMeanCode(const CardRep& rep, CardEncoderNet& model);
std::vector<torch::Tensor> GetCardsMeanCodes(const std::vector<CardRep>& reps, CardEncoderNet& model);

double TrainCardEncoderNoValidationSet(const std::vector<CardRep>& reps, int num_epochs, CardEncoderNet& model, double& kl_val, double& class_acc, double& value_mse); // returns the loss before training, the kl_val, class_acc, value_mse are all before training
double TrainCardEncoderNoValidationSetWeighted(const std::vector<CardRep>& reps, const std::vector<double>& weights, int num_epochs, CardEncoderNet& model, double& kl_val, double& class_acc, double& value_mse); // the weighted version of above
void TrainCardEncoderWithValidationSet(const std::vector<CardRep>& train_reps, const std::vector<CardRep>& validate_reps, int num_epochs, CardEncoderNet& model, int seed, const char* model_file_path, const char* log_file_path);

