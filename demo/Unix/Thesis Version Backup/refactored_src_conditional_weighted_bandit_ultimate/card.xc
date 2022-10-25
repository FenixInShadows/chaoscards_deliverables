giglstart;

import "Player.h";
import "CardEncoder.h";
import <iostream>;

#define SUPPRSS_MATCH_PROMPTS

#define DEFAULT_OVERHEAT_THRESHOLD 10

#define NOT_LEADER_PARAM 0.9 // a prob. parameter for not allowing played leaders be a valid target (not allowing leader vs not allowing on field)
#define NOT_POS_PARAM 0.9 // a prob. parameter for not allowing moving to the same type of position, i.e. field to field, hand to hand, deck to deck (not allowing same type of position vs not allowing moving to same allegiance)

#define COST_DEC_PROB 0.8
/* for atkHpModEff, the different categories probabilites are computed according to the sampling method
1. Compute the prob of abs of atkmod (or hpmod) being sampled as 0, recorded as x; then as the strong positive case is atkmod >= 0, its prob would be (1+x)/2
2. As the strong negative case is atkmod < 0 and hpmod == 0, its prob would be (1 - strong_positive_prob) * x = x(1-x)/2
3. The neutral case is atkmod + hpmod == 0 (but they can't be both 0 as then atkmod would be modified and it belongs to strong positive as it already does), so we compute for (-1, 1), (-2, 2)...
	which would be P(atkmod==1)^2 + P(atkmod==2)^2 +... (assuming x is symmetrically sampled and one side of x is sampled with the same shape of y), recorded as y;
	then as there's half chance atkmod is sampled on the negative side for its raw value (before rounding), the neutral case prob would be y/2.
4. The weak positive case is atkmod < 0 and atkmod + hpmod > 0. Its prob would be ((1 - strong_positive_prob)*(1 - x) - neutral_prob)/2 = ((1-x)^2-y)/4
	as given atkmod is in [-10, -1] and hpmod is in [1, 10], it is equal likely for abs(atkmod) being larger and being smaller than hpmod.
5. The weak negative case is atkmod + hpmod < 0 and hpmod > 0. Its prob would be ((1 - strong_positive_prob)*(1 - x) - neutral_prob)/2 = ((1-x)^2-y)/4
	The reason is similar to above but here we want to exclude the strong negative case. Alternatively this can also be computed by subtracting all other probs from 1.
*/
#define ATK_HP_STRONG_POSITIVE_PROB 0.608237
#define ATK_HP_WEAK_POSITIVE_PROB 0.062468
#define ATK_HP_NEUTRAL_PROB 0.182021
#define ATK_HP_WEAK_NEGATIVE_PROB 0.062468
#define ATK_HP_STRONG_NEGATIVE_PROB 0.084806
#define ATK_TIMES_DEC_PROB 0.2

#define DAMAGE_ALLY_BIAS 0.125
#define HEAL_ALLY_BIAS 8.0
#define RES_ATK_TIMES_ALLY_BIAS 32.0
#define COST_MOD_ALLY_BIAS_POSITIVE 0.125
#define COST_MOD_ALLY_BIAS_NEGATIVE 8.0 
#define ATK_HP_MOD_ALLY_BIAS_STRONG_POSITIVE 8.0
#define ATK_HP_MOD_ALLY_BIAS_WEAK_POSITIVE 2.0
#define ATK_HP_MOD_ALLY_BIAS_NEUTRAL 1.0
#define ATK_HP_MOD_ALLY_BIAS_WEAK_NEGATIVE 0.5
#define ATK_HP_MOD_ALLY_BIAS_STRONG_NEGATIVE 0.125
#define ATK_TIMES_MOD_ALLY_BIAS_POSITIVE 16.0
#define ATK_TIMES_MOD_ALLY_BIAS_NEGATIVE 0.0625
#define DESTROY_ALLY_BIAS 0.125
#define DISCARD_ALLY_BIAS 0.125
#define MOVE_ALLY_BIAS 2.0
#define COPY_ALLY_BIAS 4.0
#define TRANSFORM_ALLY_BIAS 0.25
#define CHANGE_MINION_TYPE_ALLY_BIAS 4.0
#define GIVE_TAUNT_OR_STEALTH_ALLY_BIAS 4.0 // give taunt or stealth can be used on the opponent for lifting targeting constraints
#define GIVE_UNTARGETABLE_ALLY_BIAS 8.0 // give untargetability can also prevent the opponent from targeting their own characters/cards
#define GIVE_OTHER_ABILITY_ALLY_BIAS 32.0
#define RM_ABILITIES_ALLY_BIAS 0.0625
#define SET_OVERHEAT_ALLY_BIAS 0.03125
#define DEC_OH_THRESHOLD_ALLY_BIAS 0.03125
#define RESET_STATE_ALLY_BIAS 0.25
#define GIVE_EFFECTS_ALLY_BIAS 8.0

#define DEFAULT_EFFECT_EVAL 0.2

class Player;
class CondConfig;
class Card;
typedecl PtrRedirMap;
typedecl PtrRedirMapIter;
typedecl CardRep;
typedecl NodeRep;
typedecl DescTokenVec;
typedecl CardEncoderNet;
typedecl torch_Tensor;
typedecl ExtraCardGenConfig;

giglconfig GetDefaultGenConfig(int seed, void* extra_config);
Card* CreatePlainMinionToken(string parent_name, CardEncoderNet* encoder, const torch_Tensor& code);
Card* CreateFixedCardToken(string parent_name, int min_eff_num, int max_eff_num, int eff_depth, CardEncoderNet* encoder, const torch_Tensor& code);
Card* CreateFixedMinionToken(string parent_name, int min_eff_num, int max_eff_num, int eff_depth, CardEncoderNet* encoder, const torch_Tensor& code);
Card* CreateNamedCardFromRep(const string& name, NodeRep*& rep);
Card* CreateNamedCardFromLatentCode(const string& name, CardEncoderNet* encoder, const torch_Tensor& code);
Card* CreateNamedCardFromLatentCodeWithCost(const string& name, CardEncoderNet* encoder, const torch_Tensor& code, int cost);
Card* CreateNamedMinionFromLatentCodeWithCost(const string& name, CardEncoderNet* encoder, const torch_Tensor& code, int cost);


gigltype Card{int seed, int max_eff_num, int max_eff_depth, void* extra_config}: // seed is only used for default naming (for generated cards), for constructed (including copied) cards it doesn't matter; card_rep is for creating cards from a saved representation, if nullptr then it means generate from scratch
{
wrapper:
	initializer
	{
		contribution = nullptr;
		cost_constrained_contribution = nullptr;
		is_dying = false;
		is_resetting = false;
		replacement = nullptr;
		is_first_turn_at_field = false;
		owner = nullptr;
		opponent = nullptr;
		card_pos = CARD_POS_UNKNOWN;
		orig_mana = mana = -1;
		orig_atk = atk = -1;
		orig_hp = max_hp = -1;
		hp_loss = 0;
		orig_n_atks = max_n_atks = -1;
		n_atks_loss = 0;
		orig_minion_type = minion_type = NONE_MINION;
		orig_is_charge = is_charge = false;
		orig_is_taunt = is_taunt = false;
		orig_is_stealth = is_stealth = false;
		orig_is_untargetable = is_untargetable = false;
		orig_is_shielded = is_shielded = false;
		orig_is_poisonous = is_poisonous = false;
		orig_is_lifesteal = is_lifesteal = false;
	}
	generator
	{
		ExtraCardGenConfig* ex_conf = (ExtraCardGenConfig*)extra_config;
		if (ex_conf && ex_conf->name.length() > 0)
			name = ex_conf->name;
		else
			name = "#" + IntToStr(seed);
		CondConfig tmp_config = GetDefaultConfig(); // to counter the problem of rvalue passed to lvalue ref
		if (ex_conf)
		{
			int tmp_cost = -1;
			if (ex_conf->forcing_minion)
			{
				tmp_config.flag = MINION_COND_FILTER;
				tmp_cost = SampleNonLeaderCostClamped(ex_conf->min_cost, ex_conf->max_cost);
			}
			else
			{
				tmp_cost = SampleDefaultStatClamped(ex_conf->min_cost, ex_conf->max_cost); // use the range for all possible cost, as we don't yet know about the card type at this point
			}
			tmp_config.min_cost = tmp_config.max_cost = tmp_cost; // forcing the cost (no matter what the encoder later predicts for the cost field, it'll be corrected; the encoder still predicts for the cost because certain token cards will skip this item level generator and does not want the cost be predetermined)

			if (ex_conf->encoder)
			{
				torch_Tensor tmp_cat_list[] = {CreateSingleValTensor(NormalizeCode(tmp_cost, 0, 10)), ex_conf->code}; // use the range for all possible cost, as we don't yet know about the card type at this point
				torch_Tensor root_code = UnitForwardWithIntermediate(torch_cat(tmp_cat_list), ex_conf->encoder->condition_layer_to_intermediate, ex_conf->encoder->condition_layer_to_final, false);
				root = generate CardRoot(tmp_config, false, ex_conf->rep, ex_conf->encoder, root_code); // in this case the rep is generally a nullptr (but needs a variable because it is a reference)
			}
			else
			{
				root = generate CardRoot(tmp_config, false, ex_conf->rep, nullptr, CreateEmptyTensor());
			}
		}
		else
		{
			NodeRep* tmp_nullptr = nullptr;
			root = generate CardRoot(tmp_config, false, tmp_nullptr, nullptr, CreateEmptyTensor()); // currently we only control the type and cost through the config so the issue on the range for hp is not a problem
		}
	}
	constructor
	{
		ExtraCardGenConfig* ex_conf = (ExtraCardGenConfig*)extra_config;
		if (ex_conf && ex_conf->name.length() > 0)
			name = ex_conf->name;
	}
	destructor
	{
		ClearExtraEffects();
		if (replacement)
			delete replacement;
	}
	void RegisterContribution(int* counter, int* cost_constrained_counter)
	{
		contribution = counter;
		cost_constrained_contribution = cost_constrained_counter;
	}
	void IncContribution(bool is_cost_constrained)
	{
		if (contribution)
			(*contribution)++;
		if (cost_constrained_contribution && is_cost_constrained)
			(*cost_constrained_contribution)++;
	}
	void SetAffiliation(Player* _owner)
	{
		owner = _owner;
		opponent = _owner->opponent;
	}
	int GetTargetIndex() // should limit the usage of this function as much as possible
	{
		if (owner->leader == item)
			return 0;
		for (int i = 0; i < owner->field.size(); i++)
			if (owner->field[i] == item)
				return i + 1;
		for (int i = 0; i < owner->hand.size(); i++)
			if (owner->hand[i] == item)
				return i + owner->field.size() + opponent->field.size() + 2;
		for (int i = 0; i < owner->deck.size(); i++)
			if (owner->deck[i] == item)
				return i + owner->field.size() + owner->hand.size() + opponent->field.size() + 2;
		return -1;  // shouldn't happen
	}
	void FillCardDescTokens(DescTokenVec& tokens, int indent_size, bool is_derived) // normally base info (stats & the type) is displayed on other visual parts, but for description of derived cards those info needs to be included in the text here 
	{
		int tmp_need_starting_newline = 0;

		// base info if needed (description of derived cards)
		if (is_derived)
		{
			tokens.push_back(mkNewLineToken());
			switch(card_type)
			{
			case LEADER_CARD:
				tokens.push_back(mkWordToken(IntToStr(orig_mana) + "/" + IntToStr(orig_atk) + "/" + IntToStr(orig_hp), indent_size, true));
				tokens.push_back(mkWordToken("Leader", indent_size, true));
				tokens.push_back(mkPunctToken(".", indent_size, true));
				break;
			case MINION_CARD:
				tokens.push_back(mkWordToken(IntToStr(orig_mana) + "/" + IntToStr(orig_atk) + "/" + IntToStr(orig_hp), indent_size, true));
				tokens.push_back(mkWordToken(MinionTypeDescription(minion_type), indent_size, true));
				tokens.push_back(mkPunctToken(".", indent_size, true));
				break;
			case SPELL_CARD:
				tokens.push_back(mkWordToken("Cost", indent_size, true));
				tokens.push_back(mkWordToken(IntToStr(orig_mana), indent_size, true));
				tokens.push_back(mkWordToken("Spell", indent_size, true));
				tokens.push_back(mkPunctToken(".", indent_size, true));
				break;
			}

			tmp_need_starting_newline = 1;
		}

		// main description
		root->FillTokens(tokens, indent_size, tmp_need_starting_newline);

		// extra effects (well, currently not really needed, but just in case)
		for (int i = 0; i < effects_extra.size(); i++)
			effects_extra[i]->FillTokens(tokens, indent_size, tmp_need_starting_newline);	
	}
	string BriefInfo()
	{
		return name + ", " + root->Brief();
	} 
	string DetailInfo()
	{
		return "Name: " + name + ".\n" + root->DetailIndent(0); 
	}
	bool IsSleeping() // use a getter to get dynamically so that it is easier to deal with giveCharge, removeAbilities, resetState etc.
	{
		return is_first_turn_at_field && !is_charge;
	}
	bool isTargetedAtPlay(int x, int y)
	{
		return root->isTargetedAtPlay(x, y, item);
	} 
	bool CheckPlayValid(int x, int y, int& z) 
	{ 
		return root->CheckPlayValid(x, y, z, item);
	} 
	void Play(int x, int y, int z)
	{
		root->Play(x, y, z, item);
		for (int i = 0; i < effects_extra.size(); i++)
			effects_extra[i]->Play(x, y, z, item);
	} 
	void Destroy()
	{
		root->Destroy(item);
		for (int i = 0; i < effects_extra.size(); i++)
			effects_extra[i]->Destroy(item);
	}
	void Discard()
	{
		root->Discard(item);
		for (int i = 0; i < effects_extra.size(); i++)
			effects_extra[i]->Discard(item);
	}
	bool CheckAttackValid(int x, int z) 
	{ 
		return root->CheckAttackValid(x, z, item); // currently assuming effects does not affect what target is valid to attack (so nor do the extra effects)
	}
	void Attack(int x, int z)
	{
		root->Attack(x, z, item);
		// currently there no on attack effects are implemented yet so extra effects are not yet included here
		/*
		for (int i = 0; i < effects_extra.size(); i++)
			effects_extra[i]->Attack(x, z, item);
		*/
	}
	void TurnStart(Card* leader)
	{
		root->TurnStart(leader, item);
		for (int i = 0; i < effects_extra.size(); i++)
			effects_extra[i]->TurnStart(leader, item);
	}
	void TurnEnd(Card* leader)
	{
		root->TurnEnd(leader, item);
		for (int i = 0; i < effects_extra.size(); i++)
			effects_extra[i]->TurnEnd(leader, item);
	}
	void SetAllOverheatCounts(int val) // has to use a different name as the node version due to artifacts from GIGL (if the signature is the same then it'll collide with the auto-added duplicates of the node version)
	{
		root->SetOverheatCounts(val);
		for (int i = 0; i < effects_extra.size(); i++)
			effects_extra[i]->SetOverheatCounts(val);
	}
	void SetAllOverheatThresholds(int val) // has to use a different name as the node version due to artifacts from GIGL (if the signature is the same then it'll collide with the auto-added duplicates of the node version)
	{
		root->SetOverheatThresholds(val);
		for (int i = 0; i < effects_extra.size(); i++)
			effects_extra[i]->SetOverheatThresholds(val);
	}
	void ModAllOverheatThresholds(int amount) // has to use a different name as the node version due to artifacts from GIGL (if the signature is the same then it'll collide with the auto-added duplicates of the node version)
	{
		root->ModOverheatThresholds(amount); // note the amount here is always negative
		for (int i = 0; i < effects_extra.size(); i++)
			effects_extra[i]->ModOverheatThresholds(amount);
	}
	void TakeDamage(int amount, Card* src, bool start_of_batch)
	{
		if (is_shielded)
		{
			if (amount > 0)
			{
				#ifndef SUPPRSS_MATCH_PROMPTS
				if (!owner->is_exploration)
					cout << "Damage to " << owner->name << "\'s " << name << " absorbed by divine shield." << endl;
				#endif
				is_shielded = false;
			}
			if (src && src->is_poisonous && card_type == MINION_CARD) // note, zero damage poison function the same as non-zero damage poison on minions (probably not the same as Hearthstone; but similar to Shadowverse, except for blocked by shield though)
			{
				#ifndef SUPPRSS_MATCH_PROMPTS
				if (!owner->is_exploration)
					cout << "Poision to " << owner->name << "\'s " << name << " absorbed by divine shield." << endl;
				#endif
				is_shielded = false;
			}
			#ifndef SUPPRSS_MATCH_PROMPTS
			if (!owner->is_exploration)
			{
				if (is_shielded) // note. zero damage does not break shield (probably not the same as Shadowverse but similar to Hearthstone) 
					cout << "Zero damage to " << owner->name << "\'s " << name << " does not break the divine shield" << endl;
				else
					cout << owner->name << "\'s " << name << "\'s divine shield consumed." << endl;
			}
			#endif
		}
		else
		{
			#ifndef SUPPRSS_MATCH_PROMPTS
			if (!owner->is_exploration)
				cout << IntToStr(amount) << " damage dealt to " << owner->name << "\'s " << name << "." << endl;
			#endif
			// reduce health before checking for lifesteal (for cases like dealing damage to your own leader with lifesteal, so that at full/near full health the hp restore will not be wasted because of overflowing)
			// note, leader with lifesteal attacking a minion will always first restore health because the counter attack is always computed later (so leader attacking at full health to a minion may actualy waste hp retore because of overflowing)
			hp_loss += amount;
			if (src && src->is_lifesteal && !src->owner->leader->is_dying)
			{
				#ifndef SUPPRSS_MATCH_PROMPTS
				if (!owner->is_exploration)
					cout << src->owner->name << "\'s " << src->name << " lifesteal ability activated." << endl;
				#endif
				src->owner->leader->RestoreHp(amount);
			}
			if (hp_loss >= max_hp) // do this check after checking lifesteal because for cases like dealing damage to your own leader with lifesteal it shouldn't destroyed if lifesteal gets it back
				owner->FlagDestroy(item, start_of_batch);
			if (src && src->is_poisonous && card_type == MINION_CARD) // note, zero damage poison function the same as non-zero damage poison on minions (probably not the same as Hearthstone; but similart to Shadowverse, except for blocked by shield though)
			{
				#ifndef SUPPRSS_MATCH_PROMPTS
				if (!owner->is_exploration)
					cout << src->owner->name << "\'s " << src->name << " poisonous ability activated." << endl;
				#endif
				if (!is_dying) // to prevent double-destroy (there'a another potential destroy above)
					owner->FlagDestroy(item, start_of_batch);
			}
		}
	}
	void RestoreHp(int amount)
	{
		#ifndef SUPPRSS_MATCH_PROMPTS
		if (!owner->is_exploration)
			cout << IntToStr(amount) << " health restored to " << owner->name << "\'s " << name << "." << endl;
		#endif
		if (hp_loss < amount)
		{
			#ifndef SUPPRSS_MATCH_PROMPTS
			if (!owner->is_exploration)
				cout << "Actually only restored " << IntToStr(hp_loss) << "." << endl;
			#endif
			hp_loss = 0;
		}
		else
		{
			hp_loss -= amount;
		}
	}
	void RestoreAtkTimes(int amount)
	{
		#ifndef SUPPRSS_MATCH_PROMPTS
		if (!owner->is_exploration)
			cout << IntToStr(amount) << " attack times restored to " << owner->name << "\'s " << name << "." << endl;
		#endif
		if (n_atks_loss < amount)
		{
			#ifndef SUPPRSS_MATCH_PROMPTS
			if (!owner->is_exploration)
				cout << "Actually only restored " << IntToStr(n_atks_loss) << "." << endl;
			#endif
			n_atks_loss = 0;
		}
		else
		{
			n_atks_loss -= amount;
		}
	}
	void ModifyAtkHp(int atkmod, int hpmod) // assume hpmod >= 0
	{
		#ifndef SUPPRSS_MATCH_PROMPTS
		if (!owner->is_exploration)
			cout << (atkmod < 0 ? "" : "+") << IntToStr(atkmod) << "/" << (hpmod < 0 ? "" : "+") << IntToStr(hpmod) << " given to " << owner->name << "\'s " << name << "." << endl;
		#endif
		atk += atkmod;
		if (atk < 0)
			atk = 0;
		max_hp += hpmod;
	}
	void ModifyCost(int amount)
	{
		#ifndef SUPPRSS_MATCH_PROMPTS
		if (!owner->is_exploration)
			cout << (amount < 0 ? "" : "+") << IntToStr(amount) << " applied to the mana cost of " << owner->name << "\'s " << name << "." << endl;
		#endif
		mana += amount;
		if (mana < 0)
			mana = 0; 
	}
	void ModifyAtkTimes(int amount)
	{
		#ifndef SUPPRSS_MATCH_PROMPTS
		if (!owner->is_exploration)
			cout << (amount < 0 ? "" : "+") << IntToStr(amount) << " applied to the atk times of " << owner->name << "\'s " << name << "." << endl;
		#endif
		max_n_atks += amount;
		if (max_n_atks < 0)
			max_n_atks = 0;
	}
	double GetHeuristicEval()
	{
		double tmp_eval = root->GetNodeHeuristicEval(EFFECT_TIMING_DEFAULT, item);
		for (int i = 0; i < effects_extra.size(); i++)
			tmp_eval += effects_extra[i]->GetNodeHeuristicEval(EFFECT_TIMING_DEFAULT, item);
		return tmp_eval;
	}
	int* contribution; // pointer to the counter for contribution (for evaluation of card strength), if nullptr, then it means the match is not used for evaluation
	int* cost_constrained_contribution; // similar to above but only count the contribution that is considered constrained by cost (e.g. playing a card)
	string name;
	bool is_dying; // for deferred card removal
	bool is_resetting; // for deferred removal of extra effects
	Card* replacement; // used for tranform effect (deferred mechanism)
	bool is_first_turn_at_field; // whether it is the first turn the charactor come on to the field
	Player* owner;
	Player* opponent;
	int card_pos;
	int mana;
	int orig_mana;
	int atk;
	int orig_atk;
	int max_hp;
	int orig_hp;
	int hp_loss;
	int max_n_atks;
	int orig_n_atks;
	int n_atks_loss;
	int card_type;
	int minion_type;
	int orig_minion_type;
	bool is_charge;
	bool orig_is_charge;
	bool is_taunt;
	bool orig_is_taunt;
	bool is_stealth;
	bool orig_is_stealth;
	bool is_untargetable;
	bool orig_is_untargetable;
	bool is_shielded;
	bool orig_is_shielded;
	bool is_poisonous;
	bool orig_is_poisonous;
	bool is_lifesteal;
	bool orig_is_lifesteal;
	
node:
	void FillRep(CardRep& rep);
	void FillTokens(DescTokenVec& tokens, int indent_size, int& need_starting_newline) {} // well the int& really is intended to be bool&, but GIGL translates bool to int and would be incompatible when calling outside functions with bool& type by passing the same parameter, so let's just make it explicitly int
	void FillTokensAlt1(DescTokenVec& tokens, int indent_size, int& need_starting_newline) { FillTokens(tokens, indent_size, need_starting_newline); }
	void FillTokensAlt2(DescTokenVec& tokens, int indent_size, int& need_starting_newline) { FillTokens(tokens, indent_size, need_starting_newline); }
	void FillTokensAlt3(DescTokenVec& tokens, int indent_size, int& need_starting_newline) { FillTokens(tokens, indent_size, need_starting_newline); }
	void FillTokensAlt4(DescTokenVec& tokens, int indent_size, int& need_starting_newline) { FillTokens(tokens, indent_size, need_starting_newline); }
	void FillTokensAlt5(DescTokenVec& tokens, int indent_size, int& need_starting_newline) { FillTokens(tokens, indent_size, need_starting_newline); }
	void FillTokensAlt6(DescTokenVec& tokens, int indent_size, int& need_starting_newline) { FillTokens(tokens, indent_size, need_starting_newline); }
	void FillTokensAlt7(DescTokenVec& tokens, int indent_size, int& need_starting_newline) { FillTokens(tokens, indent_size, need_starting_newline); }
	void FillTokensSuffix(DescTokenVec& tokens, int indent_size) {}
	void FillTokensSuffixAlt1(DescTokenVec& tokens, int indent_size) { FillTokensSuffix(tokens, indent_size); }
	void FillTokensPostfix(DescTokenVec& tokens, int indent_size) {}
	string Brief() { return ""; }
	string Detail() { return ""; }
	string DetailIndent(int indent_size) { return RepeatSpace(indent_size) + Detail(); }
	string DetailAlt1() { return Detail(); } // for different phrasing, similar below
	string DetailAlt2() { return Detail(); }
	string DetailAlt3() { return Detail(); }
	string DetailAlt4() { return Detail(); }
	string DetailAlt5() { return Detail(); }
	string DetailAlt6() { return Detail(); }
	string Suffix() { return ""; } // for two-part description
	string SuffixAlt1() { return Suffix(); }
	string Postfix { return "\n"; } // for nesting (even after Suffix), the default newline is adopted for easy print formatting
	string PostfixIndent(int indent_size) { return Postfix(); }
	bool IsPlural() { return false; }
	bool IsThirdPersonSingle() { return false; }
	Node* CreateNodeHardCopy(Card* card_copy, PtrRedirMap& redir_map) { return nullptr; }
	int GetEffectNum() { return 0; }
	SpecialEffects* GetEffects() { return nullptr; }
	Card* GetRelatedCard(Card* parent_card) { return nullptr; }
	bool IsCondTrivial() { return false; }
	double GetAllyBias() { return 1.0; } // the ratio of attaching this with ally over opponent (should be non-negative, > 1.0 means favoring ally, < 1.0 means favoring opponent)
	unsigned GetInitAbilFlag() { return TARGET_TYPE_NOTHING; } // this is applied with bitwise or's so the default should be all zeros, this shouldn't be needed until we construct the card in parts (because generator would just modify the config in place)
	CondConfig GetPlayTargetConfig() { return GetTargetConfig(); } // for the target of a targeted effect
	CondConfig GetGlobalSelfConfig(const CondConfig& self_config, unsigned effect_timing) { return GetTargetConfig(); } // for getting constraints for effects that gives effects to cards
	CondConfig GetSelfConfig(const CondConfig& self_config) { return GetTargetConfig(); } // for getting constraints for source condition, those are mostly caused by self-targetting effects
	CondConfig GetLeaderConfig() { return GetDefaultConfig(); }
	CondConfig GetTargetConfig() { return GetDefaultConfig(); } // this is the most basic version, for general use (untargeted version)
	void AdjustGlobalStatRange(int& min_val, int& max_val) {}
	void AdjustStatRange(int& min_val, int& max_val) {}
	bool IsForbiddingOwnTurnStart(const CondConfig& self_config) { return false; } // if the atkTimes condition on a character is trivial, we do not allow it to be "at the start of your own turn", as attack times resets at that time
	bool isTargetedAtPlay(int x, int y, Card* parent_card) { return false; } // might related to position as some effects could be made only be able to specify target when played from and/or at specific positions; parent card is needed to accommodate the need of sharing effects accross cards
	bool CheckPlayValid(int x, int y, int& z, Card* parent_card) { return true; } 
	void Play(int x, int y, int z, Card* parent_card) {} // assuming valid
	void Destroy(Card* parent_card) {}
	void Discard(Card* parent_card) {}
	bool CheckAttackValid(int x, int z, Card* parent_card) { return true; }
	void Attack(int x, int z, Card* parent_card) {} // assuming valid	
	void TurnStart(Card* leader, Card* parent_card) {}
	void TurnEnd(Card* leader, Card* parent_card) {}
	bool TargetedAction(int z, Card* parent_card, bool start_of_batch, bool is_cost_constrained) { return false; } // return whether a deferred event is added to the event queue during the execution of this function (including calling other functions, but not including events added afterwards caused by changes by the function), used for AoE or other effects that must be all be checked for a potential draw
	void SubTargetedAction(Card* target, Card* parent_card, bool start_of_batch) {}
	void UntargetedAction(Card* parent_card, bool is_cost_constrained) {}
	bool CheckCardValid(Card* card, Card* parent_card) { return true; } // well currently it might not need the parent_card argument but if there were conditions like having attack more than this card etc. then it would be needed
	bool CheckThisValid(Card* parent_card) { return true; }
	bool CheckStatValid(int stat_val) { return true; }
	void Mutate(int min_eff_n, int max_eff_n, int effect_depth, CardEncoderNet* encoder, const torch_Tensor& code) {} // redo effects and attack times (used to do the two-step child card generation to overcome ableC artifacts), may add poisonous and lifesteal abilities but does not change any other ability
	void SetOverheatCounts(int val)
	{
		overheat_count = val;
	}
	void SetOverheatThresholds(int val)
	{
		overheat_threshold = val;
		if (overheat_threshold < 0)
			overheat_threshold = 0;
		else if (overheat_threshold > DEFAULT_OVERHEAT_THRESHOLD)
			overheat_threshold = DEFAULT_OVERHEAT_THRESHOLD;
	}
	void ModOverheatThresholds(int amount) // note the amount here is always negative
	{
		overheat_threshold += amount;
		if (overheat_threshold < 0)
			overheat_threshold = 0;
		else if (overheat_threshold > DEFAULT_OVERHEAT_THRESHOLD)
			overheat_threshold = DEFAULT_OVERHEAT_THRESHOLD;
	}
	double GetNodeHeuristicEval(unsigned effect_timing, Card* parent_card) { return 0.0; }
	double GetPoisonousGrowthFactor() { return 1.0; }
	double GetLifestealGrowthFactor() { return 1.0; }
	int num_refs; // number of references, for sharing nodes; currently only effective on special effects, stored at SpecialEffects level
	int overheat_count; // the number of times an effect (aggregated for ones from the same source, which are stored in the same address anyway) is triggered during a pair of turns, cleared at the end of your turns; the mechanism is used to prevent overly long action/turns from repeated activation of the same effect, stored at the TargetedEff or UntargetedEff level
	int overheat_threshold; // the max number an effect may be triggered during a pair of turns, default is 10
	bool prioritize_forbidding_leader; // forbid leader versus forbid field when need to be decided randomly, this is like storing a seed so multiple queries are consistent
	bool prioritize_forbidding_pos_collision; // forbid position collision versus forbid allegiance collision for a move effect when need to be decided randomly, this is like storing a seed so multiple queries are consistent

nonterminal:
	CardRoot(CondConfig& global_config, bool is_plain, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code); // if is_plain is true, then force no effects and single attack
	AttackTimes(NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	MinionType(NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	Abilities(CondConfig& self_config, int damage, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	DamageAbilities(CondConfig& self_config, unsigned target_mode, int damage, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	ChargeAbil(CondConfig& self_config, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	TauntAbil(CondConfig& self_config, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	StealthAbil(CondConfig& self_config, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	UntargetableAbil(CondConfig& self_config, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	ShieldAbil(CondConfig& self_config, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	PoisonousAbil(CondConfig& self_config, int damage, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	LifestealAbil(CondConfig& self_config, int damage, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	SpecialEffects(CondConfig& self_config, int min_n, int max_n, int effect_depth, bool give_eff, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code); // give_eff denotes whether this is generated inside a giveEffectsEff
	TargetedPlayEff(CondConfig& self_config, int effect_depth, bool give_eff, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	OtherEffs(CondConfig& self_config, int min_n, int max_n, int effect_depth, bool give_eff, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	OtherEff(CondConfig& self_config, int effect_depth, bool give_eff, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	TargetedEff(CondConfig& self_config, unsigned effect_timing, int effect_depth, bool give_eff, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	UntargetedEff(CondConfig& self_config, unsigned effect_timing, int effect_depth, bool give_eff, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	TargetCond(CondConfig& init_config, CondConfig& instant_config, unsigned target_mode, unsigned effect_timing, double ally_bias, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code); // init_config applies refers to the original state (of the card), instant_config refers to state at the instant when the effect activates
	CharTargetCond(CondConfig& init_config, CondConfig& instant_config, unsigned target_mode, unsigned effect_timing, double ally_bias, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	CharTypeCond(CondConfig& init_config, CondConfig& instant_config, unsigned target_mode, unsigned effect_timing, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	CardTargetCond(CondConfig& init_config, CondConfig& instant_config, unsigned target_mode, unsigned effect_timing, double ally_bias, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	CardTypeCond(CondConfig& init_config, CondConfig& instant_config, unsigned target_mode, unsigned effect_timing, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	CardPosCond(CondConfig& init_config, CondConfig& instant_config, unsigned target_mode, unsigned effect_timing, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	AllegianceCond(CondConfig& init_config, CondConfig& instant_config, unsigned target_mode, unsigned effect_timing, double ally_bias, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	AbilCond(CondConfig& init_config, CondConfig& instant_config, unsigned target_mode, unsigned effect_timing, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	StatCond(CondConfig& init_config, CondConfig& instant_config, unsigned target_mode, unsigned effect_timing, bool force_nontrivial, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	StatCondVariant(int lower_min, int lower_max, int upper_min, int upper_max, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	IndeCond(CondConfig& init_config, CondConfig& instant_self_config, CondConfig& instant_leader_config, unsigned effect_timing, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code); // for conditions that are not associated with a specific card, the config would be associated to leader card though, as leader condition is considered in here
	BaseTargetedEff(CondConfig& self_config, unsigned target_mode, unsigned effect_timing, int effect_depth, bool give_eff, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	BaseUntargetedEff(CondConfig& self_config, unsigned effect_timing, int effect_depth, bool give_eff, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	Destination(CondConfig& self_config, unsigned target_mode, unsigned effect_timing, unsigned dest_mode, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);
	NewCardVariant(CondConfig& self_config, unsigned target_mode, unsigned effect_timing, int effect_depth, NodeRep*& rep, CardEncoderNet* encoder, const torch_Tensor& code);

wrapper: // due to some type system artifact in GIGL, certian parts (e.g. the arguement type in AddExtraEffects) wouldn't work if it is put before the nontermial declaration part
	gigltable<SpecialEffects*> effects_extra;
	void AddExtraEffects(SpecialEffects* effects)
	{
		effects_extra.push_back(effects);
		effects->num_refs++;
	}
	void ClearExtraEffects()
	{
		for (int i = 0; i < effects_extra.size(); i++)
		{
			effects_extra[i]->num_refs--;
			if (effects_extra[i]->num_refs <= 0)
				delete effects_extra[i];
		}
		effects_extra.clear();
	}
	string GetExtraEffectsBrief()
	{
		string tmp_str = "";
		for (int i = 0; i < effects_extra.size(); i++)
			tmp_str += effects_extra[i]->Brief();
		return tmp_str;
	}
	string GetExtraEffectsDetail()
	{
		if (effects_extra.size() == 0)
			return "";

		string tmp_str = "Extra Effects:\n";
		for (int i = 0; i < effects_extra.size(); i++)
			tmp_str += effects_extra[i]->Detail() + "\n";
		return tmp_str;
	}
	string GetExtraEffectsDetailIndent(int indent_size)
	{
		if (effects_extra.size() == 0)
			return "";

		string tmp_str = RepeatSpace(indent_size) + "Extra Effects:\n";
		for (int i = 0; i < effects_extra.size(); i++)
			tmp_str += effects_extra[i]->DetailIndent(indent_size);
		return tmp_str;
	}
	Card* CreateHardCopy(PtrRedirMap& redir_map) // this version share no memory with the source of the copy, it copies everything (internal shared effects are enrepd in the hash map redir_map)
	{
		// cannot directly use the construct statement as we need to pass the item reference for the copy down the function
		Card* card_copy;
		if (config.IsEmpty())
			card_copy = new Card();
		else
			card_copy = new Card(config); // config is needed as the copied card may need to spawn new random card when effect on it is activated, which need the config
		card_copy->root = root->CreateNodeHardCopy(card_copy, redir_map);

		// note: no need to register the contribution counter as this type of copy is only used for AI exploration or backups in simulation, which does not count in the actual match contribution
		// note: no need to set the owner/opponent as they must be set after a pair of players are copied
		card_copy->name = name;
		card_copy->is_first_turn_at_field = is_first_turn_at_field;
		card_copy->card_pos = card_pos;
		card_copy->mana = mana;
		card_copy->atk = atk;
		card_copy->max_hp = max_hp;
		card_copy->hp_loss = hp_loss;
		card_copy->max_n_atks = max_n_atks;
		card_copy->n_atks_loss = n_atks_loss;
		card_copy->minion_type = minion_type;
		card_copy->is_charge = is_charge;
		card_copy->is_taunt = is_taunt;
		card_copy->is_stealth = is_stealth;
		card_copy->is_untargetable = is_untargetable;
		card_copy->is_shielded = is_shielded;
		card_copy->is_poisonous = is_poisonous;
		card_copy->is_lifesteal = is_lifesteal;

		for (int i = 0; i < effects_extra.size(); i++) // don't use AddExtraEffects as the num_refs are managed separately
			card_copy->effects_extra.push_back((SpecialEffects*)(effects_extra[i]->CreateNodeHardCopy(card_copy, redir_map))); // using the card_copy as the item reference can be problematic, but we need to make the granted effects to work independent of the original card anyway
		
		return card_copy;
	}
	Card* CreateCopy() // this version shares the effects with the source of the copy, also it resets some abilities such as consumed attack times, is_first_turn_at_field flag etc.
	{
		Card* card_copy;
		SpecialEffects* effects = root->GetEffects();

		switch (card_type)
		{
			case LEADER_CARD:
				card_copy = construct Card(leaderCard(orig_mana, orig_atk, orig_hp, multipleAttack(orig_n_atks),
												justAbilities((orig_is_charge ? (ChargeAbil*)justCharge() : (ChargeAbil*)noCharge()),
																(orig_is_taunt ? (TauntAbil*)justTaunt() : (TauntAbil*)noTaunt()),
																(orig_is_stealth ? (StealthAbil*)justStealth() : (StealthAbil*)noStealth()),
																(orig_is_untargetable ? (UntargetableAbil*)justUntargetable() : (UntargetableAbil*)noUntargetable()),
																(orig_is_shielded ? (ShieldAbil*)justShield() : (ShieldAbil*)noShield()),
																(orig_is_poisonous ? (PoisonousAbil*)justPoisonous() : (PoisonousAbil*)noPoisonous()),
																(orig_is_lifesteal ? (LifestealAbil*)justLifesteal() : (LifestealAbil*)noLifesteal())),
												effects										
												)) with GetDefaultGenConfig(-1, nullptr);
				break;
			case MINION_CARD:
				card_copy = construct Card(minionCard(orig_mana, orig_atk, orig_hp, multipleAttack(orig_n_atks),
												(orig_minion_type == BEAST_MINION ? (MinionType*)beastMinion() : (orig_minion_type == DRAGON_MINION ? (MinionType*)dragonMinion() : (MinionType*)demonMinion())),
												justAbilities((orig_is_charge ? (ChargeAbil*)justCharge() : (ChargeAbil*)noCharge()),
																(orig_is_taunt ? (TauntAbil*)justTaunt() : (TauntAbil*)noTaunt()),
																(orig_is_stealth ? (StealthAbil*)justStealth() : (StealthAbil*)noStealth()),
																(orig_is_untargetable ? (UntargetableAbil*)justUntargetable() : (UntargetableAbil*)noUntargetable()),
																(orig_is_shielded ? (ShieldAbil*)justShield() : (ShieldAbil*)noShield()),
																(orig_is_poisonous ? (PoisonousAbil*)justPoisonous() : (PoisonousAbil*)noPoisonous()),
																(orig_is_lifesteal ? (LifestealAbil*)justLifesteal() : (LifestealAbil*)noLifesteal())),
												effects								
												)) with GetDefaultGenConfig(-1, nullptr);
				break;
			default:
				card_copy = construct Card(spellCard(orig_mana,
												justAbilities((orig_is_charge ? (ChargeAbil*)justCharge() : (ChargeAbil*)noCharge()),
																(orig_is_taunt ? (TauntAbil*)justTaunt() : (TauntAbil*)noTaunt()),
																(orig_is_stealth ? (StealthAbil*)justStealth() : (StealthAbil*)noStealth()),
																(orig_is_untargetable ? (UntargetableAbil*)justUntargetable() : (UntargetableAbil*)noUntargetable()),
																(orig_is_shielded ? (ShieldAbil*)justShield() : (ShieldAbil*)noShield()),
																(orig_is_poisonous ? (PoisonousAbil*)justPoisonous() : (PoisonousAbil*)noPoisonous()),
																(orig_is_lifesteal ? (LifestealAbil*)justLifesteal() : (LifestealAbil*)noLifesteal())),
												effects									
												)) with GetDefaultGenConfig(-1, nullptr);
				break;
		}
		
		effects->num_refs++;

		card_copy->contribution = contribution; // copy the pointer to the counter for contribution
		card_copy->cost_constrained_contribution = cost_constrained_contribution; // copy the pointer to the counter for contribution
		card_copy->name = name;
		card_copy->owner = owner; // for a copy effect this will be modified later when the event is resolved and the copy arrives at the destination
		card_copy->opponent = opponent; // for a copy effect this will be modified later when the event is resolved and the copy arrives at the destination
		card_copy->card_pos = card_pos; // for a copy effect this will be modified later when the event is resolved and the copy arrives at the destination
		card_copy->mana = mana;
		card_copy->atk = atk;
		card_copy->max_hp = max_hp;
		card_copy->hp_loss = hp_loss; // preserve hp loss
		card_copy->max_n_atks = max_n_atks;
		card_copy->minion_type = minion_type;
		card_copy->is_charge = is_charge;
		card_copy->is_taunt = is_taunt;
		card_copy->is_stealth = is_stealth;
		card_copy->is_untargetable = is_untargetable;
		card_copy->is_shielded = is_shielded;
		card_copy->is_poisonous = is_poisonous;
		card_copy->is_lifesteal = is_lifesteal;

		for (int i = 0; i < effects_extra.size(); i++) // cannot just say card_copy->effects_extra = effects_extra, as the num_refs needs to be properly maintained
			card_copy->AddExtraEffects(effects_extra[i]);
		
		return card_copy;
	}


rule:
	CardRoot
	default:
		{
			preselector
			{
				if (rep)
				{
					force [rep->choice]; // must wait until all terminal info is processed before ++
				}
				else
				{
					if (encoder)
					{
						double choice_code_adjust[3] = {0.0};
						if (!(global_config & TARGET_IS_LEADER) || global_config.max_cost < 8)
							choice_code_adjust[CHOICE_LEADER_CARD] = -1e10; // just having some large negative number
						if (!(global_config & TARGET_IS_MINION) || global_config.min_cost > 7)
							choice_code_adjust[CHOICE_MINION_CARD] = -1e10;
						if (!(global_config & TARGET_IS_SPELL) | global_config.min_cost > 7)
							choice_code_adjust[CHOICE_SPELL_CARD] = -1e10;
						torch_Tensor adjust_tensor = CreateVectorTensorFromArray(choice_code_adjust, 3);
						torch_Tensor intermediate_code = UnitForward(code, encoder->card_root_decoder_to_intermediate, false);
						torch_Tensor choice_tensor = ForwardClassifyOut(intermediate_code, encoder->card_root_decoder_classify).narrow(0, 0, 3);
						torch_Tensor final_tensor = choice_tensor + adjust_tensor;
						force [GetMaxIndex(final_tensor)];
					}
					else
					{
						if (!(global_config & TARGET_IS_LEADER) || global_config.max_cost < 8)
							forbid leaderCard;
						if (!(global_config & TARGET_IS_MINION) || global_config.min_cost > 7)
							forbid minionCard;
						if (!(global_config & TARGET_IS_SPELL) || global_config.min_cost > 7)
							forbid spellCard;
						
						// if leader and non-leaders are both possible, the prob of leader card is higher with higher costs
						probof(leaderCard) *= (double)(global_config.max_cost - 7)/3.0; // get the portion from cost 8, 9, 10 (very rough as they are not uniform)
						probof(minionCard) *= (double)(8 - global_config.min_cost)/8.0; // get the portion from cost 0, 1, ..., 7 (very rough as they are not uniform)
						probof(spellCard) *= (double)(8 - global_config.min_cost)/8.0; // get the portion from cost 0, 1, ..., 7 (very rough as they are not uniform)
					}
				}
			}
		}
	:=
	| leaderCard: int cost, int attack, int health, AttackTimes* attack_times, Abilities* abilities, SpecialEffects* effects
		{
			card_type = LEADER_CARD;
			generator
			{
				if (rep)
				{
					cost = DenormalizeCode(rep->term_info[0], 8, 10);
					attack = DenormalizeCode(rep->term_info[1], 0, 10);
					health = DenormalizeCode(rep->term_info[2], 10, 40);
					rep++;
				}
				else if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->card_root_decoder_to_intermediate, false);
					torch_Tensor value_tensor = ForwardValueOut(intermediate_code, encoder->card_root_decoder_value);
					cost = DenormalizeCode(GetSingleValFromTensor(value_tensor.narrow(0, 0, 1)), 8, 10);
					if (cost < global_config.min_cost)
						cost = global_config.min_cost;
					else if (cost > global_config.max_cost)
						cost = global_config.max_cost;
					attack = DenormalizeCode(GetSingleValFromTensor(value_tensor.narrow(0, 1, 1)), 0, 10);
					health = DenormalizeCode(GetSingleValFromTensor(value_tensor.narrow(0, 2, 1)), 10, 40);
				}
				else
				{
					// note leader card should have at least 8 mana cost
					int min_cost = 8, max_cost = 10;
					if (min_cost < global_config.min_cost)
						min_cost = global_config.min_cost;
					if (max_cost > global_config.max_cost)
						max_cost = global_config.max_cost;
					if (max_cost < min_cost) // shouldn't really happen but just in case we want to have some token leader card be of low cost (probably not happening atm)
						min_cost = max_cost;
					cost = SampleRangeLowerConcentratedLinearFromInt(min_cost, max_cost); // currently we only control the cost through the config
					attack = SampleRangeLowerConcentratedLinearFromInt(0, 10);
					health = SampleRangeCenterConcentratedLinearFromInt(10, 40);
				}
			}
			orig_mana = mana = cost;
			orig_atk = atk = attack;
			orig_hp = max_hp = health;
			generator
			{
				CondConfig self_config = GetInitConfigFromCard(item);
				if (!rep && is_plain)
				{
					attack_times = construct singleAttack(); // attack times info is only used for constraining effects part, and here, there's no effect, so no need to update
					if (encoder)
					{
						torch_Tensor intermediate_code = UnitForward(code, encoder->card_root_decoder_to_intermediate, false);
						torch_Tensor binary_classify_tensor = ForwardBinaryClassifyOut(intermediate_code, encoder->card_root_decoder_binary_classify);
						abilities = generate Abilities(self_config, atk, rep, encoder, binary_classify_tensor); // in this case rep will be nullptr anyway (can't just write a nullptr as it needs to fit Node*&)
					}
					else
					{
						abilities = generate Abilities(self_config, atk, rep, encoder, code); // in this case rep will be nullptr anyway (can't just write a nullptr as it needs to fit Node*&)
					}
					effects = construct specialEffects(noTargetedPlayEff(), noOtherEffs());
				}
				else 
				{
					if (encoder)
					{
						torch_Tensor intermediate_code = UnitForward(code, encoder->card_root_decoder_to_intermediate, false);
						torch_Tensor classify_tensor = ForwardClassifyOut(intermediate_code, encoder->card_root_decoder_classify);
						torch_Tensor binary_classify_tensor = ForwardBinaryClassifyOut(intermediate_code, encoder->card_root_decoder_binary_classify);
						torch_Tensor value_tensor = ForwardValueOut(intermediate_code, encoder->card_root_decoder_value);
						torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->card_root_decoder_hidden, false);
						attack_times = generate AttackTimes(rep, encoder, value_tensor.narrow(0, 3, Attack_Times_Size));
						self_config = GetInitConfigFromCard(item); // update attack times info
						abilities = generate Abilities(self_config, atk, rep, encoder, binary_classify_tensor);
						effects = generate SpecialEffects(self_config, 0, max_eff_num, 0, false, rep, encoder, hidden_tensor);
					}
					else
					{
						attack_times = generate AttackTimes(rep, encoder, code); // in this case the encoder will be nullptr anyway (writing a nullptr would be equivalanet)
						self_config = GetInitConfigFromCard(item); // update attack times info
						abilities = generate Abilities(self_config, atk, rep, encoder, code);
						effects = generate SpecialEffects(self_config, 0, max_eff_num, 0, false, rep, encoder, code);
					}
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_LEADER_CARD, <double>{NormalizeCode(cost, 8, 10), NormalizeCode(attack, 0, 10), NormalizeCode(health, 10, 40)}));
				attack_times->FillRep(rep);
				abilities->FillRep(rep);
				effects->FillRep(rep);
			}
			FillTokens
			{
				if (max_n_atks != 1)
				{
					if (need_starting_newline)
						tokens.push_back(mkNewLineToken());
					tokens.push_back(mkWordToken("Attacks", indent_size, true));
					tokens.push_back(mkWordToken(IntToStr(max_n_atks), indent_size, true));
					tokens.push_back(mkWordToken("Times", indent_size, true));
					tokens.push_back(mkPunctToken(".", indent_size, true));
					need_starting_newline = 1;	
				}
				AbilityFillTokens(is_charge, is_taunt, is_stealth, is_untargetable, is_shielded, is_poisonous, is_lifesteal, tokens, indent_size, need_starting_newline);
				effects->FillTokens(tokens, indent_size, need_starting_newline);
			}
			Brief
			{
				return IntToStr(mana) + "/" + IntToStr(atk) + "/" + IntToStr(max_hp - hp_loss) + ", " + IntToStr(max_n_atks - n_atks_loss) + "Attacks" + (IsSleeping() ? "(Sleeping)" : "")
				+ ", Leader. " + abilities->Brief() + " " + effects->Brief() + GetExtraEffectsBrief();
			}
			Detail
			{
				string mana_info = "MANA: " + IntToStr(mana) + " (originally " + IntToStr(orig_mana) + ").";
				string atk_info = "ATK: " + IntToStr(atk) + " (originally " + IntToStr(orig_atk) + ").";
				string hp_info = "HP: " + IntToStr(max_hp - hp_loss) + " of " + IntToStr(max_hp) + " (originally " + IntToStr(orig_hp) + ").";
				string atk_times_info = "ATK TIMES: " + IntToStr(max_n_atks - n_atks_loss) + " of " + IntToStr(max_n_atks) + " (originally " + IntToStr(orig_n_atks) + ")";
				string att_times_explain = "";
				if (max_n_atks == 0)
					att_times_explain = " (Cannot attack)";
				else if (max_n_atks > 1)
					att_times_explain = " (Can attack " + IntToStr(max_n_atks) + " times per turn)";
				if (IsSleeping())
					att_times_explain += "(Sleeping).";
				else
					att_times_explain += ".";
				string type_info = "TYPE: Leader (Does not counter-attack).";
				string abil_info = abilities->Detail();
				string effect_info = effects->Detail();
				string extra_effect_info = GetExtraEffectsDetail();
				return mana_info + "\n" + atk_info + "\n" + hp_info + "\n" + atk_times_info + att_times_explain + "\n" + type_info + "\n" + abil_info + effect_info + extra_effect_info;
			}
			DetailIndent
			{
				string mana_info = RepeatSpace(indent_size) + "MANA: " + IntToStr(mana) + " (originally " + IntToStr(orig_mana) + ").";
				string atk_info = RepeatSpace(indent_size) + "ATK: " + IntToStr(atk) + " (originally " + IntToStr(orig_atk) + ").";
				string hp_info = RepeatSpace(indent_size) + "HP: " + IntToStr(max_hp - hp_loss) + " of " + IntToStr(max_hp) + " (originally " + IntToStr(orig_hp) + ").";
				string atk_times_info = RepeatSpace(indent_size) + "ATK TIMES: " + IntToStr(max_n_atks - n_atks_loss) + " of " + IntToStr(max_n_atks) + " (originally " + IntToStr(orig_n_atks) + ")";
				string att_times_explain = "";
				if (max_n_atks == 0)
					att_times_explain = " (Cannot attack)";
				else if (max_n_atks > 1)
					att_times_explain = " (Can attack " + IntToStr(max_n_atks) + " times per turn)";
				if (IsSleeping())
					att_times_explain += "(Sleeping).";
				else
					att_times_explain += ".";
				string type_info = RepeatSpace(indent_size) + "TYPE: Leader (Does not counter-attack).";
				string abil_info = abilities->DetailIndent(indent_size);
				string effect_info = effects->DetailIndent(indent_size);
				string extra_effect_info = GetExtraEffectsDetailIndent(indent_size);
				return mana_info + "\n" + atk_info + "\n" + hp_info + "\n" + atk_times_info + att_times_explain + "\n" + type_info + "\n" + abil_info + effect_info + extra_effect_info;
			}
			CreateNodeHardCopy = new leaderCard(card_copy, cost, attack, health, 
				(AttackTimes*)(attack_times->CreateNodeHardCopy(card_copy, redir_map)),
				(Abilities*)(abilities->CreateNodeHardCopy(card_copy, redir_map)),
				(SpecialEffects*)(effects->CreateNodeHardCopy(card_copy, redir_map)));
			GetEffects = effects;
			isTargetedAtPlay
			{
				if (!effects->isTargetedAtPlay(x, y, parent_card))
					return false;

				// check if there is a valid target for battlecry
				for (int i = 0; i <= parent_card->owner->field.size() + parent_card->opponent->field.size() + parent_card->owner->hand.size() + 1; i++)
					if (x != i && effects->CheckPlayValid(x, y, i, parent_card))
						return true;

				// if there is no valid target for battlecry, it also counts as not targeted
				return false;
			}
			CheckPlayValid
			{
				// Check if the target for battlecry is valid when it is targeted, if the play is untargeted (because there's no targeted play effect, or because there is no valid target, or the condition to activate fails), set z to -1
				if (isTargetedAtPlay(x, y, parent_card))
				{
					if (!effects->CheckPlayValid(x, y, z, parent_card))
					{
						#ifndef SUPPRSS_MATCH_PROMPTS
						if (!parent_card->owner->is_exploration && !parent_card->owner->is_guest)
							cout << "Error: Invalid target for the targeted battlecry, while valid target exists." << endl << endl;
						#endif
						return false;
					}
				}
				else
				{
					z = -1;
				}

				return true;
			}
			Play 
			{
				// Play card (replace leader)
				#ifndef SUPPRSS_MATCH_PROMPTS
				if (!parent_card->owner->is_exploration)
					cout << "The leader card " << parent_card->name << " played and the current leader replaced." << endl << endl;
				#endif
				delete parent_card->owner->leader;
				parent_card->owner->leader = parent_card;
				parent_card->is_first_turn_at_field = true;
				parent_card->card_pos = CARD_POS_AT_LEADER;
				parent_card->IncContribution(true);

				// Adjusting target index if necessary (the card gets played and the number of minions doesn't increase)
				if (x < z) z--;
				#ifndef SUPPRSS_MATCH_PROMPTS
				if (!parent_card->owner->is_exploration)
					parent_card->owner->PrintBoard();
				#endif

				// Trigger battlecry
				effects->Play(x, y, z, parent_card);
			}
			Discard
			{
				// Trigger On-discard Effects
				effects->Discard(parent_card);
			}
			CheckAttackValid
			{
				// Check if it can attack
				if (parent_card->n_atks_loss >= parent_card->max_n_atks)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration && !parent_card->owner->is_guest)
						cout << "Error: This leader cannot attack currently." << endl << endl;
					#endif
					return false;
				}

				// Check if it is sleeping
				if (parent_card->IsSleeping())
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration && !parent_card->owner->is_guest)
						cout << "Error: This leader cannot attack currently, please wait for next turn." << endl << endl;
					#endif
					return false;
				}

				// Check if the target is valid
				if (z <= parent_card->owner->field.size() || z > parent_card->owner->field.size() + parent_card->opponent->field.size() + 1)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration && !parent_card->owner->is_guest)
						cout << "Error: Invalid target for attacking." << endl << endl;
					#endif
					return false;
				}

				// Check for taunt and stealth ability
				Card* target = (z == parent_card->owner->field.size() + parent_card->opponent->field.size() + 1 ? parent_card->opponent->leader : parent_card->opponent->field[z - parent_card->owner->field.size() - 1]);
				if (target->is_stealth)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration && !parent_card->owner->is_guest)
						cout << "Error: Stealth characters cannot be attacked." << endl << endl;
					#endif
					return false;
				}
				if (!target->is_taunt)
				{
					for (int i = 0; i < parent_card->opponent->field.size(); i++)
					{
						if (parent_card->opponent->field[i]->is_taunt && !parent_card->opponent->field[i]->is_stealth)
						{
							#ifndef SUPPRSS_MATCH_PROMPTS
							if (!parent_card->owner->is_exploration && !parent_card->owner->is_guest)
								cout << "Error: You must attack a taunt character first." << endl << endl;
							#endif
							return false;
						}
					}
					if (parent_card->opponent->leader->is_taunt && !parent_card->opponent->leader->is_stealth)
					{
						#ifndef SUPPRSS_MATCH_PROMPTS
						if (!parent_card->owner->is_exploration && !parent_card->owner->is_guest)
							cout << "Error: You must attack a taunt character first." << endl << endl;
						#endif
						return false;
					}
				}

				return true;
			}
			Attack
			{
				Card* target = parent_card->owner->GetTargetCard(z);

				/* 
				effects->Attack(x, z, parent_card);
				if (parent_card->owner->ProcessDeferredEvents()) return;
				*/

				if (parent_card->is_stealth)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << parent_card->owner->name << "\'s " << parent_card->name << " loses stealth." << endl << endl;
					#endif
					parent_card->is_stealth = false;
				}

				// if the target or itself is already being dying before the attack, do not perform the actual attack (well, possily not in actual effect now as there's no on attack effects)
				if (parent_card->is_dying || target->is_dying) return;

				// Perform the actual attack
				#ifndef SUPPRSS_MATCH_PROMPTS
				if (!parent_card->owner->is_exploration)
					cout << parent_card->owner->name << "\'s " << parent_card->name << " attacks " << target->owner->name << "\'s " << target->name << "." << endl << endl;
				#endif
				parent_card->n_atks_loss++;								
				target->TakeDamage(atk, parent_card, true);
				if (target->card_type != LEADER_CARD)
					TakeDamage(target->atk, target, !target->is_dying);
			}
			TurnStart { effects->TurnStart(leader, parent_card); }
			TurnEnd { effects->TurnEnd(leader, parent_card); }
			Mutate
			{
				NodeRep* tmp_nullptr = nullptr;
				delete attack_times;
				effects->num_refs--;
				if (effects->num_refs <= 0)
					delete effects;

				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->card_root_decoder_to_intermediate, false);
					torch_Tensor value_tensor = ForwardValueOut(intermediate_code, encoder->card_root_decoder_value);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->card_root_decoder_hidden, false);
					attack_times = generate AttackTimes(tmp_nullptr, encoder, value_tensor.narrow(0, 3, Attack_Times_Size));
					CondConfig self_config = GetInitConfigFromCard(item);
					effects = generate SpecialEffects(self_config, min_eff_n, max_eff_n, effect_depth, false, tmp_nullptr, encoder, hidden_tensor);
				}
				else
				{
					attack_times = generate AttackTimes(tmp_nullptr, encoder, code);
					CondConfig self_config = GetInitConfigFromCard(item);
					effects = generate SpecialEffects(self_config, min_eff_n, max_eff_n, effect_depth, false, tmp_nullptr, encoder, code);
				}
			}
			SetOverheatCounts { effects->SetOverheatCounts(val); }
			SetOverheatThresholds { effects->SetOverheatThresholds(val); }
			ModOverheatThresholds { effects->ModOverheatThresholds(amount); }
			GetNodeHeuristicEval
			{
				double cost_divisor = 1.0 + (double)parent_card->mana;
				double atk_times_stat = (double)parent_card->max_n_atks;
				double atk_rating = (double)parent_card->atk/10.0;
				double atk_bonus_factor = 1.0 + atk_times_stat * atk_rating;
				double atk_bonus_factor_single = 1.0 + atk_rating;
				double hp_rating = (double)(parent_card->max_hp - parent_card->hp_loss)/40.0;
				double hp_bonus_factor = 1.0 + hp_rating;
				double hp_bonus_factor_multiple = 1.0 + atk_times_stat * hp_rating;
			
				// stats and abilities	
				double tmp_eval = atk_times_stat * atk_rating * hp_rating;
				if (parent_card->is_charge) tmp_eval *= atk_bonus_factor;
				if (parent_card->is_taunt) tmp_eval *= hp_bonus_factor;
				if (parent_card->is_stealth) tmp_eval *= atk_bonus_factor; // leader card normally shouldn't have stealth, but just in case
				if (parent_card->is_untargetable) tmp_eval *= atk_bonus_factor;
				if (parent_card->is_shielded) tmp_eval *= atk_bonus_factor_single;
				if (parent_card->is_poisonous) tmp_eval *= hp_bonus_factor_multiple;
				if (parent_card->is_lifesteal) tmp_eval *= atk_bonus_factor;
				if (parent_card->card_pos == CARD_POS_AT_HAND)
					tmp_eval /= cost_divisor * 2.0;
				else if (parent_card->card_pos == CARD_POS_AT_DECK)
					tmp_eval /= cost_divisor * 10.0;
	
				// effects
				tmp_eval += effects->GetNodeHeuristicEval(EFFECT_TIMING_DEFAULT, parent_card);
				
				return tmp_eval;
			}
			destructor
			{
				effects->num_refs--;
				if (effects->num_refs <= 0)
					delete effects;
			}
		}
	| minionCard: int cost, int attack, int health, AttackTimes* attack_times, MinionType* type, Abilities* abilities, SpecialEffects* effects
		{
			card_type = MINION_CARD;
			generator
			{
				if (rep)
				{
					cost = DenormalizeCode(rep->term_info[0], 0, 7);
					attack = DenormalizeCode(rep->term_info[1], 0, 10);
					health = DenormalizeCode(rep->term_info[2], 1, 10);
					rep++;
				}
				else if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->card_root_decoder_to_intermediate, false);
					torch_Tensor value_tensor = ForwardValueOut(intermediate_code, encoder->card_root_decoder_value);
					cost = DenormalizeCode(GetSingleValFromTensor(value_tensor.narrow(0, 0, 1)), 0, 7);
					if (cost < global_config.min_cost)
						cost = global_config.min_cost;
					else if (cost > global_config.max_cost)
						cost = global_config.max_cost;
					attack = DenormalizeCode(GetSingleValFromTensor(value_tensor.narrow(0, 1, 1)), 0, 10);
					health = DenormalizeCode(GetSingleValFromTensor(value_tensor.narrow(0, 2, 1)), 1, 10);
				}
				else
				{
					// note minion card should have at most 7 mana cost
					int min_cost = 0, max_cost = 7;
					if (min_cost < global_config.min_cost)
						min_cost = global_config.min_cost;
					if (max_cost > global_config.max_cost)
						max_cost = global_config.max_cost;
					if (max_cost < min_cost) // shouldn't really happen but just in case we want to have some token minion card be of high cost (probably not happening atm)
						max_cost = min_cost;
					cost = SampleNonLeaderCostClamped(min_cost, max_cost); // currently we only control the cost through the config
					attack = SampleDefaultStat();
					health = SampleDefaultStatClamped(1, 10);
				}
			}			
			orig_mana = mana = cost;
			orig_atk = atk = attack;
			orig_hp = max_hp = health;
			generator
			{
				CondConfig self_config = GetInitConfigFromCard(item);
				if (!rep && is_plain)
				{
					attack_times = construct singleAttack();  // attack times info is only used for constraining effects part, and here, there's no effect, so no need to update
					if (encoder)
					{
						torch_Tensor intermediate_code = UnitForward(code, encoder->card_root_decoder_to_intermediate, false);
						torch_Tensor classify_tensor = ForwardClassifyOut(intermediate_code, encoder->card_root_decoder_classify);
						torch_Tensor binary_classify_tensor = ForwardBinaryClassifyOut(intermediate_code, encoder->card_root_decoder_binary_classify);
						type = generate MinionType(rep, encoder, classify_tensor.narrow(0, 3, Minion_Type_Size));
						abilities = generate Abilities(self_config, atk, rep, encoder, binary_classify_tensor);
					}
					else
					{
						type = generate MinionType(rep, encoder, code);
						abilities = generate Abilities(self_config, atk, rep, encoder, code);
					}
					effects = construct specialEffects(noTargetedPlayEff(), noOtherEffs());
				}
				else 
				{
					if (encoder)
					{
						torch_Tensor intermediate_code = UnitForward(code, encoder->card_root_decoder_to_intermediate, false);
						torch_Tensor classify_tensor = ForwardClassifyOut(intermediate_code, encoder->card_root_decoder_classify);
						torch_Tensor binary_classify_tensor = ForwardBinaryClassifyOut(intermediate_code, encoder->card_root_decoder_binary_classify);
						torch_Tensor value_tensor = ForwardValueOut(intermediate_code, encoder->card_root_decoder_value);
						torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->card_root_decoder_hidden, false);
						attack_times = generate AttackTimes(rep, encoder, value_tensor.narrow(0, 3, Attack_Times_Size));
						self_config = GetInitConfigFromCard(item); // update attack times info
						type = generate MinionType(rep, encoder, classify_tensor.narrow(0, 3, Minion_Type_Size));
						abilities = generate Abilities(self_config, atk, rep, encoder, binary_classify_tensor);
						effects = generate SpecialEffects(self_config, 0, max_eff_num, 0, false, rep, encoder, hidden_tensor);
					}
					else
					{
						attack_times = generate AttackTimes(rep, encoder, code);
						self_config = GetInitConfigFromCard(item); // update attack times info
						type = generate MinionType(rep, encoder, code);
						abilities = generate Abilities(self_config, atk, rep, encoder, code);
						effects = generate SpecialEffects(self_config, 0, max_eff_num, 0, false, rep, encoder, code);
					}
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_MINION_CARD, <double>{NormalizeCode(cost, 0, 7), NormalizeCode(attack, 0, 10), NormalizeCode(health, 1, 10)}));
				attack_times->FillRep(rep);
				type->FillRep(rep);
				abilities->FillRep(rep);
				effects->FillRep(rep);
			}
			FillTokens
			{
				if (max_n_atks != 1)
				{
					if (need_starting_newline)
						tokens.push_back(mkNewLineToken());
					tokens.push_back(mkWordToken("Attacks", indent_size, true));
					tokens.push_back(mkWordToken(IntToStr(max_n_atks), indent_size, true));
					tokens.push_back(mkWordToken("Times", indent_size, true));
					tokens.push_back(mkPunctToken(".", indent_size, true));
					need_starting_newline = 1;	
				}
				AbilityFillTokens(is_charge, is_taunt, is_stealth, is_untargetable, is_shielded, is_poisonous, is_lifesteal, tokens, indent_size, need_starting_newline);
				effects->FillTokens(tokens, indent_size, need_starting_newline);
			}
			Brief
			{
				return IntToStr(mana) + "/" + IntToStr(atk) + "/" + IntToStr(max_hp - hp_loss) + ", " + IntToStr(max_n_atks - n_atks_loss) + "Attacks" + (IsSleeping() ? "(Sleeping)" : "")
				+ ", " + MinionTypeDescription(minion_type) + ". " + abilities->Brief() + " " + effects->Brief() + GetExtraEffectsBrief();
			}
			Detail
			{
				string mana_info = "MANA: " + IntToStr(mana) + " (originally " + IntToStr(orig_mana) + ").";
				string atk_info = "ATK: " + IntToStr(atk) + " (originally " + IntToStr(orig_atk) + ").";
				string hp_info = "HP: " + IntToStr(max_hp - hp_loss) + " of " + IntToStr(max_hp) + " (originally " + IntToStr(orig_hp) + ").";
				string atk_times_info = "ATK TIMES: " + IntToStr(max_n_atks - n_atks_loss) + " of " + IntToStr(max_n_atks) + " (originally " + IntToStr(orig_n_atks) + ")";
				string att_times_explain = "";
				if (max_n_atks == 0)
					att_times_explain = " (Cannot attack)";
				else if (max_n_atks > 1)
					att_times_explain = " (Can attack " + IntToStr(max_n_atks) + " times per turn)";
				if (IsSleeping())
					att_times_explain += "(Sleeping).";
				else
					att_times_explain += ".";
				string type_info = "TYPE: " + MinionTypeDescription(minion_type) + " (Originally " + MinionTypeDescription(orig_minion_type) + ") Minion."; // note the type may be modified in place
				string abil_info = abilities->Detail();
				string effect_info = effects->Detail();
				string extra_effect_info = GetExtraEffectsDetail();
				return mana_info + "\n" + atk_info + "\n" + hp_info + "\n" + atk_times_info + att_times_explain + "\n" + type_info + "\n" + abil_info + effect_info + extra_effect_info;
			}
			DetailIndent
			{
				string mana_info = RepeatSpace(indent_size) + "MANA: " + IntToStr(mana) + " (originally " + IntToStr(orig_mana) + ").";
				string atk_info = RepeatSpace(indent_size) + "ATK: " + IntToStr(atk) + " (originally " + IntToStr(orig_atk) + ").";
				string hp_info = RepeatSpace(indent_size) + "HP: " + IntToStr(max_hp - hp_loss) + " of " + IntToStr(max_hp) + " (originally " + IntToStr(orig_hp) + ").";
				string atk_times_info = RepeatSpace(indent_size) + "ATK TIMES: " + IntToStr(max_n_atks - n_atks_loss) + " of " + IntToStr(max_n_atks) + " (originally " + IntToStr(orig_n_atks) + ")";
				string att_times_explain = "";
				if (max_n_atks == 0)
					att_times_explain = " (Cannot attack)";
				else if (max_n_atks > 1)
					att_times_explain = " (Can attack " + IntToStr(max_n_atks) + " times per turn)";
				if (IsSleeping())
					att_times_explain += "(Sleeping).";
				else
					att_times_explain += ".";
				string type_info = RepeatSpace(indent_size) + "TYPE: " + MinionTypeDescription(minion_type) + " (Originally " + MinionTypeDescription(orig_minion_type) + ") Minion."; // note the type may be modified in place
				string abil_info = abilities->DetailIndent(indent_size);
				string effect_info = effects->DetailIndent(indent_size);
				string extra_effect_info = GetExtraEffectsDetailIndent(indent_size);
				return mana_info + "\n" + atk_info + "\n" + hp_info + "\n" + atk_times_info + att_times_explain + "\n" + type_info + "\n" + abil_info + effect_info + extra_effect_info;
			}
			CreateNodeHardCopy = new minionCard(card_copy, cost, attack, health, 
				(AttackTimes*)(attack_times->CreateNodeHardCopy(card_copy, redir_map)),
				(MinionType*)(type->CreateNodeHardCopy(card_copy, redir_map)),
				(Abilities*)(abilities->CreateNodeHardCopy(card_copy, redir_map)),
				(SpecialEffects*)(effects->CreateNodeHardCopy(card_copy, redir_map)));
			GetEffects = effects;
			isTargetedAtPlay
			{
				if (!effects->isTargetedAtPlay(x, y, parent_card))
					return false;

				// check if there is a valid target for battlecry
				for (int i = 0; i <= parent_card->owner->field.size() + parent_card->opponent->field.size() + parent_card->owner->hand.size() + 1; i++)
					if (x != i && effects->CheckPlayValid(x, y, i, parent_card))
						return true;

				// if there is no valid target for battlecry, it also counts as not targeted
				return false;
			}
			CheckPlayValid
			{
				// Check if the field is full
				if (parent_card->owner->GetActualFieldSize() >= MAX_FIELD_SIZE)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration && !parent_card->owner->is_guest)
						cout << "Error: Your field is full." << endl << endl;
					#endif
					return false;
				}

				// Check if the position is valid
				if (y < 0 || y > parent_card->owner->field.size())
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration && !parent_card->owner->is_guest)
						cout << "Error: Invalid position." << endl << endl;
					#endif
					return false;
				}

				// Check if the target for battlecry is valid when it is targeted, if the play is untargeted (because there's no targeted play effect, or because there is no valid target, or the condition to activate fails), set z to -1
				if (isTargetedAtPlay(x, y, parent_card))
				{
					if (!effects->CheckPlayValid(x, y, z, parent_card))
					{
						#ifndef SUPPRSS_MATCH_PROMPTS
						if (!parent_card->owner->is_exploration && !parent_card->owner->is_guest)
							cout << "Error: Invalid target for the targeted battlecry, while valid target exists." << endl << endl;
						#endif
						return false;
					}
				}
				else
				{
					z = -1;
				}

				return true;
			}
			Play 
			{
				// Play card (place minion)
				#ifndef SUPPRSS_MATCH_PROMPTS
				if (!parent_card->owner->is_exploration)
					cout << "The minion card " << parent_card->name << " played." << endl << endl;
				#endif
				owner->field.insert(parent_card->owner->field.begin() + y, parent_card);
				parent_card->is_first_turn_at_field = true;
				parent_card->card_pos = CARD_POS_AT_FIELD;
				parent_card->IncContribution(true);

				// Adjusting target index if necessary (the card gets played and the minion is inserted)
				if (y < z && z < x) z++;
				#ifndef SUPPRSS_MATCH_PROMPTS
				if (!parent_card->owner->is_exploration)
					parent_card->owner->PrintBoard();
				#endif

				// Trigger battlecry
				effects->Play(x, y, z, parent_card);
			}
			Destroy
			{
				// Trigger Deathrattle
				effects->Destroy(parent_card);
			}
			Discard
			{
				// Trigger On-discard Effects
				effects->Discard(parent_card);
			}
			CheckAttackValid
			{
				// Check if it can attack
				if (parent_card->n_atks_loss >= parent_card->max_n_atks)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration && !parent_card->owner->is_guest)
						cout << "Error: This minion cannot attack currently." << endl << endl;
					#endif
					return false;
				}

				// Check if it is sleeping
				if (parent_card->IsSleeping())
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration && !parent_card->owner->is_guest)
						cout << "Error: This minion cannot attack currently, please wait for next turn." << endl << endl;
					#endif
					return false;
				}

				// Check if the target is valid
				if (z <= parent_card->owner->field.size() || z > parent_card->owner->field.size() + parent_card->opponent->field.size() + 1)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration && !parent_card->owner->is_guest)
						cout << "Error: Invalid target for attacking." << endl << endl;
					#endif
					return false;
				}

				// Check for taunt and stealth ability
				Card* target = (z == parent_card->owner->field.size() + parent_card->opponent->field.size() + 1 ? parent_card->opponent->leader : parent_card->opponent->field[z - parent_card->owner->field.size() - 1]);
				if (target->is_stealth)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration && !parent_card->owner->is_guest)
						cout << "Error: Stealth characters cannot be attacked." << endl << endl;
					#endif
					return false;
				}
				if (!target->is_taunt)
				{
					for (int i = 0; i < parent_card->opponent->field.size(); i++)
					{
						if (parent_card->opponent->field[i]->is_taunt && !parent_card->opponent->field[i]->is_stealth)
						{
							#ifndef SUPPRSS_MATCH_PROMPTS
							if (!parent_card->owner->is_exploration && !parent_card->owner->is_guest)
								cout << "Error: You must attack a taunt character first." << endl << endl;
							#endif
							return false;
						}
					}
					if (parent_card->opponent->leader->is_taunt && !parent_card->opponent->leader->is_stealth)
					{
						#ifndef SUPPRSS_MATCH_PROMPTS
						if (!parent_card->owner->is_exploration && !parent_card->owner->is_guest)
							cout << "Error: You must attack a taunt character first." << endl << endl;
						#endif
						return false;
					}
				}

				return true;
			}
			Attack
			{
				Card* target = parent_card->owner->GetTargetCard(z);

				/* 
				effects->Attack(x, z, parent_card);
				if (parent_card->owner->ProcessDeferredEvents()) return;
				*/

				if (parent_card->is_stealth)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << parent_card->owner->name << "\'s " << parent_card->name << " loses stealth." << endl << endl;
					#endif
					parent_card->is_stealth = false;
				}

				// if the target or itself is already dying before the attack, do not perform the actual attack (well, possily not in actual effect now as there's no on attack effects)
				if (parent_card->is_dying || target->is_dying) return;

				// Perform the actual attack
				#ifndef SUPPRSS_MATCH_PROMPTS
				if (!parent_card->owner->is_exploration)
					cout << parent_card->owner->name << "\'s " << parent_card->name << " attacks " << target->owner->name << "\'s " << target->name << "." << endl << endl;
				#endif
				parent_card->n_atks_loss++;								
				target->TakeDamage(atk, parent_card, true);
				if (target->card_type != LEADER_CARD)
					TakeDamage(target->atk, target, !target->is_dying);
			}
			TurnStart { effects->TurnStart(leader, parent_card); }
			TurnEnd { effects->TurnEnd(leader, parent_card); }
			Mutate
			{
				NodeRep* tmp_nullptr = nullptr;
				delete attack_times;
				effects->num_refs--;
				if (effects->num_refs <= 0)
					delete effects;

				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->card_root_decoder_to_intermediate, false);
					torch_Tensor value_tensor = ForwardValueOut(intermediate_code, encoder->card_root_decoder_value);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->card_root_decoder_hidden, false);
					attack_times = generate AttackTimes(tmp_nullptr, encoder, value_tensor.narrow(0, 3, Attack_Times_Size));
					CondConfig self_config = GetInitConfigFromCard(item);
					effects = generate SpecialEffects(self_config, min_eff_n, max_eff_n, effect_depth, false, tmp_nullptr, encoder, hidden_tensor);
				}
				else
				{
					attack_times = generate AttackTimes(tmp_nullptr, encoder, code);
					CondConfig self_config = GetInitConfigFromCard(item);
					effects = generate SpecialEffects(self_config, min_eff_n, max_eff_n, effect_depth, false, tmp_nullptr, encoder, code);
				}
			}
			SetOverheatCounts { effects->SetOverheatCounts(val); }
			SetOverheatThresholds { effects->SetOverheatThresholds(val); }
			ModOverheatThresholds { effects->ModOverheatThresholds(amount); }
			GetNodeHeuristicEval
			{
				double cost_divisor = 1.0 + (double)parent_card->mana;
				double atk_times_stat = (double)parent_card->max_n_atks;
				double atk_rating = (double)parent_card->atk/10.0;
				double atk_bonus_factor = 1.0 + atk_times_stat * atk_rating;
				double atk_bonus_factor_single = 1.0 + atk_rating;
				double hp_rating = (double)(parent_card->max_hp - parent_card->hp_loss)/10.0;
				double hp_bonus_factor = 1.0 + hp_rating;
				double hp_bonus_factor_multiple = 1.0 + atk_times_stat * hp_rating;
			
				// stats and abilities	
				double tmp_eval = atk_times_stat * atk_rating * hp_rating;
				if (parent_card->is_charge) tmp_eval *= atk_bonus_factor;
				if (parent_card->is_taunt) tmp_eval *= hp_bonus_factor;
				if (parent_card->is_stealth) tmp_eval *= atk_bonus_factor; // leader card normally shouldn't have stealth, but just in case
				if (parent_card->is_untargetable) tmp_eval *= atk_bonus_factor;
				if (parent_card->is_shielded) tmp_eval *= atk_bonus_factor_single;
				if (parent_card->is_poisonous) tmp_eval *= hp_bonus_factor_multiple;
				if (parent_card->is_lifesteal) tmp_eval *= atk_bonus_factor;
				if (parent_card->card_pos == CARD_POS_AT_HAND)
					tmp_eval /= cost_divisor * 2.0;
				else if (parent_card->card_pos == CARD_POS_AT_DECK)
					tmp_eval /= cost_divisor * 10.0;

				// effects
				tmp_eval += effects->GetNodeHeuristicEval(EFFECT_TIMING_DEFAULT, parent_card);
				
				return tmp_eval;
			}
			destructor
			{
				effects->num_refs--;
				if (effects->num_refs <= 0)
					delete effects;
			}
		}
	| spellCard: int cost, Abilities* abilities, SpecialEffects* effects
		{
			card_type = SPELL_CARD;
			generator
			{
				if (rep)
				{
					cost = DenormalizeCode(rep->term_info[0], 0, 7);
					rep++;
				}
				else if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->card_root_decoder_to_intermediate, false);
					torch_Tensor value_tensor = ForwardValueOut(intermediate_code, encoder->card_root_decoder_value);
					cost = DenormalizeCode(GetSingleValFromTensor(value_tensor.narrow(0, 0, 1)), 0, 7);
					if (cost < global_config.min_cost)
						cost = global_config.min_cost;
					else if (cost > global_config.max_cost)
						cost = global_config.max_cost;
				}
				else
				{
					// note spell card should have at most 7 mana cost
					int min_cost = 0, max_cost = 7;
					if (min_cost < global_config.min_cost)
						min_cost = global_config.min_cost;
					if (max_cost > global_config.max_cost)
						max_cost = global_config.max_cost;
					if (max_cost < min_cost) // shouldn't really happen but just in case we want to have some token spell card be of high cost (probably not happening atm)
						max_cost = min_cost;
					cost = SampleNonLeaderCostClamped(min_cost, max_cost); // currently we only control the cost through the config
				}
			}
			orig_mana = mana = cost;
			generator
			{
				CondConfig self_config = GetInitConfigFromCard(item);
				if (!rep && is_plain)
				{
					if (encoder)
					{
						torch_Tensor intermediate_code = UnitForward(code, encoder->card_root_decoder_to_intermediate, false);
						torch_Tensor binary_classify_tensor = ForwardBinaryClassifyOut(intermediate_code, encoder->card_root_decoder_binary_classify);
						abilities = generate Abilities(self_config, -1, rep, encoder, binary_classify_tensor);
					}
					else
					{
						abilities = generate Abilities(self_config, -1, rep, encoder, code); // -1 means not applicable
					}
					effects = construct specialEffects(noTargetedPlayEff(), noOtherEffs());
				}
				else 
				{
					if (encoder)
					{
						torch_Tensor intermediate_code = UnitForward(code, encoder->card_root_decoder_to_intermediate, false);
						torch_Tensor binary_classify_tensor = ForwardBinaryClassifyOut(intermediate_code, encoder->card_root_decoder_binary_classify);
						torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->card_root_decoder_hidden, false);
						abilities = generate Abilities(self_config, -1, rep, encoder, binary_classify_tensor);
						effects = generate SpecialEffects(self_config, 1, max_eff_num, 0, false, rep, encoder, hidden_tensor);
					}
					else
					{
						abilities = generate Abilities(self_config, -1, rep, encoder, code);
						effects = generate SpecialEffects(self_config, 1, max_eff_num, 0, false, rep, encoder, code);
					}
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_SPELL_CARD, <double>{NormalizeCode(cost, 0, 7)}));
				abilities->FillRep(rep);
				effects->FillRep(rep);
			}
			FillTokens
			{
				AbilityFillTokens(is_charge, is_taunt, is_stealth, is_untargetable, is_shielded, is_poisonous, is_lifesteal, tokens, indent_size, need_starting_newline);
				effects->FillTokens(tokens, indent_size, need_starting_newline);
			}
			Brief
			{
				return IntToStr(mana) + " Mana Spell. " + abilities->Brief() + " " + effects->Brief() + GetExtraEffectsBrief();
			}
			Detail
			{
				string mana_info = "MANA: " + IntToStr(mana) + " (originally " + IntToStr(orig_mana) + ")";
				string type_info = "TYPE: Spell.";
				string abil_info = abilities->Detail();
				string effect_info = effects->Detail();
				string extra_effect_info = GetExtraEffectsDetail();
				return mana_info + "\n" + type_info + "\n" + abil_info + effect_info + extra_effect_info;
			}
			DetailIndent
			{
				string mana_info = RepeatSpace(indent_size) + "MANA: " + IntToStr(mana) + " (originally " + IntToStr(orig_mana) + ")";
				string type_info = RepeatSpace(indent_size) + "TYPE: Spell.";
				string abil_info = abilities->DetailIndent(indent_size);
				string effect_info = effects->DetailIndent(indent_size);
				string extra_effect_info = GetExtraEffectsDetailIndent(indent_size);
				return mana_info + "\n" + type_info + "\n" + abil_info + effect_info + extra_effect_info;
			}
			CreateNodeHardCopy = new spellCard(card_copy, cost,
				(Abilities*)(abilities->CreateNodeHardCopy(card_copy, redir_map)),
				(SpecialEffects*)(effects->CreateNodeHardCopy(card_copy, redir_map)));
			GetEffects = effects;
			isTargetedAtPlay
			{
				return effects->isTargetedAtPlay(x, y, parent_card);
			}
			CheckPlayValid
			{
				// Check if the target for the spell is valid, if not return false
				if (isTargetedAtPlay(x, y, parent_card) && !effects->CheckPlayValid(x, y, z, parent_card))
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration && !parent_card->owner->is_guest)
						cout << "Error: Invalid target for the targeted spell." << endl << endl;
					#endif
					return false;
				}

				return true;
			}
			Play
			{
				// Play card (play spell)
				#ifndef SUPPRSS_MATCH_PROMPTS
				if (!parent_card->owner->is_exploration)
					cout << "The spell card " << parent_card->name << " played." << endl << endl;
				#endif

				// Mark the card for deletion
				parent_card->owner->FlagCastSpell(parent_card, true);

				// Trigger effects
				effects->Play(x, y, z, parent_card);
			}
			Discard
			{
				// Trigger On-discard Effects
				effects->Discard(parent_card);
			}
			TurnStart { effects->TurnStart(leader, parent_card); }
			TurnEnd { effects->TurnEnd(leader, parent_card); }
			Mutate
			{
				NodeRep* tmp_nullptr = nullptr;
				effects->num_refs--;
				if (effects->num_refs <= 0)
					delete effects;
				CondConfig self_config = GetInitConfigFromCard(item);

				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->card_root_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->card_root_decoder_hidden, false);
					effects = generate SpecialEffects(self_config, (min_eff_n > 1 ? min_eff_n : 1), max_eff_n, effect_depth, false, tmp_nullptr, encoder, hidden_tensor); // spell has to at least have one effect
				}
				else
				{
					effects = generate SpecialEffects(self_config, (min_eff_n > 1 ? min_eff_n : 1), max_eff_n, effect_depth, false, tmp_nullptr, encoder, code); // spell has to at least have one effect
				}
			}
			SetOverheatCounts { effects->SetOverheatCounts(val); }
			SetOverheatThresholds { effects->SetOverheatThresholds(val); }
			ModOverheatThresholds { effects->ModOverheatThresholds(amount); }
			GetNodeHeuristicEval
			{
				// effects
				double tmp_eval = effects->GetNodeHeuristicEval(EFFECT_TIMING_DEFAULT, parent_card);

				// spell being untargetable is usually a bad thing
				if (parent_card->is_untargetable) tmp_eval /= 1.1;

				return tmp_eval;
			}
			destructor
			{
				effects->num_refs--;
				if (effects->num_refs <= 0)
					delete effects;
			}
		}
		
	AttackTimes
	default:
		{
			preselector
			{
				if (rep)
				{
					force [rep->choice];
				}
				else if (encoder)
				{
					// special: use the value to determine with category to choose (might as well just construct as there's no other parameters)
					int n_atks_value = DenormalizeCode(GetSingleValFromTensor(code), 0, 5);
					if (n_atks_value < 1)
						return construct zeroAttack();
					if (n_atks_value == 1)
						return construct singleAttack();
					return construct multipleAttack(n_atks_value);
				}
			}
		}
	:=
	| zeroAttack:
		{
			generator { if (rep) rep++; }
			orig_n_atks = max_n_atks = 0;
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_ZERO_ATTACK));
			}
			CreateNodeHardCopy = new zeroAttack(card_copy);
		}
	| singleAttack:
		{
			generator { if (rep) rep++; }
			orig_n_atks = max_n_atks = 1;
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_SINGLE_ATTACK));
			}
			CreateNodeHardCopy = new singleAttack(card_copy);
		}
	| multipleAttack: int n_atks
		{
			generator
			{ 
				if (rep)
				{
					n_atks = DenormalizeCode(rep->term_info[0], 0, 5); // note the code is normalized against 0 ~ 5 range
					rep++;
				}
				else
				{
					n_atks = SampleRangeLowerConcentratedPolynomialFromInt(2, 5, 8.0);
				}
			}
			n_atks_loss = 0;
			orig_n_atks = max_n_atks = n_atks;
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_MULTIPLE_ATTACK, <double>{NormalizeCode(n_atks, 0, 5)})); // not using the range of 2 ~ 5 for normalization for more consistent encoding considering the recursive nn version
			}
			CreateNodeHardCopy = new multipleAttack(card_copy, n_atks);
		}

	MinionType
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else if (encoder)
				{
					force [GetMaxIndex(code)];
				}
			}
		}
	:=
	| beastMinion:
		{
			orig_minion_type = minion_type = BEAST_MINION;
			FillRep
			{
				rep.push_back(mkNodeRep(0u));
			}
			CreateNodeHardCopy = new beastMinion(card_copy);
		}
	| dragonMinion:
		{
			orig_minion_type = minion_type = DRAGON_MINION;
			FillRep
			{
				rep.push_back(mkNodeRep(1u));
			}
			CreateNodeHardCopy = new dragonMinion(card_copy);
		}
	| demonMinion:
		{
			orig_minion_type = minion_type = DEMON_MINION;
			FillRep
			{
				rep.push_back(mkNodeRep(2u));
			}
			CreateNodeHardCopy = new demonMinion(card_copy);
		}

	Abilities :=
	| justAbilities: ChargeAbil* c, TauntAbil* t, StealthAbil* s, UntargetableAbil* u, ShieldAbil* d, PoisonousAbil* p, LifestealAbil* l
		{
			generator
			{
				if (encoder)
				{
					c = generate ChargeAbil(self_config, rep, encoder, code.narrow(0, 0, 1));
					t = generate TauntAbil(self_config, rep, encoder, code.narrow(0, 1, 1));
					s = generate StealthAbil(self_config, rep, encoder, code.narrow(0, 2, 1));
					u = generate UntargetableAbil(self_config, rep, encoder, code.narrow(0, 3, 1));
					d = generate ShieldAbil(self_config, rep, encoder, code.narrow(0, 4, 1));
					p = generate PoisonousAbil(self_config, damage, rep, encoder, code.narrow(0, 5, 1));
					l = generate LifestealAbil(self_config, damage, rep, encoder, code.narrow(0, 6, 1));
				}
				else
				{
					c = generate ChargeAbil(self_config, rep, encoder, code);
					t = generate TauntAbil(self_config, rep, encoder, code);
					s = generate StealthAbil(self_config, rep, encoder, code);
					u = generate UntargetableAbil(self_config, rep, encoder, code);
					d = generate ShieldAbil(self_config, rep, encoder, code);
					p = generate PoisonousAbil(self_config, damage, rep, encoder, code);
					l = generate LifestealAbil(self_config, damage, rep, encoder, code);
				}
			}
			FillRep
			{
				// this is just a pass-down node
				c->FillRep(rep);
				t->FillRep(rep);
				s->FillRep(rep);
				u->FillRep(rep);
				d->FillRep(rep);
				p->FillRep(rep);
				l->FillRep(rep);
			}
			Brief = AbilityDescriptionBrief(is_charge, is_taunt, is_stealth, is_untargetable, is_shielded, is_poisonous, is_lifesteal);
			Detail
			{
				string cur_info = AbilityDescriptionDetail(is_charge, is_taunt, is_stealth, is_untargetable, is_shielded, is_poisonous, is_lifesteal, 0); // note the abilities may be modified in place
				string orig_info = AbilityDescriptionBrief(orig_is_charge, orig_is_taunt, orig_is_stealth, orig_is_untargetable, orig_is_shielded, orig_is_poisonous, orig_is_lifesteal);
				if (cur_info.length() == 0 && orig_info.length() == 0)
					return "";
				return (cur_info.length() == 0 ? "No Ability " : cur_info) + "(Originally " + (orig_info.length() == 0 ? "No Ability" : orig_info) + ")\n";
			}
			DetailIndent
			{
				string cur_info = AbilityDescriptionDetail(is_charge, is_taunt, is_stealth, is_untargetable, is_shielded, is_poisonous, is_lifesteal, indent_size); // note the abilities may be modified in place
				string orig_info = AbilityDescriptionBrief(orig_is_charge, orig_is_taunt, orig_is_stealth, orig_is_untargetable, orig_is_shielded, orig_is_poisonous, orig_is_lifesteal);
				if (cur_info.length() == 0 && orig_info.length() == 0)
					return "";
				return RepeatSpace(indent_size) + (cur_info.length() == 0 ? "No Ability " : cur_info) + "(Originally " + (orig_info.length() == 0 ? "No Ability" : orig_info) + ")\n";
			}
			CreateNodeHardCopy = new justAbilities(card_copy,
				(ChargeAbil*)(c->CreateNodeHardCopy(card_copy, redir_map)),
				(TauntAbil*)(t->CreateNodeHardCopy(card_copy, redir_map)),
				(StealthAbil*)(s->CreateNodeHardCopy(card_copy, redir_map)),
				(UntargetableAbil*)(u->CreateNodeHardCopy(card_copy, redir_map)),
				(ShieldAbil*)(d->CreateNodeHardCopy(card_copy, redir_map)),
				(PoisonousAbil*)(p->CreateNodeHardCopy(card_copy, redir_map)),
				(LifestealAbil*)(l->CreateNodeHardCopy(card_copy, redir_map)));
			GetInitAbilFlag = c->GetInitAbilFlag() | t->GetInitAbilFlag() | s->GetInitAbilFlag() | u->GetInitAbilFlag() | d->GetInitAbilFlag() | p->GetInitAbilFlag() | l->GetInitAbilFlag(); // currently not used
		}

	DamageAbilities := // this doesn't need description, just contributing to abilities
	| damageAbilities: PoisonousAbil* p, LifestealAbil* l
		{
			generator
			{
				if (!rep && (target_mode == TARGET_MODE_LEADER || (target_mode == TARGET_MODE_SELF && !(self_config & TARGET_NOT_LEADER)))) // damage towards leader shouldn't have poisonous abilities
					p = construct noPoisonous();
				else if (encoder)
					p = generate PoisonousAbil(self_config, damage, rep, encoder, code.narrow(0, 0, 1));
				else
					p = generate PoisonousAbil(self_config, damage, rep, encoder, code);

				if (encoder)
					l = generate LifestealAbil(self_config, damage, rep, encoder, code.narrow(0, 1, 1));
				else
					l = generate LifestealAbil(self_config, damage, rep, encoder, code);
			}
			FillRep
			{
				// this is just a pass-down node
				p->FillRep(rep);
				l->FillRep(rep);
			}
			CreateNodeHardCopy = new damageAbilities(card_copy,
				(PoisonousAbil*)(p->CreateNodeHardCopy(card_copy, redir_map)),
				(LifestealAbil*)(l->CreateNodeHardCopy(card_copy, redir_map)));
			GetInitAbilFlag = p->GetInitAbilFlag() | l->GetInitAbilFlag();
		}

	ChargeAbil
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else
				{
					if (self_config & TARGET_IS_SPELL)
						force noCharge;
					else if (encoder)
						force [GetBinaryIndex(code)];
				}
			}
		}
	:=
	| noCharge: 
		{
			FillRep
			{
				rep.push_back(mkNodeRep(0u));
			}
			CreateNodeHardCopy = new noCharge(card_copy);
		}
	| justCharge:
		{ 
			orig_is_charge = is_charge = true;
			generator { self_config |= TARGET_IS_CHARGE; }
			FillRep
			{
				rep.push_back(mkNodeRep(1u));
			}
			CreateNodeHardCopy = new justCharge(card_copy);
			GetInitAbilFlag = TARGET_IS_CHARGE;
		}
		
	TauntAbil
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else
				{
					if (self_config & TARGET_IS_SPELL)
						force noTaunt;
					else if (encoder)
						force [GetBinaryIndex(code)];
				}
			}
		}
	:=
	| noTaunt:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(0u));
			}
			CreateNodeHardCopy = new noTaunt(card_copy);
		}
	| justTaunt: 
		{
			orig_is_taunt = is_taunt = true;
			generator { self_config |= TARGET_IS_TAUNT; }
			FillRep
			{
				rep.push_back(mkNodeRep(1u));
			}
			CreateNodeHardCopy = new justTaunt(card_copy);
			GetInitAbilFlag = TARGET_IS_TAUNT;
		}

	StealthAbil
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else
				{
					if (self_config & (TARGET_IS_LEADER | TARGET_IS_SPELL)) // leaders never get generated with stealth (otherwise too OP)
						force noStealth;
					else if (encoder)
						force [GetBinaryIndex(code)];
				}
			}
		}
	:=
	| noStealth:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(0u));
			}
			CreateNodeHardCopy = new noStealth(card_copy);
		}
	| justStealth:
		{
			orig_is_stealth = is_stealth = true;
			generator { self_config |= TARGET_IS_STEALTH; }
			FillRep
			{
				rep.push_back(mkNodeRep(1u));
			}
			CreateNodeHardCopy = new justStealth(card_copy);
			GetInitAbilFlag = TARGET_IS_STEALTH;
		}

	UntargetableAbil
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else if (encoder)
				{
					force [GetBinaryIndex(code)];
				}
			}
		}
	:=
	| noUntargetable:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(0u));
			}
			CreateNodeHardCopy = new noUntargetable(card_copy);
		}
	| justUntargetable:
		{
			orig_is_untargetable = is_untargetable = true;
			generator { self_config |= TARGET_IS_UNTARGETABLE; }
			FillRep
			{
				rep.push_back(mkNodeRep(1u));
			}
			CreateNodeHardCopy = new justUntargetable(card_copy);
			GetInitAbilFlag = TARGET_IS_UNTARGETABLE;
		}

	ShieldAbil
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else
				{
					if (self_config & TARGET_IS_SPELL)
						force noShield;
					else if (encoder)
						force [GetBinaryIndex(code)];
				}
			}
		}
	:=
	| noShield:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(0u));
			}
			CreateNodeHardCopy = new noShield(card_copy);
		}
	| justShield:
		{
			orig_is_shielded = is_shielded = true;
			generator { self_config |= TARGET_IS_SHIELDED; }
			FillRep
			{
				rep.push_back(mkNodeRep(1u));
			}
			CreateNodeHardCopy = new justShield(card_copy);
			GetInitAbilFlag = TARGET_IS_SHIELDED;
		}

	PoisonousAbil
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else
				{
					if (damage < 0 || (self_config & TARGET_IS_POISONOUS)) // negative damage means not applicable; the other condition applies to two cases (mainly for the latter one): 1. it already had the ability, so it doesn't matter; 2. previous effect has source condition of it, so to avoid making it trivial, don't add the ability
						force noPoisonous;
					else if (encoder)
						force [GetBinaryIndex(code)];
				}
			}
		}
	:=
	| noPoisonous:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(0u));
			}
			CreateNodeHardCopy = new noPoisonous(card_copy);
		}
	| justPoisonous:
		{
			orig_is_poisonous = is_poisonous = true;
			generator { self_config |= TARGET_IS_POISONOUS; }
			FillRep
			{
				rep.push_back(mkNodeRep(1u));
			}
			CreateNodeHardCopy = new justPoisonous(card_copy);
			GetInitAbilFlag = TARGET_IS_POISONOUS;
		}

	LifestealAbil
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else
				{
					if (damage < 0 || (self_config & TARGET_IS_LIFESTEAL)) // negative damage means not applicable; the other condition applies to two cases (mainly for the latter one): 1. it already had the ability, so it doesn't matter; 2. previous effect has source condition of it, so to avoid making it trivial, don't add the ability
						force noLifesteal;
					else if (encoder)
						force [GetBinaryIndex(code)];
				}
			}
		}
	:=
	| noLifesteal:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(0u));
			}
			CreateNodeHardCopy = new noLifesteal(card_copy);
		}
	| justLifesteal:
		{ 
			orig_is_lifesteal = is_lifesteal = true;
			generator { self_config |= TARGET_IS_LIFESTEAL; }
			FillRep
			{
				rep.push_back(mkNodeRep(1u));
			}
			CreateNodeHardCopy = new justLifesteal(card_copy);
			GetInitAbilFlag = TARGET_IS_LIFESTEAL;
		}

	SpecialEffects
	default:
		{
			pregencontor
			{
				num_refs = 1;
			}
		}
	:=
	| specialEffects: TargetedPlayEff* effect, OtherEffs* effects // only one targeted effect per category is allowed, 
		{
			generator
			{
				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->special_effects_decoder_to_intermediate, false);
					torch_Tensor classify_tensor = ForwardClassifyOut(intermediate_code, encoder->special_effects_decoder_classify);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->special_effects_decoder_hidden, false);
					torch_Tensor tmp_cat_list[] = {classify_tensor, hidden_tensor.narrow(0, 0, Single_Effect_Size)};
					effect = generate TargetedPlayEff(self_config, effect_depth, give_eff, rep, encoder, torch_cat(tmp_cat_list));
					int l_num = effect->GetEffectNum(); // note implicit child generation is added before these
					self_config &= ExtractEffectIndependentConfig(effect->GetGlobalSelfConfig(self_config, EFFECT_TIMING_DEFAULT));
					effects = generate OtherEffs(self_config, min_n - l_num, max_n - l_num, effect_depth, give_eff, rep, encoder, hidden_tensor.narrow(0, Single_Effect_Size, Effect_Seq_Size));
				}
				else
				{
					effect = generate TargetedPlayEff(self_config, effect_depth, give_eff, rep, encoder, code);
					int l_num = effect->GetEffectNum(); // note implicit child generation is added before these
					self_config &= ExtractEffectIndependentConfig(effect->GetGlobalSelfConfig(self_config, EFFECT_TIMING_DEFAULT));
					effects = generate OtherEffs(self_config, min_n - l_num, max_n - l_num, effect_depth, give_eff, rep, encoder, code);
				}
			}
			FillRep
			{
				// this is just a pass-down node
				effect->FillRep(rep);
				effects->FillRep(rep);
			}
			FillTokens
			{
				effect->FillTokens(tokens, indent_size, need_starting_newline);
				effects->FillTokens(tokens, indent_size, need_starting_newline);	
			}
			Brief = effect->Brief() + effects->Brief();
			Detail = effect->Detail() + effects->Detail();
			DetailIndent = effect->DetailIndent(indent_size) + effects->DetailIndent(indent_size);
			CreateNodeHardCopy
			{
				SpecialEffects* effects_copy = nullptr;
				if (num_refs <= 1) // do not need to look up or record in the mapping if there is no repeated reference
				{
					effects_copy = new specialEffects(card_copy,
						(TargetedPlayEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)),
						(OtherEffs*)(effects->CreateNodeHardCopy(card_copy, redir_map)));
					effects_copy->num_refs = num_refs;
				}
				else
				{
					void* tmp_key = (void*)this;
					PtrRedirMapIter it = redir_map.find(tmp_key);
					if (it == redir_map.end()) // not found
					{
						effects_copy = new specialEffects(card_copy,
							(TargetedPlayEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)),
							(OtherEffs*)(effects->CreateNodeHardCopy(card_copy, redir_map)));
						redir_map[tmp_key] = effects_copy;
					}
					else // found
					{
						effects_copy = (SpecialEffects*)(it->second);
						effects_copy->num_refs++;
					}
				}
				
				return effects_copy;
			}
			GetGlobalSelfConfig
			{
				CondConfig tmp_config = effect->GetGlobalSelfConfig(self_config, EFFECT_TIMING_DEFAULT);
				tmp_config &= effects->GetGlobalSelfConfig(self_config, EFFECT_TIMING_DEFAULT);
				return tmp_config;
			}
			isTargetedAtPlay = effect->isTargetedAtPlay(x, y, parent_card);
			CheckPlayValid = effect->CheckPlayValid(x, y, z, parent_card);
			Play { effect->Play(x, y, z, parent_card); if (parent_card->owner->ProcessDeferredEvents()) return; effects->Play(x, y, z, parent_card); }
			Destroy { effects->Destroy(parent_card); }
			Discard { effects->Discard(parent_card); }
			TurnStart { effects->TurnStart(leader, parent_card); }
			TurnEnd { effects->TurnEnd(leader, parent_card); }
			SetOverheatCounts { effect->SetOverheatCounts(val); effects->SetOverheatCounts(val);}
			SetOverheatThresholds { effect->SetOverheatThresholds(val); effects->SetOverheatThresholds(val);}
			ModOverheatThresholds { effect->ModOverheatThresholds(amount); effects->ModOverheatThresholds(amount); }
			GetNodeHeuristicEval = effect->GetNodeHeuristicEval(EFFECT_TIMING_DEFAULT, parent_card) + effects->GetNodeHeuristicEval(EFFECT_TIMING_DEFAULT, parent_card);
		}
	
	TargetedPlayEff
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else
				{
					// targeted effects on played leaders are never generated through recursive generation path, so there shouldn't need to be extra check for played leaders
					if (give_eff)
					{
						force noTargetedPlayEff;
					}
					else if (encoder)
					{
						double choice_code_adjust[3] = {0.0};
						if (!(self_config & TARGET_ANY_CHAR)) // well, played leaders should be forbidden to have battlecries, but we currently don't generate effects adding targeted effects, or transforming played leaders
							choice_code_adjust[CHOICE_TARGETED_BATTLECRY_EFF] = -1e10;
						if (!(self_config & TARGET_IS_SPELL))
							choice_code_adjust[CHOICE_TARGETED_CAST_EFF] = -1e10;
						
						torch_Tensor adjust_tensor = CreateVectorTensorFromArray(choice_code_adjust, 3);
						torch_Tensor choice_tensor = code.narrow(0, 0, 3);
						torch_Tensor final_tensor = choice_tensor + adjust_tensor;
						force [GetMaxIndex(final_tensor)];
					}
					else
					{
						if (!(self_config & TARGET_ANY_CHAR)) // well, played leaders should be forbidden to have battlecries, but we currently don't generate effects adding targeted effects, or transforming played leaders
							forbid targetedBattlecryEff;
						if (!(self_config & TARGET_IS_SPELL))
							forbid targetedCastEff;
					}
				}
			}
		}
	:=
	| noTargetedPlayEff:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_NO_TARGETED_PLAY_EFF));
			}
			DetailIndent = ""; // avoid having extra spaces
			CreateNodeHardCopy = new noTargetedPlayEff(card_copy);
			SetOverheatCounts {} // have to have these three emtpy functions here because otherwise it'd run the version on the node where the overheat threshold and cverheat count are accessed with meaningless (uninitialized) values
			SetOverheatThresholds {}
			ModOverheatThresholds {}
		}
	| targetedBattlecryEff: TargetedEff* effect
		{
			generator
			{
				prioritize_forbidding_leader = RandomRoll(NOT_LEADER_PARAM);
				
				self_config &= CHAR_COND_FILTER;
				if (encoder)
					effect = generate TargetedEff(self_config, EFFECT_TIMING_PLAY, effect_depth, give_eff, rep, encoder, code.narrow(0, 3, Single_Effect_Size));
				else
					effect = generate TargetedEff(self_config, EFFECT_TIMING_PLAY, effect_depth, give_eff, rep, encoder, code);
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_TARGETED_BATTLECRY_EFF));
				effect->FillRep(rep);
			}
			FillTokens
			{
				if (need_starting_newline)
					tokens.push_back(mkNewLineToken());
				tokens.push_back(mkWordToken("Battlecry", indent_size, true));
				tokens.push_back(mkPunctToken(":", indent_size, true));
				effect->FillTokens(tokens, indent_size, need_starting_newline); // the input need_starting_newline doesn't matter in the middle of a line
				tokens.push_back(mkPunctToken(".", indent_size, false));
				effect->FillTokensPostfix(tokens, indent_size);
				need_starting_newline = 1;
			}
			Brief = "<Battlecry" + effect->Brief() + ">";
			Detail = "Battlecry: " + effect->Detail() + "." + effect->Postfix();
			DetailIndent = RepeatSpace(indent_size) + "Battlecry: " + effect->Detail() + "." + effect->PostfixIndent(indent_size);
			CreateNodeHardCopy = new targetedBattlecryEff(card_copy, (TargetedEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)));
			GetEffectNum = 1;
			GetInitAbilFlag = effect->GetInitAbilFlag();
			GetGlobalSelfConfig
			{
				CondConfig tmp_config = effect->GetGlobalSelfConfig(self_config, EFFECT_TIMING_PLAY);
				tmp_config &= CHAR_COND_FILTER;
				// leader and field shall not be selected at the same time if giving a battlecry (leader card is special in that they cannot change their place anymore after being placed)
				if (!(self_config & TARGET_POS_HAND_OR_DECK) || !(tmp_config & TARGET_POS_HAND_OR_DECK))
					tmp_config &= NOT_LEADER_COND_FILTER;
				else if (!(self_config & TARGET_NOT_LEADER) || !(tmp_config & TARGET_NOT_LEADER)) 
					tmp_config &= HAND_OR_DECK_COND_FILTER;
				else
					tmp_config &= (prioritize_forbidding_leader ? NOT_LEADER_COND_FILTER : HAND_OR_DECK_COND_FILTER);
				return tmp_config;
			}
			isTargetedAtPlay = effect->isTargetedAtPlay(x, y, parent_card); // battlecries and spell become untargeted if the condition for it to activate fails (for spells it is different from no valid target in which case the spell cannot be played)
			CheckPlayValid = effect->CheckPlayValid(x, y, z, parent_card);
			Play
			{
				if (z >= 0)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << parent_card->owner->name << "\'s " << parent_card->name << "\'s battlecry effect activated: " << effect->Detail() << "." << effect->PostfixIndent(4);
					#endif
					effect->TargetedAction(z, parent_card, true, true); // negative means no valid target and ignored; play effect is cost constrained
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << endl;
					#endif
				}
				#ifndef SUPPRSS_MATCH_PROMPTS
				else
					if (!parent_card->owner->is_exploration)				
						cout << "Note: condition failed or overheated or no valid target, target ignored, and battlecry not triggered." << endl;
				#endif			
			}
			SetOverheatCounts { effect->SetOverheatCounts(val); }
			SetOverheatThresholds { effect->SetOverheatThresholds(val); }
			ModOverheatThresholds { effect->ModOverheatThresholds(amount); }
			GetNodeHeuristicEval = effect->GetNodeHeuristicEval(EFFECT_TIMING_PLAY, parent_card);
		}
	| targetedCastEff: TargetedEff* effect
		{
			generator
			{
				self_config &= SPELL_COND_FILTER;
				if (encoder)
					effect = generate TargetedEff(self_config, EFFECT_TIMING_PLAY, effect_depth, give_eff, rep, encoder, code.narrow(0, 3, Single_Effect_Size));
				else
					effect = generate TargetedEff(self_config, EFFECT_TIMING_PLAY, effect_depth, give_eff, rep, encoder, code);
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_TARGETED_CAST_EFF));
				effect->FillRep(rep);
			}
			FillTokens
			{
				if (need_starting_newline)
					tokens.push_back(mkNewLineToken());
				tokens.push_back(mkWordToken("Cast", indent_size, true));
				tokens.push_back(mkPunctToken(":", indent_size, true));
				effect->FillTokens(tokens, indent_size, need_starting_newline); // the input need_starting_newline doesn't matter in the middle of a line
				tokens.push_back(mkPunctToken(".", indent_size, false));
				effect->FillTokensPostfix(tokens, indent_size);
				need_starting_newline = 1;
			}
			Brief = "<Cast" + effect->Brief() + ">";
			Detail = "Cast: " + effect->Detail() + "." + effect->Postfix();
			DetailIndent = RepeatSpace(indent_size) + "Cast: " + effect->Detail() + "." + effect->PostfixIndent(indent_size);
			CreateNodeHardCopy = new targetedCastEff(card_copy, (TargetedEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)));
			GetEffectNum = 1;
			GetInitAbilFlag = effect->GetInitAbilFlag();
			GetGlobalSelfConfig
			{
				CondConfig tmp_config = effect->GetGlobalSelfConfig(self_config, EFFECT_TIMING_PLAY);
				tmp_config &= SPELL_COND_FILTER;
				return tmp_config;
			}
			isTargetedAtPlay = effect->isTargetedAtPlay(x, y, parent_card); // battlecries and spell become untargeted if the condition for it to activate fails (for spells it is different from no valid target in which case the spell cannot be played)
			CheckPlayValid = effect->CheckPlayValid(x, y, z, parent_card);
			Play
			{
				if (z >= 0)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << parent_card->owner->name << "\'s " << parent_card->name << "\'s spell cast effect activated: " << effect->Detail() << "." << effect->PostfixIndent(4);
					#endif
					effect->TargetedAction(z, parent_card, true, true); // negative means no valid target and ignored; play effect is cost constrained
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << endl;
					#endif
				}
				#ifndef SUPPRSS_MATCH_PROMPTS
				else
					if (!parent_card->owner->is_exploration)
						cout << "Note: condition failed or overheated or no valid target, target ignored, and cast effect not triggered." << endl;
				#endif			
			}
			SetOverheatCounts { effect->SetOverheatCounts(val); }
			SetOverheatThresholds { effect->SetOverheatThresholds(val); }
			ModOverheatThresholds { effect->ModOverheatThresholds(amount); }
			GetNodeHeuristicEval = effect->GetNodeHeuristicEval(EFFECT_TIMING_PLAY, parent_card);
		}	

	OtherEffs
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else
				{
					if (min_n > 0) // this takes precedence as the lower bound is often some constant (0, or 1) while the upper bound may be accidentally smaller than the lower bound if computed by some formula
					{
						force consOtherEffs;
					}
					else if (max_n <= 0)
					{
						force noOtherEffs;
					}
					else if (encoder)
					{
						torch_Tensor intermediate_code = UnitForward(code, encoder->other_effs_decoder_to_intermediate, false);
						torch_Tensor choice_tensor = ForwardBinaryClassifyOut(intermediate_code, encoder->other_effs_decoder_binary_classify); 
						force [GetBinaryIndex(choice_tensor)];
					}
					else
					{
						// the first effect for leader/minion card has higher prob to appear
						// while for spell card, here it would be the second as the first one is forced, and the second effect for spells has relatively low prob.
						if (min_n == 0 && (self_config & TARGET_ANY_CHAR))
						{
							probof(noOtherEffs) = 0.1;
							probof(consOtherEffs) = 0.9;
						}
					}	
				}
			}
		}
	:=
	| noOtherEffs:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_NO_OTHER_EFFS));
			}
			DetailIndent = ""; // avoid having extra spaces
			CreateNodeHardCopy = new noOtherEffs(card_copy);
			SetOverheatCounts {} // have to have these three emtpy functions here because otherwise it'd run the version on the node where the overheat threshold and cverheat count are accessed with meaningless (uninitialized) values
			SetOverheatThresholds {}
			ModOverheatThresholds {}
		}
	| consOtherEffs: OtherEff* effect, OtherEffs* effects
		{
			generator
			{
				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->other_effs_decoder_to_intermediate, false);
					torch_Tensor classify_tensor = ForwardClassifyOut(intermediate_code, encoder->other_effs_decoder_classify);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->other_effs_decoder_hidden, false);
					torch_Tensor tmp_cat_list[] = { classify_tensor.narrow(0, 0, 6), hidden_tensor.narrow(0, 0, Single_Effect_Size), classify_tensor.narrow(0, 6, Alle_Cond_Size) };
					effect = generate OtherEff(self_config, effect_depth, give_eff, rep, encoder, torch_cat(tmp_cat_list));
					self_config &= ExtractEffectIndependentConfig(effect->GetGlobalSelfConfig(self_config, EFFECT_TIMING_DEFAULT));
					effects = generate OtherEffs(self_config, min_n - 1, max_n - 1, effect_depth, give_eff, rep, encoder, hidden_tensor.narrow(0, Single_Effect_Size, Effect_Seq_Size));
				}
				else
				{
					effect = generate OtherEff(self_config, effect_depth, give_eff, rep, encoder, code);
					self_config &= ExtractEffectIndependentConfig(effect->GetGlobalSelfConfig(self_config, EFFECT_TIMING_DEFAULT));
					effects = generate OtherEffs(self_config, min_n - 1, max_n - 1, effect_depth, give_eff, rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_CONS_OTHER_EFFS));
				effect->FillRep(rep);
				effects->FillRep(rep);
			}
			FillTokens
			{
				effect->FillTokens(tokens, indent_size, need_starting_newline);
				effects->FillTokens(tokens, indent_size, need_starting_newline);	
			}
			Brief = effect->Brief() + effects->Brief();
			Detail = effect->Detail() + effects->Detail();
			DetailIndent = effect->DetailIndent(indent_size) + effects->DetailIndent(indent_size);
			CreateNodeHardCopy = new consOtherEffs(card_copy,
				(OtherEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)),
				(OtherEffs*)(effects->CreateNodeHardCopy(card_copy, redir_map)));
			GetEffectNum = 1 + effects->GetEffectNum(); // currently not used
			GetGlobalSelfConfig
			{
				CondConfig tmp_config = effect->GetGlobalSelfConfig(self_config, EFFECT_TIMING_DEFAULT);
				tmp_config &= effects->GetGlobalSelfConfig(self_config, EFFECT_TIMING_DEFAULT);
				return tmp_config;
			}
			Play { effect->Play(x, y, z, parent_card); if (parent_card->owner->ProcessDeferredEvents()) return; effects->Play(x, y, z, parent_card); } // if this minion dies in the middle of multiple battlecries, the latter ones does not gets activated, similar below except deathrattle
			Destroy { effect->Destroy(parent_card); if (parent_card->owner->ProcessDeferredEvents()) return; effects->Destroy(parent_card); }
			Discard { effect->Discard(parent_card); if (parent_card->owner->ProcessDeferredEvents()) return; effects->Discard(parent_card); }
			TurnStart { effect->TurnStart(leader, parent_card); if (parent_card->owner->ProcessDeferredEvents()) return; effects->TurnStart(leader, parent_card); }
			TurnEnd { effect->TurnEnd(leader, parent_card); if (parent_card->owner->ProcessDeferredEvents()) return; effects->TurnEnd(leader, parent_card); }
			SetOverheatCounts { effect->SetOverheatCounts(val); effects->SetOverheatCounts(val);}
			SetOverheatThresholds { effect->SetOverheatThresholds(val); effects->SetOverheatThresholds(val);}
			ModOverheatThresholds { effect->ModOverheatThresholds(amount); effects->ModOverheatThresholds(amount); }
			GetNodeHeuristicEval = effect->GetNodeHeuristicEval(EFFECT_TIMING_DEFAULT, parent_card) + effects->GetNodeHeuristicEval(EFFECT_TIMING_DEFAULT, parent_card);
		}
	
	OtherEff
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else if (encoder)
				{
					double choice_code_adjust[6] = {0.0};
					if (!(self_config & TARGET_ANY_CHAR)) // must be spell
						choice_code_adjust[CHOICE_UNTARGETED_BATTLECRY_EFF] = choice_code_adjust[CHOICE_DEATHRATTLE_EFF] = -1e10;
					if (!(self_config & TARGET_IS_SPELL)) // must not be spell
						choice_code_adjust[CHOICE_UNTARGETED_CAST_EFF] = -1e10;
					if (!(self_config & TARGET_NOT_LEADER)) // must be leader
					{
						choice_code_adjust[CHOICE_DEATHRATTLE_EFF] = -1e10;
						if (!(self_config & TARGET_POS_HAND_OR_DECK)) // played leaders should not be granted battlecry or on discard effects
							choice_code_adjust[CHOICE_UNTARGETED_BATTLECRY_EFF] = choice_code_adjust[CHOICE_ONDISCARD_EFF] = -1e10;
					}
					torch_Tensor adjust_tensor = CreateVectorTensorFromArray(choice_code_adjust, 6);
					torch_Tensor choice_tensor = code.narrow(0, 0, 6);
					torch_Tensor final_tensor = choice_tensor + adjust_tensor;
					force [GetMaxIndex(final_tensor)];
				}
				else
				{
					if (!(self_config & TARGET_ANY_CHAR)) // must be spell
					{
						forbid <transferto untargetedCastEff> untargetedBattlecryEff, deathrattleEff;
						probof(untargetedCastEff) *= 9.0; // make this extra large for spells as others should be very rare
					}
					if (!(self_config & TARGET_IS_SPELL)) // must not be spell
					{
						forbid <transferto untargetedBattlecryEff> untargetedCastEff;
					}
					if (!(self_config & TARGET_NOT_LEADER)) // must be leader
					{
						forbid <transferto untargetedBattlecryEff> deathrattleEff;
						if (!(self_config & TARGET_POS_HAND_OR_DECK)) // played leaders should not be granted battlecry or on discard effects
							forbid untargetedBattlecryEff, onDiscardEff;
					}
				}
			}
		}
	:=
	| untargetedBattlecryEff: UntargetedEff* effect
		{
			generator
			{
				prioritize_forbidding_leader = RandomRoll(NOT_LEADER_PARAM);
				
				self_config &= CHAR_COND_FILTER;
				if (encoder)
					effect = generate UntargetedEff(self_config, EFFECT_TIMING_PLAY, effect_depth, give_eff, rep, encoder, code.narrow(0, 6, Single_Effect_Size));
				else
					effect = generate UntargetedEff(self_config, EFFECT_TIMING_PLAY, effect_depth, give_eff, rep, encoder, code);
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_UNTARGETED_BATTLECRY_EFF));
				effect->FillRep(rep);
			}
			FillTokens
			{
				if (need_starting_newline)
					tokens.push_back(mkNewLineToken());
				tokens.push_back(mkWordToken("Battlecry", indent_size, true));
				tokens.push_back(mkPunctToken(":", indent_size, true));
				effect->FillTokens(tokens, indent_size, need_starting_newline); // the input need_starting_newline doesn't matter in the middle of a line
				tokens.push_back(mkPunctToken(".", indent_size, false));
				effect->FillTokensPostfix(tokens, indent_size);
				need_starting_newline = 1;
			}
			Brief = "<Battlecry" + effect->Brief() + ">";
			Detail = "Battlecry: " + effect->Detail() + "." + effect->Postfix();
			DetailIndent = RepeatSpace(indent_size) + "Battlecry: " + effect->Detail() + "." + effect->PostfixIndent(indent_size);
			CreateNodeHardCopy = new untargetedBattlecryEff(card_copy, (UntargetedEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)));
			GetInitAbilFlag = effect->GetInitAbilFlag();
			GetGlobalSelfConfig
			{
				CondConfig tmp_config = effect->GetGlobalSelfConfig(self_config, EFFECT_TIMING_PLAY);
				tmp_config &= CHAR_COND_FILTER;
				// leader and field shall not be selected at the same time if giving a battlecry (leader card is special in that they cannot change their place anymore after being placed)
				if (!(self_config & TARGET_POS_HAND_OR_DECK) || !(tmp_config & TARGET_POS_HAND_OR_DECK))
					tmp_config &= NOT_LEADER_COND_FILTER;
				else if (!(self_config & TARGET_NOT_LEADER) || !(tmp_config & TARGET_NOT_LEADER)) 
					tmp_config &= HAND_OR_DECK_COND_FILTER;
				else
					tmp_config &= (prioritize_forbidding_leader ? NOT_LEADER_COND_FILTER : HAND_OR_DECK_COND_FILTER);
				return tmp_config;
			}
			Play 
			{
				#ifndef SUPPRSS_MATCH_PROMPTS
				if (!parent_card->owner->is_exploration)
					cout << parent_card->owner->name << "\'s " << parent_card->name << "\'s battlecry effect activated: " << effect->Detail() << "." << effect->PostfixIndent(4);
				#endif
				effect->UntargetedAction(parent_card, true); // play effect is cost constrained
				#ifndef SUPPRSS_MATCH_PROMPTS
				if (!parent_card->owner->is_exploration)
					cout << endl;
				#endif
			}
			SetOverheatCounts { effect->SetOverheatCounts(val); }
			SetOverheatThresholds { effect->SetOverheatThresholds(val); }
			ModOverheatThresholds { effect->ModOverheatThresholds(amount); }
			GetNodeHeuristicEval = effect->GetNodeHeuristicEval(EFFECT_TIMING_PLAY, parent_card);
		}
	| untargetedCastEff: UntargetedEff* effect
		{
			generator
			{
				self_config &= SPELL_COND_FILTER;
				if (encoder)
					effect = generate UntargetedEff(self_config, EFFECT_TIMING_PLAY, effect_depth, give_eff, rep, encoder, code.narrow(0, 6, Single_Effect_Size));
				else
					effect = generate UntargetedEff(self_config, EFFECT_TIMING_PLAY, effect_depth, give_eff, rep, encoder, code);
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_UNTARGETED_CAST_EFF));
				effect->FillRep(rep);
			}
			FillTokens
			{
				if (need_starting_newline)
					tokens.push_back(mkNewLineToken());
				tokens.push_back(mkWordToken("Cast", indent_size, true));
				tokens.push_back(mkPunctToken(":", indent_size, true));
				effect->FillTokens(tokens, indent_size, need_starting_newline); // the input need_starting_newline doesn't matter in the middle of a line
				tokens.push_back(mkPunctToken(".", indent_size, false));
				effect->FillTokensPostfix(tokens, indent_size);
				need_starting_newline = 1;
			}
			Brief = "<Cast" + effect->Brief() + ">";
			Detail = "Cast: " + effect->Detail() + "." + effect->Postfix();
			DetailIndent = RepeatSpace(indent_size) + "Cast: " + effect->Detail() + "." + effect->PostfixIndent(indent_size);
			CreateNodeHardCopy = new untargetedCastEff(card_copy, (UntargetedEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)));
			GetInitAbilFlag = effect->GetInitAbilFlag();
			GetGlobalSelfConfig
			{
				CondConfig tmp_config = effect->GetGlobalSelfConfig(self_config, EFFECT_TIMING_PLAY);
				tmp_config &= SPELL_COND_FILTER;
				return tmp_config;
			}
			Play 
			{ 
				#ifndef SUPPRSS_MATCH_PROMPTS
				if (!parent_card->owner->is_exploration)
					cout << parent_card->owner->name << "\'s " << parent_card->name << "\'s spell effect activated: " << effect->Detail() << "." << effect->PostfixIndent(4);
				#endif
				effect->UntargetedAction(parent_card, true); // play effect is cost constrained
				#ifndef SUPPRSS_MATCH_PROMPTS
				if (!parent_card->owner->is_exploration)
					cout << endl;
				#endif
			}
			SetOverheatCounts { effect->SetOverheatCounts(val); }
			SetOverheatThresholds { effect->SetOverheatThresholds(val); }
			ModOverheatThresholds { effect->ModOverheatThresholds(amount); }
			GetNodeHeuristicEval = effect->GetNodeHeuristicEval(EFFECT_TIMING_PLAY, parent_card);
		}
	| deathrattleEff: UntargetedEff* effect
		{
			generator
			{
				self_config &= MINION_COND_FILTER; 
				if (encoder)
					effect = generate UntargetedEff(self_config, EFFECT_TIMING_DESTROY, effect_depth, give_eff, rep, encoder, code.narrow(0, 6, Single_Effect_Size));
				else
					effect = generate UntargetedEff(self_config, EFFECT_TIMING_DESTROY, effect_depth, give_eff, rep, encoder, code);
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_DEATHRATTLE_EFF));
				effect->FillRep(rep);
			}
			FillTokens
			{
				if (need_starting_newline)
					tokens.push_back(mkNewLineToken());
				tokens.push_back(mkWordToken("Deathrattle", indent_size, true));
				tokens.push_back(mkPunctToken(":", indent_size, true));
				effect->FillTokens(tokens, indent_size, need_starting_newline); // the input need_starting_newline doesn't matter in the middle of a line
				tokens.push_back(mkPunctToken(".", indent_size, false));
				effect->FillTokensPostfix(tokens, indent_size);
				need_starting_newline = 1;
			}
			Brief = "<Deathrattle" + effect->Brief() + ">";
			Detail = "Deathrattle: " + effect->Detail() + "." + effect->Postfix();
			DetailIndent = RepeatSpace(indent_size) + "Deathrattle: " + effect->Detail() + "." + effect->PostfixIndent(indent_size);
			CreateNodeHardCopy = new deathrattleEff(card_copy, (UntargetedEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)));
			GetInitAbilFlag = effect->GetInitAbilFlag();
			GetGlobalSelfConfig
			{
				CondConfig tmp_config = effect->GetGlobalSelfConfig(self_config, EFFECT_TIMING_DESTROY);
				tmp_config &= MINION_COND_FILTER;
				return tmp_config;
			}
			Destroy
			{
				#ifndef SUPPRSS_MATCH_PROMPTS
				if (!parent_card->owner->is_exploration)
					cout << parent_card->owner->name << "\'s " << parent_card->name << "\'s deathrattle effect activated: " << effect->Detail() << "." << effect->PostfixIndent(4);
				#endif
				effect->UntargetedAction(parent_card, true); // deathrattle is considered constrained as normally the card has be be played before the deathrattle can be triggered (exception exists)
				#ifndef SUPPRSS_MATCH_PROMPTS
				if (!parent_card->owner->is_exploration)
					cout << endl;
				#endif
			}
			SetOverheatCounts { effect->SetOverheatCounts(val); }
			SetOverheatThresholds { effect->SetOverheatThresholds(val); }
			ModOverheatThresholds { effect->ModOverheatThresholds(amount); }
			GetNodeHeuristicEval = effect->GetNodeHeuristicEval(EFFECT_TIMING_DESTROY, parent_card);
		}
	| onDiscardEff: UntargetedEff* effect
		{
			generator
			{
				prioritize_forbidding_leader = RandomRoll(NOT_LEADER_PARAM);
				
				if (encoder)
					effect = generate UntargetedEff(self_config, EFFECT_TIMING_DISCARD, effect_depth, give_eff, rep, encoder, code.narrow(0, 6, Single_Effect_Size));
				else
					effect = generate UntargetedEff(self_config, EFFECT_TIMING_DISCARD, effect_depth, give_eff, rep, encoder, code);
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_ONDISCARD_EFF));
				effect->FillRep(rep);
			}
			FillTokens
			{
				if (need_starting_newline)
					tokens.push_back(mkNewLineToken());
				tokens.push_back(mkWordToken("Discard", indent_size, true));
				tokens.push_back(mkPunctToken(":", indent_size, true));
				effect->FillTokens(tokens, indent_size, need_starting_newline); // the input need_starting_newline doesn't matter in the middle of a line
				tokens.push_back(mkPunctToken(".", indent_size, false));
				effect->FillTokensPostfix(tokens, indent_size);
				need_starting_newline = 1;
			}
			Brief = "<Discard" + effect->Brief() + ">";
			Detail = "Discard: " + effect->Detail() + "." + effect->Postfix();
			DetailIndent = RepeatSpace(indent_size) + "Discard: " + effect->Detail() + "." + effect->PostfixIndent(indent_size);
			CreateNodeHardCopy = new onDiscardEff(card_copy, (UntargetedEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)));
			GetInitAbilFlag = effect->GetInitAbilFlag();
			GetGlobalSelfConfig
			{
				CondConfig tmp_config = effect->GetGlobalSelfConfig(self_config, EFFECT_TIMING_DISCARD);
				// leader and field shall not be selected at the same time if giving a on-discard effect (leader card is special in that they cannot change their place anymore after being placed)
				if (!(self_config & TARGET_POS_HAND_OR_DECK) || !(tmp_config & TARGET_POS_HAND_OR_DECK))
					tmp_config &= NOT_LEADER_COND_FILTER;
				else if (!(self_config & TARGET_NOT_LEADER) || !(tmp_config & TARGET_NOT_LEADER)) 
					tmp_config &= HAND_OR_DECK_COND_FILTER;
				else
					tmp_config &= (prioritize_forbidding_leader ? NOT_LEADER_COND_FILTER : HAND_OR_DECK_COND_FILTER);
				return tmp_config;
			}
			Discard
			{
				#ifndef SUPPRSS_MATCH_PROMPTS
				if (!parent_card->owner->is_exploration)
					cout << parent_card->owner->name << "\'s " << parent_card->name << "\'s on-discard effect activated: " << effect->Detail() << "." << effect->PostfixIndent(4);
				#endif
				effect->UntargetedAction(parent_card, parent_card->card_pos == CARD_POS_AT_FIELD); // on discard is considered cost constrained if discarded from the field (note, played leaders cannot be discarded)
				#ifndef SUPPRSS_MATCH_PROMPTS
				if (!parent_card->owner->is_exploration)
					cout << endl;
				#endif
			}
			SetOverheatCounts { effect->SetOverheatCounts(val); }
			SetOverheatThresholds { effect->SetOverheatThresholds(val); }
			ModOverheatThresholds { effect->ModOverheatThresholds(amount); }
			GetNodeHeuristicEval = effect->GetNodeHeuristicEval(EFFECT_TIMING_DISCARD, parent_card);
		}
	| turnStartEff: UntargetedEff* effect, AllegianceCond* alle
		{
			generator
			{
				if (encoder)
				{
					effect = generate UntargetedEff(self_config, EFFECT_TIMING_TURN, effect_depth, give_eff, rep, encoder, code.narrow(0, 6, Single_Effect_Size));
					double ally_bias = (effect->IsForbiddingOwnTurnStart(self_config) ? 0.0 : 0.5);
					CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
					CondConfig tmp_config = GetDefaultConfig(); // to counter the problem of rvalue passed to lvalue ref
					alle = generate AllegianceCond(tmp_init_config, tmp_config, TARGET_MODE_DEFAULT, EFFECT_TIMING_TURN, ally_bias, rep, encoder, code.narrow(0, 6 + Single_Effect_Size, Alle_Cond_Size));
				}
				else
				{
					effect = generate UntargetedEff(self_config, EFFECT_TIMING_TURN, effect_depth, give_eff, rep, encoder, code);
					double ally_bias = (effect->IsForbiddingOwnTurnStart(self_config) ? 0.0 : 0.5);
					CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
					CondConfig tmp_config = GetDefaultConfig(); // to counter the problem of rvalue passed to lvalue ref
					alle = generate AllegianceCond(tmp_init_config, tmp_config, TARGET_MODE_DEFAULT, EFFECT_TIMING_TURN, ally_bias, rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_TURN_START_EFF));
				effect->FillRep(rep);
				alle->FillRep(rep);
			}
			FillTokens
			{
				if (need_starting_newline)
					tokens.push_back(mkNewLineToken());
				tokens.push_back(mkWordToken("Start", indent_size, true));
				tokens.push_back(mkWordToken("of", indent_size, true));
				alle->FillTokensAlt7(tokens, indent_size, need_starting_newline); // the input need_starting_newline doesn't matter in the middle of a line
				tokens.push_back(mkWordToken("Turns", indent_size, true));
				tokens.push_back(mkPunctToken(":", indent_size, true));
				effect->FillTokens(tokens, indent_size, need_starting_newline); // the input need_starting_newline doesn't matter in the middle of a line
				tokens.push_back(mkPunctToken(".", indent_size, false));
				effect->FillTokensPostfix(tokens, indent_size);
				need_starting_newline = 1;
			}
			Brief = "<Trigger" + effect->Brief() + ">";
			Detail = "At the start of " + alle->DetailAlt2() + "turns: " + effect->Detail() + "." + effect->Postfix();
			DetailIndent = RepeatSpace(indent_size) + "At the start of " + alle->DetailAlt2() + "turns: " + effect->Detail() + "." + effect->PostfixIndent(indent_size);
			CreateNodeHardCopy = new turnStartEff(card_copy,
				(UntargetedEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)),
				(AllegianceCond*)(alle->CreateNodeHardCopy(card_copy, redir_map)));
			GetInitAbilFlag = effect->GetInitAbilFlag();
			GetGlobalSelfConfig = effect->GetGlobalSelfConfig(self_config, EFFECT_TIMING_TURN);
			TurnStart 
			{ 
				if (alle->CheckCardValid(leader, parent_card))
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << parent_card->owner->name << "\'s " << parent_card->name << "\'s turn start effect activated: " << effect->Detail() << "." << effect->PostfixIndent(4);
					#endif
					effect->UntargetedAction(parent_card, parent_card->card_pos == CARD_POS_AT_FIELD || parent_card->card_pos == CARD_POS_AT_LEADER); // turn start effect is considered cost constrained if already on the field (or played leader) 
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << endl;
					#endif
				}
			}
			SetOverheatCounts { effect->SetOverheatCounts(val); }
			SetOverheatThresholds { effect->SetOverheatThresholds(val); }
			ModOverheatThresholds { effect->ModOverheatThresholds(amount); }
			GetNodeHeuristicEval = effect->GetNodeHeuristicEval(EFFECT_TIMING_TURN, parent_card);
		}
	| turnEndEff: UntargetedEff* effect, AllegianceCond* alle
		{
			generator
			{
				if (encoder)
				{
					effect = generate UntargetedEff(self_config, EFFECT_TIMING_TURN, effect_depth, give_eff, rep, encoder, code.narrow(0, 6, Single_Effect_Size));
					CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
					CondConfig tmp_config = GetDefaultConfig(); // to counter the problem of rvalue passed to lvalue ref
					alle = generate AllegianceCond(tmp_init_config, tmp_config, TARGET_MODE_DEFAULT, EFFECT_TIMING_TURN, 2.0, rep, encoder, code.narrow(0, 6 + Single_Effect_Size, Alle_Cond_Size));
				}
				else
				{
					effect = generate UntargetedEff(self_config, EFFECT_TIMING_TURN, effect_depth, give_eff, rep, encoder, code);
					CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
					CondConfig tmp_config = GetDefaultConfig(); // to counter the problem of rvalue passed to lvalue ref
					alle = generate AllegianceCond(tmp_init_config, tmp_config, TARGET_MODE_DEFAULT, EFFECT_TIMING_TURN, 2.0, rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_TURN_END_EFF));
				effect->FillRep(rep);
				alle->FillRep(rep);
			}
			FillTokens
			{
				if (need_starting_newline)
					tokens.push_back(mkNewLineToken());
				tokens.push_back(mkWordToken("End", indent_size, true));
				tokens.push_back(mkWordToken("of", indent_size, true));
				alle->FillTokensAlt7(tokens, indent_size, need_starting_newline); // the input need_starting_newline doesn't matter in the middle of a line
				tokens.push_back(mkWordToken("Turns", indent_size, true));
				tokens.push_back(mkPunctToken(":", indent_size, true));
				effect->FillTokens(tokens, indent_size, need_starting_newline); // the input need_starting_newline doesn't matter in the middle of a line
				tokens.push_back(mkPunctToken(".", indent_size, false));
				effect->FillTokensPostfix(tokens, indent_size);
				need_starting_newline = 1;
			}
			Brief = "<Trigger" + effect->Brief() + ">";
			Detail = "At the end of " + alle->DetailAlt2() + "turns: " + effect->Detail() + "." + effect->Postfix();
			DetailIndent = RepeatSpace(indent_size) + "At the end of " + alle->DetailAlt2() + "turns: " + effect->Detail() + "." + effect->PostfixIndent(indent_size);
			CreateNodeHardCopy = new turnEndEff(card_copy,
				(UntargetedEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)),
				(AllegianceCond*)(alle->CreateNodeHardCopy(card_copy, redir_map)));
			GetInitAbilFlag = effect->GetInitAbilFlag();
			GetGlobalSelfConfig = effect->GetGlobalSelfConfig(self_config, EFFECT_TIMING_TURN);
			TurnEnd 
			{ 
				if (alle->CheckCardValid(leader, parent_card))
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << parent_card->owner->name << "\'s " << parent_card->name << "\'s turn end effect activated: " << effect->Detail() << "." << effect->PostfixIndent(4);
					#endif
					effect->UntargetedAction(parent_card, parent_card->card_pos == CARD_POS_AT_FIELD || parent_card->card_pos == CARD_POS_AT_LEADER); // turn end effect is considered cost constrained if already on the field (or played leader) 
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << endl;
					#endif
				}
			}
			SetOverheatCounts { effect->SetOverheatCounts(val); }
			SetOverheatThresholds { effect->SetOverheatThresholds(val); }
			ModOverheatThresholds { effect->ModOverheatThresholds(amount); }
			GetNodeHeuristicEval = effect->GetNodeHeuristicEval(EFFECT_TIMING_TURN, parent_card);
		}

	TargetedEff // currently the destination only has a constraint (not a condition to trigger effect), and other different types of conditions does not appear together with each other (similar for untargeted one)
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else if (encoder)
				{
					double choice_code_adjust[3] = {0.0};
					if ((self_config & TARGET_IS_SPELL) // as spell position at play is questionable we simply don't allow this sort of conditions
						|| !(self_config & TARGET_NOT_LEADER)) // the source condition on leaders (as characters) at play is not needed as it can be covered by leader condition
						choice_code_adjust[CHOICE_SRC_COND_EFF] = -1e10;

					torch_Tensor adjust_tensor = CreateVectorTensorFromArray(choice_code_adjust, 3);
					torch_Tensor intermediate_code = UnitForward(code, encoder->targeted_eff_decoder_to_intermediate, false);
					torch_Tensor choice_tensor = ForwardClassifyOut(intermediate_code, encoder->targeted_eff_decoder_classify);
					torch_Tensor final_tensor = choice_tensor + adjust_tensor;
					force [GetMaxIndex(final_tensor)];
				}
				else
				{
					if ((self_config & TARGET_IS_SPELL) // as spell position at play is questionable we simply don't allow this sort of conditions
						|| !(self_config & TARGET_NOT_LEADER)) // the source condition on leaders (as characters) at play is not needed as it can be covered by leader condition
						forbid srcCondTargetedEff;
				}
			}
		}
	:=
	| noCondTargetedEff: BaseTargetedEff* effect, TargetCond* desconstr
		{
			generator
			{
				unsigned effect_timing_filter = TARGET_TYPE_ANY;
				if (!(self_config & TARGET_IS_SPELL))
					effect_timing_filter = PLAY_CHAR_SRC_FILTER;
				else if (!(self_config & TARGET_ANY_CHAR))
					effect_timing_filter = PLAY_SPELL_SRC_FILTER;

				CondConfig self_config_copy = self_config;
				self_config_copy &= effect_timing_filter;
				AdjustInitConfigWithAnotherConfig(self_config_copy, self_config);
				
				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->targeted_eff_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->targeted_eff_decoder_hidden, false);
					effect = generate BaseTargetedEff(self_config_copy, TARGET_MODE_PLAY, effect_timing, effect_depth, give_eff, rep, encoder, hidden_tensor.narrow(0, 0, Base_Targeted_Eff_Size));
					CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
					CondConfig tmp_config = effect->GetPlayTargetConfig(); // to counter the problem of rvalue passed to lvalue ref
					desconstr = generate TargetCond(tmp_init_config, tmp_config, TARGET_MODE_PLAY, effect_timing, effect->GetAllyBias(), rep, encoder, hidden_tensor.narrow(0, Base_Targeted_Eff_Size, Cond_Size));
				}
				else
				{
					effect = generate BaseTargetedEff(self_config_copy, TARGET_MODE_PLAY, effect_timing, effect_depth, give_eff, rep, encoder, code);
					CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
					CondConfig tmp_config = effect->GetPlayTargetConfig(); // to counter the problem of rvalue passed to lvalue ref
					desconstr = generate TargetCond(tmp_init_config, tmp_config, TARGET_MODE_PLAY, effect_timing, effect->GetAllyBias(), rep, encoder, code);
				}
			}
			overheat_count = 0;
			overheat_threshold = DEFAULT_OVERHEAT_THRESHOLD;
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_NO_COND_EFF));
				effect->FillRep(rep);
				desconstr->FillRep(rep);
			}
			FillTokens
			{
				effect->FillTokens(tokens, indent_size, need_starting_newline);
				desconstr->FillTokensAlt1(tokens, indent_size, need_starting_newline);
				effect->FillTokensSuffix(tokens, indent_size);	
			}
			FillTokensPostfix = effect->FillTokensPostfix(tokens, indent_size);
			Brief = (overheat_count < overheat_threshold ? "" : "(Overheated)");
			Detail
			{
				string overheat_indicator = (overheat_count < overheat_threshold ? "" : "(Overheated)");
				string overheat_counter = (display_overheat_counts ? "(OH: " + IntToStr(overheat_count) + " of " + IntToStr(overheat_threshold) + ")" : "");
				return overheat_indicator + overheat_counter + effect->Detail() + desconstr->DetailAlt1() + effect->Suffix();
			}
			DetailIndent
			{
				string overheat_indicator = (overheat_count < overheat_threshold ? "" : "(Overheated)");
				string overheat_counter = (display_overheat_counts ? "(OH: " + IntToStr(overheat_count) + " of " + IntToStr(overheat_threshold) + ")" : "");
				return overheat_indicator + overheat_counter + effect->Detail() + desconstr->DetailAlt1() + effect->Suffix();
			}
			Postfix = effect->Postfix();
			PostfixIndent = effect->PostfixIndent(indent_size);
			CreateNodeHardCopy
			{
				noCondTargetedEff* effect_copy = new noCondTargetedEff(card_copy,
					(BaseTargetedEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)),
					(TargetCond*)(desconstr->CreateNodeHardCopy(card_copy, redir_map)));

				// make sure to also copy overheat status
				effect_copy->overheat_count = overheat_count;
				effect_copy->overheat_threshold = overheat_threshold;

				return effect_copy;
			}	
			isTargetedAtPlay = overheat_count < overheat_threshold;
			CheckPlayValid
			{
				// check for untargetable and stealth abilities
				if (parent_card->owner->IsValidTarget(z))
				{
					Card* card = parent_card->owner->GetTargetCard(z);
					if (card->is_untargetable || (card->is_stealth && parent_card->owner->IsValidCharTarget(z)))
						return false;
				}
				else
					return false;

				return desconstr->CheckPlayValid(x, y, z, parent_card);
			}
			TargetedAction
			{
				if (overheat_count < overheat_threshold)
				{
					overheat_count++;
					parent_card->IncContribution(is_cost_constrained); // though currently targeted effects should always be cost constrained (play effects), but just in case for future extension
					return effect->TargetedAction(z, parent_card, true, is_cost_constrained);
				}
				#ifndef SUPPRSS_MATCH_PROMPTS
				else
					if (!parent_card->owner->is_exploration)
						cout << "Overheated, not actually activated." << endl;
				#endif

				return false;
			}
			GetNodeHeuristicEval
			{
				double cost_divisor = 1.0 + (double)parent_card->mana;
				
				double tmp_val = DEFAULT_EFFECT_EVAL;
				if (effect_timing == EFFECT_TIMING_PLAY)
				{
					if (parent_card->card_pos == CARD_POS_AT_LEADER)
						return 0.0; // played leaders cannot be moved or copied
					if (parent_card->card_pos == CARD_POS_AT_FIELD)
						tmp_val /= cost_divisor * 50.0; // slight chance to be played again
					else if (parent_card->card_pos == CARD_POS_AT_HAND)
						tmp_val /= cost_divisor;
					else if (parent_card->card_pos == CARD_POS_AT_DECK)
						tmp_val /= cost_divisor * 10.0;
				}
				else if (effect_timing == EFFECT_TIMING_DESTROY)
				{
					if (parent_card->card_pos == CARD_POS_AT_HAND)
						tmp_val /= cost_divisor * 2.0;
					else if (parent_card->card_pos == CARD_POS_AT_DECK)
						tmp_val /= cost_divisor * 10.0;
				}

				// poisonous and lifesteal affects certain effects (damageEff)
				if (parent_card->is_poisonous)
					tmp_val *= effect->GetPoisonousGrowthFactor();	
				if (parent_card->is_lifesteal)
					tmp_val *= effect->GetLifestealGrowthFactor();

				// overheat consideration
				double overheat_rate = ((double)overheat_threshold - 0.5 * (double)overheat_count)/(double)DEFAULT_OVERHEAT_THRESHOLD; // consider both the temporary remaining and the overall allowable trigger times per pair of turns
				if (overheat_rate < 0.0)
					return 0.0;
				tmp_val *= overheat_rate;

				return tmp_val;
			}
		}
	| indeCondTargetedEff: BaseTargetedEff* effect, TargetCond* desconstr, IndeCond* cond
		{
			generator
			{
				unsigned effect_timing_filter = TARGET_TYPE_ANY;
				if (!(self_config & TARGET_IS_SPELL))
					effect_timing_filter = PLAY_CHAR_SRC_FILTER;
				else if (!(self_config & TARGET_ANY_CHAR))
					effect_timing_filter = PLAY_SPELL_SRC_FILTER;

				CondConfig self_config_copy = self_config;
				self_config_copy &= effect_timing_filter;
				AdjustInitConfigWithAnotherConfig(self_config_copy, self_config);
				CondConfig self_instant_config = GetInstantConfigFromInitConfig(self_config_copy);

				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->targeted_eff_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->targeted_eff_decoder_hidden, false);
					effect = generate BaseTargetedEff(self_config_copy, TARGET_MODE_PLAY, effect_timing, effect_depth, give_eff, rep, encoder, hidden_tensor.narrow(0, 0, Base_Targeted_Eff_Size));
					CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
					CondConfig tmp_instant_config = effect->GetPlayTargetConfig(); // to counter the problem of rvalue passed to lvalue ref
					desconstr = generate TargetCond(tmp_init_config, tmp_instant_config, TARGET_MODE_PLAY, effect_timing, effect->GetAllyBias(), rep, encoder, hidden_tensor.narrow(0, Base_Targeted_Eff_Size, Cond_Size));
					CondConfig leader_instant_config = desconstr->GetLeaderConfig(); // needed for considering mp/maxMp related stuffs, as it is connected to the cost of cards played
					cond = generate IndeCond(self_config_copy, self_instant_config, leader_instant_config, effect_timing, rep, encoder, hidden_tensor.narrow(0, Base_Targeted_Eff_Size + Cond_Size, Cond_Size));
				}
				else
				{
					effect = generate BaseTargetedEff(self_config_copy, TARGET_MODE_PLAY, effect_timing, effect_depth, give_eff, rep, encoder, code);
					CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
					CondConfig tmp_instant_config = effect->GetPlayTargetConfig(); // to counter the problem of rvalue passed to lvalue ref
					desconstr = generate TargetCond(tmp_init_config, tmp_instant_config, TARGET_MODE_PLAY, effect_timing, effect->GetAllyBias(), rep, encoder, code);
					CondConfig leader_instant_config = desconstr->GetLeaderConfig(); // needed for considering mp/maxMp related stuffs, as it is connected to the cost of cards played
					cond = generate IndeCond(self_config_copy, self_instant_config, leader_instant_config, effect_timing, rep, encoder, code);
				}
			}
			overheat_count = 0;
			overheat_threshold = DEFAULT_OVERHEAT_THRESHOLD;
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_INDE_COND_EFF));
				effect->FillRep(rep);
				desconstr->FillRep(rep);
				cond->FillRep(rep);
			}
			FillTokens
			{
				effect->FillTokens(tokens, indent_size, need_starting_newline);
				desconstr->FillTokensAlt1(tokens, indent_size, need_starting_newline);
				effect->FillTokensSuffix(tokens, indent_size);
				tokens.push_back(mkPunctToken(",", indent_size, false));
				tokens.push_back(mkWordToken("if", indent_size, false));
				cond->FillTokens(tokens, indent_size, need_starting_newline);
			}
			FillTokensPostfix = effect->FillTokensPostfix(tokens, indent_size);
			Brief = (overheat_count < overheat_threshold ? "" : "(Overheated)");
			Detail
			{
				string overheat_indicator = (overheat_count < overheat_threshold ? "" : "(Overheated)");
				string overheat_counter = (display_overheat_counts ? "(OH: " + IntToStr(overheat_count) + " of " + IntToStr(overheat_threshold) + ")" : "");
				return overheat_indicator + overheat_counter + effect->Detail() + desconstr->DetailAlt1() + effect->Suffix() + ", if " + cond->Detail();
			}
			DetailIndent
			{
				string overheat_indicator = (overheat_count < overheat_threshold ? "" : "(Overheated)");
				string overheat_counter = (display_overheat_counts ? "(OH: " + IntToStr(overheat_count) + " of " + IntToStr(overheat_threshold) + ")" : "");
				return overheat_indicator + overheat_counter + effect->Detail() + desconstr->DetailAlt1() + effect->Suffix() + ", if " + cond->Detail();
			}
			Postfix = effect->Postfix();
			PostfixIndent = effect->PostfixIndent(indent_size);
			CreateNodeHardCopy
			{
				indeCondTargetedEff* effect_copy = new indeCondTargetedEff(card_copy,
					(BaseTargetedEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)),
					(TargetCond*)(desconstr->CreateNodeHardCopy(card_copy, redir_map)),
					(IndeCond*)(cond->CreateNodeHardCopy(card_copy, redir_map)));

				// make sure to also copy overheat status
				effect_copy->overheat_count = overheat_count;
				effect_copy->overheat_threshold = overheat_threshold;

				return effect_copy;
			}
			isTargetedAtPlay = overheat_count < overheat_threshold && cond->CheckThisValid(parent_card);
			CheckPlayValid
			{
				// check for untargetable and stealth abilities
				if (parent_card->owner->IsValidTarget(z))
				{
					Card* card = parent_card->owner->GetTargetCard(z);
					if (card->is_untargetable || (card->is_stealth && parent_card->owner->IsValidCharTarget(z)))
						return false;
				}
				else
					return false;

				return desconstr->CheckPlayValid(x, y, z, parent_card);
			}
			TargetedAction
			{ 
				if (cond->CheckThisValid(parent_card) && overheat_count < overheat_threshold)
				{
					overheat_count++;
					parent_card->IncContribution(is_cost_constrained); // though currently targeted effects should always be cost constrained (play effects), but just in case for future extension
					return effect->TargetedAction(z, parent_card, true, is_cost_constrained);
				}
				#ifndef SUPPRSS_MATCH_PROMPTS
				else
					if (!parent_card->owner->is_exploration)
						cout << "Condition check failed or overheated, not actually activated." << endl;
				#endif
				return false;
			}
			GetNodeHeuristicEval
			{
				double cost_divisor = 1.0 + (double)parent_card->mana;
				
				double tmp_val = DEFAULT_EFFECT_EVAL;
				if (effect_timing == EFFECT_TIMING_PLAY)
				{
					if (parent_card->card_pos == CARD_POS_AT_LEADER)
						return 0.0; // played leaders cannot be moved or copied
					if (parent_card->card_pos == CARD_POS_AT_FIELD)
						tmp_val /= cost_divisor * 50.0; // slight chance to be played again
					else if (parent_card->card_pos == CARD_POS_AT_HAND)
						tmp_val /= cost_divisor;
					else if (parent_card->card_pos == CARD_POS_AT_DECK)
						tmp_val /= cost_divisor * 10.0;
				}
				else if (effect_timing == EFFECT_TIMING_DESTROY)
				{
					if (parent_card->card_pos == CARD_POS_AT_HAND)
						tmp_val /= cost_divisor * 2.0;
					else if (parent_card->card_pos == CARD_POS_AT_DECK)
						tmp_val /= cost_divisor * 10.0;
				}

				// poisonous and lifesteal affects certain effects (damageEff)
				if (parent_card->is_poisonous)
					tmp_val *= effect->GetPoisonousGrowthFactor();	
				if (parent_card->is_lifesteal)
					tmp_val *= effect->GetLifestealGrowthFactor();

				// overheat consideration
				double overheat_rate = ((double)overheat_threshold - 0.5 * (double)overheat_count)/(double)DEFAULT_OVERHEAT_THRESHOLD; // consider both the temporary remaining and the overall allowable trigger times per pair of turns
				if (overheat_rate < 0.0)
					return 0.0;
				tmp_val *= overheat_rate;

				return tmp_val;
			}
		}
	| srcCondTargetedEff: BaseTargetedEff* effect, TargetCond* desconstr, TargetCond* srccond
		{
			generator
			{
				unsigned effect_timing_filter = TARGET_TYPE_ANY;
				if (!(self_config & TARGET_IS_SPELL))
					effect_timing_filter = PLAY_CHAR_SRC_FILTER;
				else if (!(self_config & TARGET_ANY_CHAR))
					effect_timing_filter = PLAY_SPELL_SRC_FILTER;

				CondConfig self_config_copy = self_config;
				self_config_copy &= effect_timing_filter;
				AdjustInitConfigWithAnotherConfig(self_config_copy, self_config);
				CondConfig self_instant_config = GetInstantConfigFromInitConfig(self_config_copy);

				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->targeted_eff_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->targeted_eff_decoder_hidden, false);			
					effect = generate BaseTargetedEff(self_config_copy, TARGET_MODE_PLAY, effect_timing, effect_depth, give_eff, rep, encoder, hidden_tensor.narrow(0, 0, Base_Targeted_Eff_Size));
					CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
					CondConfig tmp_config = effect->GetPlayTargetConfig(); // to counter the problem of rvalue passed to lvalue ref
					desconstr = generate TargetCond(tmp_init_config, tmp_config, TARGET_MODE_PLAY, effect_timing, effect->GetAllyBias(), rep, encoder, hidden_tensor.narrow(0, Base_Targeted_Eff_Size, Cond_Size));
					// note for source cond the ally bias doesn't matter as it'll be forced as anyAllegiance (the actual card is always ally) 
					srccond = generate TargetCond(self_config_copy, self_instant_config, TARGET_MODE_SOURCE, effect_timing, 1.0, rep, encoder, hidden_tensor.narrow(0, Base_Targeted_Eff_Size + Cond_Size, Cond_Size));
				}
				else
				{
					effect = generate BaseTargetedEff(self_config_copy, TARGET_MODE_PLAY, effect_timing, effect_depth, give_eff, rep, encoder, code);
					CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
					CondConfig tmp_config = effect->GetPlayTargetConfig(); // to counter the problem of rvalue passed to lvalue ref
					desconstr = generate TargetCond(tmp_init_config, tmp_config, TARGET_MODE_PLAY, effect_timing, effect->GetAllyBias(), rep, encoder, code);
					// note for source cond the ally bias doesn't matter as it'll be forced as anyAllegiance (the actual card is always ally) 
					srccond = generate TargetCond(self_config_copy, self_instant_config, TARGET_MODE_SOURCE, effect_timing, 1.0, rep, encoder, code);
				}

				self_config |= srccond->GetInitAbilFlag();
			}
			overheat_count = 0;
			overheat_threshold = DEFAULT_OVERHEAT_THRESHOLD;
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_SRC_COND_EFF));
				effect->FillRep(rep);
				desconstr->FillRep(rep);
				srccond->FillRep(rep);
			}
			FillTokens
			{
				effect->FillTokens(tokens, indent_size, need_starting_newline);
				desconstr->FillTokensAlt1(tokens, indent_size, need_starting_newline);
				effect->FillTokensSuffix(tokens, indent_size);
				tokens.push_back(mkPunctToken(",", indent_size, false));
				tokens.push_back(mkWordToken("if", indent_size, false));
				tokens.push_back(mkWordToken("itself", indent_size, false));
				tokens.push_back(mkWordToken("is", indent_size, false));
				srccond->FillTokensAlt3(tokens, indent_size, need_starting_newline);
			}
			FillTokensPostfix = effect->FillTokensPostfix(tokens, indent_size);
			Brief = (overheat_count < overheat_threshold ? "" : "(Overheated)");
			Detail
			{
				string overheat_indicator = (overheat_count < overheat_threshold ? "" : "(Overheated)");
				string overheat_counter = (display_overheat_counts ? "(OH: " + IntToStr(overheat_count) + " of " + IntToStr(overheat_threshold) + ")" : "");
				return overheat_indicator + overheat_counter + effect->Detail() + desconstr->DetailAlt1() + effect->Suffix() + ", if itself is " + srccond->DetailAlt3();
			}
			DetailIndent
			{
				string overheat_indicator = (overheat_count < overheat_threshold ? "" : "(Overheated)");
				string overheat_counter = (display_overheat_counts ? "(OH: " + IntToStr(overheat_count) + " of " + IntToStr(overheat_threshold) + ")" : "");
				return overheat_indicator + overheat_counter + effect->Detail() + desconstr->DetailAlt1() + effect->Suffix() + ", if itself is " + srccond->DetailAlt3();
			}
			Postfix = effect->Postfix();
			PostfixIndent = effect->PostfixIndent(indent_size);
			CreateNodeHardCopy
			{
				srcCondTargetedEff* effect_copy = new srcCondTargetedEff(card_copy,
					(BaseTargetedEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)),
					(TargetCond*)(desconstr->CreateNodeHardCopy(card_copy, redir_map)),
					(TargetCond*)(srccond->CreateNodeHardCopy(card_copy, redir_map)));

				// make sure to also copy overheat status
				effect_copy->overheat_count = overheat_count;
				effect_copy->overheat_threshold = overheat_threshold;

				return effect_copy;
			}
			GetInitAbilFlag = srccond->GetInitAbilFlag();
			GetGlobalSelfConfig
			{
				CondConfig self_config_copy = self_config; // make a copy for making detailed changes when considering the subtree of the srccond (in particular, whether it is a target on the field or some card target)
				return srccond->GetGlobalSelfConfig(self_config_copy, effect_timing);
			}
			isTargetedAtPlay = overheat_count < overheat_threshold && srccond->CheckThisValid(parent_card);
			CheckPlayValid
			{
				// check for untargetable and stealth abilities
				if (parent_card->owner->IsValidTarget(z))
				{
					Card* card = parent_card->owner->GetTargetCard(z);
					if (card->is_untargetable || (card->is_stealth && parent_card->owner->IsValidCharTarget(z)))
						return false;
				}
				else
					return false;

				return desconstr->CheckPlayValid(x, y, z, parent_card);
			}
			TargetedAction // assuming the destination constraint is valid
			{ 
				if (srccond->CheckThisValid(parent_card) && overheat_count < overheat_threshold) // do not check if the source is dying or something, as deathrattle and on-discard effects still needs be triggered
				{
					overheat_count++;
					parent_card->IncContribution(is_cost_constrained);
					return effect->TargetedAction(z, parent_card, true, is_cost_constrained); // though currently targeted effects should always be cost constrained (play effects), but just in case for future extension
				}
				#ifndef SUPPRSS_MATCH_PROMPTS
				else
					if (!parent_card->owner->is_exploration)
						cout << "Condition check failed or overheated, not actually activated." << endl;
				#endif
				return false;
			}
			GetNodeHeuristicEval
			{
				double cost_divisor = 1.0 + (double)parent_card->mana;
				
				double tmp_val = DEFAULT_EFFECT_EVAL;
				if (effect_timing == EFFECT_TIMING_PLAY)
				{
					if (parent_card->card_pos == CARD_POS_AT_LEADER)
						return 0.0; // played leaders cannot be moved or copied
					if (parent_card->card_pos == CARD_POS_AT_FIELD)
						tmp_val /= cost_divisor * 50.0; // slight chance to be played again
					else if (parent_card->card_pos == CARD_POS_AT_HAND)
						tmp_val /= cost_divisor;
					else if (parent_card->card_pos == CARD_POS_AT_DECK)
						tmp_val /= cost_divisor * 10.0;
				}
				else if (effect_timing == EFFECT_TIMING_DESTROY)
				{
					if (parent_card->card_pos == CARD_POS_AT_HAND)
						tmp_val /= cost_divisor * 2.0;
					else if (parent_card->card_pos == CARD_POS_AT_DECK)
						tmp_val /= cost_divisor * 10.0;
				}
				else
				{
					CondConfig src_config = srccond->GetTargetConfig();
					if (!(src_config & TARGET_POS_HAND_OR_DECK))
					{
						double hp_rating = (double)(parent_card->max_hp - parent_card->hp_loss)/10.0;
						double hp_bonus_factor = 1.0 + hp_rating;
						if (effect_timing == EFFECT_TIMING_TURN)
							tmp_val *= hp_bonus_factor; // the longer the minion/leader stays on the field the more impact the effect would have
						
						if (parent_card->card_pos == CARD_POS_AT_HAND)
							tmp_val /= cost_divisor * 2.0;
						else if (parent_card->card_pos == CARD_POS_AT_DECK)
							tmp_val /= cost_divisor * 10.0;
					}
					else if (!(src_config & TARGET_POS_FIELD))
					{
						if (parent_card->card_pos == CARD_POS_AT_LEADER)
							return 0.0; // played leaders cannot be moved or copied
						if (parent_card->card_pos == CARD_POS_AT_FIELD)
							tmp_val /= 25.0; // slight chance to be returned to hand/deck
					}
					else if (!(src_config & TARGET_NOT_HAND))
					{
						if (parent_card->card_pos == CARD_POS_AT_LEADER)
							return 0.0; // played leaders cannot be moved or copied
						if (parent_card->card_pos == CARD_POS_AT_FIELD)
							tmp_val /= 25.0; // slight chance to be returned to hand
						else if (parent_card->card_pos == CARD_POS_AT_DECK)
							tmp_val /= 5.0;
					}
					else if (!(src_config & TARGET_NOT_DECK))
					{
						if (parent_card->card_pos == CARD_POS_AT_LEADER)
							return 0.0; // played leaders cannot be moved or copied
						if (parent_card->card_pos == CARD_POS_AT_FIELD)
							tmp_val /= 25.0; // slight chance to be returned to deck
						else if (parent_card->card_pos == CARD_POS_AT_HAND)
							tmp_val /= 25.0; // slight chance to be returned to deck
					}
				}

				// poisonous and lifesteal affects certain effects (damageEff)
				if (parent_card->is_poisonous)
					tmp_val *= effect->GetPoisonousGrowthFactor();	
				if (parent_card->is_lifesteal)
					tmp_val *= effect->GetLifestealGrowthFactor();

				// overheat consideration
				double overheat_rate = ((double)overheat_threshold - 0.5 * (double)overheat_count)/(double)DEFAULT_OVERHEAT_THRESHOLD; // consider both the temporary remaining and the overall allowable trigger times per pair of turns
				if (overheat_rate < 0.0)
					return 0.0;
				tmp_val *= overheat_rate;

				return tmp_val;
			}
		}

	UntargetedEff
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else
				{
					if (effect_timing == EFFECT_TIMING_TURN) // for turn start and turn end effects, it needs to at least specify where the card is (field/hand&/deck), doesn't apply to discard as discard doesn't happen so often as turn start/turn end
						force srcCondUntargetedEff;

					if (encoder)
					{
						double choice_code_adjust[3] = {0.0};
						if (((self_config & TARGET_IS_SPELL) || !(self_config & TARGET_NOT_LEADER)) && effect_timing == EFFECT_TIMING_PLAY) // as spell position at play is questionable we simply don't allow this sort of conditions; the source condition on leaders (as characters) at play is not needed as it can be covered by leader condition
							choice_code_adjust[CHOICE_SRC_COND_EFF] = -1e10;

						torch_Tensor adjust_tensor = CreateVectorTensorFromArray(choice_code_adjust, 3);
						torch_Tensor intermediate_code = UnitForward(code, encoder->untargeted_eff_decoder_to_intermediate, false);
						torch_Tensor choice_tensor = ForwardClassifyOut(intermediate_code, encoder->untargeted_eff_decoder_classify);
						torch_Tensor final_tensor = choice_tensor + adjust_tensor;
						force [GetMaxIndex(final_tensor)];
					}
					else
					{
						if (((self_config & TARGET_IS_SPELL) || !(self_config & TARGET_NOT_LEADER)) && effect_timing == EFFECT_TIMING_PLAY) // as spell position at play is questionable we simply don't allow this sort of conditions; the source condition on leaders (as characters) at play is not needed as it can be covered by leader condition
							forbid srcCondUntargetedEff;
					}
				}
			}
		}
	:=
	| noCondUntargetedEff: BaseUntargetedEff* effect
		{
			generator
			{
				unsigned effect_timing_filter = TARGET_TYPE_ANY;
				switch (effect_timing)
				{
					case EFFECT_TIMING_PLAY:
						if (!(self_config & TARGET_IS_SPELL))
							effect_timing_filter = PLAY_CHAR_SRC_FILTER;
						else if (!(self_config & TARGET_ANY_CHAR))
							effect_timing_filter = PLAY_SPELL_SRC_FILTER;
						break;
					case EFFECT_TIMING_DESTROY:
						effect_timing_filter = DESTROY_SRC_FILTER;
						break;	
				}

				CondConfig self_config_copy = self_config;
				self_config_copy &= effect_timing_filter;
				AdjustInitConfigWithAnotherConfig(self_config_copy, self_config);

				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->untargeted_eff_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->untargeted_eff_decoder_hidden, false);
					effect = generate BaseUntargetedEff(self_config_copy, effect_timing, effect_depth, give_eff, rep, encoder, hidden_tensor.narrow(0, 0, Base_Untargeted_Eff_Size));
				}
				else
				{
					effect = generate BaseUntargetedEff(self_config_copy, effect_timing, effect_depth, give_eff, rep, encoder, code);
				}
			}
			overheat_count = 0;
			overheat_threshold = DEFAULT_OVERHEAT_THRESHOLD;
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_NO_COND_EFF));
				effect->FillRep(rep);
			}
			FillTokens
			{
				effect->FillTokens(tokens, indent_size, need_starting_newline);
			}
			FillTokensPostfix = effect->FillTokensPostfix(tokens, indent_size);
			Brief = (overheat_count < overheat_threshold ? "" : "(Overheated)");
			Detail
			{
				string overheat_indicator = (overheat_count < overheat_threshold ? "" : "(Overheated)");
				string overheat_counter = (display_overheat_counts ? "(OH: " + IntToStr(overheat_count) + " of " + IntToStr(overheat_threshold) + ")" : "");
				return overheat_indicator + overheat_counter + effect->Detail();
			}
			DetailIndent
			{
				string overheat_indicator = (overheat_count < overheat_threshold ? "" : "(Overheated)");
				string overheat_counter = (display_overheat_counts ? "(OH: " + IntToStr(overheat_count) + " of " + IntToStr(overheat_threshold) + ")" : "");
				return overheat_indicator + overheat_counter + effect->Detail();
			}
			Postfix = effect->Postfix();
			PostfixIndent = effect->PostfixIndent(indent_size);
			CreateNodeHardCopy
			{
				noCondUntargetedEff* effect_copy = new noCondUntargetedEff(card_copy, (BaseUntargetedEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)));

				// make sure to also copy overheat status
				effect_copy->overheat_count = overheat_count;
				effect_copy->overheat_threshold = overheat_threshold;

				return effect_copy;
			}
			GetInitAbilFlag = effect->GetInitAbilFlag();
			GetGlobalSelfConfig = effect->GetGlobalSelfConfig(self_config, effect_timing);
			UntargetedAction
			{
				if (overheat_count < overheat_threshold)
				{
					overheat_count++;
					parent_card->IncContribution(is_cost_constrained);
					effect->UntargetedAction(parent_card, is_cost_constrained);
				}
				#ifndef SUPPRSS_MATCH_PROMPTS
				else
					if (!parent_card->owner->is_exploration)
						cout << "Overheated, not actually activated." << endl;
				#endif
			}
			GetNodeHeuristicEval
			{
				double cost_divisor = 1.0 + (double)parent_card->mana;
				
				double tmp_val = DEFAULT_EFFECT_EVAL;
				if (effect_timing == EFFECT_TIMING_PLAY)
				{
					if (parent_card->card_pos == CARD_POS_AT_LEADER)
						return 0.0; // played leaders cannot be moved or copied
					if (parent_card->card_pos == CARD_POS_AT_FIELD)
						tmp_val /= cost_divisor * 50.0; // slight chance to be played again
					else if (parent_card->card_pos == CARD_POS_AT_HAND)
						tmp_val /= cost_divisor;
					else if (parent_card->card_pos == CARD_POS_AT_DECK)
						tmp_val /= cost_divisor * 10.0;
				}
				else if (effect_timing == EFFECT_TIMING_DESTROY)
				{
					if (parent_card->card_pos == CARD_POS_AT_HAND)
						tmp_val /= cost_divisor * 2.0;
					else if (parent_card->card_pos == CARD_POS_AT_DECK)
						tmp_val /= cost_divisor * 10.0;
				}

				// poisonous and lifesteal affects certain effects (damageEff)
				if (parent_card->is_poisonous)
					tmp_val *= effect->GetPoisonousGrowthFactor();	
				if (parent_card->is_lifesteal)
					tmp_val *= effect->GetLifestealGrowthFactor();

				// overheat consideration
				double overheat_rate = ((double)overheat_threshold - 0.5 * (double)overheat_count)/(double)DEFAULT_OVERHEAT_THRESHOLD; // consider both the temporary remaining and the overall allowable trigger times per pair of turns
				if (overheat_rate < 0.0)
					return 0.0;
				tmp_val *= overheat_rate;

				return tmp_val;
			}
		}
	| indeCondUntargetedEff: BaseUntargetedEff* effect, IndeCond* cond
		{
			generator
			{
				unsigned effect_timing_filter = TARGET_TYPE_ANY;
				switch (effect_timing)
				{
					case EFFECT_TIMING_PLAY:
						if (!(self_config & TARGET_IS_SPELL))
							effect_timing_filter = PLAY_CHAR_SRC_FILTER;
						else if (!(self_config & TARGET_ANY_CHAR))
							effect_timing_filter = PLAY_SPELL_SRC_FILTER;
						break;
					case EFFECT_TIMING_DESTROY:
						effect_timing_filter = DESTROY_SRC_FILTER;
						break;
				}

				CondConfig self_config_copy = self_config;
				self_config_copy &= effect_timing_filter;
				AdjustInitConfigWithAnotherConfig(self_config_copy, self_config);
				CondConfig self_instant_config = GetInstantConfigFromInitConfig(self_config_copy);

				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->untargeted_eff_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->untargeted_eff_decoder_hidden, false);				
					effect = generate BaseUntargetedEff(self_config_copy, effect_timing, effect_depth, give_eff, rep, encoder, hidden_tensor.narrow(0, 0, Base_Untargeted_Eff_Size));
					CondConfig sub_self_config = effect->GetSelfConfig(self_config);
					AdjustInitConfigWithAnotherConfig(self_config_copy, sub_self_config);
					self_instant_config &= sub_self_config;
					CondConfig leader_instant_config = effect->GetLeaderConfig();
					cond = generate IndeCond(self_config_copy, self_instant_config, leader_instant_config, effect_timing, rep, encoder, hidden_tensor.narrow(0, Base_Untargeted_Eff_Size, Cond_Size));
				}
				else
				{
					effect = generate BaseUntargetedEff(self_config_copy, effect_timing, effect_depth, give_eff, rep, encoder, code);
					CondConfig sub_self_config = effect->GetSelfConfig(self_config);
					AdjustInitConfigWithAnotherConfig(self_config_copy, sub_self_config);
					self_instant_config &= sub_self_config;
					CondConfig leader_instant_config = effect->GetLeaderConfig();
					cond = generate IndeCond(self_config_copy, self_instant_config, leader_instant_config, effect_timing, rep, encoder, code);
				}
			}
			overheat_count = 0;
			overheat_threshold = DEFAULT_OVERHEAT_THRESHOLD;
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_INDE_COND_EFF));
				effect->FillRep(rep);
				cond->FillRep(rep);
			}
			FillTokens
			{
				effect->FillTokens(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkPunctToken(",", indent_size, false));
				tokens.push_back(mkWordToken("if", indent_size, false));
				cond->FillTokens(tokens, indent_size, need_starting_newline);
			}
			FillTokensPostfix = effect->FillTokensPostfix(tokens, indent_size);
			Brief = (overheat_count < overheat_threshold ? "" : "(Overheated)");
			Detail
			{
				string overheat_indicator = (overheat_count < overheat_threshold ? "" : "(Overheated)");
				string overheat_counter = (display_overheat_counts ? "(OH: " + IntToStr(overheat_count) + " of " + IntToStr(overheat_threshold) + ")" : "");
				return overheat_indicator + overheat_counter + effect->Detail() + ", if " + cond->Detail();
			}
			DetailIndent
			{
				string overheat_indicator = (overheat_count < overheat_threshold ? "" : "(Overheated)");
				string overheat_counter = (display_overheat_counts ? "(OH: " + IntToStr(overheat_count) + " of " + IntToStr(overheat_threshold) + ")" : "");
				return overheat_indicator + overheat_counter + effect->Detail() + ", if " + cond->Detail();
			}
			Postfix = effect->Postfix();
			PostfixIndent = effect->PostfixIndent(indent_size);
			CreateNodeHardCopy
			{
				indeCondUntargetedEff* effect_copy = new indeCondUntargetedEff(card_copy,
					(BaseUntargetedEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)),
					(IndeCond*)(cond->CreateNodeHardCopy(card_copy, redir_map)));

				// make sure to also copy overheat status
				effect_copy->overheat_count = overheat_count;
				effect_copy->overheat_threshold = overheat_threshold;

				return effect_copy;
			}
			GetInitAbilFlag = effect->GetInitAbilFlag();
			UntargetedAction
			{ 
				if (cond->CheckThisValid(parent_card) && overheat_count < overheat_threshold)
				{
					overheat_count++;
					parent_card->IncContribution(is_cost_constrained);
					effect->UntargetedAction(parent_card, is_cost_constrained);
				}
				#ifndef SUPPRSS_MATCH_PROMPTS
				else
					if (!parent_card->owner->is_exploration)
						cout << "Condition check failed or overheated, not actually activated." << endl;	
				#endif			
			}
			GetNodeHeuristicEval
			{
				double cost_divisor = 1.0 + (double)parent_card->mana;
				
				double tmp_val = DEFAULT_EFFECT_EVAL;
				if (effect_timing == EFFECT_TIMING_PLAY)
				{
					if (parent_card->card_pos == CARD_POS_AT_LEADER)
						return 0.0; // played leaders cannot be moved or copied
					if (parent_card->card_pos == CARD_POS_AT_FIELD)
						tmp_val /= cost_divisor * 50.0; // slight chance to be played again
					else if (parent_card->card_pos == CARD_POS_AT_HAND)
						tmp_val /= cost_divisor;
					else if (parent_card->card_pos == CARD_POS_AT_DECK)
						tmp_val /= cost_divisor * 10.0;
				}
				else if (effect_timing == EFFECT_TIMING_DESTROY)
				{
					if (parent_card->card_pos == CARD_POS_AT_HAND)
						tmp_val /= cost_divisor * 2.0;
					else if (parent_card->card_pos == CARD_POS_AT_DECK)
						tmp_val /= cost_divisor * 10.0;
				}

				// poisonous and lifesteal affects certain effects (damageEff)
				if (parent_card->is_poisonous)
					tmp_val *= effect->GetPoisonousGrowthFactor();	
				if (parent_card->is_lifesteal)
					tmp_val *= effect->GetLifestealGrowthFactor();

				// overheat consideration
				double overheat_rate = ((double)overheat_threshold - 0.5 * (double)overheat_count)/(double)DEFAULT_OVERHEAT_THRESHOLD; // consider both the temporary remaining and the overall allowable trigger times per pair of turns
				if (overheat_rate < 0.0)
					return 0.0;
				tmp_val *= overheat_rate;

				return tmp_val;
			}
		}
	| srcCondUntargetedEff: BaseUntargetedEff* effect, TargetCond* srccond
		{
			generator 
			{
				unsigned effect_timing_filter = TARGET_TYPE_ANY;
				switch (effect_timing)
				{
					case EFFECT_TIMING_PLAY:
						if (!(self_config & TARGET_IS_SPELL))
							effect_timing_filter = PLAY_CHAR_SRC_FILTER;
						else if (!(self_config & TARGET_ANY_CHAR))
							effect_timing_filter = PLAY_SPELL_SRC_FILTER;
						break;
					case EFFECT_TIMING_DESTROY:
						effect_timing_filter = DESTROY_SRC_FILTER;
						break;
				}

				CondConfig self_config_copy = self_config;
				self_config_copy &= effect_timing_filter;
				AdjustInitConfigWithAnotherConfig(self_config_copy, self_config);
				CondConfig self_instant_config = GetInstantConfigFromInitConfig(self_config_copy);
				
				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->untargeted_eff_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->untargeted_eff_decoder_hidden, false);
					effect = generate BaseUntargetedEff(self_config_copy, effect_timing, effect_depth, give_eff, rep, encoder, hidden_tensor.narrow(0, 0, Base_Untargeted_Eff_Size));
					CondConfig sub_self_config = effect->GetSelfConfig(self_config);
					AdjustInitConfigWithAnotherConfig(self_config_copy, sub_self_config);
					self_instant_config &= sub_self_config;
					// note for source cond the ally bias doesn't matter as it'll be forced as anyAllegiance (the actual card is always ally) 
					srccond = generate TargetCond(self_config_copy, self_instant_config, TARGET_MODE_SOURCE, effect_timing, 1.0, rep, encoder, hidden_tensor.narrow(0, Base_Untargeted_Eff_Size, Cond_Size));
				}
				else
				{
					effect = generate BaseUntargetedEff(self_config_copy, effect_timing, effect_depth, give_eff, rep, encoder, code);
					CondConfig sub_self_config = effect->GetSelfConfig(self_config);
					AdjustInitConfigWithAnotherConfig(self_config_copy, sub_self_config);
					self_instant_config &= sub_self_config;
					// note for source cond the ally bias doesn't matter as it'll be forced as anyAllegiance (the actual card is always ally) 
					srccond = generate TargetCond(self_config_copy, self_instant_config, TARGET_MODE_SOURCE, effect_timing, 1.0, rep, encoder, code);
				}

				self_config |= srccond->GetInitAbilFlag();
			}
			overheat_count = 0;
			overheat_threshold = DEFAULT_OVERHEAT_THRESHOLD;
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_SRC_COND_EFF));
				effect->FillRep(rep);
				srccond->FillRep(rep);
			}
			FillTokens
			{
				effect->FillTokens(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkPunctToken(",", indent_size, false));
				tokens.push_back(mkWordToken("if", indent_size, false));
				tokens.push_back(mkWordToken("itself", indent_size, false));
				tokens.push_back(mkWordToken("is", indent_size, false));
				srccond->FillTokensAlt3(tokens, indent_size, need_starting_newline);
			}
			FillTokensPostfix = effect->FillTokensPostfix(tokens, indent_size);
			Brief = (overheat_count < overheat_threshold ? "" : "(Overheated)");
			Detail
			{
				string overheat_indicator = (overheat_count < overheat_threshold ? "" : "(Overheated)");
				string overheat_counter = (display_overheat_counts ? "(OH:" + IntToStr(overheat_count) + " of " + IntToStr(overheat_threshold) + ")" : "");
				return overheat_indicator + overheat_counter + effect->Detail() + ", if itself is " + srccond->DetailAlt3();
			}
			DetailIndent
			{
				string overheat_indicator = (overheat_count < overheat_threshold ? "" : "(Overheated)");
				string overheat_counter = (display_overheat_counts ? "(OH:" + IntToStr(overheat_count) + " of " + IntToStr(overheat_threshold) + ")" : "");
				return overheat_indicator + overheat_counter + effect->Detail() + ", if itself is " + srccond->DetailAlt3();
			}
			Postfix = effect->Postfix();
			PostfixIndent = effect->PostfixIndent(indent_size);
			CreateNodeHardCopy
			{ 
				srcCondUntargetedEff* effect_copy = new srcCondUntargetedEff(card_copy,
					(BaseUntargetedEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)),
					(TargetCond*)(srccond->CreateNodeHardCopy(card_copy, redir_map)));

				// make sure to also copy overheat status
				effect_copy->overheat_count = overheat_count;
				effect_copy->overheat_threshold = overheat_threshold;

				return effect_copy;
			}
			GetInitAbilFlag = effect->GetInitAbilFlag() | srccond->GetInitAbilFlag();
			GetGlobalSelfConfig
			{
				CondConfig tmp_config = effect->GetGlobalSelfConfig(self_config, effect_timing);
				CondConfig self_config_copy = self_config; // make a copy for making detailed changes when considering the subtree of the srccond (in particular, whether it is a target on the field or some card target)
				self_config_copy &= tmp_config;
				tmp_config &= srccond->GetGlobalSelfConfig(self_config_copy, effect_timing);
				return tmp_config;
			}
			IsForbiddingOwnTurnStart = srccond->IsForbiddingOwnTurnStart(self_config); 
			UntargetedAction
			{ 
				if (srccond->CheckThisValid(parent_card) && overheat_count < overheat_threshold) // do not check whether the source is dying or something, as deathrattle and on-discard effects still needs be triggered
				{
					overheat_count++;
					parent_card->IncContribution(is_cost_constrained);
					effect->UntargetedAction(parent_card, is_cost_constrained);
				}
				#ifndef SUPPRSS_MATCH_PROMPTS
				else
					if (!parent_card->owner->is_exploration)
						cout << "Condition check failed or overheated, not actually activated." << endl;
				#endif
			}
			GetNodeHeuristicEval
			{
				double cost_divisor = 1.0 + (double)parent_card->mana;
				
				double tmp_val = DEFAULT_EFFECT_EVAL;
				if (effect_timing == EFFECT_TIMING_PLAY)
				{
					if (parent_card->card_pos == CARD_POS_AT_LEADER)
						return 0.0; // played leaders cannot be moved or copied
					if (parent_card->card_pos == CARD_POS_AT_FIELD)
						tmp_val /= cost_divisor * 50.0; // slight chance to be played again
					else if (parent_card->card_pos == CARD_POS_AT_HAND)
						tmp_val /= cost_divisor;
					else if (parent_card->card_pos == CARD_POS_AT_DECK)
						tmp_val /= cost_divisor * 10.0;
				}
				else if (effect_timing == EFFECT_TIMING_DESTROY)
				{
					if (parent_card->card_pos == CARD_POS_AT_HAND)
						tmp_val /= cost_divisor * 2.0;
					else if (parent_card->card_pos == CARD_POS_AT_DECK)
						tmp_val /= cost_divisor * 10.0;
				}
				else
				{
					CondConfig src_config = srccond->GetTargetConfig();
					if (!(src_config & TARGET_POS_HAND_OR_DECK))
					{
						double hp_rating = (double)(parent_card->max_hp - parent_card->hp_loss)/10.0;
						double hp_bonus_factor = 1.0 + hp_rating;
						if (effect_timing == EFFECT_TIMING_TURN)
							tmp_val *= hp_bonus_factor; // the longer the minion/leader stays on the field the more impact the effect would have
						
						if (parent_card->card_pos == CARD_POS_AT_HAND)
							tmp_val /= cost_divisor * 2.0;
						else if (parent_card->card_pos == CARD_POS_AT_DECK)
							tmp_val /= cost_divisor * 10.0;
					}
					else if (!(src_config & TARGET_POS_FIELD))
					{
						if (parent_card->card_pos == CARD_POS_AT_LEADER)
							return 0.0; // played leaders cannot be moved or copied
						if (parent_card->card_pos == CARD_POS_AT_FIELD)
							tmp_val /= 25.0; // slight chance to be returned to hand/deck
					}
					else if (!(src_config & TARGET_NOT_HAND))
					{
						if (parent_card->card_pos == CARD_POS_AT_LEADER)
							return 0.0; // played leaders cannot be moved or copied
						if (parent_card->card_pos == CARD_POS_AT_FIELD)
							tmp_val /= 25.0; // slight chance to be returned to hand
						else if (parent_card->card_pos == CARD_POS_AT_DECK)
							tmp_val /= 5.0;
					}
					else if (!(src_config & TARGET_NOT_DECK))
					{
						if (parent_card->card_pos == CARD_POS_AT_LEADER)
							return 0.0; // played leaders cannot be moved or copied
						if (parent_card->card_pos == CARD_POS_AT_FIELD)
							tmp_val /= 25.0; // slight chance to be returned to deck
						else if (parent_card->card_pos == CARD_POS_AT_HAND)
							tmp_val /= 25.0; // slight chance to be returned to deck
					}
				}

				// poisonous and lifesteal affects certain effects (damageEff)
				if (parent_card->is_poisonous)
					tmp_val *= effect->GetPoisonousGrowthFactor();	
				if (parent_card->is_lifesteal)
					tmp_val *= effect->GetLifestealGrowthFactor();

				// overheat consideration
				double overheat_rate = ((double)overheat_threshold - 0.5 * (double)overheat_count)/(double)DEFAULT_OVERHEAT_THRESHOLD; // consider both the temporary remaining and the overall allowable trigger times per pair of turns
				if (overheat_rate < 0.0)
					return 0.0;
				tmp_val *= overheat_rate;

				return tmp_val;
			}
		}

	TargetCond // inside a subtree under TargetCond, all configs are modified in place
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else
				{
					if (!(init_config & TARGET_ANY_CHAR) || !(init_config & TARGET_POS_FIELD)|| !(instant_config & TARGET_ANY_CHAR) || !(instant_config & TARGET_POS_FIELD))
					{
						force cardTargetCond;
					}
					else if (!(init_config & TARGET_POS_HAND_OR_DECK) || !(instant_config & TARGET_POS_HAND_OR_DECK)
						|| (target_mode == TARGET_MODE_PLAY && (!(init_config & TARGET_IS_ALLY) || !(instant_config & TARGET_IS_ALLY) // for the target of an targeted effect, if doesn't allow ally, then it has to target at a character
																|| !(init_config & TARGET_POS_HAND) || !(instant_config & TARGET_POS_HAND)))) // for the target of an targeted effect, if doesn't allow hand, then it has to target at a character
					{
						force charTargetCond;
					}
					else if (encoder)
					{
						torch_Tensor intermediate_code = UnitForward(code, encoder->target_cond_decoder_to_intermediate, false);
						force [GetMaxIndex(ForwardClassifyOut(intermediate_code, encoder->target_cond_decoder_classify))];
					}
				}
			}
		}
	:=
	| charTargetCond: CharTargetCond* cond
		{
			generator
			{
				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->target_cond_decoder_to_intermediate, false);
					cond = generate CharTargetCond(init_config, instant_config, target_mode, effect_timing, ally_bias, rep, encoder, UnitForward(intermediate_code, encoder->target_cond_decoder_hidden, false));
				}
				else
				{
					cond = generate CharTargetCond(init_config, instant_config, target_mode, effect_timing, ally_bias, rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_CHAR_TARGET_COND));
				cond->FillRep(rep);
			}
			FillTokens = cond->FillTokens(tokens, indent_size, need_starting_newline);
			FillTokensAlt1 = cond->FillTokensAlt1(tokens, indent_size, need_starting_newline);
			FillTokensAlt2 = cond->FillTokensAlt2(tokens, indent_size, need_starting_newline);
			FillTokensAlt3
			{
				cond->FillTokensAlt3(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken("on", indent_size, false));
				tokens.push_back(mkWordToken("the", indent_size, false));
				tokens.push_back(mkWordToken("field", indent_size, false));
			}
			Detail = cond->Detail();
			DetailAlt1 = cond->DetailAlt1();
			DetailAlt2 = cond->DetailAlt2();
			DetailAlt3 = cond->DetailAlt3() + " on the field"; // for source condition
			CreateNodeHardCopy = new charTargetCond(card_copy, (CharTargetCond*)(cond->CreateNodeHardCopy(card_copy, redir_map)));
			GetInitAbilFlag = cond->GetInitAbilFlag();
			GetGlobalSelfConfig = cond->GetGlobalSelfConfig(self_config, effect_timing);
			GetLeaderConfig = cond->GetLeaderConfig(); // currently only played cards put constraints on maxMP, which is passed via leader cond
			GetTargetConfig
			{
				CondConfig tmp_config = cond->GetTargetConfig();	
				tmp_config &= FIELD_COND_FILTER;
				return tmp_config;
			}
			IsForbiddingOwnTurnStart = cond->IsForbiddingOwnTurnStart(self_config); 
			CheckPlayValid = parent_card->owner->IsValidCharTarget(z) && cond->CheckPlayValid(x, y, z, parent_card);
			CheckCardValid = (card->card_pos == CARD_POS_AT_LEADER || card->card_pos == CARD_POS_AT_FIELD) && cond->CheckCardValid(card, parent_card);
			CheckThisValid = (parent_card->card_pos == CARD_POS_AT_LEADER || parent_card->card_pos == CARD_POS_AT_FIELD) && cond->CheckThisValid(parent_card);
		}
	| cardTargetCond: CardTargetCond* cond
		{
			generator
			{
				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->target_cond_decoder_to_intermediate, false);
					cond = generate CardTargetCond(init_config, instant_config, target_mode, effect_timing, ally_bias, rep, encoder, UnitForward(intermediate_code, encoder->target_cond_decoder_hidden, false));
				}
				else
				{
					cond = generate CardTargetCond(init_config, instant_config, target_mode, effect_timing, ally_bias, rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_CARD_TARGET_COND));
				cond->FillRep(rep);
			}
			FillTokens = cond->FillTokens(tokens, indent_size, need_starting_newline);
			FillTokensAlt1 = cond->FillTokensAlt1(tokens, indent_size, need_starting_newline);
			FillTokensAlt2 = cond->FillTokensAlt2(tokens, indent_size, need_starting_newline);
			FillTokensAlt3 = cond->FillTokensAlt3(tokens, indent_size, need_starting_newline);
			Detail = cond->Detail();
			DetailAlt1 = cond->DetailAlt1();
			DetailAlt2 = cond->DetailAlt2();
			DetailAlt3 = cond->DetailAlt3(); // for source condition
			CreateNodeHardCopy = new cardTargetCond(card_copy, (CardTargetCond*)(cond->CreateNodeHardCopy(card_copy, redir_map)));
			GetInitAbilFlag = cond->GetInitAbilFlag();
			GetGlobalSelfConfig = cond->GetGlobalSelfConfig(self_config, effect_timing);
			GetTargetConfig
			{
				CondConfig tmp_config = cond->GetTargetConfig();	
				tmp_config &= HAND_OR_DECK_COND_FILTER;
				return tmp_config;
			}
			CheckPlayValid = parent_card->owner->IsValidCardTarget(z) && cond->CheckPlayValid(x, y, z, parent_card);
			CheckCardValid = (card->card_pos == CARD_POS_AT_HAND || card->card_pos == CARD_POS_AT_DECK) && cond->CheckCardValid(card, parent_card);
			CheckThisValid = (parent_card->card_pos == CARD_POS_AT_HAND || parent_card->card_pos == CARD_POS_AT_DECK) && cond->CheckThisValid(parent_card);
		}

	CharTargetCond :=
	| justCharTargetCond: AllegianceCond* alle, CharTypeCond* typecond, AbilCond* abilcond, StatCond* statcond
		{
			generator
			{
				// this part has to be here (instead of in the rules for TargetCond as certain ways of reaching here skip the above rules, e.g. the existence conditions)
				if (target_mode == TARGET_MODE_SOURCE)
				{
					if (effect_timing != EFFECT_TIMING_PLAY)
					{
						// hp or n_atks can naturally change on the field, so the original hp (range) or n_atks (range) is not a trivial setting
						init_config.min_hp = -9;
						init_config.min_n_atks = 0; // note that for n_atks there's an additional case to exclude beyond playing (battlecry), that is the start of own turns, this is deal by other means (IsForbiddingOwnTurnStart) 
					}
					else // effect_timing == EFFECT_TIMING_PLAY
					{
						// note that max hp cannot be reduced by effects (smaller than original hp is only possible if earlier battlecry on this card deals damage to itself, which is not very common)
						if (instant_config.min_hp < init_config.min_hp)
							instant_config.min_hp = init_config.min_hp;
					}
				}
				if (target_mode != TARGET_MODE_SOURCE || effect_timing != EFFECT_TIMING_DESTROY)
				{
					// zero or negative HP should only appear on the source of on destroy effect (deathrattle)
					if (init_config.min_hp < 1)
						init_config.min_hp = 1;
					if (instant_config.min_hp < 1)
						instant_config.min_hp = 1;
				}
				if (target_mode == TARGET_MODE_PLAY)
					instant_config &= PLAY_CHAR_TARGET_FILTER;

				if (!rep && (target_mode == TARGET_MODE_SOURCE && (!(init_config & TARGET_NOT_LEADER) || !(instant_config & TARGET_NOT_LEADER))))
				{
					// source cond on target that must be a leader should only say it is a character (as oppose to a card), as other cond should be covered by leader cond; currently applies for turn start/end effects
					alle = construct anyAllegiance();
					typecond = construct isCharacter();
					abilcond = construct noAbilCond();
					statcond = construct noStatCond();
				}
				else
				{
					if (encoder)
					{
						torch_Tensor intermediate_code = UnitForward(code, encoder->char_target_cond_decoder_to_intermediate, false);
						torch_Tensor classify_tensor = ForwardClassifyOut(intermediate_code, encoder->char_target_cond_decoder_classify);
						torch_Tensor value_tensor = ForwardValueOut(intermediate_code, encoder->char_target_cond_decoder_value);
						alle = generate AllegianceCond(init_config, instant_config, target_mode, effect_timing, ally_bias, rep, encoder, classify_tensor.narrow(0, 0, Alle_Cond_Size));
						typecond = generate CharTypeCond(init_config, instant_config, target_mode, effect_timing, rep, encoder, classify_tensor.narrow(0, Alle_Cond_Size, Char_Type_Cond_Size));
						CondConfig type_config = typecond->GetTargetConfig();
						if (!(init_config & TARGET_NOT_LEADER) || !(instant_config & TARGET_NOT_LEADER))
							type_config.max_hp = 40; // an exception: normally we don't want put put HP cond to be beyond 10, but if it is only leaders then it is fine
						init_config &= type_config; // choice of earlier part may affect what is available later
						instant_config &= type_config; // choice of earlier part may affect what is available later
						abilcond = generate AbilCond(init_config, instant_config, target_mode, effect_timing, rep, encoder, classify_tensor.narrow(0, Alle_Cond_Size + Char_Type_Cond_Size, Abil_Cond_Size));
						torch_Tensor tmp_cat_list[] = { classify_tensor.narrow(0, Alle_Cond_Size + Char_Type_Cond_Size + Abil_Cond_Size, 7), value_tensor }; // 7 is from the 5 classes in StatCond and 2 classes in StatCondVariant
						if (!(instant_config & TARGET_POS_HAND_OR_DECK) && target_mode == TARGET_MODE_SOURCE
							&& typecond->IsCondTrivial() && abilcond->IsCondTrivial()) // do not put trivial condition on source if it is always triggered on the field, e.g. deathrattle (current implementation relies this specific part to resolve it, alle part is always trivial on source mode)
							statcond = generate StatCond(init_config, instant_config, target_mode, effect_timing, true, rep, encoder, torch_cat(tmp_cat_list));
						else
							statcond = generate StatCond(init_config, instant_config, target_mode, effect_timing, false, rep, encoder, torch_cat(tmp_cat_list));
					}
					else
					{
						alle = generate AllegianceCond(init_config, instant_config, target_mode, effect_timing, ally_bias, rep, encoder, code);
						typecond = generate CharTypeCond(init_config, instant_config, target_mode, effect_timing, rep, encoder, code);
						CondConfig type_config = typecond->GetTargetConfig();
						if (!(init_config & TARGET_NOT_LEADER) || !(instant_config & TARGET_NOT_LEADER))
							type_config.max_hp = 40; // an exception: normally we don't want put put HP cond to be beyond 10, but if it is only leaders then it is fine
						init_config &= type_config; // choice of earlier part may affect what is available later
						instant_config &= type_config; // choice of earlier part may affect what is available later
						abilcond = generate AbilCond(init_config, instant_config, target_mode, effect_timing, rep, encoder, code);
						if (!(instant_config & TARGET_POS_HAND_OR_DECK) && target_mode == TARGET_MODE_SOURCE
							&& typecond->IsCondTrivial() && abilcond->IsCondTrivial()) // do not put trivial condition on source if it is always triggered on the field, e.g. deathrattle (current implementation relies this specific part to resolve it, alle part is always trivial on source mode)
							statcond = generate StatCond(init_config, instant_config, target_mode, effect_timing, true, rep, encoder, code);
						else
							statcond = generate StatCond(init_config, instant_config, target_mode, effect_timing, false, rep, encoder, code);
					}
				}
			}
			FillRep
			{
				// this is just a pass-down node
				alle->FillRep(rep);
				typecond->FillRep(rep);
				abilcond->FillRep(rep);
				statcond->FillRep(rep);
			}
			FillTokens
			{
				alle->FillTokens(tokens, indent_size, need_starting_newline);
				typecond->FillTokens(tokens, indent_size, need_starting_newline);
				abilcond->FillTokens(tokens, indent_size, need_starting_newline);
				statcond->FillTokens(tokens, indent_size, need_starting_newline);
			}
			FillTokensAlt1
			{
				if ((alle->Detail()).length() == 0)
				{
					typecond->FillTokensAlt1(tokens, indent_size, need_starting_newline);
				}
				else
				{
					alle->FillTokensAlt1(tokens, indent_size, need_starting_newline);
					typecond->FillTokens(tokens, indent_size, need_starting_newline);
				}
				abilcond->FillTokens(tokens, indent_size, need_starting_newline);
				statcond->FillTokens(tokens, indent_size, need_starting_newline);
			}
			FillTokensAlt2
			{
				alle->FillTokens(tokens, indent_size, need_starting_newline);
				typecond->FillTokensAlt2(tokens, indent_size, need_starting_newline);
				abilcond->FillTokens(tokens, indent_size, need_starting_newline);
				statcond->FillTokens(tokens, indent_size, need_starting_newline);
			}
			FillTokensAlt3
			{
				typecond->FillTokensAlt1(tokens, indent_size, need_starting_newline);
				abilcond->FillTokens(tokens, indent_size, need_starting_newline);
				statcond->FillTokens(tokens, indent_size, need_starting_newline);
			}
			Detail = alle->Detail() + typecond->Detail() + abilcond->Detail() + statcond->Detail(); // no leading article (for random targetting etc.)
			DetailAlt1 = ((alle->Detail()).length() == 0 ? typecond->DetailAlt1() : alle->DetailAlt1() + typecond->Detail()) + abilcond->Detail() + statcond->Detail();
			DetailAlt2 = alle->Detail() + typecond->DetailAlt2() + abilcond->Detail() + statcond->Detail(); // plural
			DetailAlt3 = typecond->DetailAlt1() + abilcond->Detail() + statcond->Detail(); // source cond always does not need to check for allegiance (any allegiance but in fact is always ally)
			CreateNodeHardCopy = new justCharTargetCond(card_copy,
				(AllegianceCond*)(alle->CreateNodeHardCopy(card_copy, redir_map)),
				(CharTypeCond*)(typecond->CreateNodeHardCopy(card_copy, redir_map)),
				(AbilCond*)(abilcond->CreateNodeHardCopy(card_copy, redir_map)),
				(StatCond*)(statcond->CreateNodeHardCopy(card_copy, redir_map)));
			GetInitAbilFlag = abilcond->GetInitAbilFlag();
			GetGlobalSelfConfig
			{ 
				// the alle part doesn't matter as it is only relevant in source cond where there is no need to check for alle (always generated as any alle but effectively always ally)
				// no need to update self_config in the middle as only whether or not it is on field matters
				CondConfig tmp_config = typecond->GetGlobalSelfConfig(self_config, effect_timing);
				tmp_config &= abilcond->GetGlobalSelfConfig(self_config, effect_timing);
				tmp_config &= statcond->GetGlobalSelfConfig(self_config, effect_timing);
				return tmp_config;
			}
			GetLeaderConfig = statcond->GetLeaderConfig();
			GetTargetConfig
			{
				CondConfig tmp_config = alle->GetTargetConfig();
				tmp_config &= typecond->GetTargetConfig();
				tmp_config &= abilcond->GetTargetConfig();
				tmp_config &= statcond->GetTargetConfig();
				return tmp_config;
			}
			IsForbiddingOwnTurnStart = statcond->IsForbiddingOwnTurnStart(self_config); 
			CheckPlayValid = alle->CheckPlayValid(x, y, z, parent_card) && typecond->CheckPlayValid(x, y, z, parent_card) && abilcond->CheckPlayValid(x, y, z, parent_card) && statcond->CheckPlayValid(x, y, z, parent_card);
			CheckCardValid = alle->CheckCardValid(card, parent_card) && typecond->CheckCardValid(card, parent_card) && abilcond->CheckCardValid(card, parent_card) && statcond->CheckCardValid(card, parent_card);
			CheckThisValid = alle->CheckThisValid(parent_card) && typecond->CheckThisValid(parent_card) && abilcond->CheckThisValid(parent_card) && statcond->CheckThisValid(parent_card);
		}

	CharTypeCond
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else
				{
					// note that the card type doesn't change (while minion type can)
					// if it isn't a minion (so it is a leader) then isCharacter is the only choice
					if (!(init_config & TARGET_IS_MINION) || !(instant_config & TARGET_IS_MINION))
						force isCharacter;

					if (encoder)
					{
						double choice_code_adjust[5] = {0.0};
						
						//  if the condition doesn't allow leaders, we don't want to include leader as a possibility (well we could always say a charactor for source condition but it would sound better to say a minion, or a beast minion etc.)
						if (!(init_config & TARGET_IS_LEADER) || !(instant_config & TARGET_IS_LEADER))
							choice_code_adjust[CHOICE_IS_CHARACTER] = -1e10;

						// note forbidding a minion type meaning not specifically conditioned on that type, but the effect may still be applicable for that type
						// also, we don't want the minion condition to be too trivial (initially true)
						if (!(init_config & TARGET_NOT_BEAST) || !(instant_config & TARGET_IS_BEAST))
							choice_code_adjust[CHOICE_IS_BEAST] = -1e10;
						if (!(init_config & TARGET_NOT_DRAGON) || !(instant_config & TARGET_IS_DRAGON))
							choice_code_adjust[CHOICE_IS_DRAGON] = -1e10;
						if (!(init_config & TARGET_NOT_DEMON) || !(instant_config & TARGET_IS_DEMON))
							choice_code_adjust[CHOICE_IS_DEMON] = -1e10;

						torch_Tensor adjust_tensor = CreateVectorTensorFromArray(choice_code_adjust, 5);
						torch_Tensor final_tensor = code + adjust_tensor;
						force [GetMaxIndex(final_tensor)];
					}
					else
					{
						//  if the condition doesn't allow leaders, we don't want to include leader as a possibility (well we could always say a charactor for source condition but it would sound better to say a minion, or a beast minion etc.)
						if (!(init_config & TARGET_IS_LEADER) || !(instant_config & TARGET_IS_LEADER))
							forbid isCharacter;

						// note forbidding a minion type meaning not specifically conditioned on that type, but the effect may still be applicable for that type
						// also, we don't want the minion condition to be too trivial (initially true)
						if (!(init_config & TARGET_NOT_BEAST) || !(instant_config & TARGET_IS_BEAST))
							forbid isBeast;
						if (!(init_config & TARGET_NOT_DRAGON) || !(instant_config & TARGET_IS_DRAGON))
							forbid isDragon;
						if (!(init_config & TARGET_NOT_DEMON) || !(instant_config & TARGET_IS_DEMON))
							forbid isDemon;
					}
				}
			}
		}
	:=
	| isCharacter:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_IS_CHARACTER));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("character", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken("character", indent_size, false));
			}
			FillTokensAlt2
			{
				tokens.push_back(mkWordToken("characters", indent_size, false));
			}
			Detail = "character";
			DetailAlt1 = "a character";
			DetailAlt2 = "characters";
			CreateNodeHardCopy = new isCharacter(card_copy);
			IsCondTrivial = true;
			GetGlobalSelfConfig = GetDefaultConfig(); // for GlobalSelfConfig, as it is not effect specific, we don't need to do the special treatment as in GetTargetConfig in this case
			GetTargetConfig
			{
				CondConfig tmp_config = CHAR_COND_FILTER; // this will call the constructor (so no need to explicitly call GetFlagConfig here), similar for cases later
				tmp_config.max_hp = 10; // make this to be also limited to 10, for convenience of the config system (otherwise it may try to give minions HP conditions that is larger than 10, which would be very hard to resolve)
				if (tmp_config.min_hp > 10)
					tmp_config.min_hp = 10;
				return tmp_config;
			}
		}
	| isMinion:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_IS_MINION));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("minion", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken("minion", indent_size, false));
			}
			FillTokensAlt2
			{
				tokens.push_back(mkWordToken("minions", indent_size, false));
			}
			Detail = "minion";
			DetailAlt1 = "a minion";
			DetailAlt2 = "minions";
			CreateNodeHardCopy = new isMinion(card_copy);
			IsCondTrivial = true; // card type cannot change, so for our purpose, this is also considered trivial
			GetTargetConfig
			{
				CondConfig tmp_config =	MINION_COND_FILTER;
				tmp_config.max_cost = 7;
				if (tmp_config.min_cost > 7)
					tmp_config.min_cost = 7;
				tmp_config.max_hp = 10;
				if (tmp_config.min_hp > 10)
					tmp_config.min_hp = 10;
				return tmp_config;
			}
			CheckPlayValid = parent_card->owner->IsValidMinionTarget(z);
			CheckCardValid = card->card_type == MINION_CARD;
			CheckThisValid = parent_card->card_type == MINION_CARD;
		}
	| isBeast:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_IS_BEAST));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Beast", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken("Beast", indent_size, false));
			}
			FillTokensAlt2
			{
				tokens.push_back(mkWordToken("Beasts", indent_size, false));
			}
			Detail = "Beast";
			DetailAlt1 = "a Beast";
			DetailAlt2 = "Beasts";
			CreateNodeHardCopy = new isBeast(card_copy);
			GetGlobalSelfConfig
			{
				CondConfig tmp_config = NOT_BEAST_COND_FILTER;
				tmp_config.max_cost = 7;
				if (tmp_config.min_cost > 7)
					tmp_config.min_cost = 7;
				tmp_config.max_hp = 10;
				if (tmp_config.min_hp > 10)
					tmp_config.min_hp = 10;
				return tmp_config;
			}
			GetTargetConfig
			{
				CondConfig tmp_config =	BEAST_COND_FILTER;
				tmp_config.max_cost = 7;
				if (tmp_config.min_cost > 7)
					tmp_config.min_cost = 7;
				tmp_config.max_hp = 10;
				if (tmp_config.min_hp > 10)
					tmp_config.min_hp = 10;
				return tmp_config;
			}
			CheckPlayValid = parent_card->owner->IsValidMinionTarget(z) && parent_card->owner->GetTargetCard(z)->minion_type == BEAST_MINION;
			CheckCardValid = card->minion_type == BEAST_MINION;
			CheckThisValid = parent_card->minion_type == BEAST_MINION;
		}
	| isDragon:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_IS_DRAGON));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Dragon", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken("Dragon", indent_size, false));
			}
			FillTokensAlt2
			{
				tokens.push_back(mkWordToken("Dragons", indent_size, false));
			}
			Detail = "Dragon";
			DetailAlt1 = "a Dragon";
			DetailAlt2 = "Dragons";
			CreateNodeHardCopy = new isDragon(card_copy);
			GetGlobalSelfConfig
			{
				CondConfig tmp_config = NOT_DRAGON_COND_FILTER;
				tmp_config.max_cost = 7;
				if (tmp_config.min_cost > 7)
					tmp_config.min_cost = 7;
				tmp_config.max_hp = 10;
				if (tmp_config.min_hp > 10)
					tmp_config.min_hp = 10;
				return tmp_config;
			}
			GetTargetConfig
			{
				CondConfig tmp_config =	DRAGON_COND_FILTER;
				tmp_config.max_cost = 7;
				if (tmp_config.min_cost > 7)
					tmp_config.min_cost = 7;
				tmp_config.max_hp = 10;
				if (tmp_config.min_hp > 10)
					tmp_config.min_hp = 10;
				return tmp_config;
			}
			CheckPlayValid = parent_card->owner->IsValidMinionTarget(z) && parent_card->owner->GetTargetCard(z)->minion_type == DRAGON_MINION;
			CheckCardValid = card->minion_type == DRAGON_MINION;
			CheckThisValid = parent_card->minion_type == DRAGON_MINION;
		}
	| isDemon:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_IS_DEMON));
			}	
			FillTokens
			{
				tokens.push_back(mkWordToken("Demon", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken("Demon", indent_size, false));
			}
			FillTokensAlt2
			{
				tokens.push_back(mkWordToken("Demons", indent_size, false));
			}
			Detail = "Demon";
			DetailAlt1 = "a Demon";
			DetailAlt2 = "Demons";
			CreateNodeHardCopy = new isDemon(card_copy);
			GetGlobalSelfConfig
			{
				CondConfig tmp_config = NOT_DEMON_COND_FILTER;
				tmp_config.max_cost = 7;
				if (tmp_config.min_cost > 7)
					tmp_config.min_cost = 7;
				tmp_config.max_hp = 10;
				if (tmp_config.min_hp > 10)
					tmp_config.min_hp = 10;
				return tmp_config;
			}
			GetTargetConfig
			{
				CondConfig tmp_config =	DEMON_COND_FILTER;
				tmp_config.max_cost = 7;
				if (tmp_config.min_cost > 7)
					tmp_config.min_cost = 7;
				tmp_config.max_hp = 10;
				if (tmp_config.min_hp > 10)
					tmp_config.min_hp = 10;
				return tmp_config;
			}
			CheckPlayValid = parent_card->owner->IsValidMinionTarget(z) && parent_card->owner->GetTargetCard(z)->minion_type == DEMON_MINION;
			CheckCardValid = card->minion_type == DEMON_MINION;
			CheckThisValid = parent_card->minion_type == DEMON_MINION;
		}

	CardTargetCond :=
	| justCardTargetCond: CardPosCond* pos, AllegianceCond* alle, CardTypeCond* typecond, AbilCond* abilcond, StatCond* statcond
		{
			generator
			{
				// this part has to be here (instead of in the rules for TargetCond as certain ways of reaching here skip the above rules, e.g. the existence conditions)
				// note that max hp cannot be reduced by effects
				if (instant_config.min_hp < init_config.min_hp)
					instant_config.min_hp = init_config.min_hp;
				// zero or negative HP should only appear on the source of on destroy effect (deathrattle), never happen to card target
				if (init_config.min_hp < 1)
					init_config.min_hp = 1;
				if (instant_config.min_hp < 1)
					instant_config.min_hp = 1;
				if (target_mode == TARGET_MODE_PLAY)
					instant_config &= PLAY_CARD_TARGET_FILTER;

				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->card_target_cond_decoder_to_intermediate, false);
					torch_Tensor classify_tensor = ForwardClassifyOut(intermediate_code, encoder->card_target_cond_decoder_classify);
					torch_Tensor value_tensor = ForwardValueOut(intermediate_code, encoder->card_target_cond_decoder_value);
					pos = generate CardPosCond(init_config, instant_config, target_mode, effect_timing, rep, encoder, classify_tensor.narrow(0, 0, Card_Pos_Cond_Size)); // it is important not to update config with this part (or at least should not remove the bit on TARGET_POS_FIELD), as it is used for the trivialibility mechanism below
					alle = generate AllegianceCond(init_config, instant_config, target_mode, effect_timing, ally_bias, rep, encoder, classify_tensor.narrow(0, Card_Pos_Cond_Size, Alle_Cond_Size));
					typecond = generate CardTypeCond(init_config, instant_config, target_mode, effect_timing, rep, encoder, classify_tensor.narrow(0, Card_Pos_Cond_Size + Alle_Cond_Size, Card_Type_Cond_Size));
					CondConfig type_config = typecond->GetTargetConfig();
					if (!(init_config & TARGET_NOT_LEADER) || !(instant_config & TARGET_NOT_LEADER))
						type_config.max_hp = 40; // an exception: normally we don't want put put HP cond to be beyond 10, but if it is only leaders then it is fine
					init_config &= type_config; // choice of earlier part may affect what is available later
					instant_config &= type_config; // choice of earlier part may affect what is available later
					abilcond = generate AbilCond(init_config, instant_config, target_mode, effect_timing, rep, encoder, classify_tensor.narrow(0, Card_Pos_Cond_Size + Alle_Cond_Size + Card_Type_Cond_Size, Abil_Cond_Size));
					torch_Tensor tmp_cat_list[] = { classify_tensor.narrow(0, Card_Pos_Cond_Size + Alle_Cond_Size + Card_Type_Cond_Size + Abil_Cond_Size, 7), value_tensor }; // 7 is from the 5 classes in StatCond and 2 classes in StatCondVariant
					if (!(instant_config & TARGET_POS_FIELD) && target_mode == TARGET_MODE_SOURCE
						&& pos->IsCondTrivial() && typecond->IsCondTrivial() && abilcond->IsCondTrivial()) // note: only using instant_config would not say "hand_or_deck" is trivial for a spell card (as that would be in the init_config part), and that is what we want (still says "a card at hand or deck" even it's always true for a spell card); not sure if this would force nontrival for any actual case but just in case
						statcond = generate StatCond(init_config, instant_config, target_mode, effect_timing, true, rep, encoder, torch_cat(tmp_cat_list));
					else
						statcond = generate StatCond(init_config, instant_config, target_mode, effect_timing, false, rep, encoder, torch_cat(tmp_cat_list));
				}
				else
				{
					pos = generate CardPosCond(init_config, instant_config, target_mode, effect_timing, rep, encoder, code); // it is important not to update config with this part (or at least should not remove the bit on TARGET_POS_FIELD), as it is used for the trivialibility mechanism below
					alle = generate AllegianceCond(init_config, instant_config, target_mode, effect_timing, ally_bias, rep, encoder, code);
					typecond = generate CardTypeCond(init_config, instant_config, target_mode, effect_timing, rep, encoder, code);
					CondConfig type_config = typecond->GetTargetConfig();
					if (!(init_config & TARGET_NOT_LEADER) || !(instant_config & TARGET_NOT_LEADER))
						type_config.max_hp = 40; // an exception: normally we don't want put put HP cond to be beyond 10, but if it is only leaders then it is fine
					init_config &= type_config; // choice of earlier part may affect what is available later
					instant_config &= type_config; // choice of earlier part may affect what is available later
					abilcond = generate AbilCond(init_config, instant_config, target_mode, effect_timing, rep, encoder, code);
					if (!(instant_config & TARGET_POS_FIELD) && target_mode == TARGET_MODE_SOURCE
						&& pos->IsCondTrivial() && typecond->IsCondTrivial() && abilcond->IsCondTrivial()) // note: only using instant_config would not say "hand_or_deck" is trivial for a spell card (as that would be in the init_config part), and that is what we want (still says "a card at hand or deck" even it's always true for a spell card); not sure if this would force nontrival for any actual case but just in case
						statcond = generate StatCond(init_config, instant_config, target_mode, effect_timing, true, rep, encoder, code);
					else
						statcond = generate StatCond(init_config, instant_config, target_mode, effect_timing, false, rep, encoder, code);
				}
			}
			FillRep
			{
				// this is just a pass-down node
				pos->FillRep(rep);
				alle->FillRep(rep);
				typecond->FillRep(rep);
				abilcond->FillRep(rep);
				statcond->FillRep(rep);
			}
			FillTokens
			{
				typecond->FillTokens(tokens, indent_size, need_starting_newline);
				abilcond->FillTokens(tokens, indent_size, need_starting_newline);
				statcond->FillTokens(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken("at", indent_size, false));
				alle->FillTokensAlt3(tokens, indent_size, need_starting_newline);
				pos->FillTokens(tokens, indent_size, need_starting_newline);
			}
			FillTokensAlt1
			{
				typecond->FillTokensAlt1(tokens, indent_size, need_starting_newline);
				abilcond->FillTokens(tokens, indent_size, need_starting_newline);
				statcond->FillTokens(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken("at", indent_size, false));
				alle->FillTokensAlt3(tokens, indent_size, need_starting_newline);
				pos->FillTokens(tokens, indent_size, need_starting_newline);
			}
			FillTokensAlt2
			{
				typecond->FillTokensAlt2(tokens, indent_size, need_starting_newline);
				abilcond->FillTokens(tokens, indent_size, need_starting_newline);
				statcond->FillTokens(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken("at", indent_size, false));
				alle->FillTokensAlt2(tokens, indent_size, need_starting_newline);
				if ((alle->Detail()).length() == 0)
					pos->FillTokensAlt2(tokens, indent_size, need_starting_newline);
				else
					pos->FillTokensAlt1(tokens, indent_size, need_starting_newline);
			}
			FillTokensAlt3
			{
				typecond->FillTokensAlt1(tokens, indent_size, need_starting_newline);
				abilcond->FillTokens(tokens, indent_size, need_starting_newline);
				statcond->FillTokens(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken("at", indent_size, false));
				pos->FillTokens(tokens, indent_size, need_starting_newline);
			}
			Detail = typecond->Detail() + abilcond->Detail() + statcond->Detail() + " at " + alle->DetailAlt3() + pos->Detail(); // no leading article (for random targetting etc.)
			DetailAlt1 = typecond->DetailAlt1() + abilcond->Detail() + statcond->Detail() + " at " + alle->DetailAlt3() + pos->Detail();
			DetailAlt2 = typecond->DetailAlt2() + abilcond->Detail() + statcond->Detail() + " at " + alle->DetailAlt2() + ((alle->Detail()).length() == 0 ? pos->DetailAlt2() : pos->DetailAlt1()); // plural
			DetailAlt3 = typecond->DetailAlt1() + abilcond->Detail() + statcond->Detail() + " at " + pos->Detail(); // source cond always does not need to check for allegiance (any allegiance but in fact is always ally)
			CreateNodeHardCopy = new justCardTargetCond(card_copy,
				(CardPosCond*)(pos->CreateNodeHardCopy(card_copy, redir_map)),
				(AllegianceCond*)(alle->CreateNodeHardCopy(card_copy, redir_map)),
				(CardTypeCond*)(typecond->CreateNodeHardCopy(card_copy, redir_map)),
				(AbilCond*)(abilcond->CreateNodeHardCopy(card_copy, redir_map)),
				(StatCond*)(statcond->CreateNodeHardCopy(card_copy, redir_map)));
			GetInitAbilFlag = abilcond->GetInitAbilFlag();
			GetGlobalSelfConfig
			{
				// the pos part doesn't matter as pos isn't giving any globally meaningful constraints 
				// the alle part doesn't matter as it is only relevant in source cond where there is no need to check for alle (always generated as any alle but effectively always ally)
				// no need to update self_config in the middle as only whether or not it is on field matters
				CondConfig tmp_config = typecond->GetGlobalSelfConfig(self_config, effect_timing);
				tmp_config &= abilcond->GetGlobalSelfConfig(self_config, effect_timing);
				tmp_config &= statcond->GetGlobalSelfConfig(self_config, effect_timing);
				return tmp_config;
			}
			GetTargetConfig
			{
				CondConfig tmp_config = pos->GetTargetConfig();
				tmp_config &= alle->GetTargetConfig();
				tmp_config &= typecond->GetTargetConfig();
				tmp_config &= abilcond->GetTargetConfig();
				tmp_config &= statcond->GetTargetConfig();
				return tmp_config;
			}
			CheckPlayValid = pos->CheckPlayValid(x, y, z, parent_card) && alle->CheckPlayValid(x, y, z, parent_card) && typecond->CheckPlayValid(x, y, z, parent_card) && abilcond->CheckPlayValid(x, y, z, parent_card) && statcond->CheckPlayValid(x, y, z, parent_card);
			CheckCardValid = pos->CheckCardValid(card, parent_card) && alle->CheckCardValid(card, parent_card) && typecond->CheckCardValid(card, parent_card) && abilcond->CheckCardValid(card, parent_card) && statcond->CheckCardValid(card, parent_card);
			CheckThisValid = pos->CheckThisValid(parent_card) && alle->CheckThisValid(parent_card) && typecond->CheckThisValid(parent_card) && abilcond->CheckThisValid(parent_card) && statcond->CheckThisValid(parent_card);
		}

	CardTypeCond
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else if (encoder)
				{
					double choice_code_adjust[7] = {0.0};
						
					// note that card type doesn't change (while minion type can)
					// so don't just require on card type if it is source condition (somewhat different from the CardTypeCond mechanism just because there minion sounds better to say than character when it applies)
					if (target_mode == TARGET_MODE_SOURCE)
					{
						choice_code_adjust[CHOICE_IS_LEADER_CARD] = choice_code_adjust[CHOICE_IS_MINION_CARD] = choice_code_adjust[CHOICE_IS_SPELL_CARD] = -1e10;
						if (!(init_config & TARGET_IS_MINION) || !(instant_config & TARGET_IS_MINION))
							force isCard; // as all the detailed minion types are also forbidden in this case
					}
					else
					{
						// forbid card type if it is not allowed, also forbidding types that contains the type (isCard), as this is not the case of source condition
						if (!(init_config & TARGET_IS_LEADER) || !(instant_config & TARGET_IS_LEADER))
							choice_code_adjust[CHOICE_IS_CARD] = choice_code_adjust[CHOICE_IS_LEADER_CARD] = -1e10;
						if (!(init_config & TARGET_IS_MINION) || !(instant_config & TARGET_IS_MINION))
							choice_code_adjust[CHOICE_IS_CARD] = choice_code_adjust[CHOICE_IS_MINION_CARD] =
								choice_code_adjust[CHOICE_IS_BEAST_CARD] = choice_code_adjust[CHOICE_IS_DRAGON_CARD] = choice_code_adjust[CHOICE_IS_DEMON_CARD] = -1e10;
						if (!(init_config & TARGET_IS_SPELL) || !(instant_config & TARGET_IS_SPELL))
							choice_code_adjust[CHOICE_IS_CARD] = choice_code_adjust[CHOICE_IS_SPELL_CARD] = -1e10;
					}

					// note forbidding a minion type meaning not specifically conditioned on that type, but the effect may still be applicable for that type
					// also, we don't want the minion condition to be too trivial (initially true)
					if (!(init_config & TARGET_NOT_BEAST) || !(instant_config & TARGET_IS_BEAST))
						choice_code_adjust[CHOICE_IS_BEAST_CARD] = -1e10;
					if (!(init_config & TARGET_NOT_DRAGON) || !(instant_config & TARGET_IS_DRAGON))
						choice_code_adjust[CHOICE_IS_DRAGON_CARD] = -1e10;
					if (!(init_config & TARGET_NOT_DEMON) || !(instant_config & TARGET_IS_DEMON))
						choice_code_adjust[CHOICE_IS_DEMON_CARD] = -1e10;

					torch_Tensor adjust_tensor = CreateVectorTensorFromArray(choice_code_adjust, 7);
					torch_Tensor final_tensor = code + adjust_tensor;
					force [GetMaxIndex(final_tensor)];
				}
				else
				{
					// note that card type doesn't change (while minion type can)
					// so don't just require on card type if it is source condition (somewhat different from the CardTypeCond mechanism just because there minion sounds better to say than character when it applies)
					if (target_mode == TARGET_MODE_SOURCE)
					{
						forbid isLeaderCard, isMinionCard, isSpellCard;
						if (!(init_config & TARGET_IS_MINION) || !(instant_config & TARGET_IS_MINION))
							force isCard; // as all the detailed minion types are also forbidden in this case
					}
					else
					{
						// forbid card type if it is not allowed, also forbidding types that contains the type (isCard), as this is not the case of source condition
						if (!(init_config & TARGET_IS_LEADER) || !(instant_config & TARGET_IS_LEADER))
							forbid isCard, isLeaderCard;
						if (!(init_config & TARGET_IS_MINION) || !(instant_config & TARGET_IS_MINION))
							forbid isCard, isMinionCard, isBeastCard, isDragonCard, isDemonCard;
						if (!(init_config & TARGET_IS_SPELL) || !(instant_config & TARGET_IS_SPELL))
							forbid isCard, isSpellCard;
					}

					// note forbidding a minion type meaning not specifically conditioned on that type, but the effect may still be applicable for that type
					// also, we don't want the minion condition to be too trivial (initially true)
					if (!(init_config & TARGET_NOT_BEAST) || !(instant_config & TARGET_IS_BEAST))
						forbid isBeastCard;
					if (!(init_config & TARGET_NOT_DRAGON) || !(instant_config & TARGET_IS_DRAGON))
						forbid isDragonCard;
					if (!(init_config & TARGET_NOT_DEMON) || !(instant_config & TARGET_IS_DEMON))
						forbid isDemonCard;
				}
			}
		}
	:=
	| isCard:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_IS_CARD));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("card", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken("card", indent_size, false));
			}
			FillTokensAlt2
			{
				tokens.push_back(mkWordToken("cards", indent_size, false));
			}
			Detail = "card";
			DetailAlt1 = "a card";
			DetailAlt2 = "cards";
			CreateNodeHardCopy = new isCard(card_copy);
			IsCondTrivial = true;
			GetGlobalSelfConfig = GetDefaultConfig(); // for GlobalSelfConfig, as it is not effect specific, we don't need to do the special treatment as in GetTargetConfig in this case
			GetTargetConfig
			{
				CondConfig tmp_config =	TARGET_TYPE_ANY;
				tmp_config.max_hp = 10; // make this to be also limited to 10, for convenience of the config system (otherwise it may try to give minions HP conditions that is larger than 10, which would be very hard to resolve, potentiall through nested effects)
				if (tmp_config.min_hp > 10)
					tmp_config.min_hp = 10;
				return tmp_config;
			}
		}
	| isLeaderCard:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_IS_LEADER_CARD));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("leader", indent_size, false));
				tokens.push_back(mkWordToken("card", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken("leader", indent_size, false));
				tokens.push_back(mkWordToken("card", indent_size, false));
			}
			FillTokensAlt2
			{
				tokens.push_back(mkWordToken("leader", indent_size, false));
				tokens.push_back(mkWordToken("cards", indent_size, false));
			}
			Detail = "leader card";
			DetailAlt1 = "a leader card";
			DetailAlt2 = "leader cards";
			CreateNodeHardCopy = new isLeaderCard(card_copy);
			IsCondTrivial = true; // card type cannot change, so for our purpose, this is also considered trivial
			GetTargetConfig
			{
				CondConfig tmp_config = LEADER_COND_FILTER;
				tmp_config.min_cost = 8;
				if (tmp_config.max_cost < 8)
					tmp_config.max_cost = 8;
				return tmp_config;
			}
			CheckPlayValid = parent_card->owner->GetTargetCard(z)->card_type == LEADER_CARD;
			CheckCardValid = card->card_type == LEADER_CARD;
			CheckThisValid = parent_card->card_type == LEADER_CARD;
		}
	| isMinionCard:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_IS_MINION_CARD));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("minion", indent_size, false));
				tokens.push_back(mkWordToken("card", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken("minion", indent_size, false));
				tokens.push_back(mkWordToken("card", indent_size, false));
			}
			FillTokensAlt2
			{
				tokens.push_back(mkWordToken("minion", indent_size, false));
				tokens.push_back(mkWordToken("cards", indent_size, false));
			}
			Detail = "minion card";
			DetailAlt1 = "a minion card";
			DetailAlt2 = "minion cards";
			CreateNodeHardCopy = new isMinionCard(card_copy);
			IsCondTrivial = true; // card type cannot change, so for our purpose, this is also considered trivial
			GetTargetConfig
			{
				CondConfig tmp_config =	MINION_COND_FILTER;
				tmp_config.max_cost = 7;
				if (tmp_config.min_cost > 7)
					tmp_config.min_cost = 7;
				tmp_config.max_hp = 10;
				if (tmp_config.min_hp > 10)
					tmp_config.min_hp = 10;
				return tmp_config;
			}
			CheckPlayValid = parent_card->owner->GetTargetCard(z)->card_type == MINION_CARD;
			CheckCardValid = card->card_type == MINION_CARD;
			CheckThisValid = parent_card->card_type == MINION_CARD;
		}
	| isSpellCard:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_IS_SPELL_CARD));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("spell", indent_size, false));
				tokens.push_back(mkWordToken("card", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken("spell", indent_size, false));
				tokens.push_back(mkWordToken("card", indent_size, false));
			}
			FillTokensAlt2
			{
				tokens.push_back(mkWordToken("spell", indent_size, false));
				tokens.push_back(mkWordToken("cards", indent_size, false));
			}
			Detail = "spell card";
			DetailAlt1 = "a spell card";
			DetailAlt2 = "spell cards";
			CreateNodeHardCopy = new isSpellCard(card_copy);
			IsCondTrivial = true; // card type cannot change, so for our purpose, this is also considered trivial
			GetTargetConfig
			{
				CondConfig tmp_config = SPELL_COND_FILTER;
				tmp_config.max_cost = 7;
				if (tmp_config.min_cost > 7)
					tmp_config.min_cost = 7;
				return tmp_config;
			}
			CheckPlayValid = parent_card->owner->GetTargetCard(z)->card_type == SPELL_CARD;
			CheckCardValid = card->card_type == SPELL_CARD;
			CheckThisValid = parent_card->card_type == SPELL_CARD;
		}
	| isBeastCard:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_IS_BEAST_CARD));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Beast", indent_size, false));
				tokens.push_back(mkWordToken("card", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken("Beast", indent_size, false));
				tokens.push_back(mkWordToken("card", indent_size, false));
			}
			FillTokensAlt2
			{
				tokens.push_back(mkWordToken("Beast", indent_size, false));
				tokens.push_back(mkWordToken("cards", indent_size, false));
			}
			Detail = "Beast card";
			DetailAlt1 = "a Beast card";
			DetailAlt2 = "Beast cards";
			CreateNodeHardCopy = new isBeastCard(card_copy);
			GetGlobalSelfConfig
			{
				CondConfig tmp_config =	NOT_BEAST_COND_FILTER;
				tmp_config.max_cost = 7;
				if (tmp_config.min_cost > 7)
					tmp_config.min_cost = 7;
				tmp_config.max_hp = 10;
				if (tmp_config.min_hp > 10)
					tmp_config.min_hp = 10;
				return tmp_config;
			}
			GetTargetConfig
			{
				CondConfig tmp_config =	BEAST_COND_FILTER;
				tmp_config.max_cost = 7;
				if (tmp_config.min_cost > 7)
					tmp_config.min_cost = 7;
				tmp_config.max_hp = 10;
				if (tmp_config.min_hp > 10)
					tmp_config.min_hp = 10;
				return tmp_config;
			}
			CheckPlayValid = parent_card->owner->GetTargetCard(z)->minion_type == BEAST_MINION;
			CheckCardValid = card->minion_type == BEAST_MINION;
			CheckThisValid = parent_card->minion_type == BEAST_MINION;
		}
	| isDragonCard:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_IS_DRAGON_CARD));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Dragon", indent_size, false));
				tokens.push_back(mkWordToken("card", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken("Dragon", indent_size, false));
				tokens.push_back(mkWordToken("card", indent_size, false));
			}
			FillTokensAlt2
			{
				tokens.push_back(mkWordToken("Dragon", indent_size, false));
				tokens.push_back(mkWordToken("cards", indent_size, false));
			}
			Detail = "Dragon card";
			DetailAlt1 = "a Dragon card";
			DetailAlt2 = "Dragon cards";
			CreateNodeHardCopy = new isDragonCard(card_copy);
			GetGlobalSelfConfig
			{
				CondConfig tmp_config =	NOT_DRAGON_COND_FILTER;
				tmp_config.max_cost = 7;
				if (tmp_config.min_cost > 7)
					tmp_config.min_cost = 7;
				tmp_config.max_hp = 10;
				if (tmp_config.min_hp > 10)
					tmp_config.min_hp = 10;
				return tmp_config;
			}
			GetTargetConfig
			{
				CondConfig tmp_config =	DRAGON_COND_FILTER;
				tmp_config.max_cost = 7;
				if (tmp_config.min_cost > 7)
					tmp_config.min_cost = 7;
				tmp_config.max_hp = 10;
				if (tmp_config.min_hp > 10)
					tmp_config.min_hp = 10;
				return tmp_config;
			}
			CheckPlayValid = parent_card->owner->GetTargetCard(z)->minion_type == DRAGON_MINION;
			CheckCardValid = card->minion_type == DRAGON_MINION;
			CheckThisValid = parent_card->minion_type == DRAGON_MINION;
		}
	| isDemonCard:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_IS_DEMON_CARD));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Demon", indent_size, false));
				tokens.push_back(mkWordToken("card", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken("Demon", indent_size, false));
				tokens.push_back(mkWordToken("card", indent_size, false));
			}
			FillTokensAlt2
			{
				tokens.push_back(mkWordToken("Demon", indent_size, false));
				tokens.push_back(mkWordToken("cards", indent_size, false));
			}
			Detail = "Demon card";
			DetailAlt1 = "a Demon card";
			DetailAlt2 = "Demon cards";
			CreateNodeHardCopy = new isDemonCard(card_copy);
			GetGlobalSelfConfig
			{
				CondConfig tmp_config =	NOT_DEMON_COND_FILTER;
				tmp_config.max_cost = 7;
				if (tmp_config.min_cost > 7)
					tmp_config.min_cost = 7;
				tmp_config.max_hp = 10;
				if (tmp_config.min_hp > 10)
					tmp_config.min_hp = 10;
				return tmp_config;
			}
			GetTargetConfig
			{
				CondConfig tmp_config =	DEMON_COND_FILTER;
				tmp_config.max_cost = 7;
				if (tmp_config.min_cost > 7)
					tmp_config.min_cost = 7;
				tmp_config.max_hp = 10;
				if (tmp_config.min_hp > 10)
					tmp_config.min_hp = 10;
				return tmp_config;
			}
			CheckPlayValid = parent_card->owner->GetTargetCard(z)->minion_type == DEMON_MINION;
			CheckCardValid = card->minion_type == DEMON_MINION;
			CheckThisValid = parent_card->minion_type == DEMON_MINION;
		}

	CardPosCond
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else
				{
					if (!(instant_config & TARGET_POS_DECK))
						force cardPosAtHand;
					else if (!(instant_config & TARGET_POS_HAND))
						force cardPosAtDeck;
					else if (encoder)
						force [GetMaxIndex(code)];
				}
			}
		}
	:=	// note that only cards in ones own hand is targetable at play, which is already deal with by the IsValidCardTarget function for the higher up rules, therefore no need to be considered here
	| cardPosAtHandOrDeck:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(0u));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("hand", indent_size, false));
				tokens.push_back(mkWordToken("or", indent_size, false));
				tokens.push_back(mkWordToken("deck", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("hand", indent_size, false));
				tokens.push_back(mkWordToken("and", indent_size, false));
				tokens.push_back(mkWordToken("deck", indent_size, false));
			}
			FillTokensAlt2
			{
				tokens.push_back(mkWordToken("hands", indent_size, false));
				tokens.push_back(mkWordToken("and", indent_size, false));
				tokens.push_back(mkWordToken("decks", indent_size, false));
			}
			Detail = "hand or deck";
			DetailAlt1 = "hand and deck";
			DetailAlt2 = "hands and decks";
			CreateNodeHardCopy = new cardPosAtHandOrDeck(card_copy);
			IsCondTrivial = true;
		}
	| cardPosAtHand:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(1u));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("hand", indent_size, false));
			}
			FillTokensAlt2
			{
				tokens.push_back(mkWordToken("hands", indent_size, false));
			}
			Detail = "hand";
			DetailAlt2 = "hands";
			CreateNodeHardCopy = new cardPosAtHand(card_copy);
			GetTargetConfig = HAND_COND_FILTER; 
			CheckCardValid = card->card_pos == CARD_POS_AT_HAND;
			CheckThisValid = parent_card->card_pos == CARD_POS_AT_HAND;
		}
	| cardPosAtDeck:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(2u));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("deck", indent_size, false));
			}
			FillTokensAlt2
			{
				tokens.push_back(mkWordToken("decks", indent_size, false));
			}
			Detail = "deck";
			DetailAlt2 = "decks";
			CreateNodeHardCopy = new cardPosAtDeck(card_copy);
			GetTargetConfig = DECK_COND_FILTER; 
			CheckCardValid = card->card_pos == CARD_POS_AT_DECK;
			CheckThisValid = parent_card->card_pos == CARD_POS_AT_DECK;
		}

	AllegianceCond
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else
				{
					if (target_mode == TARGET_MODE_SOURCE || !(instant_config & TARGET_ANY_ALLE_TYPE)) // for conditions on source it is forced anyAllegiance, as it is always ally, doesn't make sense to put any condition; also sometimes we use both bits being zero to indicate forcing anyAllegiance (like for cards to be moved/copied to owner-type destinations)
						force anyAllegiance;	
					if (!(instant_config & TARGET_IS_OPPO))
						force allyAllegiance;
					if (!(instant_config & TARGET_IS_ALLY))
						force oppoAllegiance;

					if (encoder)
					{
						double choice_code_adjust[3] = {0.0};
						if (target_mode == TARGET_MODE_MOVE_DEST || target_mode == TARGET_MODE_WIN_GAME) // for move destination, it can only be either ally or opponent, but not both; for win game we think an effect intensionally result in a draw is boring
							choice_code_adjust[CHOICE_ANY_ALLE] = -1e10;
						torch_Tensor adjust_tensor = CreateVectorTensorFromArray(choice_code_adjust, 3);
						torch_Tensor final_tensor = code + adjust_tensor;
						force [GetMaxIndex(final_tensor)];
					}
					else
					{
						if (target_mode == TARGET_MODE_MOVE_DEST || target_mode == TARGET_MODE_WIN_GAME) // for move destination, it can only be either ally or opponent, but not both; for win game we think an effect intensionally result in a draw is boring
							forbid anyAllegiance;
					}
				}
			}
		}
	:=
	| anyAllegiance:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_ANY_ALLE));
			}
			FillTokensAlt2
			{
				tokens.push_back(mkWordToken("both", indent_size, false));
				tokens.push_back(mkWordToken("players\'", indent_size, false)); // note that here the "'" is part of the word
			}
			FillTokensAlt3
			{
				tokens.push_back(mkWordToken("either", indent_size, false));
				tokens.push_back(mkWordToken("player\'s", indent_size, false));
			}
			FillTokensAlt4
			{
				tokens.push_back(mkWordToken("both", indent_size, false));
				tokens.push_back(mkWordToken("leaders", indent_size, false));
			}
			FillTokensAlt5
			{
				tokens.push_back(mkWordToken("Both", indent_size, false));
				tokens.push_back(mkWordToken("players", indent_size, false));
			}
			FillTokensAlt6
			{
				tokens.push_back(mkWordToken("both", indent_size, false));
				tokens.push_back(mkWordToken("players", indent_size, false));
			}
			FillTokensAlt7 // for the label in turn start/turn end effects
			{
				tokens.push_back(mkWordToken("All", indent_size, true));
			}
			DetailAlt2 = "both players\' "; // for aoe, etc.
			DetailAlt3 = "either player\'s "; // for random, etc.
			DetailAlt4 = "both leaders"; // for leader untargeted effects
			DetailAlt5 = "Both players "; // for targets with the player as subject
			DetailAlt6 = "both players"; // a variant to above, not at the start of a sentence
			IsPlural = true;
			CreateNodeHardCopy = new anyAllegiance(card_copy);
			IsCondTrivial = true;
		}
	| allyAllegiance:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_ALLY_ALLE));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("friendly", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken("friendly", indent_size, false));
			}
			FillTokensAlt2
			{
				tokens.push_back(mkWordToken("your", indent_size, false));
			}
			FillTokensAlt3
			{
				tokens.push_back(mkWordToken("your", indent_size, false));
			}
			FillTokensAlt4
			{
				tokens.push_back(mkWordToken("your", indent_size, false));
				tokens.push_back(mkWordToken("leader", indent_size, false));
			}
			FillTokensAlt5
			{
				tokens.push_back(mkWordToken("You", indent_size, false));
			}
			FillTokensAlt6
			{
				tokens.push_back(mkWordToken("you", indent_size, false));
			}
			FillTokensAlt7 // for the label in turn start/turn end effects
			{
				tokens.push_back(mkWordToken("Your", indent_size, true));
			}
			Detail = "friendly ";
			DetailAlt1 = "a friendly ";
			DetailAlt2 = "your ";
			DetailAlt3 = "your ";
			DetailAlt4 = "your leader"; // for leader untargeted effects
			DetailAlt5 = "You "; // for targets with the player as subject
			DetailAlt6 = "you"; // a variant to above, not at the start of a sentence
			CreateNodeHardCopy = new allyAllegiance(card_copy);
			GetTargetConfig = ALLY_COND_FILTER;
			CheckPlayValid = parent_card->owner->IsTargetAlly(z);
			CheckCardValid = card->owner == parent_card->owner;
		}
	| oppoAllegiance:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_OPPO_ALLE));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("enemy", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("an", indent_size, false));
				tokens.push_back(mkWordToken("enemy", indent_size, false));
			}
			FillTokensAlt2
			{
				tokens.push_back(mkWordToken("the", indent_size, false));
				tokens.push_back(mkWordToken("opponent\'s", indent_size, false));
			}
			FillTokensAlt3
			{
				tokens.push_back(mkWordToken("the", indent_size, false));
				tokens.push_back(mkWordToken("opponent\'s", indent_size, false));
			}
			FillTokensAlt4
			{
				tokens.push_back(mkWordToken("the", indent_size, false));
				tokens.push_back(mkWordToken("opponent\'s", indent_size, false));
				tokens.push_back(mkWordToken("leader", indent_size, false));
			}
			FillTokensAlt5
			{
				tokens.push_back(mkWordToken("The", indent_size, false));
				tokens.push_back(mkWordToken("opponent", indent_size, false));
			}
			FillTokensAlt6
			{
				tokens.push_back(mkWordToken("the", indent_size, false));
				tokens.push_back(mkWordToken("opponent", indent_size, false));
			}
			FillTokensAlt7 // for the label in turn start/turn end effects
			{
				tokens.push_back(mkWordToken("Enemy", indent_size, true));
			}
			Detail = "enemy ";
			DetailAlt1 = "an enemy ";
			DetailAlt2 = "the opponent\'s ";
			DetailAlt3 = "the opponent\'s ";
			DetailAlt4 = "the opponent\'s leader"; // for leader untargeted effects
			DetailAlt5 = "The opponent "; // for targets with the player as subject
			DetailAlt6 = "the opponent"; // a variant to above, not at the start of a sentence
			IsThirdPersonSingle = true;
			CreateNodeHardCopy = new oppoAllegiance(card_copy);
			GetTargetConfig = OPPO_COND_FILTER;
			CheckThisValid = false;
			CheckPlayValid = parent_card->owner->IsTargetOpponent(z);
			CheckCardValid = card->owner == parent_card->opponent;
		}

	AbilCond
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else if (encoder)
				{
					double choice_code_adjust[8] = {0.0};
					// we don't want the ability condition to be too trivial (initially true)
					// stealth and shield abilities are treated a little bit differently as they are easily lost
					if ((init_config & TARGET_IS_CHARGE) || !(instant_config & TARGET_IS_CHARGE) || (init_config & TARGET_IS_SPELL))
						choice_code_adjust[CHOICE_CHARGE_COND] = -1e10;
					if ((init_config & TARGET_IS_TAUNT) || !(instant_config & TARGET_IS_TAUNT) || (init_config & TARGET_IS_SPELL))
						choice_code_adjust[CHOICE_TAUNT_COND] = -1e10;
					if (((init_config & TARGET_IS_STEALTH) && (!(instant_config & TARGET_POS_FIELD) || (target_mode == TARGET_MODE_SOURCE && effect_timing == EFFECT_TIMING_PLAY))) || !(instant_config & TARGET_IS_STEALTH) || (init_config & (TARGET_IS_SPELL | TARGET_IS_LEADER)))
						choice_code_adjust[CHOICE_STEALTH_COND] = -1e10;
					if ((init_config & TARGET_IS_UNTARGETABLE) || !(instant_config & TARGET_IS_UNTARGETABLE))
						choice_code_adjust[CHOICE_UNTARGETABLE_COND] = -1e10;
					if (((init_config & TARGET_IS_SHIELDED) && (!(instant_config & TARGET_POS_FIELD) || (target_mode == TARGET_MODE_SOURCE && effect_timing == EFFECT_TIMING_PLAY))) || !(instant_config & TARGET_IS_SHIELDED) || (init_config & TARGET_IS_SPELL))
						choice_code_adjust[CHOICE_SHIELD_COND] = -1e10;
					if ((init_config & TARGET_IS_POISONOUS) || !(instant_config & TARGET_IS_POISONOUS))
						choice_code_adjust[CHOICE_POISONOUS_COND] = -1e10;
					if ((init_config & TARGET_IS_LIFESTEAL) || !(instant_config & TARGET_IS_LIFESTEAL))
						choice_code_adjust[CHOICE_LIFESTEAL_COND] = -1e10;
					torch_Tensor adjust_tensor = CreateVectorTensorFromArray(choice_code_adjust, 8);
					torch_Tensor final_tensor = code + adjust_tensor;
					force [GetMaxIndex(final_tensor)];
				}
				else
				{
					// we don't want the ability condition to be too trivial (initially true)
					// stealth and shield abilities are treated a little bit differently as they are easily lost
					if ((init_config & TARGET_IS_CHARGE) || !(instant_config & TARGET_IS_CHARGE) || (init_config & TARGET_IS_SPELL))
						forbid chargeCond;
					if ((init_config & TARGET_IS_TAUNT) || !(instant_config & TARGET_IS_TAUNT) || (init_config & TARGET_IS_SPELL))
						forbid tauntCond;
					if (((init_config & TARGET_IS_STEALTH) && (!(instant_config & TARGET_POS_FIELD) || (target_mode == TARGET_MODE_SOURCE && effect_timing == EFFECT_TIMING_PLAY))) || !(instant_config & TARGET_IS_STEALTH) || (init_config & (TARGET_IS_SPELL | TARGET_IS_LEADER)))
						forbid stealthCond;
					if ((init_config & TARGET_IS_UNTARGETABLE) || !(instant_config & TARGET_IS_UNTARGETABLE))
						forbid untargetableCond;
					if (((init_config & TARGET_IS_SHIELDED) && (!(instant_config & TARGET_POS_FIELD) || (target_mode == TARGET_MODE_SOURCE && effect_timing == EFFECT_TIMING_PLAY))) || !(instant_config & TARGET_IS_SHIELDED) || (init_config & TARGET_IS_SPELL))
						forbid shieldCond;
					if ((init_config & TARGET_IS_POISONOUS) || !(instant_config & TARGET_IS_POISONOUS))
						forbid poisonousCond;
					if ((init_config & TARGET_IS_LIFESTEAL) || !(instant_config & TARGET_IS_LIFESTEAL))
						forbid lifestealCond;
				}
			}
		}
	:=
	| noAbilCond:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(0u));
			}
			CreateNodeHardCopy = new noAbilCond(card_copy);
			IsCondTrivial = true;
		}
	| chargeCond:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(1u));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("with", indent_size, false));
				tokens.push_back(mkWordToken("Charge", indent_size, false));
			}
			Detail = " with Charge";
			CreateNodeHardCopy = new chargeCond(card_copy);
			GetInitAbilFlag = TARGET_IS_CHARGE;
			GetGlobalSelfConfig = NOT_CHARGE_COND_FILTER;
			GetTargetConfig = CHARGE_COND_FILTER;
			CheckPlayValid = parent_card->owner->IsValidTarget(z) && parent_card->owner->GetTargetCard(z)->is_charge;
			CheckCardValid = card->is_charge;
			CheckThisValid = parent_card->is_charge;
		}
	| tauntCond:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(2u));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("with", indent_size, false));
				tokens.push_back(mkWordToken("Taunt", indent_size, false));
			}
			Detail = " with Taunt";
			CreateNodeHardCopy = new tauntCond(card_copy);
			GetInitAbilFlag = TARGET_IS_TAUNT;
			GetGlobalSelfConfig = NOT_TAUNT_COND_FILTER;
			GetTargetConfig = TAUNT_COND_FILTER;
			CheckPlayValid = parent_card->owner->IsValidTarget(z) && parent_card->owner->GetTargetCard(z)->is_taunt;
			CheckCardValid = card->is_taunt;
			CheckThisValid = parent_card->is_taunt;
		}
	| stealthCond:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(3u));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("with", indent_size, false));
				tokens.push_back(mkWordToken("Stealth", indent_size, false));
			}
			Detail = " with Stealth";
			CreateNodeHardCopy = new stealthCond(card_copy);
			GetInitAbilFlag = TARGET_IS_STEALTH;
			GetGlobalSelfConfig = ((!(self_config & TARGET_POS_FIELD) || effect_timing == EFFECT_TIMING_PLAY) ? NOT_STEALTH_COND_FILTER : TARGET_TYPE_ANY); // special as stealth is easily lost when staying on the field
			GetTargetConfig = STEALTH_COND_FILTER;
			CheckPlayValid = parent_card->owner->IsValidTarget(z) && parent_card->owner->GetTargetCard(z)->is_stealth;
			CheckCardValid = card->is_stealth;
			CheckThisValid = parent_card->is_stealth;
		}
	| untargetableCond:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(4u));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("with", indent_size, false));
				tokens.push_back(mkWordToken("Untargetability", indent_size, false));
			}
			Detail = " with Untargetability";
			CreateNodeHardCopy = new untargetableCond(card_copy);
			GetInitAbilFlag = TARGET_IS_UNTARGETABLE;
			GetGlobalSelfConfig = NOT_UNTARGETABLE_COND_FILTER;
			GetTargetConfig = UNTARGETABLE_COND_FILTER;
			CheckPlayValid = parent_card->owner->IsValidTarget(z) && parent_card->owner->GetTargetCard(z)->is_untargetable;
			CheckCardValid = card->is_untargetable;
			CheckThisValid = parent_card->is_untargetable;
		}
	| shieldCond:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(5u));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("with", indent_size, false));
				tokens.push_back(mkWordToken("Shield", indent_size, false));
			}
			Detail = " with Shield";
			CreateNodeHardCopy = new shieldCond(card_copy);
			GetInitAbilFlag = TARGET_IS_SHIELDED;
			GetGlobalSelfConfig = ((!(self_config & TARGET_POS_FIELD) || effect_timing == EFFECT_TIMING_PLAY) ? NOT_SHIELDED_COND_FILTER : TARGET_TYPE_ANY); // special as stealth is easily lost when staying on the field
			GetTargetConfig = SHIELDED_COND_FILTER;
			CheckPlayValid = parent_card->owner->IsValidTarget(z) && parent_card->owner->GetTargetCard(z)->is_shielded;
			CheckCardValid = card->is_shielded;
			CheckThisValid = parent_card->is_shielded;
		}
	| poisonousCond:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(6u));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("with", indent_size, false));
				tokens.push_back(mkWordToken("Poison", indent_size, false));
			}
			Detail = " with Poison";
			CreateNodeHardCopy = new poisonousCond(card_copy);
			GetInitAbilFlag = TARGET_IS_POISONOUS;
			GetGlobalSelfConfig = NOT_POISONOUS_COND_FILTER;
			GetTargetConfig = POISONOUS_COND_FILTER;
			CheckPlayValid = parent_card->owner->IsValidTarget(z) && parent_card->owner->GetTargetCard(z)->is_poisonous;
			CheckCardValid = card->is_poisonous;
			CheckThisValid = parent_card->is_poisonous;
		}
	| lifestealCond:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(7u));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("with", indent_size, false));
				tokens.push_back(mkWordToken("Lifesteal", indent_size, false));
			}
			Detail = " with Lifesteal";
			CreateNodeHardCopy = new lifestealCond(card_copy);
			GetInitAbilFlag = TARGET_IS_LIFESTEAL;
			GetGlobalSelfConfig = NOT_LIFESTEAL_COND_FILTER;
			GetTargetConfig = LIFESTEAL_COND_FILTER;
			CheckPlayValid = parent_card->owner->IsValidTarget(z) && parent_card->owner->GetTargetCard(z)->is_lifesteal;
			CheckCardValid = card->is_lifesteal;
			CheckThisValid = parent_card->is_lifesteal;
		}

	StatCond
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else if (encoder)
				{
					double choice_code_adjust[5] = {0.0};

					if ((init_config & TARGET_IS_SPELL) || (instant_config & TARGET_IS_SPELL)) // spells only has cost stat
					{
						choice_code_adjust[CHOICE_STAT_ATK_COND] = choice_code_adjust[CHOICE_STAT_HP_COND] = choice_code_adjust[CHOICE_STAT_ATK_TIMES_COND] = -1e10;
						forbid atkCond, hpCond, atkTimesCond; // note that this forbid statement is still needed as we'll need to query the probs immediately afterwards
					}

					// note that the default clamping off of negative probs are done before these so we need to check if the scales are negative here manually
					int cost_scale = init_config.max_cost - init_config.min_cost + instant_config.max_cost - instant_config.min_cost;
					if (cost_scale <= 0)
						choice_code_adjust[CHOICE_STAT_COST_COND] = -1e10;	
					int atk_scale = init_config.max_atk - init_config.min_atk + instant_config.max_atk - instant_config.min_atk;
					if (atk_scale <= 0)
						choice_code_adjust[CHOICE_STAT_ATK_COND] = -1e10;	
					int hp_scale = init_config.max_hp - init_config.min_hp + instant_config.max_hp - instant_config.min_hp;
					if (hp_scale <= 0)
						choice_code_adjust[CHOICE_STAT_HP_COND] = -1e10;	
					int n_atks_scale = init_config.max_n_atks - init_config.min_n_atks + instant_config.max_n_atks - instant_config.min_n_atks;
					if (n_atks_scale <= 0)
						choice_code_adjust[CHOICE_STAT_ATK_TIMES_COND] = -1e10;	

					if (force_nontrivial && 
						(choice_code_adjust[CHOICE_STAT_COST_COND] == 0.0 ||
							choice_code_adjust[CHOICE_STAT_ATK_COND] == 0.0 ||
							choice_code_adjust[CHOICE_STAT_HP_COND] == 0.0 ||
							choice_code_adjust[CHOICE_STAT_ATK_TIMES_COND] == 0.0))
						choice_code_adjust[CHOICE_NO_STAT_COND] = -1e10;

					torch_Tensor adjust_tensor = CreateVectorTensorFromArray(choice_code_adjust, 5);
					torch_Tensor choice_tensor = code.narrow(0, 0, 5);
					torch_Tensor final_tensor = choice_tensor + adjust_tensor;
					force [GetMaxIndex(final_tensor)];
				}
				else
				{
					if ((init_config & TARGET_IS_SPELL) || (instant_config & TARGET_IS_SPELL)) // spells only has cost stat
						forbid atkCond, hpCond, atkTimesCond;

					// scale each type of condition with nontrival ranges (the range for lower bound requirement + the range for upper bound requirement), assuming when foring nontrival one of this has to have none zero weights
					// note that the default clamping off of negative probs are done before these so we need to check if the scales are negative here manually
					int cost_scale = init_config.max_cost - init_config.min_cost + instant_config.max_cost - instant_config.min_cost;
					probof(costCond) *= (cost_scale > 0 ? 0.1 * cost_scale : 0); // the 0.1 is to balance with noStatCond
					int atk_scale = init_config.max_atk - init_config.min_atk + instant_config.max_atk - instant_config.min_atk;
					probof(atkCond) *= (atk_scale > 0 ? 0.1 * atk_scale : 0);
					int hp_scale = init_config.max_hp - init_config.min_hp + instant_config.max_hp - instant_config.min_hp;
					probof(hpCond) *= (hp_scale > 0 ? 0.1 * hp_scale : 0);
					int n_atks_scale = init_config.max_n_atks - init_config.min_n_atks + instant_config.max_n_atks - instant_config.min_n_atks;
					probof(atkTimesCond) *= (n_atks_scale > 0 ? 0.1 * n_atks_scale : 0);
					if (force_nontrivial && (probof(costCond) > 0 || probof(atkCond) > 0 || probof(hpCond) > 0 || probof(atkTimesCond) > 0))
						forbid noStatCond;
				}
			}
		}
	:=
	| noStatCond:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_NO_STAT_COND));
			}
			CreateNodeHardCopy = new noStatCond(card_copy);
			IsCondTrivial = true;
		}
	| costCond: StatCondVariant* variant
		{
			generator 
			{
				int lower_min = init_config.min_cost + 1;
				int lower_max = instant_config.max_cost;
				int upper_min = instant_config.min_cost;
				int upper_max = init_config.max_cost - 1;

				if (encoder)
					variant = generate StatCondVariant(lower_min, lower_max, upper_min, upper_max, rep, encoder, code.narrow(0, 5, Stat_Cond_Variant_Size));
				else
					variant = generate StatCondVariant(lower_min, lower_max, upper_min, upper_max, rep, encoder, code);
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_STAT_COST_COND));
				variant->FillRep(rep);
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("having", indent_size, false));
				variant->FillTokens(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken("mana", indent_size, false));
				tokens.push_back(mkWordToken("cost", indent_size, false));
			}
			Detail = " having" + variant->Detail() + " mana cost";
			CreateNodeHardCopy = new costCond(card_copy, (StatCondVariant*)(variant->CreateNodeHardCopy(card_copy, redir_map)));
			GetGlobalSelfConfig
			{
				int min_val = self_config.min_cost;
				int max_val = self_config.max_cost;
				variant->AdjustGlobalStatRange(min_val, max_val);
				unsigned flag = TARGET_TYPE_ANY;
				if (min_val > 7)
					flag = LEADER_COND_FILTER;
				else if (max_val < 8)
					flag = NOT_LEADER_COND_FILTER;
				return GetCostConfig(flag, min_val, max_val);
			}
			GetLeaderConfig
			{
				int min_val = 0;
				int max_val = 10;
				variant->AdjustStatRange(min_val, max_val);
				return GetMaxMpConfig(min_val, 10);
			}
			GetTargetConfig
			{
				int min_val = 0;
				int max_val = 10;
				variant->AdjustStatRange(min_val, max_val);
				unsigned flag = TARGET_TYPE_ANY;
				if (min_val > 7)
					flag = LEADER_COND_FILTER;
				else if (max_val < 8)
					flag = NOT_LEADER_COND_FILTER;
                                return GetCostConfig(flag, min_val, max_val);
			}
			CheckPlayValid = variant->CheckStatValid(parent_card->owner->GetTargetCard(z)->mana);
			CheckCardValid = variant->CheckStatValid(card->mana);
			CheckThisValid = variant->CheckStatValid(parent_card->mana);
		}
	| atkCond: StatCondVariant* variant
		{
			generator 
			{
				int lower_min = init_config.min_atk + 1;
				int lower_max = instant_config.max_atk;
				int upper_min = instant_config.min_atk;
				int upper_max = init_config.max_atk - 1;

				if (encoder)
					variant = generate StatCondVariant(lower_min, lower_max, upper_min, upper_max, rep, encoder, code.narrow(0, 5, Stat_Cond_Variant_Size));
				else
					variant = generate StatCondVariant(lower_min, lower_max, upper_min, upper_max, rep, encoder, code);
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_STAT_ATK_COND));
				variant->FillRep(rep);
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("having", indent_size, false));
				variant->FillTokens(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken("attack", indent_size, false));
			}
			Detail = " having" + variant->Detail() + " attack";
			CreateNodeHardCopy = new atkCond(card_copy, (StatCondVariant*)(variant->CreateNodeHardCopy(card_copy, redir_map)));
			GetGlobalSelfConfig
			{
				int min_val = self_config.min_atk;
				int max_val = self_config.max_atk;
				variant->AdjustGlobalStatRange(min_val, max_val);
				return GetAtkConfig(CHAR_COND_FILTER, min_val, max_val);
			}
			GetTargetConfig
			{
				int min_val = 0;
				int max_val = 10;
				variant->AdjustStatRange(min_val, max_val);
                                return GetAtkConfig(CHAR_COND_FILTER, min_val, max_val);
			}
			CheckPlayValid = variant->CheckStatValid(parent_card->owner->GetTargetCard(z)->atk);
			CheckCardValid = variant->CheckStatValid(card->atk);
			CheckThisValid = variant->CheckStatValid(parent_card->atk);
		}
	| hpCond: StatCondVariant* variant
		{
			generator
			{
				int lower_min = init_config.min_hp + 1;
				int lower_max = instant_config.max_hp;
				int upper_min = instant_config.min_hp;
				int upper_max = init_config.max_hp - 1;

				if (encoder)
					variant = generate StatCondVariant(lower_min, lower_max, upper_min, upper_max, rep, encoder, code.narrow(0, 5, Stat_Cond_Variant_Size));
				else
					variant = generate StatCondVariant(lower_min, lower_max, upper_min, upper_max, rep, encoder, code);
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_STAT_HP_COND));
				variant->FillRep(rep);
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("having", indent_size, false));
				variant->FillTokens(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken("health", indent_size, false));
			}
			Detail = " having" + variant->Detail() + " health";
			CreateNodeHardCopy = new hpCond(card_copy, (StatCondVariant*)(variant->CreateNodeHardCopy(card_copy, redir_map)));
			GetGlobalSelfConfig
			{
				int min_val = self_config.min_hp;
				int max_val = self_config.max_hp;
				variant->AdjustGlobalStatRange(min_val, max_val);
				unsigned flag = CHAR_COND_FILTER;
				if (min_val > 10)
					flag = LEADER_COND_FILTER;
				return GetHpConfig(flag, min_val, max_val);
			}
			GetTargetConfig
			{
				int min_val = -9;
				int max_val = 40;
				variant->AdjustStatRange(min_val, max_val);
				unsigned flag = CHAR_COND_FILTER;
				if (min_val > 10)
					flag = LEADER_COND_FILTER;
                                return GetHpConfig(flag, min_val, max_val);
			}
			CheckPlayValid { Card* card = parent_card->owner->GetTargetCard(z); return variant->CheckStatValid(card->max_hp - card->hp_loss); }
			CheckCardValid = variant->CheckStatValid(card->max_hp - card->hp_loss);
			CheckThisValid = variant->CheckStatValid(parent_card->max_hp - parent_card->hp_loss);
		}
	| atkTimesCond: StatCondVariant* variant
		{
			generator
			{
				int lower_min = init_config.min_n_atks + 1;
				int lower_max = instant_config.max_n_atks;
				int upper_min = instant_config.min_n_atks;
				int upper_max = init_config.max_n_atks - 1;

				if (encoder)
					variant = generate StatCondVariant(lower_min, lower_max, upper_min, upper_max, rep, encoder, code.narrow(0, 5, Stat_Cond_Variant_Size));
				else
					variant = generate StatCondVariant(lower_min, lower_max, upper_min, upper_max, rep, encoder, code);
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_STAT_ATK_TIMES_COND));
				variant->FillRep(rep);
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("having", indent_size, false));
				variant->FillTokens(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken("attack", indent_size, false));
				tokens.push_back(mkWordToken((variant->IsPlural() ? "times" : "time"), indent_size, false));
					
			}
			Detail = " having" + variant->Detail() + (variant->IsPlural() ? " attack times" : " attack time");
			CreateNodeHardCopy = new atkTimesCond(card_copy, (StatCondVariant*)(variant->CreateNodeHardCopy(card_copy, redir_map)));
			GetGlobalSelfConfig
			{
				int min_val = self_config.min_n_atks;
				int max_val = self_config.max_n_atks;
				variant->AdjustGlobalStatRange(min_val, max_val);
				return GetAtkTimesConfig(CHAR_COND_FILTER, min_val, max_val);
			}
			GetTargetConfig
			{
				int min_val = 0;
				int max_val = 5;
				variant->AdjustStatRange(min_val, max_val);
                                return GetAtkTimesConfig(CHAR_COND_FILTER, min_val, max_val);
			}
			IsForbiddingOwnTurnStart
			{
				int min_val = 0;
				int max_val = 5;
				variant->AdjustStatRange(min_val, max_val);
				return min_val <= self_config.min_n_atks && max_val >= self_config.max_n_atks;
			}
			CheckPlayValid { Card* card = parent_card->owner->GetTargetCard(z); return variant->CheckStatValid(card->max_n_atks - card->n_atks_loss); }
			CheckCardValid = variant->CheckStatValid(card->max_n_atks - card->n_atks_loss);
			CheckThisValid = variant->CheckStatValid(parent_card->max_n_atks - parent_card->n_atks_loss);
		}

	StatCondVariant
	default:
		{
			preselector
			{
				if (rep)
				{
					force [rep->choice];
				}
				else
				{
					// scale the weights for "at least" and "at most" with the reasonable range
					probof(statGe) *= lower_max - lower_min + 1;
					probof(statLe) *= upper_max - upper_min + 1;
					// this part wouldn't be needed if not considering the encoder (the default process would zero-out negative probs before selection anyway)
					// but as the encoder case does the force, which would override any probs here, so it is necessary to intercept it here
					if (probof(statGe) <= 0.0)
						force statLe;
					else if (probof(statLe) <= 0.0)
						force statGe;

					if (encoder)
						force [GetMaxIndex(code.narrow(0, 0, 2))];
				}
			}
		}
	:=
	| statGe: int val
		{
			generator
			{
				if (rep)
				{
					val = DenormalizeCode(rep->term_info[0], 0, 10);
					rep++;
				}
				else if (encoder)
				{
					val = DenormalizeCode(GetSingleValFromTensor(code.narrow(0, 2, 1)), 0, 10);
					if (val < lower_min)
						val = lower_min;
					else if (val > lower_max)
						val = lower_max;
				}
				else
				{
					val = SampleRangeCenterConcentratedLinearFromInt(lower_min, lower_max);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(0u, <double>{NormalizeCode(val, 0, 10)})); // well the val can be outside this range but it will be in for most of the time
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("at", indent_size, false));
				tokens.push_back(mkWordToken("least", indent_size, false));
				tokens.push_back(mkWordToken(IntToStr(val), indent_size, false));
			}
			Detail = " at least " + IntToStr(val);
			IsPlural = (val != 1 && val != -1);
			CreateNodeHardCopy = new statGe(card_copy, val);
			AdjustGlobalStatRange { if (max_val >= val) max_val = val - 1; }
			AdjustStatRange { if (min_val < val) min_val = val; }
			CheckStatValid = stat_val >= val;
		}
	| statLe: int val
		{
			generator
			{ 
				if (rep)
				{
					val = DenormalizeCode(rep->term_info[0], 0, 10);
					rep++;
				}
				else if (encoder)
				{
					val = DenormalizeCode(GetSingleValFromTensor(code.narrow(0, 2, 1)), 0, 10);
					if (val < upper_min)
						val = upper_min;
					else if (val > upper_max)
						val = upper_max;
				}
				else
				{
					val = SampleRangeCenterConcentratedLinearFromInt(upper_min, upper_max);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(1u, <double>{NormalizeCode(val, 0, 10)})); // well the val can be outside this range but it will be in for most of the time
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("at", indent_size, false));
				tokens.push_back(mkWordToken("most", indent_size, false));
				tokens.push_back(mkWordToken(IntToStr(val), indent_size, false));
			}
			Detail = " at most " + IntToStr(val);
			IsPlural = (val != 1 && val != -1);
			CreateNodeHardCopy = new statLe(card_copy, val);
			AdjustGlobalStatRange { if (min_val <= val) min_val = val + 1; }
			AdjustStatRange { if (max_val > val) max_val = val; }
			CheckStatValid = stat_val <= val;
		}

	IndeCond
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else if (encoder)
				{
					double choice_code_adjust[5] = {0.0};
					
					if (effect_timing == EFFECT_TIMING_PLAY) // as playing a card generally consumes MP, having mpCond on play effect has some nasty ambiguity, we just forbid this from happening
						choice_code_adjust[CHOICE_MP_COND] = -1e10;
					
					// stat range consideration (see the generator of maxMpCond for justification)
					if ((effect_timing == EFFECT_TIMING_PLAY || effect_timing == EFFECT_TIMING_DESTROY)
						&& ((init_config.min_cost + 1 > instant_leader_config.max_max_mp) && (init_config.min_cost > 9 || instant_self_config.min_cost > 9 || instant_leader_config.min_max_mp > 9)))
						choice_code_adjust[CHOICE_MAX_MP_COND] = -1e10;
					
					torch_Tensor adjust_tensor = CreateVectorTensorFromArray(choice_code_adjust, 5);
					torch_Tensor intermediate_code = UnitForward(code, encoder->inde_cond_decoder_to_intermediate, false);
					torch_Tensor choice_tensor = ForwardClassifyOut(intermediate_code, encoder->inde_cond_decoder_classify);
					torch_Tensor final_tensor = choice_tensor + adjust_tensor;
					force [GetMaxIndex(final_tensor)];
				}
				else
				{
					if (effect_timing == EFFECT_TIMING_PLAY) // as playing a card generally consumes MP, having mpCond on play effect has some nasty ambiguity, we just forbid this from happening
						forbid mpCond;
					
					// stat range consideration (see the generator of maxMpCond for justification)
					if ((effect_timing == EFFECT_TIMING_PLAY || effect_timing == EFFECT_TIMING_DESTROY)
						&& ((init_config.min_cost + 1 > instant_leader_config.max_max_mp) && (init_config.min_cost > 9 || instant_self_config.min_cost > 9 || instant_leader_config.min_max_mp > 9)))
						forbid maxMpCond;
				}
			}
		}
	:=
	| fieldExistenceCond: CharTargetCond* cond
		{
			generator 
			{
				CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
				CondConfig tmp_config = TARGET_POS_FIELD | TARGET_IS_MINION | TARGET_ANY_ALLE_MINION_ABIL_TYPE; // to counter the problem of rvalue passed to lvalue ref
				
				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->inde_cond_decoder_to_intermediate, false);
					cond = generate CharTargetCond(tmp_init_config, tmp_config, TARGET_MODE_EXIST, effect_timing, 2.0, rep, encoder, UnitForward(intermediate_code, encoder->inde_cond_decoder_hidden, false));
				}
				else
				{
					cond = generate CharTargetCond(tmp_init_config, tmp_config, TARGET_MODE_EXIST, effect_timing, 2.0, rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_FIELD_EXISTENCE_COND));
				cond->FillRep(rep);
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("there\'s", indent_size, false));
				cond->FillTokensAlt1(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken("on", indent_size, false));
				tokens.push_back(mkWordToken("the", indent_size, false));
				tokens.push_back(mkWordToken("field", indent_size, false));
				tokens.push_back(mkWordToken("excluding", indent_size, false));
				tokens.push_back(mkWordToken("this", indent_size, false));
				tokens.push_back(mkWordToken("card", indent_size, false));
			}
			Detail = "there is " + cond->DetailAlt1() + " on the field (not considering this card)";
			CreateNodeHardCopy = new fieldExistenceCond(card_copy, (CharTargetCond*)(cond->CreateNodeHardCopy(card_copy, redir_map)));
			CheckThisValid
			{
				for (int i = 0; i < parent_card->owner->field.size(); i++)
				{
					Card* target = parent_card->owner->field[i];
					if (target && target != parent_card && !target->is_dying && cond->CheckCardValid(target, parent_card))
						return true;
				}
				for (int i = 0; i < parent_card->opponent->field.size(); i++)
				{
					Card* target = parent_card->opponent->field[i];
					if (target && target != parent_card && !target->is_dying && cond->CheckCardValid(target, parent_card))
						return true;
				}
				return false;
			}
		}
	| cardExistenceCond: CardTargetCond* cond
		{
			generator
			{
				CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
				CondConfig tmp_config = TARGET_POS_HAND | TARGET_IS_ALLY | TARGET_ANY_CARD_MINION_ABIL_TYPE; // only allowing "your hand" so that you always know exactly whether this condition is met or not (also make it a little easiler for search AI)
				
				// ally bias doens't matter as it'll force ally anyway
				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->inde_cond_decoder_to_intermediate, false);
					cond = generate CardTargetCond(tmp_init_config, tmp_config, TARGET_MODE_EXIST, effect_timing, 1.0, rep, encoder, UnitForward(intermediate_code, encoder->inde_cond_decoder_hidden, false));
				}
				else
				{
					cond = generate CardTargetCond(tmp_init_config, tmp_config, TARGET_MODE_EXIST, effect_timing, 1.0, rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_CARD_EXISTENCE_COND));
				cond->FillRep(rep);
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("there\'s", indent_size, false));
				cond->FillTokensAlt1(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken("excluding", indent_size, false));
				tokens.push_back(mkWordToken("this", indent_size, false));
				tokens.push_back(mkWordToken("card", indent_size, false));
			}
			Detail = "there is " + cond->DetailAlt1() + " (not considering this card)";
			CreateNodeHardCopy = new cardExistenceCond(card_copy, (CardTargetCond*)(cond->CreateNodeHardCopy(card_copy, redir_map)));
			CheckThisValid
			{
				for (int i = 0; i < parent_card->owner->hand.size(); i++)
				{
					Card* target = parent_card->owner->hand[i];
					if (target && target != parent_card && !target->is_dying && cond->CheckCardValid(target, parent_card))
						return true;
				}
				for (int i = 0; i < parent_card->owner->deck.size(); i++)
				{
					Card* target = parent_card->owner->deck[i];
					if (target && target != parent_card && !target->is_dying && cond->CheckCardValid(target, parent_card))
						return true;
				}
				return false;
			}
		}
	| leaderCond: AllegianceCond* alle, AbilCond* abilcond, StatCond* statcond
		{
			generator
			{
				CondConfig tmp_init_config = GetDefaultInitConfig();
				CondConfig tmp_init_config_filter = TARGET_POS_FIELD | TARGET_IS_LEADER | TARGET_ANY_ALLE_TYPE | TARGET_NOT_STEALTH;
				tmp_init_config.min_hp = 1; // condition on leaders should always ask for positive hp
				tmp_init_config.min_cost = 8;	
				CondConfig tmp_instant_config = LEADER_COND_FILTER & FIELD_COND_FILTER;
				tmp_instant_config.min_hp = 1; // condition on leaders should always ask for positive hp
				tmp_instant_config.min_cost = 8;
				
				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->inde_cond_decoder_to_intermediate, false);
					torch_Tensor leader_cond_tensor = UnitForward(intermediate_code, encoder->inde_cond_decoder_hidden, false);
					torch_Tensor sub_intermediate_code = UnitForward(leader_cond_tensor, encoder->leader_cond_decoder_to_intermediate, false);
					torch_Tensor sub_classify_tensor = ForwardClassifyOut(sub_intermediate_code, encoder->leader_cond_decoder_classify);
					torch_Tensor sub_value_tensor = ForwardValueOut(sub_intermediate_code, encoder->leader_cond_decoder_value);
					alle = generate AllegianceCond(tmp_init_config, tmp_instant_config, TARGET_MODE_LEADER, effect_timing, 4.0, rep, encoder, sub_classify_tensor.narrow(0, 0, Alle_Cond_Size));
					// only apply the self config if this is a leader card and it is battlecry and the allgiance is ally (leader cards doesn't have deathrattles or discard effects that only works on the field, turn starting/ending effects cannot have indeCond currently)
					if (!(init_config & TARGET_NOT_LEADER) && (effect_timing == EFFECT_TIMING_PLAY) && !(alle->GetTargetConfig() & TARGET_IS_OPPO))
					{
						tmp_init_config = init_config;
						tmp_instant_config &= instant_self_config;
					}
					tmp_init_config &= tmp_init_config_filter; // we can't just merge the init config normally as they don't work like filters
					// only apply the leader config if the allegiance setting in the config is covered by the allegiance setting here
					if (!(instant_leader_config.flag & ~alle->GetTargetConfig().flag))
						tmp_instant_config &= instant_leader_config;
					abilcond = generate AbilCond(tmp_init_config, tmp_instant_config, TARGET_MODE_LEADER, effect_timing, rep, encoder, sub_classify_tensor.narrow(0, Alle_Cond_Size, Abil_Cond_Size));
					torch_Tensor tmp_cat_list[] = { sub_classify_tensor.narrow(0, Alle_Cond_Size + Abil_Cond_Size, 7), sub_value_tensor }; // 7 is from the 5 classes in StatCond and 2 classes in StatCondVariant
					if (abilcond->IsCondTrivial())
						statcond = generate StatCond(tmp_init_config, tmp_instant_config, TARGET_MODE_LEADER, effect_timing, true, rep, encoder, torch_cat(tmp_cat_list));
					else
						statcond = generate StatCond(tmp_init_config, tmp_instant_config, TARGET_MODE_LEADER, effect_timing, false, rep, encoder, torch_cat(tmp_cat_list));
				}
				else
				{
					alle = generate AllegianceCond(tmp_init_config, tmp_instant_config, TARGET_MODE_LEADER, effect_timing, 4.0, rep, encoder, code);
					// only apply the self config if this is a leader card and it is battlecry and the allgiance is ally
					if (!(init_config & TARGET_NOT_LEADER) && (effect_timing == EFFECT_TIMING_PLAY) && !(alle->GetTargetConfig() & TARGET_IS_OPPO))
					{
						tmp_init_config = init_config;
						tmp_instant_config &= instant_self_config;
					}
					tmp_init_config &= tmp_init_config_filter; // we can't just merge the init config normally as they don't work like filters
					// only apply the leader config if the allegiance setting in the config is covered by the allegiance setting here
					if (!(instant_leader_config.flag & ~alle->GetTargetConfig().flag))
						tmp_instant_config &= instant_leader_config;
					abilcond = generate AbilCond(tmp_init_config, tmp_instant_config, TARGET_MODE_LEADER, effect_timing, rep, encoder, code);
					if (abilcond->IsCondTrivial())
						statcond = generate StatCond(tmp_init_config, tmp_instant_config, TARGET_MODE_LEADER, effect_timing, true, rep, encoder, code);
					else
						statcond = generate StatCond(tmp_init_config, tmp_instant_config, TARGET_MODE_LEADER, effect_timing, false, rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_LEADER_COND));
				alle->FillRep(rep);
				abilcond->FillRep(rep);
				statcond->FillRep(rep);
			}
			FillTokens
			{
				alle->FillTokensAlt4(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken((alle->IsPlural() ? "are" : "is"), indent_size, false));
				abilcond->FillTokens(tokens, indent_size, need_starting_newline);
				statcond->FillTokens(tokens, indent_size, need_starting_newline);
			}
			Detail = alle->DetailAlt4() + (alle->IsPlural() ? " are" : " is") + abilcond->Detail() + statcond->Detail();
			CreateNodeHardCopy = new leaderCond(card_copy,
				(AllegianceCond*)(alle->CreateNodeHardCopy(card_copy, redir_map)),
				(AbilCond*)(abilcond->CreateNodeHardCopy(card_copy, redir_map)),
				(StatCond*)(statcond->CreateNodeHardCopy(card_copy, redir_map)));
			CheckThisValid
			{
				Card* target_a = parent_card->owner->leader;
				if (alle->CheckCardValid(target_a, parent_card))
					if (!(abilcond->CheckCardValid(target_a, parent_card) && statcond->CheckCardValid(target_a, parent_card)))
						return false;
				Card* target_b = parent_card->opponent->leader;
				if (alle->CheckCardValid(target_b, parent_card))
					if (!(abilcond->CheckCardValid(target_b, parent_card) && statcond->CheckCardValid(target_b, parent_card)))
						return false;
				return true;
			}
		}
	| mpCond: AllegianceCond* alle, StatCondVariant* variant
		{
			generator
			{
				CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
				CondConfig tmp_config = GetDefaultConfig(); // to counter the problem of rvalue passed to lvalue ref
				
				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->inde_cond_decoder_to_intermediate, false);
					torch_Tensor mp_or_max_cond_tensor = UnitForward(intermediate_code, encoder->inde_cond_decoder_hidden, false);
					torch_Tensor sub_intermediate_code = UnitForward(mp_or_max_cond_tensor, encoder->mp_or_max_cond_decoder_to_intermediate, false);
					torch_Tensor sub_classify_tensor = ForwardClassifyOut(sub_intermediate_code, encoder->mp_or_max_cond_decoder_classify);
					torch_Tensor sub_value_tensor = ForwardValueOut(sub_intermediate_code, encoder->mp_or_max_cond_decoder_value);

					alle = generate AllegianceCond(tmp_init_config, tmp_config, TARGET_MODE_DEFAULT, effect_timing, 8.0, rep, encoder, sub_classify_tensor.narrow(0, 0, Alle_Cond_Size));

					// we are using the configure that is used for leader also for mp and max mp as currently they can't be both modified in the same effect.
					int instant_min_mp = 0, instant_max_mp = 10;
					// only apply the leader config if the allegiance setting in the config is covered by the allegiance setting here
					if (!(instant_leader_config.flag & ~alle->GetTargetConfig().flag))
					{
						instant_min_mp = instant_leader_config.min_mp;
						instant_max_mp = instant_leader_config.max_mp;
					}
			
					// note that the "init_config" here is not passed in as a parameter as it is always default
					int lower_min = 1;
					int lower_max = instant_max_mp;
					int upper_min = instant_min_mp;
					int upper_max = 9;

					torch_Tensor tmp_cat_list[] = { sub_classify_tensor.narrow(0, Alle_Cond_Size, 2), sub_value_tensor };
					variant = generate StatCondVariant(lower_min, lower_max, upper_min, upper_max, rep, encoder, torch_cat(tmp_cat_list));
				}
				else
				{
					alle = generate AllegianceCond(tmp_init_config, tmp_config, TARGET_MODE_DEFAULT, effect_timing, 8.0, rep, encoder, code);

					// we are using the configure that is used for leader also for mp and max mp as currently they can't be both modified in the same effect.
					int instant_min_mp = 0, instant_max_mp = 10;
					// only apply the leader config if the allegiance setting in the config is covered by the allegiance setting here
					if (!(instant_leader_config.flag & ~alle->GetTargetConfig().flag))
					{
						instant_min_mp = instant_leader_config.min_mp;
						instant_max_mp = instant_leader_config.max_mp;
					}
			
					// note that the "init_config" here is not passed in as a parameter as it is always default
					int lower_min = 1;
					int lower_max = instant_max_mp;
					int upper_min = instant_min_mp;
					int upper_max = 9;

					variant = generate StatCondVariant(lower_min, lower_max, upper_min, upper_max, rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_MP_COND));
				alle->FillRep(rep);
				variant->FillRep(rep);
			}
			FillTokens
			{
				alle->FillTokensAlt6(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken((alle->IsThirdPersonSingle() ?  "has" : "have"), indent_size, false));
				variant->FillTokens(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken("MP", indent_size, false));
			}
			Detail = alle->DetailAlt6() + (alle->IsThirdPersonSingle() ?  " has" : " have") + variant->Detail() + " MP";
			CreateNodeHardCopy = new mpCond(card_copy,
				(AllegianceCond*)(alle->CreateNodeHardCopy(card_copy, redir_map)),
				(StatCondVariant*)(variant->CreateNodeHardCopy(card_copy, redir_map)));
			CheckThisValid
			{
				Card* target_a = parent_card->owner->leader;
				if (alle->CheckCardValid(target_a, parent_card))
					if (!variant->CheckStatValid(parent_card->owner->max_mp - parent_card->owner->mp_loss))
						return false;
				Card* target_b = parent_card->opponent->leader;
				if (alle->CheckCardValid(target_b, parent_card))
					if (!variant->CheckStatValid(parent_card->opponent->max_mp - parent_card->opponent->mp_loss))
						return false;
				return true;
			}
		}
	| maxMpCond: AllegianceCond* alle, StatCondVariant* variant
		{
			generator
			{
				CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
				CondConfig tmp_config = GetDefaultConfig(); // to counter the problem of rvalue passed to lvalue ref
				
				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->inde_cond_decoder_to_intermediate, false);
					torch_Tensor mp_or_max_cond_tensor = UnitForward(intermediate_code, encoder->inde_cond_decoder_hidden, false);
					torch_Tensor sub_intermediate_code = UnitForward(mp_or_max_cond_tensor, encoder->mp_or_max_cond_decoder_to_intermediate, false);
					torch_Tensor sub_classify_tensor = ForwardClassifyOut(sub_intermediate_code, encoder->mp_or_max_cond_decoder_classify);
					torch_Tensor sub_value_tensor = ForwardValueOut(sub_intermediate_code, encoder->mp_or_max_cond_decoder_value);

					alle = generate AllegianceCond(tmp_init_config, tmp_config, TARGET_MODE_DEFAULT, effect_timing, 8.0, rep, encoder, sub_classify_tensor.narrow(0, 0, Alle_Cond_Size));

					// we are using the configure that is used for leader also for mp and max mp as currently they can't be both modified in the same effect
					// we could choose to only apply the leader config if the allegiance setting in the config is covered by the allegiance setting here
					// but as the max_mp are often very close for both sides so we just apply the configure no matter what
					int instant_min_max_mp = instant_leader_config.min_max_mp;
					int instant_max_max_mp = instant_leader_config.max_max_mp;
			
					// note that the "init_config" here is not passed in as a parameter as it is always default
					int lower_min = 1;
					int lower_max = instant_max_max_mp;
					int upper_min = instant_min_max_mp;
					int upper_max = 9;

					// the player has to have the max mp to play the card, so there would be some constraints
					// the constraints is applied when the effect require the card being played, which includes play effects, deathrattles
					// turn start/end effects would also need to be considered when card is on the field, but those effects require srcCond, which current cannot coexist with indeCond, so no need to consider
					// discard effects is similar but doesn't always require srcCond, but when it does not then it isn't necessary on the field anyway, so no need to consider
					if (effect_timing == EFFECT_TIMING_PLAY || effect_timing == EFFECT_TIMING_DESTROY)
					{
						// not checking for the lower_max or upper_max, because max_mp larger than the cost of this card is very normal
						lower_min = init_config.min_cost + 1;
						// here we check for both init and instant for the upper_min (instead of just the instant one as other stats), because here we aren't quite interested in assuming the cost been modified 
						if (upper_min < init_config.min_cost)
							upper_min = init_config.min_cost;
						if (upper_min < instant_self_config.min_cost)
							upper_min = instant_self_config.min_cost;
					}

					torch_Tensor tmp_cat_list[] = { sub_classify_tensor.narrow(0, Alle_Cond_Size, 2), sub_value_tensor };
					variant = generate StatCondVariant(lower_min, lower_max, upper_min, upper_max, rep, encoder, torch_cat(tmp_cat_list));
				}
				else
				{
					alle = generate AllegianceCond(tmp_init_config, tmp_config, TARGET_MODE_DEFAULT, effect_timing, 8.0, rep, encoder, code);

					// we are using the configure that is used for leader also for mp and max mp as currently they can't be both modified in the same effect
					// we could choose to only apply the leader config if the allegiance setting in the config is covered by the allegiance setting here
					// but as the max_mp are often very close for both sides so we just apply the configure no matter what
					int instant_min_max_mp = instant_leader_config.min_max_mp;
					int instant_max_max_mp = instant_leader_config.max_max_mp;
			
					// note that the "init_config" here is not passed in as a parameter as it is always default
					int lower_min = 1;
					int lower_max = instant_max_max_mp;
					int upper_min = instant_min_max_mp;
					int upper_max = 9;

					// the player has to have the max mp to play the card, so there would be some constraints
					// the constraints is applied when the effect require the card being played, which includes play effects, deathrattles
					// turn start/end effects would also need to be considered when card is on the field, but those effects require srcCond, which current cannot coexist with indeCond, so no need to consider
					// discard effects is similar but doesn't always require srcCond, but when it does not then it isn't necessary on the field anyway, so no need to consider
					if (effect_timing == EFFECT_TIMING_PLAY || effect_timing == EFFECT_TIMING_DESTROY)
					{
						// not checking for the lower_max or upper_max, because max_mp larger than the cost of this card is very normal
						lower_min = init_config.min_cost + 1;
						// here we check for both init and instant for the upper_min (instead of just the instant one as other stats), because here we aren't quite interested in assuming the cost been modified 
						if (upper_min < init_config.min_cost)
							upper_min = init_config.min_cost;
						if (upper_min < instant_self_config.min_cost)
							upper_min = instant_self_config.min_cost;
					}

					variant = generate StatCondVariant(lower_min, lower_max, upper_min, upper_max, rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_MAX_MP_COND));
				alle->FillRep(rep);
				variant->FillRep(rep);
			}
			FillTokens
			{
				alle->FillTokensAlt6(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken((alle->IsThirdPersonSingle() ?  "has" : "have"), indent_size, false));
				variant->FillTokens(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken("Max", indent_size, false));
				tokens.push_back(mkWordToken("MP", indent_size, false));
			}
			Detail = alle->DetailAlt6() + (alle->IsThirdPersonSingle() ? " has" : " have") + variant->Detail() + " Max MP";
			CreateNodeHardCopy = new maxMpCond(card_copy,
				(AllegianceCond*)(alle->CreateNodeHardCopy(card_copy, redir_map)),
				(StatCondVariant*)(variant->CreateNodeHardCopy(card_copy, redir_map)));
			CheckThisValid
			{
				Card* target_a = parent_card->owner->leader;
				if (alle->CheckCardValid(target_a, parent_card))
					if (!variant->CheckStatValid(parent_card->owner->max_mp))
						return false;
				Card* target_b = parent_card->opponent->leader;
				if (alle->CheckCardValid(target_b, parent_card))
					if (!variant->CheckStatValid(parent_card->opponent->max_mp))
						return false;
				return true;
			}
		}
	
	BaseTargetedEff
	default:
		{
			preselector
			{
				if (rep)
				{
					force [rep->choice];
				}
				else if (encoder)
				{
					double choice_code_adjust[26] = {0.0};
					
					if (target_mode == TARGET_MODE_LEADER)
					{
						choice_code_adjust[CHOICE_COST_MOD_EFF] = choice_code_adjust[CHOICE_DESTROY_EFF] = choice_code_adjust[CHOICE_DISCARD_EFF] =
							choice_code_adjust[CHOICE_MOVE_EFF] = choice_code_adjust[CHOICE_COPY_EFF] = choice_code_adjust[CHOICE_TRANSFORM_EFF] = 
							choice_code_adjust[CHOICE_CHANGE_TO_BEAST_EFF] = choice_code_adjust[CHOICE_CHANGE_TO_DRAGON_EFF] = choice_code_adjust[CHOICE_CHANGE_TO_DEMON_EFF] =
							choice_code_adjust[CHOICE_GIVE_STEALTH_EFF] = -1e10;
					}
					else if (target_mode == TARGET_MODE_SELF)
					{
						if (!(self_config & TARGET_POS_HAND_OR_DECK)) // don't generate costModEff if it is guaranteed on the field
							choice_code_adjust[CHOICE_COST_MOD_EFF] = -1e10;

						if (self_config & TARGET_IS_LEADER)
						{
							choice_code_adjust[CHOICE_DESTROY_EFF] = choice_code_adjust[CHOICE_CHANGE_TO_BEAST_EFF] = choice_code_adjust[CHOICE_CHANGE_TO_DRAGON_EFF] =
								choice_code_adjust[CHOICE_CHANGE_TO_DEMON_EFF] = choice_code_adjust[CHOICE_GIVE_STEALTH_EFF] = -1e10;
							if (!(self_config & TARGET_POS_HAND_OR_DECK)) // played leaders cannot be moved, copied, or transformed
								choice_code_adjust[CHOICE_MOVE_EFF] = choice_code_adjust[CHOICE_COPY_EFF] = choice_code_adjust[CHOICE_TRANSFORM_EFF] = -1e10;
						}
						else if (self_config & TARGET_IS_SPELL)
							choice_code_adjust[CHOICE_DAMAGE_EFF] = choice_code_adjust[CHOICE_HEAL_EFF] = choice_code_adjust[CHOICE_RES_ATK_TIMES_EFF] =
								choice_code_adjust[CHOICE_ATK_HP_MOD_EFF] = choice_code_adjust[CHOICE_ATK_TIMES_MOD_EFF] = choice_code_adjust[CHOICE_DESTROY_EFF] =
								choice_code_adjust[CHOICE_CHANGE_TO_BEAST_EFF] = choice_code_adjust[CHOICE_CHANGE_TO_DRAGON_EFF] = choice_code_adjust[CHOICE_CHANGE_TO_DEMON_EFF] =
								choice_code_adjust[CHOICE_GIVE_CHARGE_EFF] = choice_code_adjust[CHOICE_GIVE_TAUNT_EFF] = choice_code_adjust[CHOICE_GIVE_STEALTH_EFF] =
								choice_code_adjust[CHOICE_GIVE_SHIELD_EFF] = -1e10;

						// leader cards self directing effect when on the field can be covered by leader effect don't actually need self effect (to simplify the mechanism for avoiding trivial conditions)
						if (!(self_config & TARGET_NOT_LEADER))
							choice_code_adjust[CHOICE_DAMAGE_EFF] = choice_code_adjust[CHOICE_HEAL_EFF] = choice_code_adjust[CHOICE_RES_ATK_TIMES_EFF] = -1e10;

						if (effect_timing == EFFECT_TIMING_PLAY) // doesn't make sense to heal or restore attack times when being played (note: selfEff at deathrattle or on-discard is already forbidden)
							choice_code_adjust[CHOICE_HEAL_EFF] = choice_code_adjust[CHOICE_RES_ATK_TIMES_EFF] = -1e10;
						else if (effect_timing == EFFECT_TIMING_TURN) // repeatedly modifying itself with something that is not easily changed isn't interesting; similar for stats with auto resets
							choice_code_adjust[CHOICE_CHANGE_TO_BEAST_EFF] = choice_code_adjust[CHOICE_CHANGE_TO_DRAGON_EFF] = choice_code_adjust[CHOICE_CHANGE_TO_DEMON_EFF] =
								choice_code_adjust[CHOICE_GIVE_CHARGE_EFF] = choice_code_adjust[CHOICE_GIVE_TAUNT_EFF] = choice_code_adjust[CHOICE_GIVE_UNTARGETABLE_EFF] =
								choice_code_adjust[CHOICE_GIVE_POISONOUS_EFF] = choice_code_adjust[CHOICE_GIVE_LIEFSTEAL_EFF] =
								choice_code_adjust[CHOICE_RES_ATK_TIMES_EFF] = choice_code_adjust[CHOICE_SET_OVERHEAT_EFF] = -1e10;

						// avoid modifying to something that is initially true, stealth and shield is a little bit special has they are easily lost 
						if (!(self_config & TARGET_NOT_BEAST))
							choice_code_adjust[CHOICE_CHANGE_TO_BEAST_EFF] = -1e10;
						if (!(self_config & TARGET_NOT_DRAGON))
							choice_code_adjust[CHOICE_CHANGE_TO_DRAGON_EFF] = -1e10;
						if (!(self_config & TARGET_NOT_DEMON))
							choice_code_adjust[CHOICE_CHANGE_TO_DEMON_EFF] = -1e10;
						if (self_config & TARGET_IS_CHARGE)
							choice_code_adjust[CHOICE_GIVE_CHARGE_EFF] = -1e10;
						if (self_config & TARGET_IS_TAUNT)
							choice_code_adjust[CHOICE_GIVE_TAUNT_EFF] = -1e10;
						if ((self_config & TARGET_IS_STEALTH) && effect_timing == EFFECT_TIMING_PLAY)
							choice_code_adjust[CHOICE_GIVE_STEALTH_EFF] = -1e10;
						if (self_config & TARGET_IS_UNTARGETABLE)
							choice_code_adjust[CHOICE_GIVE_UNTARGETABLE_EFF] = -1e10;
						if ((self_config & TARGET_IS_SHIELDED) && effect_timing == EFFECT_TIMING_PLAY)
							choice_code_adjust[CHOICE_GIVE_SHIELD_EFF] = -1e10;
						if (self_config & TARGET_IS_POISONOUS)
							choice_code_adjust[CHOICE_GIVE_POISONOUS_EFF] = -1e10;
						if (self_config & TARGET_IS_LIFESTEAL)
							choice_code_adjust[CHOICE_GIVE_LIEFSTEAL_EFF] = -1e10;
					}

					if (effect_depth >= max_eff_depth)
						choice_code_adjust[CHOICE_TRANSFORM_EFF] = choice_code_adjust[CHOICE_GIVE_EFFECTS_EFF] =  -1e10;

					torch_Tensor adjust_tensor = CreateVectorTensorFromArray(choice_code_adjust, 26);
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_targeted_eff_decoder_to_intermediate, false);
					torch_Tensor choice_tensor = ForwardClassifyOut(intermediate_code, encoder->base_targeted_eff_decoder_classify);
					torch_Tensor final_tensor = choice_tensor + adjust_tensor;
					force [GetMaxIndex(final_tensor)];
				}
				else
				{
					if (target_mode == TARGET_MODE_LEADER)
					{
						forbid costModEff, destroyEff, discardEff, moveEff, copyEff, transformEff,
							changeToBeastEff, changeToDragonEff, changeToDemonEff, giveStealthEff;
					}
					else if (target_mode == TARGET_MODE_SELF)
					{
						if (!(self_config & TARGET_POS_HAND_OR_DECK)) // don't generate costModEff if it is guaranteed on the field
							forbid costModEff;

						if (self_config & TARGET_IS_LEADER)
						{
							forbid destroyEff, changeToBeastEff, changeToDragonEff, changeToDemonEff, giveStealthEff;
							if (!(self_config & TARGET_POS_HAND_OR_DECK)) // played leaders cannot be moved, copied, or transformed
								forbid moveEff, copyEff, transformEff;
						}
						else if (self_config & TARGET_IS_SPELL)
							forbid damageEff, healEff, resAtkTimesEff, atkHpModEff, atkTimesModEff, destroyEff, changeToBeastEff, changeToDragonEff, changeToDemonEff, giveChargeEff, giveTauntEff, giveStealthEff, giveShieldEff;

						// leader cards self directing effect when on the field can be covered by leader effect don't actually need self effect (to simplify the mechanism for avoiding trivial conditions)
						if (!(self_config & TARGET_NOT_LEADER))
							forbid damageEff, healEff, resAtkTimesEff;

						if (effect_timing == EFFECT_TIMING_PLAY) // doesn't make sense to heal or restore attack times when being played (note: selfEff at deathrattle or on-discard is already forbidden)
							forbid healEff, resAtkTimesEff;
						else if (effect_timing == EFFECT_TIMING_TURN) // repeatedly modifying itself with something that is not easily changed isn't interesting; similar for stats with auto resets
							forbid changeToBeastEff, changeToDragonEff, changeToDemonEff, giveChargeEff, giveTauntEff, giveUntargetableEff, givePoisonousEff, giveLifestealEff,
								resAtkTimesEff, setOverheatEff;

						// avoid modifying to something that is initially true, stealth and shield is a little bit special has they are easily lost 
						if (!(self_config & TARGET_NOT_BEAST))
							forbid changeToBeastEff;
						if (!(self_config & TARGET_NOT_DRAGON))
							forbid changeToDragonEff;
						if (!(self_config & TARGET_NOT_DEMON))
							forbid changeToDemonEff;
						if (self_config & TARGET_IS_CHARGE)
							forbid giveChargeEff;
						if (self_config & TARGET_IS_TAUNT)
							forbid giveTauntEff;
						if ((self_config & TARGET_IS_STEALTH) && effect_timing == EFFECT_TIMING_PLAY)
							forbid giveStealthEff;
						if (self_config & TARGET_IS_UNTARGETABLE)
							forbid giveUntargetableEff;
						if ((self_config & TARGET_IS_SHIELDED) && effect_timing == EFFECT_TIMING_PLAY)
							forbid giveShieldEff;
						if (self_config & TARGET_IS_POISONOUS)
							forbid givePoisonousEff;
						if (self_config & TARGET_IS_LIFESTEAL)
							forbid giveLifestealEff;

						// for self-targeting effects, makking effects biasing towards allies more likely and those biasing towards opponents less likely
						// strictly speaking this should follow the Bayes Theorem
						probof(damageEff) *= DAMAGE_ALLY_BIAS;
						probof(healEff) *= HEAL_ALLY_BIAS;
						probof(resAtkTimesEff) *= RES_ATK_TIMES_ALLY_BIAS;
						probof(costModEff) *=  (1.0 - COST_DEC_PROB) * COST_MOD_ALLY_BIAS_POSITIVE + COST_DEC_PROB * COST_MOD_ALLY_BIAS_NEGATIVE;
						probof(atkHpModEff) *= ATK_HP_STRONG_POSITIVE_PROB * ATK_HP_MOD_ALLY_BIAS_STRONG_POSITIVE
								+ ATK_HP_WEAK_POSITIVE_PROB * ATK_HP_MOD_ALLY_BIAS_WEAK_POSITIVE 
								+ ATK_HP_NEUTRAL_PROB * ATK_HP_MOD_ALLY_BIAS_NEUTRAL 
								+ ATK_HP_WEAK_NEGATIVE_PROB * ATK_HP_MOD_ALLY_BIAS_WEAK_NEGATIVE
								+ ATK_HP_STRONG_NEGATIVE_PROB * ATK_HP_MOD_ALLY_BIAS_STRONG_NEGATIVE;
						probof(atkTimesModEff) *= (1.0 - ATK_TIMES_DEC_PROB) * ATK_TIMES_MOD_ALLY_BIAS_POSITIVE + ATK_TIMES_DEC_PROB * ATK_TIMES_MOD_ALLY_BIAS_NEGATIVE; 
						probof(destroyEff) *= DESTROY_ALLY_BIAS;
						probof(discardEff) *= DISCARD_ALLY_BIAS;
						probof(moveEff) *= MOVE_ALLY_BIAS; // this has detailed prob model inside which is too complex so we just use one overal bias here (far from accurate)
						probof(copyEff) *= COPY_ALLY_BIAS; // this has detailed prob model inside which is too complex so we just use one overal bias here (far from accurate)
						probof(transformEff) *= TRANSFORM_ALLY_BIAS;
						probof(changeToBeastEff) *= CHANGE_MINION_TYPE_ALLY_BIAS;
						probof(changeToDragonEff) *= CHANGE_MINION_TYPE_ALLY_BIAS;
						probof(changeToDemonEff) *= CHANGE_MINION_TYPE_ALLY_BIAS;
						probof(giveChargeEff) *= GIVE_OTHER_ABILITY_ALLY_BIAS;
						probof(giveTauntEff) *= GIVE_TAUNT_OR_STEALTH_ALLY_BIAS;
						probof(giveStealthEff) *= GIVE_TAUNT_OR_STEALTH_ALLY_BIAS;
						probof(giveUntargetableEff) *= GIVE_UNTARGETABLE_ALLY_BIAS;
						probof(giveShieldEff) *= GIVE_OTHER_ABILITY_ALLY_BIAS;
						probof(givePoisonousEff) *= GIVE_OTHER_ABILITY_ALLY_BIAS;
						probof(giveLifestealEff) *= GIVE_OTHER_ABILITY_ALLY_BIAS;
						probof(rmAbilitiesEff) *= RM_ABILITIES_ALLY_BIAS;
						probof(setOverheatEff) *= SET_OVERHEAT_ALLY_BIAS;
						probof(decOhThresholdEff) *= DEC_OH_THRESHOLD_ALLY_BIAS;
						probof(resetStateEff) *= RESET_STATE_ALLY_BIAS;
						probof(giveEffectsEff) *= GIVE_EFFECTS_ALLY_BIAS;
					}

					if (effect_depth >= max_eff_depth)
						forbid transformEff, giveEffectsEff;
				}
			}
		}
	:=
	| damageEff: int val, DamageAbilities* abil // abil part doesn't need description here, it is only to contribute to abilities
		{
			generator 
			{
				if (rep)
				{
					val = DenormalizeCode(rep->term_info[0], 1, 10);
					rep++;
				}
				else if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_targeted_eff_decoder_to_intermediate, false);
					torch_Tensor value_tensor = ForwardValueOut(intermediate_code, encoder->base_targeted_eff_decoder_value);
					val = DenormalizeCode(GetSingleValFromTensor(value_tensor.narrow(0, 0, 1)), 1, 10);
				}
				else
				{
					val = SampleDefaultStatClamped(1, 10);
				}

				if (!rep && give_eff)
				{
					abil = construct damageAbilities(noPoisonous(), noLifesteal());
				}
				else if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_targeted_eff_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->base_targeted_eff_decoder_hidden, false);
					torch_Tensor sub_intermediate_code = UnitForward(hidden_tensor, encoder->damage_eff_decoder_to_intermediate, false);
					torch_Tensor sub_classify_tensor = ForwardBinaryClassifyOut(sub_intermediate_code, encoder->damage_eff_decoder_binary_classify);
					abil = generate DamageAbilities(self_config, target_mode, val, rep, encoder, sub_classify_tensor);
				}
				else
				{
					abil = generate DamageAbilities(self_config, target_mode, val, rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_DAMAGE_EFF, <double>{NormalizeCode(val, 1, 10)}));
				abil->FillRep(rep);
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Deal", indent_size, false));
				tokens.push_back(mkWordToken(IntToStr(val), indent_size, false));
				tokens.push_back(mkWordToken("damage", indent_size, false));
				tokens.push_back(mkWordToken("to", indent_size, false));	
			}
			Detail = "Deal " + IntToStr(val) + " damage to ";
			CreateNodeHardCopy = new damageEff(card_copy, val,
				(DamageAbilities*)(abil->CreateNodeHardCopy(card_copy, redir_map)));
			GetAllyBias = DAMAGE_ALLY_BIAS;
			GetGlobalSelfConfig = CHAR_COND_FILTER;
			GetTargetConfig = FIELD_COND_FILTER;
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
				{
					target->TakeDamage(val, parent_card, start_of_batch);
					return target->is_dying;
				}
				return false;
			}
			GetPoisonousGrowthFactor = 1.5;
			GetLifestealGrowthFactor = 1.0 + (double)val/10.0;
		}
	| healEff: int val
		{
			generator
			{
				if (rep)
				{
					val = DenormalizeCode(rep->term_info[0], 1, 10);
					rep++;
				}
				else if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_targeted_eff_decoder_to_intermediate, false);
					torch_Tensor value_tensor = ForwardValueOut(intermediate_code, encoder->base_targeted_eff_decoder_value);
					val = DenormalizeCode(GetSingleValFromTensor(value_tensor.narrow(0, 1, 1)), 1, 10);
				}
				else
				{
					val = SampleDefaultStatClamped(1, 10);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_HEAL_EFF, <double>{NormalizeCode(val, 1, 10)}));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Restore", indent_size, false));
				tokens.push_back(mkWordToken(IntToStr(val), indent_size, false));
				tokens.push_back(mkWordToken("health", indent_size, false));
				tokens.push_back(mkWordToken("to", indent_size, false));	
			}
			Detail = "Restore " + IntToStr(val) + " health to ";
			CreateNodeHardCopy = new healEff(card_copy, val);
			GetAllyBias = HEAL_ALLY_BIAS;
			GetGlobalSelfConfig = CHAR_COND_FILTER;
			GetTargetConfig = FIELD_COND_FILTER;
			TargetedAction 
			{ 
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
					target->RestoreHp(val);
				return false;
			}
		}
	| resAtkTimesEff: int val
		{
			generator
			{
				if (rep)
				{
					val = DenormalizeCode(rep->term_info[0], 1, 5);
					rep++;
				}
				else if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_targeted_eff_decoder_to_intermediate, false);
					torch_Tensor value_tensor = ForwardValueOut(intermediate_code, encoder->base_targeted_eff_decoder_value);
					val = DenormalizeCode(GetSingleValFromTensor(value_tensor.narrow(0, 0, 1)), 1, 5);
				}
				else
				{
					val = SampleRangeLowerConcentratedPolynomialFromInt(1, 5, 8.0);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_RES_ATK_TIMES_EFF, <double>{NormalizeCode(val, 1, 5)}));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Restore", indent_size, false));
				tokens.push_back(mkWordToken(IntToStr(val), indent_size, false));
				tokens.push_back(mkWordToken("attack", indent_size, false));	
				tokens.push_back(mkWordToken((val == 1 ? "time" : "times"), indent_size, false));
				tokens.push_back(mkWordToken("to", indent_size, false));	
			}
			Detail = "Restore " + IntToStr(val) + " attack " + (val == 1 ? "time" : "times") + " to ";
			CreateNodeHardCopy = new resAtkTimesEff(card_copy, val);
			GetAllyBias = RES_ATK_TIMES_ALLY_BIAS;
			GetGlobalSelfConfig = CHAR_COND_FILTER;
			GetTargetConfig = FIELD_COND_FILTER;
			TargetedAction 
			{ 
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
					target->RestoreAtkTimes(val);
				return false;
			}
		}
	| costModEff{double dec_prob}: int val
		{
			generator
			{
				if (rep)
				{
					val = DenormalizeCode(rep->term_info[0], -10, 10);
					rep++;
				}
				else if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_targeted_eff_decoder_to_intermediate, false);
					torch_Tensor value_tensor = ForwardValueOut(intermediate_code, encoder->base_targeted_eff_decoder_value);
					double normalized = GetSingleValFromTensor(value_tensor.narrow(0, 2, 1));
					val = DenormalizeCode(normalized, -10, 10);
					if (target_mode == TARGET_MODE_SELF && val < -self_config.max_cost)
						val = -self_config.max_cost;	
					if (val == 0) // force non-zero
					{
						if (DenormalizeCodeReal(normalized, -10, 10) < 0)
							val = -1;
						else
							val = 1;
					}
				}
				else
				{
					val = SampleRangeLowerConcentratedPolynomialFromInt(1, 10, 8.0);
					if (target_mode == TARGET_MODE_SELF)
					{
						double dec_weight = dec_prob * COST_MOD_ALLY_BIAS_NEGATIVE;
						double inc_weight = (1 - dec_prob) * COST_MOD_ALLY_BIAS_POSITIVE;
						if (RandomRoll(dec_weight/(dec_weight + inc_weight)) && self_config.max_cost > 0)
						{
							val = -val;
							if (val < -self_config.max_cost)
								val = -self_config.max_cost;
						}
					}
					else
					{
						if (RandomRoll(dec_prob))
						{
							val = -val;
						}
					}
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_COST_MOD_EFF, <double>{NormalizeCode(val, -10, 10)}));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Give", indent_size, false));
				tokens.push_back(mkWordToken((val < 0 ? "" : "+") + IntToStr(val), indent_size, false));
				tokens.push_back(mkWordToken("to", indent_size, false));	
				tokens.push_back(mkWordToken("the", indent_size, false));	
				tokens.push_back(mkWordToken("cost", indent_size, false));	
				tokens.push_back(mkWordToken("of", indent_size, false));	
			}
			Detail = "Give " + ((val < 0 ? "" : "+") + IntToStr(val)) + " to the cost of ";
			CreateNodeHardCopy = new costModEff(card_copy, val);
			GetAllyBias = (val > 0 ? COST_MOD_ALLY_BIAS_POSITIVE : COST_MOD_ALLY_BIAS_NEGATIVE);
			GetGlobalSelfConfig
			{
				if (val < 0)
				{
					unsigned flag = TARGET_TYPE_ANY;
					int min_cost = -val;
					if (min_cost > 7)
						flag &= LEADER_COND_FILTER;
					return GetCostConfig(flag, min_cost, 10);
				}
				else
				{
					return GetDefaultConfig();
				}
			}
			// note that this cost modification does not need to have leader config to impact the range for maxMp, because it is always targeting at the card in hand or deck
			GetTargetConfig
			{
				if (val < 0)
				{
					unsigned flag = HAND_OR_DECK_COND_FILTER;
					int min_cost = -val;
					if (min_cost > 7)
						flag &= LEADER_COND_FILTER;
					return GetCostConfig(flag, min_cost, 10);
				}
				else
				{
					return HAND_OR_DECK_COND_FILTER;
				}
			}
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
					target->ModifyCost(val);
				
				return false;
			}
		}
	| atkHpModEff: int atkmod, int hpmod
		{
			generator
			{
				if (rep)
				{
					atkmod = DenormalizeCode(rep->term_info[0], -10, 10);
					hpmod = DenormalizeCode(rep->term_info[1], 0, 10);
					rep++;
				}
				else if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_targeted_eff_decoder_to_intermediate, false);
					torch_Tensor value_tensor = ForwardValueOut(intermediate_code, encoder->base_targeted_eff_decoder_value);
					double atkmod_normalized = GetSingleValFromTensor(value_tensor.narrow(0, 0, 1));
					atkmod = DenormalizeCode(atkmod_normalized, -10, 10);
					if (target_mode == TARGET_MODE_SELF && atkmod < -self_config.max_atk)
						atkmod = -self_config.max_atk;
					double hpmod_normalized = GetSingleValFromTensor(value_tensor.narrow(0, 1, 1));
					hpmod = DenormalizeCode(hpmod_normalized, 0, 10);
					if (atkmod == 0 && hpmod == 0) // force either of them being non-zero (move the one closer to the next unit)
					{
						double real_atkmod = DenormalizeCodeReal(atkmod_normalized, -10, 10);
						double real_hpmod = DenormalizeCodeReal(hpmod_normalized, 0, 10);
						if (fabs(real_atkmod) < fabs(real_hpmod))
							hpmod = 1;
						else
						{
							if (real_atkmod < 0)
								atkmod = -1;
							else
								atkmod = 1;
						}
					}
				}
				else
				{
					if (target_mode == TARGET_MODE_SELF)
					{
						double strong_positive_weight = ATK_HP_STRONG_POSITIVE_PROB * ATK_HP_MOD_ALLY_BIAS_STRONG_POSITIVE;
						double weak_positive_weight = ATK_HP_WEAK_POSITIVE_PROB * ATK_HP_MOD_ALLY_BIAS_WEAK_POSITIVE;
						double neutral_weight = ATK_HP_NEUTRAL_PROB * ATK_HP_MOD_ALLY_BIAS_NEUTRAL;
						double weak_negative_weight = ATK_HP_WEAK_NEGATIVE_PROB * ATK_HP_MOD_ALLY_BIAS_WEAK_NEGATIVE;
						double strong_negative_weight = ATK_HP_STRONG_NEGATIVE_PROB * ATK_HP_MOD_ALLY_BIAS_STRONG_NEGATIVE;
						double total_weight = strong_positive_weight + weak_positive_weight + neutral_weight + weak_negative_weight + strong_negative_weight;
						double case_probs[5] = { strong_positive_weight/total_weight, weak_positive_weight/total_weight,
							neutral_weight/total_weight, weak_negative_weight/total_weight, strong_negative_weight/total_weight};
						int choice = GetRandChoiceFromProbs(case_probs, 5);
						switch (choice)
						{
							case 0: // strong positive
								atkmod = SampleRangeLowerConcentratedPolynomialFromInt(0, 10, 4.0);
								hpmod = SampleRangeLowerConcentratedPolynomialFromInt(0, 10, 4.0);
								break;
							case 1: // weak positive
								atkmod = SampleRangeUpperConcentratedPolynomialFromInt(-9, -1, 4.0);	
								hpmod = SampleRangeLowerConcentratedPolynomialFromInt(-atkmod + 1, 10, 4.0);
								break;
							case 2: // neutral
								atkmod = SampleRangeUpperConcentratedPolynomialFromInt(-10, -1, 4.0);
								hpmod = -atkmod;
								break;
							case 3: // weak negative
								atkmod = SampleRangeUpperConcentratedPolynomialFromInt(-10, -2, 4.0);	
								hpmod = SampleRangeLowerConcentratedPolynomialFromInt(0, -atkmod - 1, 4.0);
								break;
							default: // strong negative
								atkmod = SampleRangeUpperConcentratedPolynomialFromInt(-10, -1, 4.0);
								hpmod = 0;
								break;
						}
						
						if (atkmod < -self_config.max_atk)
							atkmod = -self_config.max_atk;
					}
					else
					{
						atkmod = SampleRangeCenterConcentratedPolynomialFromInt(-10, 10, 4.0);
						hpmod = SampleRangeLowerConcentratedPolynomialFromInt(0, 10, 4.0);
					}

					if (atkmod == 0 && hpmod == 0) // having two zeros is pointless
						atkmod = 1; // +1 atk is a common choice	
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_ATK_HP_MOD_EFF, <double>{NormalizeCode(atkmod, -10, 10), NormalizeCode(hpmod, 0, 10)}));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Give", indent_size, false));
				tokens.push_back(mkWordToken((atkmod < 0 ? "" : "+") + IntToStr(atkmod) + "/+" + IntToStr(hpmod), indent_size, false));
				tokens.push_back(mkWordToken("to", indent_size, false));	
			}
			Detail = "Give " + ((atkmod < 0 ? "" : "+") + IntToStr(atkmod)) + "/+" + IntToStr(hpmod) + " to ";
			CreateNodeHardCopy = new atkHpModEff(card_copy, atkmod, hpmod);
			GetAllyBias
			{
				if (atkmod >= 0)
					return ATK_HP_MOD_ALLY_BIAS_STRONG_POSITIVE;
				if (atkmod + hpmod > 0)
					return ATK_HP_MOD_ALLY_BIAS_WEAK_POSITIVE;
				if (atkmod + hpmod == 0)
					return ATK_HP_MOD_ALLY_BIAS_NEUTRAL;
				if (hpmod > 0)
					return ATK_HP_MOD_ALLY_BIAS_WEAK_NEGATIVE;
				return ATK_HP_MOD_ALLY_BIAS_STRONG_NEGATIVE;
			}
			GetTargetConfig = GetAtkConfig(CHAR_COND_FILTER, (atkmod < 0 ? -atkmod : 0), 10);
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
					target->ModifyAtkHp(atkmod, hpmod);

				return false;
			}
		}
	| atkTimesModEff{double dec_prob}: int val
		{
			generator
			{
				if (rep)
				{
					val = DenormalizeCode(rep->term_info[0], -3, 3);
					rep++;
				}
				else if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_targeted_eff_decoder_to_intermediate, false);
					torch_Tensor value_tensor = ForwardValueOut(intermediate_code, encoder->base_targeted_eff_decoder_value);
					double normalized = GetSingleValFromTensor(value_tensor.narrow(0, 0, 1));
					val = DenormalizeCode(normalized, -3, 3);
					if (target_mode == TARGET_MODE_SELF && val < -self_config.max_n_atks)
						val = -self_config.max_n_atks;
					if (val == 0) // force non-zero
					{
						if (DenormalizeCodeReal(normalized, -3, 3) < 0)
							val = -1;
						else
							val = 1;
					}
				}
				else
				{
					val = SampleRangeLowerConcentratedPolynomialFromInt(1, 3, 8.0);
					if (target_mode == TARGET_MODE_SELF)
					{
						double dec_weight = dec_prob * ATK_TIMES_MOD_ALLY_BIAS_NEGATIVE;
						double inc_weight = (1 - dec_prob) * ATK_TIMES_MOD_ALLY_BIAS_POSITIVE;
						if (RandomRoll(dec_weight/(dec_weight + inc_weight)) && self_config.max_n_atks > 0)
						{
							val = -val;
							if (val < -self_config.max_n_atks)
								val = -self_config.max_n_atks;
						}
					}
					else
					{
						if (RandomRoll(dec_prob))
						{
							val = -val;
						}
					}
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_ATK_TIMES_MOD_EFF, <double>{NormalizeCode(val, -3, 3)}));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Give", indent_size, false));
				tokens.push_back(mkWordToken((val < 0 ? "" : "+") + IntToStr(val), indent_size, false));
				tokens.push_back(mkWordToken("to", indent_size, false));	
				tokens.push_back(mkWordToken("the", indent_size, false));	
				tokens.push_back(mkWordToken("attack", indent_size, false));	
				tokens.push_back(mkWordToken("times", indent_size, false));	
				tokens.push_back(mkWordToken("of", indent_size, false));	
			}
			Detail = "Give " + ((val < 0 ? "" : "+") + IntToStr(val)) + " to the attack times of ";
			CreateNodeHardCopy = new atkTimesModEff(card_copy, val);
			GetAllyBias = (val > 0 ? ATK_TIMES_MOD_ALLY_BIAS_POSITIVE : ATK_TIMES_MOD_ALLY_BIAS_NEGATIVE);
			GetTargetConfig = GetAtkTimesConfig(CHAR_COND_FILTER, (val < 0 ? -val : 0), 5);
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
					target->ModifyAtkTimes(val);

				return false;
			}
		}
	| destroyEff:
		{
			generator { if (rep) rep++; }
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_DESTROY_EFF));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Destroy", indent_size, false));
			}
			Detail = "Destroy ";
			CreateNodeHardCopy = new destroyEff(card_copy);
			GetAllyBias = DESTROY_ALLY_BIAS;
			GetGlobalSelfConfig = MINION_COND_FILTER;
			GetTargetConfig = TARGET_POS_FIELD | TARGET_IS_MINION | TARGET_ANY_ALLE_MINION_ABIL_TYPE;
			TargetedAction 
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
				{
					target->owner->FlagDestroy(target, start_of_batch);
					return true;
				}
				return false;
			}
		}
	| discardEff:
		{
			generator 
			{ 
				prioritize_forbidding_leader = RandomRoll(NOT_LEADER_PARAM);
				
				if (rep) rep++;
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_DISCARD_EFF));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Discard", indent_size, false));
			}
			Detail = "Discard ";
			CreateNodeHardCopy = new discardEff(card_copy);
			GetAllyBias = DISCARD_ALLY_BIAS;
			GetGlobalSelfConfig
			{
				// leader and field cannot be allowed at the same time
				unsigned tmp_filter;
				if (!(self_config & TARGET_POS_HAND_OR_DECK))
					tmp_filter = NOT_LEADER_COND_FILTER;
				else if (!(self_config & TARGET_NOT_LEADER))
					tmp_filter = HAND_OR_DECK_COND_FILTER;
				else
					tmp_filter = (prioritize_forbidding_leader ? NOT_LEADER_COND_FILTER : HAND_OR_DECK_COND_FILTER);

				return tmp_filter;
			}
			GetSelfConfig
			{
				// leader and field cannot be allowed at the same time
				unsigned tmp_filter;
				if (!(self_config & TARGET_POS_HAND_OR_DECK))
					tmp_filter = NOT_LEADER_COND_FILTER;
				else if (!(self_config & TARGET_NOT_LEADER))
					tmp_filter = HAND_OR_DECK_COND_FILTER;
				else
					tmp_filter = (prioritize_forbidding_leader ? NOT_LEADER_COND_FILTER : HAND_OR_DECK_COND_FILTER);

				return tmp_filter;
			}
			// note: this third version (GetTargetConfig) won't be used at the same time with the other two versions so it is ok that the result would be inconsistent between this and the other two
			GetTargetConfig = (prioritize_forbidding_leader ? NOT_LEADER_COND_FILTER : HAND_OR_DECK_COND_FILTER); // leader and field cannot be allowed at the same time
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
				{
					switch(target->card_pos)
					{
						case CARD_POS_AT_FIELD:
							target->owner->FlagFieldDiscard(target, start_of_batch);
							break;
						case CARD_POS_AT_HAND:
							target->owner->FlagHandDiscard(target, start_of_batch);
							break;
						case CARD_POS_AT_DECK:
							target->owner->FlagDeckDiscard(target, start_of_batch);
							break;
					}
					// it is necessary to consider discard in the batch mechanism, as it can trigger winGameEff, which is resolved immediately (no delay); in general this is a good generalizable practice
					return true;
				}
				return false;
			}
		}
	| moveEff: Destination* dest
		{
			generator
			{
				prioritize_forbidding_leader = RandomRoll(NOT_LEADER_PARAM);
				prioritize_forbidding_pos_collision = RandomRoll(NOT_POS_PARAM);

				if (rep) rep++;

				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_targeted_eff_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->base_targeted_eff_decoder_hidden, false);
					torch_Tensor sub_intermediate_code = UnitForward(hidden_tensor, encoder->move_or_copy_eff_decoder_to_intermediate, false);
					torch_Tensor sub_classify_tensor = ForwardClassifyOut(sub_intermediate_code, encoder->move_or_copy_eff_decoder_classify);
					dest = generate Destination(self_config, target_mode, effect_timing, TARGET_MODE_MOVE_DEST, rep, encoder, sub_classify_tensor);
				}
				else
				{
					dest = generate Destination(self_config, target_mode, effect_timing, TARGET_MODE_MOVE_DEST, rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_MOVE_EFF));
				dest->FillRep(rep);
			}
			FillTokens
			{
				dest->FillTokens(tokens, indent_size, need_starting_newline);
			}
			FillTokensSuffix
			{
				dest->FillTokensSuffix(tokens, indent_size);
			}
			FillTokensSuffixAlt1
			{
				dest->FillTokensSuffixAlt1(tokens, indent_size);
			}
			Detail = dest->Detail();
			Suffix = dest->Suffix();
			SuffixAlt1 = dest->SuffixAlt1();
			CreateNodeHardCopy = new moveEff(card_copy, (Destination*)(dest->CreateNodeHardCopy(card_copy, redir_map)));
			GetAllyBias = MOVE_ALLY_BIAS * dest->GetAllyBias();
			GetPlayTargetConfig
			{
				CondConfig dest_config = dest->GetTargetConfig();
				unsigned type_filter = dest_config | ~TARGET_ANY_CARD_TYPE;
				// should not move to the same position
				unsigned pos_complement_filter = ~(dest_config & TARGET_ANY_POS_TYPE);
 				unsigned alle_flag = dest_config & TARGET_ANY_ALLE_TYPE;
 				unsigned alle_complement_filter = ~alle_flag;
				unsigned dest_constraint_filter = type_filter;
				if (!alle_flag) // meaning the "owner" type destination
					dest_constraint_filter &= pos_complement_filter & FORCE_ANY_ALLE_COND_FILTER & FIELD_COND_FILTER; // for targeted effects, forcing anyAllegiance needs to also forbid hand
				else
					dest_constraint_filter &= (prioritize_forbidding_pos_collision ? pos_complement_filter : alle_complement_filter);
				if (!(dest_constraint_filter & TARGET_POS_HAND)) // if not at hand (has to be on the field for a targeted effect), only minions are possible
					dest_constraint_filter &= MINION_COND_FILTER;

				// leader and field cannot be allowed at the same time
				unsigned leader_constraint_filter;
				if(!(dest_constraint_filter & TARGET_IS_ALLY) || !(dest_constraint_filter & TARGET_POS_HAND)) // in this case the target has to be on the field, so we should forbid leader
					leader_constraint_filter = NOT_LEADER_COND_FILTER;
				else // otherwise apply one of the constraints randomly
					leader_constraint_filter = (prioritize_forbidding_leader ? NOT_LEADER_COND_FILTER : HAND_OR_DECK_COND_FILTER); // use HAND_COND_FILTER at the end would be the same as it would still be processed in the TargetCond part

				return dest_constraint_filter & leader_constraint_filter;
			}
			GetGlobalSelfConfig
			{
				CondConfig dest_config = dest->GetTargetConfig();
				unsigned dest_constraint_filter = dest_config | ~TARGET_ANY_CARD_TYPE; // only the type matters because other parts can easily change (we want aspects that are unchangeable or hard to change)
				
				// leader and field cannot be allowed at the same time
				CondConfig self_config_copy = self_config;
				self_config_copy &= dest_constraint_filter;
				unsigned leader_constraint_filter;
				if (!(self_config_copy & TARGET_POS_HAND_OR_DECK))
					leader_constraint_filter = NOT_LEADER_COND_FILTER;
				else if (!(self_config_copy & TARGET_NOT_LEADER))
					leader_constraint_filter = HAND_OR_DECK_COND_FILTER;
				else
					leader_constraint_filter = (prioritize_forbidding_leader ? NOT_LEADER_COND_FILTER : HAND_OR_DECK_COND_FILTER);

				return dest_constraint_filter & leader_constraint_filter;
			}
			GetSelfConfig
			{
				CondConfig dest_config = dest->GetTargetConfig();
				unsigned type_filter = dest_config | ~TARGET_ANY_CARD_TYPE;
				// should not move to the same position
				unsigned pos_complement_filter = ~(dest_config & TARGET_ANY_POS_TYPE);
 				unsigned alle_flag = dest_config & TARGET_ANY_ALLE_TYPE;
				unsigned dest_constraint_filter = type_filter & ALLY_COND_FILTER; // this ally is because of self mode (doesn't really matter as the alle part in the source cond will be treated specially)
				if (!alle_flag || alle_flag == TARGET_IS_ALLY) // all zero-bits means the "owner" type destination
					dest_constraint_filter &= pos_complement_filter; // for self config it doesn't matter if we force anyAllegiance or not in the filter (it will be always forced)
				
				// leader and field cannot be allowed at the same time
				CondConfig self_config_copy = self_config;
				self_config_copy &= dest_constraint_filter;
				unsigned leader_constraint_filter;
				if (!(self_config_copy & TARGET_POS_HAND_OR_DECK))
					leader_constraint_filter = NOT_LEADER_COND_FILTER;
				else if (!(self_config_copy & TARGET_NOT_LEADER))
					leader_constraint_filter = HAND_OR_DECK_COND_FILTER;
				else
					leader_constraint_filter = (prioritize_forbidding_leader ? NOT_LEADER_COND_FILTER : HAND_OR_DECK_COND_FILTER);

				return dest_constraint_filter & leader_constraint_filter;
			}
			GetTargetConfig
			{
				CondConfig dest_config = dest->GetTargetConfig();
				unsigned type_filter = dest_config | ~TARGET_ANY_CARD_TYPE;
				// should not move to the same position
				unsigned pos_complement_filter = ~(dest_config & TARGET_ANY_POS_TYPE);
				unsigned alle_flag = dest_config & TARGET_ANY_ALLE_TYPE;
 				unsigned alle_complement_filter = ~alle_flag;
				unsigned dest_constraint_filter = type_filter;
				if (!alle_flag) // meaning the "owner" type destination
					dest_constraint_filter &= pos_complement_filter & FORCE_ANY_ALLE_COND_FILTER;
				else
					dest_constraint_filter &= (prioritize_forbidding_pos_collision ? pos_complement_filter : alle_complement_filter);
				
				// leader and field cannot be allowed at the same time (note the dest_constraint_filter part never forbid both hand and deck, or force leader)
				unsigned leader_constraint_filter = (prioritize_forbidding_leader ? NOT_LEADER_COND_FILTER : HAND_OR_DECK_COND_FILTER);
				
				return dest_constraint_filter & leader_constraint_filter;
			}
			TargetedAction
			{
				Card* target = parent_card->owner->ExtractTargetCard(z);
				if (target && !target->is_dying)
				{
					dest->SubTargetedAction(target, parent_card, start_of_batch);
					// note, it is necessary to consider card movement/copy in the batch mechanism as the discard caused by them are considered in the same batch and can trigger winGameEff, which is resolved immediately (no delay); in general this is a good generalizable practice
					return true; 
				}
				return false;
			}
		}
	| copyEff: Destination* dest
		{
			generator
			{
				prioritize_forbidding_leader = RandomRoll(NOT_LEADER_PARAM);

				if (rep) rep++;

				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_targeted_eff_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->base_targeted_eff_decoder_hidden, false);
					torch_Tensor sub_intermediate_code = UnitForward(hidden_tensor, encoder->move_or_copy_eff_decoder_to_intermediate, false);
					torch_Tensor sub_classify_tensor = ForwardClassifyOut(sub_intermediate_code, encoder->move_or_copy_eff_decoder_classify);
					dest = generate Destination(self_config, target_mode, effect_timing, TARGET_MODE_COPY_DEST, rep, encoder, sub_classify_tensor);
				}
				else
				{
					dest = generate Destination(self_config, target_mode, effect_timing, TARGET_MODE_COPY_DEST, rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_COPY_EFF));
				dest->FillRep(rep);
			}
			FillTokens
			{
				dest->FillTokens(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken("copy", indent_size, false));
				tokens.push_back(mkWordToken("of", indent_size, false));
			}
			FillTokensAlt1
			{
				dest->FillTokens(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken("copies", indent_size, false));
				tokens.push_back(mkWordToken("of", indent_size, false));
			}
			FillTokensSuffix
			{
				dest->FillTokensSuffix(tokens, indent_size);
			}
			FillTokensSuffixAlt1
			{
				dest->FillTokensSuffixAlt1(tokens, indent_size);
			}
			Detail = dest->Detail() + "a copy of ";
			DetailAlt1 = dest->Detail() + "copies of "; // for plural
			Suffix = dest->Suffix();
			SuffixAlt1 = dest->SuffixAlt1();
			CreateNodeHardCopy = new copyEff(card_copy, (Destination*)(dest->CreateNodeHardCopy(card_copy, redir_map)));
			GetAllyBias = COPY_ALLY_BIAS * dest->GetAllyBias();
			GetPlayTargetConfig
			{
				CondConfig dest_config = dest->GetTargetConfig();
				unsigned dest_constraint_filter = dest_config | ~TARGET_ANY_CARD_TYPE; // for copy, only the type matters (except we don't allow deck to deck copy, which isn't possible for targeted effects anyway; also except for owner type destination, which is dealt below)
				if (!(dest_config & TARGET_ANY_ALLE_TYPE)) // for "owner" type destination, we want to force anyAllegiance, for play effect, that also means forbidding hand (and of course deck), so has to be on the field
					dest_constraint_filter &= FORCE_ANY_ALLE_COND_FILTER & FIELD_COND_FILTER;

				// leader and field cannot be allowed at the same time
				// well, copying played leader to hand or deck could be OK but it doesn't hurt to forbid it, so we don't need to consider the possibility of adding battlecries to played leaders
				unsigned leader_constraint_filter;
				if(!(dest_constraint_filter & TARGET_POS_HAND)) // in this case the target has to be on the field, so we should forbid leader
					leader_constraint_filter = NOT_LEADER_COND_FILTER;
				else // otherwise apply one of the constraints randomly
					leader_constraint_filter = (prioritize_forbidding_leader ? NOT_LEADER_COND_FILTER : HAND_OR_DECK_COND_FILTER); // use HAND_COND_FILTER at the end would be the same as it would still be processed in the TargetCond part

                                return dest_constraint_filter & leader_constraint_filter;
			}
			GetGlobalSelfConfig
			{
				CondConfig dest_config = dest->GetTargetConfig();
				unsigned dest_constraint_filter = dest_config | ~TARGET_ANY_CARD_TYPE; // only the type matters because other parts can easily change (we want aspects that are unchangeable or hard to change)
				
				// leader and field cannot be allowed at the same time
				// well, copying played leader to hand or deck could be OK but it doesn't hurt to forbid it, so we don't need to consider the possibility of adding battlecries to played leaders
				CondConfig self_config_copy = self_config;
				self_config_copy &= dest_constraint_filter;
				unsigned leader_constraint_filter;
				if (!(self_config_copy & TARGET_POS_HAND_OR_DECK))
					leader_constraint_filter = NOT_LEADER_COND_FILTER;
				else if (!(self_config_copy & TARGET_NOT_LEADER))
					leader_constraint_filter = HAND_OR_DECK_COND_FILTER;
				else
					leader_constraint_filter = (prioritize_forbidding_leader ? NOT_LEADER_COND_FILTER : HAND_OR_DECK_COND_FILTER);

				return dest_constraint_filter & leader_constraint_filter;
			}
			GetSelfConfig
			{
				CondConfig dest_config = dest->GetTargetConfig();
				unsigned dest_constraint_filter = dest_config | ~TARGET_ANY_CARD_TYPE; // for copy, only the type matters (except we don't allow deck to deck copy); also for self config it doesn't matter if we force anyAllegiance or not in the filter (it will be always forced)
				// disallow deck to deck copy, to avoid deck size explosion (even with overheat mechanism and turn limit), because deck size is unbounded
				if (dest_config & TARGET_POS_DECK)
					dest_constraint_filter &= NOT_DECK_COND_FILTER;
				
				// leader and field cannot be allowed at the same time
				// well, copying played leader to hand or deck could be OK but it doesn't hurt to forbid it, so we don't need to consider the possibility of adding battlecries to played leaders
				CondConfig self_config_copy = self_config;
				self_config_copy &= dest_constraint_filter;
				unsigned leader_constraint_filter;
				if (!(self_config_copy & TARGET_POS_HAND_OR_DECK))
					leader_constraint_filter = NOT_LEADER_COND_FILTER;
				else if (!(self_config_copy & TARGET_NOT_LEADER))
					leader_constraint_filter = HAND_OR_DECK_COND_FILTER;
				else
					leader_constraint_filter = (prioritize_forbidding_leader ? NOT_LEADER_COND_FILTER : HAND_OR_DECK_COND_FILTER);

				return dest_constraint_filter & leader_constraint_filter;
			}
			GetTargetConfig
			{
				CondConfig dest_config = dest->GetTargetConfig();
				unsigned dest_constraint_filter = dest_config | ~TARGET_ANY_CARD_TYPE; // for copy, only the type matters (except we don't allow deck to deck copy, also except for owner type destination, which are dealt below)
				if (!(dest_config & TARGET_ANY_ALLE_TYPE)) // for "owner" type destination, we want to force anyAllegiance
					dest_constraint_filter &= FORCE_ANY_ALLE_COND_FILTER;
				// disallow deck to deck copy, to avoid deck size explosion (even with overheat mechanism and turn limit), because deck size is unbounded
				if (dest_config & TARGET_POS_DECK)
					dest_constraint_filter &= NOT_DECK_COND_FILTER;
				
				// leader and field cannot be allowed at the same time (note the dest_constraint_filter part never forbid both hand and deck, or force leader)
				// well, copying played leader to hand or deck could be OK but it doesn't hurt to forbid it, so we don't need to consider the possibility of adding battlecries to played leaders
				unsigned leader_constraint_filter = (prioritize_forbidding_leader ? NOT_LEADER_COND_FILTER : HAND_OR_DECK_COND_FILTER);
				
				return dest_constraint_filter & leader_constraint_filter;
			}
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
				{
					dest->SubTargetedAction(target->CreateCopy(), parent_card, start_of_batch);
					// note, it is necessary to consider card movement/copy in the batch mechanism as the discard caused by them are considered in the same batch and can trigger winGameEff, which is resolved immediately (no delay); in general this is a good generalizable practice
					return true;
				}
				return false;
			}
		}
	| transformEff: NewCardVariant* variant
		{
			generator
			{
				prioritize_forbidding_leader = RandomRoll(NOT_LEADER_PARAM);
				
				if (rep) rep++;

				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_targeted_eff_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->base_targeted_eff_decoder_hidden, false);
					torch_Tensor sub_intermediate_code = UnitForward(hidden_tensor, encoder->transform_eff_decoder_to_intermediate, false);
					torch_Tensor sub_classify_tensor = ForwardClassifyOut(sub_intermediate_code, encoder->transform_eff_decoder_classify);
					torch_Tensor sub_value_tensor = ForwardValueOut(sub_intermediate_code, encoder->transform_eff_decoder_value);
					torch_Tensor sub_hidden_tensor = UnitForward(sub_intermediate_code, encoder->transform_eff_decoder_hidden, false);
					torch_Tensor tmp_cat_list[] = { sub_classify_tensor, sub_hidden_tensor, sub_value_tensor };
					variant = generate NewCardVariant(self_config, target_mode, effect_timing, effect_depth, rep, encoder, torch_cat(tmp_cat_list));
				}
				else
				{
					variant = generate NewCardVariant(self_config, target_mode, effect_timing, effect_depth, rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_TRANSFORM_EFF));
				variant->FillRep(rep);
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Transform", indent_size, false));
			}
			FillTokensSuffix
			{
				int tmp_need_starting_newline = 0; 
				tokens.push_back(mkWordToken("to", indent_size, false));
				variant->FillTokens(tokens, indent_size, tmp_need_starting_newline);
			}
			FillTokensSuffixAlt1
			{
				int tmp_need_starting_newline = 0; 
				tokens.push_back(mkWordToken("to", indent_size, false));
				variant->FillTokensAlt1(tokens, indent_size, tmp_need_starting_newline);
			}
			FillTokensPostfix = variant->FillTokensPostfix(tokens, indent_size);
			Detail = "Transform ";
			Suffix = " to " + variant->Detail();
			SuffixAlt1 = " to " + variant->DetailAlt1(); // for plural
			Postfix = variant->Postfix();
			PostfixIndent = variant->PostfixIndent(indent_size);
			CreateNodeHardCopy = new transformEff(card_copy, (NewCardVariant*)(variant->CreateNodeHardCopy(card_copy, redir_map)));
			GetAllyBias = TRANSFORM_ALLY_BIAS;
			GetGlobalSelfConfig
			{
				CondConfig variant_config = variant->GetTargetConfig();
				unsigned variant_constraint_filter = variant_config | ~TARGET_ANY_CARD_TYPE; // only the type matters because other parts can easily change (we want aspects that are unchangeable or hard to change)

				// leader and field cannot be allowed at the same time
				CondConfig self_config_copy = self_config;
				self_config_copy &= variant_constraint_filter;
				unsigned leader_constraint_filter;
				if (!(self_config_copy & TARGET_POS_HAND_OR_DECK))
					leader_constraint_filter = NOT_LEADER_COND_FILTER;
				else if (!(self_config_copy & TARGET_NOT_LEADER))
					leader_constraint_filter = HAND_OR_DECK_COND_FILTER;
				else
					leader_constraint_filter = (prioritize_forbidding_leader ? NOT_LEADER_COND_FILTER : HAND_OR_DECK_COND_FILTER);

				return variant_constraint_filter & leader_constraint_filter;
			}
			GetTargetConfig = variant->GetTargetConfig(); // the variant will either force it to be minion on the field, or force hand or deck, so there's no need to do the extra leader constraint
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
				{
					Card* card = variant->GetRelatedCard(parent_card);
					// cannot replace and delete target here because in case itself get transformed (through aoeEff, randEff, or selfEff) and there are more effects to trigger on the card at the same instant
					target->owner->FlagCardTransform(target, start_of_batch, card);
					// note, transform event cannot add to the potential draw through AoE etc. but as a generalizable practice, we still have it in the batch mechanism (any delayed event is considered in there)
					return true;
				}
				return false;
			}
		}
	| changeToBeastEff:
		{
			generator { if (rep) rep++; }
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_CHANGE_TO_BEAST_EFF));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Change", indent_size, false));
				tokens.push_back(mkWordToken("the", indent_size, false));
				tokens.push_back(mkWordToken("minion", indent_size, false));
				tokens.push_back(mkWordToken("type", indent_size, false));
				tokens.push_back(mkWordToken("of", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("Change", indent_size, false));
				tokens.push_back(mkWordToken("the", indent_size, false));
				tokens.push_back(mkWordToken("minion", indent_size, false));
				tokens.push_back(mkWordToken("types", indent_size, false));
				tokens.push_back(mkWordToken("of", indent_size, false));
			}
			FillTokensSuffix
			{
				tokens.push_back(mkWordToken("to", indent_size, false));
				tokens.push_back(mkWordToken("Beast", indent_size, false));
			}
			FillTokensSuffixAlt1
			{
				tokens.push_back(mkWordToken("to", indent_size, false));
				tokens.push_back(mkWordToken("Beasts", indent_size, false));
			}
			Detail = "Change the minion type of ";
			DetailAlt1 = "Change the minion types of "; // for plural
			Suffix = " to Beast";
			SuffixAlt1 = " to Beasts"; // for plural
			CreateNodeHardCopy = new changeToBeastEff(card_copy);
			GetAllyBias = CHANGE_MINION_TYPE_ALLY_BIAS;
			GetTargetConfig = NOT_BEAST_COND_FILTER;
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << target->owner->name << "\'s " << target->name << " minion type set to Beast." << endl;
					#endif
					target->minion_type = BEAST_MINION;
				}
				return false;
			}
		}
	| changeToDragonEff:
		{
			generator { if (rep) rep++; }
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_CHANGE_TO_DRAGON_EFF));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Change", indent_size, false));
				tokens.push_back(mkWordToken("the", indent_size, false));
				tokens.push_back(mkWordToken("minion", indent_size, false));
				tokens.push_back(mkWordToken("type", indent_size, false));
				tokens.push_back(mkWordToken("of", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("Change", indent_size, false));
				tokens.push_back(mkWordToken("the", indent_size, false));
				tokens.push_back(mkWordToken("minion", indent_size, false));
				tokens.push_back(mkWordToken("types", indent_size, false));
				tokens.push_back(mkWordToken("of", indent_size, false));
			}
			FillTokensSuffix
			{
				tokens.push_back(mkWordToken("to", indent_size, false));
				tokens.push_back(mkWordToken("Dragon", indent_size, false));
			}
			FillTokensSuffixAlt1
			{
				tokens.push_back(mkWordToken("to", indent_size, false));
				tokens.push_back(mkWordToken("Dragons", indent_size, false));
			}
			Detail = "Change the minion type of ";
			DetailAlt1 = "Change the minion types of "; // for plural
			Suffix = " to Dragon";
			SuffixAlt1 = " to Dragons"; // for plural
			CreateNodeHardCopy = new changeToDragonEff(card_copy);
			GetAllyBias = CHANGE_MINION_TYPE_ALLY_BIAS;
			GetTargetConfig = NOT_DRAGON_COND_FILTER;
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << target->owner->name << "\'s " << target->name << " minion type set to Dragon." << endl;
					#endif
					target->minion_type = DRAGON_MINION;
				}
				return false;
			}
		}
	| changeToDemonEff:
		{
			generator { if (rep) rep++; }
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_CHANGE_TO_DEMON_EFF));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Change", indent_size, false));
				tokens.push_back(mkWordToken("the", indent_size, false));
				tokens.push_back(mkWordToken("minion", indent_size, false));
				tokens.push_back(mkWordToken("type", indent_size, false));
				tokens.push_back(mkWordToken("of", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("Change", indent_size, false));
				tokens.push_back(mkWordToken("the", indent_size, false));
				tokens.push_back(mkWordToken("minion", indent_size, false));
				tokens.push_back(mkWordToken("types", indent_size, false));
				tokens.push_back(mkWordToken("of", indent_size, false));
			}
			FillTokensSuffix
			{
				tokens.push_back(mkWordToken("to", indent_size, false));
				tokens.push_back(mkWordToken("Demon", indent_size, false));
			}
			FillTokensSuffixAlt1
			{
				tokens.push_back(mkWordToken("to", indent_size, false));
				tokens.push_back(mkWordToken("Demons", indent_size, false));
			}
			Detail = "Change the minion type of ";
			DetailAlt1 = "Change the minion types of "; // for plural
			Suffix = " to Demon";
			SuffixAlt1 = " to Demons"; // for plural
			CreateNodeHardCopy = new changeToDemonEff(card_copy);
			GetAllyBias = CHANGE_MINION_TYPE_ALLY_BIAS;
			GetTargetConfig = NOT_DEMON_COND_FILTER;
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << target->owner->name << "\'s " << target->name << " minion type set to Demon." << endl;
					#endif
					target->minion_type = DEMON_MINION;
				}
				return false;
			}
		}
	| giveChargeEff:
		{
			generator { if (rep) rep++; }
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_GIVE_CHARGE_EFF));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Grant", indent_size, false));
				tokens.push_back(mkWordToken("Charge", indent_size, false));
				tokens.push_back(mkWordToken("to", indent_size, false));
			}
			Detail = "Grant Charge to ";
			CreateNodeHardCopy = new giveChargeEff(card_copy);
			GetAllyBias = GIVE_OTHER_ABILITY_ALLY_BIAS;
			GetTargetConfig = NOT_CHARGE_COND_FILTER;
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << "Change ability given to " << target->owner->name << "\'s " << target->name << endl;
					#endif
					target->is_charge = true;
				}
				return false;
			}
		}
	| giveTauntEff:
		{
			generator { if (rep) rep++; }
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_GIVE_TAUNT_EFF));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Grant", indent_size, false));
				tokens.push_back(mkWordToken("Taunt", indent_size, false));
				tokens.push_back(mkWordToken("to", indent_size, false));
			}
			Detail = "Grant Taunt to ";
			CreateNodeHardCopy = new giveTauntEff(card_copy);
			GetAllyBias = GIVE_TAUNT_OR_STEALTH_ALLY_BIAS;
			GetTargetConfig = NOT_TAUNT_COND_FILTER;
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << "Taunt ability given to " << target->owner->name << "\'s " << target->name << endl;
					#endif
					target->is_taunt = true;
				}
				return false;
			}
		}
	| giveStealthEff:
		{
			generator { if (rep) rep++; }
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_GIVE_STEALTH_EFF));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Grant", indent_size, false));
				tokens.push_back(mkWordToken("Stealth", indent_size, false));
				tokens.push_back(mkWordToken("to", indent_size, false));
			}
			Detail = "Grant Stealth to ";
			CreateNodeHardCopy = new giveStealthEff(card_copy);
			GetAllyBias = GIVE_TAUNT_OR_STEALTH_ALLY_BIAS;
			GetGlobalSelfConfig = (effect_timing == EFFECT_TIMING_PLAY ? NOT_STEALTH_COND_FILTER : MINION_COND_FILTER);
			GetSelfConfig = TARGET_POS_FIELD | TARGET_IS_MINION | TARGET_NOT_STEALTH | TARGET_ANY_ALLE_MINION_TYPE; // make sure giving itself stealth or shield only possible on the field (for turn start and end effect, as these abilities only lose easily on the field)
			GetTargetConfig = NOT_STEALTH_COND_FILTER;
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << "Stealth ability given to " << target->owner->name << "\'s " << target->name << endl;
					#endif
					target->is_stealth = true;
				}
				return false;
			}
		}
	| giveUntargetableEff:
		{
			generator { if (rep) rep++; }
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_GIVE_UNTARGETABLE_EFF));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Grant", indent_size, false));
				tokens.push_back(mkWordToken("Untargetability", indent_size, false));
				tokens.push_back(mkWordToken("to", indent_size, false));
			}
			Detail = "Grant Untargetability to ";
			CreateNodeHardCopy = new giveUntargetableEff(card_copy);
			GetAllyBias = GIVE_UNTARGETABLE_ALLY_BIAS;
			GetTargetConfig = NOT_UNTARGETABLE_COND_FILTER;
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << "Untargetability ability given to " << target->owner->name << "\'s " << target->name << endl;
					#endif
					target->is_untargetable = true;
				}
				return false;
			}
		}
	| giveShieldEff:
		{
			generator { if (rep) rep++; }
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_GIVE_SHIELD_EFF));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Grant", indent_size, false));
				tokens.push_back(mkWordToken("Shield", indent_size, false));
				tokens.push_back(mkWordToken("to", indent_size, false));
			}
			Detail = "Grant Shield to ";
			CreateNodeHardCopy = new giveShieldEff(card_copy);
			GetAllyBias = GIVE_OTHER_ABILITY_ALLY_BIAS;
			GetGlobalSelfConfig = (effect_timing == EFFECT_TIMING_PLAY ? NOT_SHIELDED_COND_FILTER : CHAR_COND_FILTER);
			GetSelfConfig = TARGET_POS_FIELD | TARGET_ANY_CHAR | TARGET_NOT_SHIELDED | TARGET_ANY_ALLE_MINION_TYPE; // make sure giving itself stealth or shield only possible on the field (for turn start and end effect, as these abilities only lose easily on the field)
			GetTargetConfig = NOT_SHIELDED_COND_FILTER;
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << "Divine Shield ability given to " << target->owner->name << "\'s " << target->name << endl;
					#endif
					target->is_shielded = true;
				}
				return false;
			}
		}
	| givePoisonousEff:
		{
			generator { if (rep) rep++; }
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_GIVE_POISONOUS_EFF));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Grant", indent_size, false));
				tokens.push_back(mkWordToken("Poisonous", indent_size, false));
				tokens.push_back(mkWordToken("to", indent_size, false));
			}
			Detail = "Grant Poisonous to ";
			CreateNodeHardCopy = new givePoisonousEff(card_copy);
			GetAllyBias = GIVE_OTHER_ABILITY_ALLY_BIAS;
			GetTargetConfig = NOT_POISONOUS_COND_FILTER;
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << "Poisonous ability given to " << target->owner->name << "\'s " << target->name << endl;
					#endif
					target->is_poisonous = true;
				}
				return false;
			}
		}
	| giveLifestealEff:
		{
			generator { if (rep) rep++; }
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_GIVE_LIEFSTEAL_EFF));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Grant", indent_size, false));
				tokens.push_back(mkWordToken("Lifesteal", indent_size, false));
				tokens.push_back(mkWordToken("to", indent_size, false));
			}
			Detail = "Grant Lifesteal to ";
			CreateNodeHardCopy = new giveLifestealEff(card_copy);
			GetAllyBias = GIVE_OTHER_ABILITY_ALLY_BIAS;
			GetTargetConfig = NOT_LIFESTEAL_COND_FILTER;
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << "Lifesteal ability given to " << target->owner->name << "\'s " << target->name << endl;
					#endif
					target->is_lifesteal = true;
				}
				return false;
			}
		}
	| rmAbilitiesEff:
		{
			generator { if (rep) rep++; }
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_RM_ABILITIES_EFF));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Remove", indent_size, false));
				tokens.push_back(mkWordToken("all", indent_size, false));
				tokens.push_back(mkWordToken("ability", indent_size, false));
				tokens.push_back(mkWordToken("keywords", indent_size, false));
				tokens.push_back(mkWordToken("from", indent_size, false));
			}
			Detail = "Remove all ability keywords from ";
			CreateNodeHardCopy = new rmAbilitiesEff(card_copy);
			GetAllyBias = RM_ABILITIES_ALLY_BIAS;
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << "Ability keywords removed from " << target->owner->name << "\'s " << target->name << endl;
					#endif
					target->is_charge = false;
					target->is_taunt = false;
					target->is_stealth = false;
					target->is_untargetable = false;
					target->is_shielded = false;
					target->is_poisonous = false;
					target->is_lifesteal = false;
				}
				return false;
			}
		}
	| setOverheatEff:
		{
			generator { if (rep) rep++; }
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_SET_OVERHEAT_EFF));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Set", indent_size, false));
				tokens.push_back(mkWordToken("every", indent_size, false));
				tokens.push_back(mkWordToken("effect", indent_size, false));
				tokens.push_back(mkWordToken("on", indent_size, false));
			}
			FillTokensSuffix
			{
				tokens.push_back(mkWordToken("to", indent_size, false));
				tokens.push_back(mkWordToken("overheat", indent_size, false));
			}
			Detail = "Set every effect on ";
			Suffix = " to overheat";
			CreateNodeHardCopy = new setOverheatEff(card_copy);
			GetAllyBias = SET_OVERHEAT_ALLY_BIAS;
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << "Every effect on " << target->owner->name << "\'s " << target->name << " set to overheat." << endl;
					#endif
					target->SetAllOverheatCounts(DEFAULT_OVERHEAT_THRESHOLD);
				}
				return false;
			}
		}
	| decOhThresholdEff: int val
		{
			generator 
			{
				if (rep)
				{
					val = DenormalizeCode(rep->term_info[0], 3, 10);
					rep++;
				}
				else if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_targeted_eff_decoder_to_intermediate, false);
					torch_Tensor value_tensor = ForwardValueOut(intermediate_code, encoder->base_targeted_eff_decoder_value);
					val = DenormalizeCode(GetSingleValFromTensor(value_tensor.narrow(0, 2, 1)), 3, 10);
				}
				else
				{
					val = SampleRangeUpperConcentratedLinearFromInt(3, 10);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_DEC_OH_THRESHOLD_EFF, <double>{NormalizeCode(val, 3, 10)}));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Reduce", indent_size, false));
				tokens.push_back(mkWordToken("the", indent_size, false));
				tokens.push_back(mkWordToken("overheat", indent_size, false));
				tokens.push_back(mkWordToken("threshold", indent_size, false));
				tokens.push_back(mkWordToken("for", indent_size, false));
				tokens.push_back(mkWordToken("every", indent_size, false));
				tokens.push_back(mkWordToken("effect", indent_size, false));
				tokens.push_back(mkWordToken("on", indent_size, false));
			}
			FillTokensSuffix
			{
				tokens.push_back(mkWordToken("by", indent_size, false));
				tokens.push_back(mkWordToken(IntToStr(val), indent_size, false));
			}
			Detail = "Reduce the overheat threshold for every effect on ";
			Suffix = " by " + IntToStr(val) + " (applied repeatedly for duplicated effects)";
			CreateNodeHardCopy = new decOhThresholdEff(card_copy, val);
			GetAllyBias = DEC_OH_THRESHOLD_ALLY_BIAS;
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << "The overheat threshhold for every effects on " << target->owner->name << "\'s " << target->name << " reduced by " << IntToStr(val) << "." << endl;
					#endif
					target->ModAllOverheatThresholds(-val);
				}
				return false;
			}
		}
	| resetStateEff:
		{
			generator { if (rep) rep++; }
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_RESET_STATE_EFF));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Reset", indent_size, false));
			}
			FillTokensSuffix
			{
				tokens.push_back(mkWordToken("to", indent_size, false));
				tokens.push_back(mkWordToken("its", indent_size, false));
				tokens.push_back(mkWordToken("original", indent_size, false));
				tokens.push_back(mkWordToken("state", indent_size, false));
			}
			FillTokensSuffixAlt1
			{
				tokens.push_back(mkWordToken("to", indent_size, false));
				tokens.push_back(mkWordToken("their", indent_size, false));
				tokens.push_back(mkWordToken("original", indent_size, false));
				tokens.push_back(mkWordToken("states", indent_size, false));
			}
			Detail = "Reset ";
			Suffix = " to its original state (not directly applying to the sleeping state, consumed attack times, and overheat counts)"; // if it is related to Charge, sleeping state may change indirectly though
			SuffixAlt1 = " to their original states (not directly applying to sleeping states, consumed attack times, and overheat counts)"; // plural
			CreateNodeHardCopy = new resetStateEff(card_copy);
			GetAllyBias = RESET_STATE_ALLY_BIAS;
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << target->owner->name << "\'s " << target->name << " set to its original state." << endl;
					#endif
					target->mana = target->orig_mana;
					target->atk = target->orig_atk;
					target->max_hp = target->orig_hp;
					target->hp_loss = 0;
					target->max_n_atks = target->orig_n_atks;
					target->minion_type = target->orig_minion_type;
					target->is_charge = target->orig_is_charge;
					target->is_taunt = target->orig_is_taunt;
					target->is_stealth = target->orig_is_stealth;
					target->is_untargetable = target->orig_is_untargetable;
					target->is_shielded = target->orig_is_shielded;
					target->is_poisonous = target->orig_is_poisonous;
					target->is_lifesteal = target->orig_is_lifesteal;
					target->owner->FlagCardReset(target, start_of_batch); // need deferred mechanism, in case this is on an extra effect and reset the card itself (through aoeEff, randEff, selfEff, leaderEff etc.)
					target->SetAllOverheatThresholds(DEFAULT_OVERHEAT_THRESHOLD);
					return true;	
				}
				return false;
			}
		}
	| giveEffectsEff: SpecialEffects* effects 
		{
			generator
			{
				if (rep) rep++;
				int next_depth = effect_depth + 1;
				int min_num_effs = 1;
				int max_num_effs = max_eff_num >> next_depth;
				CondConfig tmp_config = GetDefaultInitConfig();
				if (target_mode == TARGET_MODE_LEADER)
				{
					tmp_config &= LEADER_COND_FILTER;
					tmp_config &= FIELD_COND_FILTER;
				}
				else if (target_mode == TARGET_MODE_SELF)
				{
					tmp_config = self_config;
				}

				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_targeted_eff_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->base_targeted_eff_decoder_hidden, false);
					torch_Tensor sub_intermediate_code = UnitForward(hidden_tensor, encoder->give_effects_eff_decoder_to_intermediate, false);
					torch_Tensor sub_hidden_tensor = UnitForward(sub_intermediate_code, encoder->give_effects_eff_decoder_hidden, false);
					effects = generate SpecialEffects(tmp_config, min_num_effs, max_num_effs, next_depth, true, rep, encoder, sub_hidden_tensor);
				}
				else
				{
					effects = generate SpecialEffects(tmp_config, min_num_effs, max_num_effs, next_depth, true, rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_GIVE_EFFECTS_EFF));
				effects->FillRep(rep);
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Attach", indent_size, false));
				tokens.push_back(mkWordToken("the", indent_size, false));
				tokens.push_back(mkWordToken("following", indent_size, false));
				tokens.push_back(mkWordToken("to", indent_size, false));
			}
			FillTokensPostfix
			{
				int tmp_need_starting_newline = 1;
				effects->FillTokens(tokens, indent_size + 2, tmp_need_starting_newline);
			}
			Detail = "Attach the following effect(s) to ";
			Postfix = "\n" + effects->DetailIndent(4);
			PostfixIndent = "\n" + effects->DetailIndent(indent_size + 4); 
			CreateNodeHardCopy = new giveEffectsEff(card_copy, (SpecialEffects*)(effects->CreateNodeHardCopy(card_copy, redir_map)));
			GetAllyBias = GIVE_EFFECTS_ALLY_BIAS;
			GetTargetConfig = effects->GetGlobalSelfConfig(GetDefaultInitConfig(), EFFECT_TIMING_DEFAULT);
			TargetedAction
			{
				Card* target = parent_card->owner->GetTargetCard(z);
				if (target && !target->is_dying)
				{
					#ifndef SUPPRSS_MATCH_PROMPTS
					if (!parent_card->owner->is_exploration)
						cout << "Extra effects granted to " << target->owner->name << "\'s " << target->name << endl;
					#endif
					target->AddExtraEffects(effects);
				}
				return false;
			}
			destructor
			{
				effects->num_refs--;
				if (effects->num_refs <= 0)
					delete effects;
			}
		}

	BaseUntargetedEff
	default:
		{
			preselector
			{
				if (rep)
				{
					force [rep->choice];
				}
				else if (encoder)
				{
					double choice_code_adjust[9] = {0.0};
					
					if (effect_timing == EFFECT_TIMING_DESTROY || effect_timing == EFFECT_TIMING_DISCARD || // deathrattle and on-discard effects should not be directed to itself
						((self_config & TARGET_IS_SPELL) && effect_timing == EFFECT_TIMING_PLAY) || // spell play effect should not be directed to itself
						(!(self_config & TARGET_NOT_LEADER) && !(self_config & TARGET_POS_HAND_OR_DECK))) // leader cards self directing effect when on the field can be covered by leader effect don't actually need self effect (to simplify the mechanism for avoiding trivial conditions)
						choice_code_adjust[CHOICE_SELF_EFF] = -1e10;

					if (effect_timing == EFFECT_TIMING_TURN) // mpEff at the start or end of a turn isn't that meaningful (well they could impact condition check, and losing mp at the start of turn could do something, but still in general not that interesting)
						choice_code_adjust[CHOICE_MP_EFF] = -1e10;

					if (effect_depth >= max_eff_depth)
						choice_code_adjust[CHOICE_NEW_EFF] = -1e10;

					torch_Tensor adjust_tensor = CreateVectorTensorFromArray(choice_code_adjust, 9);
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_untargeted_eff_decoder_to_intermediate, false);
					torch_Tensor choice_tensor = ForwardClassifyOut(intermediate_code, encoder->base_untargeted_eff_decoder_classify);
					torch_Tensor final_tensor = choice_tensor + adjust_tensor;
					force [GetMaxIndex(final_tensor)];
				}
				else
				{
					if (effect_timing == EFFECT_TIMING_DESTROY || effect_timing == EFFECT_TIMING_DISCARD || // deathrattle and on-discard effects should not be directed to itself
						((self_config & TARGET_IS_SPELL) && effect_timing == EFFECT_TIMING_PLAY) || // spell play effect should not be directed to itself
						(!(self_config & TARGET_NOT_LEADER) && !(self_config & TARGET_POS_HAND_OR_DECK))) // leader cards self directing effect when on the field can be covered by leader effect don't actually need self effect (to simplify the mechanism for avoiding trivial conditions)
						forbid selfEff;
					
					if (effect_timing == EFFECT_TIMING_TURN) // mpEff at the start or end of a turn isn't that meaningful (well they could impact condition check, and losing mp at the start of turn could do something, but still in general not that interesting)
						forbid mpEff;

					if (effect_depth >= max_eff_depth)
						forbid newEff;
				}
			}
		}
	:=
	| aoeEff: BaseTargetedEff* effect, TargetCond* cond
		{
			generator
			{
				if (rep) rep++;

				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_untargeted_eff_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->base_untargeted_eff_decoder_hidden, false);
					effect = generate BaseTargetedEff(self_config, TARGET_MODE_DEFAULT, effect_timing, effect_depth, give_eff, rep, encoder, hidden_tensor.narrow(0, 0, Base_Targeted_Eff_Size));
					CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
					CondConfig tmp_config = effect->GetTargetConfig(); // to counter the problem of rvalue passed to lvalue ref
					cond = generate TargetCond(tmp_init_config, tmp_config, TARGET_MODE_DEFAULT, effect_timing, effect->GetAllyBias(), rep, encoder, hidden_tensor.narrow(0, Base_Targeted_Eff_Size, Cond_Size));
				}
				else
				{
					effect = generate BaseTargetedEff(self_config, TARGET_MODE_DEFAULT, effect_timing, effect_depth, give_eff, rep, encoder, code);
					CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
					CondConfig tmp_config = effect->GetTargetConfig(); // to counter the problem of rvalue passed to lvalue ref
					cond = generate TargetCond(tmp_init_config, tmp_config, TARGET_MODE_DEFAULT, effect_timing, effect->GetAllyBias(), rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_AOE_EFF));
				effect->FillRep(rep);
				cond->FillRep(rep);
			}
			FillTokens
			{
				effect->FillTokensAlt1(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken("all", indent_size, false));
				cond->FillTokensAlt2(tokens, indent_size, need_starting_newline);
				effect->FillTokensSuffixAlt1(tokens, indent_size);
			}
			FillTokensPostfix = effect->FillTokensPostfix(tokens, indent_size);
			Detail = effect->DetailAlt1() + "all " + cond->DetailAlt2() + effect->SuffixAlt1();
			Postfix = effect->Postfix();
			PostfixIndent = effect->PostfixIndent(indent_size);
			CreateNodeHardCopy = new aoeEff(card_copy,
				(BaseTargetedEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)),
				(TargetCond*)(cond->CreateNodeHardCopy(card_copy, redir_map)));
			GetLeaderConfig = cond->GetLeaderConfig();
			UntargetedAction
			{
				bool start_of_batch = true;
				int total_cards = 2 + parent_card->owner->field.size() + parent_card->owner->hand.size() + parent_card->owner->deck.size()
					+ parent_card->opponent->field.size() + parent_card->opponent->hand.size() + parent_card->opponent->deck.size();
				for (int i = 0; i < total_cards; i++)
				{
					Card* target = parent_card->owner->GetTargetCard(i);
					if (target && !target->is_dying && cond->CheckCardValid(target, parent_card)) // even that target && target->is_dying is checked in TargetedAction, checking here still helps batching
						start_of_batch = !effect->TargetedAction(i, parent_card, start_of_batch, is_cost_constrained) && start_of_batch; // note the order of the operands for && cannot be inverted, because shortcut mechanism may skip the action part otherwise
				}
			}
			GetPoisonousGrowthFactor
			{
				double tmp_factor = effect->GetPoisonousGrowthFactor();
				return tmp_factor * tmp_factor; // roughly simulate multi-target
			}
			GetLifestealGrowthFactor
			{
				double tmp_factor = effect->GetLifestealGrowthFactor();
				return tmp_factor * tmp_factor; // roughly simulate multi-target
			}
		}
	| randEff: BaseTargetedEff* effect, TargetCond* cond
		{
			generator
			{
				if (rep) rep++;

				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_untargeted_eff_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->base_untargeted_eff_decoder_hidden, false);
					effect = generate BaseTargetedEff(self_config, TARGET_MODE_DEFAULT, effect_timing, effect_depth, give_eff, rep, encoder, hidden_tensor.narrow(0, 0, Base_Targeted_Eff_Size));
					CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
					CondConfig tmp_config = effect->GetTargetConfig(); // to counter the problem of rvalue passed to lvalue ref
					cond = generate TargetCond(tmp_init_config, tmp_config, TARGET_MODE_DEFAULT, effect_timing, effect->GetAllyBias(), rep, encoder, hidden_tensor.narrow(0, Base_Targeted_Eff_Size, Cond_Size));
				}
				else
				{
					effect = generate BaseTargetedEff(self_config, TARGET_MODE_DEFAULT, effect_timing, effect_depth, give_eff, rep, encoder, code);
					CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
					CondConfig tmp_config = effect->GetTargetConfig(); // to counter the problem of rvalue passed to lvalue ref
					cond = generate TargetCond(tmp_init_config, tmp_config, TARGET_MODE_DEFAULT, effect_timing, effect->GetAllyBias(), rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_RAND_EFF));
				effect->FillRep(rep);
				cond->FillRep(rep);
			}
			FillTokens
			{
				effect->FillTokens(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken("random", indent_size, false));
				cond->FillTokens(tokens, indent_size, need_starting_newline);
				effect->FillTokensSuffix(tokens, indent_size);
			}
			FillTokensPostfix = effect->FillTokensPostfix(tokens, indent_size);
			Detail = effect->Detail() + "a random " + cond->Detail() + effect->Suffix();
			Postfix = effect->Postfix();
			PostfixIndent = effect->PostfixIndent(indent_size);
			CreateNodeHardCopy = new randEff(card_copy,
				(BaseTargetedEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)),
				(TargetCond*)(cond->CreateNodeHardCopy(card_copy, redir_map)));
			GetLeaderConfig = cond->GetLeaderConfig();
			UntargetedAction
			{
				int total_cards = 2 + parent_card->owner->field.size() + parent_card->owner->hand.size() + parent_card->owner->deck.size()
					+ parent_card->opponent->field.size() + parent_card->opponent->hand.size() + parent_card->opponent->deck.size();
				int* candidates = new int[total_cards];
				int tmp_num = 0;
				for (int i = 0; i < total_cards; i++)
				{
					Card* target = parent_card->owner->GetTargetCard(i);
					if (target && !target->is_dying && cond->CheckCardValid(target, parent_card))
						candidates[tmp_num++] = i;
				}
				if (tmp_num > 0)
				{
					int chosen_index = GetRandInt(tmp_num);
					effect->TargetedAction(candidates[chosen_index], parent_card, true, is_cost_constrained);
				}
				delete [] candidates;
			}
			GetPoisonousGrowthFactor = effect->GetPoisonousGrowthFactor();
			GetLifestealGrowthFactor = effect->GetLifestealGrowthFactor();
		}
	| leaderEff: BaseTargetedEff* effect, AllegianceCond* alle
		{
			generator
			{
				if (rep) rep++;

				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_untargeted_eff_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->base_untargeted_eff_decoder_hidden, false);
					effect = generate BaseTargetedEff(self_config, TARGET_MODE_LEADER, effect_timing, effect_depth, give_eff, rep, encoder, hidden_tensor.narrow(0, 0, Base_Targeted_Eff_Size));
					CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
					CondConfig tmp_config = effect->GetTargetConfig(); // to counter the problem of rvalue passed to lvalue ref
					torch_Tensor sub_intermediate_code = UnitForward(hidden_tensor.narrow(0, Base_Targeted_Eff_Size, Cond_Size), encoder->alle_to_cond_decoder_to_intermediate, false);
					torch_Tensor sub_classify_tensor = ForwardClassifyOut(sub_intermediate_code, encoder->alle_to_cond_decoder_classify);
					alle = generate AllegianceCond(tmp_init_config, tmp_config, TARGET_MODE_LEADER, effect_timing, effect->GetAllyBias(), rep, encoder, sub_classify_tensor);
				}
				else
				{
					effect = generate BaseTargetedEff(self_config, TARGET_MODE_LEADER, effect_timing, effect_depth, give_eff, rep, encoder, code);
					CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
					CondConfig tmp_config = effect->GetTargetConfig(); // to counter the problem of rvalue passed to lvalue ref
					alle = generate AllegianceCond(tmp_init_config, tmp_config, TARGET_MODE_LEADER, effect_timing, effect->GetAllyBias(), rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_LEADER_EFF));
				effect->FillRep(rep);
				alle->FillRep(rep);
			}
			FillTokens
			{
				if (alle->IsPlural())
				{
					effect->FillTokensAlt1(tokens, indent_size, need_starting_newline);
					alle->FillTokensAlt4(tokens, indent_size, need_starting_newline);
					effect->FillTokensSuffixAlt1(tokens, indent_size);
				}
				else
				{
					effect->FillTokens(tokens, indent_size, need_starting_newline);
					alle->FillTokensAlt4(tokens, indent_size, need_starting_newline);
					effect->FillTokensSuffix(tokens, indent_size);
				}
			}
			FillTokensPostfix = effect->FillTokensPostfix(tokens, indent_size);
			Detail = (alle->IsPlural() ? effect->DetailAlt1() + alle->DetailAlt4() + effect->SuffixAlt1() : effect->Detail() + alle->DetailAlt4() + effect->Suffix());
			Postfix = effect->Postfix();
			PostfixIndent = effect->PostfixIndent(indent_size);
			CreateNodeHardCopy = new leaderEff(card_copy,
				(BaseTargetedEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)),
				(AllegianceCond*)(alle->CreateNodeHardCopy(card_copy, redir_map)));
			GetLeaderConfig
			{
				CondConfig tmp_config = effect->GetTargetConfig(); // note: do not use GetLeaderConfig call here
				tmp_config &= alle->GetTargetConfig(); // used to distinguish which side(s) this is applied on; note: do not use GetLeaderConfig call here
				return tmp_config;
			}
			UntargetedAction
			{
				bool start_of_batch = true;
				Card* target_a = parent_card->owner->leader;
				if (!target_a->is_dying && alle->CheckCardValid(target_a, parent_card))
					start_of_batch = !effect->TargetedAction(0, parent_card, true, is_cost_constrained) && start_of_batch; // note the order of the operands for && cannot be inverted, because shortcut mechanism may skip the action part otherwise
				Card* target_b = parent_card->opponent->leader;
				if (!target_b->is_dying && alle->CheckCardValid(target_b, parent_card))
					effect->TargetedAction(parent_card->owner->field.size() + parent_card->opponent->field.size() + 1, parent_card, start_of_batch, is_cost_constrained);
			}
			GetLifestealGrowthFactor = effect->GetLifestealGrowthFactor();
		}
	| selfEff: BaseTargetedEff* effect
		{
			generator
			{
				prioritize_forbidding_leader = RandomRoll(NOT_LEADER_PARAM);

				if (rep) rep++;
				 
				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_untargeted_eff_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->base_untargeted_eff_decoder_hidden, false);
					effect = generate BaseTargetedEff(self_config, TARGET_MODE_SELF, effect_timing, effect_depth, give_eff, rep, encoder, hidden_tensor.narrow(0, 0, Base_Targeted_Eff_Size));
				}
				else
				{
					effect = generate BaseTargetedEff(self_config, TARGET_MODE_SELF, effect_timing, effect_depth, give_eff, rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_SELF_EFF));
				effect->FillRep(rep);
			}
			FillTokens
			{
				effect->FillTokens(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken("itself", indent_size, false));
				effect->FillTokensSuffix(tokens, indent_size);
			}
			FillTokensPostfix = effect->FillTokensPostfix(tokens, indent_size);
			Detail = effect->Detail() + "itself" + effect->Suffix();
			Postfix = effect->Postfix();
			PostfixIndent = effect->PostfixIndent(indent_size);
			CreateNodeHardCopy = new selfEff(card_copy, (BaseTargetedEff*)(effect->CreateNodeHardCopy(card_copy, redir_map)));
			GetGlobalSelfConfig
			{
				CondConfig tmp_config = effect->GetGlobalSelfConfig(self_config, effect_timing);
				// leader and field shall not be selected at the same time if there is self targetting effect (leader card is special in that they cannot change their place anymore after being placed)
				if (!(self_config & TARGET_POS_HAND_OR_DECK) || !(tmp_config & TARGET_POS_HAND_OR_DECK))
					tmp_config &= NOT_LEADER_COND_FILTER;
				else if (!(self_config & TARGET_NOT_LEADER) || !(tmp_config & TARGET_NOT_LEADER)) 
					tmp_config &= HAND_OR_DECK_COND_FILTER;
				else
					tmp_config &= (prioritize_forbidding_leader ? NOT_LEADER_COND_FILTER : HAND_OR_DECK_COND_FILTER);
				return tmp_config;
			}
			GetSelfConfig
			{
				CondConfig tmp_config = effect->GetSelfConfig(self_config);
				// leader cards self directing effect when on the field can be covered by leader effect don't actually need self effect (to simplify the mechanism for avoiding trivial conditions)
				if (!(self_config & TARGET_POS_HAND_OR_DECK) || !(tmp_config & TARGET_POS_HAND_OR_DECK))
					tmp_config &= NOT_LEADER_COND_FILTER;
				else if (!(self_config & TARGET_NOT_LEADER) || !(tmp_config & TARGET_NOT_LEADER)) 
					tmp_config &= HAND_OR_DECK_COND_FILTER;
				else
					tmp_config &= (prioritize_forbidding_leader ? NOT_LEADER_COND_FILTER : HAND_OR_DECK_COND_FILTER);
				return tmp_config;
			}
			UntargetedAction
			{
				if (!parent_card->is_dying)
					effect->TargetedAction(parent_card->GetTargetIndex(), parent_card, true, is_cost_constrained);
			}
			GetLifestealGrowthFactor = effect->GetLifestealGrowthFactor();
		}
	| drawCardEff: int val, AllegianceCond* alle
		{
			generator 
			{
				if (rep)
				{
					val = DenormalizeCode(rep->term_info[0], 1, 3);
					rep++;
				}
				else if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_untargeted_eff_decoder_to_intermediate, false);
					torch_Tensor value_tensor = ForwardValueOut(intermediate_code, encoder->base_untargeted_eff_decoder_value);
					val = DenormalizeCode(GetSingleValFromTensor(value_tensor), 1, 3);
				}
				else
				{
					val = SampleRangeLowerConcentratedPolynomialFromInt(1, 3, 2.0);
				}

				CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
				CondConfig tmp_config = GetDefaultConfig(); // to counter the problem of rvalue passed to lvalue ref
				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_untargeted_eff_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->base_untargeted_eff_decoder_hidden, false);
					torch_Tensor sub_intermediate_code = UnitForward(hidden_tensor.narrow(0, Base_Targeted_Eff_Size, Cond_Size), encoder->alle_to_cond_decoder_to_intermediate, false);
					torch_Tensor sub_classify_tensor = ForwardClassifyOut(sub_intermediate_code, encoder->alle_to_cond_decoder_classify);
					alle = generate AllegianceCond(tmp_init_config, tmp_config, TARGET_MODE_DEFAULT, effect_timing, 8.0, rep, encoder, sub_classify_tensor);
				}
				else
				{
					alle = generate AllegianceCond(tmp_init_config, tmp_config, TARGET_MODE_DEFAULT, effect_timing, 8.0, rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_DRAW_CARD_EFF, <double>{NormalizeCode(val, 1, 3)}));
				alle->FillRep(rep);
			}
			FillTokens
			{
				alle->FillTokensAlt5(tokens, indent_size, need_starting_newline);
				tokens.push_back(mkWordToken((alle->IsThirdPersonSingle() ? "draws" : "draw"), indent_size, false));
				tokens.push_back(mkWordToken(IntToStr(val), indent_size, false));
				tokens.push_back(mkWordToken((val == 1 ? "card" : "cards"), indent_size, false));
			}
			Detail = alle->DetailAlt5() + (alle->IsThirdPersonSingle() ? "draws " : "draw ") + IntToStr(val) + (val == 1 ? " card" : " cards");
			CreateNodeHardCopy = new drawCardEff(card_copy, val,
				(AllegianceCond*)(alle->CreateNodeHardCopy(card_copy, redir_map)));
			UntargetedAction
			{
				bool start_of_batch = true;
				Card* target_a = parent_card->owner->leader;
				if (alle->CheckCardValid(target_a, parent_card))
					for (int i = 0; i < val; i++)
					{
						parent_card->owner->DrawCard(start_of_batch);
						start_of_batch = false;
					}
				Card* target_b = parent_card->opponent->leader;
				if (alle->CheckCardValid(target_b, parent_card))
					for (int i = 0; i < val; i++)
					{
						parent_card->opponent->DrawCard(start_of_batch);
						start_of_batch = false;
					}
			}
		}
	| mpEff{double dec_prob}: int val, AllegianceCond* alle
		{
			generator
			{
				if (rep)
				{
					val = DenormalizeCode(rep->term_info[0], -10, 10);
					rep++;
				}
				else if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_untargeted_eff_decoder_to_intermediate, false);
					torch_Tensor value_tensor = ForwardValueOut(intermediate_code, encoder->base_untargeted_eff_decoder_value);
					double normalized = GetSingleValFromTensor(value_tensor);
					val = DenormalizeCode(normalized, -10, 10);
					if (val == 0) // force non-zero
					{
						if (DenormalizeCodeReal(normalized, -10, 10) < 0)
							val = -1;
						else
							val = 1;
					}
				}
				else
				{
					val = SampleRangeLowerConcentratedPolynomialFromInt(1, 10, 16.0);
					if (RandomRoll(dec_prob))
						val = -val;
				}

				CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
				CondConfig tmp_config = GetDefaultConfig(); // to counter the problem of rvalue passed to lvalue ref
				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_untargeted_eff_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->base_untargeted_eff_decoder_hidden, false);
					torch_Tensor sub_intermediate_code = UnitForward(hidden_tensor.narrow(0, Base_Targeted_Eff_Size, Cond_Size), encoder->alle_to_cond_decoder_to_intermediate, false);
					torch_Tensor sub_classify_tensor = ForwardClassifyOut(sub_intermediate_code, encoder->alle_to_cond_decoder_classify);
					alle = generate AllegianceCond(tmp_init_config, tmp_config, TARGET_MODE_DEFAULT, effect_timing, (val > 0 ? 16.0 : 0.0625), rep, encoder, sub_classify_tensor);
				}
				else
				{
					alle = generate AllegianceCond(tmp_init_config, tmp_config, TARGET_MODE_DEFAULT, effect_timing, (val > 0 ? 16.0 : 0.0625), rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_MP_EFF, <double>{NormalizeCode(val, -10, 10)}));
				alle->FillRep(rep);
			}
			FillTokens
			{
				alle->FillTokensAlt5(tokens, indent_size, need_starting_newline);
				string tmp_verb = (val >= 0 ? "recover" : "exhaust");
				if (alle->IsThirdPersonSingle())
					tmp_verb += "s";
				tokens.push_back(mkWordToken(tmp_verb, indent_size, false));
				tokens.push_back(mkWordToken(IntToStr(abs(val)), indent_size, false));
				tokens.push_back(mkWordToken("MP", indent_size, false));
			}
			Detail = alle->DetailAlt5() + (val >= 0 ? "recover" : "exhaust") + (alle->IsThirdPersonSingle() ? "s " : " ") + IntToStr(abs(val)) + " MP";
			CreateNodeHardCopy = new mpEff(card_copy, val,
				(AllegianceCond*)(alle->CreateNodeHardCopy(card_copy, redir_map)));
			GetLeaderConfig
			{
				CondConfig tmp_config;
				if (val < 0)
					tmp_config = GetMpConfig(-val, 10);
				else
					tmp_config = GetMpConfig(0, 10 - val);
				tmp_config &= alle->GetTargetConfig(); // used to distinguish which side(s) this is applied on
				return tmp_config;
			}
			UntargetedAction
			{
				Card* target_a = parent_card->owner->leader;
				if (alle->CheckCardValid(target_a, parent_card))
					parent_card->owner->ModifyMp(val);
				Card* target_b = parent_card->opponent->leader;
				if (alle->CheckCardValid(target_b, parent_card))
					parent_card->opponent->ModifyMp(val);
			}
		}
	| maxMpEff{double dec_prob}: int val, AllegianceCond* alle
		{
			generator
			{
				if (rep)
				{
					val = DenormalizeCode(rep->term_info[0], -10, 10);
					rep++;
				}
				else if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_untargeted_eff_decoder_to_intermediate, false);
					torch_Tensor value_tensor = ForwardValueOut(intermediate_code, encoder->base_untargeted_eff_decoder_value);
					double normalized = GetSingleValFromTensor(value_tensor);
					val = DenormalizeCode(normalized, -10, 10);
					if (val == 0) // force non-zero
					{
						if (DenormalizeCodeReal(normalized, -10, 10) < 0)
							val = -1;
						else
							val = 1;
					}
				}
				else
				{
					val = SampleRangeLowerConcentratedPolynomialFromInt(1, 10, 16.0);
					if (RandomRoll(dec_prob))
						val = -val;
				}

				CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
				CondConfig tmp_config = GetDefaultConfig(); // to counter the problem of rvalue passed to lvalue ref
				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_untargeted_eff_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->base_untargeted_eff_decoder_hidden, false);
					torch_Tensor sub_intermediate_code = UnitForward(hidden_tensor.narrow(0, Base_Targeted_Eff_Size, Cond_Size), encoder->alle_to_cond_decoder_to_intermediate, false);
					torch_Tensor sub_classify_tensor = ForwardClassifyOut(sub_intermediate_code, encoder->alle_to_cond_decoder_classify);
					alle = generate AllegianceCond(tmp_init_config, tmp_config, TARGET_MODE_DEFAULT, effect_timing, (val > 0 ? 16.0 : 0.0625), rep, encoder, sub_classify_tensor);
				}
				else
				{
					alle = generate AllegianceCond(tmp_init_config, tmp_config, TARGET_MODE_DEFAULT, effect_timing, (val > 0 ? 16.0 : 0.0625), rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_MAX_MP_EFF, <double>{NormalizeCode(val, -10, 10)}));
				alle->FillRep(rep);
			}
			FillTokens
			{
				alle->FillTokensAlt5(tokens, indent_size, need_starting_newline);
				string tmp_verb = (val >= 0 ? "gain" : "lose");
				if (alle->IsThirdPersonSingle())
					tmp_verb += "s";
				tokens.push_back(mkWordToken(tmp_verb, indent_size, false));
				tokens.push_back(mkWordToken(IntToStr(abs(val)), indent_size, false));
				tokens.push_back(mkWordToken("Max", indent_size, false));
				tokens.push_back(mkWordToken("MP", indent_size, false));
				tokens.push_back(mkWordToken("and", indent_size, false));
				tokens.push_back(mkWordToken("MP", indent_size, false));
			}
			Detail = alle->DetailAlt5() + (val >= 0 ? "gain" : "lose") + (alle->IsThirdPersonSingle() ? "s " : " ") + IntToStr(abs(val)) + " Max MP (and MP)";
			CreateNodeHardCopy = new maxMpEff(card_copy, val,
				(AllegianceCond*)(alle->CreateNodeHardCopy(card_copy, redir_map)));
			GetLeaderConfig
			{
				CondConfig tmp_config;
				if (val < 0)
					tmp_config = GetMaxMpConfig(-val, 10);
				else
					tmp_config = GetMaxMpConfig(0, 10 - val);
				tmp_config &= alle->GetTargetConfig(); // used to distinguish which side(s) this is applied on
				return tmp_config;
			}
			UntargetedAction
			{
				Card* target_a = parent_card->owner->leader;
				if (alle->CheckCardValid(target_a, parent_card))
					parent_card->owner->ModifyMaxMp(val);
				Card* target_b = parent_card->opponent->leader;
				if (alle->CheckCardValid(target_b, parent_card))
					parent_card->opponent->ModifyMaxMp(val);
			}
		}
	| newEff: int val, Destination* dest, NewCardVariant* variant
		{
			generator
			{
				if (rep)
				{
					val = DenormalizeCode(rep->term_info[0], 1, 7);
					rep++;
				}
				else if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_untargeted_eff_decoder_to_intermediate, false);
					torch_Tensor value_tensor = ForwardValueOut(intermediate_code, encoder->base_untargeted_eff_decoder_value);
					val = DenormalizeCode(GetSingleValFromTensor(value_tensor), 1, 7);
				}
				else
				{
					val = SampleRangeLowerConcentratedPolynomialFromInt(1, 7, 8.0);
				}

				CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_untargeted_eff_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->base_untargeted_eff_decoder_hidden, false);
					torch_Tensor sub_intermediate_code = UnitForward(hidden_tensor.narrow(0, 0, Base_Targeted_Eff_Size), encoder->new_eff_decoder_to_intermediate, false);
					torch_Tensor sub_classify_tensor = ForwardClassifyOut(sub_intermediate_code, encoder->new_eff_decoder_classify);
					torch_Tensor sub_value_tensor = ForwardValueOut(sub_intermediate_code, encoder->new_eff_decoder_value);
					torch_Tensor sub_hidden_tensor = UnitForward(sub_intermediate_code, encoder->new_eff_decoder_hidden, false);
					dest = generate Destination(tmp_init_config, TARGET_MODE_NEW, effect_timing, TARGET_MODE_NEW_DEST, rep, encoder, sub_classify_tensor.narrow(0, 0, Destination_Size));
					CondConfig tmp_config = dest->GetTargetConfig(); // to counter the problem of rvalue passed to lvalue ref
					torch_Tensor tmp_cat_list[] = { sub_classify_tensor.narrow(0, Destination_Size, 6), sub_hidden_tensor, sub_value_tensor };
					variant = generate NewCardVariant(tmp_config, TARGET_MODE_NEW, effect_timing, effect_depth, rep, encoder, torch_cat(tmp_cat_list));
				}
				else
				{
					dest = generate Destination(tmp_init_config, TARGET_MODE_NEW, effect_timing, TARGET_MODE_NEW_DEST, rep, encoder, code);
					CondConfig tmp_config = dest->GetTargetConfig(); // to counter the problem of rvalue passed to lvalue ref
					variant = generate NewCardVariant(tmp_config, TARGET_MODE_NEW, effect_timing, effect_depth, rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_NEW_EFF, <double>{NormalizeCode(val, 1, 7)}));
				dest->FillRep(rep);
				variant->FillRep(rep);
			}
			FillTokens
			{
				dest->FillTokens(tokens, indent_size, need_starting_newline);
				if (val == 1)
				{
					variant->FillTokens(tokens, indent_size, need_starting_newline);
				}
				else
				{
					tokens.push_back(mkWordToken(IntToStr(val), indent_size, false));
					variant->FillTokensAlt1(tokens, indent_size, need_starting_newline);
				}
				dest->FillTokensSuffix(tokens, indent_size);
			}
			FillTokensPostfix = variant->FillTokensPostfix(tokens, indent_size);
			Detail = dest->Detail() + (val == 1 ? variant->Detail() : IntToStr(val) + " " + variant->DetailAlt1()) + dest->Suffix();
			Postfix = variant->Postfix();
			PostfixIndent = variant->PostfixIndent(indent_size);
			CreateNodeHardCopy = new newEff(card_copy, val,
				(Destination*)(dest->CreateNodeHardCopy(card_copy, redir_map)),
				(NewCardVariant*)(variant->CreateNodeHardCopy(card_copy, redir_map)));
			UntargetedAction
			{
				bool start_of_batch = true;
				for (int i = 0; i < val; i++)
				{
					Card* card = variant->GetRelatedCard(parent_card);
					dest->SubTargetedAction(card, parent_card, start_of_batch);
					start_of_batch = false;
				}
			}
		}
	| winGameEff: AllegianceCond* alle
		{
			generator
			{
				if (rep) rep++;

				CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
				CondConfig tmp_config = GetDefaultConfig(); // to counter the problem of rvalue passed to lvalue ref
				if (encoder)
				{
					torch_Tensor intermediate_code = UnitForward(code, encoder->base_untargeted_eff_decoder_to_intermediate, false);
					torch_Tensor hidden_tensor = UnitForward(intermediate_code, encoder->base_untargeted_eff_decoder_hidden, false);
					torch_Tensor sub_intermediate_code = UnitForward(hidden_tensor.narrow(0, Base_Targeted_Eff_Size, Cond_Size), encoder->alle_to_cond_decoder_to_intermediate, false);
					torch_Tensor sub_classify_tensor = ForwardClassifyOut(sub_intermediate_code, encoder->alle_to_cond_decoder_classify);
					alle = generate AllegianceCond(tmp_init_config, tmp_config, TARGET_MODE_WIN_GAME, effect_timing, 8.0, rep, encoder, sub_classify_tensor);
				}
				else
				{
					alle = generate AllegianceCond(tmp_init_config, tmp_config, TARGET_MODE_WIN_GAME, effect_timing, 8.0, rep, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_WIN_GAME_EFF));
				alle->FillRep(rep);
			}
			FillTokens
			{
				alle->FillTokensAlt5(tokens, indent_size, need_starting_newline);
				string tmp_verb = "win";
				if (alle->IsThirdPersonSingle())
					tmp_verb += "s";
				tokens.push_back(mkWordToken(tmp_verb, indent_size, false));
				tokens.push_back(mkWordToken("the", indent_size, false));
				tokens.push_back(mkWordToken("game", indent_size, false));
			}
			Detail = alle->DetailAlt5() + "win" + (alle->IsThirdPersonSingle() ? "s " : " ") + "the game"; // well we don't actually want to have both players win the game but at least this is left as a possibility for constructed cards
			CreateNodeHardCopy = new winGameEff(card_copy, (AllegianceCond*)(alle->CreateNodeHardCopy(card_copy, redir_map)));
			UntargetedAction
			{
				Card* target_a = parent_card->owner->leader;
				if (alle->CheckCardValid(target_a, parent_card))
					parent_card->opponent->SetLose();
				Card* target_b = parent_card->opponent->leader;
				if (alle->CheckCardValid(target_b, parent_card))
					parent_card->owner->SetLose();
			}
		}


	Destination
	default:
		{
			preselector
			{
				if (rep)
				{
					force [(rep++)->choice]; // no terminal info, fine to ++
				}
				else if (encoder)
				{
					double choice_code_adjust[6] = {0.0};
					
					if (target_mode == TARGET_MODE_PLAY)
					{
						if (dest_mode == TARGET_MODE_MOVE_DEST)
							choice_code_adjust[CHOICE_OWN_FIELD_DEST] = -1e10; // this is because owner type of destination wants anyAllegiance, but it would require "your hand" for moving to field for a targeted effect
					}
					else if (target_mode == TARGET_MODE_SELF)
					{
						choice_code_adjust[CHOICE_OWN_FIELD_DEST] = choice_code_adjust[CHOICE_OWN_HAND_DEST] = choice_code_adjust[CHOICE_OWN_DECK_DEST] = -1e10; // for selfeff, these are covered by "yours"
						if ((self_config & TARGET_IS_LEADER) || (self_config & TARGET_IS_SPELL))
							choice_code_adjust[CHOICE_FIELD_DEST] = -1e10;
						if (dest_mode == TARGET_MODE_COPY_DEST && !(self_config & TARGET_NOT_DECK)) // forbid deck to deck copy
							choice_code_adjust[CHOICE_DECK_DEST] = -1e10;
					}
					else if (target_mode == TARGET_MODE_NEW)
					{
						choice_code_adjust[CHOICE_OWN_FIELD_DEST] = choice_code_adjust[CHOICE_OWN_HAND_DEST] = choice_code_adjust[CHOICE_OWN_DECK_DEST] = -1e10;
					}

					torch_Tensor adjust_tensor = CreateVectorTensorFromArray(choice_code_adjust, 6);
					torch_Tensor choice_tensor = code.narrow(0, 0, 6);
					torch_Tensor final_tensor = choice_tensor + adjust_tensor;
					force [GetMaxIndex(final_tensor)];
				}
				else
				{
					if (target_mode == TARGET_MODE_PLAY)
					{
						if (dest_mode == TARGET_MODE_MOVE_DEST)
							forbid ownerFieldDest; // this is because owner type of destination wants anyAllegiance, but it would require "your hand" for moving to field for a targeted effect
					}
					else if (target_mode == TARGET_MODE_SELF)
					{
						forbid ownerFieldDest, ownerHandDest, ownerDeckDest; // for selfeff, these are covered by "yours"
						if ((self_config & TARGET_IS_LEADER) || (self_config & TARGET_IS_SPELL))
							forbid fieldDest;
						if (dest_mode == TARGET_MODE_COPY_DEST && !(self_config & TARGET_NOT_DECK)) // forbid deck to deck copy
							forbid deckDest;

						// considering ally bias, because for the same type of pos it has to move to the opponent side
						if (dest_mode == TARGET_MODE_MOVE_DEST)
						{
							if (!(self_config & TARGET_POS_HAND_OR_DECK))
								probof(fieldDest) *= 0.125;
							else if (!(self_config & TARGET_NOT_HAND))
								probof(handDest) *= 0.125;
							else if (!(self_config & TARGET_NOT_DECK))
								probof(deckDest) *= 0.125;
						}
					}
					else if (target_mode == TARGET_MODE_NEW)
					{
						forbid ownerFieldDest, ownerHandDest, ownerDeckDest;
					}
				}
			}
		}
	:=
	| fieldDest: AllegianceCond* alle
		{
			generator 
			{
				CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
				CondConfig tmp_config = GetDefaultConfig();
				if (target_mode == TARGET_MODE_SELF && dest_mode == TARGET_MODE_MOVE_DEST && !(self_config & TARGET_POS_HAND_OR_DECK))
					tmp_config = OPPO_COND_FILTER;

				double ally_bias = 1.0;
				switch (dest_mode)
				{
					case TARGET_MODE_MOVE_DEST:
						ally_bias = 4.0;
						break;
					case TARGET_MODE_COPY_DEST:
						ally_bias = 16.0;
						break;
					case TARGET_MODE_NEW_DEST:
						ally_bias = 8.0;
						break;
				}
				if (target_mode == TARGET_MODE_SELF)
					ally_bias *= 2.0;

				if (encoder)
					alle = generate AllegianceCond(tmp_init_config, tmp_config, dest_mode, effect_timing, ally_bias, rep, encoder, code.narrow(0, 6, Alle_Cond_Size));
				else
					alle = generate AllegianceCond(tmp_init_config, tmp_config, dest_mode, effect_timing, ally_bias, rep, encoder, code);
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_FIELD_DEST));
				alle->FillRep(rep);
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Summon", indent_size, false));
			}
			FillTokensSuffix
			{
				int tmp_need_starting_newline = 0; 
				tokens.push_back(mkWordToken("to", indent_size, false));
				alle->FillTokensAlt2(tokens, indent_size, tmp_need_starting_newline);
				tokens.push_back(mkWordToken((alle->IsPlural() ? "fields" : "field"), indent_size, false));
			}
			Detail = "Summon ";
			Suffix = " to " + alle->DetailAlt2() + (alle->IsPlural() ? "fields (duplicate on both sides)" : "field");
			CreateNodeHardCopy = new fieldDest(card_copy, (AllegianceCond*)(alle->CreateNodeHardCopy(card_copy, redir_map)));
			GetAllyBias // this is more like a correction part, will be involved as a part in the corresponding BaseTargetedEff (move and copy)
			{
				double ally_bias = 1.0;
				CondConfig filter = alle->GetTargetConfig();
				if (filter & TARGET_IS_ALLY)
					ally_bias *= 2.0;
				if (filter & TARGET_IS_OPPO)
					ally_bias *= 0.125;
				return ally_bias;
			}
			GetTargetConfig = alle->GetTargetConfig() & FIELD_COND_FILTER & MINION_COND_FILTER; // this is not really config for cards but rather the config for the destination (or what the card would end up like after being moved/added)
			SubTargetedAction
			{
				bool is_ally = alle->GetTargetConfig() & TARGET_IS_ALLY;
				bool is_opponent = alle->GetTargetConfig() & TARGET_IS_OPPO;
				if (is_ally)
				{
					parent_card->owner->FlagFieldSummon(target, start_of_batch);
					if (is_opponent) // if both ally and opponent apply, create a new copy (note, this will create a duplicate even for dynamically randomized card)
						parent_card->opponent->FlagFieldSummon(target->CreateCopy(), false);
				}
				else
				{
					if (is_opponent)
						parent_card->opponent->FlagFieldSummon(target, start_of_batch);
				}
			}
		}
	| ownerFieldDest:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_OWN_FIELD_DEST));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Summon", indent_size, false));
			}
			FillTokensSuffix
			{
				tokens.push_back(mkWordToken("to", indent_size, false));
				tokens.push_back(mkWordToken("its", indent_size, false));
				tokens.push_back(mkWordToken("owner\'s", indent_size, false));
				tokens.push_back(mkWordToken("field", indent_size, false));
			}
			FillTokensSuffixAlt1
			{
				tokens.push_back(mkWordToken("to", indent_size, false));
				tokens.push_back(mkWordToken("their", indent_size, false));
				tokens.push_back(mkWordToken("owners\'", indent_size, false));
				tokens.push_back(mkWordToken("fields", indent_size, false));
			}
			Detail = "Summon ";
			Suffix = " to its owner\'s field";
			SuffixAlt1 = " to their owners\' fields"; // for plural
			CreateNodeHardCopy = new ownerFieldDest(card_copy);
			GetTargetConfig = ~TARGET_ANY_ALLE_TYPE & FIELD_COND_FILTER & MINION_COND_FILTER; // we use all zero bits for allegiance to indicate the "owner" versions
			SubTargetedAction
			{
				target->owner->FlagFieldSummon(target, start_of_batch);
			}
		}
	| handDest: AllegianceCond* alle
		{
			generator
			{
				CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
				CondConfig tmp_config = GetDefaultConfig();
				if (target_mode == TARGET_MODE_SELF && dest_mode == TARGET_MODE_MOVE_DEST && !(self_config & TARGET_NOT_HAND))
					tmp_config = OPPO_COND_FILTER;
				
				double ally_bias = 1.0;
				switch (dest_mode)
				{
					case TARGET_MODE_MOVE_DEST:
						ally_bias = 2.0;
						break;
					case TARGET_MODE_COPY_DEST:
						ally_bias = 8.0;
						break;
					case TARGET_MODE_NEW_DEST:
						ally_bias = 4.0;
						break;
				}
				if (target_mode == TARGET_MODE_SELF)
					ally_bias *= 4.0;

				if (encoder)
					alle = generate AllegianceCond(tmp_init_config, tmp_config, dest_mode, effect_timing, ally_bias, rep, encoder, code.narrow(0, 6, Alle_Cond_Size));
				else
					alle = generate AllegianceCond(tmp_init_config, tmp_config, dest_mode, effect_timing, ally_bias, rep, encoder, code);
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_HAND_DEST));
				alle->FillRep(rep);
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Put", indent_size, false));
			}
			FillTokensSuffix
			{
				int tmp_need_starting_newline = 0; 
				tokens.push_back(mkWordToken("to", indent_size, false));
				alle->FillTokensAlt2(tokens, indent_size, tmp_need_starting_newline);
				tokens.push_back(mkWordToken((alle->IsPlural() ? "hands" : "hand"), indent_size, false));
			}
			Detail = "Put ";
			Suffix = " to " + alle->DetailAlt2() + (alle->IsPlural() ? "hands (duplicate on both sides)" : "hand");
			CreateNodeHardCopy = new handDest(card_copy, (AllegianceCond*)(alle->CreateNodeHardCopy(card_copy, redir_map)));
			GetAllyBias // this is more like a correction part, will be involved as a part in the corresponding BaseTargetedEff (move and copy)
			{
				double ally_bias = 1.0;
				CondConfig filter = alle->GetTargetConfig();
				if (filter & TARGET_IS_ALLY)
					ally_bias *= 2.0;
				if (filter & TARGET_IS_OPPO)
					ally_bias *= 0.0625;
				return ally_bias;
			}
			GetTargetConfig = alle->GetTargetConfig() & HAND_COND_FILTER; // this is not really config for cards but rather the config for the destination (or what the card would end up like after being moved/added)
			SubTargetedAction
			{
				bool is_ally = alle->GetTargetConfig() & TARGET_IS_ALLY;
				bool is_opponent = alle->GetTargetConfig() & TARGET_IS_OPPO;
				if (is_ally)
				{
					parent_card->owner->FlagHandPut(target, start_of_batch);
					if (is_opponent) // if both ally and opponent apply, create a new copy (note, this will create a duplicate even for dynamically randomized card)
						parent_card->opponent->FlagHandPut(target->CreateCopy(), false);
				}
				else
				{
					if (is_opponent)
						parent_card->opponent->FlagHandPut(target, start_of_batch);
				}
			}
		}
	| ownerHandDest:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_OWN_HAND_DEST));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Put", indent_size, false));
			}
			FillTokensSuffix
			{
				tokens.push_back(mkWordToken("to", indent_size, false));
				tokens.push_back(mkWordToken("its", indent_size, false));
				tokens.push_back(mkWordToken("owner\'s", indent_size, false));
				tokens.push_back(mkWordToken("hand", indent_size, false));
			}
			FillTokensSuffixAlt1
			{
				tokens.push_back(mkWordToken("to", indent_size, false));
				tokens.push_back(mkWordToken("their", indent_size, false));
				tokens.push_back(mkWordToken("owners\'", indent_size, false));
				tokens.push_back(mkWordToken("hands", indent_size, false));
			}
			Detail = "Put ";
			Suffix = " to its owner\'s hand";
			SuffixAlt1 = " to their owners\' hands"; // for plural
			CreateNodeHardCopy = new ownerHandDest(card_copy);
			GetTargetConfig = ~TARGET_ANY_ALLE_TYPE & HAND_COND_FILTER; // we use all zero bits for allegiance to indicate the "owner" versions
			SubTargetedAction
			{
				target->owner->FlagHandPut(target, start_of_batch);
			}
		}
	| deckDest: AllegianceCond* alle
		{
			generator
			{
				CondConfig tmp_init_config = GetDefaultInitConfig(); // to counter the problem of rvalue passed to lvalue ref
				CondConfig tmp_config = GetDefaultConfig();
				if (target_mode == TARGET_MODE_SELF && dest_mode == TARGET_MODE_MOVE_DEST && !(self_config & TARGET_NOT_DECK))
					tmp_config = OPPO_COND_FILTER;
				
				double ally_bias = 1.0;
				switch (dest_mode)
				{
					case TARGET_MODE_MOVE_DEST:
						ally_bias = 1.0;
						break;
					case TARGET_MODE_COPY_DEST:
						ally_bias = 4.0;
						break;
					case TARGET_MODE_NEW_DEST:
						ally_bias = 2.0;
						break;
				}
				if (target_mode == TARGET_MODE_SELF)
					ally_bias *= 8.0;

				if (encoder)
					alle = generate AllegianceCond(tmp_init_config, tmp_config, dest_mode, effect_timing, ally_bias, rep, encoder, code.narrow(0, 6, Alle_Cond_Size));
				else
					alle = generate AllegianceCond(tmp_init_config, tmp_config, dest_mode, effect_timing, ally_bias, rep, encoder, code);
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_DECK_DEST));
				alle->FillRep(rep);
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Shuffle", indent_size, false));
			}
			FillTokensSuffix
			{
				int tmp_need_starting_newline = 0; 
				tokens.push_back(mkWordToken("to", indent_size, false));
				alle->FillTokensAlt2(tokens, indent_size, tmp_need_starting_newline);
				tokens.push_back(mkWordToken((alle->IsPlural() ? "decks (duplicate on both sides)" : "deck"), indent_size, false));
			}
			Detail = "Shuffle ";
			Suffix = " to " + alle->DetailAlt2() + (alle->IsPlural() ? "decks" : "deck");
			CreateNodeHardCopy = new deckDest(card_copy, (AllegianceCond*)(alle->CreateNodeHardCopy(card_copy, redir_map)));
			GetAllyBias // this is more like a correction part, will be involved as a part in the corresponding BaseTargetedEff (move and copy)
			{
				double ally_bias = 1.0;
				CondConfig filter = alle->GetTargetConfig();
				if (filter & TARGET_IS_ALLY)
					ally_bias *= 2.0;
				if (filter & TARGET_IS_OPPO)
					ally_bias *= 0.03125;
				return ally_bias;
			}
			GetTargetConfig = alle->GetTargetConfig() & DECK_COND_FILTER; // this is not really config for cards but rather the config for the destination (or what the card would end up like after being moved/added)
			SubTargetedAction
			{
				bool is_ally = alle->GetTargetConfig() & TARGET_IS_ALLY;
				bool is_opponent = alle->GetTargetConfig() & TARGET_IS_OPPO;
				if (is_ally)
				{
					parent_card->owner->FlagDeckShuffle(target, start_of_batch);
					if (is_opponent) // if both ally and opponent apply, create a new copy (note, this will create a duplicate even for dynamically randomized card)
						parent_card->opponent->FlagDeckShuffle(target->CreateCopy(), false);
				}
				else
				{
					if (is_opponent)
						parent_card->opponent->FlagDeckShuffle(target, start_of_batch);
				}
			}
		}
	| ownerDeckDest:
		{
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_OWN_DECK_DEST));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("Shuffle", indent_size, false));
			}
			FillTokensSuffix
			{
				tokens.push_back(mkWordToken("to", indent_size, false));
				tokens.push_back(mkWordToken("its", indent_size, false));
				tokens.push_back(mkWordToken("owner\'s", indent_size, false));
				tokens.push_back(mkWordToken("deck", indent_size, false));
			}
			FillTokensSuffixAlt1
			{
				tokens.push_back(mkWordToken("to", indent_size, false));
				tokens.push_back(mkWordToken("their", indent_size, false));
				tokens.push_back(mkWordToken("owners\'", indent_size, false));
				tokens.push_back(mkWordToken("decks", indent_size, false));
			}
			Detail = "Shuffle ";
			Suffix = " to its owner\'s deck";
			SuffixAlt1 = " to their owners\' decks"; // for plural
			CreateNodeHardCopy = new ownerDeckDest(card_copy);
			GetTargetConfig = ~TARGET_ANY_ALLE_TYPE & DECK_COND_FILTER; // we use all zero bits for allegiance to indicate the "owner" versions
			SubTargetedAction
			{
				target->owner->FlagDeckShuffle(target, start_of_batch);
			}
		}

	NewCardVariant
	default:
		{
			preselector
			{
				if (rep)
				{
					force [rep->choice];
				}
				else if (encoder)
				{
					double choice_code_adjust[6] = {0.0};
					
					if (target_mode == TARGET_MODE_SELF || target_mode == TARGET_MODE_NEW) // the new mode uses self_config for the config from selecting destination
					{
						if (!(self_config & TARGET_POS_FIELD) || !(self_config & TARGET_NOT_LEADER)) // the second half makes sure not transforming played leaders
							choice_code_adjust[CHOICE_PLAIN_MINION] = choice_code_adjust[CHOICE_FIXED_MINION] = choice_code_adjust[CHOICE_RANDOM_MINION] = -1e10;
						if (!(self_config & TARGET_POS_HAND_OR_DECK))
							choice_code_adjust[CHOICE_PLAIN_MINION_CARD] = choice_code_adjust[CHOICE_FIXED_CARD] = choice_code_adjust[CHOICE_RANDOM_CARD] = -1e10;
					}

					torch_Tensor adjust_tensor = CreateVectorTensorFromArray(choice_code_adjust, 6);
					torch_Tensor choice_tensor = code.narrow(0, 0, 6);
					torch_Tensor final_tensor = choice_tensor + adjust_tensor;
					force [GetMaxIndex(final_tensor)];
				}
				else
				{
					if (target_mode == TARGET_MODE_SELF || target_mode == TARGET_MODE_NEW) // the new mode uses self_config for the config from selecting destination
					{
						if (!(self_config & TARGET_POS_FIELD) || !(self_config & TARGET_NOT_LEADER)) // the second half makes sure not transforming played leaders
							forbid plainMinion, fixedMinion, randomMinion;
						if (!(self_config & TARGET_POS_HAND_OR_DECK))
							forbid plainMinionCard, fixedCard, randomCard;
					}
				}
			}
		}
	:=
	| plainMinionCard: Card* card
		{
			generator
			{
				if (rep)
				{
					rep++;
					card = CreateNamedCardFromRep(name + "_Spawn_#" + IntToStr(GetRandInt()), rep); // note as the spawned token name is not provided so we just append a random number in the name
				}
				else if (encoder)
				{
					card = CreatePlainMinionToken(name, encoder, code.narrow(0, 6, Card_Root_Size));
				}
				else
				{
					card = CreatePlainMinionToken(name, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_PLAIN_MINION_CARD));
				card->FillRep(rep);
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken(IntToStr(card->orig_mana) + "/" + IntToStr(card->orig_atk) + "/" + IntToStr(card->orig_hp), indent_size, false));
				tokens.push_back(mkWordToken(MinionTypeDescription(card->minion_type), indent_size, false));
				tokens.push_back(mkWordToken("card", indent_size, false));
				AbilityFillTokensInline(card->orig_is_charge, card->orig_is_taunt, card->orig_is_stealth, card->orig_is_untargetable, card->orig_is_shielded, card->orig_is_poisonous, card->orig_is_lifesteal, tokens, indent_size);
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken(IntToStr(card->orig_mana) + "/" + IntToStr(card->orig_atk) + "/" + IntToStr(card->orig_hp), indent_size, false));
				tokens.push_back(mkWordToken(MinionTypeDescription(card->minion_type), indent_size, false));
				tokens.push_back(mkWordToken("cards", indent_size, false));
				AbilityFillTokensInline(card->orig_is_charge, card->orig_is_taunt, card->orig_is_stealth, card->orig_is_untargetable, card->orig_is_shielded, card->orig_is_poisonous, card->orig_is_lifesteal, tokens, indent_size);
			}
			Detail = "a " + IntToStr(card->orig_mana) + "/" + IntToStr(card->orig_atk) + "/" + IntToStr(card->orig_hp) + " " + MinionTypeDescription(card->minion_type) + " card"
					+ AbilityDescriptionInline(card->orig_is_charge, card->orig_is_taunt, card->orig_is_stealth, card->orig_is_untargetable, card->orig_is_shielded, card->orig_is_poisonous, card->orig_is_lifesteal);
			DetailAlt1 = IntToStr(card->orig_mana) + "/" + IntToStr(card->orig_atk) + "/" + IntToStr(card->orig_hp) + " " + MinionTypeDescription(card->minion_type) + " cards"
					+ AbilityDescriptionInline(card->orig_is_charge, card->orig_is_taunt, card->orig_is_stealth, card->orig_is_untargetable, card->orig_is_shielded, card->orig_is_poisonous, card->orig_is_lifesteal); // for plural
			CreateNodeHardCopy = new plainMinionCard(card_copy, card->CreateHardCopy(redir_map));
			GetTargetConfig = HAND_OR_DECK_COND_FILTER; // note: do not need MINION_COND_FITLER as the target (for a transform effect does not have to be minion card)
			GetRelatedCard = card->CreateCopy();
		}
	| plainMinion: Card* card
		{
			generator
			{
				if (rep)
				{
					rep++;
					card = CreateNamedCardFromRep(name + "_Spawn_#" + IntToStr(GetRandInt()), rep); // note as the spawned token name is not provided so we just append a random number in the name
				}
				else if (encoder)
				{
					card = CreatePlainMinionToken(name, encoder, code.narrow(0, 6, Card_Root_Size));
				}
				else
				{
					card = CreatePlainMinionToken(name, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_PLAIN_MINION));
				card->FillRep(rep);
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken(IntToStr(card->orig_mana) + "/" + IntToStr(card->orig_atk) + "/" + IntToStr(card->orig_hp), indent_size, false));
				tokens.push_back(mkWordToken(MinionTypeDescription(card->minion_type), indent_size, false));
				tokens.push_back(mkWordToken("minion", indent_size, false));
				AbilityFillTokensInline(card->orig_is_charge, card->orig_is_taunt, card->orig_is_stealth, card->orig_is_untargetable, card->orig_is_shielded, card->orig_is_poisonous, card->orig_is_lifesteal, tokens, indent_size);
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken(IntToStr(card->orig_mana) + "/" + IntToStr(card->orig_atk) + "/" + IntToStr(card->orig_hp), indent_size, false));
				tokens.push_back(mkWordToken(MinionTypeDescription(card->minion_type), indent_size, false));
				tokens.push_back(mkWordToken("minions", indent_size, false));
				AbilityFillTokensInline(card->orig_is_charge, card->orig_is_taunt, card->orig_is_stealth, card->orig_is_untargetable, card->orig_is_shielded, card->orig_is_poisonous, card->orig_is_lifesteal, tokens, indent_size);
			}
			Detail = "a " + IntToStr(card->orig_mana) + "/" + IntToStr(card->orig_atk) + "/" + IntToStr(card->orig_hp) + " " + MinionTypeDescription(card->minion_type) + " minion"
					+ AbilityDescriptionInline(card->orig_is_charge, card->orig_is_taunt, card->orig_is_stealth, card->orig_is_untargetable, card->orig_is_shielded, card->orig_is_poisonous, card->orig_is_lifesteal);
			DetailAlt1 = IntToStr(card->orig_mana) + "/" + IntToStr(card->orig_atk) + "/" + IntToStr(card->orig_hp) + " " + MinionTypeDescription(card->minion_type) + " minions"
					+ AbilityDescriptionInline(card->orig_is_charge, card->orig_is_taunt, card->orig_is_stealth, card->orig_is_untargetable, card->orig_is_shielded, card->orig_is_poisonous, card->orig_is_lifesteal); // for plural
			CreateNodeHardCopy = new plainMinion(card_copy, card->CreateHardCopy(redir_map));
			GetTargetConfig = MINION_COND_FILTER & FIELD_COND_FILTER;
			GetRelatedCard = card->CreateCopy();
		}
	| fixedCard: Card* card
		{
			generator
			{
				if (rep)
				{
					rep++;
					card = CreateNamedCardFromRep(name + "_Spawn_#" + IntToStr(GetRandInt()), rep); // note as the spawned token name is not provided so we just append a random number in the name
				}
				else
				{
					int next_depth = effect_depth + 1;
					int min_num_effs = 1;
					int max_num_effs = max_eff_num >> next_depth;
					if (encoder)
						card = CreateFixedCardToken(name, min_num_effs, max_num_effs, next_depth, encoder, code.narrow(0, 6, Card_Root_Size));
					else
						card = CreateFixedCardToken(name, min_num_effs, max_num_effs, next_depth, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_FIXED_CARD));
				card->FillRep(rep);
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken("copy", indent_size, false));
				tokens.push_back(mkWordToken("of", indent_size, false));
				tokens.push_back(mkWordToken("the", indent_size, false));
				tokens.push_back(mkWordToken("following", indent_size, false));
				tokens.push_back(mkWordToken("card", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("copies", indent_size, false));
				tokens.push_back(mkWordToken("of", indent_size, false));
				tokens.push_back(mkWordToken("the", indent_size, false));
				tokens.push_back(mkWordToken("following", indent_size, false));
				tokens.push_back(mkWordToken("card", indent_size, false));
			}
			FillTokensPostfix
			{
				card->FillCardDescTokens(tokens, indent_size + 2, true);
			}
			Detail = "a copy of the following card";
			DetailAlt1 = "copies of the following card"; // for plural
			Postfix = "\n" + card->DetailIndent(4);
			PostfixIndent = "\n" + card->DetailIndent(indent_size + 4);
			CreateNodeHardCopy = new fixedCard(card_copy, card->CreateHardCopy(redir_map));
			GetTargetConfig = HAND_OR_DECK_COND_FILTER;
			GetRelatedCard = card->CreateCopy();
		}
	| fixedMinion: Card* card
		{
			generator
			{ 
				if (rep)
				{
					rep++;
					card = CreateNamedCardFromRep(name + "_Spawn_#" + IntToStr(GetRandInt()), rep);
				}
				else
				{
					int next_depth = effect_depth + 1;
					int min_num_effs = 1;
					int max_num_effs = max_eff_num >> next_depth;
					if (encoder)
						card = CreateFixedMinionToken(name, min_num_effs, max_num_effs, next_depth, encoder, code.narrow(0, 6, Card_Root_Size));
					else
						card = CreateFixedMinionToken(name, min_num_effs, max_num_effs, next_depth, encoder, code);
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_FIXED_MINION));
				card->FillRep(rep);
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken("copy", indent_size, false));
				tokens.push_back(mkWordToken("of", indent_size, false));
				tokens.push_back(mkWordToken("the", indent_size, false));
				tokens.push_back(mkWordToken("following", indent_size, false));
				tokens.push_back(mkWordToken("minion", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("copies", indent_size, false));
				tokens.push_back(mkWordToken("of", indent_size, false));
				tokens.push_back(mkWordToken("the", indent_size, false));
				tokens.push_back(mkWordToken("following", indent_size, false));
				tokens.push_back(mkWordToken("minion", indent_size, false));
			}
			FillTokensPostfix
			{
				card->FillCardDescTokens(tokens, indent_size + 2, true);
			}
			Detail = "a copy the following minion";
			DetailAlt1 = "copies of the following minion"; // for plural
			Postfix = "\n" + card->DetailIndent(4);
			PostfixIndent = "\n" + card->DetailIndent(indent_size + 4);
			CreateNodeHardCopy = new fixedMinion(card_copy, card->CreateHardCopy(redir_map));
			GetTargetConfig = MINION_COND_FILTER & FIELD_COND_FILTER;
			GetRelatedCard = card->CreateCopy();
		}
	| randomCard: int cost
		{
			generator
			{ 
				if (rep)
				{
					cost = DenormalizeCode(rep->term_info[0], 0, 10);
					rep++;
				}
				else if (encoder)
				{
					cost = DenormalizeCode(GetSingleValFromTensor(code.narrow(0, 6 + Card_Root_Size, 1)), 0, 10);
				}
				else
				{
					cost = SampleDefaultStat();
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_RANDOM_CARD, <double>{NormalizeCode(cost, 0, 10)}));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken("random", indent_size, false));
				tokens.push_back(mkWordToken("cost", indent_size, false));
				tokens.push_back(mkWordToken(IntToStr(cost), indent_size, false));
				tokens.push_back(mkWordToken("card", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("random", indent_size, false));
				tokens.push_back(mkWordToken("cost", indent_size, false));
				tokens.push_back(mkWordToken(IntToStr(cost), indent_size, false));
				tokens.push_back(mkWordToken("cards", indent_size, false));
			}
			Detail = "a random cost " + IntToStr(cost) + " card";
			DetailAlt1 = "random cost " + IntToStr(cost) + " cards";
			CreateNodeHardCopy = new randomCard(card_copy, cost);
			GetTargetConfig = HAND_OR_DECK_COND_FILTER;
			GetRelatedCard = GenerateSampledCardWithCost(parent_card->name + "_Spawn", cost, parent_card->owner->encoder); 
		}
	| randomMinion: int cost
		{
			generator
			{ 
				if (rep)
				{
					cost = DenormalizeCode(rep->term_info[0], 0, 7);
					rep++;
				}
				else if (encoder)
				{
					cost = DenormalizeCode(GetSingleValFromTensor(code.narrow(0, 6 + Card_Root_Size, 1)), 0, 7);
				}
				else
				{
					cost = SampleNonLeaderCost();
				}
			}
			FillRep
			{
				rep.push_back(mkNodeRep(CHOICE_RANDOM_MINION, <double>{NormalizeCode(cost, 0, 7)}));
			}
			FillTokens
			{
				tokens.push_back(mkWordToken("a", indent_size, false));
				tokens.push_back(mkWordToken("random", indent_size, false));
				tokens.push_back(mkWordToken("cost", indent_size, false));
				tokens.push_back(mkWordToken(IntToStr(cost), indent_size, false));
				tokens.push_back(mkWordToken("minion", indent_size, false));
			}
			FillTokensAlt1
			{
				tokens.push_back(mkWordToken("random", indent_size, false));
				tokens.push_back(mkWordToken("cost", indent_size, false));
				tokens.push_back(mkWordToken(IntToStr(cost), indent_size, false));
				tokens.push_back(mkWordToken("minions", indent_size, false));
			}
			Detail = "a random cost " + IntToStr(cost) + " minion";
			DetailAlt1 = "random cost " + IntToStr(cost) + " minions";
			CreateNodeHardCopy = new randomMinion(card_copy, cost);
			GetTargetConfig = MINION_COND_FILTER & FIELD_COND_FILTER;
			GetRelatedCard = GenerateSampledMinionWithCost(parent_card->name + "_Spawn", cost, parent_card->owner->encoder);
		}

};

giglconfig GetDefaultGenConfig(int seed, void* extra_config) // using void* as GIGL seems doesn't handle external custom types very well in generator configuration
{
	return <* Card{#seed, 3, 2, #extra_config}: // after separated the config, it is important to add # to the local (non-reference) variables at configuration time, as local addresses are not preserved
		CardRoot := leaderCard @ {0.1} | minionCard @ {0.6} | spellCard @ {0.3},
		AttackTimes := zeroAttack @ {0.02} | singleAttack | multipleAttack @ {0.08},
		MinionType := beastMinion | dragonMinion | demonMinion,
		Abilities := justAbilities,
		DamageAbilities := damageAbilities,
		ChargeAbil := noCharge | justCharge @ {0.05},
		TauntAbil := noTaunt | justTaunt @ {0.05},
		StealthAbil := noStealth | justStealth @ {0.05},
		UntargetableAbil := noUntargetable | justUntargetable @ {0.05},
		ShieldAbil := noShield | justShield @ {0.05},
		PoisonousAbil := noPoisonous | justPoisonous @ {0.01 * (10 - damage)},
		LifestealAbil := noLifesteal | justLifesteal @ {damage == 0 ? 0.001 : 0.05},
		SpecialEffects := specialEffects,
		TargetedPlayEff := noTargetedPlayEff @ {40} | targetedBattlecryEff @ {20} | targetedCastEff @ {60}, // here the probs will not be further manipulated beyond simple forbidding
		OtherEffs := noOtherEffs | consOtherEffs @ {0.1},
		OtherEff := untargetedBattlecryEff @ {35} | untargetedCastEff @ {25} | deathrattleEff @ {25} | onDiscardEff @ {5} | turnStartEff @ {5} | turnEndEff @ {5}, // here the probs will be further manipulated in various ways, it takesinto consideration of the nested effects through giveEffectsEff
		TargetedEff := noCondTargetedEff | indeCondTargetedEff @ {0.07} | srcCondTargetedEff @ {0.03},
		UntargetedEff := noCondUntargetedEff | indeCondUntargetedEff @ {0.07} | srcCondUntargetedEff @ {0.03},
		TargetCond := charTargetCond @ {0.8} | cardTargetCond @ {0.2},
		CharTargetCond := justCharTargetCond,
		CharTypeCond := isCharacter @ {0.4} | isMinion @ {0.3} | isBeast | isDragon | isDemon,
		CardTargetCond := justCardTargetCond,
		CardTypeCond := isCard @ {0.35} | isLeaderCard @ {0.10} | isMinionCard @ {0.30} | isSpellCard @ {0.10} | isBeastCard | isDragonCard | isDemonCard,
		CardPosCond := cardPosAtHandOrDeck @ {0.1} | cardPosAtHand @ {0.7} | cardPosAtDeck @ {0.2},
		AllegianceCond := anyAllegiance @ {0.25 * (ally_bias + 1.0)} | allyAllegiance @ {ally_bias} | oppoAllegiance @ {1.0},
		AbilCond := noAbilCond @ {950} | chargeCond @ {10} | tauntCond @ {10} | stealthCond @ {8} | untargetableCond @ {7} | shieldCond @ {5} | poisonousCond @ {5} | lifestealCond @ {5},
		StatCond := noStatCond @ {900} | costCond @ {30} | atkCond @ {35} | hpCond @ {30} | atkTimesCond @ {5}, 
		StatCondVariant := statGe | statLe,
		IndeCond := fieldExistenceCond @ {0.4} | cardExistenceCond @ {0.2} | leaderCond @ {0.2} | mpCond @ {0.1} | maxMpCond @ {0.1},
		BaseTargetedEff := damageEff @ {250} | healEff @ {150} | resAtkTimesEff @ {20} | costModEff{COST_DEC_PROB} @ {60} | atkHpModEff @ {70} | atkTimesModEff{ATK_TIMES_DEC_PROB} @ {10} | destroyEff @ {90} | discardEff @ {60} | moveEff @ {50} | copyEff @ {50} | transformEff @ {20.0/(1 + effect_depth)}
			| changeToBeastEff @ {10} | changeToDragonEff @ {10} | changeToDemonEff @ {10} | giveChargeEff @ {10} | giveTauntEff @ {10} | giveStealthEff @ {10} | giveUntargetableEff @ {10} | giveShieldEff @ {10} | givePoisonousEff @ {10} | giveLifestealEff @ {10}
			| rmAbilitiesEff @ {20} | setOverheatEff @ {10} | decOhThresholdEff @ {10} | resetStateEff @ {10} | giveEffectsEff @ {20.0/(1 + effect_depth)},
		BaseUntargetedEff := aoeEff @ {400} | randEff @ {250} | leaderEff @ {150} | selfEff @ {50} | drawCardEff @ {100} | mpEff{0.2} @ {10} | maxMpEff{0.3} @ {9} | newEff @ {30} | winGameEff @ {1},
		Destination := fieldDest @ {10} | ownerFieldDest @ {15} | handDest @ {20} | ownerHandDest @ {40} | deckDest @ {5} | ownerDeckDest @ {10},
		NewCardVariant := plainMinionCard @ {20} | plainMinion @ {50} | fixedCard @ {5} | fixedMinion @ {15} | randomCard @ {3} | randomMinion @ {7},
		*>;
}


Card* GenerateCard(int seed)
{
	RandInit(seed);
	return generate Card with GetDefaultGenConfig(seed, nullptr);
}

Card* GenerateCardWithCost(int cost, int seed)
{
	RandInit(seed);
	NodeRep* tmp_nullptr = nullptr;
	string name = "#" + IntToStr(seed) + "_Cost_" + IntToStr(cost);
	void* extra_config = (void*)MkExtraCardGenConfigWithCost(name, tmp_nullptr, cost);
	Card* card = generate Card with GetDefaultGenConfig(-1, extra_config);
	delete (ExtraCardGenConfig*)extra_config;
	return card;
}

Card* CreateDefaultLeader(int hp)
{
	Card* leader = construct Card(leaderCard(9, 0, hp, singleAttack(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	leader->name = "Default Leader";
	return leader;
}

Card* CreateSndPlayerToken()
{
	Card* token = construct Card(spellCard(0, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
						specialEffects(targetedCastEff(noCondTargetedEff(costModEff(-1), cardTargetCond(justCardTargetCond(cardPosAtHand(), allyAllegiance(), isCard(), noAbilCond(), noStatCond())))),
							consOtherEffs(untargetedCastEff(noCondUntargetedEff(drawCardEff(1, allyAllegiance()))),
								noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	token->name = "Second Player Token";
	return token;
}

Card* CreatePlainMinionToken(string parent_name, CardEncoderNet* encoder, const torch_Tensor& code) // set encoder to nullptr to not using latent code
{
	CondConfig tmp_config = GetFlagConfig(MINION_COND_FILTER); // to get around the issue of rvalue passed to lvalue ref
	NodeRep* tmp_nullptr = nullptr; // to get around the issue of ref type
	void* extra_config = (void*)MkExtraCardGenConfigWithLatentCode(parent_name + "_Spawn", tmp_nullptr, encoder, code);
	Card* card = construct Card(generate CardRoot(tmp_config, true, tmp_nullptr, encoder, code)) with GetDefaultGenConfig(-1, extra_config);
	delete (ExtraCardGenConfig*)extra_config;
	return card;
}

Card* CreateFixedCardToken(string parent_name, int min_eff_num, int max_eff_num, int eff_depth, CardEncoderNet* encoder, const torch_Tensor& code) // set encoder to nullptr to not using latent code; also if using the code, it is the one after conditioning (or rather, skipped conditioning)
{
	// this uses a two step process because current implementation item constructor in GIGL relies on lifting decls to global scope, therefore any generation recursive on the item level will overwrite the global lifted variable in the middle of the process and messing it up
	CondConfig tmp_config = GetDefaultConfig();
	NodeRep* tmp_nullptr = nullptr; // to get around the issue of ref type
	void* extra_config = (void*)MkExtraCardGenConfigWithLatentCode(parent_name + "_Spawn", tmp_nullptr, encoder, code);
	Card* card = construct Card(generate CardRoot(tmp_config, true, tmp_nullptr, encoder, code)) with GetDefaultGenConfig(-1, extra_config);
	card->Mutate(min_eff_num, max_eff_num, eff_depth, encoder, code);
	delete (ExtraCardGenConfig*)extra_config;
	return card;
}

Card* CreateFixedMinionToken(string parent_name, int min_eff_num, int max_eff_num, int eff_depth, CardEncoderNet* encoder, const torch_Tensor& code) // set encoder to nullptr to not using latent code; also if using the code, it is the one after conditioning (or rather, skipped conditioning)
{
	// this uses a two step process because current implementation item constructor in GIGL relies on lifting decls to global scope, therefore any generation recursive on the item level will overwrite the global lifted variable in the middle of the process and messing it up
	CondConfig tmp_config = GetFlagConfig(MINION_COND_FILTER);
	NodeRep* tmp_nullptr = nullptr; // to get around the issue of ref type
	void* extra_config = (void*)MkExtraCardGenConfigWithLatentCode(parent_name + "_Spawn", tmp_nullptr, encoder, code); // doesn't need to set forcing_minion in this extra_config, because it is only used in the item level generator which is skipped here
	Card* card = construct Card(generate CardRoot(tmp_config, true, tmp_nullptr, encoder, code)) with GetDefaultGenConfig(-1, extra_config);
	card->Mutate(min_eff_num, max_eff_num, eff_depth, encoder, code);
	delete (ExtraCardGenConfig*)extra_config;
	return card;
}

Card* CreateNamedCardFromRep(const string& name, NodeRep*& rep)
{
	void* extra_config = (void*)MkExtraCardGenConfigWithRep(name, rep);
	Card* card = generate Card with GetDefaultGenConfig(-1, extra_config);
	delete (ExtraCardGenConfig*)extra_config;
	return card;
}

Card* CreateNamedCardFromLatentCode(const string& name, CardEncoderNet* encoder, const torch_Tensor& code) // this code is the one before conditioning
{
	// the condition forwarding will be performed at the item level generator
	NodeRep* tmp_nullptr = nullptr; 
	void* extra_config = (void*)MkExtraCardGenConfigWithLatentCode(name, tmp_nullptr, encoder, code);
	Card* card = generate Card with GetDefaultGenConfig(-1, extra_config);
	delete (ExtraCardGenConfig*)extra_config;
	return card;
}

Card* CreateNamedCardFromLatentCodeWithCost(const string& name, CardEncoderNet* encoder, const torch_Tensor& code, int cost) // this code is the one before conditioning
{
	// the condition forwarding will be performed at the item level generator
	NodeRep* tmp_nullptr = nullptr;
	void* extra_config = (void*)MkExtraCardGenConfigWithLatentCode(name, tmp_nullptr, encoder, code, cost);
	Card* card = generate Card with GetDefaultGenConfig(-1, extra_config);
	delete (ExtraCardGenConfig*)extra_config;
	return card;
}

Card* CreateNamedMinionFromLatentCodeWithCost(const string& name, CardEncoderNet* encoder, const torch_Tensor& code, int cost) // this code is the one before conditioning
{
	// the condition forwarding will be performed at the item level generator
	NodeRep* tmp_nullptr = nullptr;
	void* extra_config = (void*)MkExtraCardGenConfigWithLatentCode(name, tmp_nullptr, encoder, code, cost, true);
	Card* card = generate Card with GetDefaultGenConfig(-1, extra_config);
	delete (ExtraCardGenConfig*)extra_config;
	return card;
}


Card* CreateDemoCard1()
{
	Card* card = construct Card(minionCard(3, 3, 3, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Demo Card 1";
	return card;
}

Card* CreateDemoCard1Alt(int v)
{
	Card* card = construct Card(minionCard(3, v, 6 - v, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Demo Card 1 Alt Ver." + IntToStr(v);
	return card;
}

Card* CreateDemoCard2(int v)
{
	Card* card = construct Card(minionCard(3, v, v, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Demo Card 2 Ver." + IntToStr(v);
	return card;
}

Card* CreateDemoCard3(int v)
{
	Card* card = construct Card(minionCard(v, v, v, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Demo Card 3 Ver." + IntToStr(v);
	return card;
}

Card* CreateDemoCard4(int v)
{
	Card* card = construct Card(minionCard(3, v, v, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedBattlecryEff(noCondTargetedEff(damageEff(6 - v, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(oppoAllegiance(), isCharacter(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Demo Card 4 Ver." + IntToStr(v);
	return card;
}

Card* CreateDemoCard4Alt(int v)
{
	Card* card = construct Card(minionCard(v, v, v, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedBattlecryEff(noCondTargetedEff(damageEff(v, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(oppoAllegiance(), isCharacter(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Demo Card 4 Alt Ver." + IntToStr(v);
	return card;
}

Card* CreateDemoCard5Beast()
{
	Card* card = construct Card(minionCard(3, 3, 3, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedBattlecryEff(noCondTargetedEff(atkHpModEff(3, 3), charTargetCond(justCharTargetCond(allyAllegiance(), isBeast(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Demo Card 5 Beast";
	return card;
}

Card* CreateDemoCard5Dragon()
{
	Card* card = construct Card(minionCard(3, 3, 3, singleAttack(), dragonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedBattlecryEff(noCondTargetedEff(atkHpModEff(3, 3), charTargetCond(justCharTargetCond(allyAllegiance(), isDragon(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Demo Card 5 Dragon";
	return card;
}

Card* CreateDemoCard6Beast(int v)
{
	Card* card = construct Card(minionCard(v, v, v, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedBattlecryEff(noCondTargetedEff(atkHpModEff(v, v), charTargetCond(justCharTargetCond(allyAllegiance(), isBeast(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Demo Card 5 Beast Ver." + IntToStr(v);
	return card;
}

Card* CreateDemoCard6Dragon(int v)
{
	Card* card = construct Card(minionCard(v, v, v, singleAttack(), dragonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedBattlecryEff(noCondTargetedEff(atkHpModEff(v, v), charTargetCond(justCharTargetCond(allyAllegiance(), isDragon(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Demo Card 5 Dragon Ver." + IntToStr(v);
	return card;
}

Card* CreateDemoCard8Beast()
{
	Card* card = construct Card(minionCard(0, 1, 1, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedBattlecryEff(noCondUntargetedEff(newEff(1, handDest(anyAllegiance()), randomCard(3)))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Demo Card 8 Beast";
	return card;
}

Card* CreateDemoCard8Spell()
{
	Card* token = construct Card(minionCard(2, 3, 3, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	token->name = "Demo Card 8 Spell Token";
	Card* card = construct Card(spellCard(3, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(newEff(2, deckDest(allyAllegiance()), randomCard(9)))),
							consOtherEffs(untargetedCastEff(noCondUntargetedEff(newEff(1, fieldDest(allyAllegiance()), plainMinion(token)))),
								noOtherEffs()))))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Demo Card 8 Spell";
	return card;
}

Card* CreateDemoCard8Dragon()
{
	Card* token = construct Card(minionCard(1, 2, 1, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(deathrattleEff(noCondUntargetedEff(aoeEff(decOhThresholdEff(10), charTargetCond(justCharTargetCond(oppoAllegiance(), isCharacter(), noAbilCond(), noStatCond()))))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	token->name = "Demo Card 8 Dragon Token";
	Card* card = construct Card(minionCard(6, 4, 4, singleAttack(), dragonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedBattlecryEff(noCondTargetedEff(transformEff(randomMinion(0)), charTargetCond(justCharTargetCond(oppoAllegiance(), isMinion(), noAbilCond(), noStatCond())))),
						consOtherEffs(deathrattleEff(noCondUntargetedEff(newEff(1, fieldDest(allyAllegiance()), fixedMinion(token)))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Demo Card 8 Dragon";
	return card;
}

Card* CreateDemoCard8Leader()
{
	Card* token1 = construct Card(spellCard(1, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(resetStateEff(), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	token1->name = "Demo Card 8 Leader Token 1";
	Card* token2 = construct Card(minionCard(7, 0, 1, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	token2->name = "Demo Card 8 Leader Token 2";
	Card* card = construct Card(leaderCard(9, 2, 35, multipleAttack(2), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedBattlecryEff(noCondTargetedEff(transformEff(randomCard(6)), cardTargetCond(justCardTargetCond(cardPosAtHand(), allyAllegiance(), isCard(), noAbilCond(), noStatCond())))),
						consOtherEffs(untargetedBattlecryEff(noCondUntargetedEff(newEff(2, handDest(allyAllegiance()), fixedCard(token1)))),
							consOtherEffs(turnEndEff(srcCondUntargetedEff(newEff(1, handDest(oppoAllegiance()), plainMinionCard(token2)), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond()))), allyAllegiance()),
								noOtherEffs()))))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Demo Card 8 Leader";
	return card;
}

Card* CreateDemoCardXBeast(int c) // c stands for cost and should be in 0 ~ 7
{
	Card* card = construct Card(minionCard(c, c + 1, c + 1, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Demo Card X Beast Cost " + IntToStr(c);
	return card;
}

Card* CreateDemoCardXDragon(int c) // c stands for cost and should be in 0 ~ 7
{
	Card* card = construct Card(minionCard(c, c + 1, c + 1, singleAttack(), dragonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Demo Card X Dragon Cost " + IntToStr(c);
	return card;
}

Card* CreateDemoCardXDemon(int c) // c stands for cost and should be in 0 ~ 7
{
	Card* card = construct Card(minionCard(c, c + 1, c + 1, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Demo Card X Demon Cost " + IntToStr(c);
	return card;
}

Card* CreateDemoCardXSpell(int c) // c stands for cost and should be in 0 ~ 7
{
	Card* card = construct Card(spellCard(c, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(damageEff(c + 1, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(oppoAllegiance(), isCharacter(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Demo Card X Spell Cost " + IntToStr(c);
	return card;
}

Card* CreateDemoCardXLeader(int c) // c stands for cost and should be in 8 ~ 10
{
	Card* card = construct Card(leaderCard(c, 3 * c - 23, 10 * c - 65, singleAttack(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Demo Card X Leader Cost";
	return card;
}

Card* CreateElvenArcher()
{
	Card* card = construct Card(minionCard(1, 1, 1, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedBattlecryEff(noCondTargetedEff(damageEff(1, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Elven Archer";
	return card;
}

Card* CreateGoldshireFootman()
{
	Card* card = construct Card(minionCard(1, 1, 2, singleAttack(), dragonMinion(), justAbilities(noCharge(), justTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Goldshire Footman";
	return card;
}

Card* CreateMurlocRaider()
{
	Card* card = construct Card(minionCard(1, 2, 1, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Murloc Raider";
	return card;
}

Card* CreateStonetuskBoar()
{
	Card* card = construct Card(minionCard(1, 1, 1, singleAttack(), beastMinion(), justAbilities(justCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Stonetusk Boar";
	return card;
}

Card* CreateVoodooDoctor()
{
	Card* card = construct Card(minionCard(1, 2, 1, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedBattlecryEff(noCondTargetedEff(healEff(2), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Voodoo Doctor";
	return card;
}

Card* CreateBloodfenRapter()
{
	Card* card = construct Card(minionCard(2, 3, 2, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Bloodfen Rapter";
	return card;
}

Card* CreateBluegillWarrior()
{
	Card* card = construct Card(minionCard(2, 2, 1, singleAttack(), demonMinion(), justAbilities(justCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Bluegill Warrior";
	return card;
}

Card* CreateFrostwolfGrunt()
{
	Card* card = construct Card(minionCard(2, 2, 2, singleAttack(), dragonMinion(), justAbilities(noCharge(), justTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Frostwolf Grunt";
	return card;
}

Card* CreateMurlocTidehunter()
{
	Card* token = construct Card(minionCard(1, 1, 1, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	token->name = "Murloc Scout";
	Card* card = construct Card(minionCard(2, 2, 1, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedBattlecryEff(noCondUntargetedEff(newEff(1, fieldDest(allyAllegiance()), plainMinion(token)))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Murloc Tidehunter";
	return card;
}

Card* CreateNoviceEngineer()
{
	Card* card = construct Card(minionCard(2, 1, 1, singleAttack(), dragonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedBattlecryEff(noCondUntargetedEff(drawCardEff(1, allyAllegiance()))),
								noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Novice Engineer";
	return card;
}

Card* CreateRiverCrocolisk()
{
	Card* card = construct Card(minionCard(2, 2, 3, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "River Crocolisk";
	return card;
}

Card* CreateIronforgeRifleman()
{
	Card* card = construct Card(minionCard(3, 2, 2, singleAttack(), dragonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedBattlecryEff(noCondTargetedEff(damageEff(1, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Ironforge Rifleman";
	return card;
}

Card* CreateIronfurGrizzly()
{
	Card* card = construct Card(minionCard(3, 3, 3, singleAttack(), beastMinion(), justAbilities(noCharge(), justTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Ironfur Grizzly";
	return card;
}

Card* CreateMagmaRager()
{
	Card* card = construct Card(minionCard(3, 5, 1, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Magma Rager";
	return card;
}

Card* CreateRazorfenHunter()
{
	Card* token = construct Card(minionCard(1, 1, 1, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	token->name = "Boar";
	Card* card = construct Card(minionCard(3, 2, 3, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedBattlecryEff(noCondUntargetedEff(newEff(1, fieldDest(allyAllegiance()), plainMinion(token)))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Razorfen Hunter";
	return card;
}

Card* CreateShatteredSunCleric()
{
	Card* card = construct Card(minionCard(3, 3, 2, singleAttack(), dragonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedBattlecryEff(noCondTargetedEff(atkHpModEff(1, 1), charTargetCond(justCharTargetCond(allyAllegiance(), isMinion(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Shattered Sun Cleric";
	return card;
}

Card* CreateSilverbackPatriarch()
{
	Card* card = construct Card(minionCard(3, 1, 4, singleAttack(), beastMinion(), justAbilities(noCharge(), justTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Silverback Patriarch";
	return card;
}

Card* CreateWolfrider()
{
	Card* card = construct Card(minionCard(3, 3, 1, singleAttack(), beastMinion(), justAbilities(justCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Wolfrider";
	return card;
}

Card* CreateChillwindYeti()
{
	Card* card = construct Card(minionCard(4, 4, 5, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Chillwind Yeti";
	return card;
}

Card* CreateDragonlingMechanic()
{
	Card* token = construct Card(minionCard(1, 2, 1, singleAttack(), dragonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	token->name = "Mechanical Dragonling";
	Card* card = construct Card(minionCard(4, 2, 4, singleAttack(), dragonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedBattlecryEff(noCondUntargetedEff(newEff(1, fieldDest(allyAllegiance()), plainMinion(token)))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Dragonling Mechanic";
	return card;
}

Card* CreateGnomishInventor()
{
	Card* card = construct Card(minionCard(4, 2, 4, singleAttack(), dragonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedBattlecryEff(noCondUntargetedEff(drawCardEff(1, allyAllegiance()))),
								noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Gnomish Inventor";
	return card;
}

Card* CreateOasisSnapjaw()
{
	Card* card = construct Card(minionCard(4, 2, 7, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Oasis Snapjaw";
	return card;
}

Card* CreateSenjinShieldmasta()
{
	Card* card = construct Card(minionCard(4, 3, 5, singleAttack(), demonMinion(), justAbilities(noCharge(), justTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Senjin Shieldmasta";
	return card;
}

Card* CreateStormwindKnight()
{
	Card* card = construct Card(minionCard(4, 2, 5, singleAttack(), beastMinion(), justAbilities(justCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Stormwind Knight";
	return card;
}

Card* CreateBootyBayBodyguard()
{
	Card* card = construct Card(minionCard(5, 5, 4, singleAttack(), demonMinion(), justAbilities(noCharge(), justTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Booty Bay Bodyguard";
	return card;
}

Card* CreateDarkscaleHealer()
{
	Card* card = construct Card(minionCard(5, 4, 5, singleAttack(), dragonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedBattlecryEff(noCondUntargetedEff(aoeEff(healEff(2), charTargetCond(justCharTargetCond(allyAllegiance(), isCharacter(), noAbilCond(), noStatCond()))))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Darkscale Healer";
	return card;
}

Card* CreateNightblade()
{
	Card* card = construct Card(minionCard(5, 4, 4, singleAttack(), dragonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedBattlecryEff(noCondUntargetedEff(leaderEff(damageEff(3, damageAbilities(noPoisonous(), noLifesteal())), oppoAllegiance()))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Nightblade";
	return card;
}

Card* CreateStormpikeCommando()
{
	Card* card = construct Card(minionCard(5, 4, 2, singleAttack(), dragonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedBattlecryEff(noCondTargetedEff(damageEff(2, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Stormpike Commando";
	return card;
}

Card* CreateBoulderfistOgre()
{
	Card* card = construct Card(minionCard(6, 6, 7, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Boulderfist Orge";
	return card;
}

Card* CreateLordOfTheArena()
{
	Card* card = construct Card(minionCard(6, 6, 5, singleAttack(), dragonMinion(), justAbilities(noCharge(), justTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Lord of the Arena";
	return card;
}

Card* CreateRecklessRocketeer()
{
	Card* card = construct Card(minionCard(6, 5, 2, singleAttack(), demonMinion(), justAbilities(justCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Reckless Rocketeer";
	return card;
}

Card* CreateCoreHound()
{
	Card* card = construct Card(minionCard(7, 9, 5, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Core Hound";
	return card;
}

Card* CreateWarGolem()
{
	Card* card = construct Card(minionCard(7, 7, 7, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "War Golem";
	return card;
}

Card* CreateChaosNova()
{
	Card* card = construct Card(spellCard(5, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(aoeEff(damageEff(4, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(anyAllegiance(), isMinion(), noAbilCond(), noStatCond()))))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Chaos Nova";
	return card;
}

Card* CreateMoonfire()
{
	Card* card = construct Card(spellCard(0, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(damageEff(1, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Moonfire";
	return card;
}

Card* CreateHealingTouch()
{
	Card* card = construct Card(spellCard(3, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(healEff(8), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Healing Touch";
	return card;
}

Card* CreateStarfire()
{
	Card* card = construct Card(spellCard(6, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(damageEff(5, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond())))),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(drawCardEff(1, allyAllegiance()))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Starfire";
	return card;
}

Card* CreateArcaneShot()
{
	Card* card = construct Card(spellCard(1, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(damageEff(2, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Arcane Shot";
	return card;
}

Card* CreateMirrorImage()
{
	Card* token = construct Card(minionCard(0, 0, 2, singleAttack(), dragonMinion(), justAbilities(noCharge(), justTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	token->name = "Mirror Image (Token)";
	Card* card = construct Card(spellCard(1, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(newEff(2, fieldDest(allyAllegiance()), plainMinion(token)))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Mirror Image";
	return card;
}

Card* CreateArcaneExplosion()
{
	Card* card = construct Card(spellCard(2, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(aoeEff(damageEff(1, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(oppoAllegiance(), isMinion(), noAbilCond(), noStatCond()))))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Arcane Explosion";
	return card;
}

Card* CreateArcaneIntellect()
{
	Card* card = construct Card(spellCard(3, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(drawCardEff(2, allyAllegiance()))),
								noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Arcane Intellect";
	return card;
}

Card* CreateFireball()
{
	Card* card = construct Card(spellCard(4, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(damageEff(6, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Fireball";
	return card;
}

Card* CreatePolymorph()
{
	Card* token = construct Card(minionCard(1, 1, 1, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	token->name = "Sheep";
	Card* card = construct Card(spellCard(4, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(transformEff(plainMinion(token)), charTargetCond(justCharTargetCond(anyAllegiance(), isMinion(), noAbilCond(), noStatCond())))),
							noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Polymorph";
	return card;
}

Card* CreateFlamestrike()
{
	Card* card = construct Card(spellCard(7, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(aoeEff(damageEff(5, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(oppoAllegiance(), isMinion(), noAbilCond(), noStatCond()))))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Flame Strike";
	return card;
}

Card* CreateBlessingOfMight()
{
	Card* card = construct Card(spellCard(1, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(atkHpModEff(3, 0), charTargetCond(justCharTargetCond(anyAllegiance(), isMinion(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Blessing of Might";
	return card;
}

Card* CreateHandOfProtection()
{
	Card* card = construct Card(spellCard(1, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(giveShieldEff(), charTargetCond(justCharTargetCond(anyAllegiance(), isMinion(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Hand of Protection";
	return card;	
}

Card* CreateHolyLight()
{
	Card* card = construct Card(spellCard(2, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(leaderEff(healEff(8), allyAllegiance()))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Holy Light";
	return card;
}

Card* CreateBlessingOfKings()
{
	Card* card = construct Card(spellCard(4, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(atkHpModEff(4, 4), charTargetCond(justCharTargetCond(anyAllegiance(), isMinion(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Blessing of Kings";
	return card;
}

Card* CreateConsecration()
{
	Card* card = construct Card(spellCard(4, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(aoeEff(damageEff(2, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(oppoAllegiance(), isCharacter(), noAbilCond(), noStatCond()))))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Consecration";
	return card;
}

Card* CreateHammerOfWrath()
{
	Card* card = construct Card(spellCard(4, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(damageEff(3, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond())))),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(drawCardEff(1, allyAllegiance()))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Hammer of Wrath";
	return card;
}

Card* CreateGuardianOfKings()
{
	Card* card = construct Card(minionCard(7, 5, 7, singleAttack(), dragonMinion(), justAbilities(noCharge(), justTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedBattlecryEff(noCondUntargetedEff(leaderEff(healEff(6), allyAllegiance()))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Guardian of Kings";
	return card;
}

Card* CreatePowerWordShield()
{
	Card* card = construct Card(spellCard(1, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(atkHpModEff(0, 2), charTargetCond(justCharTargetCond(anyAllegiance(), isMinion(), noAbilCond(), noStatCond())))),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(drawCardEff(1, allyAllegiance()))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Power Word - Shield";
	return card;
}

Card* CreateHolySmite()
{
	Card* card = construct Card(spellCard(1, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(damageEff(3, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(anyAllegiance(), isMinion(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Holy Smite";
	return card;
}

Card* CreateMindVision()
{
	Card* card = construct Card(spellCard(1, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(randEff(copyEff(handDest(allyAllegiance())), cardTargetCond(justCardTargetCond(cardPosAtHand(), oppoAllegiance(), isCard(), noAbilCond(), noStatCond()))))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Mind Vision";
	return card;
}

Card* CreatePsychicConjurer()
{
	Card* card = construct Card(minionCard(1, 1, 1, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedBattlecryEff(noCondUntargetedEff(randEff(copyEff(handDest(allyAllegiance())), cardTargetCond(justCardTargetCond(cardPosAtDeck(), oppoAllegiance(), isCard(), noAbilCond(), noStatCond()))))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Psychic Conjurer";
	return card;
}

Card* CreateRadiance()
{
	Card* card = construct Card(spellCard(1, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(leaderEff(healEff(5), allyAllegiance()))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Radiance";
	return card;
}

Card* CreateShadowWordDeath()
{
	Card* card = construct Card(spellCard(2, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(destroyEff(), charTargetCond(justCharTargetCond(anyAllegiance(), isMinion(), noAbilCond(), atkCond(statGe(5)))))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Shadow Word - Death";
	return card;
}

Card* CreateShadowWordPain()
{
	Card* card = construct Card(spellCard(2, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(destroyEff(), charTargetCond(justCharTargetCond(anyAllegiance(), isMinion(), noAbilCond(), atkCond(statLe(3)))))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Shadow Word - Pain";
	return card;
}

Card* CreateHolyNova()
{
	Card* card = construct Card(spellCard(4, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(aoeEff(damageEff(2, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(oppoAllegiance(), isMinion(), noAbilCond(), noStatCond()))))),
							consOtherEffs(untargetedCastEff(noCondUntargetedEff(aoeEff(healEff(2), charTargetCond(justCharTargetCond(allyAllegiance(), isCharacter(), noAbilCond(), noStatCond()))))),
								noOtherEffs()))))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Holy Nova";
	return card;
}

Card* CreatePowerInfusion()
{
	Card* card = construct Card(spellCard(4, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(atkHpModEff(2, 6), charTargetCond(justCharTargetCond(anyAllegiance(), isMinion(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Power Infusion";
	return card;
}

Card* CreateSinisterStrike()
{
	Card* card = construct Card(spellCard(1, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(leaderEff(damageEff(3, damageAbilities(noPoisonous(), noLifesteal())), oppoAllegiance()))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Sinister Strike";
	return card;
}

Card* CreateSap()
{
	Card* card = construct Card(spellCard(2, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(moveEff(handDest(oppoAllegiance())), charTargetCond(justCharTargetCond(oppoAllegiance(), isMinion(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Sap";
	return card;
}

Card* CreateShiv()
{
	Card* card = construct Card(spellCard(2, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(damageEff(1, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond())))),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(drawCardEff(1, allyAllegiance()))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Shiv";
	return card;
}

Card* CreateFanOfKnives()
{
	Card* card = construct Card(spellCard(3, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(aoeEff(damageEff(1, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(oppoAllegiance(), isMinion(), noAbilCond(), noStatCond()))))),
							consOtherEffs(untargetedCastEff(noCondUntargetedEff(drawCardEff(1, allyAllegiance()))),
								noOtherEffs()))))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Fan of Knives";
	return card;
}

Card* CreatePlaguebringer()
{
	Card* card = construct Card(minionCard(4, 3, 3, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedBattlecryEff(noCondTargetedEff(givePoisonousEff(), charTargetCond(justCharTargetCond(allyAllegiance(), isMinion(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Plaguebringer";
	return card;
}

Card* CreateAssassinate()
{
	Card* card = construct Card(spellCard(4, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(destroyEff(), charTargetCond(justCharTargetCond(oppoAllegiance(), isMinion(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Assassinate";
	return card;
}

Card* CreateHex()
{
	Card* token = construct Card(minionCard(0, 0, 1, singleAttack(), demonMinion(), justAbilities(noCharge(), justTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	token->name = "Frog";
	Card* card = construct Card(spellCard(4, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(transformEff(plainMinion(token)), charTargetCond(justCharTargetCond(anyAllegiance(), isMinion(), noAbilCond(), noStatCond())))),
							noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Hex";
	return card;
}

Card* CreateFireElemental()
{
	Card* card = construct Card(minionCard(6, 6, 5, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedBattlecryEff(noCondTargetedEff(damageEff(4, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Fire Elemental";
	return card;
}

Card* CreateSoulfire()
{
	Card* card = construct Card(spellCard(1, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(damageEff(4, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond())))),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(randEff(discardEff(), cardTargetCond(justCardTargetCond(cardPosAtHand(), allyAllegiance(), isCard(), noAbilCond(), noStatCond()))))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Soulfire";
	return card;
}

Card* CreateFelstalker()
{
	Card* card = construct Card(minionCard(2, 4, 3, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedBattlecryEff(noCondUntargetedEff(randEff(discardEff(), cardTargetCond(justCardTargetCond(cardPosAtHand(), allyAllegiance(), isCard(), noAbilCond(), noStatCond()))))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Felstalker";
	return card;
}

Card* CreateDrainLife()
{
	Card* card = construct Card(spellCard(3, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(damageEff(2, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond())))),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(leaderEff(healEff(2), allyAllegiance()))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Drain Life";
	return card;
}

Card* CreateShadowBolt()
{
	Card* card = construct Card(spellCard(3, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(damageEff(4, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(anyAllegiance(), isMinion(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Shadow Bolt";
	return card;
}

Card* CreateHellfire()
{
	Card* card = construct Card(spellCard(4, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(aoeEff(damageEff(3, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond()))))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Hellfire";
	return card;
}

Card* CreateWhirlwind()
{
	Card* card = construct Card(spellCard(1, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedCastEff(noCondUntargetedEff(aoeEff(damageEff(1, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(anyAllegiance(), isMinion(), noAbilCond(), noStatCond()))))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Whirlwind";
	return card;
}

Card* CreateKorkronElite()
{
	Card* card = construct Card(minionCard(4, 4, 3, singleAttack(), beastMinion(), justAbilities(justCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Korkron Elite";
	return card;
}

Card* CreateWisp()
{
	Card* card = construct Card(minionCard(0, 1, 1, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Wisp";
	return card;
}

Card* CreateLeperGnome()
{
	Card* card = construct Card(minionCard(1, 2, 1, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(deathrattleEff(noCondUntargetedEff(leaderEff(damageEff(2, damageAbilities(noPoisonous(), noLifesteal())), oppoAllegiance()))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Leper Gnome";
	return card;
}

Card* CreateWorgenInfiltrator()
{
	Card* card = construct Card(minionCard(1, 2, 1, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), justStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Worgen Infiltrator";
	return card;
}

Card* CreateFaerieDragon()
{
	Card* card = construct Card(minionCard(2, 3, 2, singleAttack(), dragonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), justUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Wisp";
	return card;
}

Card* CreateLootHoarder()
{
	Card* card = construct Card(minionCard(2, 2, 1, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(deathrattleEff(noCondUntargetedEff(drawCardEff(1, allyAllegiance()))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Loot Hoarder";
	return card;
}

Card* CreateEarthenRingFarseer()
{
	Card* card = construct Card(minionCard(3, 3, 3, singleAttack(), dragonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedBattlecryEff(noCondTargetedEff(healEff(3), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Earthen Ring Farseer";
	return card;
}

Card* CreateHarvestGolem()
{
	Card* token = construct Card(minionCard(1, 2, 1, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	token->name = "Damaged Golem";
	Card* card = construct Card(minionCard(3, 2, 3, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(deathrattleEff(noCondUntargetedEff(newEff(1, fieldDest(allyAllegiance()), plainMinion(token)))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Harvest Golem";
	return card;
}

Card* CreateJunglePanther()
{
	Card* card = construct Card(minionCard(3, 4, 2, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), justStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Jungle Panther";
	return card;
}

Card* CreateScarletCrusader()
{
	Card* card = construct Card(minionCard(3, 3, 1, singleAttack(), dragonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), justShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Scarlet Crusader";
	return card;
}

Card* CreateThrallmarFarseer()
{
	Card* card = construct Card(minionCard(3, 2, 3, multipleAttack(2), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Thrallmar Farseer";
	return card;
}

Card* CreateStranglethornTiger()
{
	Card* card = construct Card(minionCard(5, 5, 5, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), justStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Stranglethorn Tiger";
	return card;
}

Card* CreateWindfuryHarpy()
{
	Card* card = construct Card(minionCard(6, 4, 5, multipleAttack(2), dragonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Windfury Harpy";
	return card;
}

Card* CreateEmperorCobra()
{
	Card* card = construct Card(minionCard(3, 2, 3, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), justPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Emperor Cobra";
	return card;
}

Card* CreateImpMaster()
{
	Card* token = construct Card(minionCard(1, 1, 1, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	token->name = "Imp";
	Card* card = construct Card(minionCard(3, 1, 5, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(turnEndEff(srcCondUntargetedEff(selfEff(damageEff(1, damageAbilities(noPoisonous(), noLifesteal()))), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond()))), allyAllegiance()),
							consOtherEffs(turnEndEff(srcCondUntargetedEff(newEff(1, fieldDest(allyAllegiance()), plainMinion(token)), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond()))), allyAllegiance()),
								noOtherEffs()))))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Imp Master";
	return card;
}

Card* CreateInjuredBlademaster()
{
	Card* card = construct Card(minionCard(3, 4, 7, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedBattlecryEff(noCondUntargetedEff(selfEff(damageEff(4, damageAbilities(noPoisonous(), noLifesteal()))))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Injured Blademaster";
	return card;
}

Card* CreateAbomination()
{
	Card* card = construct Card(minionCard(5, 4, 4, singleAttack(), demonMinion(), justAbilities(noCharge(), justTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(deathrattleEff(noCondUntargetedEff(aoeEff(damageEff(2, damageAbilities(noPoisonous(), noLifesteal())), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond()))))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Abomination";
	return card;
}

Card* CreateStampedingKodo()
{
	Card* card = construct Card(minionCard(5, 3, 5, singleAttack(), demonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedBattlecryEff(noCondUntargetedEff(randEff(destroyEff(), charTargetCond(justCharTargetCond(oppoAllegiance(), isMinion(), noAbilCond(), atkCond(statLe(2))))))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Stampeding Kodo";
	return card;
}

Card* CreateSunwalker()
{
	Card* card = construct Card(minionCard(6, 4, 5, singleAttack(), dragonMinion(), justAbilities(noCharge(), justTaunt(), noStealth(), noUntargetable(), justShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Sunwalker";
	return card;
}

Card* CreateRavenholdtAssassin()
{
	Card* card = construct Card(minionCard(7, 7, 5, singleAttack(), dragonMinion(), justAbilities(noCharge(), noTaunt(), justStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Ravenholdt Assassin";
	return card;
}

Card* CreateBigGameHunter()
{
	Card* card = construct Card(minionCard(4, 4, 2, singleAttack(), dragonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedBattlecryEff(noCondTargetedEff(destroyEff(), charTargetCond(justCharTargetCond(anyAllegiance(), isMinion(), noAbilCond(), atkCond(statGe(7)))))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Big Game Hunter";
	return card;
}

Card* CreateKingMukla()
{
	Card* token = construct Card(spellCard(1, justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedCastEff(noCondTargetedEff(atkHpModEff(1, 1), charTargetCond(justCharTargetCond(anyAllegiance(), isMinion(), noAbilCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	token->name = "Bananas";
	Card* card = construct Card(minionCard(3, 5, 5, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(untargetedBattlecryEff(noCondUntargetedEff(newEff(2, handDest(oppoAllegiance()), fixedCard(token)))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "King Mukla";
	return card;
}

Card* CreateCairneBloodhoof()
{
	Card* token = construct Card(minionCard(5, 5, 5, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	token->name = "Baine Bloodhoof";
	Card* card = construct Card(minionCard(6, 5, 5, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(deathrattleEff(noCondUntargetedEff(newEff(1, fieldDest(allyAllegiance()), plainMinion(token)))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "CairneBloodhoof";
	return card;
}

Card* CreateHogger()
{
	Card* token = construct Card(minionCard(2, 2, 2, singleAttack(), beastMinion(), justAbilities(noCharge(), justTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	token->name = "Gnoll";
	Card* card = construct Card(minionCard(6, 4, 4, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(turnEndEff(srcCondUntargetedEff(newEff(1, fieldDest(allyAllegiance()), plainMinion(token)), charTargetCond(justCharTargetCond(anyAllegiance(), isCharacter(), noAbilCond(), noStatCond()))), allyAllegiance()),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "Hogger";
	return card;
}

Card* CreateTheBeast()
{
	Card* token = construct Card(minionCard(3, 3, 3, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()), specialEffects(noTargetedPlayEff(), noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	token->name = "Finkle Einhorn";
	Card* card = construct Card(minionCard(6, 9, 7, singleAttack(), beastMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(noTargetedPlayEff(),
						consOtherEffs(deathrattleEff(noCondUntargetedEff(newEff(1, fieldDest(oppoAllegiance()), plainMinion(token)))),
							noOtherEffs())))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "The Beast";
	return card;
}

Card* CreateTheBlackKnight()
{
	Card* card = construct Card(minionCard(6, 4, 5, singleAttack(), dragonMinion(), justAbilities(noCharge(), noTaunt(), noStealth(), noUntargetable(), noShield(), noPoisonous(), noLifesteal()),
					specialEffects(targetedBattlecryEff(noCondTargetedEff(destroyEff(), charTargetCond(justCharTargetCond(oppoAllegiance(), isMinion(), tauntCond(), noStatCond())))),
						noOtherEffs()))) with GetDefaultGenConfig(-1, nullptr);
	card->name = "The Black Knight";
	return card;
}

