// note that this is a file containing rep for variety of purposes, approximately indicated by the sections with the comments
// it is not easy to separate into different files due to artifacts from GIGL (not separating header and implementation), which requires all interface to Cards (accessing class member variable or methods, exceptions includes only using the class type) 
// must be implemented in one file (here it is Player.cpp), so there is only one file (Player.cpp) that directly or indirectly include card.generated.h,
// having #pragma once or #include guards on card.generated.h would NOT work as an alternative
#pragma once

#include "DataUtil.h"

#include <vector>
#include <queue>
#include <string>
#include <map>

/* Card/Player section */


// type of positions for a card
#define CARD_POS_AT_LEADER 1
#define CARD_POS_AT_FIELD 2
#define CARD_POS_AT_HAND 3
#define CARD_POS_AT_DECK 4
#define CARD_POS_UNKNOWN 5

#define INIT_HAND_SIZE 3
#define MAX_FIELD_SIZE 7
#define MAX_MP 10
#define MAX_HAND_SIZE 10
#define MAX_NUM_TURNS 30 // the design is to make it same as the HP on default leader so that the game can normally end with a rate of losing 1 HP per pair of turns

#define LEADER_CARD 0
#define MINION_CARD 1
#define SPELL_CARD 2

#define NONE_MINION 0
#define BEAST_MINION 1
#define DRAGON_MINION 2
#define DEMON_MINION 3

#define COPY_EXACT 0
#define COPY_ALLY 1
#define COPY_OPPO 2

class Card;
class ActionEntity;
class ActionSetEntity;
class DeferredEvent;

typedef std::map<void*, void*> PtrRedirMap;
typedef std::map<void*, void*>::iterator PtrRedirMapIter;

