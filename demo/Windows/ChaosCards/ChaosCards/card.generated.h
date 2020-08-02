
#include "GiglDefaultInclude.h"


#include "Player.h"


#include <iostream>


class Player;

class CondConfig;

class Card;







GiglConfig GetDefaultGenConfig(signed int  seed);

Card  *CreatePlainMinion(string  parent_name);

Card  *CreateRandomMinion(string  parent_name, signed int  cost, signed int  min_eff_num, signed int  max_eff_num, signed int  eff_depth);

Card  *CreateRandomCard(string  parent_name, signed int  cost, signed int  min_eff_num, signed int  max_eff_num, signed int  eff_depth);
Card  *Card__ItemRef__532__54;
Card  *Card__ItemRef__544__54;
Card  *Card__ItemRef__557__54;


class Card;

class Card__Node;

class Card__CardRoot;

class Card__AttackTimes;

class Card__MinionType;

class Card__Attributes;

class Card__DamageAttributes;

class Card__ChargeAttr;

class Card__TauntAttr;

class Card__StealthAttr;

class Card__UntargetableAttr;

class Card__ShieldAttr;

class Card__PoisonousAttr;

class Card__LifestealAttr;

class Card__SpecialEffects;

class Card__TargetedPlayEff;

class Card__OtherEffs;

class Card__OtherEff;

class Card__TargetedEff;

class Card__UntargetedEff;

class Card__TargetCond;

class Card__CharTargetCond;

class Card__CharTypeCond;

class Card__CardTargetCond;

class Card__CardTypeCond;

class Card__CardPosCond;

class Card__AllegianceCond;

class Card__AttrCond;

class Card__StatCond;

class Card__StatCondVariant;

class Card__IndeCond;

class Card__BaseTargetedEff;

class Card__BaseUntargetedEff;

class Card__Destination;

class Card__NewCardVariant;

class Card__leaderCard;

class Card__minionCard;

class Card__spellCard;

class Card__zeroAttack;

class Card__singleAttack;

class Card__multipleAttack;

class Card__beastMinion;

class Card__dragonMinion;

class Card__demonMinion;

class Card__justAttributes;

class Card__damageAttributes;

class Card__noCharge;

class Card__justCharge;

class Card__noTaunt;

class Card__justTaunt;

class Card__noStealth;

class Card__justStealth;

class Card__noUntargetable;

class Card__justUntargetable;

class Card__noShield;

class Card__justShield;

class Card__noPoisonous;

class Card__justPoisonous;

class Card__noLifesteal;

class Card__justLifesteal;

class Card__specialEffects;

class Card__noTargetedPlayEff;

class Card__targetedBattlecryEff;

class Card__targetedCastEff;

class Card__noOtherEffs;

class Card__consOtherEffs;

class Card__untargetedBattlecryEff;

class Card__untargetedCastEff;

class Card__deathrattleEff;

class Card__onDiscardEff;

class Card__turnStartEff;

class Card__turnEndEff;

class Card__noCondTargetedEff;

class Card__indeCondTargetedEff;

class Card__srcCondTargetedEff;

class Card__noCondUntargetedEff;

class Card__indeCondUntargetedEff;

class Card__srcCondUntargetedEff;

class Card__charTargetCond;

class Card__cardTargetCond;

class Card__justCharTargetCond;

class Card__isCharacter;

class Card__isMinion;

class Card__isBeast;

class Card__isDragon;

class Card__isDemon;

class Card__justCardTargetCond;

class Card__isCard;

class Card__isLeaderCard;

class Card__isMinionCard;

class Card__isSpellCard;

class Card__isBeastCard;

class Card__isDragonCard;

class Card__isDemonCard;

class Card__cardPosAtHandOrDeck;

class Card__cardPosAtHand;

class Card__cardPosAtDeck;

class Card__anyAllegiance;

class Card__allyAllegiance;

class Card__oppoAllegiance;

class Card__noAttrCond;

class Card__chargeCond;

class Card__tauntCond;

class Card__stealthCond;

class Card__untargetableCond;

class Card__shieldCond;

class Card__poisonousCond;

class Card__lifestealCond;

class Card__noStatCond;

class Card__costCond;

class Card__atkCond;

class Card__hpCond;

class Card__atkTimesCond;

class Card__statGe;

class Card__statLe;

class Card__fieldExistenceCond;

class Card__cardExistenceCond;

class Card__leaderCond;

class Card__mpCond;

class Card__maxMpCond;

class Card__damageEff;

class Card__healEff;

class Card__resAtkTimesEff;

class Card__costModEff;

class Card__atkHpModEff;

class Card__atkTimesModEff;

class Card__destroyEff;

class Card__discardEff;

class Card__moveEff;

class Card__copyEff;

class Card__transformEff;

class Card__changeToBeastEff;

class Card__changeToDragonEff;

class Card__changeToDemonEff;

class Card__giveChargeEff;

class Card__giveTauntEff;

class Card__giveStealthEff;

class Card__giveUntargetableEff;

class Card__giveShieldEff;

class Card__givePoisonousEff;

class Card__giveLifestealEff;

class Card__rmAttributesEff;

class Card__setOverheatEff;

class Card__decOhThresholdEff;

class Card__resetStateEff;

class Card__giveEffectsEff;

class Card__aoeEff;

class Card__randEff;

class Card__leaderEff;

class Card__selfEff;

class Card__drawCardEff;

class Card__mpEff;

class Card__maxMpEff;

class Card__newEff;

class Card__winGameEff;

class Card__fieldDest;

class Card__ownerFieldDest;

class Card__handDest;

class Card__ownerHandDest;

class Card__deckDest;

class Card__ownerDeckDest;

class Card__plainMinionCard;

class Card__plainMinion;

class Card__fixedCard;

class Card__fixedMinion;

class Card__randomCard;

class Card__randomMinion;

class Card {
public:
  Card();
  Card(const GiglConfig  & input__config);
  Card(GiglConfig  && input__config);
  ~Card();
  Card  *Item__Generator();
  Card  *Item__Constructor(Card__Node  * input__root);
  GiglConfig config;
  signed int seed;
  signed int max_eff_num;
  signed int max_eff_depth;
  Card__Node  *root;
  void RegisterContribution(signed int  * counter);
  void IncContribution();
  void SetAffiliation(Player  * _owner);
  signed int GetTargetIndex();
  string BriefInfo();
  string DetailInfo();
  signed int IsSleeping();
  signed int isTargetedAtPlay(signed int  x, signed int  y);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z);
  void Play(signed int  x, signed int  y, signed int  z);
  void Destroy();
  void Discard();
  signed int CheckAttackValid(signed int  x, signed int  z);
  void Attack(signed int  x, signed int  z);
  void TurnStart(Card  * leader);
  void TurnEnd(Card  * leader);
  void SetAllOverheatCounts(signed int  val);
  void SetAllOverheatThresholds(signed int  val);
  void ModAllOverheatThresholds(signed int  amount);
  void TakeDamage(signed int  amount, Card  * src, signed int  start_of_batch);
  void RestoreHp(signed int  amount);
  void RestoreAtkTimes(signed int  amount);
  void ModifyAtkHp(signed int  atkmod, signed int  hpmod);
  void ModifyCost(signed int  amount);
  void ModifyAtkTimes(signed int  amount);
  signed int  *contribution;
  string name;
  signed int is_dying;
  signed int is_resetting;
  Card  *replacement;
  signed int is_first_turn_at_field;
  Player  *owner;
  Player  *opponent;
  signed int card_pos;
  signed int mana;
  signed int orig_mana;
  signed int atk;
  signed int orig_atk;
  signed int max_hp;
  signed int orig_hp;
  signed int hp_loss;
  signed int max_n_atks;
  signed int orig_n_atks;
  signed int n_atks_loss;
  signed int card_type;
  signed int minion_type;
  signed int orig_minion_type;
  signed int is_charge;
  signed int orig_is_charge;
  signed int is_taunt;
  signed int orig_is_taunt;
  signed int is_stealth;
  signed int orig_is_stealth;
  signed int is_untargetable;
  signed int orig_is_untargetable;
  signed int is_shielded;
  signed int orig_is_shielded;
  signed int is_poisonous;
  signed int orig_is_poisonous;
  signed int is_lifesteal;
  signed int orig_is_lifesteal;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  string DetailAlt1();
  string DetailAlt2();
  string DetailAlt3();
  string DetailAlt4();
  string DetailAlt5();
  string DetailAlt6();
  string Suffix();
  string SuffixAlt1();
  string Postfix();
  string PostfixIndent(signed int  indent_size);
  signed int IsPlural();
  signed int IsThirdPersonSingle();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  signed int GetEffectNum();
  Card__SpecialEffects  *GetEffects();
  Card  *GetRelatedCard(Card  * parent_card);
  signed int isCondTrivial();
  unsigned int GetInitAttrFlag();
  CondConfig GetPlayTargetConfig();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  CondConfig GetSelfConfig(const CondConfig  & self_config);
  CondConfig GetLeaderConfig();
  CondConfig GetTargetConfig();
  void AdjustGlobalStatRange(signed int  & min_val, signed int  & max_val);
  signed int isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  void Play(signed int  x, signed int  y, signed int  z, Card  * parent_card);
  void Destroy(Card  * parent_card);
  void Discard(Card  * parent_card);
  signed int CheckAttackValid(signed int  x, signed int  z, Card  * parent_card);
  void Attack(signed int  x, signed int  z, Card  * parent_card);
  void TurnStart(Card  * leader, Card  * parent_card);
  void TurnEnd(Card  * leader, Card  * parent_card);
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  void SubTargetedAction(Card  * target, Card  * parent_card, signed int  start_of_batch);
  void UntargetedAction(Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  signed int CheckStatValid(signed int  stat_val);
  void Mutate(signed int  min_eff_n, signed int  max_eff_n, signed int  effect_depth);
  void SetOverheatCounts(signed int  val);
  void SetOverheatThresholds(signed int  val);
  void ModOverheatThresholds(signed int  amount);
  signed int num_refs;
  signed int overheat_count;
  signed int overheat_threshold;
  vector<Card__SpecialEffects *> effects_extra;
  void AddExtraEffects(Card__SpecialEffects  * effects);
  void ClearExtraEffects();
  string GetExtraEffectsBrief();
  string GetExtraEffectsDetail();
  string GetExtraEffectsDetailIndent(signed int  indent_size);
  Card  *CreateHardCopy(PtrRedirMap  & redir_map);
  Card  *CreateCopy();
  Card__CardRoot  *Generate__CardRoot(CondConfig  & global_config, signed int  is_plain);
  Card__leaderCard  *Generate__leaderCard(CondConfig  & global_config, signed int  is_plain);
  Card__minionCard  *Generate__minionCard(CondConfig  & global_config, signed int  is_plain);
  Card__spellCard  *Generate__spellCard(CondConfig  & global_config, signed int  is_plain);
  Card__AttackTimes  *Generate__AttackTimes(void);
  Card__zeroAttack  *Generate__zeroAttack(void);
  Card__singleAttack  *Generate__singleAttack(void);
  Card__multipleAttack  *Generate__multipleAttack(void);
  Card__MinionType  *Generate__MinionType(void);
  Card__beastMinion  *Generate__beastMinion(void);
  Card__dragonMinion  *Generate__dragonMinion(void);
  Card__demonMinion  *Generate__demonMinion(void);
  Card__Attributes  *Generate__Attributes(CondConfig  & self_config, signed int  damage);
  Card__justAttributes  *Generate__justAttributes(CondConfig  & self_config, signed int  damage);
  Card__DamageAttributes  *Generate__DamageAttributes(CondConfig  & self_config, unsigned int  target_mode, signed int  damage);
  Card__damageAttributes  *Generate__damageAttributes(CondConfig  & self_config, unsigned int  target_mode, signed int  damage);
  Card__ChargeAttr  *Generate__ChargeAttr(CondConfig  & self_config);
  Card__noCharge  *Generate__noCharge(CondConfig  & self_config);
  Card__justCharge  *Generate__justCharge(CondConfig  & self_config);
  Card__TauntAttr  *Generate__TauntAttr(CondConfig  & self_config);
  Card__noTaunt  *Generate__noTaunt(CondConfig  & self_config);
  Card__justTaunt  *Generate__justTaunt(CondConfig  & self_config);
  Card__StealthAttr  *Generate__StealthAttr(CondConfig  & self_config);
  Card__noStealth  *Generate__noStealth(CondConfig  & self_config);
  Card__justStealth  *Generate__justStealth(CondConfig  & self_config);
  Card__UntargetableAttr  *Generate__UntargetableAttr(CondConfig  & self_config);
  Card__noUntargetable  *Generate__noUntargetable(CondConfig  & self_config);
  Card__justUntargetable  *Generate__justUntargetable(CondConfig  & self_config);
  Card__ShieldAttr  *Generate__ShieldAttr(CondConfig  & self_config);
  Card__noShield  *Generate__noShield(CondConfig  & self_config);
  Card__justShield  *Generate__justShield(CondConfig  & self_config);
  Card__PoisonousAttr  *Generate__PoisonousAttr(CondConfig  & self_config, signed int  damage);
  Card__noPoisonous  *Generate__noPoisonous(CondConfig  & self_config, signed int  damage);
  Card__justPoisonous  *Generate__justPoisonous(CondConfig  & self_config, signed int  damage);
  Card__LifestealAttr  *Generate__LifestealAttr(CondConfig  & self_config, signed int  damage);
  Card__noLifesteal  *Generate__noLifesteal(CondConfig  & self_config, signed int  damage);
  Card__justLifesteal  *Generate__justLifesteal(CondConfig  & self_config, signed int  damage);
  Card__SpecialEffects  *Generate__SpecialEffects(CondConfig  & self_config, signed int  min_n, signed int  max_n, signed int  effect_depth, signed int  give_eff);
  Card__specialEffects  *Generate__specialEffects(CondConfig  & self_config, signed int  min_n, signed int  max_n, signed int  effect_depth, signed int  give_eff);
  Card__TargetedPlayEff  *Generate__TargetedPlayEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff);
  Card__noTargetedPlayEff  *Generate__noTargetedPlayEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff);
  Card__targetedBattlecryEff  *Generate__targetedBattlecryEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff);
  Card__targetedCastEff  *Generate__targetedCastEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff);
  Card__OtherEffs  *Generate__OtherEffs(CondConfig  & self_config, signed int  min_n, signed int  max_n, signed int  effect_depth, signed int  give_eff);
  Card__noOtherEffs  *Generate__noOtherEffs(CondConfig  & self_config, signed int  min_n, signed int  max_n, signed int  effect_depth, signed int  give_eff);
  Card__consOtherEffs  *Generate__consOtherEffs(CondConfig  & self_config, signed int  min_n, signed int  max_n, signed int  effect_depth, signed int  give_eff);
  Card__OtherEff  *Generate__OtherEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff);
  Card__untargetedBattlecryEff  *Generate__untargetedBattlecryEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff);
  Card__untargetedCastEff  *Generate__untargetedCastEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff);
  Card__deathrattleEff  *Generate__deathrattleEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff);
  Card__onDiscardEff  *Generate__onDiscardEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff);
  Card__turnStartEff  *Generate__turnStartEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff);
  Card__turnEndEff  *Generate__turnEndEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff);
  Card__TargetedEff  *Generate__TargetedEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__noCondTargetedEff  *Generate__noCondTargetedEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__indeCondTargetedEff  *Generate__indeCondTargetedEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__srcCondTargetedEff  *Generate__srcCondTargetedEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__UntargetedEff  *Generate__UntargetedEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__noCondUntargetedEff  *Generate__noCondUntargetedEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__indeCondUntargetedEff  *Generate__indeCondUntargetedEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__srcCondUntargetedEff  *Generate__srcCondUntargetedEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__TargetCond  *Generate__TargetCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__charTargetCond  *Generate__charTargetCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__cardTargetCond  *Generate__cardTargetCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__CharTargetCond  *Generate__CharTargetCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__justCharTargetCond  *Generate__justCharTargetCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__CharTypeCond  *Generate__CharTypeCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__isCharacter  *Generate__isCharacter(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__isMinion  *Generate__isMinion(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__isBeast  *Generate__isBeast(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__isDragon  *Generate__isDragon(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__isDemon  *Generate__isDemon(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__CardTargetCond  *Generate__CardTargetCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__justCardTargetCond  *Generate__justCardTargetCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__CardTypeCond  *Generate__CardTypeCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__isCard  *Generate__isCard(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__isLeaderCard  *Generate__isLeaderCard(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__isMinionCard  *Generate__isMinionCard(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__isSpellCard  *Generate__isSpellCard(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__isBeastCard  *Generate__isBeastCard(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__isDragonCard  *Generate__isDragonCard(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__isDemonCard  *Generate__isDemonCard(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__CardPosCond  *Generate__CardPosCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__cardPosAtHandOrDeck  *Generate__cardPosAtHandOrDeck(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__cardPosAtHand  *Generate__cardPosAtHand(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__cardPosAtDeck  *Generate__cardPosAtDeck(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__AllegianceCond  *Generate__AllegianceCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__anyAllegiance  *Generate__anyAllegiance(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__allyAllegiance  *Generate__allyAllegiance(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__oppoAllegiance  *Generate__oppoAllegiance(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__AttrCond  *Generate__AttrCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__noAttrCond  *Generate__noAttrCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__chargeCond  *Generate__chargeCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__tauntCond  *Generate__tauntCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__stealthCond  *Generate__stealthCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__untargetableCond  *Generate__untargetableCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__shieldCond  *Generate__shieldCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__poisonousCond  *Generate__poisonousCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__lifestealCond  *Generate__lifestealCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
  Card__StatCond  *Generate__StatCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial);
  Card__noStatCond  *Generate__noStatCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial);
  Card__costCond  *Generate__costCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial);
  Card__atkCond  *Generate__atkCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial);
  Card__hpCond  *Generate__hpCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial);
  Card__atkTimesCond  *Generate__atkTimesCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial);
  Card__StatCondVariant  *Generate__StatCondVariant(signed int  lower_min, signed int  lower_max, signed int  upper_min, signed int  upper_max);
  Card__statGe  *Generate__statGe(signed int  lower_min, signed int  lower_max, signed int  upper_min, signed int  upper_max);
  Card__statLe  *Generate__statLe(signed int  lower_min, signed int  lower_max, signed int  upper_min, signed int  upper_max);
  Card__IndeCond  *Generate__IndeCond(CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing);
  Card__fieldExistenceCond  *Generate__fieldExistenceCond(CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing);
  Card__cardExistenceCond  *Generate__cardExistenceCond(CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing);
  Card__leaderCond  *Generate__leaderCond(CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing);
  Card__mpCond  *Generate__mpCond(CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing);
  Card__maxMpCond  *Generate__maxMpCond(CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing);
  Card__BaseTargetedEff  *Generate__BaseTargetedEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__damageEff  *Generate__damageEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__healEff  *Generate__healEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__resAtkTimesEff  *Generate__resAtkTimesEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__costModEff  *Generate__costModEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__atkHpModEff  *Generate__atkHpModEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__atkTimesModEff  *Generate__atkTimesModEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__destroyEff  *Generate__destroyEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__discardEff  *Generate__discardEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__moveEff  *Generate__moveEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__copyEff  *Generate__copyEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__transformEff  *Generate__transformEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__changeToBeastEff  *Generate__changeToBeastEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__changeToDragonEff  *Generate__changeToDragonEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__changeToDemonEff  *Generate__changeToDemonEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__giveChargeEff  *Generate__giveChargeEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__giveTauntEff  *Generate__giveTauntEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__giveStealthEff  *Generate__giveStealthEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__giveUntargetableEff  *Generate__giveUntargetableEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__giveShieldEff  *Generate__giveShieldEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__givePoisonousEff  *Generate__givePoisonousEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__giveLifestealEff  *Generate__giveLifestealEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__rmAttributesEff  *Generate__rmAttributesEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__setOverheatEff  *Generate__setOverheatEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__decOhThresholdEff  *Generate__decOhThresholdEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__resetStateEff  *Generate__resetStateEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__giveEffectsEff  *Generate__giveEffectsEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__BaseUntargetedEff  *Generate__BaseUntargetedEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__aoeEff  *Generate__aoeEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__randEff  *Generate__randEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__leaderEff  *Generate__leaderEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__selfEff  *Generate__selfEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__drawCardEff  *Generate__drawCardEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__mpEff  *Generate__mpEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__maxMpEff  *Generate__maxMpEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__newEff  *Generate__newEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__winGameEff  *Generate__winGameEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
  Card__Destination  *Generate__Destination(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode);
  Card__fieldDest  *Generate__fieldDest(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode);
  Card__ownerFieldDest  *Generate__ownerFieldDest(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode);
  Card__handDest  *Generate__handDest(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode);
  Card__ownerHandDest  *Generate__ownerHandDest(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode);
  Card__deckDest  *Generate__deckDest(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode);
  Card__ownerDeckDest  *Generate__ownerDeckDest(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode);
  Card__NewCardVariant  *Generate__NewCardVariant(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth);
  Card__plainMinionCard  *Generate__plainMinionCard(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth);
  Card__plainMinion  *Generate__plainMinion(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth);
  Card__fixedCard  *Generate__fixedCard(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth);
  Card__fixedMinion  *Generate__fixedMinion(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth);
  Card__randomCard  *Generate__randomCard(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth);
  Card__randomMinion  *Generate__randomMinion(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth);
  

};
class Card__Node {
public:
  Card__Node(Card  * input__item);
  Card__Node();
  virtual ~Card__Node();
  Card  *item;
  void virtual FillRep(CardRep  & rep) = 0;
  string virtual Brief();
  string virtual Detail();
  string virtual DetailIndent(signed int  indent_size);
  string virtual DetailAlt1();
  string virtual DetailAlt2();
  string virtual DetailAlt3();
  string virtual DetailAlt4();
  string virtual DetailAlt5();
  string virtual DetailAlt6();
  string virtual Suffix();
  string virtual SuffixAlt1();
  string virtual Postfix();
  string virtual PostfixIndent(signed int  indent_size);
  signed int virtual IsPlural();
  signed int virtual IsThirdPersonSingle();
  Card__Node virtual  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  signed int virtual GetEffectNum();
  Card__SpecialEffects virtual  *GetEffects();
  Card virtual  *GetRelatedCard(Card  * parent_card);
  signed int virtual isCondTrivial();
  unsigned int virtual GetInitAttrFlag();
  CondConfig virtual GetPlayTargetConfig();
  CondConfig virtual GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  CondConfig virtual GetSelfConfig(const CondConfig  & self_config);
  CondConfig virtual GetLeaderConfig();
  CondConfig virtual GetTargetConfig();
  void virtual AdjustGlobalStatRange(signed int  & min_val, signed int  & max_val);
  signed int virtual isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card);
  signed int virtual CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  void virtual Play(signed int  x, signed int  y, signed int  z, Card  * parent_card);
  void virtual Destroy(Card  * parent_card);
  void virtual Discard(Card  * parent_card);
  signed int virtual CheckAttackValid(signed int  x, signed int  z, Card  * parent_card);
  void virtual Attack(signed int  x, signed int  z, Card  * parent_card);
  void virtual TurnStart(Card  * leader, Card  * parent_card);
  void virtual TurnEnd(Card  * leader, Card  * parent_card);
  signed int virtual TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  void virtual SubTargetedAction(Card  * target, Card  * parent_card, signed int  start_of_batch);
  void virtual UntargetedAction(Card  * parent_card);
  signed int virtual CheckCardValid(Card  * card, Card  * parent_card);
  signed int virtual CheckThisValid(Card  * parent_card);
  signed int virtual CheckStatValid(signed int  stat_val);
  void virtual Mutate(signed int  min_eff_n, signed int  max_eff_n, signed int  effect_depth);
  void virtual SetOverheatCounts(signed int  val);
  void virtual SetOverheatThresholds(signed int  val);
  void virtual ModOverheatThresholds(signed int  amount);
  signed int num_refs;
  signed int overheat_count;
  signed int overheat_threshold;
  

};
class Card__CardRoot : public Card__Node {
public:
  Card__CardRoot(Card  * input__item);
  Card__CardRoot();
  

};
class Card__AttackTimes : public Card__Node {
public:
  Card__AttackTimes(Card  * input__item);
  Card__AttackTimes();
  

};
class Card__MinionType : public Card__Node {
public:
  Card__MinionType(Card  * input__item);
  Card__MinionType();
  

};
class Card__Attributes : public Card__Node {
public:
  Card__Attributes(Card  * input__item);
  Card__Attributes();
  

};
class Card__DamageAttributes : public Card__Node {
public:
  Card__DamageAttributes(Card  * input__item);
  Card__DamageAttributes();
  

};
class Card__ChargeAttr : public Card__Node {
public:
  Card__ChargeAttr(Card  * input__item);
  Card__ChargeAttr();
  

};
class Card__TauntAttr : public Card__Node {
public:
  Card__TauntAttr(Card  * input__item);
  Card__TauntAttr();
  

};
class Card__StealthAttr : public Card__Node {
public:
  Card__StealthAttr(Card  * input__item);
  Card__StealthAttr();
  

};
class Card__UntargetableAttr : public Card__Node {
public:
  Card__UntargetableAttr(Card  * input__item);
  Card__UntargetableAttr();
  

};
class Card__ShieldAttr : public Card__Node {
public:
  Card__ShieldAttr(Card  * input__item);
  Card__ShieldAttr();
  

};
class Card__PoisonousAttr : public Card__Node {
public:
  Card__PoisonousAttr(Card  * input__item);
  Card__PoisonousAttr();
  

};
class Card__LifestealAttr : public Card__Node {
public:
  Card__LifestealAttr(Card  * input__item);
  Card__LifestealAttr();
  

};
class Card__SpecialEffects : public Card__Node {
public:
  Card__SpecialEffects(Card  * input__item);
  Card__SpecialEffects();
  

};
class Card__TargetedPlayEff : public Card__Node {
public:
  Card__TargetedPlayEff(Card  * input__item);
  Card__TargetedPlayEff();
  

};
class Card__OtherEffs : public Card__Node {
public:
  Card__OtherEffs(Card  * input__item);
  Card__OtherEffs();
  

};
class Card__OtherEff : public Card__Node {
public:
  Card__OtherEff(Card  * input__item);
  Card__OtherEff();
  

};
class Card__TargetedEff : public Card__Node {
public:
  Card__TargetedEff(Card  * input__item);
  Card__TargetedEff();
  

};
class Card__UntargetedEff : public Card__Node {
public:
  Card__UntargetedEff(Card  * input__item);
  Card__UntargetedEff();
  

};
class Card__TargetCond : public Card__Node {
public:
  Card__TargetCond(Card  * input__item);
  Card__TargetCond();
  

};
class Card__CharTargetCond : public Card__Node {
public:
  Card__CharTargetCond(Card  * input__item);
  Card__CharTargetCond();
  

};
class Card__CharTypeCond : public Card__Node {
public:
  Card__CharTypeCond(Card  * input__item);
  Card__CharTypeCond();
  

};
class Card__CardTargetCond : public Card__Node {
public:
  Card__CardTargetCond(Card  * input__item);
  Card__CardTargetCond();
  

};
class Card__CardTypeCond : public Card__Node {
public:
  Card__CardTypeCond(Card  * input__item);
  Card__CardTypeCond();
  

};
class Card__CardPosCond : public Card__Node {
public:
  Card__CardPosCond(Card  * input__item);
  Card__CardPosCond();
  

};
class Card__AllegianceCond : public Card__Node {
public:
  Card__AllegianceCond(Card  * input__item);
  Card__AllegianceCond();
  

};
class Card__AttrCond : public Card__Node {
public:
  Card__AttrCond(Card  * input__item);
  Card__AttrCond();
  

};
class Card__StatCond : public Card__Node {
public:
  Card__StatCond(Card  * input__item);
  Card__StatCond();
  

};
class Card__StatCondVariant : public Card__Node {
public:
  Card__StatCondVariant(Card  * input__item);
  Card__StatCondVariant();
  

};
class Card__IndeCond : public Card__Node {
public:
  Card__IndeCond(Card  * input__item);
  Card__IndeCond();
  

};
class Card__BaseTargetedEff : public Card__Node {
public:
  Card__BaseTargetedEff(Card  * input__item);
  Card__BaseTargetedEff();
  

};
class Card__BaseUntargetedEff : public Card__Node {
public:
  Card__BaseUntargetedEff(Card  * input__item);
  Card__BaseUntargetedEff();
  

};
class Card__Destination : public Card__Node {
public:
  Card__Destination(Card  * input__item);
  Card__Destination();
  

};
class Card__NewCardVariant : public Card__Node {
public:
  Card__NewCardVariant(Card  * input__item);
  Card__NewCardVariant();
  

};
class Card__leaderCard : public Card__CardRoot {
public:
  Card__leaderCard();
  Card__leaderCard(Card  * input__item, signed int  input__cost, signed int  input__attack, signed int  input__health, Card__AttackTimes  * input__attack_times, Card__Attributes  * input__attributes, Card__SpecialEffects  * input__effects);
  ~Card__leaderCard();
  signed int cost;
  signed int attack;
  signed int health;
  Card__AttackTimes  *attack_times;
  Card__Attributes  *attributes;
  Card__SpecialEffects  *effects;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  Card__SpecialEffects  *GetEffects();
  signed int isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  void Play(signed int  x, signed int  y, signed int  z, Card  * parent_card);
  void Discard(Card  * parent_card);
  signed int CheckAttackValid(signed int  x, signed int  z, Card  * parent_card);
  void Attack(signed int  x, signed int  z, Card  * parent_card);
  void TurnStart(Card  * leader, Card  * parent_card);
  void TurnEnd(Card  * leader, Card  * parent_card);
  void Mutate(signed int  min_eff_n, signed int  max_eff_n, signed int  effect_depth);
  void SetOverheatCounts(signed int  val);
  void SetOverheatThresholds(signed int  val);
  void ModOverheatThresholds(signed int  amount);
  

};
class Card__minionCard : public Card__CardRoot {
public:
  Card__minionCard();
  Card__minionCard(Card  * input__item, signed int  input__cost, signed int  input__attack, signed int  input__health, Card__AttackTimes  * input__attack_times, Card__MinionType  * input__type, Card__Attributes  * input__attributes, Card__SpecialEffects  * input__effects);
  ~Card__minionCard();
  signed int cost;
  signed int attack;
  signed int health;
  Card__AttackTimes  *attack_times;
  Card__MinionType  *type;
  Card__Attributes  *attributes;
  Card__SpecialEffects  *effects;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  Card__SpecialEffects  *GetEffects();
  signed int isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  void Play(signed int  x, signed int  y, signed int  z, Card  * parent_card);
  void Destroy(Card  * parent_card);
  void Discard(Card  * parent_card);
  signed int CheckAttackValid(signed int  x, signed int  z, Card  * parent_card);
  void Attack(signed int  x, signed int  z, Card  * parent_card);
  void TurnStart(Card  * leader, Card  * parent_card);
  void TurnEnd(Card  * leader, Card  * parent_card);
  void Mutate(signed int  min_eff_n, signed int  max_eff_n, signed int  effect_depth);
  void SetOverheatCounts(signed int  val);
  void SetOverheatThresholds(signed int  val);
  void ModOverheatThresholds(signed int  amount);
  

};
class Card__spellCard : public Card__CardRoot {
public:
  Card__spellCard();
  Card__spellCard(Card  * input__item, signed int  input__cost, Card__Attributes  * input__attributes, Card__SpecialEffects  * input__effects);
  ~Card__spellCard();
  signed int cost;
  Card__Attributes  *attributes;
  Card__SpecialEffects  *effects;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  Card__SpecialEffects  *GetEffects();
  signed int isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  void Play(signed int  x, signed int  y, signed int  z, Card  * parent_card);
  void Discard(Card  * parent_card);
  void TurnStart(Card  * leader, Card  * parent_card);
  void TurnEnd(Card  * leader, Card  * parent_card);
  void Mutate(signed int  min_eff_n, signed int  max_eff_n, signed int  effect_depth);
  void SetOverheatCounts(signed int  val);
  void SetOverheatThresholds(signed int  val);
  void ModOverheatThresholds(signed int  amount);
  

};
class Card__zeroAttack : public Card__AttackTimes {
public:
  Card__zeroAttack();
  Card__zeroAttack(Card  * input__item);
  ~Card__zeroAttack();
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  

};
class Card__singleAttack : public Card__AttackTimes {
public:
  Card__singleAttack();
  Card__singleAttack(Card  * input__item);
  ~Card__singleAttack();
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  

};
class Card__multipleAttack : public Card__AttackTimes {
public:
  Card__multipleAttack();
  Card__multipleAttack(Card  * input__item, signed int  input__n_atks);
  ~Card__multipleAttack();
  signed int n_atks;
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  

};
class Card__beastMinion : public Card__MinionType {
public:
  Card__beastMinion();
  Card__beastMinion(Card  * input__item);
  ~Card__beastMinion();
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  

};
class Card__dragonMinion : public Card__MinionType {
public:
  Card__dragonMinion();
  Card__dragonMinion(Card  * input__item);
  ~Card__dragonMinion();
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  

};
class Card__demonMinion : public Card__MinionType {
public:
  Card__demonMinion();
  Card__demonMinion(Card  * input__item);
  ~Card__demonMinion();
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  

};
class Card__justAttributes : public Card__Attributes {
public:
  Card__justAttributes();
  Card__justAttributes(Card  * input__item, Card__ChargeAttr  * input__c, Card__TauntAttr  * input__t, Card__StealthAttr  * input__s, Card__UntargetableAttr  * input__u, Card__ShieldAttr  * input__d, Card__PoisonousAttr  * input__p, Card__LifestealAttr  * input__l);
  ~Card__justAttributes();
  Card__ChargeAttr  *c;
  Card__TauntAttr  *t;
  Card__StealthAttr  *s;
  Card__UntargetableAttr  *u;
  Card__ShieldAttr  *d;
  Card__PoisonousAttr  *p;
  Card__LifestealAttr  *l;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  

};
class Card__damageAttributes : public Card__DamageAttributes {
public:
  Card__damageAttributes();
  Card__damageAttributes(Card  * input__item, Card__PoisonousAttr  * input__p, Card__LifestealAttr  * input__l);
  ~Card__damageAttributes();
  Card__PoisonousAttr  *p;
  Card__LifestealAttr  *l;
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  

};
class Card__noCharge : public Card__ChargeAttr {
public:
  Card__noCharge();
  Card__noCharge(Card  * input__item);
  ~Card__noCharge();
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  

};
class Card__justCharge : public Card__ChargeAttr {
public:
  Card__justCharge();
  Card__justCharge(Card  * input__item);
  ~Card__justCharge();
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  

};
class Card__noTaunt : public Card__TauntAttr {
public:
  Card__noTaunt();
  Card__noTaunt(Card  * input__item);
  ~Card__noTaunt();
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  

};
class Card__justTaunt : public Card__TauntAttr {
public:
  Card__justTaunt();
  Card__justTaunt(Card  * input__item);
  ~Card__justTaunt();
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  

};
class Card__noStealth : public Card__StealthAttr {
public:
  Card__noStealth();
  Card__noStealth(Card  * input__item);
  ~Card__noStealth();
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  

};
class Card__justStealth : public Card__StealthAttr {
public:
  Card__justStealth();
  Card__justStealth(Card  * input__item);
  ~Card__justStealth();
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  

};
class Card__noUntargetable : public Card__UntargetableAttr {
public:
  Card__noUntargetable();
  Card__noUntargetable(Card  * input__item);
  ~Card__noUntargetable();
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  

};
class Card__justUntargetable : public Card__UntargetableAttr {
public:
  Card__justUntargetable();
  Card__justUntargetable(Card  * input__item);
  ~Card__justUntargetable();
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  

};
class Card__noShield : public Card__ShieldAttr {
public:
  Card__noShield();
  Card__noShield(Card  * input__item);
  ~Card__noShield();
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  

};
class Card__justShield : public Card__ShieldAttr {
public:
  Card__justShield();
  Card__justShield(Card  * input__item);
  ~Card__justShield();
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  

};
class Card__noPoisonous : public Card__PoisonousAttr {
public:
  Card__noPoisonous();
  Card__noPoisonous(Card  * input__item);
  ~Card__noPoisonous();
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  

};
class Card__justPoisonous : public Card__PoisonousAttr {
public:
  Card__justPoisonous();
  Card__justPoisonous(Card  * input__item);
  ~Card__justPoisonous();
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  

};
class Card__noLifesteal : public Card__LifestealAttr {
public:
  Card__noLifesteal();
  Card__noLifesteal(Card  * input__item);
  ~Card__noLifesteal();
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  

};
class Card__justLifesteal : public Card__LifestealAttr {
public:
  Card__justLifesteal();
  Card__justLifesteal(Card  * input__item);
  ~Card__justLifesteal();
  void FillRep(CardRep  & rep);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  

};
class Card__specialEffects : public Card__SpecialEffects {
public:
  Card__specialEffects();
  Card__specialEffects(Card  * input__item, Card__TargetedPlayEff  * input__effect, Card__OtherEffs  * input__effects);
  ~Card__specialEffects();
  Card__TargetedPlayEff  *effect;
  Card__OtherEffs  *effects;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  signed int isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  void Play(signed int  x, signed int  y, signed int  z, Card  * parent_card);
  void Destroy(Card  * parent_card);
  void Discard(Card  * parent_card);
  void TurnStart(Card  * leader, Card  * parent_card);
  void TurnEnd(Card  * leader, Card  * parent_card);
  void SetOverheatCounts(signed int  val);
  void SetOverheatThresholds(signed int  val);
  void ModOverheatThresholds(signed int  amount);
  

};
class Card__noTargetedPlayEff : public Card__TargetedPlayEff {
public:
  Card__noTargetedPlayEff();
  Card__noTargetedPlayEff(Card  * input__item);
  ~Card__noTargetedPlayEff();
  void FillRep(CardRep  & rep);
  string DetailIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  

};
class Card__targetedBattlecryEff : public Card__TargetedPlayEff {
public:
  Card__targetedBattlecryEff();
  Card__targetedBattlecryEff(Card  * input__item, Card__TargetedEff  * input__effect);
  ~Card__targetedBattlecryEff();
  Card__TargetedEff  *effect;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  signed int GetEffectNum();
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  signed int isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  void Play(signed int  x, signed int  y, signed int  z, Card  * parent_card);
  void SetOverheatCounts(signed int  val);
  void SetOverheatThresholds(signed int  val);
  void ModOverheatThresholds(signed int  amount);
  

};
class Card__targetedCastEff : public Card__TargetedPlayEff {
public:
  Card__targetedCastEff();
  Card__targetedCastEff(Card  * input__item, Card__TargetedEff  * input__effect);
  ~Card__targetedCastEff();
  Card__TargetedEff  *effect;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  signed int GetEffectNum();
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  signed int isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  void Play(signed int  x, signed int  y, signed int  z, Card  * parent_card);
  void SetOverheatCounts(signed int  val);
  void SetOverheatThresholds(signed int  val);
  void ModOverheatThresholds(signed int  amount);
  

};
class Card__noOtherEffs : public Card__OtherEffs {
public:
  Card__noOtherEffs();
  Card__noOtherEffs(Card  * input__item);
  ~Card__noOtherEffs();
  void FillRep(CardRep  & rep);
  string DetailIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  

};
class Card__consOtherEffs : public Card__OtherEffs {
public:
  Card__consOtherEffs();
  Card__consOtherEffs(Card  * input__item, Card__OtherEff  * input__effect, Card__OtherEffs  * input__effects);
  ~Card__consOtherEffs();
  Card__OtherEff  *effect;
  Card__OtherEffs  *effects;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  signed int GetEffectNum();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  void Play(signed int  x, signed int  y, signed int  z, Card  * parent_card);
  void Destroy(Card  * parent_card);
  void Discard(Card  * parent_card);
  void TurnStart(Card  * leader, Card  * parent_card);
  void TurnEnd(Card  * leader, Card  * parent_card);
  void SetOverheatCounts(signed int  val);
  void SetOverheatThresholds(signed int  val);
  void ModOverheatThresholds(signed int  amount);
  

};
class Card__untargetedBattlecryEff : public Card__OtherEff {
public:
  Card__untargetedBattlecryEff();
  Card__untargetedBattlecryEff(Card  * input__item, Card__UntargetedEff  * input__effect);
  ~Card__untargetedBattlecryEff();
  Card__UntargetedEff  *effect;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  void Play(signed int  x, signed int  y, signed int  z, Card  * parent_card);
  void SetOverheatCounts(signed int  val);
  void SetOverheatThresholds(signed int  val);
  void ModOverheatThresholds(signed int  amount);
  

};
class Card__untargetedCastEff : public Card__OtherEff {
public:
  Card__untargetedCastEff();
  Card__untargetedCastEff(Card  * input__item, Card__UntargetedEff  * input__effect);
  ~Card__untargetedCastEff();
  Card__UntargetedEff  *effect;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  void Play(signed int  x, signed int  y, signed int  z, Card  * parent_card);
  void SetOverheatCounts(signed int  val);
  void SetOverheatThresholds(signed int  val);
  void ModOverheatThresholds(signed int  amount);
  

};
class Card__deathrattleEff : public Card__OtherEff {
public:
  Card__deathrattleEff();
  Card__deathrattleEff(Card  * input__item, Card__UntargetedEff  * input__effect);
  ~Card__deathrattleEff();
  Card__UntargetedEff  *effect;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  void Destroy(Card  * parent_card);
  void SetOverheatCounts(signed int  val);
  void SetOverheatThresholds(signed int  val);
  void ModOverheatThresholds(signed int  amount);
  

};
class Card__onDiscardEff : public Card__OtherEff {
public:
  Card__onDiscardEff();
  Card__onDiscardEff(Card  * input__item, Card__UntargetedEff  * input__effect);
  ~Card__onDiscardEff();
  Card__UntargetedEff  *effect;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  void Discard(Card  * parent_card);
  void SetOverheatCounts(signed int  val);
  void SetOverheatThresholds(signed int  val);
  void ModOverheatThresholds(signed int  amount);
  

};
class Card__turnStartEff : public Card__OtherEff {
public:
  Card__turnStartEff();
  Card__turnStartEff(Card  * input__item, Card__UntargetedEff  * input__effect, Card__AllegianceCond  * input__alle);
  ~Card__turnStartEff();
  Card__UntargetedEff  *effect;
  Card__AllegianceCond  *alle;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  void TurnStart(Card  * leader, Card  * parent_card);
  void SetOverheatCounts(signed int  val);
  void SetOverheatThresholds(signed int  val);
  void ModOverheatThresholds(signed int  amount);
  

};
class Card__turnEndEff : public Card__OtherEff {
public:
  Card__turnEndEff();
  Card__turnEndEff(Card  * input__item, Card__UntargetedEff  * input__effect, Card__AllegianceCond  * input__alle);
  ~Card__turnEndEff();
  Card__UntargetedEff  *effect;
  Card__AllegianceCond  *alle;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  void TurnEnd(Card  * leader, Card  * parent_card);
  void SetOverheatCounts(signed int  val);
  void SetOverheatThresholds(signed int  val);
  void ModOverheatThresholds(signed int  amount);
  

};
class Card__noCondTargetedEff : public Card__TargetedEff {
public:
  Card__noCondTargetedEff();
  Card__noCondTargetedEff(Card  * input__item, Card__BaseTargetedEff  * input__effect, Card__TargetCond  * input__desconstr);
  ~Card__noCondTargetedEff();
  Card__BaseTargetedEff  *effect;
  Card__TargetCond  *desconstr;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  string Postfix();
  string PostfixIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  signed int isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__indeCondTargetedEff : public Card__TargetedEff {
public:
  Card__indeCondTargetedEff();
  Card__indeCondTargetedEff(Card  * input__item, Card__BaseTargetedEff  * input__effect, Card__TargetCond  * input__desconstr, Card__IndeCond  * input__cond);
  ~Card__indeCondTargetedEff();
  Card__BaseTargetedEff  *effect;
  Card__TargetCond  *desconstr;
  Card__IndeCond  *cond;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  string Postfix();
  string PostfixIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  signed int isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__srcCondTargetedEff : public Card__TargetedEff {
public:
  Card__srcCondTargetedEff();
  Card__srcCondTargetedEff(Card  * input__item, Card__BaseTargetedEff  * input__effect, Card__TargetCond  * input__desconstr, Card__TargetCond  * input__srccond);
  ~Card__srcCondTargetedEff();
  Card__BaseTargetedEff  *effect;
  Card__TargetCond  *desconstr;
  Card__TargetCond  *srccond;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  string Postfix();
  string PostfixIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  signed int isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__noCondUntargetedEff : public Card__UntargetedEff {
public:
  Card__noCondUntargetedEff();
  Card__noCondUntargetedEff(Card  * input__item, Card__BaseUntargetedEff  * input__effect);
  ~Card__noCondUntargetedEff();
  Card__BaseUntargetedEff  *effect;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  string Postfix();
  string PostfixIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  void UntargetedAction(Card  * parent_card);
  

};
class Card__indeCondUntargetedEff : public Card__UntargetedEff {
public:
  Card__indeCondUntargetedEff();
  Card__indeCondUntargetedEff(Card  * input__item, Card__BaseUntargetedEff  * input__effect, Card__IndeCond  * input__cond);
  ~Card__indeCondUntargetedEff();
  Card__BaseUntargetedEff  *effect;
  Card__IndeCond  *cond;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  string Postfix();
  string PostfixIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  void UntargetedAction(Card  * parent_card);
  

};
class Card__srcCondUntargetedEff : public Card__UntargetedEff {
public:
  Card__srcCondUntargetedEff();
  Card__srcCondUntargetedEff(Card  * input__item, Card__BaseUntargetedEff  * input__effect, Card__TargetCond  * input__srccond);
  ~Card__srcCondUntargetedEff();
  Card__BaseUntargetedEff  *effect;
  Card__TargetCond  *srccond;
  void FillRep(CardRep  & rep);
  string Brief();
  string Detail();
  string DetailIndent(signed int  indent_size);
  string Postfix();
  string PostfixIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  void UntargetedAction(Card  * parent_card);
  

};
class Card__charTargetCond : public Card__TargetCond {
public:
  Card__charTargetCond();
  Card__charTargetCond(Card  * input__item, Card__CharTargetCond  * input__cond);
  ~Card__charTargetCond();
  Card__CharTargetCond  *cond;
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  string DetailAlt2();
  string DetailAlt3();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__cardTargetCond : public Card__TargetCond {
public:
  Card__cardTargetCond();
  Card__cardTargetCond(Card  * input__item, Card__CardTargetCond  * input__cond);
  ~Card__cardTargetCond();
  Card__CardTargetCond  *cond;
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  string DetailAlt2();
  string DetailAlt3();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__justCharTargetCond : public Card__CharTargetCond {
public:
  Card__justCharTargetCond();
  Card__justCharTargetCond(Card  * input__item, Card__AllegianceCond  * input__alle, Card__CharTypeCond  * input__typecond, Card__AttrCond  * input__attrcond, Card__StatCond  * input__statcond);
  ~Card__justCharTargetCond();
  Card__AllegianceCond  *alle;
  Card__CharTypeCond  *typecond;
  Card__AttrCond  *attrcond;
  Card__StatCond  *statcond;
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  string DetailAlt2();
  string DetailAlt3();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__isCharacter : public Card__CharTypeCond {
public:
  Card__isCharacter();
  Card__isCharacter(Card  * input__item);
  ~Card__isCharacter();
  void FillRep(CardRep  & rep);
  signed int isCondTrivial();
  string Detail();
  string DetailAlt1();
  string DetailAlt2();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  

};
class Card__isMinion : public Card__CharTypeCond {
public:
  Card__isMinion();
  Card__isMinion(Card  * input__item);
  ~Card__isMinion();
  void FillRep(CardRep  & rep);
  signed int isCondTrivial();
  string Detail();
  string DetailAlt1();
  string DetailAlt2();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__isBeast : public Card__CharTypeCond {
public:
  Card__isBeast();
  Card__isBeast(Card  * input__item);
  ~Card__isBeast();
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  string DetailAlt2();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  CondConfig GetTargetConfig();
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__isDragon : public Card__CharTypeCond {
public:
  Card__isDragon();
  Card__isDragon(Card  * input__item);
  ~Card__isDragon();
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  string DetailAlt2();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  CondConfig GetTargetConfig();
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__isDemon : public Card__CharTypeCond {
public:
  Card__isDemon();
  Card__isDemon(Card  * input__item);
  ~Card__isDemon();
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  string DetailAlt2();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  CondConfig GetTargetConfig();
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__justCardTargetCond : public Card__CardTargetCond {
public:
  Card__justCardTargetCond();
  Card__justCardTargetCond(Card  * input__item, Card__CardPosCond  * input__pos, Card__AllegianceCond  * input__alle, Card__CardTypeCond  * input__typecond, Card__AttrCond  * input__attrcond, Card__StatCond  * input__statcond);
  ~Card__justCardTargetCond();
  Card__CardPosCond  *pos;
  Card__AllegianceCond  *alle;
  Card__CardTypeCond  *typecond;
  Card__AttrCond  *attrcond;
  Card__StatCond  *statcond;
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  string DetailAlt2();
  string DetailAlt3();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__isCard : public Card__CardTypeCond {
public:
  Card__isCard();
  Card__isCard(Card  * input__item);
  ~Card__isCard();
  void FillRep(CardRep  & rep);
  signed int isCondTrivial();
  string Detail();
  string DetailAlt1();
  string DetailAlt2();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  

};
class Card__isLeaderCard : public Card__CardTypeCond {
public:
  Card__isLeaderCard();
  Card__isLeaderCard(Card  * input__item);
  ~Card__isLeaderCard();
  void FillRep(CardRep  & rep);
  signed int isCondTrivial();
  string Detail();
  string DetailAlt1();
  string DetailAlt2();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__isMinionCard : public Card__CardTypeCond {
public:
  Card__isMinionCard();
  Card__isMinionCard(Card  * input__item);
  ~Card__isMinionCard();
  void FillRep(CardRep  & rep);
  signed int isCondTrivial();
  string Detail();
  string DetailAlt1();
  string DetailAlt2();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__isSpellCard : public Card__CardTypeCond {
public:
  Card__isSpellCard();
  Card__isSpellCard(Card  * input__item);
  ~Card__isSpellCard();
  void FillRep(CardRep  & rep);
  signed int isCondTrivial();
  string Detail();
  string DetailAlt1();
  string DetailAlt2();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__isBeastCard : public Card__CardTypeCond {
public:
  Card__isBeastCard();
  Card__isBeastCard(Card  * input__item);
  ~Card__isBeastCard();
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  string DetailAlt2();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  CondConfig GetTargetConfig();
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__isDragonCard : public Card__CardTypeCond {
public:
  Card__isDragonCard();
  Card__isDragonCard(Card  * input__item);
  ~Card__isDragonCard();
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  string DetailAlt2();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  CondConfig GetTargetConfig();
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__isDemonCard : public Card__CardTypeCond {
public:
  Card__isDemonCard();
  Card__isDemonCard(Card  * input__item);
  ~Card__isDemonCard();
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  string DetailAlt2();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  CondConfig GetTargetConfig();
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__cardPosAtHandOrDeck : public Card__CardPosCond {
public:
  Card__cardPosAtHandOrDeck();
  Card__cardPosAtHandOrDeck(Card  * input__item);
  ~Card__cardPosAtHandOrDeck();
  void FillRep(CardRep  & rep);
  signed int isCondTrivial();
  string Detail();
  string DetailAlt1();
  string DetailAlt2();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  

};
class Card__cardPosAtHand : public Card__CardPosCond {
public:
  Card__cardPosAtHand();
  Card__cardPosAtHand(Card  * input__item);
  ~Card__cardPosAtHand();
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt2();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__cardPosAtDeck : public Card__CardPosCond {
public:
  Card__cardPosAtDeck();
  Card__cardPosAtDeck(Card  * input__item);
  ~Card__cardPosAtDeck();
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt2();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__anyAllegiance : public Card__AllegianceCond {
public:
  Card__anyAllegiance();
  Card__anyAllegiance(Card  * input__item);
  ~Card__anyAllegiance();
  void FillRep(CardRep  & rep);
  signed int isCondTrivial();
  string DetailAlt2();
  string DetailAlt3();
  string DetailAlt4();
  string DetailAlt5();
  string DetailAlt6();
  signed int IsPlural();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  

};
class Card__allyAllegiance : public Card__AllegianceCond {
public:
  Card__allyAllegiance();
  Card__allyAllegiance(Card  * input__item);
  ~Card__allyAllegiance();
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  string DetailAlt2();
  string DetailAlt3();
  string DetailAlt4();
  string DetailAlt5();
  string DetailAlt6();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  

};
class Card__oppoAllegiance : public Card__AllegianceCond {
public:
  Card__oppoAllegiance();
  Card__oppoAllegiance(Card  * input__item);
  ~Card__oppoAllegiance();
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  string DetailAlt2();
  string DetailAlt3();
  string DetailAlt4();
  string DetailAlt5();
  string DetailAlt6();
  signed int IsThirdPersonSingle();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  signed int CheckThisValid(Card  * parent_card);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  

};
class Card__noAttrCond : public Card__AttrCond {
public:
  Card__noAttrCond();
  Card__noAttrCond(Card  * input__item);
  ~Card__noAttrCond();
  void FillRep(CardRep  & rep);
  signed int isCondTrivial();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  

};
class Card__chargeCond : public Card__AttrCond {
public:
  Card__chargeCond();
  Card__chargeCond(Card  * input__item);
  ~Card__chargeCond();
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__tauntCond : public Card__AttrCond {
public:
  Card__tauntCond();
  Card__tauntCond(Card  * input__item);
  ~Card__tauntCond();
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__stealthCond : public Card__AttrCond {
public:
  Card__stealthCond();
  Card__stealthCond(Card  * input__item);
  ~Card__stealthCond();
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__untargetableCond : public Card__AttrCond {
public:
  Card__untargetableCond();
  Card__untargetableCond(Card  * input__item);
  ~Card__untargetableCond();
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__shieldCond : public Card__AttrCond {
public:
  Card__shieldCond();
  Card__shieldCond(Card  * input__item);
  ~Card__shieldCond();
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__poisonousCond : public Card__AttrCond {
public:
  Card__poisonousCond();
  Card__poisonousCond(Card  * input__item);
  ~Card__poisonousCond();
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__lifestealCond : public Card__AttrCond {
public:
  Card__lifestealCond();
  Card__lifestealCond(Card  * input__item);
  ~Card__lifestealCond();
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  unsigned int GetInitAttrFlag();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__noStatCond : public Card__StatCond {
public:
  Card__noStatCond();
  Card__noStatCond(Card  * input__item);
  ~Card__noStatCond();
  void FillRep(CardRep  & rep);
  signed int isCondTrivial();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  

};
class Card__costCond : public Card__StatCond {
public:
  Card__costCond();
  Card__costCond(Card  * input__item, Card__StatCondVariant  * input__variant);
  ~Card__costCond();
  Card__StatCondVariant  *variant;
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__atkCond : public Card__StatCond {
public:
  Card__atkCond();
  Card__atkCond(Card  * input__item, Card__StatCondVariant  * input__variant);
  ~Card__atkCond();
  Card__StatCondVariant  *variant;
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__hpCond : public Card__StatCond {
public:
  Card__hpCond();
  Card__hpCond(Card  * input__item, Card__StatCondVariant  * input__variant);
  ~Card__hpCond();
  Card__StatCondVariant  *variant;
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__atkTimesCond : public Card__StatCond {
public:
  Card__atkTimesCond();
  Card__atkTimesCond(Card  * input__item, Card__StatCondVariant  * input__variant);
  ~Card__atkTimesCond();
  Card__StatCondVariant  *variant;
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  signed int CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card);
  signed int CheckCardValid(Card  * card, Card  * parent_card);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__statGe : public Card__StatCondVariant {
public:
  Card__statGe();
  Card__statGe(Card  * input__item, signed int  input__val);
  ~Card__statGe();
  signed int val;
  void FillRep(CardRep  & rep);
  string Detail();
  signed int IsPlural();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  void AdjustGlobalStatRange(signed int  & min_val, signed int  & max_val);
  signed int CheckStatValid(signed int  stat_val);
  

};
class Card__statLe : public Card__StatCondVariant {
public:
  Card__statLe();
  Card__statLe(Card  * input__item, signed int  input__val);
  ~Card__statLe();
  signed int val;
  void FillRep(CardRep  & rep);
  string Detail();
  signed int IsPlural();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  void AdjustGlobalStatRange(signed int  & min_val, signed int  & max_val);
  signed int CheckStatValid(signed int  stat_val);
  

};
class Card__fieldExistenceCond : public Card__IndeCond {
public:
  Card__fieldExistenceCond();
  Card__fieldExistenceCond(Card  * input__item, Card__CharTargetCond  * input__cond);
  ~Card__fieldExistenceCond();
  Card__CharTargetCond  *cond;
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__cardExistenceCond : public Card__IndeCond {
public:
  Card__cardExistenceCond();
  Card__cardExistenceCond(Card  * input__item, Card__CardTargetCond  * input__cond);
  ~Card__cardExistenceCond();
  Card__CardTargetCond  *cond;
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__leaderCond : public Card__IndeCond {
public:
  Card__leaderCond();
  Card__leaderCond(Card  * input__item, Card__AllegianceCond  * input__alle, Card__AttrCond  * input__attrcond, Card__StatCond  * input__statcond);
  ~Card__leaderCond();
  Card__AllegianceCond  *alle;
  Card__AttrCond  *attrcond;
  Card__StatCond  *statcond;
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__mpCond : public Card__IndeCond {
public:
  Card__mpCond();
  Card__mpCond(Card  * input__item, Card__AllegianceCond  * input__alle, Card__StatCondVariant  * input__variant);
  ~Card__mpCond();
  Card__AllegianceCond  *alle;
  Card__StatCondVariant  *variant;
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__maxMpCond : public Card__IndeCond {
public:
  Card__maxMpCond();
  Card__maxMpCond(Card  * input__item, Card__AllegianceCond  * input__alle, Card__StatCondVariant  * input__variant);
  ~Card__maxMpCond();
  Card__AllegianceCond  *alle;
  Card__StatCondVariant  *variant;
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  signed int CheckThisValid(Card  * parent_card);
  

};
class Card__damageEff : public Card__BaseTargetedEff {
public:
  Card__damageEff();
  Card__damageEff(Card  * input__item, signed int  input__val, Card__DamageAttributes  * input__attr);
  ~Card__damageEff();
  signed int val;
  Card__DamageAttributes  *attr;
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__healEff : public Card__BaseTargetedEff {
public:
  Card__healEff();
  Card__healEff(Card  * input__item, signed int  input__val);
  ~Card__healEff();
  signed int val;
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__resAtkTimesEff : public Card__BaseTargetedEff {
public:
  Card__resAtkTimesEff();
  Card__resAtkTimesEff(Card  * input__item, signed int  input__val);
  ~Card__resAtkTimesEff();
  signed int val;
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__costModEff : public Card__BaseTargetedEff {
public:
  Card__costModEff();
  Card__costModEff(Card  * input__item, signed int  input__val);
  ~Card__costModEff();
  double inc_prob;
  signed int val;
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__atkHpModEff : public Card__BaseTargetedEff {
public:
  Card__atkHpModEff();
  Card__atkHpModEff(Card  * input__item, signed int  input__atkmod, signed int  input__hpmod);
  ~Card__atkHpModEff();
  signed int atkmod;
  signed int hpmod;
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__atkTimesModEff : public Card__BaseTargetedEff {
public:
  Card__atkTimesModEff();
  Card__atkTimesModEff(Card  * input__item, signed int  input__val);
  ~Card__atkTimesModEff();
  double dec_prob;
  signed int val;
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__destroyEff : public Card__BaseTargetedEff {
public:
  Card__destroyEff();
  Card__destroyEff(Card  * input__item);
  ~Card__destroyEff();
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__discardEff : public Card__BaseTargetedEff {
public:
  Card__discardEff();
  Card__discardEff(Card  * input__item);
  ~Card__discardEff();
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  CondConfig GetSelfConfig(const CondConfig  & self_config);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__moveEff : public Card__BaseTargetedEff {
public:
  Card__moveEff();
  Card__moveEff(Card  * input__item, Card__Destination  * input__dest);
  ~Card__moveEff();
  Card__Destination  *dest;
  void FillRep(CardRep  & rep);
  string Detail();
  string Suffix();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetPlayTargetConfig();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  CondConfig GetSelfConfig(const CondConfig  & self_config);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__copyEff : public Card__BaseTargetedEff {
public:
  Card__copyEff();
  Card__copyEff(Card  * input__item, Card__Destination  * input__dest);
  ~Card__copyEff();
  Card__Destination  *dest;
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  string Suffix();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetPlayTargetConfig();
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  CondConfig GetSelfConfig(const CondConfig  & self_config);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__transformEff : public Card__BaseTargetedEff {
public:
  Card__transformEff();
  Card__transformEff(Card  * input__item, Card__NewCardVariant  * input__variant);
  ~Card__transformEff();
  Card__NewCardVariant  *variant;
  void FillRep(CardRep  & rep);
  string Detail();
  string Suffix();
  string SuffixAlt1();
  string Postfix();
  string PostfixIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__changeToBeastEff : public Card__BaseTargetedEff {
public:
  Card__changeToBeastEff();
  Card__changeToBeastEff(Card  * input__item);
  ~Card__changeToBeastEff();
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  string Suffix();
  string SuffixAlt1();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__changeToDragonEff : public Card__BaseTargetedEff {
public:
  Card__changeToDragonEff();
  Card__changeToDragonEff(Card  * input__item);
  ~Card__changeToDragonEff();
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  string Suffix();
  string SuffixAlt1();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__changeToDemonEff : public Card__BaseTargetedEff {
public:
  Card__changeToDemonEff();
  Card__changeToDemonEff(Card  * input__item);
  ~Card__changeToDemonEff();
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  string Suffix();
  string SuffixAlt1();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__giveChargeEff : public Card__BaseTargetedEff {
public:
  Card__giveChargeEff();
  Card__giveChargeEff(Card  * input__item);
  ~Card__giveChargeEff();
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__giveTauntEff : public Card__BaseTargetedEff {
public:
  Card__giveTauntEff();
  Card__giveTauntEff(Card  * input__item);
  ~Card__giveTauntEff();
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__giveStealthEff : public Card__BaseTargetedEff {
public:
  Card__giveStealthEff();
  Card__giveStealthEff(Card  * input__item);
  ~Card__giveStealthEff();
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  CondConfig GetSelfConfig(const CondConfig  & self_config);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__giveUntargetableEff : public Card__BaseTargetedEff {
public:
  Card__giveUntargetableEff();
  Card__giveUntargetableEff(Card  * input__item);
  ~Card__giveUntargetableEff();
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__giveShieldEff : public Card__BaseTargetedEff {
public:
  Card__giveShieldEff();
  Card__giveShieldEff(Card  * input__item);
  ~Card__giveShieldEff();
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  CondConfig GetSelfConfig(const CondConfig  & self_config);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__givePoisonousEff : public Card__BaseTargetedEff {
public:
  Card__givePoisonousEff();
  Card__givePoisonousEff(Card  * input__item);
  ~Card__givePoisonousEff();
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__giveLifestealEff : public Card__BaseTargetedEff {
public:
  Card__giveLifestealEff();
  Card__giveLifestealEff(Card  * input__item);
  ~Card__giveLifestealEff();
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__rmAttributesEff : public Card__BaseTargetedEff {
public:
  Card__rmAttributesEff();
  Card__rmAttributesEff(Card  * input__item);
  ~Card__rmAttributesEff();
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__setOverheatEff : public Card__BaseTargetedEff {
public:
  Card__setOverheatEff();
  Card__setOverheatEff(Card  * input__item);
  ~Card__setOverheatEff();
  void FillRep(CardRep  & rep);
  string Detail();
  string Suffix();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__decOhThresholdEff : public Card__BaseTargetedEff {
public:
  Card__decOhThresholdEff();
  Card__decOhThresholdEff(Card  * input__item, signed int  input__val);
  ~Card__decOhThresholdEff();
  signed int val;
  void FillRep(CardRep  & rep);
  string Detail();
  string Suffix();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__resetStateEff : public Card__BaseTargetedEff {
public:
  Card__resetStateEff();
  Card__resetStateEff(Card  * input__item);
  ~Card__resetStateEff();
  void FillRep(CardRep  & rep);
  string Detail();
  string Suffix();
  string SuffixAlt1();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__giveEffectsEff : public Card__BaseTargetedEff {
public:
  Card__giveEffectsEff();
  Card__giveEffectsEff(Card  * input__item, Card__SpecialEffects  * input__effects);
  ~Card__giveEffectsEff();
  Card__SpecialEffects  *effects;
  void FillRep(CardRep  & rep);
  string Detail();
  string Postfix();
  string PostfixIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  signed int TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__aoeEff : public Card__BaseUntargetedEff {
public:
  Card__aoeEff();
  Card__aoeEff(Card  * input__item, Card__BaseTargetedEff  * input__effect, Card__TargetCond  * input__cond);
  ~Card__aoeEff();
  Card__BaseTargetedEff  *effect;
  Card__TargetCond  *cond;
  void FillRep(CardRep  & rep);
  string Detail();
  string Postfix();
  string PostfixIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  void UntargetedAction(Card  * parent_card);
  

};
class Card__randEff : public Card__BaseUntargetedEff {
public:
  Card__randEff();
  Card__randEff(Card  * input__item, Card__BaseTargetedEff  * input__effect, Card__TargetCond  * input__cond);
  ~Card__randEff();
  Card__BaseTargetedEff  *effect;
  Card__TargetCond  *cond;
  void FillRep(CardRep  & rep);
  string Detail();
  string Postfix();
  string PostfixIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  void UntargetedAction(Card  * parent_card);
  

};
class Card__leaderEff : public Card__BaseUntargetedEff {
public:
  Card__leaderEff();
  Card__leaderEff(Card  * input__item, Card__BaseTargetedEff  * input__effect, Card__AllegianceCond  * input__alle);
  ~Card__leaderEff();
  Card__BaseTargetedEff  *effect;
  Card__AllegianceCond  *alle;
  void FillRep(CardRep  & rep);
  string Detail();
  string Postfix();
  string PostfixIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetLeaderConfig();
  void UntargetedAction(Card  * parent_card);
  

};
class Card__selfEff : public Card__BaseUntargetedEff {
public:
  Card__selfEff();
  Card__selfEff(Card  * input__item, Card__BaseTargetedEff  * input__effect);
  ~Card__selfEff();
  Card__BaseTargetedEff  *effect;
  void FillRep(CardRep  & rep);
  string Detail();
  string Postfix();
  string PostfixIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing);
  CondConfig GetSelfConfig(const CondConfig  & self_config);
  void UntargetedAction(Card  * parent_card);
  

};
class Card__drawCardEff : public Card__BaseUntargetedEff {
public:
  Card__drawCardEff();
  Card__drawCardEff(Card  * input__item, signed int  input__val, Card__AllegianceCond  * input__alle);
  ~Card__drawCardEff();
  signed int val;
  Card__AllegianceCond  *alle;
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  void UntargetedAction(Card  * parent_card);
  

};
class Card__mpEff : public Card__BaseUntargetedEff {
public:
  Card__mpEff();
  Card__mpEff(Card  * input__item, signed int  input__val, Card__AllegianceCond  * input__alle);
  ~Card__mpEff();
  double dec_prob;
  signed int val;
  Card__AllegianceCond  *alle;
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetLeaderConfig();
  void UntargetedAction(Card  * parent_card);
  

};
class Card__maxMpEff : public Card__BaseUntargetedEff {
public:
  Card__maxMpEff();
  Card__maxMpEff(Card  * input__item, signed int  input__val, Card__AllegianceCond  * input__alle);
  ~Card__maxMpEff();
  double dec_prob;
  signed int val;
  Card__AllegianceCond  *alle;
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetLeaderConfig();
  void UntargetedAction(Card  * parent_card);
  

};
class Card__newEff : public Card__BaseUntargetedEff {
public:
  Card__newEff();
  Card__newEff(Card  * input__item, signed int  input__val, Card__Destination  * input__dest, Card__NewCardVariant  * input__variant);
  ~Card__newEff();
  signed int val;
  Card__Destination  *dest;
  Card__NewCardVariant  *variant;
  void FillRep(CardRep  & rep);
  string Detail();
  string Postfix();
  string PostfixIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  void UntargetedAction(Card  * parent_card);
  

};
class Card__winGameEff : public Card__BaseUntargetedEff {
public:
  Card__winGameEff();
  Card__winGameEff(Card  * input__item, Card__AllegianceCond  * input__alle);
  ~Card__winGameEff();
  Card__AllegianceCond  *alle;
  void FillRep(CardRep  & rep);
  string Detail();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  void UntargetedAction(Card  * parent_card);
  

};
class Card__fieldDest : public Card__Destination {
public:
  Card__fieldDest();
  Card__fieldDest(Card  * input__item, Card__AllegianceCond  * input__alle);
  ~Card__fieldDest();
  Card__AllegianceCond  *alle;
  void FillRep(CardRep  & rep);
  string Detail();
  string Suffix();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  void SubTargetedAction(Card  * target, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__ownerFieldDest : public Card__Destination {
public:
  Card__ownerFieldDest();
  Card__ownerFieldDest(Card  * input__item);
  ~Card__ownerFieldDest();
  void FillRep(CardRep  & rep);
  string Detail();
  string Suffix();
  string SuffixAlt1();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  void SubTargetedAction(Card  * target, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__handDest : public Card__Destination {
public:
  Card__handDest();
  Card__handDest(Card  * input__item, Card__AllegianceCond  * input__alle);
  ~Card__handDest();
  Card__AllegianceCond  *alle;
  void FillRep(CardRep  & rep);
  string Detail();
  string Suffix();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  void SubTargetedAction(Card  * target, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__ownerHandDest : public Card__Destination {
public:
  Card__ownerHandDest();
  Card__ownerHandDest(Card  * input__item);
  ~Card__ownerHandDest();
  void FillRep(CardRep  & rep);
  string Detail();
  string Suffix();
  string SuffixAlt1();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  void SubTargetedAction(Card  * target, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__deckDest : public Card__Destination {
public:
  Card__deckDest();
  Card__deckDest(Card  * input__item, Card__AllegianceCond  * input__alle);
  ~Card__deckDest();
  Card__AllegianceCond  *alle;
  void FillRep(CardRep  & rep);
  string Detail();
  string Suffix();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  void SubTargetedAction(Card  * target, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__ownerDeckDest : public Card__Destination {
public:
  Card__ownerDeckDest();
  Card__ownerDeckDest(Card  * input__item);
  ~Card__ownerDeckDest();
  void FillRep(CardRep  & rep);
  string Detail();
  string Suffix();
  string SuffixAlt1();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  void SubTargetedAction(Card  * target, Card  * parent_card, signed int  start_of_batch);
  

};
class Card__plainMinionCard : public Card__NewCardVariant {
public:
  Card__plainMinionCard();
  Card__plainMinionCard(Card  * input__item, Card  * input__card);
  ~Card__plainMinionCard();
  Card  *card;
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  Card  *GetRelatedCard(Card  * parent_card);
  

};
class Card__plainMinion : public Card__NewCardVariant {
public:
  Card__plainMinion();
  Card__plainMinion(Card  * input__item, Card  * input__card);
  ~Card__plainMinion();
  Card  *card;
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  Card  *GetRelatedCard(Card  * parent_card);
  

};
class Card__fixedCard : public Card__NewCardVariant {
public:
  Card__fixedCard();
  Card__fixedCard(Card  * input__item, Card  * input__card);
  ~Card__fixedCard();
  Card  *card;
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  string Postfix();
  string PostfixIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  Card  *GetRelatedCard(Card  * parent_card);
  

};
class Card__fixedMinion : public Card__NewCardVariant {
public:
  Card__fixedMinion();
  Card__fixedMinion(Card  * input__item, Card  * input__card);
  ~Card__fixedMinion();
  Card  *card;
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  string Postfix();
  string PostfixIndent(signed int  indent_size);
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  Card  *GetRelatedCard(Card  * parent_card);
  

};
class Card__randomCard : public Card__NewCardVariant {
public:
  Card__randomCard();
  Card__randomCard(Card  * input__item, signed int  input__cost);
  ~Card__randomCard();
  signed int cost;
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  Card  *GetRelatedCard(Card  * parent_card);
  

};
class Card__randomMinion : public Card__NewCardVariant {
public:
  Card__randomMinion();
  Card__randomMinion(Card  * input__item, signed int  input__cost);
  ~Card__randomMinion();
  signed int cost;
  void FillRep(CardRep  & rep);
  string Detail();
  string DetailAlt1();
  Card__Node  *CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map);
  CondConfig GetTargetConfig();
  Card  *GetRelatedCard(Card  * parent_card);
  

};
Card::Card() : config(), seed(), max_eff_num(), max_eff_depth()
{

  {
    ((contribution) = 0);
    ((is_dying) = 0);
    ((is_resetting) = 0);
    ((replacement) = 0);
    ((is_first_turn_at_field) = 0);
    ((owner) = 0);
    ((opponent) = 0);
    ((card_pos) = (CARD_POS_UNKNOWN));
    ((orig_mana) = ((mana) = (-1)));
    ((orig_atk) = ((atk) = (-1)));
    ((orig_hp) = ((max_hp) = (-1)));
    ((hp_loss) = 0);
    ((orig_n_atks) = ((max_n_atks) = (-1)));
    ((n_atks_loss) = 0);
    ((orig_minion_type) = ((minion_type) = (NONE_MINION)));
    ((orig_is_charge) = ((is_charge) = 0));
    ((orig_is_taunt) = ((is_taunt) = 0));
    ((orig_is_stealth) = ((is_stealth) = 0));
    ((orig_is_untargetable) = ((is_untargetable) = 0));
    ((orig_is_shielded) = ((is_shielded) = 0));
    ((orig_is_poisonous) = ((is_poisonous) = 0));
    ((orig_is_lifesteal) = ((is_lifesteal) = 0));
  }
}

Card::Card(const GiglConfig  & input__config) : config((input__config)), seed((((signed int (*)(const GiglConfig  & ))(((config).funcs)[0]))((config)))), max_eff_num((((signed int (*)(const GiglConfig  & ))(((config).funcs)[1]))((config)))), max_eff_depth((((signed int (*)(const GiglConfig  & ))(((config).funcs)[2]))((config))))
{

  {
    ((contribution) = 0);
    ((is_dying) = 0);
    ((is_resetting) = 0);
    ((replacement) = 0);
    ((is_first_turn_at_field) = 0);
    ((owner) = 0);
    ((opponent) = 0);
    ((card_pos) = (CARD_POS_UNKNOWN));
    ((orig_mana) = ((mana) = (-1)));
    ((orig_atk) = ((atk) = (-1)));
    ((orig_hp) = ((max_hp) = (-1)));
    ((hp_loss) = 0);
    ((orig_n_atks) = ((max_n_atks) = (-1)));
    ((n_atks_loss) = 0);
    ((orig_minion_type) = ((minion_type) = (NONE_MINION)));
    ((orig_is_charge) = ((is_charge) = 0));
    ((orig_is_taunt) = ((is_taunt) = 0));
    ((orig_is_stealth) = ((is_stealth) = 0));
    ((orig_is_untargetable) = ((is_untargetable) = 0));
    ((orig_is_shielded) = ((is_shielded) = 0));
    ((orig_is_poisonous) = ((is_poisonous) = 0));
    ((orig_is_lifesteal) = ((is_lifesteal) = 0));
  }
}

Card::Card(GiglConfig  && input__config) : config(((move)((input__config)))), seed((((signed int (*)(const GiglConfig  & ))(((config).funcs)[0]))((config)))), max_eff_num((((signed int (*)(const GiglConfig  & ))(((config).funcs)[1]))((config)))), max_eff_depth((((signed int (*)(const GiglConfig  & ))(((config).funcs)[2]))((config))))
{

  {
    ((contribution) = 0);
    ((is_dying) = 0);
    ((is_resetting) = 0);
    ((replacement) = 0);
    ((is_first_turn_at_field) = 0);
    ((owner) = 0);
    ((opponent) = 0);
    ((card_pos) = (CARD_POS_UNKNOWN));
    ((orig_mana) = ((mana) = (-1)));
    ((orig_atk) = ((atk) = (-1)));
    ((orig_hp) = ((max_hp) = (-1)));
    ((hp_loss) = 0);
    ((orig_n_atks) = ((max_n_atks) = (-1)));
    ((n_atks_loss) = 0);
    ((orig_minion_type) = ((minion_type) = (NONE_MINION)));
    ((orig_is_charge) = ((is_charge) = 0));
    ((orig_is_taunt) = ((is_taunt) = 0));
    ((orig_is_stealth) = ((is_stealth) = 0));
    ((orig_is_untargetable) = ((is_untargetable) = 0));
    ((orig_is_shielded) = ((is_shielded) = 0));
    ((orig_is_poisonous) = ((is_poisonous) = 0));
    ((orig_is_lifesteal) = ((is_lifesteal) = 0));
  }
}

Card::~Card()
{

  {
    ((ClearExtraEffects)());
  }
  if ((root))
  {
    delete (root);
  } else {
    
  }
}

Card  *Card::Item__Generator()
{

  {
    ((name) = ("#" + ((IntToStr)((seed)))));
    CondConfig tmp_config = ((GetDefaultConfig)());
    ((root) = ((Generate__CardRoot)((tmp_config), 0)));
  }
  ((num_refs) = ((root)->num_refs));
  ((overheat_count) = ((root)->overheat_count));
  ((overheat_threshold) = ((root)->overheat_threshold));
  return (this);
}

Card  *Card::Item__Constructor(Card__Node  * input__root)
{

  ((root) = (input__root));
  ((num_refs) = ((root)->num_refs));
  ((overheat_count) = ((root)->overheat_count));
  ((overheat_threshold) = ((root)->overheat_threshold));
  return (this);
}






void Card::RegisterContribution(signed int  * counter)
{

  {
    ((contribution) = (counter));
  }
}

void Card::IncContribution()
{

  {
    if ((contribution))
    {
      (((*(contribution)))++);
    } else {
      
    }
  }
}

void Card::SetAffiliation(Player  * _owner)
{

  {
    ((owner) = (_owner));
    ((opponent) = ((_owner)->opponent));
  }
}

signed int Card::GetTargetIndex()
{

  {
    if ((((owner)->leader) == (this)))
    {
      return 0;
    } else {
      
    }
    for (signed int i = 0; ((i) < ((((owner)->field).size)())); ((i)++))
    {
      if (((((owner)->field)[(i)]) == (this)))
      {
        return ((i) + 1);
      } else {
        
      }
    }
    for (signed int i = 0; ((i) < ((((owner)->hand).size)())); ((i)++))
    {
      if (((((owner)->hand)[(i)]) == (this)))
      {
        return ((((i) + ((((owner)->field).size)())) + ((((opponent)->field).size)())) + 2);
      } else {
        
      }
    }
    for (signed int i = 0; ((i) < ((((owner)->deck).size)())); ((i)++))
    {
      if (((((owner)->deck)[(i)]) == (this)))
      {
        return (((((i) + ((((owner)->field).size)())) + ((((owner)->hand).size)())) + ((((opponent)->field).size)())) + 2);
      } else {
        
      }
    }
    return (-1);
  }
}

string Card::BriefInfo()
{

  {
    return (((name) + ", ") + (((root)->Brief)()));
  }
}

string Card::DetailInfo()
{

  {
    return ((("Name: " + (name)) + ".\n") + (((root)->DetailIndent)(0)));
  }
}

signed int Card::IsSleeping()
{

  {
    return ((is_first_turn_at_field) && (!(is_charge)));
  }
}

signed int Card::isTargetedAtPlay(signed int  x, signed int  y)
{

  {
    return (((root)->isTargetedAtPlay)((x), (y), (this)));
  }
}

signed int Card::CheckPlayValid(signed int  x, signed int  y, signed int  & z)
{

  {
    return (((root)->CheckPlayValid)((x), (y), (z), (this)));
  }
}

void Card::Play(signed int  x, signed int  y, signed int  z)
{

  {
    (((root)->Play)((x), (y), (z), (this)));
    for (signed int i = 0; ((i) < (((effects_extra).size)())); ((i)++))
    {
      ((((effects_extra)[(i)])->Play)((x), (y), (z), (this)));
    }
  }
}

void Card::Destroy()
{

  {
    (((root)->Destroy)((this)));
    for (signed int i = 0; ((i) < (((effects_extra).size)())); ((i)++))
    {
      ((((effects_extra)[(i)])->Destroy)((this)));
    }
  }
}

void Card::Discard()
{

  {
    (((root)->Discard)((this)));
    for (signed int i = 0; ((i) < (((effects_extra).size)())); ((i)++))
    {
      ((((effects_extra)[(i)])->Discard)((this)));
    }
  }
}

signed int Card::CheckAttackValid(signed int  x, signed int  z)
{

  {
    return (((root)->CheckAttackValid)((x), (z), (this)));
  }
}

void Card::Attack(signed int  x, signed int  z)
{

  {
    (((root)->Attack)((x), (z), (this)));
  }
}

void Card::TurnStart(Card  * leader)
{

  {
    (((root)->TurnStart)((leader), (this)));
    for (signed int i = 0; ((i) < (((effects_extra).size)())); ((i)++))
    {
      ((((effects_extra)[(i)])->TurnStart)((leader), (this)));
    }
  }
}

void Card::TurnEnd(Card  * leader)
{

  {
    (((root)->TurnEnd)((leader), (this)));
    for (signed int i = 0; ((i) < (((effects_extra).size)())); ((i)++))
    {
      ((((effects_extra)[(i)])->TurnEnd)((leader), (this)));
    }
  }
}

void Card::SetAllOverheatCounts(signed int  val)
{

  {
    (((root)->SetOverheatCounts)((val)));
    for (signed int i = 0; ((i) < (((effects_extra).size)())); ((i)++))
    {
      ((((effects_extra)[(i)])->SetOverheatCounts)((val)));
    }
  }
}

void Card::SetAllOverheatThresholds(signed int  val)
{

  {
    (((root)->SetOverheatThresholds)((val)));
    for (signed int i = 0; ((i) < (((effects_extra).size)())); ((i)++))
    {
      ((((effects_extra)[(i)])->SetOverheatThresholds)((val)));
    }
  }
}

void Card::ModAllOverheatThresholds(signed int  amount)
{

  {
    (((root)->ModOverheatThresholds)((amount)));
    for (signed int i = 0; ((i) < (((effects_extra).size)())); ((i)++))
    {
      ((((effects_extra)[(i)])->ModOverheatThresholds)((amount)));
    }
  }
}

void Card::TakeDamage(signed int  amount, Card  * src, signed int  start_of_batch)
{

  {
    if ((is_shielded))
    {
      {
        if (((amount) > 0))
        {
          {
            ((is_shielded) = 0);
          }
        } else {
          
        }
        if (((src) && ((src)->is_poisonous)))
        {
          {
            ((is_shielded) = 0);
          }
        } else {
          
        }
      }
    } else {
      {
        ((hp_loss) += (amount));
        if ((((src) && ((src)->is_lifesteal)) && (!((((src)->owner)->leader)->is_dying))))
        {
          {
            (((((src)->owner)->leader)->RestoreHp)((amount)));
          }
        } else {
          
        }
        if (((hp_loss) >= (max_hp)))
        {
          (((owner)->FlagDestroy)((this), (start_of_batch)));
        } else {
          
        }
        if ((((src) && ((src)->is_poisonous)) && ((card_type) == (MINION_CARD))))
        {
          {
            if ((!(is_dying)))
            {
              (((owner)->FlagDestroy)((this), (start_of_batch)));
            } else {
              
            }
          }
        } else {
          
        }
      }
    }
  }
}

void Card::RestoreHp(signed int  amount)
{

  {
    if (((hp_loss) < (amount)))
    {
      {
        ((hp_loss) = 0);
      }
    } else {
      {
        ((hp_loss) -= (amount));
      }
    }
  }
}

void Card::RestoreAtkTimes(signed int  amount)
{

  {
    if (((n_atks_loss) < (amount)))
    {
      {
        ((n_atks_loss) = 0);
      }
    } else {
      {
        ((n_atks_loss) -= (amount));
      }
    }
  }
}

void Card::ModifyAtkHp(signed int  atkmod, signed int  hpmod)
{

  {
    ((atk) += (atkmod));
    if (((atk) < 0))
    {
      ((atk) = 0);
    } else {
      
    }
    ((max_hp) += (hpmod));
  }
}

void Card::ModifyCost(signed int  amount)
{

  {
    ((mana) += (amount));
    if (((mana) < 0))
    {
      ((mana) = 0);
    } else {
      
    }
  }
}

void Card::ModifyAtkTimes(signed int  amount)
{

  {
    ((max_n_atks) += (amount));
    if (((max_n_atks) < 0))
    {
      ((max_n_atks) = 0);
    } else {
      
    }
  }
}





































void Card::FillRep(CardRep  & rep)
{

  return (((root)->FillRep)((rep)));
}

string Card::Brief()
{

  return (((root)->Brief)());
}

string Card::Detail()
{

  return (((root)->Detail)());
}

string Card::DetailIndent(signed int  indent_size)
{

  return (((root)->DetailIndent)((indent_size)));
}

string Card::DetailAlt1()
{

  return (((root)->DetailAlt1)());
}

string Card::DetailAlt2()
{

  return (((root)->DetailAlt2)());
}

string Card::DetailAlt3()
{

  return (((root)->DetailAlt3)());
}

string Card::DetailAlt4()
{

  return (((root)->DetailAlt4)());
}

string Card::DetailAlt5()
{

  return (((root)->DetailAlt5)());
}

string Card::DetailAlt6()
{

  return (((root)->DetailAlt6)());
}

string Card::Suffix()
{

  return (((root)->Suffix)());
}

string Card::SuffixAlt1()
{

  return (((root)->SuffixAlt1)());
}

string Card::Postfix()
{

  return (((root)->Postfix)());
}

string Card::PostfixIndent(signed int  indent_size)
{

  return (((root)->PostfixIndent)((indent_size)));
}

signed int Card::IsPlural()
{

  return (((root)->IsPlural)());
}

signed int Card::IsThirdPersonSingle()
{

  return (((root)->IsThirdPersonSingle)());
}

Card__Node  *Card::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (((root)->CreateNodeHardCopy)((card_copy), (redir_map)));
}

signed int Card::GetEffectNum()
{

  return (((root)->GetEffectNum)());
}

Card__SpecialEffects  *Card::GetEffects()
{

  return (((root)->GetEffects)());
}

Card  *Card::GetRelatedCard(Card  * parent_card)
{

  return (((root)->GetRelatedCard)((parent_card)));
}

signed int Card::isCondTrivial()
{

  return (((root)->isCondTrivial)());
}

unsigned int Card::GetInitAttrFlag()
{

  return (((root)->GetInitAttrFlag)());
}

CondConfig Card::GetPlayTargetConfig()
{

  return (((root)->GetPlayTargetConfig)());
}

CondConfig Card::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  return (((root)->GetGlobalSelfConfig)((self_config), (effect_timing)));
}

CondConfig Card::GetSelfConfig(const CondConfig  & self_config)
{

  return (((root)->GetSelfConfig)((self_config)));
}

CondConfig Card::GetLeaderConfig()
{

  return (((root)->GetLeaderConfig)());
}

CondConfig Card::GetTargetConfig()
{

  return (((root)->GetTargetConfig)());
}

void Card::AdjustGlobalStatRange(signed int  & min_val, signed int  & max_val)
{

  return (((root)->AdjustGlobalStatRange)((min_val), (max_val)));
}

signed int Card::isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card)
{

  return (((root)->isTargetedAtPlay)((x), (y), (parent_card)));
}

signed int Card::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return (((root)->CheckPlayValid)((x), (y), (z), (parent_card)));
}

void Card::Play(signed int  x, signed int  y, signed int  z, Card  * parent_card)
{

  return (((root)->Play)((x), (y), (z), (parent_card)));
}

void Card::Destroy(Card  * parent_card)
{

  return (((root)->Destroy)((parent_card)));
}

void Card::Discard(Card  * parent_card)
{

  return (((root)->Discard)((parent_card)));
}

signed int Card::CheckAttackValid(signed int  x, signed int  z, Card  * parent_card)
{

  return (((root)->CheckAttackValid)((x), (z), (parent_card)));
}

void Card::Attack(signed int  x, signed int  z, Card  * parent_card)
{

  return (((root)->Attack)((x), (z), (parent_card)));
}

void Card::TurnStart(Card  * leader, Card  * parent_card)
{

  return (((root)->TurnStart)((leader), (parent_card)));
}

void Card::TurnEnd(Card  * leader, Card  * parent_card)
{

  return (((root)->TurnEnd)((leader), (parent_card)));
}

signed int Card::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  return (((root)->TargetedAction)((z), (parent_card), (start_of_batch)));
}

void Card::SubTargetedAction(Card  * target, Card  * parent_card, signed int  start_of_batch)
{

  return (((root)->SubTargetedAction)((target), (parent_card), (start_of_batch)));
}

void Card::UntargetedAction(Card  * parent_card)
{

  return (((root)->UntargetedAction)((parent_card)));
}

signed int Card::CheckCardValid(Card  * card, Card  * parent_card)
{

  return (((root)->CheckCardValid)((card), (parent_card)));
}

signed int Card::CheckThisValid(Card  * parent_card)
{

  return (((root)->CheckThisValid)((parent_card)));
}

signed int Card::CheckStatValid(signed int  stat_val)
{

  return (((root)->CheckStatValid)((stat_val)));
}

void Card::Mutate(signed int  min_eff_n, signed int  max_eff_n, signed int  effect_depth)
{

  return (((root)->Mutate)((min_eff_n), (max_eff_n), (effect_depth)));
}

void Card::SetOverheatCounts(signed int  val)
{

  return (((root)->SetOverheatCounts)((val)));
}

void Card::SetOverheatThresholds(signed int  val)
{

  return (((root)->SetOverheatThresholds)((val)));
}

void Card::ModOverheatThresholds(signed int  amount)
{

  return (((root)->ModOverheatThresholds)((amount)));
}





void Card::AddExtraEffects(Card__SpecialEffects  * effects)
{

  {
    (((effects_extra).push_back)((effects)));
    (((effects)->num_refs)++);
  }
}

void Card::ClearExtraEffects()
{

  {
    for (signed int i = 0; ((i) < (((effects_extra).size)())); ((i)++))
    {
      {
        ((((effects_extra)[(i)])->num_refs)--);
        if (((((effects_extra)[(i)])->num_refs) <= 0))
        {
          delete ((effects_extra)[(i)]);
        } else {
          
        }
      }
    }
    (((effects_extra).clear)());
  }
}

string Card::GetExtraEffectsBrief()
{

  {
    string tmp_str = "";
    for (signed int i = 0; ((i) < (((effects_extra).size)())); ((i)++))
    {
      ((tmp_str) += ((((effects_extra)[(i)])->Brief)()));
    }
    return (tmp_str);
  }
}

string Card::GetExtraEffectsDetail()
{

  {
    if (((((effects_extra).size)()) == 0))
    {
      return "";
    } else {
      
    }
    string tmp_str = "Extra Effects:\n";
    for (signed int i = 0; ((i) < (((effects_extra).size)())); ((i)++))
    {
      ((tmp_str) += (((((effects_extra)[(i)])->Detail)()) + "\n"));
    }
    return (tmp_str);
  }
}

string Card::GetExtraEffectsDetailIndent(signed int  indent_size)
{

  {
    if (((((effects_extra).size)()) == 0))
    {
      return "";
    } else {
      
    }
    string tmp_str = (((RepeatSpace)((indent_size))) + "Extra Effects:\n");
    for (signed int i = 0; ((i) < (((effects_extra).size)())); ((i)++))
    {
      ((tmp_str) += ((((effects_extra)[(i)])->DetailIndent)((indent_size))));
    }
    return (tmp_str);
  }
}

Card  *Card::CreateHardCopy(PtrRedirMap  & redir_map)
{

  {
    Card  *card_copy;
    if ((((config).IsEmpty)()))
    {
      ((card_copy) = (new Card()));
    } else {
      ((card_copy) = (new Card((config))));
    }
    (((card_copy)->root) = (((root)->CreateNodeHardCopy)((card_copy), (redir_map))));
    (((card_copy)->name) = (name));
    (((card_copy)->is_first_turn_at_field) = (is_first_turn_at_field));
    (((card_copy)->card_pos) = (card_pos));
    (((card_copy)->mana) = (mana));
    (((card_copy)->atk) = (atk));
    (((card_copy)->max_hp) = (max_hp));
    (((card_copy)->hp_loss) = (hp_loss));
    (((card_copy)->max_n_atks) = (max_n_atks));
    (((card_copy)->n_atks_loss) = (n_atks_loss));
    (((card_copy)->minion_type) = (minion_type));
    (((card_copy)->is_charge) = (is_charge));
    (((card_copy)->is_taunt) = (is_taunt));
    (((card_copy)->is_stealth) = (is_stealth));
    (((card_copy)->is_untargetable) = (is_untargetable));
    (((card_copy)->is_shielded) = (is_shielded));
    (((card_copy)->is_poisonous) = (is_poisonous));
    (((card_copy)->is_lifesteal) = (is_lifesteal));
    for (signed int i = 0; ((i) < (((effects_extra).size)())); ((i)++))
    {
      ((((card_copy)->effects_extra).push_back)(((Card__SpecialEffects *)(((((effects_extra)[(i)])->CreateNodeHardCopy)((card_copy), (redir_map)))))));
    }
    return (card_copy);
  }
}

Card  *Card::CreateCopy()
{

  {
    Card  *card_copy;
    Card__SpecialEffects  *effects = (((root)->GetEffects)());
    switch ((card_type))
    {
      {
        case (LEADER_CARD):
          ((card_copy) = (((Card__ItemRef__532__54) = (new Card(((GetDefaultGenConfig)(((GetRandInt)())))))) , (((Card__ItemRef__532__54)->Item__Constructor)((new Card__leaderCard((Card__ItemRef__532__54), (orig_mana), (orig_atk), (orig_hp), (new Card__multipleAttack((Card__ItemRef__532__54), (orig_n_atks))), (new Card__justAttributes((Card__ItemRef__532__54), (((orig_is_charge) ? ((Card__ChargeAttr *)(new Card__justCharge((Card__ItemRef__532__54)))) : ((Card__ChargeAttr *)(new Card__noCharge((Card__ItemRef__532__54)))))), (((orig_is_taunt) ? ((Card__TauntAttr *)(new Card__justTaunt((Card__ItemRef__532__54)))) : ((Card__TauntAttr *)(new Card__noTaunt((Card__ItemRef__532__54)))))), (((orig_is_stealth) ? ((Card__StealthAttr *)(new Card__justStealth((Card__ItemRef__532__54)))) : ((Card__StealthAttr *)(new Card__noStealth((Card__ItemRef__532__54)))))), (((orig_is_untargetable) ? ((Card__UntargetableAttr *)(new Card__justUntargetable((Card__ItemRef__532__54)))) : ((Card__UntargetableAttr *)(new Card__noUntargetable((Card__ItemRef__532__54)))))), (((orig_is_shielded) ? ((Card__ShieldAttr *)(new Card__justShield((Card__ItemRef__532__54)))) : ((Card__ShieldAttr *)(new Card__noShield((Card__ItemRef__532__54)))))), (((orig_is_poisonous) ? ((Card__PoisonousAttr *)(new Card__justPoisonous((Card__ItemRef__532__54)))) : ((Card__PoisonousAttr *)(new Card__noPoisonous((Card__ItemRef__532__54)))))), (((orig_is_lifesteal) ? ((Card__LifestealAttr *)(new Card__justLifesteal((Card__ItemRef__532__54)))) : ((Card__LifestealAttr *)(new Card__noLifesteal((Card__ItemRef__532__54)))))))), (effects)))))));
        
        break;
        case (MINION_CARD):
          ((card_copy) = (((Card__ItemRef__544__54) = (new Card(((GetDefaultGenConfig)(((GetRandInt)())))))) , (((Card__ItemRef__544__54)->Item__Constructor)((new Card__minionCard((Card__ItemRef__544__54), (orig_mana), (orig_atk), (orig_hp), (new Card__multipleAttack((Card__ItemRef__544__54), (orig_n_atks))), ((((orig_minion_type) == (BEAST_MINION)) ? ((Card__MinionType *)(new Card__beastMinion((Card__ItemRef__544__54)))) : ((((orig_minion_type) == (DRAGON_MINION)) ? ((Card__MinionType *)(new Card__dragonMinion((Card__ItemRef__544__54)))) : ((Card__MinionType *)(new Card__demonMinion((Card__ItemRef__544__54)))))))), (new Card__justAttributes((Card__ItemRef__544__54), (((orig_is_charge) ? ((Card__ChargeAttr *)(new Card__justCharge((Card__ItemRef__544__54)))) : ((Card__ChargeAttr *)(new Card__noCharge((Card__ItemRef__544__54)))))), (((orig_is_taunt) ? ((Card__TauntAttr *)(new Card__justTaunt((Card__ItemRef__544__54)))) : ((Card__TauntAttr *)(new Card__noTaunt((Card__ItemRef__544__54)))))), (((orig_is_stealth) ? ((Card__StealthAttr *)(new Card__justStealth((Card__ItemRef__544__54)))) : ((Card__StealthAttr *)(new Card__noStealth((Card__ItemRef__544__54)))))), (((orig_is_untargetable) ? ((Card__UntargetableAttr *)(new Card__justUntargetable((Card__ItemRef__544__54)))) : ((Card__UntargetableAttr *)(new Card__noUntargetable((Card__ItemRef__544__54)))))), (((orig_is_shielded) ? ((Card__ShieldAttr *)(new Card__justShield((Card__ItemRef__544__54)))) : ((Card__ShieldAttr *)(new Card__noShield((Card__ItemRef__544__54)))))), (((orig_is_poisonous) ? ((Card__PoisonousAttr *)(new Card__justPoisonous((Card__ItemRef__544__54)))) : ((Card__PoisonousAttr *)(new Card__noPoisonous((Card__ItemRef__544__54)))))), (((orig_is_lifesteal) ? ((Card__LifestealAttr *)(new Card__justLifesteal((Card__ItemRef__544__54)))) : ((Card__LifestealAttr *)(new Card__noLifesteal((Card__ItemRef__544__54)))))))), (effects)))))));
        
        break;
        default:
          ((card_copy) = (((Card__ItemRef__557__54) = (new Card(((GetDefaultGenConfig)(((GetRandInt)())))))) , (((Card__ItemRef__557__54)->Item__Constructor)((new Card__spellCard((Card__ItemRef__557__54), (orig_mana), (new Card__justAttributes((Card__ItemRef__557__54), (((orig_is_charge) ? ((Card__ChargeAttr *)(new Card__justCharge((Card__ItemRef__557__54)))) : ((Card__ChargeAttr *)(new Card__noCharge((Card__ItemRef__557__54)))))), (((orig_is_taunt) ? ((Card__TauntAttr *)(new Card__justTaunt((Card__ItemRef__557__54)))) : ((Card__TauntAttr *)(new Card__noTaunt((Card__ItemRef__557__54)))))), (((orig_is_stealth) ? ((Card__StealthAttr *)(new Card__justStealth((Card__ItemRef__557__54)))) : ((Card__StealthAttr *)(new Card__noStealth((Card__ItemRef__557__54)))))), (((orig_is_untargetable) ? ((Card__UntargetableAttr *)(new Card__justUntargetable((Card__ItemRef__557__54)))) : ((Card__UntargetableAttr *)(new Card__noUntargetable((Card__ItemRef__557__54)))))), (((orig_is_shielded) ? ((Card__ShieldAttr *)(new Card__justShield((Card__ItemRef__557__54)))) : ((Card__ShieldAttr *)(new Card__noShield((Card__ItemRef__557__54)))))), (((orig_is_poisonous) ? ((Card__PoisonousAttr *)(new Card__justPoisonous((Card__ItemRef__557__54)))) : ((Card__PoisonousAttr *)(new Card__noPoisonous((Card__ItemRef__557__54)))))), (((orig_is_lifesteal) ? ((Card__LifestealAttr *)(new Card__justLifesteal((Card__ItemRef__557__54)))) : ((Card__LifestealAttr *)(new Card__noLifesteal((Card__ItemRef__557__54)))))))), (effects)))))));
        
        break;
      }
    }
    (((effects)->num_refs)++);
    (((card_copy)->contribution) = (contribution));
    (((card_copy)->name) = (name));
    (((card_copy)->owner) = (owner));
    (((card_copy)->opponent) = (opponent));
    (((card_copy)->card_pos) = (card_pos));
    (((card_copy)->mana) = (mana));
    (((card_copy)->atk) = (atk));
    (((card_copy)->max_hp) = (max_hp));
    (((card_copy)->hp_loss) = (hp_loss));
    (((card_copy)->max_n_atks) = (max_n_atks));
    (((card_copy)->minion_type) = (minion_type));
    (((card_copy)->is_charge) = (is_charge));
    (((card_copy)->is_taunt) = (is_taunt));
    (((card_copy)->is_stealth) = (is_stealth));
    (((card_copy)->is_untargetable) = (is_untargetable));
    (((card_copy)->is_shielded) = (is_shielded));
    (((card_copy)->is_poisonous) = (is_poisonous));
    (((card_copy)->is_lifesteal) = (is_lifesteal));
    for (signed int i = 0; ((i) < (((effects_extra).size)())); ((i)++))
    {
      (((card_copy)->AddExtraEffects)(((effects_extra)[(i)])));
    }
    return (card_copy);
  }
}

Card__CardRoot  *Card::Generate__CardRoot(CondConfig  & global_config, signed int  is_plain)
{

  double probs[3] = {(((double (*)(const GiglConfig  & , CondConfig  & , signed int  ))(((config).funcs)[3]))((config), (global_config), (is_plain))), (((double (*)(const GiglConfig  & , CondConfig  & , signed int  ))(((config).funcs)[4]))((config), (global_config), (is_plain))), (((double (*)(const GiglConfig  & , CondConfig  & , signed int  ))(((config).funcs)[5]))((config), (global_config), (is_plain)))};
  ((NonegativeProbs)((probs), 3));
  {
    if ((!(((global_config) & (TARGET_IS_LEADER)))))
    {
      (((probs)[0]) = 0.0);
      
    } else {
      
    }
    if ((!(((global_config) & (TARGET_IS_MINION)))))
    {
      (((probs)[1]) = 0.0);
      
    } else {
      
    }
    if ((!(((global_config) & (TARGET_IS_SPELL)))))
    {
      (((probs)[2]) = 0.0);
      
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 3));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 3));
  switch ((choice))
  {
    case 0:
      return ((Generate__leaderCard)((global_config), (is_plain)));
    
    case 1:
      return ((Generate__minionCard)((global_config), (is_plain)));
    
    case 2:
      return ((Generate__spellCard)((global_config), (is_plain)));
    
  }
  return 0;
}

Card__leaderCard  *Card::Generate__leaderCard(CondConfig  & global_config, signed int  is_plain)
{

  Card__leaderCard  *node = (new Card__leaderCard);
  (((node)->item) = (this));
  
  
  
  
  
  
  ((card_type) = (LEADER_CARD));
  {
    (((node)->cost) = ((GetRandInt)(((global_config).min_cost), ((global_config).max_cost))));
    (((node)->attack) = ((GetRandInt)(0, 10)));
    (((node)->health) = ((GetRandInt)(10, 40)));
  }
  ((orig_mana) = ((mana) = ((node)->cost)));
  ((orig_atk) = ((atk) = ((node)->attack)));
  ((orig_hp) = ((max_hp) = ((node)->health)));
  {
    CondConfig self_config = ((GetInitConfigFromCard)((this)));
    (((node)->attributes) = ((Generate__Attributes)((self_config), (atk))));
    if ((is_plain))
    {
      {
        (((node)->attack_times) = (new Card__singleAttack((this))));
        (((node)->effects) = (new Card__specialEffects((this), (new Card__noTargetedPlayEff((this))), (new Card__noOtherEffs((this))))));
      }
    } else {
      {
        (((node)->attack_times) = ((Generate__AttackTimes)()));
        ((self_config) = ((GetInitConfigFromCard)((this))));
        (((node)->effects) = ((Generate__SpecialEffects)((self_config), 0, (max_eff_num), 0, 0)));
      }
    }
  }
  return (node);
}

Card__minionCard  *Card::Generate__minionCard(CondConfig  & global_config, signed int  is_plain)
{

  Card__minionCard  *node = (new Card__minionCard);
  (((node)->item) = (this));
  
  
  
  
  
  
  (((node)->type) = ((Generate__MinionType)()));
  
  ((card_type) = (MINION_CARD));
  {
    (((node)->cost) = ((GetRandInt)(((global_config).min_cost), ((global_config).max_cost))));
    (((node)->attack) = ((GetRandInt)(0, 10)));
    (((node)->health) = ((GetRandInt)(1, 10)));
  }
  ((orig_mana) = ((mana) = ((node)->cost)));
  ((orig_atk) = ((atk) = ((node)->attack)));
  ((orig_hp) = ((max_hp) = ((node)->health)));
  {
    CondConfig self_config = ((GetInitConfigFromCard)((this)));
    (((node)->attributes) = ((Generate__Attributes)((self_config), (atk))));
    if ((is_plain))
    {
      {
        (((node)->attack_times) = (new Card__singleAttack((this))));
        (((node)->effects) = (new Card__specialEffects((this), (new Card__noTargetedPlayEff((this))), (new Card__noOtherEffs((this))))));
      }
    } else {
      {
        (((node)->attack_times) = ((Generate__AttackTimes)()));
        ((self_config) = ((GetInitConfigFromCard)((this))));
        (((node)->effects) = ((Generate__SpecialEffects)((self_config), 0, (max_eff_num), 0, 0)));
      }
    }
  }
  return (node);
}

Card__spellCard  *Card::Generate__spellCard(CondConfig  & global_config, signed int  is_plain)
{

  Card__spellCard  *node = (new Card__spellCard);
  (((node)->item) = (this));
  
  
  
  ((card_type) = (SPELL_CARD));
  {
    (((node)->cost) = ((GetRandInt)(((global_config).min_cost), ((global_config).max_cost))));
  }
  ((orig_mana) = ((mana) = ((node)->cost)));
  {
    CondConfig self_config = ((GetInitConfigFromCard)((this)));
    (((node)->attributes) = ((Generate__Attributes)((self_config), (-1))));
    if ((is_plain))
    {
      (((node)->effects) = (new Card__specialEffects((this), (new Card__noTargetedPlayEff((this))), (new Card__noOtherEffs((this))))));
    } else {
      (((node)->effects) = ((Generate__SpecialEffects)((self_config), 1, (max_eff_num), 0, 0)));
    }
  }
  return (node);
}

Card__AttackTimes  *Card::Generate__AttackTimes(void)
{

  double probs[3] = {(((double (*)(const GiglConfig  & ))(((config).funcs)[6]))((config))), (((double (*)(const GiglConfig  & ))(((config).funcs)[7]))((config))), (((double (*)(const GiglConfig  & ))(((config).funcs)[8]))((config)))};
  ((NonegativeProbs)((probs), 3));
  
  ((NormalizeProbs)((probs), 3));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 3));
  switch ((choice))
  {
    case 0:
      return ((Generate__zeroAttack)());
    
    case 1:
      return ((Generate__singleAttack)());
    
    case 2:
      return ((Generate__multipleAttack)());
    
  }
  return 0;
}

Card__zeroAttack  *Card::Generate__zeroAttack(void)
{

  Card__zeroAttack  *node = (new Card__zeroAttack);
  (((node)->item) = (this));
  ((orig_n_atks) = ((max_n_atks) = 0));
  return (node);
}

Card__singleAttack  *Card::Generate__singleAttack(void)
{

  Card__singleAttack  *node = (new Card__singleAttack);
  (((node)->item) = (this));
  ((orig_n_atks) = ((max_n_atks) = 1));
  return (node);
}

Card__multipleAttack  *Card::Generate__multipleAttack(void)
{

  Card__multipleAttack  *node = (new Card__multipleAttack);
  (((node)->item) = (this));
  
  {
    (((node)->n_atks) = (100 / ((GetRandInt)(17, 50))));
  }
  ((n_atks_loss) = 0);
  ((orig_n_atks) = ((max_n_atks) = ((node)->n_atks)));
  return (node);
}

Card__MinionType  *Card::Generate__MinionType(void)
{

  double probs[3] = {(((double (*)(const GiglConfig  & ))(((config).funcs)[9]))((config))), (((double (*)(const GiglConfig  & ))(((config).funcs)[10]))((config))), (((double (*)(const GiglConfig  & ))(((config).funcs)[11]))((config)))};
  ((NonegativeProbs)((probs), 3));
  
  ((NormalizeProbs)((probs), 3));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 3));
  switch ((choice))
  {
    case 0:
      return ((Generate__beastMinion)());
    
    case 1:
      return ((Generate__dragonMinion)());
    
    case 2:
      return ((Generate__demonMinion)());
    
  }
  return 0;
}

Card__beastMinion  *Card::Generate__beastMinion(void)
{

  Card__beastMinion  *node = (new Card__beastMinion);
  (((node)->item) = (this));
  ((orig_minion_type) = ((minion_type) = (BEAST_MINION)));
  return (node);
}

Card__dragonMinion  *Card::Generate__dragonMinion(void)
{

  Card__dragonMinion  *node = (new Card__dragonMinion);
  (((node)->item) = (this));
  ((orig_minion_type) = ((minion_type) = (DRAGON_MINION)));
  return (node);
}

Card__demonMinion  *Card::Generate__demonMinion(void)
{

  Card__demonMinion  *node = (new Card__demonMinion);
  (((node)->item) = (this));
  ((orig_minion_type) = ((minion_type) = (DEMON_MINION)));
  return (node);
}

Card__Attributes  *Card::Generate__Attributes(CondConfig  & self_config, signed int  damage)
{

  double probs[1] = {(((double (*)(const GiglConfig  & , CondConfig  & , signed int  ))(((config).funcs)[12]))((config), (self_config), (damage)))};
  ((NonegativeProbs)((probs), 1));
  
  ((NormalizeProbs)((probs), 1));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 1));
  switch ((choice))
  {
    case 0:
      return ((Generate__justAttributes)((self_config), (damage)));
    
  }
  return 0;
}

Card__justAttributes  *Card::Generate__justAttributes(CondConfig  & self_config, signed int  damage)
{

  Card__justAttributes  *node = (new Card__justAttributes);
  (((node)->item) = (this));
  
  
  
  
  
  
  
  {
    (((node)->c) = ((Generate__ChargeAttr)((self_config))));
    (((node)->t) = ((Generate__TauntAttr)((self_config))));
    (((node)->s) = ((Generate__StealthAttr)((self_config))));
    (((node)->u) = ((Generate__UntargetableAttr)((self_config))));
    (((node)->d) = ((Generate__ShieldAttr)((self_config))));
    (((node)->p) = ((Generate__PoisonousAttr)((self_config), (damage))));
    (((node)->l) = ((Generate__LifestealAttr)((self_config), (damage))));
  }
  return (node);
}

Card__DamageAttributes  *Card::Generate__DamageAttributes(CondConfig  & self_config, unsigned int  target_mode, signed int  damage)
{

  double probs[1] = {(((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , signed int  ))(((config).funcs)[13]))((config), (self_config), (target_mode), (damage)))};
  ((NonegativeProbs)((probs), 1));
  
  ((NormalizeProbs)((probs), 1));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 1));
  switch ((choice))
  {
    case 0:
      return ((Generate__damageAttributes)((self_config), (target_mode), (damage)));
    
  }
  return 0;
}

Card__damageAttributes  *Card::Generate__damageAttributes(CondConfig  & self_config, unsigned int  target_mode, signed int  damage)
{

  Card__damageAttributes  *node = (new Card__damageAttributes);
  (((node)->item) = (this));
  
  
  {
    if ((((target_mode) == (TARGET_MODE_LEADER)) || ((((target_mode) == (TARGET_MODE_SELF)) && (!(((self_config) & (TARGET_NOT_LEADER))))))))
    {
      (((node)->p) = (new Card__noPoisonous((this))));
    } else {
      (((node)->p) = ((Generate__PoisonousAttr)((self_config), (damage))));
    }
    (((node)->l) = ((Generate__LifestealAttr)((self_config), (damage))));
  }
  return (node);
}

Card__ChargeAttr  *Card::Generate__ChargeAttr(CondConfig  & self_config)
{

  double probs[2] = {(((double (*)(const GiglConfig  & , CondConfig  & ))(((config).funcs)[14]))((config), (self_config))), (((double (*)(const GiglConfig  & , CondConfig  & ))(((config).funcs)[15]))((config), (self_config)))};
  ((NonegativeProbs)((probs), 2));
  {
    if (((self_config) & (TARGET_IS_SPELL)))
    {
      return ((Generate__noCharge)((self_config)));
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 2));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 2));
  switch ((choice))
  {
    case 0:
      return ((Generate__noCharge)((self_config)));
    
    case 1:
      return ((Generate__justCharge)((self_config)));
    
  }
  return 0;
}

Card__noCharge  *Card::Generate__noCharge(CondConfig  & self_config)
{

  Card__noCharge  *node = (new Card__noCharge);
  (((node)->item) = (this));
  
  return (node);
}

Card__justCharge  *Card::Generate__justCharge(CondConfig  & self_config)
{

  Card__justCharge  *node = (new Card__justCharge);
  (((node)->item) = (this));
  ((orig_is_charge) = ((is_charge) = 1));
  {
    ((self_config) |= (TARGET_IS_CHARGE));
  }
  return (node);
}

Card__TauntAttr  *Card::Generate__TauntAttr(CondConfig  & self_config)
{

  double probs[2] = {(((double (*)(const GiglConfig  & , CondConfig  & ))(((config).funcs)[16]))((config), (self_config))), (((double (*)(const GiglConfig  & , CondConfig  & ))(((config).funcs)[17]))((config), (self_config)))};
  ((NonegativeProbs)((probs), 2));
  {
    if (((self_config) & (TARGET_IS_SPELL)))
    {
      return ((Generate__noTaunt)((self_config)));
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 2));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 2));
  switch ((choice))
  {
    case 0:
      return ((Generate__noTaunt)((self_config)));
    
    case 1:
      return ((Generate__justTaunt)((self_config)));
    
  }
  return 0;
}

Card__noTaunt  *Card::Generate__noTaunt(CondConfig  & self_config)
{

  Card__noTaunt  *node = (new Card__noTaunt);
  (((node)->item) = (this));
  
  return (node);
}

Card__justTaunt  *Card::Generate__justTaunt(CondConfig  & self_config)
{

  Card__justTaunt  *node = (new Card__justTaunt);
  (((node)->item) = (this));
  ((orig_is_taunt) = ((is_taunt) = 1));
  {
    ((self_config) |= (TARGET_IS_TAUNT));
  }
  return (node);
}

Card__StealthAttr  *Card::Generate__StealthAttr(CondConfig  & self_config)
{

  double probs[2] = {(((double (*)(const GiglConfig  & , CondConfig  & ))(((config).funcs)[18]))((config), (self_config))), (((double (*)(const GiglConfig  & , CondConfig  & ))(((config).funcs)[19]))((config), (self_config)))};
  ((NonegativeProbs)((probs), 2));
  {
    if (((self_config) & (((TARGET_IS_LEADER) | (TARGET_IS_SPELL)))))
    {
      return ((Generate__noStealth)((self_config)));
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 2));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 2));
  switch ((choice))
  {
    case 0:
      return ((Generate__noStealth)((self_config)));
    
    case 1:
      return ((Generate__justStealth)((self_config)));
    
  }
  return 0;
}

Card__noStealth  *Card::Generate__noStealth(CondConfig  & self_config)
{

  Card__noStealth  *node = (new Card__noStealth);
  (((node)->item) = (this));
  
  return (node);
}

Card__justStealth  *Card::Generate__justStealth(CondConfig  & self_config)
{

  Card__justStealth  *node = (new Card__justStealth);
  (((node)->item) = (this));
  ((orig_is_stealth) = ((is_stealth) = 1));
  {
    ((self_config) |= (TARGET_IS_STEALTH));
  }
  return (node);
}

Card__UntargetableAttr  *Card::Generate__UntargetableAttr(CondConfig  & self_config)
{

  double probs[2] = {(((double (*)(const GiglConfig  & , CondConfig  & ))(((config).funcs)[20]))((config), (self_config))), (((double (*)(const GiglConfig  & , CondConfig  & ))(((config).funcs)[21]))((config), (self_config)))};
  ((NonegativeProbs)((probs), 2));
  
  ((NormalizeProbs)((probs), 2));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 2));
  switch ((choice))
  {
    case 0:
      return ((Generate__noUntargetable)((self_config)));
    
    case 1:
      return ((Generate__justUntargetable)((self_config)));
    
  }
  return 0;
}

Card__noUntargetable  *Card::Generate__noUntargetable(CondConfig  & self_config)
{

  Card__noUntargetable  *node = (new Card__noUntargetable);
  (((node)->item) = (this));
  
  return (node);
}

Card__justUntargetable  *Card::Generate__justUntargetable(CondConfig  & self_config)
{

  Card__justUntargetable  *node = (new Card__justUntargetable);
  (((node)->item) = (this));
  ((orig_is_untargetable) = ((is_untargetable) = 1));
  {
    ((self_config) |= (TARGET_IS_UNTARGETABLE));
  }
  return (node);
}

Card__ShieldAttr  *Card::Generate__ShieldAttr(CondConfig  & self_config)
{

  double probs[2] = {(((double (*)(const GiglConfig  & , CondConfig  & ))(((config).funcs)[22]))((config), (self_config))), (((double (*)(const GiglConfig  & , CondConfig  & ))(((config).funcs)[23]))((config), (self_config)))};
  ((NonegativeProbs)((probs), 2));
  {
    if (((self_config) & (TARGET_IS_SPELL)))
    {
      return ((Generate__noShield)((self_config)));
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 2));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 2));
  switch ((choice))
  {
    case 0:
      return ((Generate__noShield)((self_config)));
    
    case 1:
      return ((Generate__justShield)((self_config)));
    
  }
  return 0;
}

Card__noShield  *Card::Generate__noShield(CondConfig  & self_config)
{

  Card__noShield  *node = (new Card__noShield);
  (((node)->item) = (this));
  
  return (node);
}

Card__justShield  *Card::Generate__justShield(CondConfig  & self_config)
{

  Card__justShield  *node = (new Card__justShield);
  (((node)->item) = (this));
  ((orig_is_shielded) = ((is_shielded) = 1));
  {
    ((self_config) |= (TARGET_IS_SHIELDED));
  }
  return (node);
}

Card__PoisonousAttr  *Card::Generate__PoisonousAttr(CondConfig  & self_config, signed int  damage)
{

  double probs[2] = {(((double (*)(const GiglConfig  & , CondConfig  & , signed int  ))(((config).funcs)[24]))((config), (self_config), (damage))), (((double (*)(const GiglConfig  & , CondConfig  & , signed int  ))(((config).funcs)[25]))((config), (self_config), (damage)))};
  ((NonegativeProbs)((probs), 2));
  {
    if ((((damage) < 0) || (((self_config) & (TARGET_IS_POISONOUS)))))
    {
      return ((Generate__noPoisonous)((self_config), (damage)));
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 2));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 2));
  switch ((choice))
  {
    case 0:
      return ((Generate__noPoisonous)((self_config), (damage)));
    
    case 1:
      return ((Generate__justPoisonous)((self_config), (damage)));
    
  }
  return 0;
}

Card__noPoisonous  *Card::Generate__noPoisonous(CondConfig  & self_config, signed int  damage)
{

  Card__noPoisonous  *node = (new Card__noPoisonous);
  (((node)->item) = (this));
  
  return (node);
}

Card__justPoisonous  *Card::Generate__justPoisonous(CondConfig  & self_config, signed int  damage)
{

  Card__justPoisonous  *node = (new Card__justPoisonous);
  (((node)->item) = (this));
  ((orig_is_poisonous) = ((is_poisonous) = 1));
  {
    ((self_config) |= (TARGET_IS_POISONOUS));
  }
  return (node);
}

Card__LifestealAttr  *Card::Generate__LifestealAttr(CondConfig  & self_config, signed int  damage)
{

  double probs[2] = {(((double (*)(const GiglConfig  & , CondConfig  & , signed int  ))(((config).funcs)[26]))((config), (self_config), (damage))), (((double (*)(const GiglConfig  & , CondConfig  & , signed int  ))(((config).funcs)[27]))((config), (self_config), (damage)))};
  ((NonegativeProbs)((probs), 2));
  {
    if ((((damage) < 0) || (((self_config) & (TARGET_IS_LIFESTEAL)))))
    {
      return ((Generate__noLifesteal)((self_config), (damage)));
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 2));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 2));
  switch ((choice))
  {
    case 0:
      return ((Generate__noLifesteal)((self_config), (damage)));
    
    case 1:
      return ((Generate__justLifesteal)((self_config), (damage)));
    
  }
  return 0;
}

Card__noLifesteal  *Card::Generate__noLifesteal(CondConfig  & self_config, signed int  damage)
{

  Card__noLifesteal  *node = (new Card__noLifesteal);
  (((node)->item) = (this));
  
  return (node);
}

Card__justLifesteal  *Card::Generate__justLifesteal(CondConfig  & self_config, signed int  damage)
{

  Card__justLifesteal  *node = (new Card__justLifesteal);
  (((node)->item) = (this));
  ((orig_is_lifesteal) = ((is_lifesteal) = 1));
  {
    ((self_config) |= (TARGET_IS_LIFESTEAL));
  }
  return (node);
}

Card__SpecialEffects  *Card::Generate__SpecialEffects(CondConfig  & self_config, signed int  min_n, signed int  max_n, signed int  effect_depth, signed int  give_eff)
{

  double probs[1] = {(((double (*)(const GiglConfig  & , CondConfig  & , signed int  , signed int  , signed int  , signed int  ))(((config).funcs)[28]))((config), (self_config), (min_n), (max_n), (effect_depth), (give_eff)))};
  ((NonegativeProbs)((probs), 1));
  
  ((NormalizeProbs)((probs), 1));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 1));
  switch ((choice))
  {
    case 0:
      return ((Generate__specialEffects)((self_config), (min_n), (max_n), (effect_depth), (give_eff)));
    
  }
  return 0;
}

Card__specialEffects  *Card::Generate__specialEffects(CondConfig  & self_config, signed int  min_n, signed int  max_n, signed int  effect_depth, signed int  give_eff)
{

  Card__specialEffects  *node = (new Card__specialEffects);
  (((node)->item) = (this));
  
  
  {
    (((node)->num_refs) = 1);
  }
  {
    (((node)->effect) = ((Generate__TargetedPlayEff)((self_config), (effect_depth), (give_eff))));
    signed int l_num = ((((node)->effect)->GetEffectNum)());
    ((self_config) &= ((ExtractEffectIndependentConfig)(((((node)->effect)->GetGlobalSelfConfig)((self_config), (EFFECT_TIMING_DEFAULT))))));
    (((node)->effects) = ((Generate__OtherEffs)((self_config), ((min_n) - (l_num)), ((max_n) - (l_num)), (effect_depth), (give_eff))));
  }
  return (node);
}

Card__TargetedPlayEff  *Card::Generate__TargetedPlayEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff)
{

  double probs[3] = {(((double (*)(const GiglConfig  & , CondConfig  & , signed int  , signed int  ))(((config).funcs)[29]))((config), (self_config), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , signed int  , signed int  ))(((config).funcs)[30]))((config), (self_config), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , signed int  , signed int  ))(((config).funcs)[31]))((config), (self_config), (effect_depth), (give_eff)))};
  ((NonegativeProbs)((probs), 3));
  {
    if ((give_eff))
    {
      return ((Generate__noTargetedPlayEff)((self_config), (effect_depth), (give_eff)));
    } else {
      
    }
    if ((!(((self_config) & (TARGET_ANY_CHAR)))))
    {
      (((probs)[1]) = 0.0);
      
    } else {
      
    }
    if ((!(((self_config) & (TARGET_IS_SPELL)))))
    {
      (((probs)[2]) = 0.0);
      
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 3));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 3));
  switch ((choice))
  {
    case 0:
      return ((Generate__noTargetedPlayEff)((self_config), (effect_depth), (give_eff)));
    
    case 1:
      return ((Generate__targetedBattlecryEff)((self_config), (effect_depth), (give_eff)));
    
    case 2:
      return ((Generate__targetedCastEff)((self_config), (effect_depth), (give_eff)));
    
  }
  return 0;
}

Card__noTargetedPlayEff  *Card::Generate__noTargetedPlayEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff)
{

  Card__noTargetedPlayEff  *node = (new Card__noTargetedPlayEff);
  (((node)->item) = (this));
  
  return (node);
}

Card__targetedBattlecryEff  *Card::Generate__targetedBattlecryEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff)
{

  Card__targetedBattlecryEff  *node = (new Card__targetedBattlecryEff);
  (((node)->item) = (this));
  
  {
    ((self_config) &= (CHAR_COND_FILTER));
    (((node)->effect) = ((Generate__TargetedEff)((self_config), (EFFECT_TIMING_PLAY), (effect_depth), (give_eff))));
  }
  return (node);
}

Card__targetedCastEff  *Card::Generate__targetedCastEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff)
{

  Card__targetedCastEff  *node = (new Card__targetedCastEff);
  (((node)->item) = (this));
  
  {
    ((self_config) &= (SPELL_COND_FILTER));
    (((node)->effect) = ((Generate__TargetedEff)((self_config), (EFFECT_TIMING_PLAY), (effect_depth), (give_eff))));
  }
  return (node);
}

Card__OtherEffs  *Card::Generate__OtherEffs(CondConfig  & self_config, signed int  min_n, signed int  max_n, signed int  effect_depth, signed int  give_eff)
{

  double probs[2] = {(((double (*)(const GiglConfig  & , CondConfig  & , signed int  , signed int  , signed int  , signed int  ))(((config).funcs)[32]))((config), (self_config), (min_n), (max_n), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , signed int  , signed int  , signed int  , signed int  ))(((config).funcs)[33]))((config), (self_config), (min_n), (max_n), (effect_depth), (give_eff)))};
  ((NonegativeProbs)((probs), 2));
  {
    if (((min_n) > 0))
    {
      return ((Generate__consOtherEffs)((self_config), (min_n), (max_n), (effect_depth), (give_eff)));
    } else {
      
    }
    if (((max_n) <= 0))
    {
      return ((Generate__noOtherEffs)((self_config), (min_n), (max_n), (effect_depth), (give_eff)));
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 2));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 2));
  switch ((choice))
  {
    case 0:
      return ((Generate__noOtherEffs)((self_config), (min_n), (max_n), (effect_depth), (give_eff)));
    
    case 1:
      return ((Generate__consOtherEffs)((self_config), (min_n), (max_n), (effect_depth), (give_eff)));
    
  }
  return 0;
}

Card__noOtherEffs  *Card::Generate__noOtherEffs(CondConfig  & self_config, signed int  min_n, signed int  max_n, signed int  effect_depth, signed int  give_eff)
{

  Card__noOtherEffs  *node = (new Card__noOtherEffs);
  (((node)->item) = (this));
  
  return (node);
}

Card__consOtherEffs  *Card::Generate__consOtherEffs(CondConfig  & self_config, signed int  min_n, signed int  max_n, signed int  effect_depth, signed int  give_eff)
{

  Card__consOtherEffs  *node = (new Card__consOtherEffs);
  (((node)->item) = (this));
  
  
  {
    (((node)->effect) = ((Generate__OtherEff)((self_config), (effect_depth), (give_eff))));
    ((self_config) &= ((ExtractEffectIndependentConfig)(((((node)->effect)->GetGlobalSelfConfig)((self_config), (EFFECT_TIMING_DEFAULT))))));
    (((node)->effects) = ((Generate__OtherEffs)((self_config), ((min_n) - 1), ((max_n) - 1), (effect_depth), (give_eff))));
  }
  return (node);
}

Card__OtherEff  *Card::Generate__OtherEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff)
{

  double probs[6] = {(((double (*)(const GiglConfig  & , CondConfig  & , signed int  , signed int  ))(((config).funcs)[34]))((config), (self_config), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , signed int  , signed int  ))(((config).funcs)[35]))((config), (self_config), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , signed int  , signed int  ))(((config).funcs)[36]))((config), (self_config), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , signed int  , signed int  ))(((config).funcs)[37]))((config), (self_config), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , signed int  , signed int  ))(((config).funcs)[38]))((config), (self_config), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , signed int  , signed int  ))(((config).funcs)[39]))((config), (self_config), (effect_depth), (give_eff)))};
  ((NonegativeProbs)((probs), 6));
  {
    if ((!(((self_config) & (TARGET_ANY_CHAR)))))
    {
      (((probs)[0]) = 0.0);
      
    } else {
      
    }
    if ((!(((self_config) & (TARGET_IS_SPELL)))))
    {
      (((probs)[1]) = 0.0);
      
    } else {
      
    }
    if ((!(((self_config) & (TARGET_IS_MINION)))))
    {
      (((probs)[2]) = 0.0);
      
    } else {
      
    }
    if (((!(((self_config) & (TARGET_NOT_LEADER)))) && (!(((self_config) & (TARGET_POS_HAND_OR_DECK))))))
    {
      (((probs)[0]) = 0.0);
      (((probs)[3]) = 0.0);
      
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 6));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 6));
  switch ((choice))
  {
    case 0:
      return ((Generate__untargetedBattlecryEff)((self_config), (effect_depth), (give_eff)));
    
    case 1:
      return ((Generate__untargetedCastEff)((self_config), (effect_depth), (give_eff)));
    
    case 2:
      return ((Generate__deathrattleEff)((self_config), (effect_depth), (give_eff)));
    
    case 3:
      return ((Generate__onDiscardEff)((self_config), (effect_depth), (give_eff)));
    
    case 4:
      return ((Generate__turnStartEff)((self_config), (effect_depth), (give_eff)));
    
    case 5:
      return ((Generate__turnEndEff)((self_config), (effect_depth), (give_eff)));
    
  }
  return 0;
}

Card__untargetedBattlecryEff  *Card::Generate__untargetedBattlecryEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff)
{

  Card__untargetedBattlecryEff  *node = (new Card__untargetedBattlecryEff);
  (((node)->item) = (this));
  
  {
    ((self_config) &= (CHAR_COND_FILTER));
    (((node)->effect) = ((Generate__UntargetedEff)((self_config), (EFFECT_TIMING_PLAY), (effect_depth), (give_eff))));
  }
  return (node);
}

Card__untargetedCastEff  *Card::Generate__untargetedCastEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff)
{

  Card__untargetedCastEff  *node = (new Card__untargetedCastEff);
  (((node)->item) = (this));
  
  {
    ((self_config) &= (SPELL_COND_FILTER));
    (((node)->effect) = ((Generate__UntargetedEff)((self_config), (EFFECT_TIMING_PLAY), (effect_depth), (give_eff))));
  }
  return (node);
}

Card__deathrattleEff  *Card::Generate__deathrattleEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff)
{

  Card__deathrattleEff  *node = (new Card__deathrattleEff);
  (((node)->item) = (this));
  
  {
    ((self_config) &= (MINION_COND_FILTER));
    (((node)->effect) = ((Generate__UntargetedEff)((self_config), (EFFECT_TIMING_DESTROY), (effect_depth), (give_eff))));
  }
  return (node);
}

Card__onDiscardEff  *Card::Generate__onDiscardEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff)
{

  Card__onDiscardEff  *node = (new Card__onDiscardEff);
  (((node)->item) = (this));
  
  {
    (((node)->effect) = ((Generate__UntargetedEff)((self_config), (EFFECT_TIMING_DISCARD), (effect_depth), (give_eff))));
  }
  return (node);
}

Card__turnStartEff  *Card::Generate__turnStartEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff)
{

  Card__turnStartEff  *node = (new Card__turnStartEff);
  (((node)->item) = (this));
  
  
  {
    (((node)->effect) = ((Generate__UntargetedEff)((self_config), (EFFECT_TIMING_TURN), (effect_depth), (give_eff))));
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    CondConfig tmp_config = ((GetDefaultConfig)());
    (((node)->alle) = ((Generate__AllegianceCond)((tmp_init_config), (tmp_config), (TARGET_MODE_DEFAULT), (EFFECT_TIMING_TURN))));
  }
  return (node);
}

Card__turnEndEff  *Card::Generate__turnEndEff(CondConfig  & self_config, signed int  effect_depth, signed int  give_eff)
{

  Card__turnEndEff  *node = (new Card__turnEndEff);
  (((node)->item) = (this));
  
  
  {
    (((node)->effect) = ((Generate__UntargetedEff)((self_config), (EFFECT_TIMING_TURN), (effect_depth), (give_eff))));
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    CondConfig tmp_config = ((GetDefaultConfig)());
    (((node)->alle) = ((Generate__AllegianceCond)((tmp_init_config), (tmp_config), (TARGET_MODE_DEFAULT), (EFFECT_TIMING_TURN))));
  }
  return (node);
}

Card__TargetedEff  *Card::Generate__TargetedEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  double probs[3] = {(((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , signed int  , signed int  ))(((config).funcs)[40]))((config), (self_config), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , signed int  , signed int  ))(((config).funcs)[41]))((config), (self_config), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , signed int  , signed int  ))(((config).funcs)[42]))((config), (self_config), (effect_timing), (effect_depth), (give_eff)))};
  ((NonegativeProbs)((probs), 3));
  {
    if (((((self_config) & (TARGET_IS_SPELL))) || (((!(((self_config) & (TARGET_NOT_LEADER)))) && ((effect_timing) == (EFFECT_TIMING_PLAY))))))
    {
      (((probs)[2]) = 0.0);
      
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 3));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 3));
  switch ((choice))
  {
    case 0:
      return ((Generate__noCondTargetedEff)((self_config), (effect_timing), (effect_depth), (give_eff)));
    
    case 1:
      return ((Generate__indeCondTargetedEff)((self_config), (effect_timing), (effect_depth), (give_eff)));
    
    case 2:
      return ((Generate__srcCondTargetedEff)((self_config), (effect_timing), (effect_depth), (give_eff)));
    
  }
  return 0;
}

Card__noCondTargetedEff  *Card::Generate__noCondTargetedEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__noCondTargetedEff  *node = (new Card__noCondTargetedEff);
  (((node)->item) = (this));
  
  
  {
    unsigned int effect_timing_filter = (TARGET_TYPE_ANY);
    if ((!(((self_config) & (TARGET_IS_SPELL)))))
    {
      ((effect_timing_filter) = (PLAY_CHAR_SRC_FILTER));
    } else {
      if ((!(((self_config) & (TARGET_ANY_CHAR)))))
      {
        ((effect_timing_filter) = (PLAY_SPELL_SRC_FILTER));
      } else {
        
      }
    }
    CondConfig self_config_copy = (self_config);
    ((self_config_copy) &= (effect_timing_filter));
    (((node)->effect) = ((Generate__BaseTargetedEff)((self_config_copy), (TARGET_MODE_PLAY), (effect_timing), (effect_depth), (give_eff))));
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    CondConfig tmp_config = ((((node)->effect)->GetPlayTargetConfig)());
    (((node)->desconstr) = ((Generate__TargetCond)((tmp_init_config), (tmp_config), (TARGET_MODE_PLAY), (effect_timing))));
  }
  (((node)->overheat_count) = 0);
  (((node)->overheat_threshold) = 10);
  return (node);
}

Card__indeCondTargetedEff  *Card::Generate__indeCondTargetedEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__indeCondTargetedEff  *node = (new Card__indeCondTargetedEff);
  (((node)->item) = (this));
  
  
  
  {
    unsigned int effect_timing_filter = (TARGET_TYPE_ANY);
    if ((!(((self_config) & (TARGET_IS_SPELL)))))
    {
      ((effect_timing_filter) = (PLAY_CHAR_SRC_FILTER));
    } else {
      if ((!(((self_config) & (TARGET_ANY_CHAR)))))
      {
        ((effect_timing_filter) = (PLAY_SPELL_SRC_FILTER));
      } else {
        
      }
    }
    CondConfig self_config_copy = (self_config);
    ((self_config_copy) &= (effect_timing_filter));
    (((node)->effect) = ((Generate__BaseTargetedEff)((self_config_copy), (TARGET_MODE_PLAY), (effect_timing), (effect_depth), (give_eff))));
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    CondConfig tmp_config = ((((node)->effect)->GetPlayTargetConfig)());
    (((node)->desconstr) = ((Generate__TargetCond)((tmp_init_config), (tmp_config), (TARGET_MODE_PLAY), (effect_timing))));
    (((node)->cond) = ((Generate__IndeCond)(((tmp_init_config) = ((GetDefaultInitConfig)())), ((tmp_config) = ((GetDefaultConfig)())), (effect_timing))));
  }
  (((node)->overheat_count) = 0);
  (((node)->overheat_threshold) = 10);
  return (node);
}

Card__srcCondTargetedEff  *Card::Generate__srcCondTargetedEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__srcCondTargetedEff  *node = (new Card__srcCondTargetedEff);
  (((node)->item) = (this));
  
  
  
  {
    unsigned int effect_timing_filter = (TARGET_TYPE_ANY);
    if ((!(((self_config) & (TARGET_IS_SPELL)))))
    {
      ((effect_timing_filter) = (PLAY_CHAR_SRC_FILTER));
    } else {
      if ((!(((self_config) & (TARGET_ANY_CHAR)))))
      {
        ((effect_timing_filter) = (PLAY_SPELL_SRC_FILTER));
      } else {
        
      }
    }
    CondConfig self_config_copy = (self_config);
    ((self_config_copy) &= (effect_timing_filter));
    (((node)->effect) = ((Generate__BaseTargetedEff)((self_config_copy), (TARGET_MODE_PLAY), (effect_timing), (effect_depth), (give_eff))));
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    CondConfig tmp_config = ((((node)->effect)->GetPlayTargetConfig)());
    (((node)->desconstr) = ((Generate__TargetCond)((tmp_init_config), (tmp_config), (TARGET_MODE_PLAY), (effect_timing))));
    (((node)->srccond) = ((Generate__TargetCond)((self_config_copy), ((tmp_config) = ((GetFlagConfig)((effect_timing_filter)))), (TARGET_MODE_SOURCE), (effect_timing))));
    ((self_config) |= ((((node)->srccond)->GetInitAttrFlag)()));
  }
  (((node)->overheat_count) = 0);
  (((node)->overheat_threshold) = 10);
  return (node);
}

Card__UntargetedEff  *Card::Generate__UntargetedEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  double probs[3] = {(((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , signed int  , signed int  ))(((config).funcs)[43]))((config), (self_config), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , signed int  , signed int  ))(((config).funcs)[44]))((config), (self_config), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , signed int  , signed int  ))(((config).funcs)[45]))((config), (self_config), (effect_timing), (effect_depth), (give_eff)))};
  ((NonegativeProbs)((probs), 3));
  {
    if (((effect_timing) == (EFFECT_TIMING_TURN)))
    {
      return ((Generate__srcCondUntargetedEff)((self_config), (effect_timing), (effect_depth), (give_eff)));
    } else {
      
    }
    if (((!(((self_config) & (TARGET_NOT_LEADER)))) && ((effect_timing) == (EFFECT_TIMING_PLAY))))
    {
      (((probs)[2]) = 0.0);
      
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 3));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 3));
  switch ((choice))
  {
    case 0:
      return ((Generate__noCondUntargetedEff)((self_config), (effect_timing), (effect_depth), (give_eff)));
    
    case 1:
      return ((Generate__indeCondUntargetedEff)((self_config), (effect_timing), (effect_depth), (give_eff)));
    
    case 2:
      return ((Generate__srcCondUntargetedEff)((self_config), (effect_timing), (effect_depth), (give_eff)));
    
  }
  return 0;
}

Card__noCondUntargetedEff  *Card::Generate__noCondUntargetedEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__noCondUntargetedEff  *node = (new Card__noCondUntargetedEff);
  (((node)->item) = (this));
  
  {
    unsigned int effect_timing_filter = (TARGET_TYPE_ANY);
    switch ((effect_timing))
    {
      {
        case (EFFECT_TIMING_PLAY):
          if ((!(((self_config) & (TARGET_IS_SPELL)))))
          {
            ((effect_timing_filter) = (PLAY_CHAR_SRC_FILTER));
          } else {
            if ((!(((self_config) & (TARGET_ANY_CHAR)))))
            {
              ((effect_timing_filter) = (PLAY_SPELL_SRC_FILTER));
            } else {
              
            }
          }
        
        break;
        case (EFFECT_TIMING_DESTROY):
          ((effect_timing_filter) = (DESTROY_SRC_FILTER));
        
        break;
      }
    }
    CondConfig self_config_copy = (self_config);
    ((self_config_copy) &= (effect_timing_filter));
    (((node)->effect) = ((Generate__BaseUntargetedEff)((self_config_copy), (effect_timing), (effect_depth), (give_eff))));
  }
  (((node)->overheat_count) = 0);
  (((node)->overheat_threshold) = 10);
  return (node);
}

Card__indeCondUntargetedEff  *Card::Generate__indeCondUntargetedEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__indeCondUntargetedEff  *node = (new Card__indeCondUntargetedEff);
  (((node)->item) = (this));
  
  
  {
    unsigned int effect_timing_filter = (TARGET_TYPE_ANY);
    switch ((effect_timing))
    {
      {
        case (EFFECT_TIMING_PLAY):
          if ((!(((self_config) & (TARGET_IS_SPELL)))))
          {
            ((effect_timing_filter) = (PLAY_CHAR_SRC_FILTER));
          } else {
            if ((!(((self_config) & (TARGET_ANY_CHAR)))))
            {
              ((effect_timing_filter) = (PLAY_SPELL_SRC_FILTER));
            } else {
              
            }
          }
        
        break;
        case (EFFECT_TIMING_DESTROY):
          ((effect_timing_filter) = (DESTROY_SRC_FILTER));
        
        break;
      }
    }
    CondConfig self_config_copy = (self_config);
    ((self_config_copy) &= (effect_timing_filter));
    (((node)->effect) = ((Generate__BaseUntargetedEff)((self_config_copy), (effect_timing), (effect_depth), (give_eff))));
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    CondConfig tmp_config = ((((node)->effect)->GetLeaderConfig)());
    (((node)->cond) = ((Generate__IndeCond)((tmp_init_config), (tmp_config), (effect_timing))));
  }
  (((node)->overheat_count) = 0);
  (((node)->overheat_threshold) = 10);
  return (node);
}

Card__srcCondUntargetedEff  *Card::Generate__srcCondUntargetedEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__srcCondUntargetedEff  *node = (new Card__srcCondUntargetedEff);
  (((node)->item) = (this));
  
  
  {
    unsigned int effect_timing_filter = (TARGET_TYPE_ANY);
    switch ((effect_timing))
    {
      {
        case (EFFECT_TIMING_PLAY):
          if ((!(((self_config) & (TARGET_IS_SPELL)))))
          {
            ((effect_timing_filter) = (PLAY_CHAR_SRC_FILTER));
          } else {
            if ((!(((self_config) & (TARGET_ANY_CHAR)))))
            {
              ((effect_timing_filter) = (PLAY_SPELL_SRC_FILTER));
            } else {
              
            }
          }
        
        break;
        case (EFFECT_TIMING_DESTROY):
          ((effect_timing_filter) = (DESTROY_SRC_FILTER));
        
        break;
      }
    }
    CondConfig self_config_copy = (self_config);
    ((self_config_copy) &= (effect_timing_filter));
    (((node)->effect) = ((Generate__BaseUntargetedEff)((self_config_copy), (effect_timing), (effect_depth), (give_eff))));
    CondConfig self_instant_config = ((((node)->effect)->GetSelfConfig)((self_config)));
    ((self_instant_config) &= (effect_timing_filter));
    (((node)->srccond) = ((Generate__TargetCond)((self_config_copy), (self_instant_config), (TARGET_MODE_SOURCE), (effect_timing))));
    ((self_config) |= ((((node)->srccond)->GetInitAttrFlag)()));
  }
  (((node)->overheat_count) = 0);
  (((node)->overheat_threshold) = 10);
  return (node);
}

Card__TargetCond  *Card::Generate__TargetCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  double probs[2] = {(((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[46]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[47]))((config), (init_config), (instant_config), (target_mode), (effect_timing)))};
  ((NonegativeProbs)((probs), 2));
  {
    if (((((!(((init_config) & (TARGET_ANY_CHAR)))) || (!(((init_config) & (TARGET_POS_FIELD))))) || (!(((instant_config) & (TARGET_ANY_CHAR))))) || (!(((instant_config) & (TARGET_POS_FIELD))))))
    {
      return ((Generate__cardTargetCond)((init_config), (instant_config), (target_mode), (effect_timing)));
    } else {
      
    }
    if ((((!(((init_config) & (TARGET_POS_HAND_OR_DECK)))) || (!(((instant_config) & (TARGET_POS_HAND_OR_DECK))))) || ((((target_mode) == (TARGET_MODE_PLAY)) && (((((!(((init_config) & (TARGET_IS_ALLY)))) || (!(((instant_config) & (TARGET_IS_ALLY))))) || (!(((init_config) & (TARGET_POS_HAND))))) || (!(((instant_config) & (TARGET_POS_HAND))))))))))
    {
      return ((Generate__charTargetCond)((init_config), (instant_config), (target_mode), (effect_timing)));
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 2));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 2));
  switch ((choice))
  {
    case 0:
      return ((Generate__charTargetCond)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 1:
      return ((Generate__cardTargetCond)((init_config), (instant_config), (target_mode), (effect_timing)));
    
  }
  return 0;
}

Card__charTargetCond  *Card::Generate__charTargetCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__charTargetCond  *node = (new Card__charTargetCond);
  (((node)->item) = (this));
  
  {
    (((node)->cond) = ((Generate__CharTargetCond)((init_config), (instant_config), (target_mode), (effect_timing))));
  }
  return (node);
}

Card__cardTargetCond  *Card::Generate__cardTargetCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__cardTargetCond  *node = (new Card__cardTargetCond);
  (((node)->item) = (this));
  
  {
    (((node)->cond) = ((Generate__CardTargetCond)((init_config), (instant_config), (target_mode), (effect_timing))));
  }
  return (node);
}

Card__CharTargetCond  *Card::Generate__CharTargetCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  double probs[1] = {(((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[48]))((config), (init_config), (instant_config), (target_mode), (effect_timing)))};
  ((NonegativeProbs)((probs), 1));
  
  ((NormalizeProbs)((probs), 1));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 1));
  switch ((choice))
  {
    case 0:
      return ((Generate__justCharTargetCond)((init_config), (instant_config), (target_mode), (effect_timing)));
    
  }
  return 0;
}

Card__justCharTargetCond  *Card::Generate__justCharTargetCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__justCharTargetCond  *node = (new Card__justCharTargetCond);
  (((node)->item) = (this));
  
  
  
  
  {
    if ((((target_mode) != (TARGET_MODE_SOURCE)) || ((effect_timing) != (EFFECT_TIMING_DESTROY))))
    {
      {
        if ((((init_config).min_hp) < 1))
        {
          (((init_config).min_hp) = 1);
        } else {
          
        }
        if ((((instant_config).min_hp) < 1))
        {
          (((instant_config).min_hp) = 1);
        } else {
          
        }
      }
    } else {
      
    }
    if ((((target_mode) == (TARGET_MODE_SOURCE)) && ((effect_timing) == (EFFECT_TIMING_PLAY))))
    {
      {
        if ((((instant_config).min_hp) < ((init_config).min_hp)))
        {
          (((instant_config).min_hp) = ((init_config).min_hp));
        } else {
          
        }
      }
    } else {
      
    }
    if (((target_mode) == (TARGET_MODE_PLAY)))
    {
      ((instant_config) &= (PLAY_CHAR_TARGET_FILTER));
    } else {
      
    }
    if ((((target_mode) == (TARGET_MODE_SOURCE)) && (((!(((init_config) & (TARGET_NOT_LEADER)))) || (!(((instant_config) & (TARGET_NOT_LEADER))))))))
    {
      {
        (((node)->alle) = (new Card__anyAllegiance((this))));
        (((node)->typecond) = (new Card__isCharacter((this))));
        (((node)->attrcond) = (new Card__noAttrCond((this))));
        (((node)->statcond) = (new Card__noStatCond((this))));
      }
    } else {
      {
        (((node)->alle) = ((Generate__AllegianceCond)((init_config), (instant_config), (target_mode), (effect_timing))));
        (((node)->typecond) = ((Generate__CharTypeCond)((init_config), (instant_config), (target_mode), (effect_timing))));
        ((init_config) &= ((((node)->typecond)->GetTargetConfig)()));
        ((instant_config) &= ((((node)->typecond)->GetTargetConfig)()));
        (((node)->attrcond) = ((Generate__AttrCond)((init_config), (instant_config), (target_mode), (effect_timing))));
        if (((((!(((instant_config) & (TARGET_POS_HAND_OR_DECK)))) && ((target_mode) == (TARGET_MODE_SOURCE))) && ((((node)->typecond)->isCondTrivial)())) && ((((node)->attrcond)->isCondTrivial)())))
        {
          (((node)->statcond) = ((Generate__StatCond)((init_config), (instant_config), (target_mode), (effect_timing), 1)));
        } else {
          (((node)->statcond) = ((Generate__StatCond)((init_config), (instant_config), (target_mode), (effect_timing), 0)));
        }
      }
    }
  }
  return (node);
}

Card__CharTypeCond  *Card::Generate__CharTypeCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  double probs[5] = {(((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[49]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[50]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[51]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[52]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[53]))((config), (init_config), (instant_config), (target_mode), (effect_timing)))};
  ((NonegativeProbs)((probs), 5));
  {
    if (((!(((init_config) & (TARGET_IS_MINION)))) || (!(((instant_config) & (TARGET_IS_MINION))))))
    {
      return ((Generate__isCharacter)((init_config), (instant_config), (target_mode), (effect_timing)));
    } else {
      
    }
    if (((!(((init_config) & (TARGET_IS_LEADER)))) || (!(((instant_config) & (TARGET_IS_LEADER))))))
    {
      (((probs)[0]) = 0.0);
      
    } else {
      
    }
    if (((!(((init_config) & (TARGET_NOT_BEAST)))) || (!(((instant_config) & (TARGET_IS_BEAST))))))
    {
      (((probs)[2]) = 0.0);
      
    } else {
      
    }
    if (((!(((init_config) & (TARGET_NOT_DRAGON)))) || (!(((instant_config) & (TARGET_IS_DRAGON))))))
    {
      (((probs)[3]) = 0.0);
      
    } else {
      
    }
    if (((!(((init_config) & (TARGET_NOT_DEMON)))) || (!(((instant_config) & (TARGET_IS_DEMON))))))
    {
      (((probs)[4]) = 0.0);
      
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 5));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 5));
  switch ((choice))
  {
    case 0:
      return ((Generate__isCharacter)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 1:
      return ((Generate__isMinion)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 2:
      return ((Generate__isBeast)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 3:
      return ((Generate__isDragon)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 4:
      return ((Generate__isDemon)((init_config), (instant_config), (target_mode), (effect_timing)));
    
  }
  return 0;
}

Card__isCharacter  *Card::Generate__isCharacter(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__isCharacter  *node = (new Card__isCharacter);
  (((node)->item) = (this));
  
  return (node);
}

Card__isMinion  *Card::Generate__isMinion(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__isMinion  *node = (new Card__isMinion);
  (((node)->item) = (this));
  
  return (node);
}

Card__isBeast  *Card::Generate__isBeast(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__isBeast  *node = (new Card__isBeast);
  (((node)->item) = (this));
  
  return (node);
}

Card__isDragon  *Card::Generate__isDragon(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__isDragon  *node = (new Card__isDragon);
  (((node)->item) = (this));
  
  return (node);
}

Card__isDemon  *Card::Generate__isDemon(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__isDemon  *node = (new Card__isDemon);
  (((node)->item) = (this));
  
  return (node);
}

Card__CardTargetCond  *Card::Generate__CardTargetCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  double probs[1] = {(((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[54]))((config), (init_config), (instant_config), (target_mode), (effect_timing)))};
  ((NonegativeProbs)((probs), 1));
  
  ((NormalizeProbs)((probs), 1));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 1));
  switch ((choice))
  {
    case 0:
      return ((Generate__justCardTargetCond)((init_config), (instant_config), (target_mode), (effect_timing)));
    
  }
  return 0;
}

Card__justCardTargetCond  *Card::Generate__justCardTargetCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__justCardTargetCond  *node = (new Card__justCardTargetCond);
  (((node)->item) = (this));
  
  
  
  
  
  {
    if ((((init_config).min_hp) < 1))
    {
      (((init_config).min_hp) = 1);
    } else {
      
    }
    if ((((instant_config).min_hp) < 1))
    {
      (((instant_config).min_hp) = 1);
    } else {
      
    }
    if ((((instant_config).min_hp) < ((init_config).min_hp)))
    {
      (((instant_config).min_hp) = ((init_config).min_hp));
    } else {
      
    }
    if (((target_mode) == (TARGET_MODE_PLAY)))
    {
      ((instant_config) &= (PLAY_CARD_TARGET_FILTER));
    } else {
      
    }
    (((node)->pos) = ((Generate__CardPosCond)((init_config), (instant_config), (target_mode), (effect_timing))));
    (((node)->alle) = ((Generate__AllegianceCond)((init_config), (instant_config), (target_mode), (effect_timing))));
    (((node)->typecond) = ((Generate__CardTypeCond)((init_config), (instant_config), (target_mode), (effect_timing))));
    ((init_config) &= ((((node)->typecond)->GetTargetConfig)()));
    ((instant_config) &= ((((node)->typecond)->GetTargetConfig)()));
    (((node)->attrcond) = ((Generate__AttrCond)((init_config), (instant_config), (target_mode), (effect_timing))));
    if ((((((!(((instant_config) & (TARGET_POS_FIELD)))) && ((target_mode) == (TARGET_MODE_SOURCE))) && ((((node)->pos)->isCondTrivial)())) && ((((node)->typecond)->isCondTrivial)())) && ((((node)->attrcond)->isCondTrivial)())))
    {
      (((node)->statcond) = ((Generate__StatCond)((init_config), (instant_config), (target_mode), 1, (effect_timing))));
    } else {
      (((node)->statcond) = ((Generate__StatCond)((init_config), (instant_config), (target_mode), 0, (effect_timing))));
    }
  }
  return (node);
}

Card__CardTypeCond  *Card::Generate__CardTypeCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  double probs[7] = {(((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[55]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[56]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[57]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[58]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[59]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[60]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[61]))((config), (init_config), (instant_config), (target_mode), (effect_timing)))};
  ((NonegativeProbs)((probs), 7));
  {
    if (((target_mode) == (TARGET_MODE_SOURCE)))
    {
      {
        (((probs)[1]) = 0.0);
        (((probs)[2]) = 0.0);
        (((probs)[3]) = 0.0);
        
        if (((!(((init_config) & (TARGET_IS_MINION)))) || (!(((instant_config) & (TARGET_IS_MINION))))))
        {
          return ((Generate__isCard)((init_config), (instant_config), (target_mode), (effect_timing)));
        } else {
          
        }
      }
    } else {
      {
        if (((!(((init_config) & (TARGET_IS_LEADER)))) || (!(((instant_config) & (TARGET_IS_LEADER))))))
        {
          (((probs)[0]) = 0.0);
          (((probs)[1]) = 0.0);
          
        } else {
          
        }
        if (((!(((init_config) & (TARGET_IS_MINION)))) || (!(((instant_config) & (TARGET_IS_MINION))))))
        {
          (((probs)[0]) = 0.0);
          (((probs)[2]) = 0.0);
          (((probs)[4]) = 0.0);
          (((probs)[5]) = 0.0);
          (((probs)[6]) = 0.0);
          
        } else {
          
        }
        if (((!(((init_config) & (TARGET_IS_SPELL)))) || (!(((instant_config) & (TARGET_IS_SPELL))))))
        {
          (((probs)[0]) = 0.0);
          (((probs)[3]) = 0.0);
          
        } else {
          
        }
      }
    }
    if (((!(((init_config) & (TARGET_NOT_BEAST)))) || (!(((instant_config) & (TARGET_IS_BEAST))))))
    {
      (((probs)[4]) = 0.0);
      
    } else {
      
    }
    if (((!(((init_config) & (TARGET_NOT_DRAGON)))) || (!(((instant_config) & (TARGET_IS_DRAGON))))))
    {
      (((probs)[5]) = 0.0);
      
    } else {
      
    }
    if (((!(((init_config) & (TARGET_NOT_DEMON)))) || (!(((instant_config) & (TARGET_IS_DEMON))))))
    {
      (((probs)[6]) = 0.0);
      
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 7));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 7));
  switch ((choice))
  {
    case 0:
      return ((Generate__isCard)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 1:
      return ((Generate__isLeaderCard)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 2:
      return ((Generate__isMinionCard)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 3:
      return ((Generate__isSpellCard)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 4:
      return ((Generate__isBeastCard)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 5:
      return ((Generate__isDragonCard)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 6:
      return ((Generate__isDemonCard)((init_config), (instant_config), (target_mode), (effect_timing)));
    
  }
  return 0;
}

Card__isCard  *Card::Generate__isCard(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__isCard  *node = (new Card__isCard);
  (((node)->item) = (this));
  
  return (node);
}

Card__isLeaderCard  *Card::Generate__isLeaderCard(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__isLeaderCard  *node = (new Card__isLeaderCard);
  (((node)->item) = (this));
  
  return (node);
}

Card__isMinionCard  *Card::Generate__isMinionCard(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__isMinionCard  *node = (new Card__isMinionCard);
  (((node)->item) = (this));
  
  return (node);
}

Card__isSpellCard  *Card::Generate__isSpellCard(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__isSpellCard  *node = (new Card__isSpellCard);
  (((node)->item) = (this));
  
  return (node);
}

Card__isBeastCard  *Card::Generate__isBeastCard(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__isBeastCard  *node = (new Card__isBeastCard);
  (((node)->item) = (this));
  
  return (node);
}

Card__isDragonCard  *Card::Generate__isDragonCard(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__isDragonCard  *node = (new Card__isDragonCard);
  (((node)->item) = (this));
  
  return (node);
}

Card__isDemonCard  *Card::Generate__isDemonCard(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__isDemonCard  *node = (new Card__isDemonCard);
  (((node)->item) = (this));
  
  return (node);
}

Card__CardPosCond  *Card::Generate__CardPosCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  double probs[3] = {(((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[62]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[63]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[64]))((config), (init_config), (instant_config), (target_mode), (effect_timing)))};
  ((NonegativeProbs)((probs), 3));
  {
    if ((!(((instant_config) & (TARGET_POS_DECK)))))
    {
      return ((Generate__cardPosAtHand)((init_config), (instant_config), (target_mode), (effect_timing)));
    } else {
      
    }
    if ((!(((instant_config) & (TARGET_POS_HAND)))))
    {
      return ((Generate__cardPosAtDeck)((init_config), (instant_config), (target_mode), (effect_timing)));
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 3));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 3));
  switch ((choice))
  {
    case 0:
      return ((Generate__cardPosAtHandOrDeck)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 1:
      return ((Generate__cardPosAtHand)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 2:
      return ((Generate__cardPosAtDeck)((init_config), (instant_config), (target_mode), (effect_timing)));
    
  }
  return 0;
}

Card__cardPosAtHandOrDeck  *Card::Generate__cardPosAtHandOrDeck(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__cardPosAtHandOrDeck  *node = (new Card__cardPosAtHandOrDeck);
  (((node)->item) = (this));
  
  return (node);
}

Card__cardPosAtHand  *Card::Generate__cardPosAtHand(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__cardPosAtHand  *node = (new Card__cardPosAtHand);
  (((node)->item) = (this));
  
  return (node);
}

Card__cardPosAtDeck  *Card::Generate__cardPosAtDeck(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__cardPosAtDeck  *node = (new Card__cardPosAtDeck);
  (((node)->item) = (this));
  
  return (node);
}

Card__AllegianceCond  *Card::Generate__AllegianceCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  double probs[3] = {(((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[65]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[66]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[67]))((config), (init_config), (instant_config), (target_mode), (effect_timing)))};
  ((NonegativeProbs)((probs), 3));
  {
    if (((target_mode) == (TARGET_MODE_SOURCE)))
    {
      return ((Generate__anyAllegiance)((init_config), (instant_config), (target_mode), (effect_timing)));
    } else {
      
    }
    if ((((target_mode) == (TARGET_MODE_MOVE_DEST)) || ((target_mode) == (TARGET_MODE_WIN_GAME))))
    {
      (((probs)[0]) = 0.0);
      
    } else {
      
    }
    if ((!(((instant_config) & (TARGET_IS_OPPO)))))
    {
      return ((Generate__allyAllegiance)((init_config), (instant_config), (target_mode), (effect_timing)));
    } else {
      
    }
    if ((!(((instant_config) & (TARGET_IS_ALLY)))))
    {
      return ((Generate__oppoAllegiance)((init_config), (instant_config), (target_mode), (effect_timing)));
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 3));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 3));
  switch ((choice))
  {
    case 0:
      return ((Generate__anyAllegiance)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 1:
      return ((Generate__allyAllegiance)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 2:
      return ((Generate__oppoAllegiance)((init_config), (instant_config), (target_mode), (effect_timing)));
    
  }
  return 0;
}

Card__anyAllegiance  *Card::Generate__anyAllegiance(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__anyAllegiance  *node = (new Card__anyAllegiance);
  (((node)->item) = (this));
  
  return (node);
}

Card__allyAllegiance  *Card::Generate__allyAllegiance(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__allyAllegiance  *node = (new Card__allyAllegiance);
  (((node)->item) = (this));
  
  return (node);
}

Card__oppoAllegiance  *Card::Generate__oppoAllegiance(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__oppoAllegiance  *node = (new Card__oppoAllegiance);
  (((node)->item) = (this));
  
  return (node);
}

Card__AttrCond  *Card::Generate__AttrCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  double probs[8] = {(((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[68]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[69]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[70]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[71]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[72]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[73]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[74]))((config), (init_config), (instant_config), (target_mode), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  ))(((config).funcs)[75]))((config), (init_config), (instant_config), (target_mode), (effect_timing)))};
  ((NonegativeProbs)((probs), 8));
  {
    if ((((((init_config) & (TARGET_IS_CHARGE))) || (!(((instant_config) & (TARGET_IS_CHARGE))))) || (((init_config) & (TARGET_IS_SPELL)))))
    {
      (((probs)[1]) = 0.0);
      
    } else {
      
    }
    if ((((((init_config) & (TARGET_IS_TAUNT))) || (!(((instant_config) & (TARGET_IS_TAUNT))))) || (((init_config) & (TARGET_IS_SPELL)))))
    {
      (((probs)[2]) = 0.0);
      
    } else {
      
    }
    if ((((((((init_config) & (TARGET_IS_STEALTH))) && (((!(((instant_config) & (TARGET_POS_FIELD)))) || ((((target_mode) == (TARGET_MODE_SOURCE)) && ((effect_timing) == (EFFECT_TIMING_PLAY)))))))) || (!(((instant_config) & (TARGET_IS_STEALTH))))) || (((init_config) & (((TARGET_IS_SPELL) | (TARGET_IS_LEADER)))))))
    {
      (((probs)[3]) = 0.0);
      
    } else {
      
    }
    if (((((init_config) & (TARGET_IS_UNTARGETABLE))) || (!(((instant_config) & (TARGET_IS_UNTARGETABLE))))))
    {
      (((probs)[4]) = 0.0);
      
    } else {
      
    }
    if ((((((((init_config) & (TARGET_IS_SHIELDED))) && (((!(((instant_config) & (TARGET_POS_FIELD)))) || ((((target_mode) == (TARGET_MODE_SOURCE)) && ((effect_timing) == (EFFECT_TIMING_PLAY)))))))) || (!(((instant_config) & (TARGET_IS_SHIELDED))))) || (((init_config) & (TARGET_IS_SPELL)))))
    {
      (((probs)[5]) = 0.0);
      
    } else {
      
    }
    if (((((init_config) & (TARGET_IS_POISONOUS))) || (!(((instant_config) & (TARGET_IS_POISONOUS))))))
    {
      (((probs)[6]) = 0.0);
      
    } else {
      
    }
    if (((((init_config) & (TARGET_IS_LIFESTEAL))) || (!(((instant_config) & (TARGET_IS_LIFESTEAL))))))
    {
      (((probs)[7]) = 0.0);
      
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 8));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 8));
  switch ((choice))
  {
    case 0:
      return ((Generate__noAttrCond)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 1:
      return ((Generate__chargeCond)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 2:
      return ((Generate__tauntCond)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 3:
      return ((Generate__stealthCond)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 4:
      return ((Generate__untargetableCond)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 5:
      return ((Generate__shieldCond)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 6:
      return ((Generate__poisonousCond)((init_config), (instant_config), (target_mode), (effect_timing)));
    
    case 7:
      return ((Generate__lifestealCond)((init_config), (instant_config), (target_mode), (effect_timing)));
    
  }
  return 0;
}

Card__noAttrCond  *Card::Generate__noAttrCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__noAttrCond  *node = (new Card__noAttrCond);
  (((node)->item) = (this));
  
  return (node);
}

Card__chargeCond  *Card::Generate__chargeCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__chargeCond  *node = (new Card__chargeCond);
  (((node)->item) = (this));
  
  return (node);
}

Card__tauntCond  *Card::Generate__tauntCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__tauntCond  *node = (new Card__tauntCond);
  (((node)->item) = (this));
  
  return (node);
}

Card__stealthCond  *Card::Generate__stealthCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__stealthCond  *node = (new Card__stealthCond);
  (((node)->item) = (this));
  
  return (node);
}

Card__untargetableCond  *Card::Generate__untargetableCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__untargetableCond  *node = (new Card__untargetableCond);
  (((node)->item) = (this));
  
  return (node);
}

Card__shieldCond  *Card::Generate__shieldCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__shieldCond  *node = (new Card__shieldCond);
  (((node)->item) = (this));
  
  return (node);
}

Card__poisonousCond  *Card::Generate__poisonousCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__poisonousCond  *node = (new Card__poisonousCond);
  (((node)->item) = (this));
  
  return (node);
}

Card__lifestealCond  *Card::Generate__lifestealCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  Card__lifestealCond  *node = (new Card__lifestealCond);
  (((node)->item) = (this));
  
  return (node);
}

Card__StatCond  *Card::Generate__StatCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial)
{

  double probs[5] = {(((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  ))(((config).funcs)[76]))((config), (init_config), (instant_config), (target_mode), (effect_timing), (force_nontrivial))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  ))(((config).funcs)[77]))((config), (init_config), (instant_config), (target_mode), (effect_timing), (force_nontrivial))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  ))(((config).funcs)[78]))((config), (init_config), (instant_config), (target_mode), (effect_timing), (force_nontrivial))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  ))(((config).funcs)[79]))((config), (init_config), (instant_config), (target_mode), (effect_timing), (force_nontrivial))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  ))(((config).funcs)[80]))((config), (init_config), (instant_config), (target_mode), (effect_timing), (force_nontrivial)))};
  ((NonegativeProbs)((probs), 5));
  {
    if ((((((init_config) & (TARGET_IS_SPELL))) || (!(((instant_config) & (TARGET_ANY_CHAR))))) || (((((instant_config) & (TARGET_IS_SPELL))) && ((target_mode) != (TARGET_MODE_SOURCE))))))
    {
      (((probs)[2]) = 0.0);
      (((probs)[3]) = 0.0);
      (((probs)[4]) = 0.0);
      
    } else {
      
    }
    signed int cost_scale = (((((init_config).max_cost) - ((init_config).min_cost)) + ((instant_config).max_cost)) - ((instant_config).min_cost));
    (((probs)[1]) *= ((((cost_scale) > 0) ? (0.1 * (cost_scale)) : 0)));
    signed int atk_scale = (((((init_config).max_atk) - ((init_config).min_atk)) + ((instant_config).max_atk)) - ((instant_config).min_atk));
    (((probs)[2]) *= ((((atk_scale) > 0) ? (0.1 * (atk_scale)) : 0)));
    signed int hp_scale = (((((init_config).max_hp) - ((init_config).min_hp)) + ((instant_config).max_hp)) - ((instant_config).min_hp));
    (((probs)[3]) *= ((((hp_scale) > 0) ? (0.1 * (hp_scale)) : 0)));
    signed int n_atks_scale = (((((init_config).max_n_atks) - ((init_config).min_n_atks)) + ((instant_config).max_n_atks)) - ((instant_config).min_n_atks));
    (((probs)[4]) *= ((((n_atks_scale) > 0) ? (0.1 * (n_atks_scale)) : 0)));
    if (((force_nontrivial) && (((((((probs)[1]) > 0) || (((probs)[2]) > 0)) || (((probs)[3]) > 0)) || (((probs)[4]) > 0)))))
    {
      (((probs)[0]) = 0.0);
      
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 5));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 5));
  switch ((choice))
  {
    case 0:
      return ((Generate__noStatCond)((init_config), (instant_config), (target_mode), (effect_timing), (force_nontrivial)));
    
    case 1:
      return ((Generate__costCond)((init_config), (instant_config), (target_mode), (effect_timing), (force_nontrivial)));
    
    case 2:
      return ((Generate__atkCond)((init_config), (instant_config), (target_mode), (effect_timing), (force_nontrivial)));
    
    case 3:
      return ((Generate__hpCond)((init_config), (instant_config), (target_mode), (effect_timing), (force_nontrivial)));
    
    case 4:
      return ((Generate__atkTimesCond)((init_config), (instant_config), (target_mode), (effect_timing), (force_nontrivial)));
    
  }
  return 0;
}

Card__noStatCond  *Card::Generate__noStatCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial)
{

  Card__noStatCond  *node = (new Card__noStatCond);
  (((node)->item) = (this));
  
  return (node);
}

Card__costCond  *Card::Generate__costCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial)
{

  Card__costCond  *node = (new Card__costCond);
  (((node)->item) = (this));
  
  {
    signed int lower_min = (((init_config).min_cost) + 1);
    signed int lower_max = ((instant_config).max_cost);
    signed int upper_min = ((instant_config).min_cost);
    signed int upper_max = (((init_config).max_cost) - 1);
    (((node)->variant) = ((Generate__StatCondVariant)((lower_min), (lower_max), (upper_min), (upper_max))));
  }
  return (node);
}

Card__atkCond  *Card::Generate__atkCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial)
{

  Card__atkCond  *node = (new Card__atkCond);
  (((node)->item) = (this));
  
  {
    signed int lower_min = (((init_config).min_atk) + 1);
    signed int lower_max = ((instant_config).max_atk);
    signed int upper_min = ((instant_config).min_atk);
    signed int upper_max = (((init_config).max_atk) - 1);
    (((node)->variant) = ((Generate__StatCondVariant)((lower_min), (lower_max), (upper_min), (upper_max))));
  }
  return (node);
}

Card__hpCond  *Card::Generate__hpCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial)
{

  Card__hpCond  *node = (new Card__hpCond);
  (((node)->item) = (this));
  
  {
    signed int lower_min = (((init_config).min_hp) + 1);
    signed int lower_max = ((instant_config).max_hp);
    signed int upper_min = ((instant_config).min_hp);
    signed int upper_max = (((init_config).max_hp) - 1);
    (((node)->variant) = ((Generate__StatCondVariant)((lower_min), (lower_max), (upper_min), (upper_max))));
  }
  return (node);
}

Card__atkTimesCond  *Card::Generate__atkTimesCond(CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial)
{

  Card__atkTimesCond  *node = (new Card__atkTimesCond);
  (((node)->item) = (this));
  
  {
    signed int lower_min = (((init_config).min_n_atks) + 1);
    signed int lower_max = ((instant_config).max_n_atks);
    signed int upper_min = ((instant_config).min_n_atks);
    signed int upper_max = (((init_config).max_n_atks) - 1);
    (((node)->variant) = ((Generate__StatCondVariant)((lower_min), (lower_max), (upper_min), (upper_max))));
  }
  return (node);
}

Card__StatCondVariant  *Card::Generate__StatCondVariant(signed int  lower_min, signed int  lower_max, signed int  upper_min, signed int  upper_max)
{

  double probs[2] = {(((double (*)(const GiglConfig  & , signed int  , signed int  , signed int  , signed int  ))(((config).funcs)[81]))((config), (lower_min), (lower_max), (upper_min), (upper_max))), (((double (*)(const GiglConfig  & , signed int  , signed int  , signed int  , signed int  ))(((config).funcs)[82]))((config), (lower_min), (lower_max), (upper_min), (upper_max)))};
  ((NonegativeProbs)((probs), 2));
  {
    (((probs)[0]) *= (((lower_max) - (lower_min)) + 1));
    (((probs)[1]) *= (((upper_max) - (upper_min)) + 1));
  }
  ((NormalizeProbs)((probs), 2));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 2));
  switch ((choice))
  {
    case 0:
      return ((Generate__statGe)((lower_min), (lower_max), (upper_min), (upper_max)));
    
    case 1:
      return ((Generate__statLe)((lower_min), (lower_max), (upper_min), (upper_max)));
    
  }
  return 0;
}

Card__statGe  *Card::Generate__statGe(signed int  lower_min, signed int  lower_max, signed int  upper_min, signed int  upper_max)
{

  Card__statGe  *node = (new Card__statGe);
  (((node)->item) = (this));
  
  {
    (((node)->val) = ((GetRandInt)((lower_min), (lower_max))));
  }
  return (node);
}

Card__statLe  *Card::Generate__statLe(signed int  lower_min, signed int  lower_max, signed int  upper_min, signed int  upper_max)
{

  Card__statLe  *node = (new Card__statLe);
  (((node)->item) = (this));
  
  {
    (((node)->val) = ((GetRandInt)((upper_min), (upper_max))));
  }
  return (node);
}

Card__IndeCond  *Card::Generate__IndeCond(CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing)
{

  double probs[5] = {(((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  ))(((config).funcs)[83]))((config), (init_config), (instant_leader_config), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  ))(((config).funcs)[84]))((config), (init_config), (instant_leader_config), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  ))(((config).funcs)[85]))((config), (init_config), (instant_leader_config), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  ))(((config).funcs)[86]))((config), (init_config), (instant_leader_config), (effect_timing))), (((double (*)(const GiglConfig  & , CondConfig  & , CondConfig  & , unsigned int  ))(((config).funcs)[87]))((config), (init_config), (instant_leader_config), (effect_timing)))};
  ((NonegativeProbs)((probs), 5));
  {
    if (((effect_timing) == (EFFECT_TIMING_PLAY)))
    {
      (((probs)[3]) = 0.0);
      
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 5));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 5));
  switch ((choice))
  {
    case 0:
      return ((Generate__fieldExistenceCond)((init_config), (instant_leader_config), (effect_timing)));
    
    case 1:
      return ((Generate__cardExistenceCond)((init_config), (instant_leader_config), (effect_timing)));
    
    case 2:
      return ((Generate__leaderCond)((init_config), (instant_leader_config), (effect_timing)));
    
    case 3:
      return ((Generate__mpCond)((init_config), (instant_leader_config), (effect_timing)));
    
    case 4:
      return ((Generate__maxMpCond)((init_config), (instant_leader_config), (effect_timing)));
    
  }
  return 0;
}

Card__fieldExistenceCond  *Card::Generate__fieldExistenceCond(CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing)
{

  Card__fieldExistenceCond  *node = (new Card__fieldExistenceCond);
  (((node)->item) = (this));
  
  {
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    CondConfig tmp_config = (((TARGET_POS_FIELD) | (TARGET_IS_MINION)) | (TARGET_ANY_ALLE_MINION_ATTR_TYPE));
    (((node)->cond) = ((Generate__CharTargetCond)((tmp_init_config), (tmp_config), (TARGET_MODE_EXIST), (effect_timing))));
  }
  return (node);
}

Card__cardExistenceCond  *Card::Generate__cardExistenceCond(CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing)
{

  Card__cardExistenceCond  *node = (new Card__cardExistenceCond);
  (((node)->item) = (this));
  
  {
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    CondConfig tmp_config = (((TARGET_POS_HAND_OR_DECK) | (TARGET_IS_ALLY)) | (TARGET_ANY_CARD_MINION_ATTR_TYPE));
    (((node)->cond) = ((Generate__CardTargetCond)((tmp_init_config), (tmp_config), (TARGET_MODE_EXIST), (effect_timing))));
  }
  return (node);
}

Card__leaderCond  *Card::Generate__leaderCond(CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing)
{

  Card__leaderCond  *node = (new Card__leaderCond);
  (((node)->item) = (this));
  
  
  
  {
    CondConfig init_config = (((TARGET_POS_FIELD) | (TARGET_IS_LEADER)) | (TARGET_ANY_ALLE_TYPE));
    (((init_config).min_hp) = 1);
    CondConfig instant_config = ((LEADER_COND_FILTER) & (FIELD_COND_FILTER));
    (((instant_config).min_hp) = 1);
    (((node)->alle) = ((Generate__AllegianceCond)((init_config), (instant_config), (TARGET_MODE_LEADER), (effect_timing))));
    if ((!((((instant_leader_config).flag) & (~(((((node)->alle)->GetTargetConfig)()).flag))))))
    {
      ((instant_config) &= (instant_leader_config));
    } else {
      
    }
    (((node)->attrcond) = ((Generate__AttrCond)((init_config), (instant_config), (TARGET_MODE_LEADER), (effect_timing))));
    if (((((node)->attrcond)->isCondTrivial)()))
    {
      (((node)->statcond) = ((Generate__StatCond)((init_config), (instant_config), (TARGET_MODE_LEADER), (effect_timing), 1)));
    } else {
      (((node)->statcond) = ((Generate__StatCond)((init_config), (instant_config), (TARGET_MODE_LEADER), (effect_timing), 0)));
    }
  }
  return (node);
}

Card__mpCond  *Card::Generate__mpCond(CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing)
{

  Card__mpCond  *node = (new Card__mpCond);
  (((node)->item) = (this));
  
  
  {
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    CondConfig tmp_config = ((GetDefaultConfig)());
    (((node)->alle) = ((Generate__AllegianceCond)((tmp_init_config), (tmp_config), (TARGET_MODE_DEFAULT), (effect_timing))));
    signed int instant_min_mp = 0, instant_max_mp = 10;
    if ((!((((instant_leader_config).flag) & (~(((((node)->alle)->GetTargetConfig)()).flag))))))
    {
      {
        ((instant_min_mp) = ((instant_leader_config).min_mp));
        ((instant_max_mp) = ((instant_leader_config).max_mp));
      }
    } else {
      
    }
    signed int lower_min = 1;
    signed int lower_max = (instant_max_mp);
    signed int upper_min = (instant_min_mp);
    signed int upper_max = 9;
    (((node)->variant) = ((Generate__StatCondVariant)((lower_min), (lower_max), (upper_min), (upper_max))));
  }
  return (node);
}

Card__maxMpCond  *Card::Generate__maxMpCond(CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing)
{

  Card__maxMpCond  *node = (new Card__maxMpCond);
  (((node)->item) = (this));
  
  
  {
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    CondConfig tmp_config = ((GetDefaultConfig)());
    (((node)->alle) = ((Generate__AllegianceCond)((tmp_init_config), (tmp_config), (TARGET_MODE_DEFAULT), (effect_timing))));
    signed int instant_min_max_mp = 0, instant_max_max_mp = 10;
    if ((!((((instant_leader_config).flag) & (~(((((node)->alle)->GetTargetConfig)()).flag))))))
    {
      {
        ((instant_min_max_mp) = ((instant_leader_config).min_max_mp));
        ((instant_max_max_mp) = ((instant_leader_config).max_max_mp));
      }
    } else {
      
    }
    signed int lower_min = 1;
    signed int lower_max = (instant_max_max_mp);
    signed int upper_min = (instant_min_max_mp);
    signed int upper_max = 9;
    (((node)->variant) = ((Generate__StatCondVariant)((lower_min), (lower_max), (upper_min), (upper_max))));
  }
  return (node);
}

Card__BaseTargetedEff  *Card::Generate__BaseTargetedEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  double probs[26] = {(((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[88]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[89]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[90]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[91]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[92]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[93]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[94]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[95]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[96]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[97]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[98]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[99]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[100]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[101]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[102]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[103]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[104]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[105]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[106]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[107]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[108]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[109]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[110]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[111]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[112]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[113]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)))};
  ((NonegativeProbs)((probs), 26));
  {
    if (((target_mode) == (TARGET_MODE_LEADER)))
    {
      {
        (((probs)[3]) = 0.0);
        (((probs)[6]) = 0.0);
        (((probs)[7]) = 0.0);
        (((probs)[8]) = 0.0);
        (((probs)[9]) = 0.0);
        (((probs)[10]) = 0.0);
        (((probs)[11]) = 0.0);
        (((probs)[12]) = 0.0);
        (((probs)[13]) = 0.0);
        (((probs)[16]) = 0.0);
        
      }
    } else {
      if (((target_mode) == (TARGET_MODE_SELF)))
      {
        {
          if (((self_config) & (TARGET_IS_LEADER)))
          {
            {
              (((probs)[6]) = 0.0);
              (((probs)[11]) = 0.0);
              (((probs)[12]) = 0.0);
              (((probs)[13]) = 0.0);
              (((probs)[16]) = 0.0);
              
              if ((!(((self_config) & (TARGET_POS_HAND_OR_DECK)))))
              {
                (((probs)[8]) = 0.0);
                (((probs)[9]) = 0.0);
                (((probs)[10]) = 0.0);
                
              } else {
                
              }
            }
          } else {
            if (((self_config) & (TARGET_IS_SPELL)))
            {
              (((probs)[0]) = 0.0);
              (((probs)[1]) = 0.0);
              (((probs)[2]) = 0.0);
              (((probs)[4]) = 0.0);
              (((probs)[6]) = 0.0);
              (((probs)[5]) = 0.0);
              (((probs)[11]) = 0.0);
              (((probs)[12]) = 0.0);
              (((probs)[13]) = 0.0);
              (((probs)[14]) = 0.0);
              (((probs)[15]) = 0.0);
              (((probs)[16]) = 0.0);
              (((probs)[18]) = 0.0);
              
            } else {
              
            }
          }
          if ((!(((self_config) & (TARGET_NOT_LEADER)))))
          {
            (((probs)[0]) = 0.0);
            (((probs)[1]) = 0.0);
            (((probs)[2]) = 0.0);
            
          } else {
            
          }
          if (((effect_timing) == (EFFECT_TIMING_TURN)))
          {
            (((probs)[11]) = 0.0);
            (((probs)[12]) = 0.0);
            (((probs)[13]) = 0.0);
            (((probs)[14]) = 0.0);
            (((probs)[15]) = 0.0);
            (((probs)[17]) = 0.0);
            (((probs)[19]) = 0.0);
            (((probs)[20]) = 0.0);
            
          } else {
            
          }
          if ((!(((self_config) & (TARGET_NOT_BEAST)))))
          {
            (((probs)[11]) = 0.0);
            
          } else {
            
          }
          if ((!(((self_config) & (TARGET_NOT_DRAGON)))))
          {
            (((probs)[12]) = 0.0);
            
          } else {
            
          }
          if ((!(((self_config) & (TARGET_NOT_DEMON)))))
          {
            (((probs)[13]) = 0.0);
            
          } else {
            
          }
          if (((self_config) & (TARGET_IS_CHARGE)))
          {
            (((probs)[14]) = 0.0);
            
          } else {
            
          }
          if (((self_config) & (TARGET_IS_TAUNT)))
          {
            (((probs)[15]) = 0.0);
            
          } else {
            
          }
          if (((((self_config) & (TARGET_IS_STEALTH))) && ((effect_timing) == (EFFECT_TIMING_PLAY))))
          {
            (((probs)[16]) = 0.0);
            
          } else {
            
          }
          if (((self_config) & (TARGET_IS_UNTARGETABLE)))
          {
            (((probs)[17]) = 0.0);
            
          } else {
            
          }
          if (((((self_config) & (TARGET_IS_SHIELDED))) && ((effect_timing) == (EFFECT_TIMING_PLAY))))
          {
            (((probs)[18]) = 0.0);
            
          } else {
            
          }
          if (((self_config) & (TARGET_IS_POISONOUS)))
          {
            (((probs)[19]) = 0.0);
            
          } else {
            
          }
          if (((self_config) & (TARGET_IS_LIFESTEAL)))
          {
            (((probs)[20]) = 0.0);
            
          } else {
            
          }
        }
      } else {
        
      }
    }
    if (((effect_depth) >= (max_eff_depth)))
    {
      (((probs)[25]) = 0.0);
      (((probs)[10]) = 0.0);
      
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 26));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 26));
  switch ((choice))
  {
    case 0:
      return ((Generate__damageEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 1:
      return ((Generate__healEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 2:
      return ((Generate__resAtkTimesEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 3:
      return ((Generate__costModEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 4:
      return ((Generate__atkHpModEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 5:
      return ((Generate__atkTimesModEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 6:
      return ((Generate__destroyEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 7:
      return ((Generate__discardEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 8:
      return ((Generate__moveEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 9:
      return ((Generate__copyEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 10:
      return ((Generate__transformEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 11:
      return ((Generate__changeToBeastEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 12:
      return ((Generate__changeToDragonEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 13:
      return ((Generate__changeToDemonEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 14:
      return ((Generate__giveChargeEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 15:
      return ((Generate__giveTauntEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 16:
      return ((Generate__giveStealthEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 17:
      return ((Generate__giveUntargetableEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 18:
      return ((Generate__giveShieldEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 19:
      return ((Generate__givePoisonousEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 20:
      return ((Generate__giveLifestealEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 21:
      return ((Generate__rmAttributesEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 22:
      return ((Generate__setOverheatEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 23:
      return ((Generate__decOhThresholdEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 24:
      return ((Generate__resetStateEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
    case 25:
      return ((Generate__giveEffectsEff)((self_config), (target_mode), (effect_timing), (effect_depth), (give_eff)));
    
  }
  return 0;
}

Card__damageEff  *Card::Generate__damageEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__damageEff  *node = (new Card__damageEff);
  (((node)->item) = (this));
  
  
  {
    (((node)->val) = ((GetRandInt)(1, 10)));
    if ((give_eff))
    {
      (((node)->attr) = (new Card__damageAttributes((this), (new Card__noPoisonous((this))), (new Card__noLifesteal((this))))));
    } else {
      (((node)->attr) = ((Generate__DamageAttributes)((self_config), (target_mode), ((node)->val))));
    }
  }
  return (node);
}

Card__healEff  *Card::Generate__healEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__healEff  *node = (new Card__healEff);
  (((node)->item) = (this));
  
  {
    (((node)->val) = ((GetRandInt)(1, 10)));
  }
  return (node);
}

Card__resAtkTimesEff  *Card::Generate__resAtkTimesEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__resAtkTimesEff  *node = (new Card__resAtkTimesEff);
  (((node)->item) = (this));
  
  {
    (((node)->val) = ((GetRandInt)(1, 5)));
  }
  return (node);
}

Card__costModEff  *Card::Generate__costModEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__costModEff  *node = (new Card__costModEff);
  (((node)->item) = (this));
  (((node)->inc_prob) = (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[114]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))));
  
  {
    (((node)->val) = ((GetRandInt)(1, 10)));
    if ((((RandomRoll)((1.0 - ((node)->inc_prob)))) && ((((target_mode) != (TARGET_MODE_SELF)) || (((self_config).max_cost) > 0)))))
    {
      {
        (((node)->val) = (-((node)->val)));
        if ((((target_mode) == (TARGET_MODE_SELF)) && (((node)->val) < (-((self_config).max_cost)))))
        {
          (((node)->val) = (-((self_config).max_cost)));
        } else {
          
        }
      }
    } else {
      
    }
  }
  return (node);
}

Card__atkHpModEff  *Card::Generate__atkHpModEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__atkHpModEff  *node = (new Card__atkHpModEff);
  (((node)->item) = (this));
  
  
  {
    (((node)->atkmod) = ((GetRandInt)((-5), 5)));
    if ((((target_mode) == (TARGET_MODE_SELF)) && (((node)->atkmod) < (-((self_config).max_atk)))))
    {
      (((node)->atkmod) = (-((self_config).max_atk)));
    } else {
      
    }
    if ((((node)->atkmod) == 0))
    {
      (((node)->hpmod) = ((GetRandInt)(1, 10)));
    } else {
      (((node)->hpmod) = ((GetRandInt)(0, 10)));
    }
  }
  return (node);
}

Card__atkTimesModEff  *Card::Generate__atkTimesModEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__atkTimesModEff  *node = (new Card__atkTimesModEff);
  (((node)->item) = (this));
  (((node)->dec_prob) = (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  , signed int  ))(((config).funcs)[115]))((config), (self_config), (target_mode), (effect_timing), (effect_depth), (give_eff))));
  
  {
    (((node)->val) = ((GetRandInt)(1, 3)));
    if ((((RandomRoll)(((node)->dec_prob))) && ((((target_mode) != (TARGET_MODE_SELF)) || (((self_config).max_n_atks) > 0)))))
    {
      {
        (((node)->val) = (-((node)->val)));
        if ((((target_mode) == (TARGET_MODE_SELF)) && (((node)->val) < (-((self_config).max_n_atks)))))
        {
          (((node)->val) = (-((self_config).max_n_atks)));
        } else {
          
        }
      }
    } else {
      
    }
  }
  return (node);
}

Card__destroyEff  *Card::Generate__destroyEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__destroyEff  *node = (new Card__destroyEff);
  (((node)->item) = (this));
  
  return (node);
}

Card__discardEff  *Card::Generate__discardEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__discardEff  *node = (new Card__discardEff);
  (((node)->item) = (this));
  
  return (node);
}

Card__moveEff  *Card::Generate__moveEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__moveEff  *node = (new Card__moveEff);
  (((node)->item) = (this));
  
  {
    (((node)->dest) = ((Generate__Destination)((self_config), (target_mode), (effect_timing), (TARGET_MODE_MOVE_DEST))));
  }
  return (node);
}

Card__copyEff  *Card::Generate__copyEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__copyEff  *node = (new Card__copyEff);
  (((node)->item) = (this));
  
  {
    (((node)->dest) = ((Generate__Destination)((self_config), (target_mode), (effect_timing), (TARGET_MODE_COPY_DEST))));
  }
  return (node);
}

Card__transformEff  *Card::Generate__transformEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__transformEff  *node = (new Card__transformEff);
  (((node)->item) = (this));
  
  {
    (((node)->variant) = ((Generate__NewCardVariant)((self_config), (target_mode), (effect_timing), (effect_depth))));
  }
  return (node);
}

Card__changeToBeastEff  *Card::Generate__changeToBeastEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__changeToBeastEff  *node = (new Card__changeToBeastEff);
  (((node)->item) = (this));
  
  return (node);
}

Card__changeToDragonEff  *Card::Generate__changeToDragonEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__changeToDragonEff  *node = (new Card__changeToDragonEff);
  (((node)->item) = (this));
  
  return (node);
}

Card__changeToDemonEff  *Card::Generate__changeToDemonEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__changeToDemonEff  *node = (new Card__changeToDemonEff);
  (((node)->item) = (this));
  
  return (node);
}

Card__giveChargeEff  *Card::Generate__giveChargeEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__giveChargeEff  *node = (new Card__giveChargeEff);
  (((node)->item) = (this));
  
  return (node);
}

Card__giveTauntEff  *Card::Generate__giveTauntEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__giveTauntEff  *node = (new Card__giveTauntEff);
  (((node)->item) = (this));
  
  return (node);
}

Card__giveStealthEff  *Card::Generate__giveStealthEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__giveStealthEff  *node = (new Card__giveStealthEff);
  (((node)->item) = (this));
  
  return (node);
}

Card__giveUntargetableEff  *Card::Generate__giveUntargetableEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__giveUntargetableEff  *node = (new Card__giveUntargetableEff);
  (((node)->item) = (this));
  
  return (node);
}

Card__giveShieldEff  *Card::Generate__giveShieldEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__giveShieldEff  *node = (new Card__giveShieldEff);
  (((node)->item) = (this));
  
  return (node);
}

Card__givePoisonousEff  *Card::Generate__givePoisonousEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__givePoisonousEff  *node = (new Card__givePoisonousEff);
  (((node)->item) = (this));
  
  return (node);
}

Card__giveLifestealEff  *Card::Generate__giveLifestealEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__giveLifestealEff  *node = (new Card__giveLifestealEff);
  (((node)->item) = (this));
  
  return (node);
}

Card__rmAttributesEff  *Card::Generate__rmAttributesEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__rmAttributesEff  *node = (new Card__rmAttributesEff);
  (((node)->item) = (this));
  
  return (node);
}

Card__setOverheatEff  *Card::Generate__setOverheatEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__setOverheatEff  *node = (new Card__setOverheatEff);
  (((node)->item) = (this));
  
  return (node);
}

Card__decOhThresholdEff  *Card::Generate__decOhThresholdEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__decOhThresholdEff  *node = (new Card__decOhThresholdEff);
  (((node)->item) = (this));
  
  {
    (((node)->val) = ((GetRandInt)(1, 5)));
  }
  return (node);
}

Card__resetStateEff  *Card::Generate__resetStateEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__resetStateEff  *node = (new Card__resetStateEff);
  (((node)->item) = (this));
  
  return (node);
}

Card__giveEffectsEff  *Card::Generate__giveEffectsEff(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__giveEffectsEff  *node = (new Card__giveEffectsEff);
  (((node)->item) = (this));
  
  {
    signed int next_depth = ((effect_depth) + 1);
    signed int min_num_effs = 1;
    signed int max_num_effs = ((max_eff_num) >> (next_depth));
    CondConfig tmp_config = ((GetDefaultInitConfig)());
    if (((target_mode) == (TARGET_MODE_LEADER)))
    {
      {
        ((tmp_config) &= (LEADER_COND_FILTER));
        ((tmp_config) &= (FIELD_COND_FILTER));
      }
    } else {
      if (((target_mode) == (TARGET_MODE_SELF)))
      {
        ((tmp_config) = (self_config));
      } else {
        
      }
    }
    (((node)->effects) = ((Generate__SpecialEffects)((tmp_config), (min_num_effs), (max_num_effs), (next_depth), 1)));
  }
  return (node);
}

Card__BaseUntargetedEff  *Card::Generate__BaseUntargetedEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  double probs[9] = {(((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , signed int  , signed int  ))(((config).funcs)[116]))((config), (self_config), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , signed int  , signed int  ))(((config).funcs)[117]))((config), (self_config), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , signed int  , signed int  ))(((config).funcs)[118]))((config), (self_config), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , signed int  , signed int  ))(((config).funcs)[119]))((config), (self_config), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , signed int  , signed int  ))(((config).funcs)[120]))((config), (self_config), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , signed int  , signed int  ))(((config).funcs)[121]))((config), (self_config), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , signed int  , signed int  ))(((config).funcs)[122]))((config), (self_config), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , signed int  , signed int  ))(((config).funcs)[123]))((config), (self_config), (effect_timing), (effect_depth), (give_eff))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , signed int  , signed int  ))(((config).funcs)[124]))((config), (self_config), (effect_timing), (effect_depth), (give_eff)))};
  ((NonegativeProbs)((probs), 9));
  {
    if ((((((effect_timing) == (EFFECT_TIMING_DESTROY)) || ((effect_timing) == (EFFECT_TIMING_DISCARD))) || (((((self_config) & (TARGET_IS_SPELL))) && ((effect_timing) == (EFFECT_TIMING_PLAY))))) || (((!(((self_config) & (TARGET_NOT_LEADER)))) && (!(((self_config) & (TARGET_POS_HAND_OR_DECK))))))))
    {
      (((probs)[3]) = 0.0);
      
    } else {
      
    }
    if (((effect_depth) >= (max_eff_depth)))
    {
      (((probs)[7]) = 0.0);
      
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 9));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 9));
  switch ((choice))
  {
    case 0:
      return ((Generate__aoeEff)((self_config), (effect_timing), (effect_depth), (give_eff)));
    
    case 1:
      return ((Generate__randEff)((self_config), (effect_timing), (effect_depth), (give_eff)));
    
    case 2:
      return ((Generate__leaderEff)((self_config), (effect_timing), (effect_depth), (give_eff)));
    
    case 3:
      return ((Generate__selfEff)((self_config), (effect_timing), (effect_depth), (give_eff)));
    
    case 4:
      return ((Generate__drawCardEff)((self_config), (effect_timing), (effect_depth), (give_eff)));
    
    case 5:
      return ((Generate__mpEff)((self_config), (effect_timing), (effect_depth), (give_eff)));
    
    case 6:
      return ((Generate__maxMpEff)((self_config), (effect_timing), (effect_depth), (give_eff)));
    
    case 7:
      return ((Generate__newEff)((self_config), (effect_timing), (effect_depth), (give_eff)));
    
    case 8:
      return ((Generate__winGameEff)((self_config), (effect_timing), (effect_depth), (give_eff)));
    
  }
  return 0;
}

Card__aoeEff  *Card::Generate__aoeEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__aoeEff  *node = (new Card__aoeEff);
  (((node)->item) = (this));
  
  
  {
    (((node)->effect) = ((Generate__BaseTargetedEff)((self_config), (TARGET_MODE_DEFAULT), (effect_timing), (effect_depth), (give_eff))));
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    CondConfig tmp_config = ((((node)->effect)->GetTargetConfig)());
    (((node)->cond) = ((Generate__TargetCond)((tmp_init_config), (tmp_config), (TARGET_MODE_DEFAULT), (effect_timing))));
  }
  return (node);
}

Card__randEff  *Card::Generate__randEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__randEff  *node = (new Card__randEff);
  (((node)->item) = (this));
  
  
  {
    (((node)->effect) = ((Generate__BaseTargetedEff)((self_config), (TARGET_MODE_DEFAULT), (effect_timing), (effect_depth), (give_eff))));
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    CondConfig tmp_config = ((((node)->effect)->GetTargetConfig)());
    (((node)->cond) = ((Generate__TargetCond)((tmp_init_config), (tmp_config), (TARGET_MODE_DEFAULT), (effect_timing))));
  }
  return (node);
}

Card__leaderEff  *Card::Generate__leaderEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__leaderEff  *node = (new Card__leaderEff);
  (((node)->item) = (this));
  
  
  {
    (((node)->effect) = ((Generate__BaseTargetedEff)((self_config), (TARGET_MODE_LEADER), (effect_timing), (effect_depth), (give_eff))));
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    CondConfig tmp_config = ((((node)->effect)->GetTargetConfig)());
    (((node)->alle) = ((Generate__AllegianceCond)((tmp_init_config), (tmp_config), (TARGET_MODE_LEADER), (effect_timing))));
  }
  return (node);
}

Card__selfEff  *Card::Generate__selfEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__selfEff  *node = (new Card__selfEff);
  (((node)->item) = (this));
  
  {
    (((node)->effect) = ((Generate__BaseTargetedEff)((self_config), (TARGET_MODE_SELF), (effect_timing), (effect_depth), (give_eff))));
  }
  return (node);
}

Card__drawCardEff  *Card::Generate__drawCardEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__drawCardEff  *node = (new Card__drawCardEff);
  (((node)->item) = (this));
  
  
  {
    (((node)->val) = (10 / ((GetRandInt)(3, 10))));
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    CondConfig tmp_config = ((GetDefaultConfig)());
    (((node)->alle) = ((Generate__AllegianceCond)((tmp_init_config), (tmp_config), (TARGET_MODE_DEFAULT), (effect_timing))));
  }
  return (node);
}

Card__mpEff  *Card::Generate__mpEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__mpEff  *node = (new Card__mpEff);
  (((node)->item) = (this));
  (((node)->dec_prob) = (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , signed int  , signed int  ))(((config).funcs)[125]))((config), (self_config), (effect_timing), (effect_depth), (give_eff))));
  
  
  {
    (((node)->val) = (200 / ((GetRandInt)(18, 100))));
    if (((RandomRoll)(((node)->dec_prob))))
    {
      (((node)->val) = (-((node)->val)));
    } else {
      
    }
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    CondConfig tmp_config = ((GetDefaultConfig)());
    (((node)->alle) = ((Generate__AllegianceCond)((tmp_init_config), (tmp_config), (TARGET_MODE_DEFAULT), (effect_timing))));
  }
  return (node);
}

Card__maxMpEff  *Card::Generate__maxMpEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__maxMpEff  *node = (new Card__maxMpEff);
  (((node)->item) = (this));
  (((node)->dec_prob) = (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , signed int  , signed int  ))(((config).funcs)[126]))((config), (self_config), (effect_timing), (effect_depth), (give_eff))));
  
  
  {
    (((node)->val) = (100 / ((GetRandInt)(18, 100))));
    if (((RandomRoll)(((node)->dec_prob))))
    {
      (((node)->val) = (-((node)->val)));
    } else {
      
    }
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    CondConfig tmp_config = ((GetDefaultConfig)());
    (((node)->alle) = ((Generate__AllegianceCond)((tmp_init_config), (tmp_config), (TARGET_MODE_DEFAULT), (effect_timing))));
  }
  return (node);
}

Card__newEff  *Card::Generate__newEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__newEff  *node = (new Card__newEff);
  (((node)->item) = (this));
  
  
  
  {
    (((node)->val) = (120 / ((GetRandInt)(16, 120))));
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    (((node)->dest) = ((Generate__Destination)((tmp_init_config), (TARGET_MODE_NEW), (effect_timing), (TARGET_MODE_NEW_DEST))));
    CondConfig tmp_config = ((((node)->dest)->GetTargetConfig)());
    (((node)->variant) = ((Generate__NewCardVariant)((tmp_config), (TARGET_MODE_NEW), (effect_timing), (effect_depth))));
  }
  return (node);
}

Card__winGameEff  *Card::Generate__winGameEff(CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  Card__winGameEff  *node = (new Card__winGameEff);
  (((node)->item) = (this));
  
  {
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    CondConfig tmp_config = ((GetDefaultConfig)());
    (((node)->alle) = ((Generate__AllegianceCond)((tmp_init_config), (tmp_config), (TARGET_MODE_WIN_GAME), (effect_timing))));
  }
  return (node);
}

Card__Destination  *Card::Generate__Destination(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode)
{

  double probs[6] = {(((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , unsigned int  ))(((config).funcs)[127]))((config), (self_config), (target_mode), (effect_timing), (dest_mode))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , unsigned int  ))(((config).funcs)[128]))((config), (self_config), (target_mode), (effect_timing), (dest_mode))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , unsigned int  ))(((config).funcs)[129]))((config), (self_config), (target_mode), (effect_timing), (dest_mode))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , unsigned int  ))(((config).funcs)[130]))((config), (self_config), (target_mode), (effect_timing), (dest_mode))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , unsigned int  ))(((config).funcs)[131]))((config), (self_config), (target_mode), (effect_timing), (dest_mode))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , unsigned int  ))(((config).funcs)[132]))((config), (self_config), (target_mode), (effect_timing), (dest_mode)))};
  ((NonegativeProbs)((probs), 6));
  {
    if (((target_mode) == (TARGET_MODE_SELF)))
    {
      {
        if (((((self_config) & (TARGET_IS_LEADER))) || (((self_config) & (TARGET_IS_SPELL)))))
        {
          (((probs)[0]) = 0.0);
          (((probs)[1]) = 0.0);
          
        } else {
          
        }
        if (((dest_mode) == (TARGET_MODE_MOVE_DEST)))
        {
          {
            if ((!(((self_config) & (TARGET_POS_HAND_OR_DECK)))))
            {
              (((probs)[1]) = 0.0);
              
            } else {
              
            }
            if ((!(((self_config) & (TARGET_NOT_HAND)))))
            {
              (((probs)[3]) = 0.0);
              
            } else {
              
            }
            if ((!(((self_config) & (TARGET_NOT_DECK)))))
            {
              (((probs)[5]) = 0.0);
              
            } else {
              
            }
          }
        } else {
          if (((dest_mode) == (TARGET_MODE_COPY_DEST)))
          {
            {
              if ((!(((self_config) & (TARGET_NOT_DECK)))))
              {
                (((probs)[4]) = 0.0);
                (((probs)[5]) = 0.0);
                
              } else {
                
              }
            }
          } else {
            
          }
        }
      }
    } else {
      
    }
    if (((target_mode) = (TARGET_MODE_NEW)))
    {
      {
        (((probs)[1]) = 0.0);
        (((probs)[3]) = 0.0);
        (((probs)[5]) = 0.0);
        
      }
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 6));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 6));
  switch ((choice))
  {
    case 0:
      return ((Generate__fieldDest)((self_config), (target_mode), (effect_timing), (dest_mode)));
    
    case 1:
      return ((Generate__ownerFieldDest)((self_config), (target_mode), (effect_timing), (dest_mode)));
    
    case 2:
      return ((Generate__handDest)((self_config), (target_mode), (effect_timing), (dest_mode)));
    
    case 3:
      return ((Generate__ownerHandDest)((self_config), (target_mode), (effect_timing), (dest_mode)));
    
    case 4:
      return ((Generate__deckDest)((self_config), (target_mode), (effect_timing), (dest_mode)));
    
    case 5:
      return ((Generate__ownerDeckDest)((self_config), (target_mode), (effect_timing), (dest_mode)));
    
  }
  return 0;
}

Card__fieldDest  *Card::Generate__fieldDest(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode)
{

  Card__fieldDest  *node = (new Card__fieldDest);
  (((node)->item) = (this));
  
  {
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    CondConfig tmp_config = ((GetDefaultConfig)());
    if (((((target_mode) == (TARGET_MODE_SELF)) && ((dest_mode) == (TARGET_MODE_MOVE_DEST))) && (!(((self_config) & (TARGET_POS_HAND_OR_DECK))))))
    {
      ((tmp_config) = (OPPO_COND_FILTER));
    } else {
      
    }
    (((node)->alle) = ((Generate__AllegianceCond)((tmp_init_config), (tmp_config), (dest_mode), (effect_timing))));
  }
  return (node);
}

Card__ownerFieldDest  *Card::Generate__ownerFieldDest(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode)
{

  Card__ownerFieldDest  *node = (new Card__ownerFieldDest);
  (((node)->item) = (this));
  
  return (node);
}

Card__handDest  *Card::Generate__handDest(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode)
{

  Card__handDest  *node = (new Card__handDest);
  (((node)->item) = (this));
  
  {
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    CondConfig tmp_config = ((GetDefaultConfig)());
    if (((((target_mode) == (TARGET_MODE_SELF)) && ((dest_mode) == (TARGET_MODE_MOVE_DEST))) && (!(((self_config) & (TARGET_NOT_HAND))))))
    {
      ((tmp_config) = (OPPO_COND_FILTER));
    } else {
      
    }
    (((node)->alle) = ((Generate__AllegianceCond)((tmp_init_config), (tmp_config), (dest_mode), (effect_timing))));
  }
  return (node);
}

Card__ownerHandDest  *Card::Generate__ownerHandDest(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode)
{

  Card__ownerHandDest  *node = (new Card__ownerHandDest);
  (((node)->item) = (this));
  
  return (node);
}

Card__deckDest  *Card::Generate__deckDest(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode)
{

  Card__deckDest  *node = (new Card__deckDest);
  (((node)->item) = (this));
  
  {
    CondConfig tmp_init_config = ((GetDefaultInitConfig)());
    CondConfig tmp_config = ((GetDefaultConfig)());
    if (((((target_mode) == (TARGET_MODE_SELF)) && ((dest_mode) == (TARGET_MODE_MOVE_DEST))) && (!(((self_config) & (TARGET_NOT_DECK))))))
    {
      ((tmp_config) = (OPPO_COND_FILTER));
    } else {
      
    }
    (((node)->alle) = ((Generate__AllegianceCond)((tmp_init_config), (tmp_config), (dest_mode), (effect_timing))));
  }
  return (node);
}

Card__ownerDeckDest  *Card::Generate__ownerDeckDest(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode)
{

  Card__ownerDeckDest  *node = (new Card__ownerDeckDest);
  (((node)->item) = (this));
  
  return (node);
}

Card__NewCardVariant  *Card::Generate__NewCardVariant(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth)
{

  double probs[6] = {(((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  ))(((config).funcs)[133]))((config), (self_config), (target_mode), (effect_timing), (effect_depth))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  ))(((config).funcs)[134]))((config), (self_config), (target_mode), (effect_timing), (effect_depth))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  ))(((config).funcs)[135]))((config), (self_config), (target_mode), (effect_timing), (effect_depth))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  ))(((config).funcs)[136]))((config), (self_config), (target_mode), (effect_timing), (effect_depth))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  ))(((config).funcs)[137]))((config), (self_config), (target_mode), (effect_timing), (effect_depth))), (((double (*)(const GiglConfig  & , CondConfig  & , unsigned int  , unsigned int  , signed int  ))(((config).funcs)[138]))((config), (self_config), (target_mode), (effect_timing), (effect_depth)))};
  ((NonegativeProbs)((probs), 6));
  {
    if ((((target_mode) == (TARGET_MODE_SELF)) || ((target_mode) == (TARGET_MODE_NEW))))
    {
      {
        if (((!(((self_config) & (TARGET_POS_FIELD)))) || (!(((self_config) & (TARGET_NOT_LEADER))))))
        {
          (((probs)[1]) = 0.0);
          (((probs)[3]) = 0.0);
          (((probs)[5]) = 0.0);
          
        } else {
          
        }
        if ((!(((self_config) & (TARGET_POS_HAND_OR_DECK)))))
        {
          (((probs)[0]) = 0.0);
          (((probs)[2]) = 0.0);
          (((probs)[4]) = 0.0);
          
        } else {
          
        }
      }
    } else {
      
    }
  }
  ((NormalizeProbs)((probs), 6));
  signed int choice = ((GetRandChoiceFromProbs)((probs), 6));
  switch ((choice))
  {
    case 0:
      return ((Generate__plainMinionCard)((self_config), (target_mode), (effect_timing), (effect_depth)));
    
    case 1:
      return ((Generate__plainMinion)((self_config), (target_mode), (effect_timing), (effect_depth)));
    
    case 2:
      return ((Generate__fixedCard)((self_config), (target_mode), (effect_timing), (effect_depth)));
    
    case 3:
      return ((Generate__fixedMinion)((self_config), (target_mode), (effect_timing), (effect_depth)));
    
    case 4:
      return ((Generate__randomCard)((self_config), (target_mode), (effect_timing), (effect_depth)));
    
    case 5:
      return ((Generate__randomMinion)((self_config), (target_mode), (effect_timing), (effect_depth)));
    
  }
  return 0;
}

Card__plainMinionCard  *Card::Generate__plainMinionCard(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth)
{

  Card__plainMinionCard  *node = (new Card__plainMinionCard);
  (((node)->item) = (this));
  
  {
    (((node)->card) = ((CreatePlainMinion)((name))));
  }
  return (node);
}

Card__plainMinion  *Card::Generate__plainMinion(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth)
{

  Card__plainMinion  *node = (new Card__plainMinion);
  (((node)->item) = (this));
  
  {
    (((node)->card) = ((CreatePlainMinion)((name))));
  }
  return (node);
}

Card__fixedCard  *Card::Generate__fixedCard(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth)
{

  Card__fixedCard  *node = (new Card__fixedCard);
  (((node)->item) = (this));
  
  {
    signed int next_depth = ((effect_depth) + 1);
    signed int min_num_effs = 1;
    signed int max_num_effs = ((max_eff_num) >> (next_depth));
    (((node)->card) = ((CreateRandomCard)((name), ((GetRandInt)(0, 10)), (min_num_effs), (max_num_effs), (next_depth))));
  }
  return (node);
}

Card__fixedMinion  *Card::Generate__fixedMinion(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth)
{

  Card__fixedMinion  *node = (new Card__fixedMinion);
  (((node)->item) = (this));
  
  {
    signed int next_depth = ((effect_depth) + 1);
    signed int min_num_effs = 1;
    signed int max_num_effs = ((max_eff_num) >> (next_depth));
    (((node)->card) = ((CreateRandomMinion)((name), ((GetRandInt)(0, 10)), (min_num_effs), (max_num_effs), (next_depth))));
  }
  return (node);
}

Card__randomCard  *Card::Generate__randomCard(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth)
{

  Card__randomCard  *node = (new Card__randomCard);
  (((node)->item) = (this));
  
  {
    (((node)->cost) = ((GetRandInt)(0, 10)));
  }
  return (node);
}

Card__randomMinion  *Card::Generate__randomMinion(CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth)
{

  Card__randomMinion  *node = (new Card__randomMinion);
  (((node)->item) = (this));
  
  {
    (((node)->cost) = ((GetRandInt)(0, 10)));
  }
  return (node);
}



Card__Node::Card__Node(Card  * input__item) : item((input__item))
{

  
}

Card__Node::Card__Node()
{

  
}

Card__Node::~Card__Node()
{

  
}



string Card__Node::Brief()
{

  {
    return "";
  }
}

string Card__Node::Detail()
{

  {
    return "";
  }
}

string Card__Node::DetailIndent(signed int  indent_size)
{

  {
    return (((RepeatSpace)((indent_size))) + ((Detail)()));
  }
}

string Card__Node::DetailAlt1()
{

  {
    return ((Detail)());
  }
}

string Card__Node::DetailAlt2()
{

  {
    return ((Detail)());
  }
}

string Card__Node::DetailAlt3()
{

  {
    return ((Detail)());
  }
}

string Card__Node::DetailAlt4()
{

  {
    return ((Detail)());
  }
}

string Card__Node::DetailAlt5()
{

  {
    return ((Detail)());
  }
}

string Card__Node::DetailAlt6()
{

  {
    return ((Detail)());
  }
}

string Card__Node::Suffix()
{

  {
    return "";
  }
}

string Card__Node::SuffixAlt1()
{

  {
    return ((Suffix)());
  }
}

string Card__Node::Postfix()
{

  {
    return "\n";
  }
}

string Card__Node::PostfixIndent(signed int  indent_size)
{

  {
    return ((Postfix)());
  }
}

signed int Card__Node::IsPlural()
{

  {
    return 0;
  }
}

signed int Card__Node::IsThirdPersonSingle()
{

  {
    return 0;
  }
}

Card__Node  *Card__Node::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  {
    return 0;
  }
}

signed int Card__Node::GetEffectNum()
{

  {
    return 0;
  }
}

Card__SpecialEffects  *Card__Node::GetEffects()
{

  {
    return 0;
  }
}

Card  *Card__Node::GetRelatedCard(Card  * parent_card)
{

  {
    return 0;
  }
}

signed int Card__Node::isCondTrivial()
{

  {
    return 0;
  }
}

unsigned int Card__Node::GetInitAttrFlag()
{

  {
    return (TARGET_TYPE_NOTHING);
  }
}

CondConfig Card__Node::GetPlayTargetConfig()
{

  {
    return ((GetTargetConfig)());
  }
}

CondConfig Card__Node::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    return ((GetTargetConfig)());
  }
}

CondConfig Card__Node::GetSelfConfig(const CondConfig  & self_config)
{

  {
    return ((GetTargetConfig)());
  }
}

CondConfig Card__Node::GetLeaderConfig()
{

  {
    return ((GetTargetConfig)());
  }
}

CondConfig Card__Node::GetTargetConfig()
{

  {
    return ((GetDefaultConfig)());
  }
}

void Card__Node::AdjustGlobalStatRange(signed int  & min_val, signed int  & max_val)
{

  
}

signed int Card__Node::isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card)
{

  {
    return 0;
  }
}

signed int Card__Node::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  {
    return 1;
  }
}

void Card__Node::Play(signed int  x, signed int  y, signed int  z, Card  * parent_card)
{

  
}

void Card__Node::Destroy(Card  * parent_card)
{

  
}

void Card__Node::Discard(Card  * parent_card)
{

  
}

signed int Card__Node::CheckAttackValid(signed int  x, signed int  z, Card  * parent_card)
{

  {
    return 1;
  }
}

void Card__Node::Attack(signed int  x, signed int  z, Card  * parent_card)
{

  
}

void Card__Node::TurnStart(Card  * leader, Card  * parent_card)
{

  
}

void Card__Node::TurnEnd(Card  * leader, Card  * parent_card)
{

  
}

signed int Card__Node::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    return 0;
  }
}

void Card__Node::SubTargetedAction(Card  * target, Card  * parent_card, signed int  start_of_batch)
{

  
}

void Card__Node::UntargetedAction(Card  * parent_card)
{

  
}

signed int Card__Node::CheckCardValid(Card  * card, Card  * parent_card)
{

  {
    return 1;
  }
}

signed int Card__Node::CheckThisValid(Card  * parent_card)
{

  {
    return 1;
  }
}

signed int Card__Node::CheckStatValid(signed int  stat_val)
{

  {
    return 1;
  }
}

void Card__Node::Mutate(signed int  min_eff_n, signed int  max_eff_n, signed int  effect_depth)
{

  
}

void Card__Node::SetOverheatCounts(signed int  val)
{

  {
    ((overheat_count) = (val));
  }
}

void Card__Node::SetOverheatThresholds(signed int  val)
{

  {
    ((overheat_threshold) = (val));
    if (((overheat_threshold) < 0))
    {
      ((overheat_threshold) = 0);
    } else {
      if (((overheat_threshold) > 10))
      {
        ((overheat_threshold) = 10);
      } else {
        
      }
    }
  }
}

void Card__Node::ModOverheatThresholds(signed int  amount)
{

  {
    ((overheat_threshold) += (amount));
    if (((overheat_threshold) < 0))
    {
      ((overheat_threshold) = 0);
    } else {
      if (((overheat_threshold) > 10))
      {
        ((overheat_threshold) = 10);
      } else {
        
      }
    }
  }
}






Card__CardRoot::Card__CardRoot(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__CardRoot::Card__CardRoot()
{

  
}



Card__AttackTimes::Card__AttackTimes(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__AttackTimes::Card__AttackTimes()
{

  
}



Card__MinionType::Card__MinionType(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__MinionType::Card__MinionType()
{

  
}



Card__Attributes::Card__Attributes(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__Attributes::Card__Attributes()
{

  
}



Card__DamageAttributes::Card__DamageAttributes(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__DamageAttributes::Card__DamageAttributes()
{

  
}



Card__ChargeAttr::Card__ChargeAttr(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__ChargeAttr::Card__ChargeAttr()
{

  
}



Card__TauntAttr::Card__TauntAttr(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__TauntAttr::Card__TauntAttr()
{

  
}



Card__StealthAttr::Card__StealthAttr(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__StealthAttr::Card__StealthAttr()
{

  
}



Card__UntargetableAttr::Card__UntargetableAttr(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__UntargetableAttr::Card__UntargetableAttr()
{

  
}



Card__ShieldAttr::Card__ShieldAttr(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__ShieldAttr::Card__ShieldAttr()
{

  
}



Card__PoisonousAttr::Card__PoisonousAttr(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__PoisonousAttr::Card__PoisonousAttr()
{

  
}



Card__LifestealAttr::Card__LifestealAttr(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__LifestealAttr::Card__LifestealAttr()
{

  
}



Card__SpecialEffects::Card__SpecialEffects(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__SpecialEffects::Card__SpecialEffects()
{

  
}



Card__TargetedPlayEff::Card__TargetedPlayEff(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__TargetedPlayEff::Card__TargetedPlayEff()
{

  
}



Card__OtherEffs::Card__OtherEffs(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__OtherEffs::Card__OtherEffs()
{

  
}



Card__OtherEff::Card__OtherEff(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__OtherEff::Card__OtherEff()
{

  
}



Card__TargetedEff::Card__TargetedEff(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__TargetedEff::Card__TargetedEff()
{

  
}



Card__UntargetedEff::Card__UntargetedEff(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__UntargetedEff::Card__UntargetedEff()
{

  
}



Card__TargetCond::Card__TargetCond(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__TargetCond::Card__TargetCond()
{

  
}



Card__CharTargetCond::Card__CharTargetCond(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__CharTargetCond::Card__CharTargetCond()
{

  
}



Card__CharTypeCond::Card__CharTypeCond(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__CharTypeCond::Card__CharTypeCond()
{

  
}



Card__CardTargetCond::Card__CardTargetCond(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__CardTargetCond::Card__CardTargetCond()
{

  
}



Card__CardTypeCond::Card__CardTypeCond(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__CardTypeCond::Card__CardTypeCond()
{

  
}



Card__CardPosCond::Card__CardPosCond(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__CardPosCond::Card__CardPosCond()
{

  
}



Card__AllegianceCond::Card__AllegianceCond(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__AllegianceCond::Card__AllegianceCond()
{

  
}



Card__AttrCond::Card__AttrCond(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__AttrCond::Card__AttrCond()
{

  
}



Card__StatCond::Card__StatCond(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__StatCond::Card__StatCond()
{

  
}



Card__StatCondVariant::Card__StatCondVariant(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__StatCondVariant::Card__StatCondVariant()
{

  
}



Card__IndeCond::Card__IndeCond(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__IndeCond::Card__IndeCond()
{

  
}



Card__BaseTargetedEff::Card__BaseTargetedEff(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__BaseTargetedEff::Card__BaseTargetedEff()
{

  
}



Card__BaseUntargetedEff::Card__BaseUntargetedEff(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__BaseUntargetedEff::Card__BaseUntargetedEff()
{

  
}



Card__Destination::Card__Destination(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__Destination::Card__Destination()
{

  
}



Card__NewCardVariant::Card__NewCardVariant(Card  * input__item) : Card__Node((input__item))
{

  
}

Card__NewCardVariant::Card__NewCardVariant()
{

  
}



Card__leaderCard::Card__leaderCard()
{

  
}

Card__leaderCard::Card__leaderCard(Card  * input__item, signed int  input__cost, signed int  input__attack, signed int  input__health, Card__AttackTimes  * input__attack_times, Card__Attributes  * input__attributes, Card__SpecialEffects  * input__effects) : Card__CardRoot((input__item)), cost((input__cost)), attack((input__attack)), health((input__health)), attack_times((input__attack_times)), attributes((input__attributes)), effects((input__effects))
{

  
  
  
  
  
  
  (((item)->card_type) = (LEADER_CARD));
  (((item)->orig_mana) = (((item)->mana) = (cost)));
  (((item)->orig_atk) = (((item)->atk) = (attack)));
  (((item)->orig_hp) = (((item)->max_hp) = (health)));
}

Card__leaderCard::~Card__leaderCard()
{

  {
    (((effects)->num_refs)--);
    if ((((effects)->num_refs) <= 0))
    {
      delete (effects);
    } else {
      
    }
  }
  {
    
  }
  {
    if ((attack_times))
    {
      delete (attack_times);
    } else {
      
    }
  }
  {
    if ((attributes))
    {
      delete (attributes);
    } else {
      
    }
  }
  {
    
  }
  {
    
  }
  {
    
  }
}







void Card__leaderCard::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0, (vector<double>{((NormalizeCode)((cost), 0, 10)), ((NormalizeCode)((attack), 0, 10)), ((NormalizeCode)((health), 10, 40))})))));
    (((attack_times)->FillRep)((rep)));
    (((attributes)->FillRep)((rep)));
    (((effects)->FillRep)((rep)));
  }
}

string Card__leaderCard::Brief()
{

  {
    return (((((((((((((((IntToStr)(((item)->mana))) + "/") + ((IntToStr)(((item)->atk)))) + "/") + ((IntToStr)((((item)->max_hp) - ((item)->hp_loss))))) + ", ") + ((IntToStr)((((item)->max_n_atks) - ((item)->n_atks_loss))))) + "Attacks") + (((((item)->IsSleeping)()) ? "(Sleeping)" : ""))) + ", Leader. ") + (((attributes)->Brief)())) + " ") + (((effects)->Brief)())) + (((item)->GetExtraEffectsBrief)()));
  }
}

string Card__leaderCard::Detail()
{

  {
    string mana_info = (((("MANA: " + ((IntToStr)(((item)->mana)))) + " (originally ") + ((IntToStr)(((item)->orig_mana)))) + ").");
    string atk_info = (((("ATK: " + ((IntToStr)(((item)->atk)))) + " (originally ") + ((IntToStr)(((item)->orig_atk)))) + ").");
    string hp_info = (((((("HP: " + ((IntToStr)((((item)->max_hp) - ((item)->hp_loss))))) + " of ") + ((IntToStr)(((item)->max_hp)))) + " (originally ") + ((IntToStr)(((item)->orig_hp)))) + ").");
    string atk_times_info = (((((("ATK TIMES: " + ((IntToStr)((((item)->max_n_atks) - ((item)->n_atks_loss))))) + " of ") + ((IntToStr)(((item)->max_n_atks)))) + " (originally ") + ((IntToStr)(((item)->orig_n_atks)))) + ")");
    string att_times_explain = "";
    if ((((item)->max_n_atks) == 0))
    {
      ((att_times_explain) = " (Cannot attack)");
    } else {
      if ((((item)->max_n_atks) > 1))
      {
        ((att_times_explain) = ((" (Can attack " + ((IntToStr)(((item)->max_n_atks)))) + " times per turn)"));
      } else {
        
      }
    }
    if ((((item)->IsSleeping)()))
    {
      ((att_times_explain) += "(Sleeping).");
    } else {
      ((att_times_explain) += ".");
    }
    string type_info = "TYPE: Leader (Does not counter-attack).";
    string attr_info = (((attributes)->Detail)());
    string effect_info = (((effects)->Detail)());
    string extra_effect_info = (((item)->GetExtraEffectsDetail)());
    return ((((((((((((((mana_info) + "\n") + (atk_info)) + "\n") + (hp_info)) + "\n") + (atk_times_info)) + (att_times_explain)) + "\n") + (type_info)) + "\n") + (attr_info)) + (effect_info)) + (extra_effect_info));
  }
}

string Card__leaderCard::DetailIndent(signed int  indent_size)
{

  {
    string mana_info = (((((((RepeatSpace)((indent_size))) + "MANA: ") + ((IntToStr)(((item)->mana)))) + " (originally ") + ((IntToStr)(((item)->orig_mana)))) + ").");
    string atk_info = (((((((RepeatSpace)((indent_size))) + "ATK: ") + ((IntToStr)(((item)->atk)))) + " (originally ") + ((IntToStr)(((item)->orig_atk)))) + ").");
    string hp_info = (((((((((RepeatSpace)((indent_size))) + "HP: ") + ((IntToStr)((((item)->max_hp) - ((item)->hp_loss))))) + " of ") + ((IntToStr)(((item)->max_hp)))) + " (originally ") + ((IntToStr)(((item)->orig_hp)))) + ").");
    string atk_times_info = (((((((((RepeatSpace)((indent_size))) + "ATK TIMES: ") + ((IntToStr)((((item)->max_n_atks) - ((item)->n_atks_loss))))) + " of ") + ((IntToStr)(((item)->max_n_atks)))) + " (originally ") + ((IntToStr)(((item)->orig_n_atks)))) + ")");
    string att_times_explain = "";
    if ((((item)->max_n_atks) == 0))
    {
      ((att_times_explain) = " (Cannot attack)");
    } else {
      if ((((item)->max_n_atks) > 1))
      {
        ((att_times_explain) = ((" (Can attack " + ((IntToStr)(((item)->max_n_atks)))) + " times per turn)"));
      } else {
        
      }
    }
    if ((((item)->IsSleeping)()))
    {
      ((att_times_explain) += "(Sleeping).");
    } else {
      ((att_times_explain) += ".");
    }
    string type_info = (((RepeatSpace)((indent_size))) + "TYPE: Leader (Does not counter-attack).");
    string attr_info = (((attributes)->DetailIndent)((indent_size)));
    string effect_info = (((effects)->DetailIndent)((indent_size)));
    string extra_effect_info = (((item)->GetExtraEffectsDetailIndent)((indent_size)));
    return ((((((((((((((mana_info) + "\n") + (atk_info)) + "\n") + (hp_info)) + "\n") + (atk_times_info)) + (att_times_explain)) + "\n") + (type_info)) + "\n") + (attr_info)) + (effect_info)) + (extra_effect_info));
  }
}

Card__Node  *Card__leaderCard::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__leaderCard((card_copy), (cost), (attack), (health), ((Card__AttackTimes *)((((attack_times)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__Attributes *)((((attributes)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__SpecialEffects *)((((effects)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

Card__SpecialEffects  *Card__leaderCard::GetEffects()
{

  return (effects);
}

signed int Card__leaderCard::isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card)
{

  {
    if ((!(((effects)->isTargetedAtPlay)((x), (y), (parent_card)))))
    {
      return 0;
    } else {
      
    }
    for (signed int i = 0; ((i) <= ((((((((parent_card)->owner)->field).size)()) + (((((parent_card)->opponent)->field).size)())) + (((((parent_card)->owner)->hand).size)())) + 1)); ((i)++))
    {
      if ((((x) != (i)) && (((effects)->CheckPlayValid)((x), (y), (i), (parent_card)))))
      {
        return 1;
      } else {
        
      }
    }
    return 0;
  }
}

signed int Card__leaderCard::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  {
    if (((isTargetedAtPlay)((x), (y), (parent_card))))
    {
      {
        if ((!(((effects)->CheckPlayValid)((x), (y), (z), (parent_card)))))
        {
          {
            return 0;
          }
        } else {
          
        }
      }
    } else {
      {
        ((z) = (-1));
      }
    }
    return 1;
  }
}

void Card__leaderCard::Play(signed int  x, signed int  y, signed int  z, Card  * parent_card)
{

  {
    delete (((parent_card)->owner)->leader);
    ((((parent_card)->owner)->leader) = (parent_card));
    (((parent_card)->is_first_turn_at_field) = 1);
    (((parent_card)->card_pos) = (CARD_POS_AT_LEADER));
    (((parent_card)->IncContribution)());
    if (((x) < (z)))
    {
      ((z)--);
    } else {
      
    }
    (((effects)->Play)((x), (y), (z), (parent_card)));
  }
}

void Card__leaderCard::Discard(Card  * parent_card)
{

  {
    (((effects)->Discard)((parent_card)));
  }
}

signed int Card__leaderCard::CheckAttackValid(signed int  x, signed int  z, Card  * parent_card)
{

  {
    if ((((parent_card)->n_atks_loss) >= ((parent_card)->max_n_atks)))
    {
      {
        return 0;
      }
    } else {
      
    }
    if ((((parent_card)->IsSleeping)()))
    {
      {
        return 0;
      }
    } else {
      
    }
    if ((((z) <= (((((parent_card)->owner)->field).size)())) || ((z) > (((((((parent_card)->owner)->field).size)()) + (((((parent_card)->opponent)->field).size)())) + 1))))
    {
      {
        return 0;
      }
    } else {
      
    }
    Card  *target = ((((z) == (((((((parent_card)->owner)->field).size)()) + (((((parent_card)->opponent)->field).size)())) + 1)) ? (((parent_card)->opponent)->leader) : ((((parent_card)->opponent)->field)[(((z) - (((((parent_card)->owner)->field).size)())) - 1)])));
    if (((target)->is_stealth))
    {
      {
        return 0;
      }
    } else {
      
    }
    if ((!((target)->is_taunt)))
    {
      {
        for (signed int i = 0; ((i) < (((((parent_card)->opponent)->field).size)())); ((i)++))
        {
          {
            if (((((((parent_card)->opponent)->field)[(i)])->is_taunt) && (!(((((parent_card)->opponent)->field)[(i)])->is_stealth))))
            {
              {
                return 0;
              }
            } else {
              
            }
          }
        }
        if ((((((parent_card)->opponent)->leader)->is_taunt) && (!((((parent_card)->opponent)->leader)->is_stealth))))
        {
          {
            return 0;
          }
        } else {
          
        }
      }
    } else {
      
    }
    return 1;
  }
}

void Card__leaderCard::Attack(signed int  x, signed int  z, Card  * parent_card)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((parent_card)->is_stealth))
    {
      {
        (((parent_card)->is_stealth) = 0);
      }
    } else {
      
    }
    if ((((parent_card)->is_dying) || ((target)->is_dying)))
    {
      return ;
    } else {
      
    }
    (((parent_card)->n_atks_loss)++);
    (((target)->TakeDamage)(((item)->atk), (parent_card), 1));
    if ((((target)->card_type) != (LEADER_CARD)))
    {
      (((item)->TakeDamage)(((target)->atk), (target), (!((target)->is_dying))));
    } else {
      
    }
  }
}

void Card__leaderCard::TurnStart(Card  * leader, Card  * parent_card)
{

  {
    (((effects)->TurnStart)((leader), (parent_card)));
  }
}

void Card__leaderCard::TurnEnd(Card  * leader, Card  * parent_card)
{

  {
    (((effects)->TurnEnd)((leader), (parent_card)));
  }
}

void Card__leaderCard::Mutate(signed int  min_eff_n, signed int  max_eff_n, signed int  effect_depth)
{

  {
    delete (attack_times);
    ((attack_times) = (((item)->Generate__AttackTimes)()));
    CondConfig self_config = ((GetInitConfigFromCard)((item)));
    (((effects)->num_refs)--);
    if ((((effects)->num_refs) <= 0))
    {
      delete (effects);
    } else {
      
    }
    ((effects) = (((item)->Generate__SpecialEffects)((self_config), (min_eff_n), (max_eff_n), (effect_depth), 0)));
  }
}

void Card__leaderCard::SetOverheatCounts(signed int  val)
{

  {
    (((effects)->SetOverheatCounts)((val)));
  }
}

void Card__leaderCard::SetOverheatThresholds(signed int  val)
{

  {
    (((effects)->SetOverheatThresholds)((val)));
  }
}

void Card__leaderCard::ModOverheatThresholds(signed int  amount)
{

  {
    (((effects)->ModOverheatThresholds)((amount)));
  }
}



Card__minionCard::Card__minionCard()
{

  
}

Card__minionCard::Card__minionCard(Card  * input__item, signed int  input__cost, signed int  input__attack, signed int  input__health, Card__AttackTimes  * input__attack_times, Card__MinionType  * input__type, Card__Attributes  * input__attributes, Card__SpecialEffects  * input__effects) : Card__CardRoot((input__item)), cost((input__cost)), attack((input__attack)), health((input__health)), attack_times((input__attack_times)), type((input__type)), attributes((input__attributes)), effects((input__effects))
{

  
  
  
  
  
  
  
  (((item)->card_type) = (MINION_CARD));
  (((item)->orig_mana) = (((item)->mana) = (cost)));
  (((item)->orig_atk) = (((item)->atk) = (attack)));
  (((item)->orig_hp) = (((item)->max_hp) = (health)));
}

Card__minionCard::~Card__minionCard()
{

  {
    (((effects)->num_refs)--);
    if ((((effects)->num_refs) <= 0))
    {
      delete (effects);
    } else {
      
    }
  }
  {
    
  }
  {
    if ((attack_times))
    {
      delete (attack_times);
    } else {
      
    }
  }
  {
    if ((attributes))
    {
      delete (attributes);
    } else {
      
    }
  }
  {
    
  }
  {
    
  }
  {
    
  }
  {
    if ((type))
    {
      delete (type);
    } else {
      
    }
  }
}








void Card__minionCard::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1, (vector<double>{((NormalizeCode)((cost), 0, 10)), ((NormalizeCode)((attack), 0, 10)), ((NormalizeCode)((health), 1, 10))})))));
    (((attack_times)->FillRep)((rep)));
    (((type)->FillRep)((rep)));
    (((attributes)->FillRep)((rep)));
    (((effects)->FillRep)((rep)));
  }
}

string Card__minionCard::Brief()
{

  {
    return (((((((((((((((((IntToStr)(((item)->mana))) + "/") + ((IntToStr)(((item)->atk)))) + "/") + ((IntToStr)((((item)->max_hp) - ((item)->hp_loss))))) + ", ") + ((IntToStr)((((item)->max_n_atks) - ((item)->n_atks_loss))))) + "Attacks") + (((((item)->IsSleeping)()) ? "(Sleeping)" : ""))) + ", ") + ((MinionTypeDescription)(((item)->minion_type)))) + ". ") + (((attributes)->Brief)())) + " ") + (((effects)->Brief)())) + (((item)->GetExtraEffectsBrief)()));
  }
}

string Card__minionCard::Detail()
{

  {
    string mana_info = (((("MANA: " + ((IntToStr)(((item)->mana)))) + " (originally ") + ((IntToStr)(((item)->orig_mana)))) + ").");
    string atk_info = (((("ATK: " + ((IntToStr)(((item)->atk)))) + " (originally ") + ((IntToStr)(((item)->orig_atk)))) + ").");
    string hp_info = (((((("HP: " + ((IntToStr)((((item)->max_hp) - ((item)->hp_loss))))) + " of ") + ((IntToStr)(((item)->max_hp)))) + " (originally ") + ((IntToStr)(((item)->orig_hp)))) + ").");
    string atk_times_info = (((((("ATK TIMES: " + ((IntToStr)((((item)->max_n_atks) - ((item)->n_atks_loss))))) + " of ") + ((IntToStr)(((item)->max_n_atks)))) + " (originally ") + ((IntToStr)(((item)->orig_n_atks)))) + ")");
    string att_times_explain = "";
    if ((((item)->max_n_atks) == 0))
    {
      ((att_times_explain) = " (Cannot attack)");
    } else {
      if ((((item)->max_n_atks) > 1))
      {
        ((att_times_explain) = ((" (Can attack " + ((IntToStr)(((item)->max_n_atks)))) + " times per turn)"));
      } else {
        
      }
    }
    if ((((item)->IsSleeping)()))
    {
      ((att_times_explain) += "(Sleeping).");
    } else {
      ((att_times_explain) += ".");
    }
    string type_info = (((("TYPE: " + ((MinionTypeDescription)(((item)->minion_type)))) + " (Originally ") + ((MinionTypeDescription)(((item)->orig_minion_type)))) + ") Minion.");
    string attr_info = (((attributes)->Detail)());
    string effect_info = (((effects)->Detail)());
    string extra_effect_info = (((item)->GetExtraEffectsDetail)());
    return ((((((((((((((mana_info) + "\n") + (atk_info)) + "\n") + (hp_info)) + "\n") + (atk_times_info)) + (att_times_explain)) + "\n") + (type_info)) + "\n") + (attr_info)) + (effect_info)) + (extra_effect_info));
  }
}

string Card__minionCard::DetailIndent(signed int  indent_size)
{

  {
    string mana_info = (((((((RepeatSpace)((indent_size))) + "MANA: ") + ((IntToStr)(((item)->mana)))) + " (originally ") + ((IntToStr)(((item)->orig_mana)))) + ").");
    string atk_info = (((((((RepeatSpace)((indent_size))) + "ATK: ") + ((IntToStr)(((item)->atk)))) + " (originally ") + ((IntToStr)(((item)->orig_atk)))) + ").");
    string hp_info = (((((((((RepeatSpace)((indent_size))) + "HP: ") + ((IntToStr)((((item)->max_hp) - ((item)->hp_loss))))) + " of ") + ((IntToStr)(((item)->max_hp)))) + " (originally ") + ((IntToStr)(((item)->orig_hp)))) + ").");
    string atk_times_info = (((((((((RepeatSpace)((indent_size))) + "ATK TIMES: ") + ((IntToStr)((((item)->max_n_atks) - ((item)->n_atks_loss))))) + " of ") + ((IntToStr)(((item)->max_n_atks)))) + " (originally ") + ((IntToStr)(((item)->orig_n_atks)))) + ")");
    string att_times_explain = "";
    if ((((item)->max_n_atks) == 0))
    {
      ((att_times_explain) = " (Cannot attack)");
    } else {
      if ((((item)->max_n_atks) > 1))
      {
        ((att_times_explain) = ((" (Can attack " + ((IntToStr)(((item)->max_n_atks)))) + " times per turn)"));
      } else {
        
      }
    }
    if ((((item)->IsSleeping)()))
    {
      ((att_times_explain) += "(Sleeping).");
    } else {
      ((att_times_explain) += ".");
    }
    string type_info = (((((((RepeatSpace)((indent_size))) + "TYPE: ") + ((MinionTypeDescription)(((item)->minion_type)))) + " (Originally ") + ((MinionTypeDescription)(((item)->orig_minion_type)))) + ") Minion.");
    string attr_info = (((attributes)->DetailIndent)((indent_size)));
    string effect_info = (((effects)->DetailIndent)((indent_size)));
    string extra_effect_info = (((item)->GetExtraEffectsDetailIndent)((indent_size)));
    return ((((((((((((((mana_info) + "\n") + (atk_info)) + "\n") + (hp_info)) + "\n") + (atk_times_info)) + (att_times_explain)) + "\n") + (type_info)) + "\n") + (attr_info)) + (effect_info)) + (extra_effect_info));
  }
}

Card__Node  *Card__minionCard::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__minionCard((card_copy), (cost), (attack), (health), ((Card__AttackTimes *)((((attack_times)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__MinionType *)((((type)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__Attributes *)((((attributes)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__SpecialEffects *)((((effects)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

Card__SpecialEffects  *Card__minionCard::GetEffects()
{

  return (effects);
}

signed int Card__minionCard::isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card)
{

  {
    if ((!(((effects)->isTargetedAtPlay)((x), (y), (parent_card)))))
    {
      return 0;
    } else {
      
    }
    for (signed int i = 0; ((i) <= ((((((((parent_card)->owner)->field).size)()) + (((((parent_card)->opponent)->field).size)())) + (((((parent_card)->owner)->hand).size)())) + 1)); ((i)++))
    {
      if ((((x) != (i)) && (((effects)->CheckPlayValid)((x), (y), (i), (parent_card)))))
      {
        return 1;
      } else {
        
      }
    }
    return 0;
  }
}

signed int Card__minionCard::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  {
    if ((((((parent_card)->owner)->GetActualFieldSize)()) >= (MAX_FIELD_SIZE)))
    {
      {
        return 0;
      }
    } else {
      
    }
    if ((((y) < 0) || ((y) > (((((parent_card)->owner)->field).size)()))))
    {
      {
        return 0;
      }
    } else {
      
    }
    if (((isTargetedAtPlay)((x), (y), (parent_card))))
    {
      {
        if ((!(((effects)->CheckPlayValid)((x), (y), (z), (parent_card)))))
        {
          {
            return 0;
          }
        } else {
          
        }
      }
    } else {
      {
        ((z) = (-1));
      }
    }
    return 1;
  }
}

void Card__minionCard::Play(signed int  x, signed int  y, signed int  z, Card  * parent_card)
{

  {
    (((((item)->owner)->field).insert)(((((((parent_card)->owner)->field).begin)()) + (y)), (parent_card)));
    (((parent_card)->is_first_turn_at_field) = 1);
    (((parent_card)->card_pos) = (CARD_POS_AT_FIELD));
    (((parent_card)->IncContribution)());
    if ((((y) < (z)) && ((z) < (x))))
    {
      ((z)++);
    } else {
      
    }
    (((effects)->Play)((x), (y), (z), (parent_card)));
  }
}

void Card__minionCard::Destroy(Card  * parent_card)
{

  {
    (((effects)->Destroy)((parent_card)));
  }
}

void Card__minionCard::Discard(Card  * parent_card)
{

  {
    (((effects)->Discard)((parent_card)));
  }
}

signed int Card__minionCard::CheckAttackValid(signed int  x, signed int  z, Card  * parent_card)
{

  {
    if ((((parent_card)->n_atks_loss) >= ((parent_card)->max_n_atks)))
    {
      {
        return 0;
      }
    } else {
      
    }
    if ((((parent_card)->IsSleeping)()))
    {
      {
        return 0;
      }
    } else {
      
    }
    if ((((z) <= (((((parent_card)->owner)->field).size)())) || ((z) > (((((((parent_card)->owner)->field).size)()) + (((((parent_card)->opponent)->field).size)())) + 1))))
    {
      {
        return 0;
      }
    } else {
      
    }
    Card  *target = ((((z) == (((((((parent_card)->owner)->field).size)()) + (((((parent_card)->opponent)->field).size)())) + 1)) ? (((parent_card)->opponent)->leader) : ((((parent_card)->opponent)->field)[(((z) - (((((parent_card)->owner)->field).size)())) - 1)])));
    if (((target)->is_stealth))
    {
      {
        return 0;
      }
    } else {
      
    }
    if ((!((target)->is_taunt)))
    {
      {
        for (signed int i = 0; ((i) < (((((parent_card)->opponent)->field).size)())); ((i)++))
        {
          {
            if (((((((parent_card)->opponent)->field)[(i)])->is_taunt) && (!(((((parent_card)->opponent)->field)[(i)])->is_stealth))))
            {
              {
                return 0;
              }
            } else {
              
            }
          }
        }
        if ((((((parent_card)->opponent)->leader)->is_taunt) && (!((((parent_card)->opponent)->leader)->is_stealth))))
        {
          {
            return 0;
          }
        } else {
          
        }
      }
    } else {
      
    }
    return 1;
  }
}

void Card__minionCard::Attack(signed int  x, signed int  z, Card  * parent_card)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((parent_card)->is_stealth))
    {
      {
        (((parent_card)->is_stealth) = 0);
      }
    } else {
      
    }
    if ((((parent_card)->is_dying) || ((target)->is_dying)))
    {
      return ;
    } else {
      
    }
    (((parent_card)->n_atks_loss)++);
    (((target)->TakeDamage)(((item)->atk), (parent_card), 1));
    if ((((target)->card_type) != (LEADER_CARD)))
    {
      (((item)->TakeDamage)(((target)->atk), (target), (!((target)->is_dying))));
    } else {
      
    }
  }
}

void Card__minionCard::TurnStart(Card  * leader, Card  * parent_card)
{

  {
    (((effects)->TurnStart)((leader), (parent_card)));
  }
}

void Card__minionCard::TurnEnd(Card  * leader, Card  * parent_card)
{

  {
    (((effects)->TurnEnd)((leader), (parent_card)));
  }
}

void Card__minionCard::Mutate(signed int  min_eff_n, signed int  max_eff_n, signed int  effect_depth)
{

  {
    delete (attack_times);
    ((attack_times) = (((item)->Generate__AttackTimes)()));
    CondConfig self_config = ((GetInitConfigFromCard)((item)));
    (((effects)->num_refs)--);
    if ((((effects)->num_refs) <= 0))
    {
      delete (effects);
    } else {
      
    }
    ((effects) = (((item)->Generate__SpecialEffects)((self_config), (min_eff_n), (max_eff_n), (effect_depth), 0)));
  }
}

void Card__minionCard::SetOverheatCounts(signed int  val)
{

  {
    (((effects)->SetOverheatCounts)((val)));
  }
}

void Card__minionCard::SetOverheatThresholds(signed int  val)
{

  {
    (((effects)->SetOverheatThresholds)((val)));
  }
}

void Card__minionCard::ModOverheatThresholds(signed int  amount)
{

  {
    (((effects)->ModOverheatThresholds)((amount)));
  }
}



Card__spellCard::Card__spellCard()
{

  
}

Card__spellCard::Card__spellCard(Card  * input__item, signed int  input__cost, Card__Attributes  * input__attributes, Card__SpecialEffects  * input__effects) : Card__CardRoot((input__item)), cost((input__cost)), attributes((input__attributes)), effects((input__effects))
{

  
  
  
  (((item)->card_type) = (SPELL_CARD));
  (((item)->orig_mana) = (((item)->mana) = (cost)));
}

Card__spellCard::~Card__spellCard()
{

  {
    (((effects)->num_refs)--);
    if ((((effects)->num_refs) <= 0))
    {
      delete (effects);
    } else {
      
    }
  }
  {
    if ((attributes))
    {
      delete (attributes);
    } else {
      
    }
  }
  {
    
  }
  {
    
  }
}




void Card__spellCard::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(2, (vector<double>{((NormalizeCode)((cost), 0, 10))})))));
    (((attributes)->FillRep)((rep)));
    (((effects)->FillRep)((rep)));
  }
}

string Card__spellCard::Brief()
{

  {
    return (((((((IntToStr)(((item)->mana))) + " Mana Spell. ") + (((attributes)->Brief)())) + " ") + (((effects)->Brief)())) + (((item)->GetExtraEffectsBrief)()));
  }
}

string Card__spellCard::Detail()
{

  {
    string mana_info = (((("MANA: " + ((IntToStr)(((item)->mana)))) + " (originally ") + ((IntToStr)(((item)->orig_mana)))) + ")");
    string type_info = "TYPE: Spell.";
    string attr_info = (((attributes)->Detail)());
    string effect_info = (((effects)->Detail)());
    string extra_effect_info = (((item)->GetExtraEffectsDetail)());
    return (((((((mana_info) + "\n") + (type_info)) + "\n") + (attr_info)) + (effect_info)) + (extra_effect_info));
  }
}

string Card__spellCard::DetailIndent(signed int  indent_size)
{

  {
    string mana_info = (((((((RepeatSpace)((indent_size))) + "MANA: ") + ((IntToStr)(((item)->mana)))) + " (originally ") + ((IntToStr)(((item)->orig_mana)))) + ")");
    string type_info = (((RepeatSpace)((indent_size))) + "TYPE: Spell.");
    string attr_info = (((attributes)->DetailIndent)((indent_size)));
    string effect_info = (((effects)->DetailIndent)((indent_size)));
    string extra_effect_info = (((item)->GetExtraEffectsDetailIndent)((indent_size)));
    return (((((((mana_info) + "\n") + (type_info)) + "\n") + (attr_info)) + (effect_info)) + (extra_effect_info));
  }
}

Card__Node  *Card__spellCard::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__spellCard((card_copy), (cost), ((Card__Attributes *)((((attributes)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__SpecialEffects *)((((effects)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

Card__SpecialEffects  *Card__spellCard::GetEffects()
{

  return (effects);
}

signed int Card__spellCard::isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card)
{

  {
    return (((effects)->isTargetedAtPlay)((x), (y), (parent_card)));
  }
}

signed int Card__spellCard::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  {
    if ((((isTargetedAtPlay)((x), (y), (parent_card))) && (!(((effects)->CheckPlayValid)((x), (y), (z), (parent_card))))))
    {
      {
        return 0;
      }
    } else {
      
    }
    return 1;
  }
}

void Card__spellCard::Play(signed int  x, signed int  y, signed int  z, Card  * parent_card)
{

  {
    ((((parent_card)->owner)->FlagCastSpell)((parent_card), 1));
    (((effects)->Play)((x), (y), (z), (parent_card)));
  }
}

void Card__spellCard::Discard(Card  * parent_card)
{

  {
    (((effects)->Discard)((parent_card)));
  }
}

void Card__spellCard::TurnStart(Card  * leader, Card  * parent_card)
{

  {
    (((effects)->TurnStart)((leader), (parent_card)));
  }
}

void Card__spellCard::TurnEnd(Card  * leader, Card  * parent_card)
{

  {
    (((effects)->TurnEnd)((leader), (parent_card)));
  }
}

void Card__spellCard::Mutate(signed int  min_eff_n, signed int  max_eff_n, signed int  effect_depth)
{

  {
    CondConfig self_config = ((GetInitConfigFromCard)((item)));
    (((effects)->num_refs)--);
    if ((((effects)->num_refs) <= 0))
    {
      delete (effects);
    } else {
      
    }
    ((effects) = (((item)->Generate__SpecialEffects)((self_config), ((((min_eff_n) > 1) ? (min_eff_n) : 1)), (max_eff_n), (effect_depth), 0)));
  }
}

void Card__spellCard::SetOverheatCounts(signed int  val)
{

  {
    (((effects)->SetOverheatCounts)((val)));
  }
}

void Card__spellCard::SetOverheatThresholds(signed int  val)
{

  {
    (((effects)->SetOverheatThresholds)((val)));
  }
}

void Card__spellCard::ModOverheatThresholds(signed int  amount)
{

  {
    (((effects)->ModOverheatThresholds)((amount)));
  }
}



Card__zeroAttack::Card__zeroAttack()
{

  
}

Card__zeroAttack::Card__zeroAttack(Card  * input__item) : Card__AttackTimes((input__item))
{

  (((item)->orig_n_atks) = (((item)->max_n_atks) = 0));
}

Card__zeroAttack::~Card__zeroAttack()
{

  
}

void Card__zeroAttack::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
  }
}

Card__Node  *Card__zeroAttack::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__zeroAttack((card_copy)));
}



Card__singleAttack::Card__singleAttack()
{

  
}

Card__singleAttack::Card__singleAttack(Card  * input__item) : Card__AttackTimes((input__item))
{

  (((item)->orig_n_atks) = (((item)->max_n_atks) = 1));
}

Card__singleAttack::~Card__singleAttack()
{

  
}

void Card__singleAttack::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
  }
}

Card__Node  *Card__singleAttack::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__singleAttack((card_copy)));
}



Card__multipleAttack::Card__multipleAttack()
{

  
}

Card__multipleAttack::Card__multipleAttack(Card  * input__item, signed int  input__n_atks) : Card__AttackTimes((input__item)), n_atks((input__n_atks))
{

  
  (((item)->n_atks_loss) = 0);
  (((item)->orig_n_atks) = (((item)->max_n_atks) = (n_atks)));
}

Card__multipleAttack::~Card__multipleAttack()
{

  
  {
    
  }
}


void Card__multipleAttack::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(2, (vector<double>{((NormalizeCode)((n_atks), 0, 5))})))));
  }
}

Card__Node  *Card__multipleAttack::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__multipleAttack((card_copy), (n_atks)));
}



Card__beastMinion::Card__beastMinion()
{

  
}

Card__beastMinion::Card__beastMinion(Card  * input__item) : Card__MinionType((input__item))
{

  (((item)->orig_minion_type) = (((item)->minion_type) = (BEAST_MINION)));
}

Card__beastMinion::~Card__beastMinion()
{

  
}

void Card__beastMinion::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
  }
}

Card__Node  *Card__beastMinion::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__beastMinion((card_copy)));
}



Card__dragonMinion::Card__dragonMinion()
{

  
}

Card__dragonMinion::Card__dragonMinion(Card  * input__item) : Card__MinionType((input__item))
{

  (((item)->orig_minion_type) = (((item)->minion_type) = (DRAGON_MINION)));
}

Card__dragonMinion::~Card__dragonMinion()
{

  
}

void Card__dragonMinion::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
  }
}

Card__Node  *Card__dragonMinion::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__dragonMinion((card_copy)));
}



Card__demonMinion::Card__demonMinion()
{

  
}

Card__demonMinion::Card__demonMinion(Card  * input__item) : Card__MinionType((input__item))
{

  (((item)->orig_minion_type) = (((item)->minion_type) = (DEMON_MINION)));
}

Card__demonMinion::~Card__demonMinion()
{

  
}

void Card__demonMinion::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(2))));
  }
}

Card__Node  *Card__demonMinion::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__demonMinion((card_copy)));
}



Card__justAttributes::Card__justAttributes()
{

  
}

Card__justAttributes::Card__justAttributes(Card  * input__item, Card__ChargeAttr  * input__c, Card__TauntAttr  * input__t, Card__StealthAttr  * input__s, Card__UntargetableAttr  * input__u, Card__ShieldAttr  * input__d, Card__PoisonousAttr  * input__p, Card__LifestealAttr  * input__l) : Card__Attributes((input__item)), c((input__c)), t((input__t)), s((input__s)), u((input__u)), d((input__d)), p((input__p)), l((input__l))
{

  
  
  
  
  
  
  
  
}

Card__justAttributes::~Card__justAttributes()
{

  
  {
    if ((c))
    {
      delete (c);
    } else {
      
    }
  }
  {
    if ((d))
    {
      delete (d);
    } else {
      
    }
  }
  {
    if ((l))
    {
      delete (l);
    } else {
      
    }
  }
  {
    if ((p))
    {
      delete (p);
    } else {
      
    }
  }
  {
    if ((s))
    {
      delete (s);
    } else {
      
    }
  }
  {
    if ((t))
    {
      delete (t);
    } else {
      
    }
  }
  {
    if ((u))
    {
      delete (u);
    } else {
      
    }
  }
}








void Card__justAttributes::FillRep(CardRep  & rep)
{

  {
    (((c)->FillRep)((rep)));
    (((t)->FillRep)((rep)));
    (((s)->FillRep)((rep)));
    (((u)->FillRep)((rep)));
    (((d)->FillRep)((rep)));
    (((p)->FillRep)((rep)));
    (((l)->FillRep)((rep)));
  }
}

string Card__justAttributes::Brief()
{

  return ((AttributeDescriptionBrief)(((item)->is_charge), ((item)->is_taunt), ((item)->is_stealth), ((item)->is_untargetable), ((item)->is_shielded), ((item)->is_poisonous), ((item)->is_lifesteal)));
}

string Card__justAttributes::Detail()
{

  {
    string cur_info = ((AttributeDescriptionDetail)(((item)->is_charge), ((item)->is_taunt), ((item)->is_stealth), ((item)->is_untargetable), ((item)->is_shielded), ((item)->is_poisonous), ((item)->is_lifesteal), 0));
    string orig_info = ((AttributeDescriptionBrief)(((item)->orig_is_charge), ((item)->orig_is_taunt), ((item)->orig_is_stealth), ((item)->orig_is_untargetable), ((item)->orig_is_shielded), ((item)->orig_is_poisonous), ((item)->orig_is_lifesteal)));
    if ((((((cur_info).length)()) == 0) && ((((orig_info).length)()) == 0)))
    {
      return "";
    } else {
      
    }
    return (((((((((cur_info).length)()) == 0) ? "No Attribute " : (cur_info))) + "(Originally ") + ((((((orig_info).length)()) == 0) ? "No Attribute" : (orig_info)))) + ")\n");
  }
}

string Card__justAttributes::DetailIndent(signed int  indent_size)
{

  {
    string cur_info = ((AttributeDescriptionDetail)(((item)->is_charge), ((item)->is_taunt), ((item)->is_stealth), ((item)->is_untargetable), ((item)->is_shielded), ((item)->is_poisonous), ((item)->is_lifesteal), (indent_size)));
    string orig_info = ((AttributeDescriptionBrief)(((item)->orig_is_charge), ((item)->orig_is_taunt), ((item)->orig_is_stealth), ((item)->orig_is_untargetable), ((item)->orig_is_shielded), ((item)->orig_is_poisonous), ((item)->orig_is_lifesteal)));
    if ((((((cur_info).length)()) == 0) && ((((orig_info).length)()) == 0)))
    {
      return "";
    } else {
      
    }
    return ((((((RepeatSpace)((indent_size))) + ((((((cur_info).length)()) == 0) ? "No Attribute " : (cur_info)))) + "(Originally ") + ((((((orig_info).length)()) == 0) ? "No Attribute" : (orig_info)))) + ")\n");
  }
}

Card__Node  *Card__justAttributes::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__justAttributes((card_copy), ((Card__ChargeAttr *)((((c)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__TauntAttr *)((((t)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__StealthAttr *)((((s)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__UntargetableAttr *)((((u)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__ShieldAttr *)((((d)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__PoisonousAttr *)((((p)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__LifestealAttr *)((((l)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

unsigned int Card__justAttributes::GetInitAttrFlag()
{

  return (((((((((c)->GetInitAttrFlag)()) | (((t)->GetInitAttrFlag)())) | (((s)->GetInitAttrFlag)())) | (((u)->GetInitAttrFlag)())) | (((d)->GetInitAttrFlag)())) | (((p)->GetInitAttrFlag)())) | (((l)->GetInitAttrFlag)()));
}



Card__damageAttributes::Card__damageAttributes()
{

  
}

Card__damageAttributes::Card__damageAttributes(Card  * input__item, Card__PoisonousAttr  * input__p, Card__LifestealAttr  * input__l) : Card__DamageAttributes((input__item)), p((input__p)), l((input__l))
{

  
  
  
}

Card__damageAttributes::~Card__damageAttributes()
{

  
  {
    if ((l))
    {
      delete (l);
    } else {
      
    }
  }
  {
    if ((p))
    {
      delete (p);
    } else {
      
    }
  }
}



void Card__damageAttributes::FillRep(CardRep  & rep)
{

  {
    (((p)->FillRep)((rep)));
    (((l)->FillRep)((rep)));
  }
}

Card__Node  *Card__damageAttributes::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__damageAttributes((card_copy), ((Card__PoisonousAttr *)((((p)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__LifestealAttr *)((((l)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

unsigned int Card__damageAttributes::GetInitAttrFlag()
{

  return ((((p)->GetInitAttrFlag)()) | (((l)->GetInitAttrFlag)()));
}



Card__noCharge::Card__noCharge()
{

  
}

Card__noCharge::Card__noCharge(Card  * input__item) : Card__ChargeAttr((input__item))
{

  
}

Card__noCharge::~Card__noCharge()
{

  
}

void Card__noCharge::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
  }
}

Card__Node  *Card__noCharge::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__noCharge((card_copy)));
}



Card__justCharge::Card__justCharge()
{

  
}

Card__justCharge::Card__justCharge(Card  * input__item) : Card__ChargeAttr((input__item))
{

  (((item)->orig_is_charge) = (((item)->is_charge) = 1));
}

Card__justCharge::~Card__justCharge()
{

  
}

void Card__justCharge::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
  }
}

Card__Node  *Card__justCharge::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__justCharge((card_copy)));
}

unsigned int Card__justCharge::GetInitAttrFlag()
{

  return (TARGET_IS_CHARGE);
}



Card__noTaunt::Card__noTaunt()
{

  
}

Card__noTaunt::Card__noTaunt(Card  * input__item) : Card__TauntAttr((input__item))
{

  
}

Card__noTaunt::~Card__noTaunt()
{

  
}

void Card__noTaunt::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
  }
}

Card__Node  *Card__noTaunt::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__noTaunt((card_copy)));
}



Card__justTaunt::Card__justTaunt()
{

  
}

Card__justTaunt::Card__justTaunt(Card  * input__item) : Card__TauntAttr((input__item))
{

  (((item)->orig_is_taunt) = (((item)->is_taunt) = 1));
}

Card__justTaunt::~Card__justTaunt()
{

  
}

void Card__justTaunt::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
  }
}

Card__Node  *Card__justTaunt::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__justTaunt((card_copy)));
}

unsigned int Card__justTaunt::GetInitAttrFlag()
{

  return (TARGET_IS_TAUNT);
}



Card__noStealth::Card__noStealth()
{

  
}

Card__noStealth::Card__noStealth(Card  * input__item) : Card__StealthAttr((input__item))
{

  
}

Card__noStealth::~Card__noStealth()
{

  
}

void Card__noStealth::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
  }
}

Card__Node  *Card__noStealth::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__noStealth((card_copy)));
}



Card__justStealth::Card__justStealth()
{

  
}

Card__justStealth::Card__justStealth(Card  * input__item) : Card__StealthAttr((input__item))
{

  (((item)->orig_is_stealth) = (((item)->is_stealth) = 1));
}

Card__justStealth::~Card__justStealth()
{

  
}

void Card__justStealth::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
  }
}

Card__Node  *Card__justStealth::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__justStealth((card_copy)));
}

unsigned int Card__justStealth::GetInitAttrFlag()
{

  return (TARGET_IS_STEALTH);
}



Card__noUntargetable::Card__noUntargetable()
{

  
}

Card__noUntargetable::Card__noUntargetable(Card  * input__item) : Card__UntargetableAttr((input__item))
{

  
}

Card__noUntargetable::~Card__noUntargetable()
{

  
}

void Card__noUntargetable::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
  }
}

Card__Node  *Card__noUntargetable::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__noUntargetable((card_copy)));
}



Card__justUntargetable::Card__justUntargetable()
{

  
}

Card__justUntargetable::Card__justUntargetable(Card  * input__item) : Card__UntargetableAttr((input__item))
{

  (((item)->orig_is_untargetable) = (((item)->is_untargetable) = 1));
}

Card__justUntargetable::~Card__justUntargetable()
{

  
}

void Card__justUntargetable::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
  }
}

Card__Node  *Card__justUntargetable::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__justUntargetable((card_copy)));
}

unsigned int Card__justUntargetable::GetInitAttrFlag()
{

  return (TARGET_IS_UNTARGETABLE);
}



Card__noShield::Card__noShield()
{

  
}

Card__noShield::Card__noShield(Card  * input__item) : Card__ShieldAttr((input__item))
{

  
}

Card__noShield::~Card__noShield()
{

  
}

void Card__noShield::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
  }
}

Card__Node  *Card__noShield::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__noShield((card_copy)));
}



Card__justShield::Card__justShield()
{

  
}

Card__justShield::Card__justShield(Card  * input__item) : Card__ShieldAttr((input__item))
{

  (((item)->orig_is_shielded) = (((item)->is_shielded) = 1));
}

Card__justShield::~Card__justShield()
{

  
}

void Card__justShield::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
  }
}

Card__Node  *Card__justShield::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__justShield((card_copy)));
}

unsigned int Card__justShield::GetInitAttrFlag()
{

  return (TARGET_IS_SHIELDED);
}



Card__noPoisonous::Card__noPoisonous()
{

  
}

Card__noPoisonous::Card__noPoisonous(Card  * input__item) : Card__PoisonousAttr((input__item))
{

  
}

Card__noPoisonous::~Card__noPoisonous()
{

  
}

void Card__noPoisonous::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
  }
}

Card__Node  *Card__noPoisonous::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__noPoisonous((card_copy)));
}



Card__justPoisonous::Card__justPoisonous()
{

  
}

Card__justPoisonous::Card__justPoisonous(Card  * input__item) : Card__PoisonousAttr((input__item))
{

  (((item)->orig_is_poisonous) = (((item)->is_poisonous) = 1));
}

Card__justPoisonous::~Card__justPoisonous()
{

  
}

void Card__justPoisonous::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
  }
}

Card__Node  *Card__justPoisonous::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__justPoisonous((card_copy)));
}

unsigned int Card__justPoisonous::GetInitAttrFlag()
{

  return (TARGET_IS_POISONOUS);
}



Card__noLifesteal::Card__noLifesteal()
{

  
}

Card__noLifesteal::Card__noLifesteal(Card  * input__item) : Card__LifestealAttr((input__item))
{

  
}

Card__noLifesteal::~Card__noLifesteal()
{

  
}

void Card__noLifesteal::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
  }
}

Card__Node  *Card__noLifesteal::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__noLifesteal((card_copy)));
}



Card__justLifesteal::Card__justLifesteal()
{

  
}

Card__justLifesteal::Card__justLifesteal(Card  * input__item) : Card__LifestealAttr((input__item))
{

  (((item)->orig_is_lifesteal) = (((item)->is_lifesteal) = 1));
}

Card__justLifesteal::~Card__justLifesteal()
{

  
}

void Card__justLifesteal::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
  }
}

Card__Node  *Card__justLifesteal::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__justLifesteal((card_copy)));
}

unsigned int Card__justLifesteal::GetInitAttrFlag()
{

  return (TARGET_IS_LIFESTEAL);
}



Card__specialEffects::Card__specialEffects()
{

  
}

Card__specialEffects::Card__specialEffects(Card  * input__item, Card__TargetedPlayEff  * input__effect, Card__OtherEffs  * input__effects) : Card__SpecialEffects((input__item)), effect((input__effect)), effects((input__effects))
{

  
  
  {
    ((num_refs) = 1);
  }
}

Card__specialEffects::~Card__specialEffects()
{

  
  {
    if ((effect))
    {
      delete (effect);
    } else {
      
    }
  }
  {
    if ((effects))
    {
      delete (effects);
    } else {
      
    }
  }
}



void Card__specialEffects::FillRep(CardRep  & rep)
{

  {
    (((effect)->FillRep)((rep)));
    (((effects)->FillRep)((rep)));
  }
}

string Card__specialEffects::Brief()
{

  return ((((effect)->Brief)()) + (((effects)->Brief)()));
}

string Card__specialEffects::Detail()
{

  return ((((effect)->Detail)()) + (((effects)->Detail)()));
}

string Card__specialEffects::DetailIndent(signed int  indent_size)
{

  return ((((effect)->DetailIndent)((indent_size))) + (((effects)->DetailIndent)((indent_size))));
}

Card__Node  *Card__specialEffects::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  {
    Card__SpecialEffects  *effects_copy = 0;
    if (((num_refs) <= 1))
    {
      {
        ((effects_copy) = (new Card__specialEffects((card_copy), ((Card__TargetedPlayEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__OtherEffs *)((((effects)->CreateNodeHardCopy)((card_copy), (redir_map))))))));
        (((effects_copy)->num_refs) = (num_refs));
      }
    } else {
      {
        void  *tmp_key = ((void *)(this));
        PtrRedirMapIter it = (((redir_map).find)((tmp_key)));
        if (((it) == (((redir_map).end)())))
        {
          {
            ((effects_copy) = (new Card__specialEffects((card_copy), ((Card__TargetedPlayEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__OtherEffs *)((((effects)->CreateNodeHardCopy)((card_copy), (redir_map))))))));
            (((redir_map)[(tmp_key)]) = (effects_copy));
          }
        } else {
          {
            ((effects_copy) = ((Card__SpecialEffects *)(((it)->second))));
            (((effects_copy)->num_refs)++);
          }
        }
      }
    }
    return (effects_copy);
  }
}

CondConfig Card__specialEffects::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig tmp_config = (((effect)->GetGlobalSelfConfig)((self_config), (EFFECT_TIMING_DEFAULT)));
    ((tmp_config) &= (((effects)->GetGlobalSelfConfig)((self_config), (EFFECT_TIMING_DEFAULT))));
    return (tmp_config);
  }
}

signed int Card__specialEffects::isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card)
{

  return (((effect)->isTargetedAtPlay)((x), (y), (parent_card)));
}

signed int Card__specialEffects::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return (((effect)->CheckPlayValid)((x), (y), (z), (parent_card)));
}

void Card__specialEffects::Play(signed int  x, signed int  y, signed int  z, Card  * parent_card)
{

  {
    (((effect)->Play)((x), (y), (z), (parent_card)));
    if (((((parent_card)->owner)->ProcessDeferredEvents)()))
    {
      return ;
    } else {
      
    }
    (((effects)->Play)((x), (y), (z), (parent_card)));
  }
}

void Card__specialEffects::Destroy(Card  * parent_card)
{

  {
    (((effects)->Destroy)((parent_card)));
  }
}

void Card__specialEffects::Discard(Card  * parent_card)
{

  {
    (((effects)->Discard)((parent_card)));
  }
}

void Card__specialEffects::TurnStart(Card  * leader, Card  * parent_card)
{

  {
    (((effects)->TurnStart)((leader), (parent_card)));
  }
}

void Card__specialEffects::TurnEnd(Card  * leader, Card  * parent_card)
{

  {
    (((effects)->TurnEnd)((leader), (parent_card)));
  }
}

void Card__specialEffects::SetOverheatCounts(signed int  val)
{

  {
    (((effect)->SetOverheatCounts)((val)));
    (((effects)->SetOverheatCounts)((val)));
  }
}

void Card__specialEffects::SetOverheatThresholds(signed int  val)
{

  {
    (((effect)->SetOverheatThresholds)((val)));
    (((effects)->SetOverheatThresholds)((val)));
  }
}

void Card__specialEffects::ModOverheatThresholds(signed int  amount)
{

  {
    (((effect)->ModOverheatThresholds)((amount)));
    (((effects)->ModOverheatThresholds)((amount)));
  }
}



Card__noTargetedPlayEff::Card__noTargetedPlayEff()
{

  
}

Card__noTargetedPlayEff::Card__noTargetedPlayEff(Card  * input__item) : Card__TargetedPlayEff((input__item))
{

  
}

Card__noTargetedPlayEff::~Card__noTargetedPlayEff()
{

  
}

void Card__noTargetedPlayEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
  }
}

string Card__noTargetedPlayEff::DetailIndent(signed int  indent_size)
{

  return "";
}

Card__Node  *Card__noTargetedPlayEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__noTargetedPlayEff((card_copy)));
}



Card__targetedBattlecryEff::Card__targetedBattlecryEff()
{

  
}

Card__targetedBattlecryEff::Card__targetedBattlecryEff(Card  * input__item, Card__TargetedEff  * input__effect) : Card__TargetedPlayEff((input__item)), effect((input__effect))
{

  
  
}

Card__targetedBattlecryEff::~Card__targetedBattlecryEff()
{

  
  {
    if ((effect))
    {
      delete (effect);
    } else {
      
    }
  }
}


void Card__targetedBattlecryEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
    (((effect)->FillRep)((rep)));
  }
}

string Card__targetedBattlecryEff::Brief()
{

  return (("<Battlecry" + (((effect)->Brief)())) + ">");
}

string Card__targetedBattlecryEff::Detail()
{

  return ((("Battlecry: " + (((effect)->Detail)())) + ".") + (((effect)->Postfix)()));
}

string Card__targetedBattlecryEff::DetailIndent(signed int  indent_size)
{

  return ((((((RepeatSpace)((indent_size))) + "Battlecry: ") + (((effect)->Detail)())) + ".") + (((effect)->PostfixIndent)((indent_size))));
}

Card__Node  *Card__targetedBattlecryEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__targetedBattlecryEff((card_copy), ((Card__TargetedEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

signed int Card__targetedBattlecryEff::GetEffectNum()
{

  return 1;
}

unsigned int Card__targetedBattlecryEff::GetInitAttrFlag()
{

  return (((effect)->GetInitAttrFlag)());
}

CondConfig Card__targetedBattlecryEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig tmp_config = (((effect)->GetGlobalSelfConfig)((self_config), (EFFECT_TIMING_PLAY)));
    ((tmp_config) &= (CHAR_COND_FILTER));
    if (((!(((self_config) & (TARGET_POS_HAND_OR_DECK)))) || (!(((tmp_config) & (TARGET_POS_HAND_OR_DECK))))))
    {
      ((tmp_config) &= (NOT_LEADER_COND_FILTER));
    } else {
      if (((!(((self_config) & (TARGET_NOT_LEADER)))) || (!(((tmp_config) & (TARGET_NOT_LEADER))))))
      {
        ((tmp_config) &= (HAND_OR_DECK_COND_FILTER));
      } else {
        ((tmp_config) &= ((((RandomRoll)(0.9)) ? (NOT_LEADER_COND_FILTER) : (HAND_OR_DECK_COND_FILTER))));
      }
    }
    return (tmp_config);
  }
}

signed int Card__targetedBattlecryEff::isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card)
{

  return (((effect)->isTargetedAtPlay)((x), (y), (parent_card)));
}

signed int Card__targetedBattlecryEff::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return (((effect)->CheckPlayValid)((x), (y), (z), (parent_card)));
}

void Card__targetedBattlecryEff::Play(signed int  x, signed int  y, signed int  z, Card  * parent_card)
{

  {
    if (((z) >= 0))
    {
      {
        (((effect)->TargetedAction)((z), (parent_card), 1));
      }
    } else {
      
    }
  }
}

void Card__targetedBattlecryEff::SetOverheatCounts(signed int  val)
{

  {
    (((effect)->SetOverheatCounts)((val)));
  }
}

void Card__targetedBattlecryEff::SetOverheatThresholds(signed int  val)
{

  {
    (((effect)->SetOverheatThresholds)((val)));
  }
}

void Card__targetedBattlecryEff::ModOverheatThresholds(signed int  amount)
{

  {
    (((effect)->ModOverheatThresholds)((amount)));
  }
}



Card__targetedCastEff::Card__targetedCastEff()
{

  
}

Card__targetedCastEff::Card__targetedCastEff(Card  * input__item, Card__TargetedEff  * input__effect) : Card__TargetedPlayEff((input__item)), effect((input__effect))
{

  
  
}

Card__targetedCastEff::~Card__targetedCastEff()
{

  
  {
    if ((effect))
    {
      delete (effect);
    } else {
      
    }
  }
}


void Card__targetedCastEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(2))));
    (((effect)->FillRep)((rep)));
  }
}

string Card__targetedCastEff::Brief()
{

  return (("<Cast" + (((effect)->Brief)())) + ">");
}

string Card__targetedCastEff::Detail()
{

  return ((("Cast: " + (((effect)->Detail)())) + ".") + (((effect)->Postfix)()));
}

string Card__targetedCastEff::DetailIndent(signed int  indent_size)
{

  return ((((((RepeatSpace)((indent_size))) + "Cast: ") + (((effect)->Detail)())) + ".") + (((effect)->PostfixIndent)((indent_size))));
}

Card__Node  *Card__targetedCastEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__targetedCastEff((card_copy), ((Card__TargetedEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

signed int Card__targetedCastEff::GetEffectNum()
{

  return 1;
}

unsigned int Card__targetedCastEff::GetInitAttrFlag()
{

  return (((effect)->GetInitAttrFlag)());
}

CondConfig Card__targetedCastEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig tmp_config = (((effect)->GetGlobalSelfConfig)((self_config), (EFFECT_TIMING_PLAY)));
    ((tmp_config) &= (SPELL_COND_FILTER));
    return (tmp_config);
  }
}

signed int Card__targetedCastEff::isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card)
{

  return (((effect)->isTargetedAtPlay)((x), (y), (parent_card)));
}

signed int Card__targetedCastEff::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return (((effect)->CheckPlayValid)((x), (y), (z), (parent_card)));
}

void Card__targetedCastEff::Play(signed int  x, signed int  y, signed int  z, Card  * parent_card)
{

  {
    if (((z) >= 0))
    {
      {
        (((effect)->TargetedAction)((z), (parent_card), 1));
      }
    } else {
      
    }
  }
}

void Card__targetedCastEff::SetOverheatCounts(signed int  val)
{

  {
    (((effect)->SetOverheatCounts)((val)));
  }
}

void Card__targetedCastEff::SetOverheatThresholds(signed int  val)
{

  {
    (((effect)->SetOverheatThresholds)((val)));
  }
}

void Card__targetedCastEff::ModOverheatThresholds(signed int  amount)
{

  {
    (((effect)->ModOverheatThresholds)((amount)));
  }
}



Card__noOtherEffs::Card__noOtherEffs()
{

  
}

Card__noOtherEffs::Card__noOtherEffs(Card  * input__item) : Card__OtherEffs((input__item))
{

  
}

Card__noOtherEffs::~Card__noOtherEffs()
{

  
}

void Card__noOtherEffs::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
  }
}

string Card__noOtherEffs::DetailIndent(signed int  indent_size)
{

  return "";
}

Card__Node  *Card__noOtherEffs::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__noOtherEffs((card_copy)));
}



Card__consOtherEffs::Card__consOtherEffs()
{

  
}

Card__consOtherEffs::Card__consOtherEffs(Card  * input__item, Card__OtherEff  * input__effect, Card__OtherEffs  * input__effects) : Card__OtherEffs((input__item)), effect((input__effect)), effects((input__effects))
{

  
  
  
}

Card__consOtherEffs::~Card__consOtherEffs()
{

  
  {
    if ((effect))
    {
      delete (effect);
    } else {
      
    }
  }
  {
    if ((effects))
    {
      delete (effects);
    } else {
      
    }
  }
}



void Card__consOtherEffs::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
    (((effect)->FillRep)((rep)));
    (((effects)->FillRep)((rep)));
  }
}

string Card__consOtherEffs::Brief()
{

  return ((((effect)->Brief)()) + (((effects)->Brief)()));
}

string Card__consOtherEffs::Detail()
{

  return ((((effect)->Detail)()) + (((effects)->Detail)()));
}

string Card__consOtherEffs::DetailIndent(signed int  indent_size)
{

  return ((((effect)->DetailIndent)((indent_size))) + (((effects)->DetailIndent)((indent_size))));
}

Card__Node  *Card__consOtherEffs::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__consOtherEffs((card_copy), ((Card__OtherEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__OtherEffs *)((((effects)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

signed int Card__consOtherEffs::GetEffectNum()
{

  return (1 + (((effects)->GetEffectNum)()));
}

CondConfig Card__consOtherEffs::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig tmp_config = (((effect)->GetGlobalSelfConfig)((self_config), (EFFECT_TIMING_DEFAULT)));
    ((tmp_config) &= (((effects)->GetGlobalSelfConfig)((self_config), (EFFECT_TIMING_DEFAULT))));
    return (tmp_config);
  }
}

void Card__consOtherEffs::Play(signed int  x, signed int  y, signed int  z, Card  * parent_card)
{

  {
    (((effect)->Play)((x), (y), (z), (parent_card)));
    if (((((parent_card)->owner)->ProcessDeferredEvents)()))
    {
      return ;
    } else {
      
    }
    (((effects)->Play)((x), (y), (z), (parent_card)));
  }
}

void Card__consOtherEffs::Destroy(Card  * parent_card)
{

  {
    (((effect)->Destroy)((parent_card)));
    if (((((parent_card)->owner)->ProcessDeferredEvents)()))
    {
      return ;
    } else {
      
    }
    (((effects)->Destroy)((parent_card)));
  }
}

void Card__consOtherEffs::Discard(Card  * parent_card)
{

  {
    (((effect)->Discard)((parent_card)));
    if (((((parent_card)->owner)->ProcessDeferredEvents)()))
    {
      return ;
    } else {
      
    }
    (((effects)->Discard)((parent_card)));
  }
}

void Card__consOtherEffs::TurnStart(Card  * leader, Card  * parent_card)
{

  {
    (((effect)->TurnStart)((leader), (parent_card)));
    if (((((parent_card)->owner)->ProcessDeferredEvents)()))
    {
      return ;
    } else {
      
    }
    (((effects)->TurnStart)((leader), (parent_card)));
  }
}

void Card__consOtherEffs::TurnEnd(Card  * leader, Card  * parent_card)
{

  {
    (((effect)->TurnEnd)((leader), (parent_card)));
    if (((((parent_card)->owner)->ProcessDeferredEvents)()))
    {
      return ;
    } else {
      
    }
    (((effects)->TurnEnd)((leader), (parent_card)));
  }
}

void Card__consOtherEffs::SetOverheatCounts(signed int  val)
{

  {
    (((effect)->SetOverheatCounts)((val)));
    (((effects)->SetOverheatCounts)((val)));
  }
}

void Card__consOtherEffs::SetOverheatThresholds(signed int  val)
{

  {
    (((effect)->SetOverheatThresholds)((val)));
    (((effects)->SetOverheatThresholds)((val)));
  }
}

void Card__consOtherEffs::ModOverheatThresholds(signed int  amount)
{

  {
    (((effect)->ModOverheatThresholds)((amount)));
    (((effects)->ModOverheatThresholds)((amount)));
  }
}



Card__untargetedBattlecryEff::Card__untargetedBattlecryEff()
{

  
}

Card__untargetedBattlecryEff::Card__untargetedBattlecryEff(Card  * input__item, Card__UntargetedEff  * input__effect) : Card__OtherEff((input__item)), effect((input__effect))
{

  
  
}

Card__untargetedBattlecryEff::~Card__untargetedBattlecryEff()
{

  
  {
    if ((effect))
    {
      delete (effect);
    } else {
      
    }
  }
}


void Card__untargetedBattlecryEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
    (((effect)->FillRep)((rep)));
  }
}

string Card__untargetedBattlecryEff::Brief()
{

  return (("<Battlecry" + (((effect)->Brief)())) + ">");
}

string Card__untargetedBattlecryEff::Detail()
{

  return ((("Battlecry: " + (((effect)->Detail)())) + ".") + (((effect)->Postfix)()));
}

string Card__untargetedBattlecryEff::DetailIndent(signed int  indent_size)
{

  return ((((((RepeatSpace)((indent_size))) + "Battlecry: ") + (((effect)->Detail)())) + ".") + (((effect)->PostfixIndent)((indent_size))));
}

Card__Node  *Card__untargetedBattlecryEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__untargetedBattlecryEff((card_copy), ((Card__UntargetedEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

unsigned int Card__untargetedBattlecryEff::GetInitAttrFlag()
{

  return (((effect)->GetInitAttrFlag)());
}

CondConfig Card__untargetedBattlecryEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig tmp_config = (((effect)->GetGlobalSelfConfig)((self_config), (EFFECT_TIMING_PLAY)));
    ((tmp_config) &= (CHAR_COND_FILTER));
    if (((!(((self_config) & (TARGET_POS_HAND_OR_DECK)))) || (!(((tmp_config) & (TARGET_POS_HAND_OR_DECK))))))
    {
      ((tmp_config) &= (NOT_LEADER_COND_FILTER));
    } else {
      if (((!(((self_config) & (TARGET_NOT_LEADER)))) || (!(((tmp_config) & (TARGET_NOT_LEADER))))))
      {
        ((tmp_config) &= (HAND_OR_DECK_COND_FILTER));
      } else {
        ((tmp_config) &= ((((RandomRoll)(0.9)) ? (NOT_LEADER_COND_FILTER) : (HAND_OR_DECK_COND_FILTER))));
      }
    }
    return (tmp_config);
  }
}

void Card__untargetedBattlecryEff::Play(signed int  x, signed int  y, signed int  z, Card  * parent_card)
{

  {
    (((effect)->UntargetedAction)((parent_card)));
  }
}

void Card__untargetedBattlecryEff::SetOverheatCounts(signed int  val)
{

  {
    (((effect)->SetOverheatCounts)((val)));
  }
}

void Card__untargetedBattlecryEff::SetOverheatThresholds(signed int  val)
{

  {
    (((effect)->SetOverheatThresholds)((val)));
  }
}

void Card__untargetedBattlecryEff::ModOverheatThresholds(signed int  amount)
{

  {
    (((effect)->ModOverheatThresholds)((amount)));
  }
}



Card__untargetedCastEff::Card__untargetedCastEff()
{

  
}

Card__untargetedCastEff::Card__untargetedCastEff(Card  * input__item, Card__UntargetedEff  * input__effect) : Card__OtherEff((input__item)), effect((input__effect))
{

  
  
}

Card__untargetedCastEff::~Card__untargetedCastEff()
{

  
  {
    if ((effect))
    {
      delete (effect);
    } else {
      
    }
  }
}


void Card__untargetedCastEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
    (((effect)->FillRep)((rep)));
  }
}

string Card__untargetedCastEff::Brief()
{

  return (("<Cast" + (((effect)->Brief)())) + ">");
}

string Card__untargetedCastEff::Detail()
{

  return ((("Cast: " + (((effect)->Detail)())) + ".") + (((effect)->Postfix)()));
}

string Card__untargetedCastEff::DetailIndent(signed int  indent_size)
{

  return ((((((RepeatSpace)((indent_size))) + "Cast: ") + (((effect)->Detail)())) + ".") + (((effect)->PostfixIndent)((indent_size))));
}

Card__Node  *Card__untargetedCastEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__untargetedCastEff((card_copy), ((Card__UntargetedEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

unsigned int Card__untargetedCastEff::GetInitAttrFlag()
{

  return (((effect)->GetInitAttrFlag)());
}

CondConfig Card__untargetedCastEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig tmp_config = (((effect)->GetGlobalSelfConfig)((self_config), (EFFECT_TIMING_PLAY)));
    ((tmp_config) &= (SPELL_COND_FILTER));
    return (tmp_config);
  }
}

void Card__untargetedCastEff::Play(signed int  x, signed int  y, signed int  z, Card  * parent_card)
{

  {
    (((effect)->UntargetedAction)((parent_card)));
  }
}

void Card__untargetedCastEff::SetOverheatCounts(signed int  val)
{

  {
    (((effect)->SetOverheatCounts)((val)));
  }
}

void Card__untargetedCastEff::SetOverheatThresholds(signed int  val)
{

  {
    (((effect)->SetOverheatThresholds)((val)));
  }
}

void Card__untargetedCastEff::ModOverheatThresholds(signed int  amount)
{

  {
    (((effect)->ModOverheatThresholds)((amount)));
  }
}



Card__deathrattleEff::Card__deathrattleEff()
{

  
}

Card__deathrattleEff::Card__deathrattleEff(Card  * input__item, Card__UntargetedEff  * input__effect) : Card__OtherEff((input__item)), effect((input__effect))
{

  
  
}

Card__deathrattleEff::~Card__deathrattleEff()
{

  
  {
    if ((effect))
    {
      delete (effect);
    } else {
      
    }
  }
}


void Card__deathrattleEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(2))));
    (((effect)->FillRep)((rep)));
  }
}

string Card__deathrattleEff::Brief()
{

  return (("<Deathrattle" + (((effect)->Brief)())) + ">");
}

string Card__deathrattleEff::Detail()
{

  return ((("Deathrattle: " + (((effect)->Detail)())) + ".") + (((effect)->Postfix)()));
}

string Card__deathrattleEff::DetailIndent(signed int  indent_size)
{

  return ((((((RepeatSpace)((indent_size))) + "Deathrattle: ") + (((effect)->Detail)())) + ".") + (((effect)->PostfixIndent)((indent_size))));
}

Card__Node  *Card__deathrattleEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__deathrattleEff((card_copy), ((Card__UntargetedEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

unsigned int Card__deathrattleEff::GetInitAttrFlag()
{

  return (((effect)->GetInitAttrFlag)());
}

CondConfig Card__deathrattleEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig tmp_config = (((effect)->GetGlobalSelfConfig)((self_config), (EFFECT_TIMING_DESTROY)));
    ((tmp_config) &= (MINION_COND_FILTER));
    return (tmp_config);
  }
}

void Card__deathrattleEff::Destroy(Card  * parent_card)
{

  {
    (((effect)->UntargetedAction)((parent_card)));
  }
}

void Card__deathrattleEff::SetOverheatCounts(signed int  val)
{

  {
    (((effect)->SetOverheatCounts)((val)));
  }
}

void Card__deathrattleEff::SetOverheatThresholds(signed int  val)
{

  {
    (((effect)->SetOverheatThresholds)((val)));
  }
}

void Card__deathrattleEff::ModOverheatThresholds(signed int  amount)
{

  {
    (((effect)->ModOverheatThresholds)((amount)));
  }
}



Card__onDiscardEff::Card__onDiscardEff()
{

  
}

Card__onDiscardEff::Card__onDiscardEff(Card  * input__item, Card__UntargetedEff  * input__effect) : Card__OtherEff((input__item)), effect((input__effect))
{

  
  
}

Card__onDiscardEff::~Card__onDiscardEff()
{

  
  {
    if ((effect))
    {
      delete (effect);
    } else {
      
    }
  }
}


void Card__onDiscardEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(3))));
    (((effect)->FillRep)((rep)));
  }
}

string Card__onDiscardEff::Brief()
{

  return (("<Discard" + (((effect)->Brief)())) + ">");
}

string Card__onDiscardEff::Detail()
{

  return ((("Discard: " + (((effect)->Detail)())) + ".") + (((effect)->Postfix)()));
}

string Card__onDiscardEff::DetailIndent(signed int  indent_size)
{

  return ((((((RepeatSpace)((indent_size))) + "Discard: ") + (((effect)->Detail)())) + ".") + (((effect)->PostfixIndent)((indent_size))));
}

Card__Node  *Card__onDiscardEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__onDiscardEff((card_copy), ((Card__UntargetedEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

unsigned int Card__onDiscardEff::GetInitAttrFlag()
{

  return (((effect)->GetInitAttrFlag)());
}

CondConfig Card__onDiscardEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig tmp_config = (((effect)->GetGlobalSelfConfig)((self_config), (EFFECT_TIMING_DISCARD)));
    if (((!(((self_config) & (TARGET_POS_HAND_OR_DECK)))) || (!(((tmp_config) & (TARGET_POS_HAND_OR_DECK))))))
    {
      ((tmp_config) &= (NOT_LEADER_COND_FILTER));
    } else {
      if (((!(((self_config) & (TARGET_NOT_LEADER)))) || (!(((tmp_config) & (TARGET_NOT_LEADER))))))
      {
        ((tmp_config) &= (HAND_OR_DECK_COND_FILTER));
      } else {
        ((tmp_config) &= ((((RandomRoll)(0.9)) ? (NOT_LEADER_COND_FILTER) : (HAND_OR_DECK_COND_FILTER))));
      }
    }
    return (tmp_config);
  }
}

void Card__onDiscardEff::Discard(Card  * parent_card)
{

  {
    (((effect)->UntargetedAction)((parent_card)));
  }
}

void Card__onDiscardEff::SetOverheatCounts(signed int  val)
{

  {
    (((effect)->SetOverheatCounts)((val)));
  }
}

void Card__onDiscardEff::SetOverheatThresholds(signed int  val)
{

  {
    (((effect)->SetOverheatThresholds)((val)));
  }
}

void Card__onDiscardEff::ModOverheatThresholds(signed int  amount)
{

  {
    (((effect)->ModOverheatThresholds)((amount)));
  }
}



Card__turnStartEff::Card__turnStartEff()
{

  
}

Card__turnStartEff::Card__turnStartEff(Card  * input__item, Card__UntargetedEff  * input__effect, Card__AllegianceCond  * input__alle) : Card__OtherEff((input__item)), effect((input__effect)), alle((input__alle))
{

  
  
  
}

Card__turnStartEff::~Card__turnStartEff()
{

  
  {
    if ((alle))
    {
      delete (alle);
    } else {
      
    }
  }
  {
    if ((effect))
    {
      delete (effect);
    } else {
      
    }
  }
}



void Card__turnStartEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(4))));
    (((effect)->FillRep)((rep)));
    (((alle)->FillRep)((rep)));
  }
}

string Card__turnStartEff::Brief()
{

  return (("<Trigger" + (((effect)->Brief)())) + ">");
}

string Card__turnStartEff::Detail()
{

  return ((((("At the start of " + (((alle)->DetailAlt2)())) + "turns: ") + (((effect)->Detail)())) + ".") + (((effect)->Postfix)()));
}

string Card__turnStartEff::DetailIndent(signed int  indent_size)
{

  return ((((((((RepeatSpace)((indent_size))) + "At the start of ") + (((alle)->DetailAlt2)())) + "turns: ") + (((effect)->Detail)())) + ".") + (((effect)->PostfixIndent)((indent_size))));
}

Card__Node  *Card__turnStartEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__turnStartEff((card_copy), ((Card__UntargetedEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__AllegianceCond *)((((alle)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

unsigned int Card__turnStartEff::GetInitAttrFlag()
{

  return (((effect)->GetInitAttrFlag)());
}

CondConfig Card__turnStartEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  return (((effect)->GetGlobalSelfConfig)((self_config), (EFFECT_TIMING_TURN)));
}

void Card__turnStartEff::TurnStart(Card  * leader, Card  * parent_card)
{

  {
    if ((((alle)->CheckCardValid)((leader), (parent_card))))
    {
      {
        (((effect)->UntargetedAction)((parent_card)));
      }
    } else {
      
    }
  }
}

void Card__turnStartEff::SetOverheatCounts(signed int  val)
{

  {
    (((effect)->SetOverheatCounts)((val)));
  }
}

void Card__turnStartEff::SetOverheatThresholds(signed int  val)
{

  {
    (((effect)->SetOverheatThresholds)((val)));
  }
}

void Card__turnStartEff::ModOverheatThresholds(signed int  amount)
{

  {
    (((effect)->ModOverheatThresholds)((amount)));
  }
}



Card__turnEndEff::Card__turnEndEff()
{

  
}

Card__turnEndEff::Card__turnEndEff(Card  * input__item, Card__UntargetedEff  * input__effect, Card__AllegianceCond  * input__alle) : Card__OtherEff((input__item)), effect((input__effect)), alle((input__alle))
{

  
  
  
}

Card__turnEndEff::~Card__turnEndEff()
{

  
  {
    if ((alle))
    {
      delete (alle);
    } else {
      
    }
  }
  {
    if ((effect))
    {
      delete (effect);
    } else {
      
    }
  }
}



void Card__turnEndEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(5))));
    (((effect)->FillRep)((rep)));
    (((alle)->FillRep)((rep)));
  }
}

string Card__turnEndEff::Brief()
{

  return (("<Trigger" + (((effect)->Brief)())) + ">");
}

string Card__turnEndEff::Detail()
{

  return ((((("At the end of " + (((alle)->DetailAlt2)())) + "turns: ") + (((effect)->Detail)())) + ".") + (((effect)->Postfix)()));
}

string Card__turnEndEff::DetailIndent(signed int  indent_size)
{

  return ((((((((RepeatSpace)((indent_size))) + "At the end of ") + (((alle)->DetailAlt2)())) + "turns: ") + (((effect)->Detail)())) + ".") + (((effect)->PostfixIndent)((indent_size))));
}

Card__Node  *Card__turnEndEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__turnEndEff((card_copy), ((Card__UntargetedEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__AllegianceCond *)((((alle)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

unsigned int Card__turnEndEff::GetInitAttrFlag()
{

  return (((effect)->GetInitAttrFlag)());
}

CondConfig Card__turnEndEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  return (((effect)->GetGlobalSelfConfig)((self_config), (EFFECT_TIMING_TURN)));
}

void Card__turnEndEff::TurnEnd(Card  * leader, Card  * parent_card)
{

  {
    if ((((alle)->CheckCardValid)((leader), (parent_card))))
    {
      {
        (((effect)->UntargetedAction)((parent_card)));
      }
    } else {
      
    }
  }
}

void Card__turnEndEff::SetOverheatCounts(signed int  val)
{

  {
    (((effect)->SetOverheatCounts)((val)));
  }
}

void Card__turnEndEff::SetOverheatThresholds(signed int  val)
{

  {
    (((effect)->SetOverheatThresholds)((val)));
  }
}

void Card__turnEndEff::ModOverheatThresholds(signed int  amount)
{

  {
    (((effect)->ModOverheatThresholds)((amount)));
  }
}



Card__noCondTargetedEff::Card__noCondTargetedEff()
{

  
}

Card__noCondTargetedEff::Card__noCondTargetedEff(Card  * input__item, Card__BaseTargetedEff  * input__effect, Card__TargetCond  * input__desconstr) : Card__TargetedEff((input__item)), effect((input__effect)), desconstr((input__desconstr))
{

  
  
  ((overheat_count) = 0);
  ((overheat_threshold) = 10);
}

Card__noCondTargetedEff::~Card__noCondTargetedEff()
{

  
  {
    if ((desconstr))
    {
      delete (desconstr);
    } else {
      
    }
  }
  {
    if ((effect))
    {
      delete (effect);
    } else {
      
    }
  }
}



void Card__noCondTargetedEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
    (((effect)->FillRep)((rep)));
    (((desconstr)->FillRep)((rep)));
  }
}

string Card__noCondTargetedEff::Brief()
{

  return ((((overheat_count) < (overheat_threshold)) ? "" : "(Overheated)"));
}

string Card__noCondTargetedEff::Detail()
{

  {
    string overheat_indicator = ((((overheat_count) < (overheat_threshold)) ? "" : "(Overheated)"));
    string overheat_counter = (((display_overheat_counts) ? (((("(OH: " + ((IntToStr)((overheat_count)))) + " of ") + ((IntToStr)((overheat_threshold)))) + ")") : ""));
    return (((((overheat_indicator) + (overheat_counter)) + (((effect)->Detail)())) + (((desconstr)->DetailAlt1)())) + (((effect)->Suffix)()));
  }
}

string Card__noCondTargetedEff::DetailIndent(signed int  indent_size)
{

  {
    string overheat_indicator = ((((overheat_count) < (overheat_threshold)) ? "" : "(Overheated)"));
    string overheat_counter = (((display_overheat_counts) ? (((("(OH: " + ((IntToStr)((overheat_count)))) + " of ") + ((IntToStr)((overheat_threshold)))) + ")") : ""));
    return (((((overheat_indicator) + (overheat_counter)) + (((effect)->Detail)())) + (((desconstr)->DetailAlt1)())) + (((effect)->Suffix)()));
  }
}

string Card__noCondTargetedEff::Postfix()
{

  return (((effect)->Postfix)());
}

string Card__noCondTargetedEff::PostfixIndent(signed int  indent_size)
{

  return (((effect)->PostfixIndent)((indent_size)));
}

Card__Node  *Card__noCondTargetedEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  {
    Card__noCondTargetedEff  *effect_copy = (new Card__noCondTargetedEff((card_copy), ((Card__BaseTargetedEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__TargetCond *)((((desconstr)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
    (((effect_copy)->overheat_count) = (overheat_count));
    (((effect_copy)->overheat_threshold) = (overheat_threshold));
    return (effect_copy);
  }
}

signed int Card__noCondTargetedEff::isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card)
{

  return ((overheat_count) < (overheat_threshold));
}

signed int Card__noCondTargetedEff::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  {
    if (((((parent_card)->owner)->IsValidTarget)((z))))
    {
      {
        Card  *card = ((((parent_card)->owner)->GetTargetCard)((z)));
        if ((((card)->is_untargetable) || ((((card)->is_stealth) && ((((parent_card)->owner)->IsValidCharTarget)((z)))))))
        {
          return 0;
        } else {
          
        }
      }
    } else {
      return 0;
    }
    return (((desconstr)->CheckPlayValid)((x), (y), (z), (parent_card)));
  }
}

signed int Card__noCondTargetedEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    if (((overheat_count) < (overheat_threshold)))
    {
      {
        ((overheat_count)++);
        (((parent_card)->IncContribution)());
        return (((effect)->TargetedAction)((z), (parent_card), 1));
      }
    } else {
      
    }
    return 0;
  }
}



Card__indeCondTargetedEff::Card__indeCondTargetedEff()
{

  
}

Card__indeCondTargetedEff::Card__indeCondTargetedEff(Card  * input__item, Card__BaseTargetedEff  * input__effect, Card__TargetCond  * input__desconstr, Card__IndeCond  * input__cond) : Card__TargetedEff((input__item)), effect((input__effect)), desconstr((input__desconstr)), cond((input__cond))
{

  
  
  
  ((overheat_count) = 0);
  ((overheat_threshold) = 10);
}

Card__indeCondTargetedEff::~Card__indeCondTargetedEff()
{

  
  {
    if ((cond))
    {
      delete (cond);
    } else {
      
    }
  }
  {
    if ((desconstr))
    {
      delete (desconstr);
    } else {
      
    }
  }
  {
    if ((effect))
    {
      delete (effect);
    } else {
      
    }
  }
}




void Card__indeCondTargetedEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
    (((effect)->FillRep)((rep)));
    (((desconstr)->FillRep)((rep)));
    (((cond)->FillRep)((rep)));
  }
}

string Card__indeCondTargetedEff::Brief()
{

  return ((((overheat_count) < (overheat_threshold)) ? "" : "(Overheated)"));
}

string Card__indeCondTargetedEff::Detail()
{

  {
    string overheat_indicator = ((((overheat_count) < (overheat_threshold)) ? "" : "(Overheated)"));
    string overheat_counter = (((display_overheat_counts) ? (((("(OH: " + ((IntToStr)((overheat_count)))) + " of ") + ((IntToStr)((overheat_threshold)))) + ")") : ""));
    return (((((((overheat_indicator) + (overheat_counter)) + (((effect)->Detail)())) + (((desconstr)->DetailAlt1)())) + (((effect)->Suffix)())) + ", if ") + (((cond)->Detail)()));
  }
}

string Card__indeCondTargetedEff::DetailIndent(signed int  indent_size)
{

  {
    string overheat_indicator = ((((overheat_count) < (overheat_threshold)) ? "" : "(Overheated)"));
    string overheat_counter = (((display_overheat_counts) ? (((("(OH: " + ((IntToStr)((overheat_count)))) + " of ") + ((IntToStr)((overheat_threshold)))) + ")") : ""));
    return (((((((overheat_indicator) + (overheat_counter)) + (((effect)->Detail)())) + (((desconstr)->DetailAlt1)())) + (((effect)->Suffix)())) + ", if ") + (((cond)->Detail)()));
  }
}

string Card__indeCondTargetedEff::Postfix()
{

  return (((effect)->Postfix)());
}

string Card__indeCondTargetedEff::PostfixIndent(signed int  indent_size)
{

  return (((effect)->PostfixIndent)((indent_size)));
}

Card__Node  *Card__indeCondTargetedEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  {
    Card__indeCondTargetedEff  *effect_copy = (new Card__indeCondTargetedEff((card_copy), ((Card__BaseTargetedEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__TargetCond *)((((desconstr)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__IndeCond *)((((cond)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
    (((effect_copy)->overheat_count) = (overheat_count));
    (((effect_copy)->overheat_threshold) = (overheat_threshold));
    return (effect_copy);
  }
}

signed int Card__indeCondTargetedEff::isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card)
{

  return (((overheat_count) < (overheat_threshold)) && (((cond)->CheckThisValid)((parent_card))));
}

signed int Card__indeCondTargetedEff::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  {
    if (((((parent_card)->owner)->IsValidTarget)((z))))
    {
      {
        Card  *card = ((((parent_card)->owner)->GetTargetCard)((z)));
        if ((((card)->is_untargetable) || ((((card)->is_stealth) && ((((parent_card)->owner)->IsValidCharTarget)((z)))))))
        {
          return 0;
        } else {
          
        }
      }
    } else {
      return 0;
    }
    return (((desconstr)->CheckPlayValid)((x), (y), (z), (parent_card)));
  }
}

signed int Card__indeCondTargetedEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    if (((((cond)->CheckThisValid)((parent_card))) && ((overheat_count) < (overheat_threshold))))
    {
      {
        ((overheat_count)++);
        (((parent_card)->IncContribution)());
        return (((effect)->TargetedAction)((z), (parent_card), 1));
      }
    } else {
      
    }
    return 0;
  }
}



Card__srcCondTargetedEff::Card__srcCondTargetedEff()
{

  
}

Card__srcCondTargetedEff::Card__srcCondTargetedEff(Card  * input__item, Card__BaseTargetedEff  * input__effect, Card__TargetCond  * input__desconstr, Card__TargetCond  * input__srccond) : Card__TargetedEff((input__item)), effect((input__effect)), desconstr((input__desconstr)), srccond((input__srccond))
{

  
  
  
  ((overheat_count) = 0);
  ((overheat_threshold) = 10);
}

Card__srcCondTargetedEff::~Card__srcCondTargetedEff()
{

  
  {
    if ((desconstr))
    {
      delete (desconstr);
    } else {
      
    }
  }
  {
    if ((effect))
    {
      delete (effect);
    } else {
      
    }
  }
  {
    if ((srccond))
    {
      delete (srccond);
    } else {
      
    }
  }
}




void Card__srcCondTargetedEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(2))));
    (((effect)->FillRep)((rep)));
    (((desconstr)->FillRep)((rep)));
    (((srccond)->FillRep)((rep)));
  }
}

string Card__srcCondTargetedEff::Brief()
{

  return ((((overheat_count) < (overheat_threshold)) ? "" : "(Overheated)"));
}

string Card__srcCondTargetedEff::Detail()
{

  {
    string overheat_indicator = ((((overheat_count) < (overheat_threshold)) ? "" : "(Overheated)"));
    string overheat_counter = (((display_overheat_counts) ? (((("(OH: " + ((IntToStr)((overheat_count)))) + " of ") + ((IntToStr)((overheat_threshold)))) + ")") : ""));
    return (((((((overheat_indicator) + (overheat_counter)) + (((effect)->Detail)())) + (((desconstr)->DetailAlt1)())) + (((effect)->Suffix)())) + ", if itself is ") + (((srccond)->DetailAlt3)()));
  }
}

string Card__srcCondTargetedEff::DetailIndent(signed int  indent_size)
{

  {
    string overheat_indicator = ((((overheat_count) < (overheat_threshold)) ? "" : "(Overheated)"));
    string overheat_counter = (((display_overheat_counts) ? (((("(OH: " + ((IntToStr)((overheat_count)))) + " of ") + ((IntToStr)((overheat_threshold)))) + ")") : ""));
    return (((((((overheat_indicator) + (overheat_counter)) + (((effect)->Detail)())) + (((desconstr)->DetailAlt1)())) + (((effect)->Suffix)())) + ", if itself is ") + (((srccond)->DetailAlt3)()));
  }
}

string Card__srcCondTargetedEff::Postfix()
{

  return (((effect)->Postfix)());
}

string Card__srcCondTargetedEff::PostfixIndent(signed int  indent_size)
{

  return (((effect)->PostfixIndent)((indent_size)));
}

Card__Node  *Card__srcCondTargetedEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  {
    Card__srcCondTargetedEff  *effect_copy = (new Card__srcCondTargetedEff((card_copy), ((Card__BaseTargetedEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__TargetCond *)((((desconstr)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__TargetCond *)((((srccond)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
    (((effect_copy)->overheat_count) = (overheat_count));
    (((effect_copy)->overheat_threshold) = (overheat_threshold));
    return (effect_copy);
  }
}

unsigned int Card__srcCondTargetedEff::GetInitAttrFlag()
{

  return (((srccond)->GetInitAttrFlag)());
}

CondConfig Card__srcCondTargetedEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig self_config_copy = (self_config);
    return (((srccond)->GetGlobalSelfConfig)((self_config_copy), (effect_timing)));
  }
}

signed int Card__srcCondTargetedEff::isTargetedAtPlay(signed int  x, signed int  y, Card  * parent_card)
{

  return (((overheat_count) < (overheat_threshold)) && (((srccond)->CheckThisValid)((parent_card))));
}

signed int Card__srcCondTargetedEff::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  {
    if (((((parent_card)->owner)->IsValidTarget)((z))))
    {
      {
        Card  *card = ((((parent_card)->owner)->GetTargetCard)((z)));
        if ((((card)->is_untargetable) || ((((card)->is_stealth) && ((((parent_card)->owner)->IsValidCharTarget)((z)))))))
        {
          return 0;
        } else {
          
        }
      }
    } else {
      return 0;
    }
    return (((desconstr)->CheckPlayValid)((x), (y), (z), (parent_card)));
  }
}

signed int Card__srcCondTargetedEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    if (((((srccond)->CheckThisValid)((parent_card))) && ((overheat_count) < (overheat_threshold))))
    {
      {
        ((overheat_count)++);
        (((parent_card)->IncContribution)());
        return (((effect)->TargetedAction)((z), (parent_card), 1));
      }
    } else {
      
    }
    return 0;
  }
}



Card__noCondUntargetedEff::Card__noCondUntargetedEff()
{

  
}

Card__noCondUntargetedEff::Card__noCondUntargetedEff(Card  * input__item, Card__BaseUntargetedEff  * input__effect) : Card__UntargetedEff((input__item)), effect((input__effect))
{

  
  ((overheat_count) = 0);
  ((overheat_threshold) = 10);
}

Card__noCondUntargetedEff::~Card__noCondUntargetedEff()
{

  
  {
    if ((effect))
    {
      delete (effect);
    } else {
      
    }
  }
}


void Card__noCondUntargetedEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
    (((effect)->FillRep)((rep)));
  }
}

string Card__noCondUntargetedEff::Brief()
{

  return ((((overheat_count) < (overheat_threshold)) ? "" : "(Overheated)"));
}

string Card__noCondUntargetedEff::Detail()
{

  {
    string overheat_indicator = ((((overheat_count) < (overheat_threshold)) ? "" : "(Overheated)"));
    string overheat_counter = (((display_overheat_counts) ? (((("(OH: " + ((IntToStr)((overheat_count)))) + " of ") + ((IntToStr)((overheat_threshold)))) + ")") : ""));
    return (((overheat_indicator) + (overheat_counter)) + (((effect)->Detail)()));
  }
}

string Card__noCondUntargetedEff::DetailIndent(signed int  indent_size)
{

  {
    string overheat_indicator = ((((overheat_count) < (overheat_threshold)) ? "" : "(Overheated)"));
    string overheat_counter = (((display_overheat_counts) ? (((("(OH: " + ((IntToStr)((overheat_count)))) + " of ") + ((IntToStr)((overheat_threshold)))) + ")") : ""));
    return (((overheat_indicator) + (overheat_counter)) + (((effect)->Detail)()));
  }
}

string Card__noCondUntargetedEff::Postfix()
{

  return (((effect)->Postfix)());
}

string Card__noCondUntargetedEff::PostfixIndent(signed int  indent_size)
{

  return (((effect)->PostfixIndent)((indent_size)));
}

Card__Node  *Card__noCondUntargetedEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  {
    Card__noCondUntargetedEff  *effect_copy = (new Card__noCondUntargetedEff((card_copy), ((Card__BaseUntargetedEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
    (((effect_copy)->overheat_count) = (overheat_count));
    (((effect_copy)->overheat_threshold) = (overheat_threshold));
    return (effect_copy);
  }
}

unsigned int Card__noCondUntargetedEff::GetInitAttrFlag()
{

  return (((effect)->GetInitAttrFlag)());
}

CondConfig Card__noCondUntargetedEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  return (((effect)->GetGlobalSelfConfig)((self_config), (effect_timing)));
}

void Card__noCondUntargetedEff::UntargetedAction(Card  * parent_card)
{

  {
    if (((overheat_count) < (overheat_threshold)))
    {
      {
        ((overheat_count)++);
        (((parent_card)->IncContribution)());
        (((effect)->UntargetedAction)((parent_card)));
      }
    } else {
      
    }
  }
}



Card__indeCondUntargetedEff::Card__indeCondUntargetedEff()
{

  
}

Card__indeCondUntargetedEff::Card__indeCondUntargetedEff(Card  * input__item, Card__BaseUntargetedEff  * input__effect, Card__IndeCond  * input__cond) : Card__UntargetedEff((input__item)), effect((input__effect)), cond((input__cond))
{

  
  
  ((overheat_count) = 0);
  ((overheat_threshold) = 10);
}

Card__indeCondUntargetedEff::~Card__indeCondUntargetedEff()
{

  
  {
    if ((cond))
    {
      delete (cond);
    } else {
      
    }
  }
  {
    if ((effect))
    {
      delete (effect);
    } else {
      
    }
  }
}



void Card__indeCondUntargetedEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
    (((effect)->FillRep)((rep)));
    (((cond)->FillRep)((rep)));
  }
}

string Card__indeCondUntargetedEff::Brief()
{

  return ((((overheat_count) < (overheat_threshold)) ? "" : "(Overheated)"));
}

string Card__indeCondUntargetedEff::Detail()
{

  {
    string overheat_indicator = ((((overheat_count) < (overheat_threshold)) ? "" : "(Overheated)"));
    string overheat_counter = (((display_overheat_counts) ? (((("(OH: " + ((IntToStr)((overheat_count)))) + " of ") + ((IntToStr)((overheat_threshold)))) + ")") : ""));
    return (((((overheat_indicator) + (overheat_counter)) + (((effect)->Detail)())) + ", if ") + (((cond)->Detail)()));
  }
}

string Card__indeCondUntargetedEff::DetailIndent(signed int  indent_size)
{

  {
    string overheat_indicator = ((((overheat_count) < (overheat_threshold)) ? "" : "(Overheated)"));
    string overheat_counter = (((display_overheat_counts) ? (((("(OH: " + ((IntToStr)((overheat_count)))) + " of ") + ((IntToStr)((overheat_threshold)))) + ")") : ""));
    return (((((overheat_indicator) + (overheat_counter)) + (((effect)->Detail)())) + ", if ") + (((cond)->Detail)()));
  }
}

string Card__indeCondUntargetedEff::Postfix()
{

  return (((effect)->Postfix)());
}

string Card__indeCondUntargetedEff::PostfixIndent(signed int  indent_size)
{

  return (((effect)->PostfixIndent)((indent_size)));
}

Card__Node  *Card__indeCondUntargetedEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  {
    Card__indeCondUntargetedEff  *effect_copy = (new Card__indeCondUntargetedEff((card_copy), ((Card__BaseUntargetedEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__IndeCond *)((((cond)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
    (((effect_copy)->overheat_count) = (overheat_count));
    (((effect_copy)->overheat_threshold) = (overheat_threshold));
    return (effect_copy);
  }
}

unsigned int Card__indeCondUntargetedEff::GetInitAttrFlag()
{

  return (((effect)->GetInitAttrFlag)());
}

void Card__indeCondUntargetedEff::UntargetedAction(Card  * parent_card)
{

  {
    if (((((cond)->CheckThisValid)((parent_card))) && ((overheat_count) < (overheat_threshold))))
    {
      {
        ((overheat_count)++);
        (((parent_card)->IncContribution)());
        (((effect)->UntargetedAction)((parent_card)));
      }
    } else {
      
    }
  }
}



Card__srcCondUntargetedEff::Card__srcCondUntargetedEff()
{

  
}

Card__srcCondUntargetedEff::Card__srcCondUntargetedEff(Card  * input__item, Card__BaseUntargetedEff  * input__effect, Card__TargetCond  * input__srccond) : Card__UntargetedEff((input__item)), effect((input__effect)), srccond((input__srccond))
{

  
  
  ((overheat_count) = 0);
  ((overheat_threshold) = 10);
}

Card__srcCondUntargetedEff::~Card__srcCondUntargetedEff()
{

  
  {
    if ((effect))
    {
      delete (effect);
    } else {
      
    }
  }
  {
    if ((srccond))
    {
      delete (srccond);
    } else {
      
    }
  }
}



void Card__srcCondUntargetedEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(2))));
    (((effect)->FillRep)((rep)));
    (((srccond)->FillRep)((rep)));
  }
}

string Card__srcCondUntargetedEff::Brief()
{

  return ((((overheat_count) < (overheat_threshold)) ? "" : "(Overheated)"));
}

string Card__srcCondUntargetedEff::Detail()
{

  {
    string overheat_indicator = ((((overheat_count) < (overheat_threshold)) ? "" : "(Overheated)"));
    string overheat_counter = (((display_overheat_counts) ? (((("(OH:" + ((IntToStr)((overheat_count)))) + " of ") + ((IntToStr)((overheat_threshold)))) + ")") : ""));
    return (((((overheat_indicator) + (overheat_counter)) + (((effect)->Detail)())) + ", if itself is ") + (((srccond)->DetailAlt3)()));
  }
}

string Card__srcCondUntargetedEff::DetailIndent(signed int  indent_size)
{

  {
    string overheat_indicator = ((((overheat_count) < (overheat_threshold)) ? "" : "(Overheated)"));
    string overheat_counter = (((display_overheat_counts) ? (((("(OH:" + ((IntToStr)((overheat_count)))) + " of ") + ((IntToStr)((overheat_threshold)))) + ")") : ""));
    return (((((overheat_indicator) + (overheat_counter)) + (((effect)->Detail)())) + ", if itself is ") + (((srccond)->DetailAlt3)()));
  }
}

string Card__srcCondUntargetedEff::Postfix()
{

  return (((effect)->Postfix)());
}

string Card__srcCondUntargetedEff::PostfixIndent(signed int  indent_size)
{

  return (((effect)->PostfixIndent)((indent_size)));
}

Card__Node  *Card__srcCondUntargetedEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  {
    Card__srcCondUntargetedEff  *effect_copy = (new Card__srcCondUntargetedEff((card_copy), ((Card__BaseUntargetedEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__TargetCond *)((((srccond)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
    (((effect_copy)->overheat_count) = (overheat_count));
    (((effect_copy)->overheat_threshold) = (overheat_threshold));
    return (effect_copy);
  }
}

unsigned int Card__srcCondUntargetedEff::GetInitAttrFlag()
{

  return ((((effect)->GetInitAttrFlag)()) | (((srccond)->GetInitAttrFlag)()));
}

CondConfig Card__srcCondUntargetedEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig tmp_config = (((effect)->GetGlobalSelfConfig)((self_config), (effect_timing)));
    CondConfig self_config_copy = (self_config);
    ((self_config_copy) &= (tmp_config));
    ((tmp_config) &= (((srccond)->GetGlobalSelfConfig)((self_config_copy), (effect_timing))));
    return (tmp_config);
  }
}

void Card__srcCondUntargetedEff::UntargetedAction(Card  * parent_card)
{

  {
    if (((((srccond)->CheckThisValid)((parent_card))) && ((overheat_count) < (overheat_threshold))))
    {
      {
        ((overheat_count)++);
        (((parent_card)->IncContribution)());
        (((effect)->UntargetedAction)((parent_card)));
      }
    } else {
      
    }
  }
}



Card__charTargetCond::Card__charTargetCond()
{

  
}

Card__charTargetCond::Card__charTargetCond(Card  * input__item, Card__CharTargetCond  * input__cond) : Card__TargetCond((input__item)), cond((input__cond))
{

  
  
}

Card__charTargetCond::~Card__charTargetCond()
{

  
  {
    if ((cond))
    {
      delete (cond);
    } else {
      
    }
  }
}


void Card__charTargetCond::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
    (((cond)->FillRep)((rep)));
  }
}

string Card__charTargetCond::Detail()
{

  return (((cond)->Detail)());
}

string Card__charTargetCond::DetailAlt1()
{

  return (((cond)->DetailAlt1)());
}

string Card__charTargetCond::DetailAlt2()
{

  return (((cond)->DetailAlt2)());
}

string Card__charTargetCond::DetailAlt3()
{

  return ((((cond)->DetailAlt3)()) + " on the field");
}

Card__Node  *Card__charTargetCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__charTargetCond((card_copy), ((Card__CharTargetCond *)((((cond)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

unsigned int Card__charTargetCond::GetInitAttrFlag()
{

  return (((cond)->GetInitAttrFlag)());
}

CondConfig Card__charTargetCond::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  return (((cond)->GetGlobalSelfConfig)((self_config), (effect_timing)));
}

signed int Card__charTargetCond::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return (((((parent_card)->owner)->IsValidCharTarget)((z))) && (((cond)->CheckPlayValid)((x), (y), (z), (parent_card))));
}

signed int Card__charTargetCond::CheckCardValid(Card  * card, Card  * parent_card)
{

  return ((((((card)->card_pos) == (CARD_POS_AT_LEADER)) || (((card)->card_pos) == (CARD_POS_AT_FIELD)))) && (((cond)->CheckCardValid)((card), (parent_card))));
}

signed int Card__charTargetCond::CheckThisValid(Card  * parent_card)
{

  return ((((((parent_card)->card_pos) == (CARD_POS_AT_LEADER)) || (((parent_card)->card_pos) == (CARD_POS_AT_FIELD)))) && (((cond)->CheckThisValid)((parent_card))));
}



Card__cardTargetCond::Card__cardTargetCond()
{

  
}

Card__cardTargetCond::Card__cardTargetCond(Card  * input__item, Card__CardTargetCond  * input__cond) : Card__TargetCond((input__item)), cond((input__cond))
{

  
  
}

Card__cardTargetCond::~Card__cardTargetCond()
{

  
  {
    if ((cond))
    {
      delete (cond);
    } else {
      
    }
  }
}


void Card__cardTargetCond::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
    (((cond)->FillRep)((rep)));
  }
}

string Card__cardTargetCond::Detail()
{

  return (((cond)->Detail)());
}

string Card__cardTargetCond::DetailAlt1()
{

  return (((cond)->DetailAlt1)());
}

string Card__cardTargetCond::DetailAlt2()
{

  return (((cond)->DetailAlt2)());
}

string Card__cardTargetCond::DetailAlt3()
{

  return (((cond)->DetailAlt3)());
}

Card__Node  *Card__cardTargetCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__cardTargetCond((card_copy), ((Card__CardTargetCond *)((((cond)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

unsigned int Card__cardTargetCond::GetInitAttrFlag()
{

  return (((cond)->GetInitAttrFlag)());
}

CondConfig Card__cardTargetCond::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  return (((cond)->GetGlobalSelfConfig)((self_config), (effect_timing)));
}

signed int Card__cardTargetCond::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return (((((parent_card)->owner)->IsValidCardTarget)((z))) && (((cond)->CheckPlayValid)((x), (y), (z), (parent_card))));
}

signed int Card__cardTargetCond::CheckCardValid(Card  * card, Card  * parent_card)
{

  return ((((((card)->card_pos) == (CARD_POS_AT_HAND)) || (((card)->card_pos) == (CARD_POS_AT_DECK)))) && (((cond)->CheckCardValid)((card), (parent_card))));
}

signed int Card__cardTargetCond::CheckThisValid(Card  * parent_card)
{

  return ((((((parent_card)->card_pos) == (CARD_POS_AT_HAND)) || (((parent_card)->card_pos) == (CARD_POS_AT_DECK)))) && (((cond)->CheckThisValid)((parent_card))));
}



Card__justCharTargetCond::Card__justCharTargetCond()
{

  
}

Card__justCharTargetCond::Card__justCharTargetCond(Card  * input__item, Card__AllegianceCond  * input__alle, Card__CharTypeCond  * input__typecond, Card__AttrCond  * input__attrcond, Card__StatCond  * input__statcond) : Card__CharTargetCond((input__item)), alle((input__alle)), typecond((input__typecond)), attrcond((input__attrcond)), statcond((input__statcond))
{

  
  
  
  
  
}

Card__justCharTargetCond::~Card__justCharTargetCond()
{

  
  {
    if ((alle))
    {
      delete (alle);
    } else {
      
    }
  }
  {
    if ((attrcond))
    {
      delete (attrcond);
    } else {
      
    }
  }
  {
    if ((statcond))
    {
      delete (statcond);
    } else {
      
    }
  }
  {
    if ((typecond))
    {
      delete (typecond);
    } else {
      
    }
  }
}





void Card__justCharTargetCond::FillRep(CardRep  & rep)
{

  {
    (((alle)->FillRep)((rep)));
    (((typecond)->FillRep)((rep)));
    (((attrcond)->FillRep)((rep)));
    (((statcond)->FillRep)((rep)));
  }
}

string Card__justCharTargetCond::Detail()
{

  return ((((((alle)->Detail)()) + (((typecond)->Detail)())) + (((attrcond)->Detail)())) + (((statcond)->Detail)()));
}

string Card__justCharTargetCond::DetailAlt1()
{

  return (((((((((((alle)->Detail)())).length)()) == 0) ? (((typecond)->DetailAlt1)()) : ((((alle)->DetailAlt1)()) + (((typecond)->Detail)())))) + (((attrcond)->Detail)())) + (((statcond)->Detail)()));
}

string Card__justCharTargetCond::DetailAlt2()
{

  return ((((((alle)->Detail)()) + (((typecond)->DetailAlt2)())) + (((attrcond)->Detail)())) + (((statcond)->Detail)()));
}

string Card__justCharTargetCond::DetailAlt3()
{

  return (((((typecond)->DetailAlt1)()) + (((attrcond)->Detail)())) + (((statcond)->Detail)()));
}

Card__Node  *Card__justCharTargetCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__justCharTargetCond((card_copy), ((Card__AllegianceCond *)((((alle)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__CharTypeCond *)((((typecond)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__AttrCond *)((((attrcond)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__StatCond *)((((statcond)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

unsigned int Card__justCharTargetCond::GetInitAttrFlag()
{

  return (((attrcond)->GetInitAttrFlag)());
}

CondConfig Card__justCharTargetCond::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig tmp_config = (((typecond)->GetGlobalSelfConfig)((self_config), (effect_timing)));
    ((tmp_config) &= (((attrcond)->GetGlobalSelfConfig)((self_config), (effect_timing))));
    ((tmp_config) &= (((statcond)->GetGlobalSelfConfig)((self_config), (effect_timing))));
    return (tmp_config);
  }
}

signed int Card__justCharTargetCond::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return ((((((alle)->CheckPlayValid)((x), (y), (z), (parent_card))) && (((typecond)->CheckPlayValid)((x), (y), (z), (parent_card)))) && (((attrcond)->CheckPlayValid)((x), (y), (z), (parent_card)))) && (((statcond)->CheckPlayValid)((x), (y), (z), (parent_card))));
}

signed int Card__justCharTargetCond::CheckCardValid(Card  * card, Card  * parent_card)
{

  return ((((((alle)->CheckCardValid)((card), (parent_card))) && (((typecond)->CheckCardValid)((card), (parent_card)))) && (((attrcond)->CheckCardValid)((card), (parent_card)))) && (((statcond)->CheckCardValid)((card), (parent_card))));
}

signed int Card__justCharTargetCond::CheckThisValid(Card  * parent_card)
{

  return ((((((alle)->CheckThisValid)((parent_card))) && (((typecond)->CheckThisValid)((parent_card)))) && (((attrcond)->CheckThisValid)((parent_card)))) && (((statcond)->CheckThisValid)((parent_card))));
}



Card__isCharacter::Card__isCharacter()
{

  
}

Card__isCharacter::Card__isCharacter(Card  * input__item) : Card__CharTypeCond((input__item))
{

  
}

Card__isCharacter::~Card__isCharacter()
{

  
}

void Card__isCharacter::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
  }
}

signed int Card__isCharacter::isCondTrivial()
{

  return 1;
}

string Card__isCharacter::Detail()
{

  return "character";
}

string Card__isCharacter::DetailAlt1()
{

  return "a character";
}

string Card__isCharacter::DetailAlt2()
{

  return "characters";
}

Card__Node  *Card__isCharacter::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__isCharacter((card_copy)));
}

CondConfig Card__isCharacter::GetTargetConfig()
{

  {
    CondConfig tmp_config = (CHAR_COND_FILTER);
    (((tmp_config).max_hp) = 10);
    if ((((tmp_config).min_hp) > 10))
    {
      (((tmp_config).min_hp) = 10);
    } else {
      
    }
    return (tmp_config);
  }
}



Card__isMinion::Card__isMinion()
{

  
}

Card__isMinion::Card__isMinion(Card  * input__item) : Card__CharTypeCond((input__item))
{

  
}

Card__isMinion::~Card__isMinion()
{

  
}

void Card__isMinion::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
  }
}

signed int Card__isMinion::isCondTrivial()
{

  return 1;
}

string Card__isMinion::Detail()
{

  return "minion";
}

string Card__isMinion::DetailAlt1()
{

  return "a minion";
}

string Card__isMinion::DetailAlt2()
{

  return "minions";
}

Card__Node  *Card__isMinion::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__isMinion((card_copy)));
}

CondConfig Card__isMinion::GetTargetConfig()
{

  {
    CondConfig tmp_config = (MINION_COND_FILTER);
    (((tmp_config).max_hp) = 10);
    if ((((tmp_config).min_hp) > 10))
    {
      (((tmp_config).min_hp) = 10);
    } else {
      
    }
    return (tmp_config);
  }
}

signed int Card__isMinion::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return ((((parent_card)->owner)->IsValidMinionTarget)((z)));
}

signed int Card__isMinion::CheckCardValid(Card  * card, Card  * parent_card)
{

  return (((card)->card_type) == (MINION_CARD));
}

signed int Card__isMinion::CheckThisValid(Card  * parent_card)
{

  return (((parent_card)->card_type) == (MINION_CARD));
}



Card__isBeast::Card__isBeast()
{

  
}

Card__isBeast::Card__isBeast(Card  * input__item) : Card__CharTypeCond((input__item))
{

  
}

Card__isBeast::~Card__isBeast()
{

  
}

void Card__isBeast::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(2))));
  }
}

string Card__isBeast::Detail()
{

  return "Beast";
}

string Card__isBeast::DetailAlt1()
{

  return "a Beast";
}

string Card__isBeast::DetailAlt2()
{

  return "Beasts";
}

Card__Node  *Card__isBeast::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__isBeast((card_copy)));
}

CondConfig Card__isBeast::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig tmp_config = (NOT_BEAST_COND_FILTER);
    (((tmp_config).max_hp) = 10);
    if ((((tmp_config).min_hp) > 10))
    {
      (((tmp_config).min_hp) = 10);
    } else {
      
    }
    return (tmp_config);
  }
}

CondConfig Card__isBeast::GetTargetConfig()
{

  {
    CondConfig tmp_config = (BEAST_COND_FILTER);
    (((tmp_config).max_hp) = 10);
    if ((((tmp_config).min_hp) > 10))
    {
      (((tmp_config).min_hp) = 10);
    } else {
      
    }
    return (tmp_config);
  }
}

signed int Card__isBeast::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return (((((parent_card)->owner)->IsValidMinionTarget)((z))) && ((((((parent_card)->owner)->GetTargetCard)((z)))->minion_type) == (BEAST_MINION)));
}

signed int Card__isBeast::CheckCardValid(Card  * card, Card  * parent_card)
{

  return (((card)->minion_type) == (BEAST_MINION));
}

signed int Card__isBeast::CheckThisValid(Card  * parent_card)
{

  return (((parent_card)->minion_type) == (BEAST_MINION));
}



Card__isDragon::Card__isDragon()
{

  
}

Card__isDragon::Card__isDragon(Card  * input__item) : Card__CharTypeCond((input__item))
{

  
}

Card__isDragon::~Card__isDragon()
{

  
}

void Card__isDragon::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(3))));
  }
}

string Card__isDragon::Detail()
{

  return "Dragon";
}

string Card__isDragon::DetailAlt1()
{

  return "a Dragon";
}

string Card__isDragon::DetailAlt2()
{

  return "Dragons";
}

Card__Node  *Card__isDragon::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__isDragon((card_copy)));
}

CondConfig Card__isDragon::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig tmp_config = (NOT_DRAGON_COND_FILTER);
    (((tmp_config).max_hp) = 10);
    if ((((tmp_config).min_hp) > 10))
    {
      (((tmp_config).min_hp) = 10);
    } else {
      
    }
    return (tmp_config);
  }
}

CondConfig Card__isDragon::GetTargetConfig()
{

  {
    CondConfig tmp_config = (DRAGON_COND_FILTER);
    (((tmp_config).max_hp) = 10);
    if ((((tmp_config).min_hp) > 10))
    {
      (((tmp_config).min_hp) = 10);
    } else {
      
    }
    return (tmp_config);
  }
}

signed int Card__isDragon::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return (((((parent_card)->owner)->IsValidMinionTarget)((z))) && ((((((parent_card)->owner)->GetTargetCard)((z)))->minion_type) == (DRAGON_MINION)));
}

signed int Card__isDragon::CheckCardValid(Card  * card, Card  * parent_card)
{

  return (((card)->minion_type) == (DRAGON_MINION));
}

signed int Card__isDragon::CheckThisValid(Card  * parent_card)
{

  return (((parent_card)->minion_type) == (DRAGON_MINION));
}



Card__isDemon::Card__isDemon()
{

  
}

Card__isDemon::Card__isDemon(Card  * input__item) : Card__CharTypeCond((input__item))
{

  
}

Card__isDemon::~Card__isDemon()
{

  
}

void Card__isDemon::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(4))));
  }
}

string Card__isDemon::Detail()
{

  return "Demon";
}

string Card__isDemon::DetailAlt1()
{

  return "a Demon";
}

string Card__isDemon::DetailAlt2()
{

  return "Demons";
}

Card__Node  *Card__isDemon::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__isDemon((card_copy)));
}

CondConfig Card__isDemon::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig tmp_config = (NOT_DEMON_COND_FILTER);
    (((tmp_config).max_hp) = 10);
    if ((((tmp_config).min_hp) > 10))
    {
      (((tmp_config).min_hp) = 10);
    } else {
      
    }
    return (tmp_config);
  }
}

CondConfig Card__isDemon::GetTargetConfig()
{

  {
    CondConfig tmp_config = (DEMON_COND_FILTER);
    (((tmp_config).max_hp) = 10);
    if ((((tmp_config).min_hp) > 10))
    {
      (((tmp_config).min_hp) = 10);
    } else {
      
    }
    return (tmp_config);
  }
}

signed int Card__isDemon::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return (((((parent_card)->owner)->IsValidMinionTarget)((z))) && ((((((parent_card)->owner)->GetTargetCard)((z)))->minion_type) == (DEMON_MINION)));
}

signed int Card__isDemon::CheckCardValid(Card  * card, Card  * parent_card)
{

  return (((card)->minion_type) == (DEMON_MINION));
}

signed int Card__isDemon::CheckThisValid(Card  * parent_card)
{

  return (((parent_card)->minion_type) == (DEMON_MINION));
}



Card__justCardTargetCond::Card__justCardTargetCond()
{

  
}

Card__justCardTargetCond::Card__justCardTargetCond(Card  * input__item, Card__CardPosCond  * input__pos, Card__AllegianceCond  * input__alle, Card__CardTypeCond  * input__typecond, Card__AttrCond  * input__attrcond, Card__StatCond  * input__statcond) : Card__CardTargetCond((input__item)), pos((input__pos)), alle((input__alle)), typecond((input__typecond)), attrcond((input__attrcond)), statcond((input__statcond))
{

  
  
  
  
  
  
}

Card__justCardTargetCond::~Card__justCardTargetCond()
{

  
  {
    if ((alle))
    {
      delete (alle);
    } else {
      
    }
  }
  {
    if ((attrcond))
    {
      delete (attrcond);
    } else {
      
    }
  }
  {
    if ((pos))
    {
      delete (pos);
    } else {
      
    }
  }
  {
    if ((statcond))
    {
      delete (statcond);
    } else {
      
    }
  }
  {
    if ((typecond))
    {
      delete (typecond);
    } else {
      
    }
  }
}






void Card__justCardTargetCond::FillRep(CardRep  & rep)
{

  {
    (((pos)->FillRep)((rep)));
    (((alle)->FillRep)((rep)));
    (((typecond)->FillRep)((rep)));
    (((attrcond)->FillRep)((rep)));
    (((statcond)->FillRep)((rep)));
  }
}

string Card__justCardTargetCond::Detail()
{

  return ((((((((typecond)->Detail)()) + (((attrcond)->Detail)())) + (((statcond)->Detail)())) + " at ") + (((alle)->DetailAlt3)())) + (((pos)->Detail)()));
}

string Card__justCardTargetCond::DetailAlt1()
{

  return ((((((((typecond)->DetailAlt1)()) + (((attrcond)->Detail)())) + (((statcond)->Detail)())) + " at ") + (((alle)->DetailAlt2)())) + (((pos)->Detail)()));
}

string Card__justCardTargetCond::DetailAlt2()
{

  return ((((((((typecond)->DetailAlt2)()) + (((attrcond)->Detail)())) + (((statcond)->Detail)())) + " at ") + (((alle)->DetailAlt2)())) + (((((((((alle)->Detail)())).length)()) == 0) ? (((pos)->DetailAlt2)()) : (((pos)->DetailAlt1)()))));
}

string Card__justCardTargetCond::DetailAlt3()
{

  return (((((((typecond)->DetailAlt1)()) + (((attrcond)->Detail)())) + (((statcond)->Detail)())) + " at ") + (((pos)->Detail)()));
}

Card__Node  *Card__justCardTargetCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__justCardTargetCond((card_copy), ((Card__CardPosCond *)((((pos)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__AllegianceCond *)((((alle)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__CardTypeCond *)((((typecond)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__AttrCond *)((((attrcond)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__StatCond *)((((statcond)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

unsigned int Card__justCardTargetCond::GetInitAttrFlag()
{

  return (((attrcond)->GetInitAttrFlag)());
}

CondConfig Card__justCardTargetCond::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig tmp_config = (((typecond)->GetGlobalSelfConfig)((self_config), (effect_timing)));
    ((tmp_config) &= (((attrcond)->GetGlobalSelfConfig)((self_config), (effect_timing))));
    ((tmp_config) &= (((statcond)->GetGlobalSelfConfig)((self_config), (effect_timing))));
    return (tmp_config);
  }
}

signed int Card__justCardTargetCond::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return (((((((pos)->CheckPlayValid)((x), (y), (z), (parent_card))) && (((alle)->CheckPlayValid)((x), (y), (z), (parent_card)))) && (((typecond)->CheckPlayValid)((x), (y), (z), (parent_card)))) && (((attrcond)->CheckPlayValid)((x), (y), (z), (parent_card)))) && (((statcond)->CheckPlayValid)((x), (y), (z), (parent_card))));
}

signed int Card__justCardTargetCond::CheckCardValid(Card  * card, Card  * parent_card)
{

  return (((((((pos)->CheckCardValid)((card), (parent_card))) && (((alle)->CheckCardValid)((card), (parent_card)))) && (((typecond)->CheckCardValid)((card), (parent_card)))) && (((attrcond)->CheckCardValid)((card), (parent_card)))) && (((statcond)->CheckCardValid)((card), (parent_card))));
}

signed int Card__justCardTargetCond::CheckThisValid(Card  * parent_card)
{

  return (((((((pos)->CheckThisValid)((parent_card))) && (((alle)->CheckThisValid)((parent_card)))) && (((typecond)->CheckThisValid)((parent_card)))) && (((attrcond)->CheckThisValid)((parent_card)))) && (((statcond)->CheckThisValid)((parent_card))));
}



Card__isCard::Card__isCard()
{

  
}

Card__isCard::Card__isCard(Card  * input__item) : Card__CardTypeCond((input__item))
{

  
}

Card__isCard::~Card__isCard()
{

  
}

void Card__isCard::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
  }
}

signed int Card__isCard::isCondTrivial()
{

  return 1;
}

string Card__isCard::Detail()
{

  return "card";
}

string Card__isCard::DetailAlt1()
{

  return "a card";
}

string Card__isCard::DetailAlt2()
{

  return "cards";
}

Card__Node  *Card__isCard::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__isCard((card_copy)));
}

CondConfig Card__isCard::GetTargetConfig()
{

  {
    CondConfig tmp_config = (TARGET_TYPE_ANY);
    (((tmp_config).max_hp) = 10);
    if ((((tmp_config).min_hp) > 10))
    {
      (((tmp_config).min_hp) = 10);
    } else {
      
    }
    return (tmp_config);
  }
}



Card__isLeaderCard::Card__isLeaderCard()
{

  
}

Card__isLeaderCard::Card__isLeaderCard(Card  * input__item) : Card__CardTypeCond((input__item))
{

  
}

Card__isLeaderCard::~Card__isLeaderCard()
{

  
}

void Card__isLeaderCard::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
  }
}

signed int Card__isLeaderCard::isCondTrivial()
{

  return 1;
}

string Card__isLeaderCard::Detail()
{

  return "leader card";
}

string Card__isLeaderCard::DetailAlt1()
{

  return "a leader card";
}

string Card__isLeaderCard::DetailAlt2()
{

  return "leader cards";
}

Card__Node  *Card__isLeaderCard::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__isLeaderCard((card_copy)));
}

CondConfig Card__isLeaderCard::GetTargetConfig()
{

  return (LEADER_COND_FILTER);
}

signed int Card__isLeaderCard::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return ((((((parent_card)->owner)->GetTargetCard)((z)))->card_type) == (LEADER_CARD));
}

signed int Card__isLeaderCard::CheckCardValid(Card  * card, Card  * parent_card)
{

  return (((card)->card_type) == (LEADER_CARD));
}

signed int Card__isLeaderCard::CheckThisValid(Card  * parent_card)
{

  return (((parent_card)->card_type) == (LEADER_CARD));
}



Card__isMinionCard::Card__isMinionCard()
{

  
}

Card__isMinionCard::Card__isMinionCard(Card  * input__item) : Card__CardTypeCond((input__item))
{

  
}

Card__isMinionCard::~Card__isMinionCard()
{

  
}

void Card__isMinionCard::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(2))));
  }
}

signed int Card__isMinionCard::isCondTrivial()
{

  return 1;
}

string Card__isMinionCard::Detail()
{

  return "minion card";
}

string Card__isMinionCard::DetailAlt1()
{

  return "a minion card";
}

string Card__isMinionCard::DetailAlt2()
{

  return "minion cards";
}

Card__Node  *Card__isMinionCard::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__isMinionCard((card_copy)));
}

CondConfig Card__isMinionCard::GetTargetConfig()
{

  {
    CondConfig tmp_config = (MINION_COND_FILTER);
    (((tmp_config).max_hp) = 10);
    if ((((tmp_config).min_hp) > 10))
    {
      (((tmp_config).min_hp) = 10);
    } else {
      
    }
    return (tmp_config);
  }
}

signed int Card__isMinionCard::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return ((((((parent_card)->owner)->GetTargetCard)((z)))->card_type) == (MINION_CARD));
}

signed int Card__isMinionCard::CheckCardValid(Card  * card, Card  * parent_card)
{

  return (((card)->card_type) == (MINION_CARD));
}

signed int Card__isMinionCard::CheckThisValid(Card  * parent_card)
{

  return (((parent_card)->card_type) == (MINION_CARD));
}



Card__isSpellCard::Card__isSpellCard()
{

  
}

Card__isSpellCard::Card__isSpellCard(Card  * input__item) : Card__CardTypeCond((input__item))
{

  
}

Card__isSpellCard::~Card__isSpellCard()
{

  
}

void Card__isSpellCard::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(3))));
  }
}

signed int Card__isSpellCard::isCondTrivial()
{

  return 1;
}

string Card__isSpellCard::Detail()
{

  return "spell card";
}

string Card__isSpellCard::DetailAlt1()
{

  return "a spell card";
}

string Card__isSpellCard::DetailAlt2()
{

  return "spell cards";
}

Card__Node  *Card__isSpellCard::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__isSpellCard((card_copy)));
}

CondConfig Card__isSpellCard::GetTargetConfig()
{

  return (SPELL_COND_FILTER);
}

signed int Card__isSpellCard::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return ((((((parent_card)->owner)->GetTargetCard)((z)))->card_type) == (SPELL_CARD));
}

signed int Card__isSpellCard::CheckCardValid(Card  * card, Card  * parent_card)
{

  return (((card)->card_type) == (SPELL_CARD));
}

signed int Card__isSpellCard::CheckThisValid(Card  * parent_card)
{

  return (((parent_card)->card_type) == (SPELL_CARD));
}



Card__isBeastCard::Card__isBeastCard()
{

  
}

Card__isBeastCard::Card__isBeastCard(Card  * input__item) : Card__CardTypeCond((input__item))
{

  
}

Card__isBeastCard::~Card__isBeastCard()
{

  
}

void Card__isBeastCard::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(4))));
  }
}

string Card__isBeastCard::Detail()
{

  return "Beast card";
}

string Card__isBeastCard::DetailAlt1()
{

  return "a Beast card";
}

string Card__isBeastCard::DetailAlt2()
{

  return "Beast cards";
}

Card__Node  *Card__isBeastCard::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__isBeastCard((card_copy)));
}

CondConfig Card__isBeastCard::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig tmp_config = (NOT_BEAST_COND_FILTER);
    (((tmp_config).max_hp) = 10);
    if ((((tmp_config).min_hp) > 10))
    {
      (((tmp_config).min_hp) = 10);
    } else {
      
    }
    return (tmp_config);
  }
}

CondConfig Card__isBeastCard::GetTargetConfig()
{

  {
    CondConfig tmp_config = (BEAST_COND_FILTER);
    (((tmp_config).max_hp) = 10);
    if ((((tmp_config).min_hp) > 10))
    {
      (((tmp_config).min_hp) = 10);
    } else {
      
    }
    return (tmp_config);
  }
}

signed int Card__isBeastCard::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return ((((((parent_card)->owner)->GetTargetCard)((z)))->minion_type) == (BEAST_MINION));
}

signed int Card__isBeastCard::CheckCardValid(Card  * card, Card  * parent_card)
{

  return (((card)->minion_type) == (BEAST_MINION));
}

signed int Card__isBeastCard::CheckThisValid(Card  * parent_card)
{

  return (((parent_card)->minion_type) == (BEAST_MINION));
}



Card__isDragonCard::Card__isDragonCard()
{

  
}

Card__isDragonCard::Card__isDragonCard(Card  * input__item) : Card__CardTypeCond((input__item))
{

  
}

Card__isDragonCard::~Card__isDragonCard()
{

  
}

void Card__isDragonCard::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(5))));
  }
}

string Card__isDragonCard::Detail()
{

  return "Dragon card";
}

string Card__isDragonCard::DetailAlt1()
{

  return "a Dragon card";
}

string Card__isDragonCard::DetailAlt2()
{

  return "Dragon cards";
}

Card__Node  *Card__isDragonCard::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__isDragonCard((card_copy)));
}

CondConfig Card__isDragonCard::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig tmp_config = (NOT_DRAGON_COND_FILTER);
    (((tmp_config).max_hp) = 10);
    if ((((tmp_config).min_hp) > 10))
    {
      (((tmp_config).min_hp) = 10);
    } else {
      
    }
    return (tmp_config);
  }
}

CondConfig Card__isDragonCard::GetTargetConfig()
{

  {
    CondConfig tmp_config = (DRAGON_COND_FILTER);
    (((tmp_config).max_hp) = 10);
    if ((((tmp_config).min_hp) > 10))
    {
      (((tmp_config).min_hp) = 10);
    } else {
      
    }
    return (tmp_config);
  }
}

signed int Card__isDragonCard::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return ((((((parent_card)->owner)->GetTargetCard)((z)))->minion_type) == (DRAGON_MINION));
}

signed int Card__isDragonCard::CheckCardValid(Card  * card, Card  * parent_card)
{

  return (((card)->minion_type) == (DRAGON_MINION));
}

signed int Card__isDragonCard::CheckThisValid(Card  * parent_card)
{

  return (((parent_card)->minion_type) == (DRAGON_MINION));
}



Card__isDemonCard::Card__isDemonCard()
{

  
}

Card__isDemonCard::Card__isDemonCard(Card  * input__item) : Card__CardTypeCond((input__item))
{

  
}

Card__isDemonCard::~Card__isDemonCard()
{

  
}

void Card__isDemonCard::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(6))));
  }
}

string Card__isDemonCard::Detail()
{

  return "Demon card";
}

string Card__isDemonCard::DetailAlt1()
{

  return "a Demon card";
}

string Card__isDemonCard::DetailAlt2()
{

  return "Demon cards";
}

Card__Node  *Card__isDemonCard::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__isDemonCard((card_copy)));
}

CondConfig Card__isDemonCard::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig tmp_config = (NOT_DEMON_COND_FILTER);
    (((tmp_config).max_hp) = 10);
    if ((((tmp_config).min_hp) > 10))
    {
      (((tmp_config).min_hp) = 10);
    } else {
      
    }
    return (tmp_config);
  }
}

CondConfig Card__isDemonCard::GetTargetConfig()
{

  {
    CondConfig tmp_config = (DEMON_COND_FILTER);
    (((tmp_config).max_hp) = 10);
    if ((((tmp_config).min_hp) > 10))
    {
      (((tmp_config).min_hp) = 10);
    } else {
      
    }
    return (tmp_config);
  }
}

signed int Card__isDemonCard::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return ((((((parent_card)->owner)->GetTargetCard)((z)))->minion_type) == (DEMON_MINION));
}

signed int Card__isDemonCard::CheckCardValid(Card  * card, Card  * parent_card)
{

  return (((card)->minion_type) == (DEMON_MINION));
}

signed int Card__isDemonCard::CheckThisValid(Card  * parent_card)
{

  return (((parent_card)->minion_type) == (DEMON_MINION));
}



Card__cardPosAtHandOrDeck::Card__cardPosAtHandOrDeck()
{

  
}

Card__cardPosAtHandOrDeck::Card__cardPosAtHandOrDeck(Card  * input__item) : Card__CardPosCond((input__item))
{

  
}

Card__cardPosAtHandOrDeck::~Card__cardPosAtHandOrDeck()
{

  
}

void Card__cardPosAtHandOrDeck::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
  }
}

signed int Card__cardPosAtHandOrDeck::isCondTrivial()
{

  return 1;
}

string Card__cardPosAtHandOrDeck::Detail()
{

  return "hand or deck";
}

string Card__cardPosAtHandOrDeck::DetailAlt1()
{

  return "hand and deck";
}

string Card__cardPosAtHandOrDeck::DetailAlt2()
{

  return "hands and decks";
}

Card__Node  *Card__cardPosAtHandOrDeck::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__cardPosAtHandOrDeck((card_copy)));
}



Card__cardPosAtHand::Card__cardPosAtHand()
{

  
}

Card__cardPosAtHand::Card__cardPosAtHand(Card  * input__item) : Card__CardPosCond((input__item))
{

  
}

Card__cardPosAtHand::~Card__cardPosAtHand()
{

  
}

void Card__cardPosAtHand::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
  }
}

string Card__cardPosAtHand::Detail()
{

  return "hand";
}

string Card__cardPosAtHand::DetailAlt2()
{

  return "hands";
}

Card__Node  *Card__cardPosAtHand::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__cardPosAtHand((card_copy)));
}

signed int Card__cardPosAtHand::CheckCardValid(Card  * card, Card  * parent_card)
{

  return (((card)->card_pos) == (CARD_POS_AT_HAND));
}

signed int Card__cardPosAtHand::CheckThisValid(Card  * parent_card)
{

  return (((parent_card)->card_pos) == (CARD_POS_AT_HAND));
}



Card__cardPosAtDeck::Card__cardPosAtDeck()
{

  
}

Card__cardPosAtDeck::Card__cardPosAtDeck(Card  * input__item) : Card__CardPosCond((input__item))
{

  
}

Card__cardPosAtDeck::~Card__cardPosAtDeck()
{

  
}

void Card__cardPosAtDeck::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(2))));
  }
}

string Card__cardPosAtDeck::Detail()
{

  return "deck";
}

string Card__cardPosAtDeck::DetailAlt2()
{

  return "decks";
}

Card__Node  *Card__cardPosAtDeck::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__cardPosAtDeck((card_copy)));
}

signed int Card__cardPosAtDeck::CheckCardValid(Card  * card, Card  * parent_card)
{

  return (((card)->card_pos) == (CARD_POS_AT_DECK));
}

signed int Card__cardPosAtDeck::CheckThisValid(Card  * parent_card)
{

  return (((parent_card)->card_pos) == (CARD_POS_AT_DECK));
}



Card__anyAllegiance::Card__anyAllegiance()
{

  
}

Card__anyAllegiance::Card__anyAllegiance(Card  * input__item) : Card__AllegianceCond((input__item))
{

  
}

Card__anyAllegiance::~Card__anyAllegiance()
{

  
}

void Card__anyAllegiance::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
  }
}

signed int Card__anyAllegiance::isCondTrivial()
{

  return 1;
}

string Card__anyAllegiance::DetailAlt2()
{

  return "both players\' ";
}

string Card__anyAllegiance::DetailAlt3()
{

  return "either player\'s ";
}

string Card__anyAllegiance::DetailAlt4()
{

  return "both leaders";
}

string Card__anyAllegiance::DetailAlt5()
{

  return "Both players ";
}

string Card__anyAllegiance::DetailAlt6()
{

  return "both players";
}

signed int Card__anyAllegiance::IsPlural()
{

  return 1;
}

Card__Node  *Card__anyAllegiance::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__anyAllegiance((card_copy)));
}



Card__allyAllegiance::Card__allyAllegiance()
{

  
}

Card__allyAllegiance::Card__allyAllegiance(Card  * input__item) : Card__AllegianceCond((input__item))
{

  
}

Card__allyAllegiance::~Card__allyAllegiance()
{

  
}

void Card__allyAllegiance::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
  }
}

string Card__allyAllegiance::Detail()
{

  return "friendly ";
}

string Card__allyAllegiance::DetailAlt1()
{

  return "a friendly ";
}

string Card__allyAllegiance::DetailAlt2()
{

  return "your ";
}

string Card__allyAllegiance::DetailAlt3()
{

  return "your ";
}

string Card__allyAllegiance::DetailAlt4()
{

  return "your leader";
}

string Card__allyAllegiance::DetailAlt5()
{

  return "You ";
}

string Card__allyAllegiance::DetailAlt6()
{

  return "you";
}

Card__Node  *Card__allyAllegiance::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__allyAllegiance((card_copy)));
}

CondConfig Card__allyAllegiance::GetTargetConfig()
{

  return (ALLY_COND_FILTER);
}

signed int Card__allyAllegiance::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return ((((parent_card)->owner)->IsTargetAlly)((z)));
}

signed int Card__allyAllegiance::CheckCardValid(Card  * card, Card  * parent_card)
{

  return (((card)->owner) == ((parent_card)->owner));
}



Card__oppoAllegiance::Card__oppoAllegiance()
{

  
}

Card__oppoAllegiance::Card__oppoAllegiance(Card  * input__item) : Card__AllegianceCond((input__item))
{

  
}

Card__oppoAllegiance::~Card__oppoAllegiance()
{

  
}

void Card__oppoAllegiance::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(2))));
  }
}

string Card__oppoAllegiance::Detail()
{

  return "enemy ";
}

string Card__oppoAllegiance::DetailAlt1()
{

  return "an enemy ";
}

string Card__oppoAllegiance::DetailAlt2()
{

  return "the opponent\'s ";
}

string Card__oppoAllegiance::DetailAlt3()
{

  return "the opponent\'s ";
}

string Card__oppoAllegiance::DetailAlt4()
{

  return "the opponent\'s leader";
}

string Card__oppoAllegiance::DetailAlt5()
{

  return "Your opponent ";
}

string Card__oppoAllegiance::DetailAlt6()
{

  return "your opponent";
}

signed int Card__oppoAllegiance::IsThirdPersonSingle()
{

  return 1;
}

Card__Node  *Card__oppoAllegiance::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__oppoAllegiance((card_copy)));
}

CondConfig Card__oppoAllegiance::GetTargetConfig()
{

  return (OPPO_COND_FILTER);
}

signed int Card__oppoAllegiance::CheckThisValid(Card  * parent_card)
{

  return 0;
}

signed int Card__oppoAllegiance::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return ((((parent_card)->owner)->IsTargetOpponent)((z)));
}

signed int Card__oppoAllegiance::CheckCardValid(Card  * card, Card  * parent_card)
{

  return (((card)->owner) == ((parent_card)->opponent));
}



Card__noAttrCond::Card__noAttrCond()
{

  
}

Card__noAttrCond::Card__noAttrCond(Card  * input__item) : Card__AttrCond((input__item))
{

  
}

Card__noAttrCond::~Card__noAttrCond()
{

  
}

void Card__noAttrCond::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
  }
}

signed int Card__noAttrCond::isCondTrivial()
{

  return 1;
}

Card__Node  *Card__noAttrCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__noAttrCond((card_copy)));
}



Card__chargeCond::Card__chargeCond()
{

  
}

Card__chargeCond::Card__chargeCond(Card  * input__item) : Card__AttrCond((input__item))
{

  
}

Card__chargeCond::~Card__chargeCond()
{

  
}

void Card__chargeCond::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
  }
}

string Card__chargeCond::Detail()
{

  return " with Charge";
}

Card__Node  *Card__chargeCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__chargeCond((card_copy)));
}

unsigned int Card__chargeCond::GetInitAttrFlag()
{

  return (TARGET_IS_CHARGE);
}

CondConfig Card__chargeCond::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  return (NOT_CHARGE_COND_FILTER);
}

signed int Card__chargeCond::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return (((((parent_card)->owner)->IsValidTarget)((z))) && (((((parent_card)->owner)->GetTargetCard)((z)))->is_charge));
}

signed int Card__chargeCond::CheckCardValid(Card  * card, Card  * parent_card)
{

  return ((card)->is_charge);
}

signed int Card__chargeCond::CheckThisValid(Card  * parent_card)
{

  return ((parent_card)->is_charge);
}



Card__tauntCond::Card__tauntCond()
{

  
}

Card__tauntCond::Card__tauntCond(Card  * input__item) : Card__AttrCond((input__item))
{

  
}

Card__tauntCond::~Card__tauntCond()
{

  
}

void Card__tauntCond::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(2))));
  }
}

string Card__tauntCond::Detail()
{

  return " with Taunt";
}

Card__Node  *Card__tauntCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__tauntCond((card_copy)));
}

unsigned int Card__tauntCond::GetInitAttrFlag()
{

  return (TARGET_IS_TAUNT);
}

CondConfig Card__tauntCond::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  return (NOT_TAUNT_COND_FILTER);
}

signed int Card__tauntCond::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return (((((parent_card)->owner)->IsValidTarget)((z))) && (((((parent_card)->owner)->GetTargetCard)((z)))->is_taunt));
}

signed int Card__tauntCond::CheckCardValid(Card  * card, Card  * parent_card)
{

  return ((card)->is_taunt);
}

signed int Card__tauntCond::CheckThisValid(Card  * parent_card)
{

  return ((parent_card)->is_taunt);
}



Card__stealthCond::Card__stealthCond()
{

  
}

Card__stealthCond::Card__stealthCond(Card  * input__item) : Card__AttrCond((input__item))
{

  
}

Card__stealthCond::~Card__stealthCond()
{

  
}

void Card__stealthCond::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(3))));
  }
}

string Card__stealthCond::Detail()
{

  return " with Stealth";
}

Card__Node  *Card__stealthCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__stealthCond((card_copy)));
}

unsigned int Card__stealthCond::GetInitAttrFlag()
{

  return (TARGET_IS_STEALTH);
}

CondConfig Card__stealthCond::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  return (((((!(((self_config) & (TARGET_POS_FIELD)))) || ((effect_timing) == (EFFECT_TIMING_PLAY)))) ? (NOT_STEALTH_COND_FILTER) : (TARGET_TYPE_ANY)));
}

signed int Card__stealthCond::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return (((((parent_card)->owner)->IsValidTarget)((z))) && (((((parent_card)->owner)->GetTargetCard)((z)))->is_stealth));
}

signed int Card__stealthCond::CheckCardValid(Card  * card, Card  * parent_card)
{

  return ((card)->is_stealth);
}

signed int Card__stealthCond::CheckThisValid(Card  * parent_card)
{

  return ((parent_card)->is_stealth);
}



Card__untargetableCond::Card__untargetableCond()
{

  
}

Card__untargetableCond::Card__untargetableCond(Card  * input__item) : Card__AttrCond((input__item))
{

  
}

Card__untargetableCond::~Card__untargetableCond()
{

  
}

void Card__untargetableCond::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(4))));
  }
}

string Card__untargetableCond::Detail()
{

  return " with Untargetability";
}

Card__Node  *Card__untargetableCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__untargetableCond((card_copy)));
}

unsigned int Card__untargetableCond::GetInitAttrFlag()
{

  return (TARGET_IS_UNTARGETABLE);
}

CondConfig Card__untargetableCond::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  return (NOT_UNTARGETABLE_COND_FILTER);
}

signed int Card__untargetableCond::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return (((((parent_card)->owner)->IsValidTarget)((z))) && (((((parent_card)->owner)->GetTargetCard)((z)))->is_untargetable));
}

signed int Card__untargetableCond::CheckCardValid(Card  * card, Card  * parent_card)
{

  return ((card)->is_untargetable);
}

signed int Card__untargetableCond::CheckThisValid(Card  * parent_card)
{

  return ((parent_card)->is_untargetable);
}



Card__shieldCond::Card__shieldCond()
{

  
}

Card__shieldCond::Card__shieldCond(Card  * input__item) : Card__AttrCond((input__item))
{

  
}

Card__shieldCond::~Card__shieldCond()
{

  
}

void Card__shieldCond::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(5))));
  }
}

string Card__shieldCond::Detail()
{

  return " with Shield";
}

Card__Node  *Card__shieldCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__shieldCond((card_copy)));
}

unsigned int Card__shieldCond::GetInitAttrFlag()
{

  return (TARGET_IS_SHIELDED);
}

CondConfig Card__shieldCond::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  return (((((!(((self_config) & (TARGET_POS_FIELD)))) || ((effect_timing) == (EFFECT_TIMING_PLAY)))) ? (NOT_SHIELDED_COND_FILTER) : (TARGET_TYPE_ANY)));
}

signed int Card__shieldCond::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return (((((parent_card)->owner)->IsValidTarget)((z))) && (((((parent_card)->owner)->GetTargetCard)((z)))->is_shielded));
}

signed int Card__shieldCond::CheckCardValid(Card  * card, Card  * parent_card)
{

  return ((card)->is_shielded);
}

signed int Card__shieldCond::CheckThisValid(Card  * parent_card)
{

  return ((parent_card)->is_shielded);
}



Card__poisonousCond::Card__poisonousCond()
{

  
}

Card__poisonousCond::Card__poisonousCond(Card  * input__item) : Card__AttrCond((input__item))
{

  
}

Card__poisonousCond::~Card__poisonousCond()
{

  
}

void Card__poisonousCond::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(6))));
  }
}

string Card__poisonousCond::Detail()
{

  return " with Poison";
}

Card__Node  *Card__poisonousCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__poisonousCond((card_copy)));
}

unsigned int Card__poisonousCond::GetInitAttrFlag()
{

  return (TARGET_IS_POISONOUS);
}

CondConfig Card__poisonousCond::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  return (NOT_POISONOUS_COND_FILTER);
}

signed int Card__poisonousCond::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return (((((parent_card)->owner)->IsValidTarget)((z))) && (((((parent_card)->owner)->GetTargetCard)((z)))->is_poisonous));
}

signed int Card__poisonousCond::CheckCardValid(Card  * card, Card  * parent_card)
{

  return ((card)->is_poisonous);
}

signed int Card__poisonousCond::CheckThisValid(Card  * parent_card)
{

  return ((parent_card)->is_poisonous);
}



Card__lifestealCond::Card__lifestealCond()
{

  
}

Card__lifestealCond::Card__lifestealCond(Card  * input__item) : Card__AttrCond((input__item))
{

  
}

Card__lifestealCond::~Card__lifestealCond()
{

  
}

void Card__lifestealCond::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(7))));
  }
}

string Card__lifestealCond::Detail()
{

  return " with Lifesteal";
}

Card__Node  *Card__lifestealCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__lifestealCond((card_copy)));
}

unsigned int Card__lifestealCond::GetInitAttrFlag()
{

  return (TARGET_IS_LIFESTEAL);
}

CondConfig Card__lifestealCond::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  return (NOT_LIFESTEAL_COND_FILTER);
}

signed int Card__lifestealCond::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return (((((parent_card)->owner)->IsValidTarget)((z))) && (((((parent_card)->owner)->GetTargetCard)((z)))->is_lifesteal));
}

signed int Card__lifestealCond::CheckCardValid(Card  * card, Card  * parent_card)
{

  return ((card)->is_lifesteal);
}

signed int Card__lifestealCond::CheckThisValid(Card  * parent_card)
{

  return ((parent_card)->is_lifesteal);
}



Card__noStatCond::Card__noStatCond()
{

  
}

Card__noStatCond::Card__noStatCond(Card  * input__item) : Card__StatCond((input__item))
{

  
}

Card__noStatCond::~Card__noStatCond()
{

  
}

void Card__noStatCond::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
  }
}

signed int Card__noStatCond::isCondTrivial()
{

  return 1;
}

Card__Node  *Card__noStatCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__noStatCond((card_copy)));
}



Card__costCond::Card__costCond()
{

  
}

Card__costCond::Card__costCond(Card  * input__item, Card__StatCondVariant  * input__variant) : Card__StatCond((input__item)), variant((input__variant))
{

  
  
}

Card__costCond::~Card__costCond()
{

  
  {
    if ((variant))
    {
      delete (variant);
    } else {
      
    }
  }
}


void Card__costCond::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
    (((variant)->FillRep)((rep)));
  }
}

string Card__costCond::Detail()
{

  return ((" having" + (((variant)->Detail)())) + " mana cost");
}

Card__Node  *Card__costCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__costCond((card_copy), ((Card__StatCondVariant *)((((variant)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

CondConfig Card__costCond::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    signed int min_val = ((self_config).min_cost);
    signed int max_val = ((self_config).max_cost);
    (((variant)->AdjustGlobalStatRange)((min_val), (max_val)));
    return ((GetCostConfig)((TARGET_TYPE_ANY), (min_val), (max_val)));
  }
}

signed int Card__costCond::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return (((variant)->CheckStatValid)((((((parent_card)->owner)->GetTargetCard)((z)))->mana)));
}

signed int Card__costCond::CheckCardValid(Card  * card, Card  * parent_card)
{

  return (((variant)->CheckStatValid)(((card)->mana)));
}

signed int Card__costCond::CheckThisValid(Card  * parent_card)
{

  return (((variant)->CheckStatValid)(((parent_card)->mana)));
}



Card__atkCond::Card__atkCond()
{

  
}

Card__atkCond::Card__atkCond(Card  * input__item, Card__StatCondVariant  * input__variant) : Card__StatCond((input__item)), variant((input__variant))
{

  
  
}

Card__atkCond::~Card__atkCond()
{

  
  {
    if ((variant))
    {
      delete (variant);
    } else {
      
    }
  }
}


void Card__atkCond::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(2))));
    (((variant)->FillRep)((rep)));
  }
}

string Card__atkCond::Detail()
{

  return ((" having" + (((variant)->Detail)())) + " attack");
}

Card__Node  *Card__atkCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__atkCond((card_copy), ((Card__StatCondVariant *)((((variant)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

CondConfig Card__atkCond::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    signed int min_val = ((self_config).min_atk);
    signed int max_val = ((self_config).max_atk);
    (((variant)->AdjustGlobalStatRange)((min_val), (max_val)));
    return ((GetAtkConfig)((CHAR_COND_FILTER), (min_val), (max_val)));
  }
}

signed int Card__atkCond::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  return (((variant)->CheckStatValid)((((((parent_card)->owner)->GetTargetCard)((z)))->atk)));
}

signed int Card__atkCond::CheckCardValid(Card  * card, Card  * parent_card)
{

  return (((variant)->CheckStatValid)(((card)->atk)));
}

signed int Card__atkCond::CheckThisValid(Card  * parent_card)
{

  return (((variant)->CheckStatValid)(((parent_card)->atk)));
}



Card__hpCond::Card__hpCond()
{

  
}

Card__hpCond::Card__hpCond(Card  * input__item, Card__StatCondVariant  * input__variant) : Card__StatCond((input__item)), variant((input__variant))
{

  
  
}

Card__hpCond::~Card__hpCond()
{

  
  {
    if ((variant))
    {
      delete (variant);
    } else {
      
    }
  }
}


void Card__hpCond::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(3))));
    (((variant)->FillRep)((rep)));
  }
}

string Card__hpCond::Detail()
{

  return ((" having" + (((variant)->Detail)())) + " health");
}

Card__Node  *Card__hpCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__hpCond((card_copy), ((Card__StatCondVariant *)((((variant)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

CondConfig Card__hpCond::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    signed int min_val = ((self_config).min_hp);
    signed int max_val = ((self_config).max_hp);
    (((variant)->AdjustGlobalStatRange)((min_val), (max_val)));
    return ((GetHpConfig)((CHAR_COND_FILTER), (min_val), (max_val)));
  }
}

signed int Card__hpCond::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  {
    Card  *card = ((((parent_card)->owner)->GetTargetCard)((z)));
    return (((variant)->CheckStatValid)((((card)->max_hp) - ((card)->hp_loss))));
  }
}

signed int Card__hpCond::CheckCardValid(Card  * card, Card  * parent_card)
{

  return (((variant)->CheckStatValid)((((card)->max_hp) - ((card)->hp_loss))));
}

signed int Card__hpCond::CheckThisValid(Card  * parent_card)
{

  return (((variant)->CheckStatValid)((((parent_card)->max_hp) - ((parent_card)->hp_loss))));
}



Card__atkTimesCond::Card__atkTimesCond()
{

  
}

Card__atkTimesCond::Card__atkTimesCond(Card  * input__item, Card__StatCondVariant  * input__variant) : Card__StatCond((input__item)), variant((input__variant))
{

  
  
}

Card__atkTimesCond::~Card__atkTimesCond()
{

  
  {
    if ((variant))
    {
      delete (variant);
    } else {
      
    }
  }
}


void Card__atkTimesCond::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(4))));
    (((variant)->FillRep)((rep)));
  }
}

string Card__atkTimesCond::Detail()
{

  return ((" having" + (((variant)->Detail)())) + (((((variant)->IsPlural)()) ? " attack times" : " attack time")));
}

Card__Node  *Card__atkTimesCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__atkTimesCond((card_copy), ((Card__StatCondVariant *)((((variant)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

CondConfig Card__atkTimesCond::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    signed int min_val = ((self_config).min_n_atks);
    signed int max_val = ((self_config).max_n_atks);
    (((variant)->AdjustGlobalStatRange)((min_val), (max_val)));
    return ((GetAtkTimesConfig)((CHAR_COND_FILTER), (min_val), (max_val)));
  }
}

signed int Card__atkTimesCond::CheckPlayValid(signed int  x, signed int  y, signed int  & z, Card  * parent_card)
{

  {
    Card  *card = ((((parent_card)->owner)->GetTargetCard)((z)));
    return (((variant)->CheckStatValid)((((card)->max_n_atks) - ((card)->n_atks_loss))));
  }
}

signed int Card__atkTimesCond::CheckCardValid(Card  * card, Card  * parent_card)
{

  return (((variant)->CheckStatValid)((((card)->max_n_atks) - ((card)->n_atks_loss))));
}

signed int Card__atkTimesCond::CheckThisValid(Card  * parent_card)
{

  return (((variant)->CheckStatValid)((((parent_card)->max_n_atks) - ((parent_card)->n_atks_loss))));
}



Card__statGe::Card__statGe()
{

  
}

Card__statGe::Card__statGe(Card  * input__item, signed int  input__val) : Card__StatCondVariant((input__item)), val((input__val))
{

  
  
}

Card__statGe::~Card__statGe()
{

  
  {
    
  }
}


void Card__statGe::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0, (vector<double>{((NormalizeCode)((val), 0, 10))})))));
  }
}

string Card__statGe::Detail()
{

  return (" at least " + ((IntToStr)((val))));
}

signed int Card__statGe::IsPlural()
{

  return ((((val) != 1) && ((val) != (-1))));
}

Card__Node  *Card__statGe::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__statGe((card_copy), (val)));
}

void Card__statGe::AdjustGlobalStatRange(signed int  & min_val, signed int  & max_val)
{

  {
    if (((max_val) >= (val)))
    {
      ((max_val) = ((val) - 1));
    } else {
      
    }
  }
}

signed int Card__statGe::CheckStatValid(signed int  stat_val)
{

  return ((stat_val) >= (val));
}



Card__statLe::Card__statLe()
{

  
}

Card__statLe::Card__statLe(Card  * input__item, signed int  input__val) : Card__StatCondVariant((input__item)), val((input__val))
{

  
  
}

Card__statLe::~Card__statLe()
{

  
  {
    
  }
}


void Card__statLe::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1, (vector<double>{((NormalizeCode)((val), 0, 10))})))));
  }
}

string Card__statLe::Detail()
{

  return (" at most " + ((IntToStr)((val))));
}

signed int Card__statLe::IsPlural()
{

  return ((((val) != 1) && ((val) != (-1))));
}

Card__Node  *Card__statLe::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__statLe((card_copy), (val)));
}

void Card__statLe::AdjustGlobalStatRange(signed int  & min_val, signed int  & max_val)
{

  {
    if (((min_val) <= (val)))
    {
      ((min_val) = ((val) + 1));
    } else {
      
    }
  }
}

signed int Card__statLe::CheckStatValid(signed int  stat_val)
{

  return ((stat_val) <= (val));
}



Card__fieldExistenceCond::Card__fieldExistenceCond()
{

  
}

Card__fieldExistenceCond::Card__fieldExistenceCond(Card  * input__item, Card__CharTargetCond  * input__cond) : Card__IndeCond((input__item)), cond((input__cond))
{

  
  
}

Card__fieldExistenceCond::~Card__fieldExistenceCond()
{

  
  {
    if ((cond))
    {
      delete (cond);
    } else {
      
    }
  }
}


void Card__fieldExistenceCond::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
    (((cond)->FillRep)((rep)));
  }
}

string Card__fieldExistenceCond::Detail()
{

  return (("there is " + (((cond)->DetailAlt1)())) + " on the field (not considering this card)");
}

Card__Node  *Card__fieldExistenceCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__fieldExistenceCond((card_copy), ((Card__CharTargetCond *)((((cond)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

signed int Card__fieldExistenceCond::CheckThisValid(Card  * parent_card)
{

  {
    for (signed int i = 0; ((i) < (((((parent_card)->owner)->field).size)())); ((i)++))
    {
      {
        Card  *target = ((((parent_card)->owner)->field)[(i)]);
        if (((((target) && ((target) != (parent_card))) && (!((target)->is_dying))) && (((cond)->CheckCardValid)((target), (parent_card)))))
        {
          return 1;
        } else {
          
        }
      }
    }
    for (signed int i = 0; ((i) < (((((parent_card)->opponent)->field).size)())); ((i)++))
    {
      {
        Card  *target = ((((parent_card)->opponent)->field)[(i)]);
        if (((((target) && ((target) != (parent_card))) && (!((target)->is_dying))) && (((cond)->CheckCardValid)((target), (parent_card)))))
        {
          return 1;
        } else {
          
        }
      }
    }
    return 0;
  }
}



Card__cardExistenceCond::Card__cardExistenceCond()
{

  
}

Card__cardExistenceCond::Card__cardExistenceCond(Card  * input__item, Card__CardTargetCond  * input__cond) : Card__IndeCond((input__item)), cond((input__cond))
{

  
  
}

Card__cardExistenceCond::~Card__cardExistenceCond()
{

  
  {
    if ((cond))
    {
      delete (cond);
    } else {
      
    }
  }
}


void Card__cardExistenceCond::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
    (((cond)->FillRep)((rep)));
  }
}

string Card__cardExistenceCond::Detail()
{

  return (("there is " + (((cond)->DetailAlt1)())) + " (not considering this card)");
}

Card__Node  *Card__cardExistenceCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__cardExistenceCond((card_copy), ((Card__CardTargetCond *)((((cond)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

signed int Card__cardExistenceCond::CheckThisValid(Card  * parent_card)
{

  {
    for (signed int i = 0; ((i) < (((((parent_card)->owner)->hand).size)())); ((i)++))
    {
      {
        Card  *target = ((((parent_card)->owner)->hand)[(i)]);
        if (((((target) && ((target) != (parent_card))) && (!((target)->is_dying))) && (((cond)->CheckCardValid)((target), (parent_card)))))
        {
          return 1;
        } else {
          
        }
      }
    }
    for (signed int i = 0; ((i) < (((((parent_card)->owner)->deck).size)())); ((i)++))
    {
      {
        Card  *target = ((((parent_card)->owner)->deck)[(i)]);
        if (((((target) && ((target) != (parent_card))) && (!((target)->is_dying))) && (((cond)->CheckCardValid)((target), (parent_card)))))
        {
          return 1;
        } else {
          
        }
      }
    }
    return 0;
  }
}



Card__leaderCond::Card__leaderCond()
{

  
}

Card__leaderCond::Card__leaderCond(Card  * input__item, Card__AllegianceCond  * input__alle, Card__AttrCond  * input__attrcond, Card__StatCond  * input__statcond) : Card__IndeCond((input__item)), alle((input__alle)), attrcond((input__attrcond)), statcond((input__statcond))
{

  
  
  
  
}

Card__leaderCond::~Card__leaderCond()
{

  
  {
    if ((alle))
    {
      delete (alle);
    } else {
      
    }
  }
  {
    if ((attrcond))
    {
      delete (attrcond);
    } else {
      
    }
  }
  {
    if ((statcond))
    {
      delete (statcond);
    } else {
      
    }
  }
}




void Card__leaderCond::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(2))));
    (((alle)->FillRep)((rep)));
    (((attrcond)->FillRep)((rep)));
    (((statcond)->FillRep)((rep)));
  }
}

string Card__leaderCond::Detail()
{

  return ((((((alle)->DetailAlt4)()) + (((((alle)->IsPlural)()) ? " are" : " is"))) + (((attrcond)->Detail)())) + (((statcond)->Detail)()));
}

Card__Node  *Card__leaderCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__leaderCond((card_copy), ((Card__AllegianceCond *)((((alle)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__AttrCond *)((((attrcond)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__StatCond *)((((statcond)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

signed int Card__leaderCond::CheckThisValid(Card  * parent_card)
{

  {
    Card  *target_a = (((parent_card)->owner)->leader);
    if ((((alle)->CheckCardValid)((target_a), (parent_card))))
    {
      if ((!(((((attrcond)->CheckCardValid)((target_a), (parent_card))) && (((statcond)->CheckCardValid)((target_a), (parent_card)))))))
      {
        return 0;
      } else {
        
      }
    } else {
      
    }
    Card  *target_b = (((parent_card)->opponent)->leader);
    if ((((alle)->CheckCardValid)((target_b), (parent_card))))
    {
      if ((!(((((attrcond)->CheckCardValid)((target_b), (parent_card))) && (((statcond)->CheckCardValid)((target_b), (parent_card)))))))
      {
        return 0;
      } else {
        
      }
    } else {
      
    }
    return 1;
  }
}



Card__mpCond::Card__mpCond()
{

  
}

Card__mpCond::Card__mpCond(Card  * input__item, Card__AllegianceCond  * input__alle, Card__StatCondVariant  * input__variant) : Card__IndeCond((input__item)), alle((input__alle)), variant((input__variant))
{

  
  
  
}

Card__mpCond::~Card__mpCond()
{

  
  {
    if ((alle))
    {
      delete (alle);
    } else {
      
    }
  }
  {
    if ((variant))
    {
      delete (variant);
    } else {
      
    }
  }
}



void Card__mpCond::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(3))));
    (((alle)->FillRep)((rep)));
    (((variant)->FillRep)((rep)));
  }
}

string Card__mpCond::Detail()
{

  return ((((((alle)->DetailAlt6)()) + (((((alle)->IsThirdPersonSingle)()) ? " has" : " have"))) + (((variant)->Detail)())) + " MP");
}

Card__Node  *Card__mpCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__mpCond((card_copy), ((Card__AllegianceCond *)((((alle)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__StatCondVariant *)((((variant)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

signed int Card__mpCond::CheckThisValid(Card  * parent_card)
{

  {
    Card  *target_a = (((parent_card)->owner)->leader);
    if ((((alle)->CheckCardValid)((target_a), (parent_card))))
    {
      if ((!(((variant)->CheckStatValid)(((((parent_card)->owner)->max_mp) - (((parent_card)->owner)->mp_loss))))))
      {
        return 0;
      } else {
        
      }
    } else {
      
    }
    Card  *target_b = (((parent_card)->opponent)->leader);
    if ((((alle)->CheckCardValid)((target_b), (parent_card))))
    {
      if ((!(((variant)->CheckStatValid)(((((parent_card)->opponent)->max_mp) - (((parent_card)->opponent)->mp_loss))))))
      {
        return 0;
      } else {
        
      }
    } else {
      
    }
    return 1;
  }
}



Card__maxMpCond::Card__maxMpCond()
{

  
}

Card__maxMpCond::Card__maxMpCond(Card  * input__item, Card__AllegianceCond  * input__alle, Card__StatCondVariant  * input__variant) : Card__IndeCond((input__item)), alle((input__alle)), variant((input__variant))
{

  
  
  
}

Card__maxMpCond::~Card__maxMpCond()
{

  
  {
    if ((alle))
    {
      delete (alle);
    } else {
      
    }
  }
  {
    if ((variant))
    {
      delete (variant);
    } else {
      
    }
  }
}



void Card__maxMpCond::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(4))));
    (((alle)->FillRep)((rep)));
    (((variant)->FillRep)((rep)));
  }
}

string Card__maxMpCond::Detail()
{

  return ((((((alle)->DetailAlt6)()) + (((((alle)->IsThirdPersonSingle)()) ? " has" : " have"))) + (((variant)->Detail)())) + " Max MP");
}

Card__Node  *Card__maxMpCond::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__maxMpCond((card_copy), ((Card__AllegianceCond *)((((alle)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__StatCondVariant *)((((variant)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

signed int Card__maxMpCond::CheckThisValid(Card  * parent_card)
{

  {
    Card  *target_a = (((parent_card)->owner)->leader);
    if ((((alle)->CheckCardValid)((target_a), (parent_card))))
    {
      if ((!(((variant)->CheckStatValid)((((parent_card)->owner)->max_mp)))))
      {
        return 0;
      } else {
        
      }
    } else {
      
    }
    Card  *target_b = (((parent_card)->opponent)->leader);
    if ((((alle)->CheckCardValid)((target_b), (parent_card))))
    {
      if ((!(((variant)->CheckStatValid)((((parent_card)->opponent)->max_mp)))))
      {
        return 0;
      } else {
        
      }
    } else {
      
    }
    return 1;
  }
}



Card__damageEff::Card__damageEff()
{

  
}

Card__damageEff::Card__damageEff(Card  * input__item, signed int  input__val, Card__DamageAttributes  * input__attr) : Card__BaseTargetedEff((input__item)), val((input__val)), attr((input__attr))
{

  
  
  
}

Card__damageEff::~Card__damageEff()
{

  
  {
    if ((attr))
    {
      delete (attr);
    } else {
      
    }
  }
  {
    
  }
}



void Card__damageEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0, (vector<double>{((NormalizeCode)((val), 1, 10))})))));
    (((attr)->FillRep)((rep)));
  }
}

string Card__damageEff::Detail()
{

  return (("Deal " + ((IntToStr)((val)))) + " damage to ");
}

Card__Node  *Card__damageEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__damageEff((card_copy), (val), ((Card__DamageAttributes *)((((attr)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

CondConfig Card__damageEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  return (CHAR_COND_FILTER);
}

CondConfig Card__damageEff::GetTargetConfig()
{

  return (FIELD_COND_FILTER);
}

signed int Card__damageEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        (((target)->TakeDamage)((val), (parent_card), (start_of_batch)));
        return ((target)->is_dying);
      }
    } else {
      
    }
    return 0;
  }
}



Card__healEff::Card__healEff()
{

  
}

Card__healEff::Card__healEff(Card  * input__item, signed int  input__val) : Card__BaseTargetedEff((input__item)), val((input__val))
{

  
  
}

Card__healEff::~Card__healEff()
{

  
  {
    
  }
}


void Card__healEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1, (vector<double>{((NormalizeCode)((val), 1, 10))})))));
  }
}

string Card__healEff::Detail()
{

  return (("Restore " + ((IntToStr)((val)))) + " health to ");
}

Card__Node  *Card__healEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__healEff((card_copy), (val)));
}

CondConfig Card__healEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  return (CHAR_COND_FILTER);
}

CondConfig Card__healEff::GetTargetConfig()
{

  return (FIELD_COND_FILTER);
}

signed int Card__healEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      (((target)->RestoreHp)((val)));
    } else {
      
    }
    return 0;
  }
}



Card__resAtkTimesEff::Card__resAtkTimesEff()
{

  
}

Card__resAtkTimesEff::Card__resAtkTimesEff(Card  * input__item, signed int  input__val) : Card__BaseTargetedEff((input__item)), val((input__val))
{

  
  
}

Card__resAtkTimesEff::~Card__resAtkTimesEff()
{

  
  {
    
  }
}


void Card__resAtkTimesEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(2, (vector<double>{((NormalizeCode)((val), 1, 5))})))));
  }
}

string Card__resAtkTimesEff::Detail()
{

  return (((("Restore " + ((IntToStr)((val)))) + " attack ") + ((((val) == 1) ? "time" : "times"))) + " to ");
}

Card__Node  *Card__resAtkTimesEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__resAtkTimesEff((card_copy), (val)));
}

CondConfig Card__resAtkTimesEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  return (CHAR_COND_FILTER);
}

CondConfig Card__resAtkTimesEff::GetTargetConfig()
{

  return (FIELD_COND_FILTER);
}

signed int Card__resAtkTimesEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      (((target)->RestoreAtkTimes)((val)));
    } else {
      
    }
    return 0;
  }
}



Card__costModEff::Card__costModEff()
{

  
}

Card__costModEff::Card__costModEff(Card  * input__item, signed int  input__val) : Card__BaseTargetedEff((input__item)), inc_prob(), val((input__val))
{

  
  
}

Card__costModEff::~Card__costModEff()
{

  
  {
    
  }
}



void Card__costModEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(3, (vector<double>{((NormalizeCode)((val), (-10), 10))})))));
  }
}

string Card__costModEff::Detail()
{

  return (("Give " + ((((((val) < 0) ? "" : "+")) + ((IntToStr)((val)))))) + " to the cost of ");
}

Card__Node  *Card__costModEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__costModEff((card_copy), (val)));
}

CondConfig Card__costModEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  return ((GetCostConfig)((TARGET_TYPE_ANY), ((((val) < 0) ? (-(val)) : 0)), 10));
}

CondConfig Card__costModEff::GetTargetConfig()
{

  return ((GetCostConfig)((HAND_OR_DECK_COND_FILTER), ((((val) < 0) ? (-(val)) : 0)), 10));
}

signed int Card__costModEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      (((target)->ModifyCost)((val)));
    } else {
      
    }
    return 0;
  }
}



Card__atkHpModEff::Card__atkHpModEff()
{

  
}

Card__atkHpModEff::Card__atkHpModEff(Card  * input__item, signed int  input__atkmod, signed int  input__hpmod) : Card__BaseTargetedEff((input__item)), atkmod((input__atkmod)), hpmod((input__hpmod))
{

  
  
  
}

Card__atkHpModEff::~Card__atkHpModEff()
{

  
  {
    
  }
  {
    
  }
}



void Card__atkHpModEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(4, (vector<double>{((NormalizeCode)((atkmod), (-5), 5)), ((NormalizeCode)((hpmod), 0, 10))})))));
  }
}

string Card__atkHpModEff::Detail()
{

  return (((("Give " + ((((((atkmod) < 0) ? "" : "+")) + ((IntToStr)((atkmod)))))) + "/") + ((((((hpmod) < 0) ? "" : "+")) + ((IntToStr)((hpmod)))))) + " to ");
}

Card__Node  *Card__atkHpModEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__atkHpModEff((card_copy), (atkmod), (hpmod)));
}

CondConfig Card__atkHpModEff::GetTargetConfig()
{

  return ((GetAtkConfig)((CHAR_COND_FILTER), ((((atkmod) < 0) ? (-(atkmod)) : 0)), 10));
}

signed int Card__atkHpModEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      (((target)->ModifyAtkHp)((atkmod), (hpmod)));
    } else {
      
    }
    return 0;
  }
}



Card__atkTimesModEff::Card__atkTimesModEff()
{

  
}

Card__atkTimesModEff::Card__atkTimesModEff(Card  * input__item, signed int  input__val) : Card__BaseTargetedEff((input__item)), dec_prob(), val((input__val))
{

  
  
}

Card__atkTimesModEff::~Card__atkTimesModEff()
{

  
  {
    
  }
}



void Card__atkTimesModEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(5, (vector<double>{((NormalizeCode)((val), (-3), 3))})))));
  }
}

string Card__atkTimesModEff::Detail()
{

  return (("Give " + ((((((val) < 0) ? "" : "+")) + ((IntToStr)((val)))))) + " to the attack times of ");
}

Card__Node  *Card__atkTimesModEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__atkTimesModEff((card_copy), (val)));
}

CondConfig Card__atkTimesModEff::GetTargetConfig()
{

  return ((GetAtkTimesConfig)((CHAR_COND_FILTER), ((((val) < 0) ? (-(val)) : 0)), 5));
}

signed int Card__atkTimesModEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      (((target)->ModifyAtkTimes)((val)));
    } else {
      
    }
    return 0;
  }
}



Card__destroyEff::Card__destroyEff()
{

  
}

Card__destroyEff::Card__destroyEff(Card  * input__item) : Card__BaseTargetedEff((input__item))
{

  
}

Card__destroyEff::~Card__destroyEff()
{

  
}

void Card__destroyEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(6))));
  }
}

string Card__destroyEff::Detail()
{

  return "Destroy ";
}

Card__Node  *Card__destroyEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__destroyEff((card_copy)));
}

CondConfig Card__destroyEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  return (MINION_COND_FILTER);
}

CondConfig Card__destroyEff::GetTargetConfig()
{

  return (((TARGET_POS_FIELD) | (TARGET_IS_MINION)) | (TARGET_ANY_ALLE_MINION_ATTR_TYPE));
}

signed int Card__destroyEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        ((((target)->owner)->FlagDestroy)((target), (start_of_batch)));
        return 1;
      }
    } else {
      
    }
    return 0;
  }
}



Card__discardEff::Card__discardEff()
{

  
}

Card__discardEff::Card__discardEff(Card  * input__item) : Card__BaseTargetedEff((input__item))
{

  
}

Card__discardEff::~Card__discardEff()
{

  
}

void Card__discardEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(7))));
  }
}

string Card__discardEff::Detail()
{

  return "Discard ";
}

Card__Node  *Card__discardEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__discardEff((card_copy)));
}

CondConfig Card__discardEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    unsigned int tmp_filter;
    if ((!(((self_config) & (TARGET_POS_HAND_OR_DECK)))))
    {
      ((tmp_filter) = (NOT_LEADER_COND_FILTER));
    } else {
      if ((!(((self_config) & (TARGET_NOT_LEADER)))))
      {
        ((tmp_filter) = (HAND_OR_DECK_COND_FILTER));
      } else {
        ((tmp_filter) = ((((RandomRoll)(0.9)) ? (NOT_LEADER_COND_FILTER) : (HAND_OR_DECK_COND_FILTER))));
      }
    }
    return (tmp_filter);
  }
}

CondConfig Card__discardEff::GetSelfConfig(const CondConfig  & self_config)
{

  {
    unsigned int tmp_filter;
    if ((!(((self_config) & (TARGET_POS_HAND_OR_DECK)))))
    {
      ((tmp_filter) = (NOT_LEADER_COND_FILTER));
    } else {
      if ((!(((self_config) & (TARGET_NOT_LEADER)))))
      {
        ((tmp_filter) = (HAND_OR_DECK_COND_FILTER));
      } else {
        ((tmp_filter) = ((((RandomRoll)(0.9)) ? (NOT_LEADER_COND_FILTER) : (HAND_OR_DECK_COND_FILTER))));
      }
    }
    return (tmp_filter);
  }
}

CondConfig Card__discardEff::GetTargetConfig()
{

  return ((((RandomRoll)(0.9)) ? (NOT_LEADER_COND_FILTER) : (HAND_OR_DECK_COND_FILTER)));
}

signed int Card__discardEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        switch (((target)->card_pos))
        {
          {
            case (CARD_POS_AT_FIELD):
              ((((target)->owner)->FlagFieldDiscard)((target), (start_of_batch)));
            
            break;
            case (CARD_POS_AT_HAND):
              ((((target)->owner)->FlagHandDiscard)((target), (start_of_batch)));
            
            break;
            case (CARD_POS_AT_DECK):
              ((((target)->owner)->FlagDeckDiscard)((target), (start_of_batch)));
            
            break;
          }
        }
        return 1;
      }
    } else {
      
    }
    return 0;
  }
}



Card__moveEff::Card__moveEff()
{

  
}

Card__moveEff::Card__moveEff(Card  * input__item, Card__Destination  * input__dest) : Card__BaseTargetedEff((input__item)), dest((input__dest))
{

  
  
}

Card__moveEff::~Card__moveEff()
{

  
  {
    if ((dest))
    {
      delete (dest);
    } else {
      
    }
  }
}


void Card__moveEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(8))));
    (((dest)->FillRep)((rep)));
  }
}

string Card__moveEff::Detail()
{

  return (((dest)->Detail)());
}

string Card__moveEff::Suffix()
{

  return (((dest)->Suffix)());
}

Card__Node  *Card__moveEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__moveEff((card_copy), ((Card__Destination *)((((dest)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

CondConfig Card__moveEff::GetPlayTargetConfig()
{

  {
    CondConfig dest_config = (((dest)->GetTargetConfig)());
    unsigned int type_filter = ((dest_config) | (~(TARGET_ANY_CARD_TYPE)));
    unsigned int pos_complement_filter = (~(((dest_config) & (TARGET_ANY_POS_TYPE))));
    unsigned int alle_flag = ((dest_config) & (TARGET_ANY_ALLE_TYPE));
    unsigned int alle_complement_filter = (~(alle_flag));
    unsigned int dest_constraint_filter = (type_filter);
    if ((!(alle_flag)))
    {
      ((dest_constraint_filter) &= (pos_complement_filter));
    } else {
      ((dest_constraint_filter) &= ((((RandomRoll)(0.9)) ? (pos_complement_filter) : (alle_complement_filter))));
    }
    if ((!(((dest_constraint_filter) & (TARGET_POS_HAND)))))
    {
      ((dest_constraint_filter) &= (MINION_COND_FILTER));
    } else {
      
    }
    unsigned int leader_constraint_filter;
    if (((!(((dest_constraint_filter) & (TARGET_IS_ALLY)))) || (!(((dest_constraint_filter) & (TARGET_POS_HAND))))))
    {
      ((leader_constraint_filter) = (NOT_LEADER_COND_FILTER));
    } else {
      ((leader_constraint_filter) = ((((RandomRoll)(0.9)) ? (NOT_LEADER_COND_FILTER) : (HAND_OR_DECK_COND_FILTER))));
    }
    return ((dest_constraint_filter) & (leader_constraint_filter));
  }
}

CondConfig Card__moveEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig dest_config = (((dest)->GetTargetConfig)());
    unsigned int dest_constraint_filter = ((dest_config) | (~(TARGET_ANY_CARD_TYPE)));
    CondConfig self_config_copy = (self_config);
    ((self_config_copy) &= (dest_constraint_filter));
    unsigned int leader_constraint_filter;
    if ((!(((self_config_copy) & (TARGET_POS_HAND_OR_DECK)))))
    {
      ((leader_constraint_filter) = (NOT_LEADER_COND_FILTER));
    } else {
      if ((!(((self_config_copy) & (TARGET_NOT_LEADER)))))
      {
        ((leader_constraint_filter) = (HAND_OR_DECK_COND_FILTER));
      } else {
        ((leader_constraint_filter) = ((((RandomRoll)(0.9)) ? (NOT_LEADER_COND_FILTER) : (HAND_OR_DECK_COND_FILTER))));
      }
    }
    return ((dest_constraint_filter) & (leader_constraint_filter));
  }
}

CondConfig Card__moveEff::GetSelfConfig(const CondConfig  & self_config)
{

  {
    CondConfig dest_config = (((dest)->GetTargetConfig)());
    unsigned int type_filter = ((dest_config) | (~(TARGET_ANY_CARD_TYPE)));
    unsigned int pos_complement_filter = (~(((dest_config) & (TARGET_ANY_POS_TYPE))));
    unsigned int alle_flag = ((dest_config) & (TARGET_ANY_ALLE_TYPE));
    unsigned int alle_complement_filter = (~(alle_flag));
    unsigned int dest_constraint_filter = ((type_filter) & (ALLY_COND_FILTER));
    if (((!(alle_flag)) || ((alle_flag) == (TARGET_IS_ALLY))))
    {
      ((dest_constraint_filter) &= (pos_complement_filter));
    } else {
      
    }
    CondConfig self_config_copy = (self_config);
    ((self_config_copy) &= (dest_constraint_filter));
    unsigned int leader_constraint_filter;
    if ((!(((self_config_copy) & (TARGET_POS_HAND_OR_DECK)))))
    {
      ((leader_constraint_filter) = (NOT_LEADER_COND_FILTER));
    } else {
      if ((!(((self_config_copy) & (TARGET_NOT_LEADER)))))
      {
        ((leader_constraint_filter) = (HAND_OR_DECK_COND_FILTER));
      } else {
        ((leader_constraint_filter) = ((((RandomRoll)(0.9)) ? (NOT_LEADER_COND_FILTER) : (HAND_OR_DECK_COND_FILTER))));
      }
    }
    return ((dest_constraint_filter) & (leader_constraint_filter));
  }
}

CondConfig Card__moveEff::GetTargetConfig()
{

  {
    CondConfig dest_config = (((dest)->GetTargetConfig)());
    unsigned int type_filter = ((dest_config) | (~(TARGET_ANY_CARD_TYPE)));
    unsigned int pos_complement_filter = (~(((dest_config) & (TARGET_ANY_POS_TYPE))));
    unsigned int alle_flag = ((dest_config) & (TARGET_ANY_ALLE_TYPE));
    unsigned int alle_complement_filter = (~(alle_flag));
    unsigned int dest_constraint_filter = (type_filter);
    if ((!(alle_flag)))
    {
      ((dest_constraint_filter) &= (pos_complement_filter));
    } else {
      ((dest_constraint_filter) &= ((((RandomRoll)(0.9)) ? (pos_complement_filter) : (alle_complement_filter))));
    }
    unsigned int leader_constraint_filter = ((((RandomRoll)(0.9)) ? (NOT_LEADER_COND_FILTER) : (HAND_OR_DECK_COND_FILTER)));
    return ((dest_constraint_filter) & (leader_constraint_filter));
  }
}

signed int Card__moveEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->ExtractTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        (((dest)->SubTargetedAction)((target), (parent_card), (start_of_batch)));
        return 1;
      }
    } else {
      
    }
    return 0;
  }
}



Card__copyEff::Card__copyEff()
{

  
}

Card__copyEff::Card__copyEff(Card  * input__item, Card__Destination  * input__dest) : Card__BaseTargetedEff((input__item)), dest((input__dest))
{

  
  
}

Card__copyEff::~Card__copyEff()
{

  
  {
    if ((dest))
    {
      delete (dest);
    } else {
      
    }
  }
}


void Card__copyEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(9))));
    (((dest)->FillRep)((rep)));
  }
}

string Card__copyEff::Detail()
{

  return ((((dest)->Detail)()) + "a copy of ");
}

string Card__copyEff::DetailAlt1()
{

  return ((((dest)->Detail)()) + "copies of ");
}

string Card__copyEff::Suffix()
{

  return (((dest)->Suffix)());
}

Card__Node  *Card__copyEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__copyEff((card_copy), ((Card__Destination *)((((dest)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

CondConfig Card__copyEff::GetPlayTargetConfig()
{

  {
    CondConfig dest_config = (((dest)->GetTargetConfig)());
    unsigned int dest_constraint_filter = ((dest_config) | (~(TARGET_ANY_CARD_TYPE)));
    unsigned int leader_constraint_filter = ((((RandomRoll)(0.9)) ? (NOT_LEADER_COND_FILTER) : (HAND_OR_DECK_COND_FILTER)));
    return ((dest_constraint_filter) & (leader_constraint_filter));
  }
}

CondConfig Card__copyEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig dest_config = (((dest)->GetTargetConfig)());
    unsigned int dest_constraint_filter = ((dest_config) | (~(TARGET_ANY_CARD_TYPE)));
    CondConfig self_config_copy = (self_config);
    ((self_config_copy) &= (dest_constraint_filter));
    unsigned int leader_constraint_filter;
    if ((!(((self_config_copy) & (TARGET_POS_HAND_OR_DECK)))))
    {
      ((leader_constraint_filter) = (NOT_LEADER_COND_FILTER));
    } else {
      if ((!(((self_config_copy) & (TARGET_NOT_LEADER)))))
      {
        ((leader_constraint_filter) = (HAND_OR_DECK_COND_FILTER));
      } else {
        ((leader_constraint_filter) = ((((RandomRoll)(0.9)) ? (NOT_LEADER_COND_FILTER) : (HAND_OR_DECK_COND_FILTER))));
      }
    }
    return ((dest_constraint_filter) & (leader_constraint_filter));
  }
}

CondConfig Card__copyEff::GetSelfConfig(const CondConfig  & self_config)
{

  {
    CondConfig dest_config = (((dest)->GetTargetConfig)());
    unsigned int dest_constraint_filter = ((dest_config) | (~(TARGET_ANY_CARD_TYPE)));
    if (((dest_config) & (TARGET_POS_DECK)))
    {
      ((dest_constraint_filter) &= (NOT_DECK_COND_FILTER));
    } else {
      
    }
    CondConfig self_config_copy = (self_config);
    ((self_config_copy) &= (dest_constraint_filter));
    unsigned int leader_constraint_filter;
    if ((!(((self_config_copy) & (TARGET_POS_HAND_OR_DECK)))))
    {
      ((leader_constraint_filter) = (NOT_LEADER_COND_FILTER));
    } else {
      if ((!(((self_config_copy) & (TARGET_NOT_LEADER)))))
      {
        ((leader_constraint_filter) = (HAND_OR_DECK_COND_FILTER));
      } else {
        ((leader_constraint_filter) = ((((RandomRoll)(0.9)) ? (NOT_LEADER_COND_FILTER) : (HAND_OR_DECK_COND_FILTER))));
      }
    }
    return ((dest_constraint_filter) & (leader_constraint_filter));
  }
}

CondConfig Card__copyEff::GetTargetConfig()
{

  {
    CondConfig dest_config = (((dest)->GetTargetConfig)());
    unsigned int dest_constraint_filter = ((dest_config) | (~(TARGET_ANY_CARD_TYPE)));
    if (((dest_config) & (TARGET_POS_DECK)))
    {
      ((dest_constraint_filter) &= (NOT_DECK_COND_FILTER));
    } else {
      
    }
    unsigned int leader_constraint_filter = ((((RandomRoll)(0.9)) ? (NOT_LEADER_COND_FILTER) : (HAND_OR_DECK_COND_FILTER)));
    return ((dest_constraint_filter) & (leader_constraint_filter));
  }
}

signed int Card__copyEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        (((dest)->SubTargetedAction)((((target)->CreateCopy)()), (parent_card), (start_of_batch)));
        return 1;
      }
    } else {
      
    }
    return 0;
  }
}



Card__transformEff::Card__transformEff()
{

  
}

Card__transformEff::Card__transformEff(Card  * input__item, Card__NewCardVariant  * input__variant) : Card__BaseTargetedEff((input__item)), variant((input__variant))
{

  
  
}

Card__transformEff::~Card__transformEff()
{

  
  {
    if ((variant))
    {
      delete (variant);
    } else {
      
    }
  }
}


void Card__transformEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(10))));
    (((variant)->FillRep)((rep)));
  }
}

string Card__transformEff::Detail()
{

  return "Transform ";
}

string Card__transformEff::Suffix()
{

  return (" to " + (((variant)->Detail)()));
}

string Card__transformEff::SuffixAlt1()
{

  return (" to " + (((variant)->DetailAlt1)()));
}

string Card__transformEff::Postfix()
{

  return (((variant)->Postfix)());
}

string Card__transformEff::PostfixIndent(signed int  indent_size)
{

  return (((variant)->PostfixIndent)((indent_size)));
}

Card__Node  *Card__transformEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__transformEff((card_copy), ((Card__NewCardVariant *)((((variant)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

CondConfig Card__transformEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig variant_config = (((variant)->GetTargetConfig)());
    unsigned int variant_constraint_filter = ((variant_config) | (~(TARGET_ANY_CARD_TYPE)));
    CondConfig self_config_copy = (self_config);
    ((self_config_copy) &= (variant_constraint_filter));
    unsigned int leader_constraint_filter;
    if ((!(((self_config_copy) & (TARGET_POS_HAND_OR_DECK)))))
    {
      ((leader_constraint_filter) = (NOT_LEADER_COND_FILTER));
    } else {
      if ((!(((self_config_copy) & (TARGET_NOT_LEADER)))))
      {
        ((leader_constraint_filter) = (HAND_OR_DECK_COND_FILTER));
      } else {
        ((leader_constraint_filter) = ((((RandomRoll)(0.9)) ? (NOT_LEADER_COND_FILTER) : (HAND_OR_DECK_COND_FILTER))));
      }
    }
    return ((variant_constraint_filter) & (leader_constraint_filter));
  }
}

CondConfig Card__transformEff::GetTargetConfig()
{

  return (((variant)->GetTargetConfig)());
}

signed int Card__transformEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        Card  *card = (((variant)->GetRelatedCard)((parent_card)));
        ((((target)->owner)->FlagCardTransform)((target), (start_of_batch), (card)));
        return 1;
      }
    } else {
      
    }
    return 0;
  }
}



Card__changeToBeastEff::Card__changeToBeastEff()
{

  
}

Card__changeToBeastEff::Card__changeToBeastEff(Card  * input__item) : Card__BaseTargetedEff((input__item))
{

  
}

Card__changeToBeastEff::~Card__changeToBeastEff()
{

  
}

void Card__changeToBeastEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(11))));
  }
}

string Card__changeToBeastEff::Detail()
{

  return "Change the minion type of ";
}

string Card__changeToBeastEff::DetailAlt1()
{

  return "Change the minion types of ";
}

string Card__changeToBeastEff::Suffix()
{

  return " to Beast";
}

string Card__changeToBeastEff::SuffixAlt1()
{

  return " to Beasts";
}

Card__Node  *Card__changeToBeastEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__changeToBeastEff((card_copy)));
}

CondConfig Card__changeToBeastEff::GetTargetConfig()
{

  return (NOT_BEAST_COND_FILTER);
}

signed int Card__changeToBeastEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        (((target)->minion_type) = (BEAST_MINION));
      }
    } else {
      
    }
    return 0;
  }
}



Card__changeToDragonEff::Card__changeToDragonEff()
{

  
}

Card__changeToDragonEff::Card__changeToDragonEff(Card  * input__item) : Card__BaseTargetedEff((input__item))
{

  
}

Card__changeToDragonEff::~Card__changeToDragonEff()
{

  
}

void Card__changeToDragonEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(12))));
  }
}

string Card__changeToDragonEff::Detail()
{

  return "Change the minion type of ";
}

string Card__changeToDragonEff::DetailAlt1()
{

  return "Change the minion types of ";
}

string Card__changeToDragonEff::Suffix()
{

  return " to Dragon";
}

string Card__changeToDragonEff::SuffixAlt1()
{

  return " to Dragons";
}

Card__Node  *Card__changeToDragonEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__changeToDragonEff((card_copy)));
}

CondConfig Card__changeToDragonEff::GetTargetConfig()
{

  return (NOT_DRAGON_COND_FILTER);
}

signed int Card__changeToDragonEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        (((target)->minion_type) = (DRAGON_MINION));
      }
    } else {
      
    }
    return 0;
  }
}



Card__changeToDemonEff::Card__changeToDemonEff()
{

  
}

Card__changeToDemonEff::Card__changeToDemonEff(Card  * input__item) : Card__BaseTargetedEff((input__item))
{

  
}

Card__changeToDemonEff::~Card__changeToDemonEff()
{

  
}

void Card__changeToDemonEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(13))));
  }
}

string Card__changeToDemonEff::Detail()
{

  return "Change the minion type of ";
}

string Card__changeToDemonEff::DetailAlt1()
{

  return "Change the minion types of ";
}

string Card__changeToDemonEff::Suffix()
{

  return " to Demon";
}

string Card__changeToDemonEff::SuffixAlt1()
{

  return " to Demons";
}

Card__Node  *Card__changeToDemonEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__changeToDemonEff((card_copy)));
}

CondConfig Card__changeToDemonEff::GetTargetConfig()
{

  return (NOT_DEMON_COND_FILTER);
}

signed int Card__changeToDemonEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        (((target)->minion_type) = (DEMON_MINION));
      }
    } else {
      
    }
    return 0;
  }
}



Card__giveChargeEff::Card__giveChargeEff()
{

  
}

Card__giveChargeEff::Card__giveChargeEff(Card  * input__item) : Card__BaseTargetedEff((input__item))
{

  
}

Card__giveChargeEff::~Card__giveChargeEff()
{

  
}

void Card__giveChargeEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(14))));
  }
}

string Card__giveChargeEff::Detail()
{

  return "Grant Charge to ";
}

Card__Node  *Card__giveChargeEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__giveChargeEff((card_copy)));
}

CondConfig Card__giveChargeEff::GetTargetConfig()
{

  return (NOT_CHARGE_COND_FILTER);
}

signed int Card__giveChargeEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        (((target)->is_charge) = 1);
      }
    } else {
      
    }
    return 0;
  }
}



Card__giveTauntEff::Card__giveTauntEff()
{

  
}

Card__giveTauntEff::Card__giveTauntEff(Card  * input__item) : Card__BaseTargetedEff((input__item))
{

  
}

Card__giveTauntEff::~Card__giveTauntEff()
{

  
}

void Card__giveTauntEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(15))));
  }
}

string Card__giveTauntEff::Detail()
{

  return "Grant Taunt to ";
}

Card__Node  *Card__giveTauntEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__giveTauntEff((card_copy)));
}

CondConfig Card__giveTauntEff::GetTargetConfig()
{

  return (NOT_TAUNT_COND_FILTER);
}

signed int Card__giveTauntEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        (((target)->is_taunt) = 1);
      }
    } else {
      
    }
    return 0;
  }
}



Card__giveStealthEff::Card__giveStealthEff()
{

  
}

Card__giveStealthEff::Card__giveStealthEff(Card  * input__item) : Card__BaseTargetedEff((input__item))
{

  
}

Card__giveStealthEff::~Card__giveStealthEff()
{

  
}

void Card__giveStealthEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(16))));
  }
}

string Card__giveStealthEff::Detail()
{

  return "Grant Stealth to ";
}

Card__Node  *Card__giveStealthEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__giveStealthEff((card_copy)));
}

CondConfig Card__giveStealthEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  return ((((effect_timing) == (EFFECT_TIMING_PLAY)) ? (NOT_STEALTH_COND_FILTER) : (MINION_COND_FILTER)));
}

CondConfig Card__giveStealthEff::GetSelfConfig(const CondConfig  & self_config)
{

  return ((((TARGET_POS_FIELD) | (TARGET_IS_MINION)) | (TARGET_NOT_STEALTH)) | (TARGET_ANY_ALLE_MINION_TYPE));
}

CondConfig Card__giveStealthEff::GetTargetConfig()
{

  return (NOT_STEALTH_COND_FILTER);
}

signed int Card__giveStealthEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        (((target)->is_stealth) = 1);
      }
    } else {
      
    }
    return 0;
  }
}



Card__giveUntargetableEff::Card__giveUntargetableEff()
{

  
}

Card__giveUntargetableEff::Card__giveUntargetableEff(Card  * input__item) : Card__BaseTargetedEff((input__item))
{

  
}

Card__giveUntargetableEff::~Card__giveUntargetableEff()
{

  
}

void Card__giveUntargetableEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(17))));
  }
}

string Card__giveUntargetableEff::Detail()
{

  return "Grant Untargetability to ";
}

Card__Node  *Card__giveUntargetableEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__giveUntargetableEff((card_copy)));
}

CondConfig Card__giveUntargetableEff::GetTargetConfig()
{

  return (NOT_UNTARGETABLE_COND_FILTER);
}

signed int Card__giveUntargetableEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        (((target)->is_untargetable) = 1);
      }
    } else {
      
    }
    return 0;
  }
}



Card__giveShieldEff::Card__giveShieldEff()
{

  
}

Card__giveShieldEff::Card__giveShieldEff(Card  * input__item) : Card__BaseTargetedEff((input__item))
{

  
}

Card__giveShieldEff::~Card__giveShieldEff()
{

  
}

void Card__giveShieldEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(18))));
  }
}

string Card__giveShieldEff::Detail()
{

  return "Grant Shield to ";
}

Card__Node  *Card__giveShieldEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__giveShieldEff((card_copy)));
}

CondConfig Card__giveShieldEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  return ((((effect_timing) == (EFFECT_TIMING_PLAY)) ? (NOT_SHIELDED_COND_FILTER) : (CHAR_COND_FILTER)));
}

CondConfig Card__giveShieldEff::GetSelfConfig(const CondConfig  & self_config)
{

  return ((((TARGET_POS_FIELD) | (TARGET_ANY_CHAR)) | (TARGET_NOT_SHIELDED)) | (TARGET_ANY_ALLE_MINION_TYPE));
}

CondConfig Card__giveShieldEff::GetTargetConfig()
{

  return (NOT_SHIELDED_COND_FILTER);
}

signed int Card__giveShieldEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        (((target)->is_shielded) = 1);
      }
    } else {
      
    }
    return 0;
  }
}



Card__givePoisonousEff::Card__givePoisonousEff()
{

  
}

Card__givePoisonousEff::Card__givePoisonousEff(Card  * input__item) : Card__BaseTargetedEff((input__item))
{

  
}

Card__givePoisonousEff::~Card__givePoisonousEff()
{

  
}

void Card__givePoisonousEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(19))));
  }
}

string Card__givePoisonousEff::Detail()
{

  return "Grant Poisonous to ";
}

Card__Node  *Card__givePoisonousEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__givePoisonousEff((card_copy)));
}

CondConfig Card__givePoisonousEff::GetTargetConfig()
{

  return (NOT_POISONOUS_COND_FILTER);
}

signed int Card__givePoisonousEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        (((target)->is_poisonous) = 1);
      }
    } else {
      
    }
    return 0;
  }
}



Card__giveLifestealEff::Card__giveLifestealEff()
{

  
}

Card__giveLifestealEff::Card__giveLifestealEff(Card  * input__item) : Card__BaseTargetedEff((input__item))
{

  
}

Card__giveLifestealEff::~Card__giveLifestealEff()
{

  
}

void Card__giveLifestealEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(20))));
  }
}

string Card__giveLifestealEff::Detail()
{

  return "Grant Lifesteal to ";
}

Card__Node  *Card__giveLifestealEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__giveLifestealEff((card_copy)));
}

CondConfig Card__giveLifestealEff::GetTargetConfig()
{

  return (NOT_LIFESTEAL_COND_FILTER);
}

signed int Card__giveLifestealEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        (((target)->is_lifesteal) = 1);
      }
    } else {
      
    }
    return 0;
  }
}



Card__rmAttributesEff::Card__rmAttributesEff()
{

  
}

Card__rmAttributesEff::Card__rmAttributesEff(Card  * input__item) : Card__BaseTargetedEff((input__item))
{

  
}

Card__rmAttributesEff::~Card__rmAttributesEff()
{

  
}

void Card__rmAttributesEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(21))));
  }
}

string Card__rmAttributesEff::Detail()
{

  return "Remove all attribute keywords from ";
}

Card__Node  *Card__rmAttributesEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__rmAttributesEff((card_copy)));
}

signed int Card__rmAttributesEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        (((target)->is_charge) = 0);
        (((target)->is_taunt) = 0);
        (((target)->is_stealth) = 0);
        (((target)->is_untargetable) = 0);
        (((target)->is_shielded) = 0);
        (((target)->is_poisonous) = 0);
        (((target)->is_lifesteal) = 0);
      }
    } else {
      
    }
    return 0;
  }
}



Card__setOverheatEff::Card__setOverheatEff()
{

  
}

Card__setOverheatEff::Card__setOverheatEff(Card  * input__item) : Card__BaseTargetedEff((input__item))
{

  
}

Card__setOverheatEff::~Card__setOverheatEff()
{

  
}

void Card__setOverheatEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(22))));
  }
}

string Card__setOverheatEff::Detail()
{

  return "Set every effect on ";
}

string Card__setOverheatEff::Suffix()
{

  return " to overheat";
}

Card__Node  *Card__setOverheatEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__setOverheatEff((card_copy)));
}

signed int Card__setOverheatEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        (((target)->SetAllOverheatCounts)(10));
      }
    } else {
      
    }
    return 0;
  }
}



Card__decOhThresholdEff::Card__decOhThresholdEff()
{

  
}

Card__decOhThresholdEff::Card__decOhThresholdEff(Card  * input__item, signed int  input__val) : Card__BaseTargetedEff((input__item)), val((input__val))
{

  
  
}

Card__decOhThresholdEff::~Card__decOhThresholdEff()
{

  
  {
    
  }
}


void Card__decOhThresholdEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(23, (vector<double>{((NormalizeCode)((val), 1, 5))})))));
  }
}

string Card__decOhThresholdEff::Detail()
{

  return "Reduce the overheat threshold for every effect on ";
}

string Card__decOhThresholdEff::Suffix()
{

  return ((" by " + ((IntToStr)((val)))) + " (applied repeatedly for duplicated effects)");
}

Card__Node  *Card__decOhThresholdEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__decOhThresholdEff((card_copy), (val)));
}

signed int Card__decOhThresholdEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        (((target)->ModAllOverheatThresholds)((-(val))));
      }
    } else {
      
    }
    return 0;
  }
}



Card__resetStateEff::Card__resetStateEff()
{

  
}

Card__resetStateEff::Card__resetStateEff(Card  * input__item) : Card__BaseTargetedEff((input__item))
{

  
}

Card__resetStateEff::~Card__resetStateEff()
{

  
}

void Card__resetStateEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(24))));
  }
}

string Card__resetStateEff::Detail()
{

  return "Reset ";
}

string Card__resetStateEff::Suffix()
{

  return " to its original state (not directly applying to the sleeping state, consumed attack times, and overheat counts)";
}

string Card__resetStateEff::SuffixAlt1()
{

  return " to their original states (not directly applying to sleeping states, consumed attack times, and overheat counts)";
}

Card__Node  *Card__resetStateEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__resetStateEff((card_copy)));
}

signed int Card__resetStateEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        (((target)->mana) = ((target)->orig_mana));
        (((target)->atk) = ((target)->orig_atk));
        (((target)->max_hp) = ((target)->orig_hp));
        (((target)->hp_loss) = 0);
        (((target)->max_n_atks) = ((target)->orig_n_atks));
        (((target)->minion_type) = ((target)->orig_minion_type));
        (((target)->is_charge) = ((target)->orig_is_charge));
        (((target)->is_taunt) = ((target)->orig_is_taunt));
        (((target)->is_stealth) = ((target)->orig_is_stealth));
        (((target)->is_untargetable) = ((target)->orig_is_untargetable));
        (((target)->is_shielded) = ((target)->orig_is_shielded));
        (((target)->is_poisonous) = ((target)->orig_is_poisonous));
        (((target)->is_lifesteal) = ((target)->orig_is_lifesteal));
        ((((target)->owner)->FlagCardReset)((target), (start_of_batch)));
        (((target)->SetAllOverheatThresholds)(10));
        return 1;
      }
    } else {
      
    }
    return 0;
  }
}



Card__giveEffectsEff::Card__giveEffectsEff()
{

  
}

Card__giveEffectsEff::Card__giveEffectsEff(Card  * input__item, Card__SpecialEffects  * input__effects) : Card__BaseTargetedEff((input__item)), effects((input__effects))
{

  
  
}

Card__giveEffectsEff::~Card__giveEffectsEff()
{

  {
    (((effects)->num_refs)--);
    if ((((effects)->num_refs) <= 0))
    {
      delete (effects);
    } else {
      
    }
  }
  {
    
  }
}


void Card__giveEffectsEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(25))));
    (((effects)->FillRep)((rep)));
  }
}

string Card__giveEffectsEff::Detail()
{

  return "Give the following effect(s) to ";
}

string Card__giveEffectsEff::Postfix()
{

  return ("\n" + (((effects)->DetailIndent)(4)));
}

string Card__giveEffectsEff::PostfixIndent(signed int  indent_size)
{

  return ("\n" + (((effects)->DetailIndent)(((indent_size) + 4))));
}

Card__Node  *Card__giveEffectsEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__giveEffectsEff((card_copy), ((Card__SpecialEffects *)((((effects)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

CondConfig Card__giveEffectsEff::GetTargetConfig()
{

  return (((effects)->GetGlobalSelfConfig)(((GetDefaultInitConfig)()), (EFFECT_TIMING_DEFAULT)));
}

signed int Card__giveEffectsEff::TargetedAction(signed int  z, Card  * parent_card, signed int  start_of_batch)
{

  {
    Card  *target = ((((parent_card)->owner)->GetTargetCard)((z)));
    if (((target) && (!((target)->is_dying))))
    {
      {
        (((target)->AddExtraEffects)((effects)));
      }
    } else {
      
    }
    return 0;
  }
}



Card__aoeEff::Card__aoeEff()
{

  
}

Card__aoeEff::Card__aoeEff(Card  * input__item, Card__BaseTargetedEff  * input__effect, Card__TargetCond  * input__cond) : Card__BaseUntargetedEff((input__item)), effect((input__effect)), cond((input__cond))
{

  
  
  
}

Card__aoeEff::~Card__aoeEff()
{

  
  {
    if ((cond))
    {
      delete (cond);
    } else {
      
    }
  }
  {
    if ((effect))
    {
      delete (effect);
    } else {
      
    }
  }
}



void Card__aoeEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
    (((effect)->FillRep)((rep)));
    (((cond)->FillRep)((rep)));
  }
}

string Card__aoeEff::Detail()
{

  return ((((((effect)->DetailAlt1)()) + "all ") + (((cond)->DetailAlt2)())) + (((effect)->SuffixAlt1)()));
}

string Card__aoeEff::Postfix()
{

  return (((effect)->Postfix)());
}

string Card__aoeEff::PostfixIndent(signed int  indent_size)
{

  return (((effect)->PostfixIndent)((indent_size)));
}

Card__Node  *Card__aoeEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__aoeEff((card_copy), ((Card__BaseTargetedEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__TargetCond *)((((cond)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

void Card__aoeEff::UntargetedAction(Card  * parent_card)
{

  {
    signed int start_of_batch = 1;
    signed int total_cards = ((((((2 + (((((parent_card)->owner)->field).size)())) + (((((parent_card)->owner)->hand).size)())) + (((((parent_card)->owner)->deck).size)())) + (((((parent_card)->opponent)->field).size)())) + (((((parent_card)->opponent)->hand).size)())) + (((((parent_card)->opponent)->deck).size)()));
    for (signed int i = 0; ((i) < (total_cards)); ((i)++))
    {
      {
        Card  *target = ((((parent_card)->owner)->GetTargetCard)((i)));
        if ((((target) && (!((target)->is_dying))) && (((cond)->CheckCardValid)((target), (parent_card)))))
        {
          ((start_of_batch) = ((!(((effect)->TargetedAction)((i), (parent_card), (start_of_batch)))) && (start_of_batch)));
        } else {
          
        }
      }
    }
  }
}



Card__randEff::Card__randEff()
{

  
}

Card__randEff::Card__randEff(Card  * input__item, Card__BaseTargetedEff  * input__effect, Card__TargetCond  * input__cond) : Card__BaseUntargetedEff((input__item)), effect((input__effect)), cond((input__cond))
{

  
  
  
}

Card__randEff::~Card__randEff()
{

  
  {
    if ((cond))
    {
      delete (cond);
    } else {
      
    }
  }
  {
    if ((effect))
    {
      delete (effect);
    } else {
      
    }
  }
}



void Card__randEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
    (((effect)->FillRep)((rep)));
    (((cond)->FillRep)((rep)));
  }
}

string Card__randEff::Detail()
{

  return ((((((effect)->Detail)()) + "a random ") + (((cond)->Detail)())) + (((effect)->Suffix)()));
}

string Card__randEff::Postfix()
{

  return (((effect)->Postfix)());
}

string Card__randEff::PostfixIndent(signed int  indent_size)
{

  return (((effect)->PostfixIndent)((indent_size)));
}

Card__Node  *Card__randEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__randEff((card_copy), ((Card__BaseTargetedEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__TargetCond *)((((cond)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

void Card__randEff::UntargetedAction(Card  * parent_card)
{

  {
    signed int total_cards = ((((((2 + (((((parent_card)->owner)->field).size)())) + (((((parent_card)->owner)->hand).size)())) + (((((parent_card)->owner)->deck).size)())) + (((((parent_card)->opponent)->field).size)())) + (((((parent_card)->opponent)->hand).size)())) + (((((parent_card)->opponent)->deck).size)()));
    signed int  *candidates = (new signed int[(total_cards)]);
    signed int tmp_num = 0;
    for (signed int i = 0; ((i) < (total_cards)); ((i)++))
    {
      {
        Card  *target = ((((parent_card)->owner)->GetTargetCard)((i)));
        if ((((target) && (!((target)->is_dying))) && (((cond)->CheckCardValid)((target), (parent_card)))))
        {
          (((candidates)[((tmp_num)++)]) = (i));
        } else {
          
        }
      }
    }
    if (((tmp_num) > 0))
    {
      {
        signed int chosen_index = ((GetRandInt)((tmp_num)));
        (((effect)->TargetedAction)(((candidates)[(chosen_index)]), (parent_card), 1));
      }
    } else {
      
    }
    delete[](candidates);
  }
}



Card__leaderEff::Card__leaderEff()
{

  
}

Card__leaderEff::Card__leaderEff(Card  * input__item, Card__BaseTargetedEff  * input__effect, Card__AllegianceCond  * input__alle) : Card__BaseUntargetedEff((input__item)), effect((input__effect)), alle((input__alle))
{

  
  
  
}

Card__leaderEff::~Card__leaderEff()
{

  
  {
    if ((alle))
    {
      delete (alle);
    } else {
      
    }
  }
  {
    if ((effect))
    {
      delete (effect);
    } else {
      
    }
  }
}



void Card__leaderEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(2))));
    (((effect)->FillRep)((rep)));
    (((alle)->FillRep)((rep)));
  }
}

string Card__leaderEff::Detail()
{

  return (((((alle)->IsPlural)()) ? (((((effect)->DetailAlt1)()) + (((alle)->DetailAlt4)())) + (((effect)->SuffixAlt1)())) : (((((effect)->Detail)()) + (((alle)->DetailAlt4)())) + (((effect)->Suffix)()))));
}

string Card__leaderEff::Postfix()
{

  return (((effect)->Postfix)());
}

string Card__leaderEff::PostfixIndent(signed int  indent_size)
{

  return (((effect)->PostfixIndent)((indent_size)));
}

Card__Node  *Card__leaderEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__leaderEff((card_copy), ((Card__BaseTargetedEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__AllegianceCond *)((((alle)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

CondConfig Card__leaderEff::GetLeaderConfig()
{

  {
    CondConfig tmp_config = (((effect)->GetTargetConfig)());
    ((tmp_config) &= (((alle)->GetTargetConfig)()));
    return (tmp_config);
  }
}

void Card__leaderEff::UntargetedAction(Card  * parent_card)
{

  {
    signed int start_of_batch = 1;
    Card  *target_a = (((parent_card)->owner)->leader);
    if (((!((target_a)->is_dying)) && (((alle)->CheckCardValid)((target_a), (parent_card)))))
    {
      ((start_of_batch) = ((!(((effect)->TargetedAction)(0, (parent_card), 1))) && (start_of_batch)));
    } else {
      
    }
    Card  *target_b = (((parent_card)->opponent)->leader);
    if (((!((target_b)->is_dying)) && (((alle)->CheckCardValid)((target_b), (parent_card)))))
    {
      (((effect)->TargetedAction)((((((((parent_card)->owner)->field).size)()) + (((((parent_card)->opponent)->field).size)())) + 1), (parent_card), (start_of_batch)));
    } else {
      
    }
  }
}



Card__selfEff::Card__selfEff()
{

  
}

Card__selfEff::Card__selfEff(Card  * input__item, Card__BaseTargetedEff  * input__effect) : Card__BaseUntargetedEff((input__item)), effect((input__effect))
{

  
  
}

Card__selfEff::~Card__selfEff()
{

  
  {
    if ((effect))
    {
      delete (effect);
    } else {
      
    }
  }
}


void Card__selfEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(3))));
    (((effect)->FillRep)((rep)));
  }
}

string Card__selfEff::Detail()
{

  return (((((effect)->Detail)()) + "itself") + (((effect)->Suffix)()));
}

string Card__selfEff::Postfix()
{

  return (((effect)->Postfix)());
}

string Card__selfEff::PostfixIndent(signed int  indent_size)
{

  return (((effect)->PostfixIndent)((indent_size)));
}

Card__Node  *Card__selfEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__selfEff((card_copy), ((Card__BaseTargetedEff *)((((effect)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

CondConfig Card__selfEff::GetGlobalSelfConfig(const CondConfig  & self_config, unsigned int  effect_timing)
{

  {
    CondConfig tmp_config = (((effect)->GetGlobalSelfConfig)((self_config), (effect_timing)));
    if (((!(((self_config) & (TARGET_POS_HAND_OR_DECK)))) || (!(((tmp_config) & (TARGET_POS_HAND_OR_DECK))))))
    {
      ((tmp_config) &= (NOT_LEADER_COND_FILTER));
    } else {
      if (((!(((self_config) & (TARGET_NOT_LEADER)))) || (!(((tmp_config) & (TARGET_NOT_LEADER))))))
      {
        ((tmp_config) &= (HAND_OR_DECK_COND_FILTER));
      } else {
        ((tmp_config) &= ((((RandomRoll)(0.9)) ? (NOT_LEADER_COND_FILTER) : (HAND_OR_DECK_COND_FILTER))));
      }
    }
    return (tmp_config);
  }
}

CondConfig Card__selfEff::GetSelfConfig(const CondConfig  & self_config)
{

  {
    CondConfig tmp_config = (((effect)->GetSelfConfig)((self_config)));
    if (((!(((self_config) & (TARGET_POS_HAND_OR_DECK)))) || (!(((tmp_config) & (TARGET_POS_HAND_OR_DECK))))))
    {
      ((tmp_config) &= (NOT_LEADER_COND_FILTER));
    } else {
      if (((!(((self_config) & (TARGET_NOT_LEADER)))) || (!(((tmp_config) & (TARGET_NOT_LEADER))))))
      {
        ((tmp_config) &= (HAND_OR_DECK_COND_FILTER));
      } else {
        ((tmp_config) &= ((((RandomRoll)(0.9)) ? (NOT_LEADER_COND_FILTER) : (HAND_OR_DECK_COND_FILTER))));
      }
    }
    return (tmp_config);
  }
}

void Card__selfEff::UntargetedAction(Card  * parent_card)
{

  {
    if ((!((parent_card)->is_dying)))
    {
      (((effect)->TargetedAction)((((parent_card)->GetTargetIndex)()), (parent_card), 1));
    } else {
      
    }
  }
}



Card__drawCardEff::Card__drawCardEff()
{

  
}

Card__drawCardEff::Card__drawCardEff(Card  * input__item, signed int  input__val, Card__AllegianceCond  * input__alle) : Card__BaseUntargetedEff((input__item)), val((input__val)), alle((input__alle))
{

  
  
  
}

Card__drawCardEff::~Card__drawCardEff()
{

  
  {
    if ((alle))
    {
      delete (alle);
    } else {
      
    }
  }
  {
    
  }
}



void Card__drawCardEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(4, (vector<double>{((NormalizeCode)((val), 1, 3))})))));
    (((alle)->FillRep)((rep)));
  }
}

string Card__drawCardEff::Detail()
{

  return ((((((alle)->DetailAlt5)()) + (((((alle)->IsThirdPersonSingle)()) ? "draws " : "draw "))) + ((IntToStr)((val)))) + ((((val) == 1) ? " card" : " cards")));
}

Card__Node  *Card__drawCardEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__drawCardEff((card_copy), (val), ((Card__AllegianceCond *)((((alle)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

void Card__drawCardEff::UntargetedAction(Card  * parent_card)
{

  {
    signed int start_of_batch = 1;
    Card  *target_a = (((parent_card)->owner)->leader);
    if ((((alle)->CheckCardValid)((target_a), (parent_card))))
    {
      for (signed int i = 0; ((i) < (val)); ((i)++))
      {
        {
          ((((parent_card)->owner)->DrawCard)((start_of_batch)));
          ((start_of_batch) = 0);
        }
      }
    } else {
      
    }
    Card  *target_b = (((parent_card)->opponent)->leader);
    if ((((alle)->CheckCardValid)((target_b), (parent_card))))
    {
      for (signed int i = 0; ((i) < (val)); ((i)++))
      {
        {
          ((((parent_card)->opponent)->DrawCard)((start_of_batch)));
          ((start_of_batch) = 0);
        }
      }
    } else {
      
    }
  }
}



Card__mpEff::Card__mpEff()
{

  
}

Card__mpEff::Card__mpEff(Card  * input__item, signed int  input__val, Card__AllegianceCond  * input__alle) : Card__BaseUntargetedEff((input__item)), dec_prob(), val((input__val)), alle((input__alle))
{

  
  
  
}

Card__mpEff::~Card__mpEff()
{

  
  {
    if ((alle))
    {
      delete (alle);
    } else {
      
    }
  }
  {
    
  }
}




void Card__mpEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(5, (vector<double>{((NormalizeCode)((val), (-10), 10))})))));
    (((alle)->FillRep)((rep)));
  }
}

string Card__mpEff::Detail()
{

  return (((((((alle)->DetailAlt5)()) + ((((val) >= 0) ? "recover" : "exhaust"))) + (((((alle)->IsThirdPersonSingle)()) ? "s " : " "))) + ((IntToStr)(((abs)((val)))))) + " MP");
}

Card__Node  *Card__mpEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__mpEff((card_copy), (val), ((Card__AllegianceCond *)((((alle)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

CondConfig Card__mpEff::GetLeaderConfig()
{

  {
    CondConfig tmp_config;
    if (((val) < 0))
    {
      ((tmp_config) = ((GetMpConfig)((-(val)), 10)));
    } else {
      ((tmp_config) = ((GetMpConfig)(0, (10 - (val)))));
    }
    ((tmp_config) &= (((alle)->GetTargetConfig)()));
    return (tmp_config);
  }
}

void Card__mpEff::UntargetedAction(Card  * parent_card)
{

  {
    Card  *target_a = (((parent_card)->owner)->leader);
    if ((((alle)->CheckCardValid)((target_a), (parent_card))))
    {
      ((((parent_card)->owner)->ModifyMp)((val)));
    } else {
      
    }
    Card  *target_b = (((parent_card)->opponent)->leader);
    if ((((alle)->CheckCardValid)((target_b), (parent_card))))
    {
      ((((parent_card)->opponent)->ModifyMp)((val)));
    } else {
      
    }
  }
}



Card__maxMpEff::Card__maxMpEff()
{

  
}

Card__maxMpEff::Card__maxMpEff(Card  * input__item, signed int  input__val, Card__AllegianceCond  * input__alle) : Card__BaseUntargetedEff((input__item)), dec_prob(), val((input__val)), alle((input__alle))
{

  
  
  
}

Card__maxMpEff::~Card__maxMpEff()
{

  
  {
    if ((alle))
    {
      delete (alle);
    } else {
      
    }
  }
  {
    
  }
}




void Card__maxMpEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(6, (vector<double>{((NormalizeCode)((val), (-5), 5))})))));
    (((alle)->FillRep)((rep)));
  }
}

string Card__maxMpEff::Detail()
{

  return (((((((alle)->DetailAlt5)()) + ((((val) >= 0) ? "gain" : "lose"))) + (((((alle)->IsThirdPersonSingle)()) ? "s " : " "))) + ((IntToStr)(((abs)((val)))))) + " Max MP (and MP)");
}

Card__Node  *Card__maxMpEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__maxMpEff((card_copy), (val), ((Card__AllegianceCond *)((((alle)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

CondConfig Card__maxMpEff::GetLeaderConfig()
{

  {
    CondConfig tmp_config;
    if (((val) < 0))
    {
      ((tmp_config) = ((GetMaxMpConfig)((-(val)), 10)));
    } else {
      ((tmp_config) = ((GetMaxMpConfig)(0, (10 - (val)))));
    }
    ((tmp_config) &= (((alle)->GetTargetConfig)()));
    return (tmp_config);
  }
}

void Card__maxMpEff::UntargetedAction(Card  * parent_card)
{

  {
    Card  *target_a = (((parent_card)->owner)->leader);
    if ((((alle)->CheckCardValid)((target_a), (parent_card))))
    {
      ((((parent_card)->owner)->ModifyMaxMp)((val)));
    } else {
      
    }
    Card  *target_b = (((parent_card)->opponent)->leader);
    if ((((alle)->CheckCardValid)((target_b), (parent_card))))
    {
      ((((parent_card)->opponent)->ModifyMaxMp)((val)));
    } else {
      
    }
  }
}



Card__newEff::Card__newEff()
{

  
}

Card__newEff::Card__newEff(Card  * input__item, signed int  input__val, Card__Destination  * input__dest, Card__NewCardVariant  * input__variant) : Card__BaseUntargetedEff((input__item)), val((input__val)), dest((input__dest)), variant((input__variant))
{

  
  
  
  
}

Card__newEff::~Card__newEff()
{

  
  {
    if ((dest))
    {
      delete (dest);
    } else {
      
    }
  }
  {
    
  }
  {
    if ((variant))
    {
      delete (variant);
    } else {
      
    }
  }
}




void Card__newEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(7, (vector<double>{((NormalizeCode)((val), 1, 7))})))));
    (((dest)->FillRep)((rep)));
    (((variant)->FillRep)((rep)));
  }
}

string Card__newEff::Detail()
{

  return (((((dest)->Detail)()) + ((((val) == 1) ? (((variant)->Detail)()) : ((((IntToStr)((val))) + " ") + (((variant)->DetailAlt1)()))))) + (((dest)->Suffix)()));
}

string Card__newEff::Postfix()
{

  return (((variant)->Postfix)());
}

string Card__newEff::PostfixIndent(signed int  indent_size)
{

  return (((variant)->PostfixIndent)((indent_size)));
}

Card__Node  *Card__newEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__newEff((card_copy), (val), ((Card__Destination *)((((dest)->CreateNodeHardCopy)((card_copy), (redir_map))))), ((Card__NewCardVariant *)((((variant)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

void Card__newEff::UntargetedAction(Card  * parent_card)
{

  {
    signed int start_of_batch = 1;
    for (signed int i = 0; ((i) < (val)); ((i)++))
    {
      {
        Card  *card = (((variant)->GetRelatedCard)((parent_card)));
        (((card)->owner) = ((parent_card)->owner));
        (((card)->opponent) = ((parent_card)->opponent));
        (((dest)->SubTargetedAction)((card), (parent_card), (start_of_batch)));
        ((start_of_batch) = 0);
      }
    }
  }
}



Card__winGameEff::Card__winGameEff()
{

  
}

Card__winGameEff::Card__winGameEff(Card  * input__item, Card__AllegianceCond  * input__alle) : Card__BaseUntargetedEff((input__item)), alle((input__alle))
{

  
  
}

Card__winGameEff::~Card__winGameEff()
{

  
  {
    if ((alle))
    {
      delete (alle);
    } else {
      
    }
  }
}


void Card__winGameEff::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(8))));
    (((alle)->FillRep)((rep)));
  }
}

string Card__winGameEff::Detail()
{

  return ((((((alle)->DetailAlt5)()) + "win") + (((((alle)->IsThirdPersonSingle)()) ? "s " : " "))) + "the game");
}

Card__Node  *Card__winGameEff::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__winGameEff((card_copy), ((Card__AllegianceCond *)((((alle)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

void Card__winGameEff::UntargetedAction(Card  * parent_card)
{

  {
    Card  *target_a = (((parent_card)->owner)->leader);
    if ((((alle)->CheckCardValid)((target_a), (parent_card))))
    {
      ((((parent_card)->opponent)->SetLose)());
    } else {
      
    }
    Card  *target_b = (((parent_card)->opponent)->leader);
    if ((((alle)->CheckCardValid)((target_b), (parent_card))))
    {
      ((((parent_card)->owner)->SetLose)());
    } else {
      
    }
  }
}



Card__fieldDest::Card__fieldDest()
{

  
}

Card__fieldDest::Card__fieldDest(Card  * input__item, Card__AllegianceCond  * input__alle) : Card__Destination((input__item)), alle((input__alle))
{

  
  
}

Card__fieldDest::~Card__fieldDest()
{

  
  {
    if ((alle))
    {
      delete (alle);
    } else {
      
    }
  }
}


void Card__fieldDest::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
    (((alle)->FillRep)((rep)));
  }
}

string Card__fieldDest::Detail()
{

  return "Summon ";
}

string Card__fieldDest::Suffix()
{

  return ((" to " + (((alle)->DetailAlt2)())) + (((((alle)->IsPlural)()) ? "fields" : "field")));
}

Card__Node  *Card__fieldDest::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__fieldDest((card_copy), ((Card__AllegianceCond *)((((alle)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

CondConfig Card__fieldDest::GetTargetConfig()
{

  return (((((alle)->GetTargetConfig)()) & (FIELD_COND_FILTER)) & (MINION_COND_FILTER));
}

void Card__fieldDest::SubTargetedAction(Card  * target, Card  * parent_card, signed int  start_of_batch)
{

  {
    signed int is_ally = (((alle)->CheckCardValid)((((parent_card)->owner)->leader), (parent_card)));
    signed int is_opponent = (((alle)->CheckCardValid)((((parent_card)->opponent)->leader), (parent_card)));
    if ((is_ally))
    {
      {
        ((((parent_card)->owner)->FlagFieldSummon)((target), (start_of_batch)));
        if ((is_opponent))
        {
          ((((parent_card)->opponent)->FlagFieldSummon)((((target)->CreateCopy)()), 0));
        } else {
          
        }
      }
    } else {
      {
        if ((is_opponent))
        {
          ((((parent_card)->opponent)->FlagFieldSummon)((target), (start_of_batch)));
        } else {
          
        }
      }
    }
  }
}



Card__ownerFieldDest::Card__ownerFieldDest()
{

  
}

Card__ownerFieldDest::Card__ownerFieldDest(Card  * input__item) : Card__Destination((input__item))
{

  
}

Card__ownerFieldDest::~Card__ownerFieldDest()
{

  
}

void Card__ownerFieldDest::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
  }
}

string Card__ownerFieldDest::Detail()
{

  return "Summon ";
}

string Card__ownerFieldDest::Suffix()
{

  return " to its owner's field";
}

string Card__ownerFieldDest::SuffixAlt1()
{

  return " to their owners' fields";
}

Card__Node  *Card__ownerFieldDest::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__ownerFieldDest((card_copy)));
}

CondConfig Card__ownerFieldDest::GetTargetConfig()
{

  return (((~(TARGET_ANY_ALLE_TYPE)) & (FIELD_COND_FILTER)) & (MINION_COND_FILTER));
}

void Card__ownerFieldDest::SubTargetedAction(Card  * target, Card  * parent_card, signed int  start_of_batch)
{

  {
    ((((target)->owner)->FlagFieldSummon)((target), (start_of_batch)));
  }
}



Card__handDest::Card__handDest()
{

  
}

Card__handDest::Card__handDest(Card  * input__item, Card__AllegianceCond  * input__alle) : Card__Destination((input__item)), alle((input__alle))
{

  
  
}

Card__handDest::~Card__handDest()
{

  
  {
    if ((alle))
    {
      delete (alle);
    } else {
      
    }
  }
}


void Card__handDest::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(2))));
    (((alle)->FillRep)((rep)));
  }
}

string Card__handDest::Detail()
{

  return "Put ";
}

string Card__handDest::Suffix()
{

  return ((" to " + (((alle)->DetailAlt2)())) + (((((alle)->IsPlural)()) ? "hands" : "hand")));
}

Card__Node  *Card__handDest::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__handDest((card_copy), ((Card__AllegianceCond *)((((alle)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

CondConfig Card__handDest::GetTargetConfig()
{

  return ((((alle)->GetTargetConfig)()) & (HAND_COND_FILTER));
}

void Card__handDest::SubTargetedAction(Card  * target, Card  * parent_card, signed int  start_of_batch)
{

  {
    signed int is_ally = (((alle)->CheckCardValid)((((parent_card)->owner)->leader), (parent_card)));
    signed int is_opponent = (((alle)->CheckCardValid)((((parent_card)->opponent)->leader), (parent_card)));
    if ((is_ally))
    {
      {
        ((((parent_card)->owner)->FlagHandPut)((target), (start_of_batch)));
        if ((is_opponent))
        {
          ((((parent_card)->opponent)->FlagHandPut)((((target)->CreateCopy)()), 0));
        } else {
          
        }
      }
    } else {
      {
        if ((is_opponent))
        {
          ((((parent_card)->opponent)->FlagHandPut)((target), (start_of_batch)));
        } else {
          
        }
      }
    }
  }
}



Card__ownerHandDest::Card__ownerHandDest()
{

  
}

Card__ownerHandDest::Card__ownerHandDest(Card  * input__item) : Card__Destination((input__item))
{

  
}

Card__ownerHandDest::~Card__ownerHandDest()
{

  
}

void Card__ownerHandDest::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(3))));
  }
}

string Card__ownerHandDest::Detail()
{

  return "Put ";
}

string Card__ownerHandDest::Suffix()
{

  return " to its owner's hand";
}

string Card__ownerHandDest::SuffixAlt1()
{

  return " to their owners' hands";
}

Card__Node  *Card__ownerHandDest::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__ownerHandDest((card_copy)));
}

CondConfig Card__ownerHandDest::GetTargetConfig()
{

  return ((~(TARGET_ANY_ALLE_TYPE)) & (HAND_COND_FILTER));
}

void Card__ownerHandDest::SubTargetedAction(Card  * target, Card  * parent_card, signed int  start_of_batch)
{

  {
    ((((target)->owner)->FlagHandPut)((target), (start_of_batch)));
  }
}



Card__deckDest::Card__deckDest()
{

  
}

Card__deckDest::Card__deckDest(Card  * input__item, Card__AllegianceCond  * input__alle) : Card__Destination((input__item)), alle((input__alle))
{

  
  
}

Card__deckDest::~Card__deckDest()
{

  
  {
    if ((alle))
    {
      delete (alle);
    } else {
      
    }
  }
}


void Card__deckDest::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(4))));
    (((alle)->FillRep)((rep)));
  }
}

string Card__deckDest::Detail()
{

  return "Shuffle ";
}

string Card__deckDest::Suffix()
{

  return ((" to " + (((alle)->DetailAlt2)())) + (((((alle)->IsPlural)()) ? "decks" : "deck")));
}

Card__Node  *Card__deckDest::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__deckDest((card_copy), ((Card__AllegianceCond *)((((alle)->CreateNodeHardCopy)((card_copy), (redir_map)))))));
}

CondConfig Card__deckDest::GetTargetConfig()
{

  return ((((alle)->GetTargetConfig)()) & (DECK_COND_FILTER));
}

void Card__deckDest::SubTargetedAction(Card  * target, Card  * parent_card, signed int  start_of_batch)
{

  {
    signed int is_ally = (((alle)->CheckCardValid)((((parent_card)->owner)->leader), (parent_card)));
    signed int is_opponent = (((alle)->CheckCardValid)((((parent_card)->opponent)->leader), (parent_card)));
    if ((is_ally))
    {
      {
        ((((parent_card)->owner)->FlagDeckShuffle)((target), (start_of_batch)));
        if ((is_opponent))
        {
          ((((parent_card)->opponent)->FlagDeckShuffle)((((target)->CreateCopy)()), 0));
        } else {
          
        }
      }
    } else {
      {
        if ((is_opponent))
        {
          ((((parent_card)->opponent)->FlagDeckShuffle)((target), (start_of_batch)));
        } else {
          
        }
      }
    }
  }
}



Card__ownerDeckDest::Card__ownerDeckDest()
{

  
}

Card__ownerDeckDest::Card__ownerDeckDest(Card  * input__item) : Card__Destination((input__item))
{

  
}

Card__ownerDeckDest::~Card__ownerDeckDest()
{

  
}

void Card__ownerDeckDest::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(5))));
  }
}

string Card__ownerDeckDest::Detail()
{

  return "Shuffle ";
}

string Card__ownerDeckDest::Suffix()
{

  return " to its owner's deck";
}

string Card__ownerDeckDest::SuffixAlt1()
{

  return " to their owners' decks";
}

Card__Node  *Card__ownerDeckDest::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__ownerDeckDest((card_copy)));
}

CondConfig Card__ownerDeckDest::GetTargetConfig()
{

  return ((~(TARGET_ANY_ALLE_TYPE)) & (DECK_COND_FILTER));
}

void Card__ownerDeckDest::SubTargetedAction(Card  * target, Card  * parent_card, signed int  start_of_batch)
{

  {
    ((((target)->owner)->FlagDeckShuffle)((target), (start_of_batch)));
  }
}



Card__plainMinionCard::Card__plainMinionCard()
{

  
}

Card__plainMinionCard::Card__plainMinionCard(Card  * input__item, Card  * input__card) : Card__NewCardVariant((input__item)), card((input__card))
{

  
  
}

Card__plainMinionCard::~Card__plainMinionCard()
{

  
  {
    if ((card))
    {
      delete (card);
    } else {
      
    }
  }
}


void Card__plainMinionCard::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(0))));
    (((card)->FillRep)((rep)));
  }
}

string Card__plainMinionCard::Detail()
{

  return ((((((((("a " + ((IntToStr)(((card)->orig_mana)))) + "/") + ((IntToStr)(((card)->orig_atk)))) + "/") + ((IntToStr)(((card)->orig_hp)))) + " ") + ((MinionTypeDescription)(((card)->minion_type)))) + " minion card") + ((AttributeDescriptionInline)(((card)->orig_is_charge), ((card)->orig_is_taunt), ((card)->orig_is_stealth), ((card)->orig_is_untargetable), ((card)->orig_is_shielded), ((card)->orig_is_poisonous), ((card)->orig_is_lifesteal))));
}

string Card__plainMinionCard::DetailAlt1()
{

  return ((((((((((IntToStr)(((card)->orig_mana))) + "/") + ((IntToStr)(((card)->orig_atk)))) + "/") + ((IntToStr)(((card)->orig_hp)))) + " ") + ((MinionTypeDescription)(((card)->minion_type)))) + " minion cards") + ((AttributeDescriptionInline)(((card)->orig_is_charge), ((card)->orig_is_taunt), ((card)->orig_is_stealth), ((card)->orig_is_untargetable), ((card)->orig_is_shielded), ((card)->orig_is_poisonous), ((card)->orig_is_lifesteal))));
}

Card__Node  *Card__plainMinionCard::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__plainMinionCard((card_copy), (((card)->CreateHardCopy)((redir_map)))));
}

CondConfig Card__plainMinionCard::GetTargetConfig()
{

  return (HAND_OR_DECK_COND_FILTER);
}

Card  *Card__plainMinionCard::GetRelatedCard(Card  * parent_card)
{

  return (((card)->CreateCopy)());
}



Card__plainMinion::Card__plainMinion()
{

  
}

Card__plainMinion::Card__plainMinion(Card  * input__item, Card  * input__card) : Card__NewCardVariant((input__item)), card((input__card))
{

  
  
}

Card__plainMinion::~Card__plainMinion()
{

  
  {
    if ((card))
    {
      delete (card);
    } else {
      
    }
  }
}


void Card__plainMinion::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(1))));
    (((card)->FillRep)((rep)));
  }
}

string Card__plainMinion::Detail()
{

  return ((((((((("a " + ((IntToStr)(((card)->orig_mana)))) + "/") + ((IntToStr)(((card)->orig_atk)))) + "/") + ((IntToStr)(((card)->orig_hp)))) + " ") + ((MinionTypeDescription)(((card)->minion_type)))) + " minion") + ((AttributeDescriptionInline)(((card)->orig_is_charge), ((card)->orig_is_taunt), ((card)->orig_is_stealth), ((card)->orig_is_untargetable), ((card)->orig_is_shielded), ((card)->orig_is_poisonous), ((card)->orig_is_lifesteal))));
}

string Card__plainMinion::DetailAlt1()
{

  return ((((((((((IntToStr)(((card)->orig_mana))) + "/") + ((IntToStr)(((card)->orig_atk)))) + "/") + ((IntToStr)(((card)->orig_hp)))) + " ") + ((MinionTypeDescription)(((card)->minion_type)))) + " minions") + ((AttributeDescriptionInline)(((card)->orig_is_charge), ((card)->orig_is_taunt), ((card)->orig_is_stealth), ((card)->orig_is_untargetable), ((card)->orig_is_shielded), ((card)->orig_is_poisonous), ((card)->orig_is_lifesteal))));
}

Card__Node  *Card__plainMinion::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__plainMinion((card_copy), (((card)->CreateHardCopy)((redir_map)))));
}

CondConfig Card__plainMinion::GetTargetConfig()
{

  return ((MINION_COND_FILTER) & (FIELD_COND_FILTER));
}

Card  *Card__plainMinion::GetRelatedCard(Card  * parent_card)
{

  {
    Card  *card_copy = (((card)->CreateCopy)());
    return (card_copy);
  }
}



Card__fixedCard::Card__fixedCard()
{

  
}

Card__fixedCard::Card__fixedCard(Card  * input__item, Card  * input__card) : Card__NewCardVariant((input__item)), card((input__card))
{

  
  
}

Card__fixedCard::~Card__fixedCard()
{

  
  {
    if ((card))
    {
      delete (card);
    } else {
      
    }
  }
}


void Card__fixedCard::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(2))));
    (((card)->FillRep)((rep)));
  }
}

string Card__fixedCard::Detail()
{

  return "a copy of the following card";
}

string Card__fixedCard::DetailAlt1()
{

  return "copies of the following card";
}

string Card__fixedCard::Postfix()
{

  return ("\n" + (((card)->DetailIndent)(4)));
}

string Card__fixedCard::PostfixIndent(signed int  indent_size)
{

  return ("\n" + (((card)->DetailIndent)(((indent_size) + 4))));
}

Card__Node  *Card__fixedCard::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__fixedCard((card_copy), (((card)->CreateHardCopy)((redir_map)))));
}

CondConfig Card__fixedCard::GetTargetConfig()
{

  return (HAND_OR_DECK_COND_FILTER);
}

Card  *Card__fixedCard::GetRelatedCard(Card  * parent_card)
{

  return (((card)->CreateCopy)());
}



Card__fixedMinion::Card__fixedMinion()
{

  
}

Card__fixedMinion::Card__fixedMinion(Card  * input__item, Card  * input__card) : Card__NewCardVariant((input__item)), card((input__card))
{

  
  
}

Card__fixedMinion::~Card__fixedMinion()
{

  
  {
    if ((card))
    {
      delete (card);
    } else {
      
    }
  }
}


void Card__fixedMinion::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(3))));
    (((card)->FillRep)((rep)));
  }
}

string Card__fixedMinion::Detail()
{

  return "a copy the following minion";
}

string Card__fixedMinion::DetailAlt1()
{

  return "copies of the following minion";
}

string Card__fixedMinion::Postfix()
{

  return ("\n" + (((card)->DetailIndent)(4)));
}

string Card__fixedMinion::PostfixIndent(signed int  indent_size)
{

  return ("\n" + (((card)->DetailIndent)(((indent_size) + 4))));
}

Card__Node  *Card__fixedMinion::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__fixedMinion((card_copy), (((card)->CreateHardCopy)((redir_map)))));
}

CondConfig Card__fixedMinion::GetTargetConfig()
{

  return ((MINION_COND_FILTER) & (FIELD_COND_FILTER));
}

Card  *Card__fixedMinion::GetRelatedCard(Card  * parent_card)
{

  {
    Card  *card_copy = (((card)->CreateCopy)());
    return (card_copy);
  }
}



Card__randomCard::Card__randomCard()
{

  
}

Card__randomCard::Card__randomCard(Card  * input__item, signed int  input__cost) : Card__NewCardVariant((input__item)), cost((input__cost))
{

  
  
}

Card__randomCard::~Card__randomCard()
{

  
  {
    
  }
}


void Card__randomCard::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(4, (vector<double>{((NormalizeCode)((cost), 0, 10))})))));
  }
}

string Card__randomCard::Detail()
{

  return (("a random cost " + ((IntToStr)((cost)))) + " card");
}

string Card__randomCard::DetailAlt1()
{

  return (("random cost " + ((IntToStr)((cost)))) + " cards");
}

Card__Node  *Card__randomCard::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__randomCard((card_copy), (cost)));
}

CondConfig Card__randomCard::GetTargetConfig()
{

  return (HAND_OR_DECK_COND_FILTER);
}

Card  *Card__randomCard::GetRelatedCard(Card  * parent_card)
{

  return ((CreateRandomCard)(((parent_card)->name), (cost), 0, ((parent_card)->max_eff_num), 0));
}



Card__randomMinion::Card__randomMinion()
{

  
}

Card__randomMinion::Card__randomMinion(Card  * input__item, signed int  input__cost) : Card__NewCardVariant((input__item)), cost((input__cost))
{

  
  
}

Card__randomMinion::~Card__randomMinion()
{

  
  {
    
  }
}


void Card__randomMinion::FillRep(CardRep  & rep)
{

  {
    (((rep).push_back)(((mkNodeRep)(5, (vector<double>{((NormalizeCode)((cost), 0, 10))})))));
  }
}

string Card__randomMinion::Detail()
{

  return (("a random cost " + ((IntToStr)((cost)))) + " minion");
}

string Card__randomMinion::DetailAlt1()
{

  return (("random cost " + ((IntToStr)((cost)))) + " minions");
}

Card__Node  *Card__randomMinion::CreateNodeHardCopy(Card  * card_copy, PtrRedirMap  & redir_map)
{

  return (new Card__randomMinion((card_copy), (cost)));
}

CondConfig Card__randomMinion::GetTargetConfig()
{

  return ((MINION_COND_FILTER) & (FIELD_COND_FILTER));
}

Card  *Card__randomMinion::GetRelatedCard(Card  * parent_card)
{

  {
    Card  *card = ((CreateRandomMinion)(((parent_card)->name), (cost), 0, ((parent_card)->max_eff_num), 0));
    return (card);
  }
}




signed int Card__ConfigLambda__5505__24(const GiglConfig  & config);
signed int Card__ConfigLambda__5505__30(const GiglConfig  & config);
signed int Card__ConfigLambda__5505__33(const GiglConfig  & config);
double Card__ConfigLambda__5506__42(const GiglConfig  & config, CondConfig  & global_config, signed int  is_plain);
double Card__ConfigLambda__5506__63(const GiglConfig  & config, CondConfig  & global_config, signed int  is_plain);
double Card__ConfigLambda__5506__83(const GiglConfig  & config, CondConfig  & global_config, signed int  is_plain);
double Card__ConfigLambda__5507__45(const GiglConfig  & config);
double Card__ConfigLambda__5507__65(const GiglConfig  & config);
double Card__ConfigLambda__5507__86(const GiglConfig  & config);
double Card__ConfigLambda__5508__41(const GiglConfig  & config);
double Card__ConfigLambda__5508__56(const GiglConfig  & config);
double Card__ConfigLambda__5508__70(const GiglConfig  & config);
double Card__ConfigLambda__5509__44(const GiglConfig  & config, CondConfig  & self_config, signed int  damage);
double Card__ConfigLambda__5510__52(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, signed int  damage);
double Card__ConfigLambda__5511__38(const GiglConfig  & config, CondConfig  & self_config);
double Card__ConfigLambda__5511__55(const GiglConfig  & config, CondConfig  & self_config);
double Card__ConfigLambda__5512__36(const GiglConfig  & config, CondConfig  & self_config);
double Card__ConfigLambda__5512__52(const GiglConfig  & config, CondConfig  & self_config);
double Card__ConfigLambda__5513__40(const GiglConfig  & config, CondConfig  & self_config);
double Card__ConfigLambda__5513__58(const GiglConfig  & config, CondConfig  & self_config);
double Card__ConfigLambda__5514__50(const GiglConfig  & config, CondConfig  & self_config);
double Card__ConfigLambda__5514__73(const GiglConfig  & config, CondConfig  & self_config);
double Card__ConfigLambda__5515__38(const GiglConfig  & config, CondConfig  & self_config);
double Card__ConfigLambda__5515__55(const GiglConfig  & config, CondConfig  & self_config);
double Card__ConfigLambda__5516__44(const GiglConfig  & config, CondConfig  & self_config, signed int  damage);
double Card__ConfigLambda__5516__64(const GiglConfig  & config, CondConfig  & self_config, signed int  damage);
double Card__ConfigLambda__5517__44(const GiglConfig  & config, CondConfig  & self_config, signed int  damage);
double Card__ConfigLambda__5517__64(const GiglConfig  & config, CondConfig  & self_config, signed int  damage);
double Card__ConfigLambda__5518__48(const GiglConfig  & config, CondConfig  & self_config, signed int  min_n, signed int  max_n, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5519__52(const GiglConfig  & config, CondConfig  & self_config, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5519__79(const GiglConfig  & config, CondConfig  & self_config, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5519__106(const GiglConfig  & config, CondConfig  & self_config, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5520__40(const GiglConfig  & config, CondConfig  & self_config, signed int  min_n, signed int  max_n, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5520__60(const GiglConfig  & config, CondConfig  & self_config, signed int  min_n, signed int  max_n, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5521__54(const GiglConfig  & config, CondConfig  & self_config, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5521__81(const GiglConfig  & config, CondConfig  & self_config, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5521__106(const GiglConfig  & config, CondConfig  & self_config, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5521__128(const GiglConfig  & config, CondConfig  & self_config, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5521__149(const GiglConfig  & config, CondConfig  & self_config, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5521__169(const GiglConfig  & config, CondConfig  & self_config, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5522__48(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5522__74(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5522__103(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5523__52(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5523__80(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5523__111(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5524__48(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5524__73(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5525__52(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5526__47(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5526__67(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5526__82(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5526__93(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5526__103(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5527__52(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5528__42(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5528__65(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5528__89(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5528__112(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5528__131(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5528__146(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5528__160(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5529__54(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5529__78(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5529__98(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5530__51(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5530__72(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5530__89(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5531__42(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5531__63(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5531__82(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5531__103(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5531__128(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5531__147(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5531__169(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5531__191(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing);
double Card__ConfigLambda__5532__42(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial);
double Card__ConfigLambda__5532__61(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial);
double Card__ConfigLambda__5532__78(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial);
double Card__ConfigLambda__5532__94(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial);
double Card__ConfigLambda__5532__116(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial);
double Card__ConfigLambda__5533__41(const GiglConfig  & config, signed int  lower_min, signed int  lower_max, signed int  upper_min, signed int  upper_max);
double Card__ConfigLambda__5533__50(const GiglConfig  & config, signed int  lower_min, signed int  lower_max, signed int  upper_min, signed int  upper_max);
double Card__ConfigLambda__5534__50(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing);
double Card__ConfigLambda__5534__78(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing);
double Card__ConfigLambda__5534__99(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing);
double Card__ConfigLambda__5534__116(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing);
double Card__ConfigLambda__5534__136(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing);
double Card__ConfigLambda__5535__48(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5535__66(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5535__91(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5535__108(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5535__116(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5535__137(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5535__158(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5535__166(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5535__186(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5535__206(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5535__223(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5535__240(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5535__262(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5536__46(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5536__73(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5536__99(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5536__122(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5536__144(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5536__168(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5536__197(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5536__220(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5536__246(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5536__272(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5537__45(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5537__69(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5537__96(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5537__119(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5537__143(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5538__47(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5538__65(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5538__85(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5538__103(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5538__124(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5538__136(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5538__144(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5538__159(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5538__167(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5538__183(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5538__203(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff);
double Card__ConfigLambda__5539__44(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode);
double Card__ConfigLambda__5539__68(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode);
double Card__ConfigLambda__5539__86(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode);
double Card__ConfigLambda__5539__109(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode);
double Card__ConfigLambda__5539__127(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode);
double Card__ConfigLambda__5539__149(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode);
double Card__ConfigLambda__5540__53(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth);
double Card__ConfigLambda__5540__74(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth);
double Card__ConfigLambda__5540__93(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth);
double Card__ConfigLambda__5540__113(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth);
double Card__ConfigLambda__5540__133(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth);
double Card__ConfigLambda__5540__154(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth);
signed int Card__ConfigLambda__5505__24(const GiglConfig  & config)
{

  return (*((signed int *)(((config).vars)[0])));
}
signed int Card__ConfigLambda__5505__30(const GiglConfig  & config)
{

  return 3;
}
signed int Card__ConfigLambda__5505__33(const GiglConfig  & config)
{

  return 2;
}
double Card__ConfigLambda__5506__42(const GiglConfig  & config, CondConfig  & global_config, signed int  is_plain)
{

  return 0.1;
}
double Card__ConfigLambda__5506__63(const GiglConfig  & config, CondConfig  & global_config, signed int  is_plain)
{

  return 0.6;
}
double Card__ConfigLambda__5506__83(const GiglConfig  & config, CondConfig  & global_config, signed int  is_plain)
{

  return 0.3;
}
double Card__ConfigLambda__5507__45(const GiglConfig  & config)
{

  return 0.02;
}
double Card__ConfigLambda__5507__65(const GiglConfig  & config)
{

  return (1.0 - (0.02 + 0.08));
}
double Card__ConfigLambda__5507__86(const GiglConfig  & config)
{

  return 0.08;
}
double Card__ConfigLambda__5508__41(const GiglConfig  & config)
{

  return ((1.0 - 0.0) / 3.0);
}
double Card__ConfigLambda__5508__56(const GiglConfig  & config)
{

  return ((1.0 - 0.0) / 3.0);
}
double Card__ConfigLambda__5508__70(const GiglConfig  & config)
{

  return ((1.0 - 0.0) / 3.0);
}
double Card__ConfigLambda__5509__44(const GiglConfig  & config, CondConfig  & self_config, signed int  damage)
{

  return (1.0 - 0.0);
}
double Card__ConfigLambda__5510__52(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, signed int  damage)
{

  return (1.0 - 0.0);
}
double Card__ConfigLambda__5511__38(const GiglConfig  & config, CondConfig  & self_config)
{

  return (1.0 - 0.05);
}
double Card__ConfigLambda__5511__55(const GiglConfig  & config, CondConfig  & self_config)
{

  return 0.05;
}
double Card__ConfigLambda__5512__36(const GiglConfig  & config, CondConfig  & self_config)
{

  return (1.0 - 0.05);
}
double Card__ConfigLambda__5512__52(const GiglConfig  & config, CondConfig  & self_config)
{

  return 0.05;
}
double Card__ConfigLambda__5513__40(const GiglConfig  & config, CondConfig  & self_config)
{

  return (1.0 - 0.05);
}
double Card__ConfigLambda__5513__58(const GiglConfig  & config, CondConfig  & self_config)
{

  return 0.05;
}
double Card__ConfigLambda__5514__50(const GiglConfig  & config, CondConfig  & self_config)
{

  return (1.0 - 0.05);
}
double Card__ConfigLambda__5514__73(const GiglConfig  & config, CondConfig  & self_config)
{

  return 0.05;
}
double Card__ConfigLambda__5515__38(const GiglConfig  & config, CondConfig  & self_config)
{

  return (1.0 - 0.05);
}
double Card__ConfigLambda__5515__55(const GiglConfig  & config, CondConfig  & self_config)
{

  return 0.05;
}
double Card__ConfigLambda__5516__44(const GiglConfig  & config, CondConfig  & self_config, signed int  damage)
{

  return (1.0 - (0.01 * ((10 - (damage)))));
}
double Card__ConfigLambda__5516__64(const GiglConfig  & config, CondConfig  & self_config, signed int  damage)
{

  return (0.01 * ((10 - (damage))));
}
double Card__ConfigLambda__5517__44(const GiglConfig  & config, CondConfig  & self_config, signed int  damage)
{

  return (1.0 - (((damage) == 0) ? 0.01 : 0.05));
}
double Card__ConfigLambda__5517__64(const GiglConfig  & config, CondConfig  & self_config, signed int  damage)
{

  return (((damage) == 0) ? 0.01 : 0.05);
}
double Card__ConfigLambda__5518__48(const GiglConfig  & config, CondConfig  & self_config, signed int  min_n, signed int  max_n, signed int  effect_depth, signed int  give_eff)
{

  return (1.0 - 0.0);
}
double Card__ConfigLambda__5519__52(const GiglConfig  & config, CondConfig  & self_config, signed int  effect_depth, signed int  give_eff)
{

  return (1.0 - (0.15 + 0.30));
}
double Card__ConfigLambda__5519__79(const GiglConfig  & config, CondConfig  & self_config, signed int  effect_depth, signed int  give_eff)
{

  return 0.15;
}
double Card__ConfigLambda__5519__106(const GiglConfig  & config, CondConfig  & self_config, signed int  effect_depth, signed int  give_eff)
{

  return 0.30;
}
double Card__ConfigLambda__5520__40(const GiglConfig  & config, CondConfig  & self_config, signed int  min_n, signed int  max_n, signed int  effect_depth, signed int  give_eff)
{

  return (1.0 - ((-1.0) / (((min_n) - 2))));
}
double Card__ConfigLambda__5520__60(const GiglConfig  & config, CondConfig  & self_config, signed int  min_n, signed int  max_n, signed int  effect_depth, signed int  give_eff)
{

  return ((-1.0) / (((min_n) - 2)));
}
double Card__ConfigLambda__5521__54(const GiglConfig  & config, CondConfig  & self_config, signed int  effect_depth, signed int  give_eff)
{

  return 30;
}
double Card__ConfigLambda__5521__81(const GiglConfig  & config, CondConfig  & self_config, signed int  effect_depth, signed int  give_eff)
{

  return 475;
}
double Card__ConfigLambda__5521__106(const GiglConfig  & config, CondConfig  & self_config, signed int  effect_depth, signed int  give_eff)
{

  return 45;
}
double Card__ConfigLambda__5521__128(const GiglConfig  & config, CondConfig  & self_config, signed int  effect_depth, signed int  give_eff)
{

  return 5;
}
double Card__ConfigLambda__5521__149(const GiglConfig  & config, CondConfig  & self_config, signed int  effect_depth, signed int  give_eff)
{

  return 10;
}
double Card__ConfigLambda__5521__169(const GiglConfig  & config, CondConfig  & self_config, signed int  effect_depth, signed int  give_eff)
{

  return 10;
}
double Card__ConfigLambda__5522__48(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return (1.0 - (0.1 + 0.05));
}
double Card__ConfigLambda__5522__74(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 0.1;
}
double Card__ConfigLambda__5522__103(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 0.05;
}
double Card__ConfigLambda__5523__52(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return (1.0 - (0.1 + 0.05));
}
double Card__ConfigLambda__5523__80(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 0.1;
}
double Card__ConfigLambda__5523__111(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 0.05;
}
double Card__ConfigLambda__5524__48(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return 0.8;
}
double Card__ConfigLambda__5524__73(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return 0.2;
}
double Card__ConfigLambda__5525__52(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return (1.0 - 0.0);
}
double Card__ConfigLambda__5526__47(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return 0.30;
}
double Card__ConfigLambda__5526__67(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return 0.55;
}
double Card__ConfigLambda__5526__82(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return ((1.0 - (0.30 + 0.55)) / 3.0);
}
double Card__ConfigLambda__5526__93(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return ((1.0 - (0.30 + 0.55)) / 3.0);
}
double Card__ConfigLambda__5526__103(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return ((1.0 - (0.30 + 0.55)) / 3.0);
}
double Card__ConfigLambda__5527__52(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return (1.0 - 0.0);
}
double Card__ConfigLambda__5528__42(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return 0.5;
}
double Card__ConfigLambda__5528__65(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return 0.03;
}
double Card__ConfigLambda__5528__89(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return 0.15;
}
double Card__ConfigLambda__5528__112(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return 0.12;
}
double Card__ConfigLambda__5528__131(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return ((1.0 - (0.5 + (0.03 + (0.15 + 0.12)))) / 3.0);
}
double Card__ConfigLambda__5528__146(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return ((1.0 - (0.5 + (0.03 + (0.15 + 0.12)))) / 3.0);
}
double Card__ConfigLambda__5528__160(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return ((1.0 - (0.5 + (0.03 + (0.15 + 0.12)))) / 3.0);
}
double Card__ConfigLambda__5529__54(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return 0.1;
}
double Card__ConfigLambda__5529__78(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return 0.7;
}
double Card__ConfigLambda__5529__98(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return (1.0 - (0.1 + 0.7));
}
double Card__ConfigLambda__5530__51(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return 0.3;
}
double Card__ConfigLambda__5530__72(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return ((1.0 - 0.3) / 2.0);
}
double Card__ConfigLambda__5530__89(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return ((1.0 - 0.3) / 2.0);
}
double Card__ConfigLambda__5531__42(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return 950;
}
double Card__ConfigLambda__5531__63(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return 10;
}
double Card__ConfigLambda__5531__82(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return 10;
}
double Card__ConfigLambda__5531__103(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return 8;
}
double Card__ConfigLambda__5531__128(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return 7;
}
double Card__ConfigLambda__5531__147(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return 5;
}
double Card__ConfigLambda__5531__169(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return 5;
}
double Card__ConfigLambda__5531__191(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing)
{

  return 5;
}
double Card__ConfigLambda__5532__42(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial)
{

  return 900;
}
double Card__ConfigLambda__5532__61(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial)
{

  return 30;
}
double Card__ConfigLambda__5532__78(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial)
{

  return 30;
}
double Card__ConfigLambda__5532__94(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial)
{

  return 30;
}
double Card__ConfigLambda__5532__116(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  force_nontrivial)
{

  return 10;
}
double Card__ConfigLambda__5533__41(const GiglConfig  & config, signed int  lower_min, signed int  lower_max, signed int  upper_min, signed int  upper_max)
{

  return ((1.0 - 0.0) / 2.0);
}
double Card__ConfigLambda__5533__50(const GiglConfig  & config, signed int  lower_min, signed int  lower_max, signed int  upper_min, signed int  upper_max)
{

  return ((1.0 - 0.0) / 2.0);
}
double Card__ConfigLambda__5534__50(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing)
{

  return 0.4;
}
double Card__ConfigLambda__5534__78(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing)
{

  return 0.2;
}
double Card__ConfigLambda__5534__99(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing)
{

  return 0.2;
}
double Card__ConfigLambda__5534__116(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing)
{

  return 0.1;
}
double Card__ConfigLambda__5534__136(const GiglConfig  & config, CondConfig  & init_config, CondConfig  & instant_leader_config, unsigned int  effect_timing)
{

  return 0.1;
}
double Card__ConfigLambda__5535__48(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 400;
}
double Card__ConfigLambda__5535__66(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 180;
}
double Card__ConfigLambda__5535__91(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 15;
}
double Card__ConfigLambda__5535__108(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 0.2;
}
double Card__ConfigLambda__5535__116(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 70;
}
double Card__ConfigLambda__5535__137(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 70;
}
double Card__ConfigLambda__5535__158(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 0.2;
}
double Card__ConfigLambda__5535__166(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 10;
}
double Card__ConfigLambda__5535__186(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 80;
}
double Card__ConfigLambda__5535__206(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 20;
}
double Card__ConfigLambda__5535__223(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 50;
}
double Card__ConfigLambda__5535__240(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 50;
}
double Card__ConfigLambda__5535__262(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return (20.0 / ((1 + (effect_depth))));
}
double Card__ConfigLambda__5536__46(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 10;
}
double Card__ConfigLambda__5536__73(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 10;
}
double Card__ConfigLambda__5536__99(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 10;
}
double Card__ConfigLambda__5536__122(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 10;
}
double Card__ConfigLambda__5536__144(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 10;
}
double Card__ConfigLambda__5536__168(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 10;
}
double Card__ConfigLambda__5536__197(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 10;
}
double Card__ConfigLambda__5536__220(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 10;
}
double Card__ConfigLambda__5536__246(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 10;
}
double Card__ConfigLambda__5536__272(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 10;
}
double Card__ConfigLambda__5537__45(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 20;
}
double Card__ConfigLambda__5537__69(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 10;
}
double Card__ConfigLambda__5537__96(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 10;
}
double Card__ConfigLambda__5537__119(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 10;
}
double Card__ConfigLambda__5537__143(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return (20.0 / ((1 + (effect_depth))));
}
double Card__ConfigLambda__5538__47(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 500;
}
double Card__ConfigLambda__5538__65(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 250;
}
double Card__ConfigLambda__5538__85(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 150;
}
double Card__ConfigLambda__5538__103(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 30;
}
double Card__ConfigLambda__5538__124(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 50;
}
double Card__ConfigLambda__5538__136(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 0.2;
}
double Card__ConfigLambda__5538__144(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 10;
}
double Card__ConfigLambda__5538__159(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 0.3;
}
double Card__ConfigLambda__5538__167(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 10;
}
double Card__ConfigLambda__5538__183(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 30;
}
double Card__ConfigLambda__5538__203(const GiglConfig  & config, CondConfig  & self_config, unsigned int  effect_timing, signed int  effect_depth, signed int  give_eff)
{

  return 1;
}
double Card__ConfigLambda__5539__44(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode)
{

  return 10;
}
double Card__ConfigLambda__5539__68(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode)
{

  return 15;
}
double Card__ConfigLambda__5539__86(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode)
{

  return 20;
}
double Card__ConfigLambda__5539__109(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode)
{

  return 40;
}
double Card__ConfigLambda__5539__127(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode)
{

  return 5;
}
double Card__ConfigLambda__5539__149(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, unsigned int  dest_mode)
{

  return 10;
}
double Card__ConfigLambda__5540__53(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth)
{

  return 20;
}
double Card__ConfigLambda__5540__74(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth)
{

  return 50;
}
double Card__ConfigLambda__5540__93(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth)
{

  return 5;
}
double Card__ConfigLambda__5540__113(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth)
{

  return 15;
}
double Card__ConfigLambda__5540__133(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth)
{

  return 3;
}
double Card__ConfigLambda__5540__154(const GiglConfig  & config, CondConfig  & self_config, unsigned int  target_mode, unsigned int  effect_timing, signed int  effect_depth)
{

  return 7;
}

GiglConfig GetDefaultGenConfig(signed int  seed)
{

  {
    return (GiglConfig(1, 139, (sizeof(signed int)), (SizeVec{(sizeof(signed int))}), (VarPtrVec{((VarPtr)(&((*((signed int *)((malloc)((sizeof(signed int)))))) = (seed))))}), (IndexVec{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 114, 91, 92, 115, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 116, 117, 118, 119, 120, 125, 121, 126, 122, 123, 124, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138}), (FuncPtrVec{((FuncPtr)(&(Card__ConfigLambda__5505__24))), ((FuncPtr)(&(Card__ConfigLambda__5505__30))), ((FuncPtr)(&(Card__ConfigLambda__5505__33))), ((FuncPtr)(&(Card__ConfigLambda__5506__42))), ((FuncPtr)(&(Card__ConfigLambda__5506__63))), ((FuncPtr)(&(Card__ConfigLambda__5506__83))), ((FuncPtr)(&(Card__ConfigLambda__5507__45))), ((FuncPtr)(&(Card__ConfigLambda__5507__65))), ((FuncPtr)(&(Card__ConfigLambda__5507__86))), ((FuncPtr)(&(Card__ConfigLambda__5508__41))), ((FuncPtr)(&(Card__ConfigLambda__5508__56))), ((FuncPtr)(&(Card__ConfigLambda__5508__70))), ((FuncPtr)(&(Card__ConfigLambda__5509__44))), ((FuncPtr)(&(Card__ConfigLambda__5510__52))), ((FuncPtr)(&(Card__ConfigLambda__5511__38))), ((FuncPtr)(&(Card__ConfigLambda__5511__55))), ((FuncPtr)(&(Card__ConfigLambda__5512__36))), ((FuncPtr)(&(Card__ConfigLambda__5512__52))), ((FuncPtr)(&(Card__ConfigLambda__5513__40))), ((FuncPtr)(&(Card__ConfigLambda__5513__58))), ((FuncPtr)(&(Card__ConfigLambda__5514__50))), ((FuncPtr)(&(Card__ConfigLambda__5514__73))), ((FuncPtr)(&(Card__ConfigLambda__5515__38))), ((FuncPtr)(&(Card__ConfigLambda__5515__55))), ((FuncPtr)(&(Card__ConfigLambda__5516__44))), ((FuncPtr)(&(Card__ConfigLambda__5516__64))), ((FuncPtr)(&(Card__ConfigLambda__5517__44))), ((FuncPtr)(&(Card__ConfigLambda__5517__64))), ((FuncPtr)(&(Card__ConfigLambda__5518__48))), ((FuncPtr)(&(Card__ConfigLambda__5519__52))), ((FuncPtr)(&(Card__ConfigLambda__5519__79))), ((FuncPtr)(&(Card__ConfigLambda__5519__106))), ((FuncPtr)(&(Card__ConfigLambda__5520__40))), ((FuncPtr)(&(Card__ConfigLambda__5520__60))), ((FuncPtr)(&(Card__ConfigLambda__5521__54))), ((FuncPtr)(&(Card__ConfigLambda__5521__81))), ((FuncPtr)(&(Card__ConfigLambda__5521__106))), ((FuncPtr)(&(Card__ConfigLambda__5521__128))), ((FuncPtr)(&(Card__ConfigLambda__5521__149))), ((FuncPtr)(&(Card__ConfigLambda__5521__169))), ((FuncPtr)(&(Card__ConfigLambda__5522__48))), ((FuncPtr)(&(Card__ConfigLambda__5522__74))), ((FuncPtr)(&(Card__ConfigLambda__5522__103))), ((FuncPtr)(&(Card__ConfigLambda__5523__52))), ((FuncPtr)(&(Card__ConfigLambda__5523__80))), ((FuncPtr)(&(Card__ConfigLambda__5523__111))), ((FuncPtr)(&(Card__ConfigLambda__5524__48))), ((FuncPtr)(&(Card__ConfigLambda__5524__73))), ((FuncPtr)(&(Card__ConfigLambda__5525__52))), ((FuncPtr)(&(Card__ConfigLambda__5526__47))), ((FuncPtr)(&(Card__ConfigLambda__5526__67))), ((FuncPtr)(&(Card__ConfigLambda__5526__82))), ((FuncPtr)(&(Card__ConfigLambda__5526__93))), ((FuncPtr)(&(Card__ConfigLambda__5526__103))), ((FuncPtr)(&(Card__ConfigLambda__5527__52))), ((FuncPtr)(&(Card__ConfigLambda__5528__42))), ((FuncPtr)(&(Card__ConfigLambda__5528__65))), ((FuncPtr)(&(Card__ConfigLambda__5528__89))), ((FuncPtr)(&(Card__ConfigLambda__5528__112))), ((FuncPtr)(&(Card__ConfigLambda__5528__131))), ((FuncPtr)(&(Card__ConfigLambda__5528__146))), ((FuncPtr)(&(Card__ConfigLambda__5528__160))), ((FuncPtr)(&(Card__ConfigLambda__5529__54))), ((FuncPtr)(&(Card__ConfigLambda__5529__78))), ((FuncPtr)(&(Card__ConfigLambda__5529__98))), ((FuncPtr)(&(Card__ConfigLambda__5530__51))), ((FuncPtr)(&(Card__ConfigLambda__5530__72))), ((FuncPtr)(&(Card__ConfigLambda__5530__89))), ((FuncPtr)(&(Card__ConfigLambda__5531__42))), ((FuncPtr)(&(Card__ConfigLambda__5531__63))), ((FuncPtr)(&(Card__ConfigLambda__5531__82))), ((FuncPtr)(&(Card__ConfigLambda__5531__103))), ((FuncPtr)(&(Card__ConfigLambda__5531__128))), ((FuncPtr)(&(Card__ConfigLambda__5531__147))), ((FuncPtr)(&(Card__ConfigLambda__5531__169))), ((FuncPtr)(&(Card__ConfigLambda__5531__191))), ((FuncPtr)(&(Card__ConfigLambda__5532__42))), ((FuncPtr)(&(Card__ConfigLambda__5532__61))), ((FuncPtr)(&(Card__ConfigLambda__5532__78))), ((FuncPtr)(&(Card__ConfigLambda__5532__94))), ((FuncPtr)(&(Card__ConfigLambda__5532__116))), ((FuncPtr)(&(Card__ConfigLambda__5533__41))), ((FuncPtr)(&(Card__ConfigLambda__5533__50))), ((FuncPtr)(&(Card__ConfigLambda__5534__50))), ((FuncPtr)(&(Card__ConfigLambda__5534__78))), ((FuncPtr)(&(Card__ConfigLambda__5534__99))), ((FuncPtr)(&(Card__ConfigLambda__5534__116))), ((FuncPtr)(&(Card__ConfigLambda__5534__136))), ((FuncPtr)(&(Card__ConfigLambda__5535__48))), ((FuncPtr)(&(Card__ConfigLambda__5535__66))), ((FuncPtr)(&(Card__ConfigLambda__5535__91))), ((FuncPtr)(&(Card__ConfigLambda__5535__108))), ((FuncPtr)(&(Card__ConfigLambda__5535__116))), ((FuncPtr)(&(Card__ConfigLambda__5535__137))), ((FuncPtr)(&(Card__ConfigLambda__5535__158))), ((FuncPtr)(&(Card__ConfigLambda__5535__166))), ((FuncPtr)(&(Card__ConfigLambda__5535__186))), ((FuncPtr)(&(Card__ConfigLambda__5535__206))), ((FuncPtr)(&(Card__ConfigLambda__5535__223))), ((FuncPtr)(&(Card__ConfigLambda__5535__240))), ((FuncPtr)(&(Card__ConfigLambda__5535__262))), ((FuncPtr)(&(Card__ConfigLambda__5536__46))), ((FuncPtr)(&(Card__ConfigLambda__5536__73))), ((FuncPtr)(&(Card__ConfigLambda__5536__99))), ((FuncPtr)(&(Card__ConfigLambda__5536__122))), ((FuncPtr)(&(Card__ConfigLambda__5536__144))), ((FuncPtr)(&(Card__ConfigLambda__5536__168))), ((FuncPtr)(&(Card__ConfigLambda__5536__197))), ((FuncPtr)(&(Card__ConfigLambda__5536__220))), ((FuncPtr)(&(Card__ConfigLambda__5536__246))), ((FuncPtr)(&(Card__ConfigLambda__5536__272))), ((FuncPtr)(&(Card__ConfigLambda__5537__45))), ((FuncPtr)(&(Card__ConfigLambda__5537__69))), ((FuncPtr)(&(Card__ConfigLambda__5537__96))), ((FuncPtr)(&(Card__ConfigLambda__5537__119))), ((FuncPtr)(&(Card__ConfigLambda__5537__143))), ((FuncPtr)(&(Card__ConfigLambda__5538__47))), ((FuncPtr)(&(Card__ConfigLambda__5538__65))), ((FuncPtr)(&(Card__ConfigLambda__5538__85))), ((FuncPtr)(&(Card__ConfigLambda__5538__103))), ((FuncPtr)(&(Card__ConfigLambda__5538__124))), ((FuncPtr)(&(Card__ConfigLambda__5538__136))), ((FuncPtr)(&(Card__ConfigLambda__5538__144))), ((FuncPtr)(&(Card__ConfigLambda__5538__159))), ((FuncPtr)(&(Card__ConfigLambda__5538__167))), ((FuncPtr)(&(Card__ConfigLambda__5538__183))), ((FuncPtr)(&(Card__ConfigLambda__5538__203))), ((FuncPtr)(&(Card__ConfigLambda__5539__44))), ((FuncPtr)(&(Card__ConfigLambda__5539__68))), ((FuncPtr)(&(Card__ConfigLambda__5539__86))), ((FuncPtr)(&(Card__ConfigLambda__5539__109))), ((FuncPtr)(&(Card__ConfigLambda__5539__127))), ((FuncPtr)(&(Card__ConfigLambda__5539__149))), ((FuncPtr)(&(Card__ConfigLambda__5540__53))), ((FuncPtr)(&(Card__ConfigLambda__5540__74))), ((FuncPtr)(&(Card__ConfigLambda__5540__93))), ((FuncPtr)(&(Card__ConfigLambda__5540__113))), ((FuncPtr)(&(Card__ConfigLambda__5540__133))), ((FuncPtr)(&(Card__ConfigLambda__5540__154)))})));
  }
}
Card  *Card__ItemRef__5547__24;

Card  *GenerateCard(signed int  seed)
{

  {
    ((RandInit)((seed)));
    return (((Card__ItemRef__5547__24) = (new Card(((GetDefaultGenConfig)((seed)))))) , (((Card__ItemRef__5547__24)->Item__Generator)()));
  }
}
Card  *Card__ItemRef__5552__33;

Card  *CreateDefaultLeader(signed int  hp)
{

  {
    Card  *leader = (((Card__ItemRef__5552__33) = (new Card(((GetDefaultGenConfig)(((GetRandInt)())))))) , (((Card__ItemRef__5552__33)->Item__Constructor)((new Card__leaderCard((Card__ItemRef__5552__33), 7, 0, (hp), (new Card__singleAttack((Card__ItemRef__5552__33))), (new Card__justAttributes((Card__ItemRef__5552__33), (new Card__noCharge((Card__ItemRef__5552__33))), (new Card__noTaunt((Card__ItemRef__5552__33))), (new Card__noStealth((Card__ItemRef__5552__33))), (new Card__noUntargetable((Card__ItemRef__5552__33))), (new Card__noShield((Card__ItemRef__5552__33))), (new Card__noPoisonous((Card__ItemRef__5552__33))), (new Card__noLifesteal((Card__ItemRef__5552__33))))), (new Card__specialEffects((Card__ItemRef__5552__33), (new Card__noTargetedPlayEff((Card__ItemRef__5552__33))), (new Card__noOtherEffs((Card__ItemRef__5552__33))))))))));
    (((leader)->name) = "Default Leader");
    return (leader);
  }
}
Card  *Card__ItemRef__5559__32;

Card  *CreateSndPlayerToken()
{

  {
    Card  *token = (((Card__ItemRef__5559__32) = (new Card(((GetDefaultGenConfig)(((GetRandInt)())))))) , (((Card__ItemRef__5559__32)->Item__Constructor)((new Card__spellCard((Card__ItemRef__5559__32), 0, (new Card__justAttributes((Card__ItemRef__5559__32), (new Card__noCharge((Card__ItemRef__5559__32))), (new Card__noTaunt((Card__ItemRef__5559__32))), (new Card__noStealth((Card__ItemRef__5559__32))), (new Card__noUntargetable((Card__ItemRef__5559__32))), (new Card__noShield((Card__ItemRef__5559__32))), (new Card__noPoisonous((Card__ItemRef__5559__32))), (new Card__noLifesteal((Card__ItemRef__5559__32))))), (new Card__specialEffects((Card__ItemRef__5559__32), (new Card__targetedCastEff((Card__ItemRef__5559__32), (new Card__noCondTargetedEff((Card__ItemRef__5559__32), (new Card__costModEff((Card__ItemRef__5559__32), (-1))), (new Card__cardTargetCond((Card__ItemRef__5559__32), (new Card__justCardTargetCond((Card__ItemRef__5559__32), (new Card__cardPosAtHand((Card__ItemRef__5559__32))), (new Card__allyAllegiance((Card__ItemRef__5559__32))), (new Card__isCard((Card__ItemRef__5559__32))), (new Card__noAttrCond((Card__ItemRef__5559__32))), (new Card__noStatCond((Card__ItemRef__5559__32))))))))))), (new Card__consOtherEffs((Card__ItemRef__5559__32), (new Card__untargetedCastEff((Card__ItemRef__5559__32), (new Card__noCondUntargetedEff((Card__ItemRef__5559__32), (new Card__drawCardEff((Card__ItemRef__5559__32), 1, (new Card__allyAllegiance((Card__ItemRef__5559__32))))))))), (new Card__noOtherEffs((Card__ItemRef__5559__32))))))))))));
    (((token)->name) = "Second Player Token");
    return (token);
  }
}
Card  *Card__ItemRef__5572__31;

Card  *CreatePlainMinion(string  parent_name)
{

  {
    signed int seed = ((GetRandInt)());
    ((RandInit)((seed)));
    CondConfig tmp_config = ((GetFlagConfig)((MINION_COND_FILTER)));
    Card  *card = (((Card__ItemRef__5572__31) = (new Card(((GetDefaultGenConfig)((seed)))))) , (((Card__ItemRef__5572__31)->Item__Constructor)((((Card__ItemRef__5572__31)->Generate__CardRoot)((tmp_config), 1)))));
    (((card)->name) = (((parent_name) + "_Spawn_#") + ((IntToStr)((seed)))));
    return (card);
  }
}
Card  *Card__ItemRef__5584__31;

Card  *CreateRandomMinion(string  parent_name, signed int  cost, signed int  min_eff_num, signed int  max_eff_num, signed int  eff_depth)
{

  {
    signed int seed = ((GetRandInt)());
    ((RandInit)((seed)));
    CondConfig tmp_config = ((GetCostConfig)((MINION_COND_FILTER), (cost), (cost)));
    Card  *card = (((Card__ItemRef__5584__31) = (new Card(((GetDefaultGenConfig)((seed)))))) , (((Card__ItemRef__5584__31)->Item__Constructor)((((Card__ItemRef__5584__31)->Generate__CardRoot)((tmp_config), 1)))));
    (((card)->name) = (((parent_name) + "_Spawn_#") + ((IntToStr)((seed)))));
    (((card)->Mutate)((min_eff_num), (max_eff_num), (eff_depth)));
    return (card);
  }
}
Card  *Card__ItemRef__5597__31;

Card  *CreateRandomCard(string  parent_name, signed int  cost, signed int  min_eff_num, signed int  max_eff_num, signed int  eff_depth)
{

  {
    signed int seed = ((GetRandInt)());
    ((RandInit)((seed)));
    CondConfig tmp_config = ((GetCostConfig)((TARGET_TYPE_ANY), (cost), (cost)));
    Card  *card = (((Card__ItemRef__5597__31) = (new Card(((GetDefaultGenConfig)((seed)))))) , (((Card__ItemRef__5597__31)->Item__Constructor)((((Card__ItemRef__5597__31)->Generate__CardRoot)((tmp_config), 1)))));
    (((card)->name) = (((parent_name) + "_Spawn_#") + ((IntToStr)((seed)))));
    (((card)->Mutate)((min_eff_num), (max_eff_num), (eff_depth)));
    return (card);
  }
}