class Player
{
public:
	Player(std::queue<DeferredEvent*>& _event_queue);
	Player(const std::string& _name, int _hp, const std::vector<Card*>& _deck, bool _is_guest, std::queue<DeferredEvent*>& _event_queue);
	Player(const std::string& _name, int _hp, const std::vector<Card*>& _deck, bool _is_guest, std::queue<DeferredEvent*>& _event_queue, int _ai_level);
	Player(const std::string& _name, int _hp, const std::vector<Card*>& _deck, bool _is_guest, std::queue<DeferredEvent*>& _event_queue, int _ai_level, const std::vector<Card*>& _card_pool);
	~Player();
	Card* GetRandomUnknownCard() const;
	Player* CreateKnowledgeCopy(unsigned mode, std::queue<DeferredEvent*>& event_queue, PtrRedirMap& redir_map) const; // creating a copy for the purpose of AI exploring; different modes: COPY_EXACT - copy exactly; COPY_ALLY - copy for the allied player (field and hand are preserved, deck is randomly generated); COPY_OPPO - copy for the allied player (only field is preserved, others are randomly generated)
	void RegisterCardContributions(std::vector<int>& counters, std::vector<int>& cost_constrained_counters); // link each card in the deck to a countribution counter and a cost constrained (contribution) counter
	void SetAllCardAfflications();
	void SetCardAfflication(Card* card); // used after opponent of the owner is set
	void InitialCardDraw(bool is_second_player); // this should be different from calling DrawCard() a few times, as effects (including fatigue, if any) should not be triggered here
	bool CleanUp(); // process deferred events, clear corpse, check game end etc., return whether game should end
	bool ProcessDeferredEvents(); // return whether game should end; only deal with current event queue
	void ClearCorpse(); // clean spots/cards/minions that should be removed but temporarily retained
	bool CheckGameEnd();
	void FlagDestroy(Card* card, bool start_of_batch);
	void FlagCastSpell(Card* card, bool start_of_batch);
	void FlagFieldDiscard(Card* card, bool start_of_batch);
	void FlagHandDiscard(Card* card, bool start_of_batch);
	void FlagDeckDiscard(Card* card, bool start_of_batch);
	void FlagFieldSummon(Card* card, bool start_of_batch); // if field full, issue a discard event
	void FlagHandPut(Card* card, bool start_of_batch); // if hand full, issue a discard event
	void FlagDeckShuffle(Card* card, bool start_of_batch);
	void FlagCardTransform(Card* card, bool start_of_batch, Card* replacement); // defer the deletion of transformed card
	void FlagCardReset(Card* card, bool start_of_batch); // defer the deletion of extra effects on cards when resetting states
	void SetLose(); // this is mostly used for special effects that grants lost/victory irrelevant to leader health
	bool CheckLose() const;	
	void StartTurn();
	void EndTurn();
	bool CheckPlayValid(int x, int y, int& z) const; // card x, poistion y, target z (if there is no valid target for a battlecry z will be modified to -1)
	void Play(int x, int y, int z); // card x, poistion y, target z, assume already checked valid
	bool CheckAttackValid(int x, int z) const; // card x, target z
	void Attack(int x, int z); // card x, target z, assume already checked valid
	void DrawCard(bool start_of_batch);
	void RecoverAttackTimes(); // restore all attack times for friendly characters and switch off their is_first_turn_at_field flag
	bool CheckMP(int cost) const; // return if remaining mp is enough
	void UseMP(int cost);
	void ModifyMp(int amount);
	void ModifyMaxMp(int amount);
	bool IsValidTarget(int z) const; // note that this is intended for targeted (player specified target) effects
	bool IsValidCharTarget(int z) const; // note that this is intended for targeted (player specified target) effects
	bool IsValidMinionTarget(int z) const; // note that this is intended for targeted (player specified target) effects
	bool IsValidCardTarget(int z) const; // note that this is intended for targeted (player specified target) effects
	bool IsTargetAlly(int z) const; // note that this is intended for targeted (player specified target) effects
	bool IsTargetOpponent(int z) const;	// note that this is intended for targeted (player specified target) effects
	Card* GetTargetCard(int z) const;
	Card* ExtractTargetCard(int z); // the target is removed (replaced with nullptr temporarily maintain indexing) from where it is and returned, leaders cannot be removed and is not expected to be a valid input index (will return nullptr if index is for leader or not valid), if the target is dying, also do not remove it here (returns nullptr)
	void SummonToField(Card* card); // does not trigger battlecry, this function itself does not check for field full (if it were full it will be still added but there should be a discard event in the queue right after)
	void PutToHand(Card* card); // if full, this function itself does not check for field full (if it were full it will be still added but there should be a discard event in the queue right after)
	void ShuffleToDeck(Card* card); // the position is randomized
	static double GetMaxReservedCardsValue(const std::vector<int>& card_costs, const std::vector<double>& card_values, const std::vector<int>& minion_flags, int avail_mp, int field_slots); // use dynamic programming to estimate the value that can be made out of the remaining mp 
	double GetHeuristicAdjustment(int turn_num_adjust) const; // secondary portion of heuristic evaluation, assuming at the start of the turn that is turn_num_adjust ahead of the current one
	double ForwardToNextTurnAndEval(); // evaluate a state that is either just before performing the end turn action, or just after the end turn action is performed, or already lost/drew/won, scaled and clamped withing -1 ~ 1 (normally -0.99 ~ 0.99 unless already lost or won, and more commonly -0.891 ~ 0.891)	
	double GetHeuristicEval() const; // the heuristic about how good the situation is for the player, scaled and clamped within -1.0 ~ 1.0 (normally -0.9 ~ 0.9 unless (almost) lost or won), evaluated at the start of the opponent turn, but compensate for unused cost and attack times etc. as it may be artificially ended earlier
	std::vector<ActionSetEntity*> GetOptionSet() const; // Get the collection of valid actions grouped by the type and source card index
	std::vector<ActionEntity*> GetActionList() const; // Get the flat list of actions, not grouped (different combination of type, source, position, destination are all considered different individual action)
	void TakeSearchAIInputs();
	void TakeSearchAIInput();
	void TakeRandomAIInputs();
	void TakeRandomAIInput();
	void TakeInputs();
	void TakeSingleInput();
	void DisplayHelp() const;
	void Query(int x) const;
	std::string BriefInfo() const;
	std::string DetailInfo() const;
	void PrintBoard() const;
	int GetActualFieldSize() const; // the actual size, excluding those queued for deletion
	int GetActualHandSize() const; // the actual size, excluding those queued for deletion
	int GetActualDeckSize() const; // the actual size, excluding those queued for deletion
	Card* leader; // essentially a played hero card
	std::vector<Card*> field; // ordered from left to right
	std::vector<Card*> hand; // ordered from left to right
	std::vector<Card*> deck; // ordered from bottom to top
	std::string name;
	bool is_guest; // whether it is an active controlling player from this terminal (not displaying error message etc.)
	bool is_exploration; // whether the steps it takes is exploring the 
	Player* opponent;
	bool is_lost;
	bool is_turn_active;
	int turn_num;
	int max_mp;
	int mp_loss;
	int fatigue;
	int field_size_adjust; // the discrepancy between the actual size and the size of the vector, due to the existence of deferred events
	int hand_size_adjust; // the discrepancy between the actual size and the size of the vector, due to the existence of deferred events
	int deck_size_adjust; // the discrepancy between the actual size and the size of the vector, due to the existence of deferred events
	std::queue<DeferredEvent*>& event_queue; // reference to the queue for deferred event (shared between two players)
	int ai_level; // 0 means random ai, 1 ~ 9 means search based ai (the numberical value indicate a scaling factor for the number of search trials)
	void (Player::*input_func)();
	std::vector<Card*> card_pool; // used in AI simulation to substitute unknown cards, if empty then randomly generate using the default generator; note, the player object does not own the cards so don't deallocate in this class
};


/* Generator/Descriptor Section */


// Some codings of constraints/conditions on target types
#define TARGET_TYPE_NOTHING 0u
#define TARGET_TYPE_ANY 0xFFFFFFFFu
// target types with different fields
#define TARGET_POS_FIELD 0x1u					// 0b1
#define TARGET_POS_HAND	0x2u					// 0b10
#define TARGET_POS_DECK	0x4u					// 0b100
	#define TARGET_POS_HAND_OR_DECK	0x6u					// 0b110
	#define TARGET_ANY_POS_TYPE 0x7u						// 0b111
	#define TARGET_NOT_HAND	0x5u							// 0b101
	#define TARGET_NOT_DECK 0x3u							// 0b011
#define TARGET_IS_LEADER 0x8u					// 0b1000
#define TARGET_IS_MINION 0x10u					// 0b10000
	#define TARGET_ANY_CHAR 0x18u							// 0b11000
#define TARGET_IS_SPELL	0x20u					// 0b100000
	#define TARGET_ANY_CARD_TYPE 0x38u						// 0b111000
	#define TARGET_NOT_LEADER 0x30u							// 0b110000
	#define TARGET_NOT_MINION 0x28u							// 0b101000
#define TARGET_IS_ALLY 0x40u					// 0b1000000
#define TARGET_IS_OPPO 0x80u					// 0b10000000
	#define TARGET_ANY_ALLE_TYPE 0xC0u						// 0b11000000
	#define TARGET_ANY_CARD_ALLE_TYPE (TARGET_ANY_CARD_TYPE|TARGET_ANY_ALLE_TYPE)
#define TARGET_IS_BEAST 0x100u					// 0b100000000
#define TARGET_IS_DRAGON 0x200u					// 0b1000000000
#define TARGET_IS_DEMON 0x400u					// 0b10000000000
	#define TARGET_ANY_MINION_TYPE 0x700u					// 0b11100000000
	#define TARGET_NOT_BEAST 0x600u							// 0b11000000000
	#define TARGET_NOT_DRAGON 0x500u						// 0b10100000000
	#define TARGET_NOT_DEMON 0x300u							// 0b01100000000
	#define TARGET_ANY_ALLE_MINION_TYPE (TARGET_ANY_ALLE_TYPE|TARGET_ANY_MINION_TYPE)
	#define TARGET_ANY_CHAR_MINION_TYPE (TARGET_ANY_CHAR|TARGET_ANY_MINION_TYPE)
	#define TARGET_ANY_CARD_MINION_TYPE (TARGET_ANY_CARD_TYPE|TARGET_ANY_MINION_TYPE)
	#define TARGET_ANY_CARD_ALLE_MINION_TYPE (TARGET_ANY_CARD_TYPE|TARGET_ANY_ALLE_TYPE|TARGET_ANY_MINION_TYPE)
#define TARGET_IS_CHARGE 0x800u					// 0b100000000000
#define TARGET_IS_TAUNT 0x1000u					// 0b1000000000000
#define TARGET_IS_STEALTH 0x2000u 				// 0b10000000000000
#define TARGET_IS_UNTARGETABLE 0x4000u 			// 0b100000000000000
#define TARGET_IS_SHIELDED 0x8000u 				// 0b1000000000000000
#define TARGET_IS_POISONOUS 0x10000u 			// 0b10000000000000000
#define TARGET_IS_LIFESTEAL 0x20000u 			// 0b100000000000000000
	#define TARGET_ANY_ABIL_TYPE 0x3F800u 					// 0b111111100000000000
	#define	TARGET_NOT_CHARGE 0x3F000u 						// 0b111111000000000000
	#define	TARGET_NOT_TAUNT 0x3E800u 						// 0b111110100000000000
	#define TARGET_NOT_STEALTH 0x3D800u						// 0b111101100000000000
	#define TARGET_NOT_UNTARGETABLE 0x3B800u 				// 0b111011100000000000
	#define TARGET_NOT_SHIELDED 0x37800u 					// 0b110111100000000000
	#define TARGET_NOT_POISONOUS 0x2F800u 					// 0b101111100000000000
	#define TARGET_NOT_LIFESTEAL 0x1F800u 					// 0b011111100000000000
	#define TARGET_SPELL_ABIL 0x34000u						// 0b110100000000000000
	#define TARGETABLE_CHAR_ABIL 0x39800u					// 0b111001100000000000 (targeting one's own stealth minion is allowed but we do not include such dedicated targetting condition)
	#define TARGET_ANY_MINION_ABIL_TYPE (TARGET_ANY_MINION_TYPE|TARGET_ANY_ABIL_TYPE)
	#define TARGET_ANY_ALLE_ABIL_TYPE (TARGET_ANY_ALLE_TYPE|TARGET_ANY_ABIL_TYPE)
	#define TARGET_ANY_CARD_MINION_ABIL_TYPE (TARGET_ANY_CARD_TYPE|TARGET_ANY_MINION_TYPE|TARGET_ANY_ABIL_TYPE)
	#define TARGET_ANY_ALLE_MINION_ABIL_TYPE (TARGET_ANY_ALLE_TYPE|TARGET_ANY_MINION_TYPE|TARGET_ANY_ABIL_TYPE)
	#define TARGET_ANY_CARD_ALLE_MINION_ABIL_TYPE (TARGET_ANY_CARD_TYPE|TARGET_ANY_ALLE_TYPE|TARGET_ANY_MINION_TYPE|TARGET_ANY_ABIL_TYPE)
// filters (the aboves are usually combined using bitwise ors (as components), but these are combined using bitwise ands (as full flags))
#define FIELD_COND_FILTER (TARGET_POS_FIELD|TARGET_ANY_CHAR|TARGET_ANY_ALLE_MINION_ABIL_TYPE)
#define HAND_COND_FILTER (TARGET_POS_HAND|TARGET_ANY_CARD_ALLE_MINION_ABIL_TYPE)
#define DECK_COND_FILTER (TARGET_POS_DECK|TARGET_ANY_CARD_ALLE_MINION_ABIL_TYPE)
#define HAND_OR_DECK_COND_FILTER (TARGET_POS_HAND_OR_DECK|TARGET_ANY_CARD_ALLE_MINION_ABIL_TYPE)
#define NOT_HAND_COND_FILTER (TARGET_NOT_HAND|TARGET_ANY_CARD_ALLE_MINION_ABIL_TYPE)
#define NOT_DECK_COND_FILTER (TARGET_NOT_DECK|TARGET_ANY_CARD_ALLE_MINION_ABIL_TYPE)
#define LEADER_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_IS_LEADER|TARGET_ANY_ALLE_TYPE|TARGET_NOT_STEALTH)
#define MINION_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_IS_MINION|TARGET_ANY_ALLE_MINION_ABIL_TYPE)
#define CHAR_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_ANY_CHAR|TARGET_ANY_ALLE_MINION_ABIL_TYPE)
#define SPELL_COND_FILTER (TARGET_POS_HAND_OR_DECK|TARGET_IS_SPELL|TARGET_ANY_ALLE_TYPE|TARGET_SPELL_ABIL)
#define NOT_LEADER_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_NOT_LEADER|TARGET_ANY_ALLE_MINION_ABIL_TYPE)
#define ALLY_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_IS_ALLY|TARGET_ANY_CARD_MINION_ABIL_TYPE)
#define OPPO_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_IS_OPPO|TARGET_ANY_CARD_MINION_ABIL_TYPE)
#define FORCE_ANY_ALLE_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_ANY_CARD_MINION_ABIL_TYPE) // both bits being zero means forcing to not have allegiance constraints
#define BEAST_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_IS_MINION|TARGET_IS_BEAST|TARGET_ANY_ALLE_ABIL_TYPE)
#define DRAGON_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_IS_MINION|TARGET_IS_DRAGON|TARGET_ANY_ALLE_ABIL_TYPE)
#define DEMON_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_IS_MINION|TARGET_IS_DEMON|TARGET_ANY_ALLE_ABIL_TYPE)
#define NOT_BEAST_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_IS_MINION|TARGET_NOT_BEAST|TARGET_ANY_ALLE_ABIL_TYPE)
#define NOT_DRAGON_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_IS_MINION|TARGET_NOT_DRAGON|TARGET_ANY_ALLE_ABIL_TYPE)
#define NOT_DEMON_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_IS_MINION|TARGET_NOT_DEMON|TARGET_ANY_ALLE_ABIL_TYPE)
#define CHARGE_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_ANY_CHAR|TARGET_IS_CHARGE|TARGET_ANY_ALLE_MINION_TYPE)
#define TAUNT_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_ANY_CHAR|TARGET_IS_TAUNT|TARGET_ANY_ALLE_MINION_TYPE)
#define STEALTH_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_ANY_CHAR|TARGET_IS_STEALTH|TARGET_ANY_ALLE_MINION_TYPE)
#define UNTARGETABLE_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_ANY_CHAR|TARGET_IS_UNTARGETABLE|TARGET_ANY_ALLE_MINION_TYPE)
#define SHIELDED_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_ANY_CHAR|TARGET_IS_SHIELDED|TARGET_ANY_ALLE_MINION_TYPE)
#define POISONOUS_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_ANY_CHAR|TARGET_IS_POISONOUS|TARGET_ANY_ALLE_MINION_TYPE)
#define LIFESTEAL_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_ANY_CHAR|TARGET_IS_LIFESTEAL|TARGET_ANY_ALLE_MINION_TYPE)
#define NOT_CHARGE_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_ANY_CHAR|TARGET_NOT_CHARGE|TARGET_ANY_ALLE_MINION_TYPE) // this means charge is applicable but not present, similar for the below series about ability keywords
#define NOT_TAUNT_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_ANY_CHAR|TARGET_NOT_TAUNT|TARGET_ANY_ALLE_MINION_TYPE)
#define NOT_STEALTH_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_IS_MINION|TARGET_NOT_STEALTH|TARGET_ANY_ALLE_MINION_TYPE)
#define NOT_UNTARGETABLE_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_ANY_CARD_TYPE|TARGET_NOT_UNTARGETABLE|TARGET_ANY_ALLE_MINION_TYPE)
#define NOT_SHIELDED_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_ANY_CHAR|TARGET_NOT_SHIELDED|TARGET_ANY_ALLE_MINION_TYPE)
#define NOT_POISONOUS_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_ANY_CARD_TYPE|TARGET_NOT_POISONOUS|TARGET_ANY_ALLE_MINION_TYPE)
#define NOT_LIFESTEAL_COND_FILTER (TARGET_ANY_POS_TYPE|TARGET_ANY_CARD_TYPE|TARGET_NOT_LIFESTEAL|TARGET_ANY_ALLE_MINION_TYPE)
#define PLAY_CHAR_TARGET_FILTER (TARGET_POS_FIELD|TARGET_ANY_CHAR|TARGET_ANY_ALLE_MINION_TYPE|TARGETABLE_CHAR_ABIL)
#define PLAY_CARD_TARGET_FILTER (TARGET_POS_HAND|TARGET_ANY_CARD_TYPE|TARGET_IS_ALLY|TARGET_ANY_MINION_TYPE|TARGET_NOT_UNTARGETABLE)
#define PLAY_CHAR_SRC_FILTER (TARGET_POS_FIELD|TARGET_ANY_CHAR|TARGET_IS_ALLY|TARGET_ANY_MINION_ABIL_TYPE)
#define PLAY_SPELL_SRC_FILTER (TARGET_POS_HAND|TARGET_IS_SPELL|TARGET_IS_ALLY|TARGET_SPELL_ABIL)
#define DESTROY_SRC_FILTER FIELD_COND_FILTER

// target modes
#define TARGET_MODE_DEFAULT 0u
#define TARGET_MODE_EXIST 1u
#define TARGET_MODE_PLAY 2u
#define TARGET_MODE_SOURCE 3u
#define TARGET_MODE_LEADER 4u
#define TARGET_MODE_SELF 5u
#define TARGET_MODE_NEW 7u
#define TARGET_MODE_MOVE_DEST 7u
#define TARGET_MODE_COPY_DEST 8u
#define TARGET_MODE_NEW_DEST 9u
#define TARGET_MODE_WIN_GAME 10u

// effect timing
#define EFFECT_TIMING_DEFAULT 0u
#define EFFECT_TIMING_PLAY 1u
#define EFFECT_TIMING_DESTROY 2u
#define EFFECT_TIMING_DISCARD 3u
#define EFFECT_TIMING_TURN 4u // includes turn start and turn end


void IntersectRange(int& min_result, int& max_result, int min1, int max1, int min2, int max2);
void IntersectRangeInPlace(int& min_val, int& max_val, int other_min_val, int other_max_val);
void UnionRangeInPlace(int& min_val, int& max_val, int other_min_val, int other_max_val);

class CondConfig
{
public:
	CondConfig(unsigned _flag, int _min_mp, int _max_mp, int _min_max_mp, int _max_max_mp,
		int _min_cost, int _max_cost, int _min_atk, int _max_atk, int _min_hp, int _max_hp, int _min_n_atks, int _max_n_atks); // this is the general version
	CondConfig(unsigned _flag,
		int _min_cost, int _max_cost, int _min_atk, int _max_atk, int _min_hp, int _max_hp, int _min_n_atks, int _max_n_atks); // this is the version without card-independent settings
	CondConfig(unsigned _flag); // only with flag specified, all others are default
	CondConfig(); // default
	CondConfig& operator &= (const CondConfig& config);
	CondConfig& operator &= (unsigned flag);
	CondConfig& operator |= (const CondConfig& config);
	CondConfig& operator |= (unsigned flag);
	unsigned operator & (unsigned mask) const;
	unsigned operator | (unsigned mask) const;
	unsigned flag;
	int min_mp, max_mp; // only applicable for card-independent condition
	int min_max_mp, max_max_mp; // only applicable for card-independent condition
	int min_cost, max_cost;
	int min_atk, max_atk;
	int min_hp, max_hp;
	int min_n_atks, max_n_atks;
};

CondConfig GetInitConfigFromCard(const Card* card); // this does not consider the abilities and assume no abilities
CondConfig GetInstantConfigFromInitConfig(const CondConfig& config);
void AdjustInitConfigWithAnotherConfig(CondConfig& init_config, const CondConfig& instant_config);
CondConfig ExtractEffectIndependentConfig(const CondConfig& config);
CondConfig GetDefaultInitConfig(); // this version is for the initial state of the card, particularly, ability fields of the flag are zero bits
CondConfig GetDefaultConfig(); // just a constructor like function, due to artifact from GIGL
CondConfig GetFlagConfig(unsigned flag);
CondConfig GetMpConfig(int min_mp, int max_mp);
CondConfig GetMaxMpConfig(int min_max_mp, int max_max_mp);
CondConfig GetCostConfig(unsigned flag, int min_cost, int max_cost);
CondConfig GetAtkConfig(unsigned flag, int min_atk, int max_atk);
CondConfig GetHpConfig(unsigned flag, int min_hp, int max_hp);
CondConfig GetAtkTimesConfig(unsigned flag, int min_n_atks, int max_n_atks);

extern bool display_overheat_counts; // whether or not to display overheat counter in detailed discripition.

std::string MinionTypeDescription(int type);
std::string AbilityDescriptionInline(bool is_charge, bool is_taunt, bool is_stealth, bool is_untargetable, bool is_shielded, bool is_poisonous, bool is_lifesteal); // comma separated list, starting with a " with " if there's any ability
std::string AbilityDescriptionBrief(bool is_charge, bool is_taunt, bool is_stealth, bool is_untargetable, bool is_shielded, bool is_poisonous, bool is_lifesteal);
std::string AbilityDescriptionDetail(bool is_charge, bool is_taunt, bool is_stealth, bool is_untargetable, bool is_shielded, bool is_poisonous, bool is_lifesteal, int indent_size);


/* Simulator Section */

void GiglRandInit(unsigned seed); // artifact from file including issues
int GetGiglRandInt(); // artifact from file including issues
int GetGiglRandInt(int n); // artifact from file including issues
double GetGiglRandFloat(); // 0.0 ~ 1.0; artifact from file includeing issues
double GetGiglRandFloat(double max_val); // 0.0 ~ max_val; artifact from file including issues
double GetGiglRandFloat(double min_val, double max_val); // min_val ~ max_val; artifact from file including issues
std::vector<int> CreateRandomSelection(int n, int k); // select random k numbers from 0 ~ n-1 as a list (ordered randomly); artifact from file including issues
std::vector<int> CreateRandomSelectionSorted(int n, int k); // same as above but return guarantee sorted in increasing order 
Card* GenerateSingleCard(int seed);
Card* GenerateSingleCardWithCost(int cost, int seed);
std::string GetCardName(Card* card); // artifact from file including issues
std::string GetCardBrief(Card* card); // artifact from file including issues
std::string GetCardDetail(Card* card); // artifact from file including issues
int GetCardOrigCost(Card* card); // artifact from file including issues
std::vector<Card*> GenerateCardSet(int n, int seed); // giving no duplicate card seeds
std::vector<int> GenerateCardSetSeeds(int n, int seed); // giving no duplicate card seeds
void InitMatch(std::vector<Card*>& card_list, const std::vector<int>& deck_a_orig_indices, const std::vector<int>& deck_b_orig_indices, std::vector<int>& deck_a_shuffle, std::vector<int>& deck_b_shuffle, std::vector<int>& deck_a_indices, std::vector<int>& deck_b_indices, std::vector<Card*>& deck_a, std::vector<Card*>& deck_b); // shuffle the card indices, the "shuffle" vectors pass back how the deck is ordered using indices in the deck, the "indices" vectors pass back the indices in the card pool
void DecidePlayOrder(Player* player1, Player* player2, Player*& first_player, Player*& second_player);
Card* HardCopyCard(Card* card); // artifact from file including issues
std::vector<Card*> HardCopyCards(std::vector<Card*> cards); // artifact from file including issues; not using & for the argument type because of some const/non-const issues
void DeleteCard(Card* card); // artifact from file including issues
void DeleteCards(std::vector<Card*>& cards); // artifact from file including issues

std::vector<Card*> CreateDemoCardSet1();
std::vector<Card*> CreateDemoCardSet1Alt();
std::vector<Card*> CreateDemoCardSet1Alt2();
std::vector<Card*> CreateDemoCardSet2();
std::vector<Card*> CreateDemoCardSet2Alt();
std::vector<Card*> CreateDemoCardSet3();
std::vector<Card*> CreateDemoCardSet4();
std::vector<Card*> CreateDemoCardSet4Alt();
std::vector<Card*> CreateDemoCardSet5();
std::vector<Card*> CreateDemoCardSet6();
std::vector<Card*> CreateDemoCardSet7();
std::vector<Card*> CreateDemoCardSet8();
std::vector<Card*> CreateDemoCardSetX();
std::vector<Card*> CreateDemoCardSetH();

class DeferredEvent // certain parts of effects are not applied immediately but rather pushed into a queue and dealt with afterwards, this is because we don't want inserted events to AoE effects, and also sometimes we want to maintain target indexing unchanged until the effects on one card at a certain point is fully executed
{
public:
	DeferredEvent(Card* _card, bool is_owning_card, bool _start_of_batch);
	virtual ~DeferredEvent();
	virtual void Process(Player* curr_player) = 0;

protected:
	Card* card;
	bool is_owning_card; // certain type of event owns the card (i.e. the address of the card is not stored anywhere else) before resolved, during which period object has to manage the memory

public:
	bool is_start_of_batch;
};

class DestroyEvent : public DeferredEvent
{
public:
	DestroyEvent(Card* _card, bool _start_of_batch);
	void Process(Player* curr_player);
};

class CastEvent : public DeferredEvent // currently not actually needed as there isn't anything special to do after a spell is cast
{
public:
	CastEvent(Card* _card, bool _start_of_batch);
	void Process(Player* curr_player);
};

class DiscardEvent : public DeferredEvent
{
public:
	DiscardEvent(Card* _card, bool _start_of_batch);
	void Process(Player* curr_player);
};

class FieldSummonEvent : public DeferredEvent
{
public:
	FieldSummonEvent(Card* _card, bool _start_of_batch, Player* _owner);
	void Process(Player* curr_player);

private:
	Player* owner;
};

class HandPutEvent : public DeferredEvent
{
public:
	HandPutEvent(Card* _card, bool _start_of_batch, Player* _owner);
	void Process(Player* curr_player);

private:
	Player* owner;
};

class DeckShuffleEvent : public DeferredEvent
{
public:
	DeckShuffleEvent(Card* _card, bool _start_of_batch, Player* _owner);
	void Process(Player* curr_player);

private:
	Player* owner;
};

class CardTransformEvent : public DeferredEvent
{
public:
	CardTransformEvent(Card* _card, bool _start_of_batch, Card* _replacement);
	~CardTransformEvent();
	void Process(Player* curr_player);

private:
	Card* replacement;
	bool is_owning_replacement; // owns the replacement until resolved
};

class CardResetEvent : public DeferredEvent // currently not actually needed as there isn't anything special to do after a card is reset to its original state
{
public:
	CardResetEvent(Card* _card, bool _start_of_batch);
	void Process(Player* curr_player);
};


/* AI Section */

class ActionEntity
{
public:
	ActionEntity();
	virtual ~ActionEntity(); // virtual destructor needed for polymorphism even none of the derived class (and the base class) destructor needs to do anything special, because otherwise the deallocation size will be messed up
	virtual bool CheckValid(const Player* player) const = 0;
	virtual void PerformAction(Player* player) const = 0;
};

class PlayAction : public ActionEntity
{
public:
	PlayAction(int _src, int _pos, int _des);
	bool CheckValid(const Player* player) const;
	void PerformAction(Player* player) const;

private:
	int src;
	int pos;
	int des;	
};

class AttackAction : public ActionEntity
{
public:
	AttackAction(int _src, int _des);
	bool CheckValid(const Player* player) const;
	void PerformAction(Player* player) const;

private:
	int src;
	int des;
};

class EndTurnAction : public ActionEntity
{
public:
	EndTurnAction();
	bool CheckValid(const Player* player) const;
	void PerformAction(Player* player) const;
};

class ActionSetEntity // for the set of valid actions for to play/attack with a specific card, or end turn
{
public:
	ActionSetEntity();
	virtual ~ActionSetEntity();
	void UnownActions(); // set this object to not managing the memory of the actions, used when the actions are passed to other places without passing this object
	virtual int CreateValidSet(const Player* player) = 0; // return the size of the set of valid actions
	virtual void PerformRandomAction(Player* player) const = 0;
	std::vector<ActionEntity*> action_set;
	bool is_owning_actions;
};

class PlayActionSet : public ActionSetEntity
{
public:
	PlayActionSet(int _card_index);
	int CreateValidSet(const Player* player);
	void PerformRandomAction(Player* player) const;

private:
	int card_index;
};

class AttackActionSet : public ActionSetEntity
{
public:
	AttackActionSet(int _card_index);
	int CreateValidSet(const Player* player);
	void PerformRandomAction(Player* player) const;

private:
	int card_index;
};

class EndTurnActionSet : public ActionSetEntity
{
public:
	EndTurnActionSet();
	int CreateValidSet(const Player* player);
	void PerformRandomAction(Player* player) const;
};

class KnowledgeActionNode
{
public:
	KnowledgeActionNode(const ActionEntity* _action); // it does not need a destructor to delete the action as it will be managed by the ActionSetEntity that owns it anyway
	const ActionEntity* GetAction() const;
	double TestAction(Player* player); // do a single test of action (try a single trajectory), return the heuristic evaluation at the end of the simulated turn
	int num_visits;
	double sum_eval;
	double ave_eval;

private:
	const ActionEntity* action;
};

class KnowledgeState
{
public:
	KnowledgeState(Player* _player, std::queue<DeferredEvent*>& event_queue, PtrRedirMap& redir_map);
	~KnowledgeState();
	const ActionEntity* DecideAction() const; // decide the action to peform after testing/searching
	double TestAction(Player* player); // do a single test of action (try a single trajectory), return the heuristic evaluation at the end of the simulated turn
	void PerformAction(); // test (with a biased tree search) and execute the action of choice (optimal action)
	int num_visits;

private:
	std::vector<ActionEntity*> action_list; // need this because it needs to manage the memory of those actions
	std::vector<KnowledgeActionNode*> action_nodes;
	int num_tests_scaling; // a scaling factor for number of trials (the number of trials is also related to the number of legal actions)
	Player* player;
	Player* opponent;
};


/* Formatted Print Section */

class DescToken
{
public:
	virtual ~DescToken();
	virtual void Append(std::string& script, int& row_index, int& column_index) const = 0; // this version simply append the content, not adding spacing, not checking for line switch etc.
	virtual bool StartLine(std::string& script, int& row_index, int& column_index) const = 0; // return whether it results in a start of new line (only true when there's empty lines, which shouldn't happen, but just in case)
	virtual bool AddToLine(std::string& script, int& row_index, int& column_index, int max_line_len) const = 0; // return whether it results in a start of new line (mostly caused by an explicit new line)
};

typedef std::vector<DescToken*> DescTokenVec;

class WordToken : public DescToken
{
public:
	WordToken(const std::string& _lexeme, int _curr_indent, bool _is_bold);
	void Append(std::string& script, int& row_index, int& column_index) const;
	bool StartLine(std::string& script, int& row_index, int& column_index) const;
	bool AddToLine(std::string& script, int& row_index, int& column_index, int max_line_len) const;

private:
	std::string lexeme;
	int curr_indent; // current indentation level
	bool is_bold;
};

WordToken* mkWordToken(const std::string& lexme, int curr_indent, bool is_bold);

class PunctToken : public DescToken // note: currently only consider punctuations on the right side of a word (i.e. not considering left paren etc.)
{
public:
	PunctToken(const std::string& _lexeme, int _curr_indent, bool _is_bold);
	void Append(std::string& script, int& row_index, int& column_index) const;
	bool StartLine(std::string& script, int& row_index, int& column_index) const;
	bool AddToLine(std::string& script, int& row_index, int& column_index, int max_line_len) const;

private:
	std::string lexeme;
	int curr_indent; // current indentation level
	bool is_bold;
};

PunctToken* mkPunctToken(const std::string& lexeme, int curr_indent, bool is_bold);

class NewLineToken : public DescToken // note: do not add a new line token at the end of the last line
{
public:
	NewLineToken();
	void Append(std::string& script, int& row_index, int& column_index) const;
	bool StartLine(std::string& script, int& row_index, int& column_index) const; // not used unless there's somehow empty lines (which shouldn't happen)
	bool AddToLine(std::string& script, int& row_index, int& column_index, int max_line_len) const;
};

NewLineToken* mkNewLineToken();

std::string SingleCardHtmlTableScript(Card* card);

void AbilityFillTokens(bool is_charge, bool is_taunt, bool is_stealth, bool is_untargetable, bool is_shielded, bool is_poisonous, bool is_lifesteal, DescTokenVec& tokens, int indent_size, int& need_starting_newline); // the int is intended to be bool but due to some GIGL artifacts, we make it int 
void AbilityFillTokensInline(bool is_charge, bool is_taunt, bool is_stealth, bool is_untargetable, bool is_shielded, bool is_poisonous, bool is_lifesteal, DescTokenVec& tokens, int indent_size);


/* Machine Learning Section */

void GetCardRep(Card* card, CardRep& card_rep);
void GetCardsReps(std::vector<Card*>& card_list, std::vector<CardRep>& card_reps);
Card* CreateCardFromRep(const std::string& name, CardRep& card_rep); // card_rep should also be const but the argument type in subsequent function call is a little nasty to deal with...(didn't get reference to const pointer work)
std::vector<Card*> CreateCardsFromReps(std::vector<CardRep>& card_reps); // uses "Anonymous" as the names
Card* GenerateNamedCardWithCost(const std::string& name, int cost);
Card* GenerateNamedMinionWithCost(const std::string& name, int cost);


// More neural network source are in other source files
