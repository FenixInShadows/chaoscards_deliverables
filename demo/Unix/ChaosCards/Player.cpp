#include "Player.h"
#include "card.generated.h"

#include <iostream>
#include <algorithm>
#include <cmath>
#include <numeric>

using namespace std;

// #define SUPPRESS_TOURNAMENT_PROMPTS
// #define SUPPRESS_MATCH_PROMPTS


/* Card/Player section */


Player::Player(queue<DeferredEvent*>& _event_queue) :
	leader(nullptr), field(), hand(), deck(), name(""), is_guest(true), is_exploration(false), opponent(nullptr), is_lost(false), is_turn_active(false),
	turn_num(0), max_mp(0), mp_loss(0), fatigue(0), field_size_adjust(0), hand_size_adjust(0), deck_size_adjust(0), event_queue(_event_queue), ai_level(0), input_func(&Player::TakeInputs), card_pool()
{
}

Player::Player(const string& _name, int _hp, const vector<Card*>& _deck, bool _is_guest, queue<DeferredEvent*>& _event_queue) :
	field(), hand(), deck(_deck), name(_name), is_guest(_is_guest), is_exploration(false), opponent(nullptr), is_lost(false), is_turn_active(false),
	turn_num(0), max_mp(0), mp_loss(0), fatigue(0), field_size_adjust(0), hand_size_adjust(0), deck_size_adjust(0), event_queue(_event_queue), ai_level(0), input_func(&Player::TakeInputs), card_pool()
{
	leader = CreateDefaultLeader(_hp);
	leader->card_pos = CARD_POS_AT_LEADER;
	for (auto it = deck.begin(); it != deck.end(); it++)
		(*it)->card_pos = CARD_POS_AT_DECK;
}

Player::Player(const string & _name, int _hp, const vector<Card*>& _deck, bool _is_guest, queue<DeferredEvent*>& _event_queue, int _ai_level) : Player(_name, _hp, _deck, _is_guest, _event_queue)
{
	ai_level = _ai_level;
	if (ai_level > 2) // in current single step look forward setting, there's little gain to have ai_level to be more than 2
		ai_level = 2;
	else if (ai_level < 0)
		ai_level = 0;

	if (ai_level == 0)
		input_func = &Player::TakeRandomAIInputs;
	else
		input_func = &Player::TakeSearchAIInputs;
}

Player::Player(const string & _name, int _hp, const vector<Card*>& _deck, bool _is_guest, queue<DeferredEvent*>& _event_queue, int _ai_level, const vector<Card*>& _card_pool) : Player(_name, _hp, _deck, _is_guest, _event_queue, _ai_level)
{
	card_pool = _card_pool;
}

Player::~Player()
{
	delete leader; // leader is never moved so don't need to check
	for (auto it = field.begin(); it != field.end(); it++)
		if (*it) // moved cards/minions may temperorily become nullptr
			delete (*it);
	for (auto it = hand.begin(); it != hand.end(); it++)
		if (*it) // moved cards/minions may temperorily become nullptr
			delete (*it);
	for (auto it = deck.begin(); it != deck.end(); it++)
		if (*it) // moved cards/minions may temperorily become nullptr
			delete (*it);
}

Card* Player::GetRandomUnknownCard() const
{
	int card_pool_size = card_pool.size();
	if (card_pool_size > 0)
		return HardCopyCard(card_pool[GetRandInt(card_pool_size)]);
	else
		return GenerateCard(GetRandInt());
}

Player* Player::CreateKnowledgeCopy(unsigned mode, queue<DeferredEvent*>& event_queue, PtrRedirMap& redir_map) const
{
	Player* new_player = new Player(event_queue);

	// leader, field, hand, and deck
	new_player->leader = leader->CreateHardCopy(redir_map);
	for (auto it = field.begin(); it != field.end(); it++)
	{
		Card* tmp_card = *it;
		new_player->field.push_back(tmp_card->CreateHardCopy(redir_map));
	}
	for (auto it = hand.begin(); it != hand.end(); it++)
	{
		Card* tmp_card = *it;
		if (mode == COPY_OPPO)
			new_player->hand.push_back(GetRandomUnknownCard());
		else
			new_player->hand.push_back(tmp_card->CreateHardCopy(redir_map));
	}
	for (auto it = deck.begin(); it != deck.end(); it++)
	{
		new_player->deck.push_back(GetRandomUnknownCard());
	}

	// other status, note: do not need to assign the opponent as the opponent must also be copied in order for exploration to work (after they are both copied they the opponent pointers needs to be set to the copy of each other)
	new_player->name = name;
	new_player->is_guest = is_guest;
	new_player->is_exploration = true;
	new_player->is_lost = is_lost;
	new_player->is_turn_active = is_turn_active;
	new_player->turn_num = turn_num;
	new_player->max_mp = max_mp;
	new_player->mp_loss = mp_loss;
	new_player->fatigue = fatigue;
	new_player->field_size_adjust = field_size_adjust;
	new_player->hand_size_adjust = hand_size_adjust;
	new_player->deck_size_adjust = deck_size_adjust;
	new_player->card_pool = card_pool;

	return new_player;
}

void Player::RegisterCardContributions(vector<int>& counters, vector<int>& cost_constrained_counters)
{
	auto counter_it = counters.begin();
	auto alt_counter_it = cost_constrained_counters.begin();
	for (auto it = deck.begin(); it != deck.end() && counter_it != counters.end() && alt_counter_it != cost_constrained_counters.end(); it++, counter_it++, alt_counter_it++)
		(*it)->RegisterContribution(&(*counter_it), &(*alt_counter_it));
}

void Player::SetAllCardAfflications()
{
	SetCardAfflication(leader);
	for (auto it = field.begin(); it != field.end(); it++)
		SetCardAfflication(*it);
	for (auto it = hand.begin(); it != hand.end(); it++)
		SetCardAfflication(*it);
	for (auto it = deck.begin(); it != deck.end(); it++)
		SetCardAfflication(*it);
}

void Player::SetCardAfflication(Card* card)
{
	card->SetAffiliation(this);
}

void Player::InitialCardDraw(bool is_second_player)
{
	bool start_of_batch = true;
	for (int i = 0; i < INIT_HAND_SIZE && !deck.empty(); i++)
	{
		// skip spots/cards queued for deletion
		vector<Card*>::iterator it;
		if (!deck.empty())
		{
			it = deck.end() - 1;
			while (it != deck.begin() && (!(*it) || (*it)->is_dying))
				it--;
		}
		else
			break;

		// no fatigue for initial draw (not happening for a normal rule anyway)
		if (deck.empty() || ((*it) && !(*it)->is_dying))
		{
			Card* card = *it;
			deck.erase(it);
			FlagHandPut(card, start_of_batch);
			start_of_batch = false;
		}
	}

	if (is_second_player)
	{
		Card* token = CreateSndPlayerToken();
		FlagHandPut(token, start_of_batch);
		SetCardAfflication(token);
	}

	CleanUp();
}

bool Player::CleanUp()
{
	if (CheckGameEnd()) // check for possible game end from some special win condition beyond leader health smaller or equal to zero
	{
		// make sure to clear corpse so that when printing the final board, the nullptrs caused by moved cards doesn't mess up
		ClearCorpse();
		opponent->ClearCorpse();
		return true;
	}

	if (ProcessDeferredEvents()) // leader destruction is checked here
	{
		// make sure to clear corpse so that when printing the final board, the nullptrs caused by moved cards doesn't mess up
		ClearCorpse();
		opponent->ClearCorpse();
		return true;
	}

	ClearCorpse();
	opponent->ClearCorpse();

	return false;
}

bool Player::ProcessDeferredEvents()
{
	while (!event_queue.empty())
	{		
		DeferredEvent* event = event_queue.front();
		event_queue.pop();
		event->Process(this);
		delete event;
		if (CheckGameEnd())
		{
			while (!event_queue.empty()) // check the remaining queue until the end of current batch for potential draw for the match
			{
				DeferredEvent* tmp_event = event_queue.front();
				if (tmp_event->is_start_of_batch)
					break;
				event_queue.pop();
				tmp_event->Process(this);
				delete tmp_event;
			}
			return true;
		}
	}

	return CheckGameEnd();
}

void Player::ClearCorpse()
{
	// leader (only check for resetting)
	if (leader->is_resetting)
	{
		leader->ClearExtraEffects();
		leader->is_resetting = false;
	}

	// field
	for (int i = field.size() - 1; i >= 0; i--)
	{
		Card* tmp_card = field[i];
		if (!tmp_card) // for moved minions
		{
			field.erase(field.begin() + i);
			field_size_adjust++;
		}
		else if (tmp_card->is_resetting) // a dying card cannot have its state reset (or affected by any effects), so if it is both resetting and dying it must be reset first, so we check for resetting first
		{
			tmp_card->ClearExtraEffects();
			tmp_card->is_resetting = false; // don't forget to set it back
		}
		else if (tmp_card->is_dying) // for transformed/destroyed/discarded minions
		{
			if (tmp_card->replacement) // transformed
			{
				field[i] = tmp_card->replacement;
				tmp_card->replacement = nullptr; // release the card's ownership of the replacement is it is already on the field
				delete tmp_card;
			}
			else // destroyed/discarded
			{
				field.erase(field.begin() + i);
				delete tmp_card;
				field_size_adjust++;
			}
		}
	}

	// hand
	for (int i = hand.size() - 1; i >= 0; i--)
	{
		Card* tmp_card = hand[i];
		if (!tmp_card) // for moved cards
		{
			hand.erase(hand.begin() + i);
			hand_size_adjust++;
		}
		else if (tmp_card->is_resetting) // a dying card cannot have its state reset (or affected by any effects), so if it is both resetting and dying it must be reset first, so we check for resetting first
		{
			tmp_card->ClearExtraEffects();
			tmp_card->is_resetting = false; // don't forget to set it back
		}
		else if (tmp_card->is_dying) // for transformed/cast/discarded cards
		{
			if (tmp_card->replacement) // transformed
			{
				hand[i] = tmp_card->replacement;
				tmp_card->replacement = nullptr; // release the card's ownership of the replacement is it is already at the hand
				delete tmp_card;
			}
			else // cast/discarded
			{
				hand.erase(hand.begin() + i);
				delete tmp_card;
				hand_size_adjust++;
			}
		}
	}

	// deck
	for (int i = deck.size() - 1; i >= 0; i--)
	{
		Card* tmp_card = deck[i];
		if (!tmp_card) // for moved cards
		{
			deck.erase(deck.begin() + i);
			deck_size_adjust++;
		}
		else if (tmp_card->is_resetting) // a dying card cannot have its state reset (or affected by any effects), so if it is both resetting and dying it must be reset first, so we check for resetting first
		{
			tmp_card->ClearExtraEffects();
			tmp_card->is_resetting = false; // don't forget to set it back
		}
		else if (tmp_card->is_dying) // for transformed/discarded cards
		{
			if (tmp_card->replacement) // transformed
			{
				deck[i] = tmp_card->replacement;
				tmp_card->replacement = nullptr; // release the card's ownership of the replacement is it is already at the deck
				delete tmp_card;
			}
			else // discarded
			{
				deck.erase(deck.begin() + i);
				delete tmp_card;
				deck_size_adjust++;
			}
		}
	}
}

bool Player::CheckGameEnd()
{
	if (CheckLose() || opponent->CheckLose())
	{
		// don't actually need to print board here, as game ended must because of turn start, turn end or other actions, where the board will be printed
		// if were printing board make sure to CleanUp before doing so to avoid nullptr issues
		is_turn_active = false;
		return true;
	}

	return false;
}

void Player::FlagDestroy(Card* card, bool start_of_batch)
{
	card->is_dying = true;
	event_queue.push(new DestroyEvent(card, start_of_batch));
	field_size_adjust--;
}

void Player::FlagCastSpell(Card* card, bool start_of_batch)
{
	card->is_dying = true;
	event_queue.push(new CastEvent(card, start_of_batch));
	hand_size_adjust--;
}

void Player::FlagFieldDiscard(Card* card, bool start_of_batch)
{
	card->is_dying = true;
	event_queue.push(new DiscardEvent(card, start_of_batch));
	field_size_adjust--;
}

void Player::FlagHandDiscard(Card* card, bool start_of_batch)
{
	card->is_dying = true;
	event_queue.push(new DiscardEvent(card, start_of_batch));
	hand_size_adjust--;
}

void Player::FlagDeckDiscard(Card* card, bool start_of_batch)
{
	card->is_dying = true;
	event_queue.push(new DiscardEvent(card, start_of_batch));
	deck_size_adjust--;
}

void Player::FlagFieldSummon(Card* card, bool start_of_batch)
{
	event_queue.push(new FieldSummonEvent(card, start_of_batch, this)); // the event will add it to the field, no matter if it is full or not (if it is full then it'll be discarded immediately with the next event)
	field_size_adjust++;
	if (GetActualFieldSize() > MAX_FIELD_SIZE) // if field is full, also issue a discard event
	{
		#ifndef SUPPRESS_MATCH_PROMPTS
		if (!is_exploration)
			cout << name << "'s field is full." << endl;
		#endif
		FlagFieldDiscard(card, false); // the discard caused by a card movement is considered in the same batch as the movement
	}
}

void Player::FlagHandPut(Card* card, bool start_of_batch)
{
	event_queue.push(new HandPutEvent(card, start_of_batch, this)); // the event will add it to the hand, no no matter if it is full or not (if it is full then it'll be discarded immediately with the next event)
	hand_size_adjust++;
	if (GetActualHandSize() > MAX_HAND_SIZE) // if hand is full, also issue a discard event
	{
		#ifndef SUPPRESS_MATCH_PROMPTS
		if (!is_exploration)
			cout << name << "'s hand is full." << endl;
		#endif
		FlagHandDiscard(card, false); // the discard caused by a card movement is considered in the same batch as the movement
	}
}

void Player::FlagDeckShuffle(Card* card, bool start_of_batch)
{
	event_queue.push(new DeckShuffleEvent(card, start_of_batch, this));
	deck_size_adjust++;
}

void Player::FlagCardTransform(Card* card, bool start_of_batch, Card* replacement)
{
	card->is_dying = true;
	event_queue.push(new CardTransformEvent(card, start_of_batch, replacement));
}

void Player::FlagCardReset(Card* card, bool start_of_batch)
{
	card->is_resetting = true;
	event_queue.push(new CardResetEvent(card, start_of_batch));
}

void Player::SetLose()
{
	is_turn_active = false;
	is_lost = true;
}

bool Player::CheckLose() const
{
	return is_lost;
}

void Player::StartTurn()
{
	turn_num++;

	#ifndef SUPPRESS_MATCH_PROMPTS
	if (!is_exploration)
		cout << name << "\'s Turn " << turn_num << " starts." << endl << endl;
	#endif

	if (max_mp < MAX_MP)
		max_mp += 1;
	mp_loss = 0;

	// clear consumed attack times, also turn off the is_first_turn_at_field flag
	RecoverAttackTimes(); // needs to happen before turn starting effects (otherwise newly spawned/moved cards) may be allowed to attack immediately when they should not be

	// turn starting effects
	int total_cards = 2 + field.size() + hand.size() + deck.size() + opponent->field.size() + opponent->hand.size() + opponent->deck.size();
	vector<Card*> tmp_card_list; // create a temp list so that cards newly added by effects during the process does not affect the indexing (and are not considered for triggering turn start effects)
	for (int i = 0; i < total_cards; i++)
		tmp_card_list.push_back(GetTargetCard(i));
	for (int i = 0; i < total_cards; i++)
	{
		Card* card = tmp_card_list[i];
		if (card && !card->is_dying)
			card->TurnStart(leader);
		if (ProcessDeferredEvents()) return;
	}
	if (CleanUp()) return; // can't cleanup in the loop because the effects may destroy minions/discard cards

	// draw card at the start of turn
	DrawCard(true);
	if (CleanUp()) return;

	#ifndef SUPPRESS_MATCH_PROMPTS
	if (!is_exploration)
		PrintBoard();
	#endif

	is_turn_active = true;
}

void Player::EndTurn()
{
	is_turn_active = false;

	// turn ending effects
	int total_cards = 2 + field.size() + hand.size() + deck.size() + opponent->field.size() + opponent->hand.size() + opponent->deck.size();
	vector<Card*> tmp_card_list; // create a temp list so that cards newly added by effects during the process does not affect the indexing (and are not considered for triggering turn end effects)
	for (int i = 0; i < total_cards; i++)
		tmp_card_list.push_back(GetTargetCard(i));
	for (int i = 0; i < total_cards; i++)
	{
		Card* card = tmp_card_list[i];
		if (card && !card->is_dying)
			card->TurnEnd(leader);
		if (ProcessDeferredEvents()) return;
	}
	if (CleanUp()) return; // can't cleanup in the loop because the effects may destroy minions/discard cards

	// clear overheat counters, allied cards only (should be after turn ending effects)
	leader->SetAllOverheatCounts(0);
	for (int i = 0; i < field.size(); i++)
		field[i]->SetAllOverheatCounts(0);
	for (int i = 0; i < hand.size(); i++)
		hand[i]->SetAllOverheatCounts(0);
	for (int i = 0; i < deck.size(); i++)
		deck[i]->SetAllOverheatCounts(0);

	#ifndef SUPPRESS_MATCH_PROMPTS
	if (!is_exploration)
	{
		PrintBoard();
		cout << "Turn ends." << endl << endl;
	}
	#endif

	// force draw if maximum number of turns has been reached
	if (turn_num >= MAX_NUM_TURNS && opponent->turn_num >= MAX_NUM_TURNS)
	{
		#ifndef SUPPRESS_MATCH_PROMPTS
		if (!is_exploration)
			cout << "Maximun number of turns reached." << endl;
		#endif
		SetLose();
		opponent->SetLose();
	}
}

bool Player::CheckPlayValid(int x, int y, int& z) const
{
	if (x == z)
	{
		#ifndef SUPPRESS_MATCH_PROMPTS
		if (!is_exploration && !is_guest)
			cout << "Error: Cannot target itself." << endl << endl;
		#endif
		return false;
	}

	int i = x - (field.size() + opponent->field.size() + 2);
	if (i >= hand.size())
	{
		#ifndef SUPPRESS_MATCH_PROMPTS
		if (!is_exploration && !is_guest)
			cout << "Error: Invalid card numbering." << endl << endl;
		#endif
		return false;
	}

	Card* card = hand[i];
	if (!CheckMP(card->mana))
	{
		#ifndef SUPPRESS_MATCH_PROMPTS
		if (!is_exploration && !is_guest)
			cout << "Error: Not enough MP." << endl << endl;
		#endif
		return false;
	}

	// validate target, note that for targets of minion and hero card, if there are no valid target at all, it will return true but z will be set to -1 (target ignored and effect not triggered), z is passed by reference
	if (!card->CheckPlayValid(x, y, z))
		return false;

	return true;
}

void Player::Play(int x, int y, int z)
{
	int i = x - (field.size() + opponent->field.size() + 2);
	Card* card = hand[i];
	
	UseMP(card->mana);
	if (card->card_type != SPELL_CARD)
		hand.erase(hand.begin() + i);
	card->Play(x, y, z);	

	CleanUp();

	#ifndef SUPPRESS_MATCH_PROMPTS
	if (!is_exploration)
		PrintBoard();
	#endif
}

bool Player::CheckAttackValid(int x, int z) const
{
	if (x < 0 || x > field.size())
	{
		#ifndef SUPPRESS_MATCH_PROMPTS
		if (!is_exploration && !is_guest)
			cout << "Error: Invalid attacker numbering." << endl << endl;
		#endif
		return false;
	}

	Card* card = (x == 0 ? leader : field[x - 1]);
	if (!card->CheckAttackValid(x, z))
		return false;

	return true;
}

void Player::Attack(int x, int z)
{
	Card* card = (x == 0 ? leader : field[x - 1]);

	card->Attack(x, z);

	CleanUp();

	#ifndef SUPPRESS_MATCH_PROMPTS
	if (!is_exploration)
		PrintBoard();
	#endif
}

void Player::DrawCard(bool start_of_batch)
{
	// skip spots/cards queued for deletion
	vector<Card*>::iterator it;
	if (!deck.empty())
	{
		it = deck.end() - 1;
		while (it != deck.begin() && (!(*it) || (*it)->is_dying))
			it--;
	}

	if (deck.empty() || !(*it) || (*it)->is_dying)
	{
		fatigue++;
		#ifndef SUPPRESS_MATCH_PROMPTS
		if (!is_exploration)
			cout << "Suffering fatigue: " << fatigue << "." << endl << endl;
		#endif
		leader->TakeDamage(fatigue, nullptr, true);
		return;
	}
	
	// add to hand
	#ifndef SUPPRESS_MATCH_PROMPTS
	if (!is_exploration)
		cout << name << " draws the card " << (*it)->name << "." << endl;
	#endif
	Card* card = *it;
	deck.erase(it);
	FlagHandPut(card, start_of_batch);
}

void Player::RecoverAttackTimes()
{
	leader->n_atks_loss = 0;
	leader->is_first_turn_at_field = false;
	for (auto it = field.begin(); it != field.end(); it++)
	{
		(*it)->n_atks_loss = 0;
		(*it)->is_first_turn_at_field = false;
	}
}

bool Player::CheckMP(int cost) const
{
	return mp_loss + cost <= max_mp;
}

void Player::UseMP(int cost)
{
	mp_loss += cost;
}

void Player::ModifyMp(int amount)
{
	#ifndef SUPPRESS_MATCH_PROMPTS
	if (!is_exploration)
		cout << abs(amount) << " MP" << (amount >= 0 ? " restored to " : " exhausted from ") << name << endl;
	#endif
	if (mp_loss < amount)
	{
		#ifndef SUPPRESS_MATCH_PROMPTS
		if (!is_exploration)
			cout << "Actually only restored " << mp_loss << "." << endl;
		#endif
		mp_loss = 0;
	}
	else if (mp_loss > max_mp + amount)
	{
		#ifndef SUPPRESS_MATCH_PROMPTS
		if (!is_exploration)
			cout << "Actually only exhausted " << max_mp - mp_loss << "." << endl;
		#endif
		mp_loss = max_mp;
	}
	else
	{
		mp_loss -= amount;
	}
}

void Player::ModifyMaxMp(int amount)
{
	#ifndef SUPPRESS_MATCH_PROMPTS
	if (!is_exploration)
		cout << abs(amount) << " Max MP (and MP)" << (amount >= 0 ? " added to " : " lost from ") << name << endl;
	#endif
	if (max_mp < -amount)
	{
		#ifndef SUPPRESS_MATCH_PROMPTS
		if (!is_exploration)
		{
			cout << "Actually only lost " << max_mp << " Max MP." << endl;
			cout << "Actually only lost " << max_mp - mp_loss << " MP" << endl;
		}
		#endif
		max_mp = 0;
		mp_loss = 0;
	}
	else if (max_mp > MAX_MP - amount)
	{
		#ifndef SUPPRESS_MATCH_PROMPTS
		if (!is_exploration)
			cout << "Actually only added " << MAX_MP - max_mp << " Max MP." << endl;
		#endif
		int mp_loss_correction = max_mp + amount - MAX_MP;
		if (mp_loss < mp_loss_correction)
		{
			#ifndef SUPPRESS_MATCH_PROMPTS
			if (!is_exploration)
				cout << "Actually only added " << MAX_MP - max_mp + mp_loss << " MP." << endl;
			#endif
			mp_loss = 0;
		}
		else
		{
			mp_loss -= mp_loss_correction;
		}
		max_mp = MAX_MP;
	}
	else
	{
		if (max_mp - mp_loss < -amount)
		{
			#ifndef SUPPRESS_MATCH_PROMPTS
			if (!is_exploration)
				cout << "Actually only lost " << max_mp - mp_loss << " MP." << endl;
			#endif
			mp_loss = max_mp + amount;
		}
		max_mp += amount;
	}
}

bool Player::IsValidTarget(int z) const
{
	return z >= 0 && z <= field.size() + opponent->field.size() + hand.size() + 1;
}

bool Player::IsValidCharTarget(int z) const
{
	return z >= 0 && z <= field.size() + opponent->field.size() + 1;
}

bool Player::IsValidMinionTarget(int z) const
{
	return z > 0 && z <= field.size() + opponent->field.size();
}

bool Player::IsValidCardTarget(int z) const
{
	return z > field.size() + opponent->field.size() + 1 && z <= field.size() + opponent->field.size() + hand.size() + 1;
}

bool Player::IsTargetAlly(int z) const
{
	return z <= field.size() || (z > field.size() + opponent->field.size() + 1 && z <= field.size() + hand.size() + deck.size() + opponent->field.size() + 1);
}

bool Player::IsTargetOpponent(int z) const
{
	return (z > field.size() && z <= field.size() + opponent->field.size() + 1) || z > field.size() + hand.size() + deck.size() + opponent->field.size() + 1;
}

Card* Player::GetTargetCard(int z) const
{
	// The index ordering is leader - field - opponent field - opponent leader - hand - deck - opponent deck - opponent hand
	if (z < 0)
		return nullptr;
	if (z == 0)
		return leader;
	z -= 1;
	if (z < field.size())
		return field[z];
	z -= field.size();
	if (z < opponent->field.size())
		return opponent->field[z];
	z -= opponent->field.size();
	if (z == 0)
		return opponent->leader;
	z -= 1;
	if (z < hand.size())
		return hand[z];
	z -= hand.size();
	if (z < deck.size())
		return deck[z];
	z -= deck.size();
	if (z < opponent->deck.size())
		return opponent->deck[z];
	z -= opponent->deck.size();
	if (z < opponent->hand.size())
		return opponent->hand[z];
	return nullptr;
}

Card* Player::ExtractTargetCard(int z)
{
	if (z <= 0)
		return nullptr;
	z -= 1;
	if (z < field.size())
	{
		Card* target = field[z];
		if (!target || target->is_dying)
			return nullptr;
		field[z] = nullptr;
		field_size_adjust--;
		return target;
	}
	z -= field.size();
	if (z < opponent->field.size())
	{
		Card* target = opponent->field[z];
		if (!target || target->is_dying)
			return nullptr;
		opponent->field[z] = nullptr;
		opponent->field_size_adjust--;
		return target;
	}
	z -= opponent->field.size();
	if (z == 0)
		return nullptr;
	z -= 1;
	if (z < hand.size())
	{
		Card* target = hand[z];
		if (!target || target->is_dying)
			return nullptr;
		hand[z] = nullptr;
		hand_size_adjust--;
		return target;
	}
	z -= hand.size();
	if (z < deck.size())
	{
		Card* target = deck[z];
		if (!target || target->is_dying)
			return nullptr;
		deck[z] = nullptr;
		deck_size_adjust--;
		return target;
	}
	z -= deck.size();
	if (z < opponent->deck.size())
	{
		Card* target = opponent->deck[z];
		if (!target || target->is_dying)
			return nullptr;
		opponent->deck[z] = nullptr;
		opponent->deck_size_adjust--;
		return target;
	}
	z -= opponent->deck.size();
	if (z < opponent->hand.size())
	{
		Card* target = opponent->hand[z];
		if (!target || target->is_dying)
			return nullptr;
		opponent->hand[z] = nullptr;
		opponent->hand_size_adjust--;
		return target;
	}
	return nullptr;
}

void Player::SummonToField(Card* card)
{
	field.push_back(card);
	card->IncContribution(false);
	card->card_pos = CARD_POS_AT_FIELD;
	card->SetAffiliation(this);
	card->is_first_turn_at_field = true;
	card->n_atks_loss = 0;	
}

void Player::PutToHand(Card* card)
{
	hand.push_back(card);
	card->IncContribution(false);
	card->card_pos = CARD_POS_AT_HAND;
	card->SetAffiliation(this);
	card->is_first_turn_at_field = false;
	card->n_atks_loss = 0;
}

void Player::ShuffleToDeck(Card* card)
{
	int index = GetRandInt(deck.size() + 1);
	deck.insert(deck.begin() + index, card);
	card->card_pos = CARD_POS_AT_DECK;
	card->SetAffiliation(this);
	card->is_first_turn_at_field = false;
	card->n_atks_loss = 0;
}

double Player::GetMaxReservedCardsValue(const std::vector<int>& card_costs, const std::vector<double>& card_values, const std::vector<int>& minion_flags, int avail_mp, int field_slots)
{
	// note, the algorithm ignores the issue of playing two leader cards replacing the former one; as leader cards are mostly high mana cost it should be fine
	int n_cards = card_costs.size();
	int n_minion_cards = 0;
	for (int flag : minion_flags)
		n_minion_cards += flag;
	if (field_slots > n_minion_cards) // when the available slots is more than the number of minion cards, the extra ones are not going to affect anything
		field_slots = n_minion_cards;

	double*** dp = new double** [n_cards + 1];
	for (int i = 0; i <= n_cards; i++)
	{
		dp[i] = new double* [avail_mp + 1];
		for (int j = 0; j <= avail_mp; j++)
			dp[i][j] = new double[field_slots + 1];
	}

	for (int j = 0; j <= avail_mp; j++)
		for (int k = 0; k <= field_slots; k++)
			dp[0][j][k] = 0.0;

	for (int i = 0; i < n_cards; i++)
	{
		for (int j = 0; j <= avail_mp; j++)
			for (int k = 0; k <= field_slots; k++)
				if (j < card_costs[i] || k < minion_flags[i])
				{
					dp[i + 1][j][k] = dp[i][j][k];
				}
				else
				{
					double candidate_a = dp[i][j][k];
					double candidate_b = dp[i][j - card_costs[i]][k - minion_flags[i]] + card_values[i];
					dp[i + 1][j][k] = (candidate_a > candidate_b ? candidate_a : candidate_b);
				}
	}

	double max_value = dp[n_cards][avail_mp][field_slots];

	for (int i = 0; i <= n_cards; i++)
	{
		for (int j = 0; j <= avail_mp; j++)
			delete [] dp[i][j];
		delete [] dp[i];
	}
	delete [] dp;

	return max_value;
}

double Player::GetHeuristicAdjustment(int turn_num_adjust) const
{
	int n_remaining_turns = MAX_NUM_TURNS - turn_num - turn_num_adjust + 1;
	
	// max mp to grow
	int mp_to_grow = MAX_MP - max_mp;
	int n_mp_inc_turns = (mp_to_grow < n_remaining_turns ? mp_to_grow : n_remaining_turns);
	double tmp_eval = -0.1 * ((double)mp_to_grow - 0.5 * (double)n_mp_inc_turns) * n_mp_inc_turns; // negative, as the deficit from full mp

	// hand/deck resources
	tmp_eval += 0.9 * (double)(hand.size() + turn_num_adjust > MAX_HAND_SIZE ? MAX_HAND_SIZE : hand.size() + turn_num_adjust);
	tmp_eval += -0.9 * n_remaining_turns/((double)(deck.size() < turn_num_adjust ? 0.0 : deck.size() - turn_num_adjust) + 0.5);

	// empty field slots
	tmp_eval += 0.1 * (MAX_FIELD_SIZE - field.size());
	
	// card heuristics
	tmp_eval += leader->GetHeuristicEval();
	for (auto it = field.begin(); it != field.end(); it++)
		tmp_eval += (*it)->GetHeuristicEval();
	for (auto it = hand.begin(); it != hand.end(); it++)
		tmp_eval += (*it)->GetHeuristicEval();
	for (auto it = deck.begin(); it != deck.end(); it++)
		tmp_eval += (*it)->GetHeuristicEval();
	
	return tmp_eval;
}

double Player::ForwardToNextTurnAndEval()
{
	if (CheckLose())
	{
		if (opponent->CheckLose()) // draw
			return 0.0;
		else // loss
			return -1.0;
	}
	else 
	{
		if (opponent->CheckLose()) // win
			return 1.0;
		else // other cases
		{
			if (is_turn_active) // if the turn is not active, and not loss/draw/win, it means the end turn action is already executed
			{
				EndTurn();
				if (CheckLose()) // re-check game ending status after end turn events
				{
					if (opponent->CheckLose()) // draw
						return 0.0;
					else // loss
						return -0.995;
				}
				else
				{
					if (opponent->CheckLose()) // win
						return 0.995;
					else
						is_turn_active = true; // set back this so we can check if this is artifically ended
				}
			}
			opponent->StartTurn();
			double next_turn_start_eval = GetHeuristicEval();
			return next_turn_start_eval * 0.99; // apply some discount factor as there can be some uncertainty in turn end events and turn start events
		}
	}
}

double Player::GetHeuristicEval() const
{
	if (CheckLose())
	{
		if (opponent->CheckLose()) // draw
			return 0.0;
		else // loss
			return -1.0;
	}
	else 
	{
		if (opponent->CheckLose()) // win
			return 1.0;
		else // other cases (game does not end)
		{
			bool almost_win = false;
			bool almost_lose = false;

			// collect ally strength information
			double effective_ally_atk = leader->atk * leader->max_n_atks;
			double effective_ally_hp = leader->max_hp - leader->hp_loss; // note this is mostly considering leader's hp
			double reserved_ally_atk = 0.0;
			double reserved_cards_score = 0.0;
			double ally_poison_count = 0.0;
			if (is_turn_active)
			{
				// attacks that could be performed but not yet due to early evaluation
				int tmp_leader_n_atks = leader->max_n_atks - leader->n_atks_loss;
				int tmp_leader_dmg = leader->atk * tmp_leader_n_atks;
				reserved_ally_atk += leader->atk * tmp_leader_dmg;
				if (leader->is_lifesteal)
					effective_ally_hp += tmp_leader_dmg;
				if (leader->is_poisonous) // note that we only apply poisonous consideration for previous turn, not the current turn, as it doesn't affect leaders and can only indirectly do so by clearing minions
					ally_poison_count += (double)tmp_leader_n_atks * 0.5; // may or may not actually want to attack minions, so a discount is applied
				for (auto it = field.begin(); it != field.end(); it++)
				{
					Card* tmp_minion = *it;
					if (!tmp_minion->IsSleeping())
					{
						int tmp_minion_n_atks = tmp_minion->max_n_atks - tmp_minion->n_atks_loss;
						int tmp_minion_dmg = tmp_minion->atk * tmp_minion_n_atks;
						reserved_ally_atk += tmp_minion_dmg;
						if (tmp_minion->is_lifesteal)
							effective_ally_hp += tmp_minion_dmg;
						if (tmp_minion->is_poisonous) // note that we only apply poisonous consideration for previous turn, not the current turn, as it doesn't affect leaders and can only indirectly do so by clearing minions
							ally_poison_count += (double)tmp_minion_n_atks * 0.5; // may or may not actually want to attack minions, so a discout is applied
					}
				}
				// cards that could be played but not yet due to early evaluation (only considering cost, not checking validity; checking validity at this point would be problematic anyway)
				vector<int> card_costs(0);
				vector<double> card_values(0);
				vector<int> minion_flags(0);
				for (auto it = hand.begin(); it != hand.end(); it++)
				{
					Card* tmp_card = *it;
					card_costs.push_back(tmp_card->mana);
					double tmp_value = 0.0;
					switch (tmp_card->card_type)
					{
					case LEADER_CARD:
						minion_flags.push_back(0);
						tmp_value += (tmp_card->atk * tmp_card->max_n_atks * (tmp_card->is_charge ? 2.0 : 1.0) + tmp_card->max_hp + (tmp_card->is_shielded ? 1 : 0))
							- (leader->atk * leader->max_n_atks + leader->max_hp - leader->hp_loss + (leader->is_shielded ? 1 : 0));
						break;
					case MINION_CARD:
						minion_flags.push_back(1);
						tmp_value += tmp_card->atk * tmp_card->max_n_atks * (tmp_card->is_charge ? 2.0 : 1.0);
						if (tmp_card->is_taunt && !tmp_card->is_stealth) // not checking if leader has taunt as leader could be replaced with the remaining cost, just a very rough estimate
							tmp_value += tmp_card->max_hp + (tmp_card->is_shielded ? 1 : 0);
						break;
					default:
						minion_flags.push_back(0);
						break;
					}
					tmp_value += tmp_card->GetHeuristicEval();
					card_values.push_back(tmp_value);
				}
				reserved_cards_score = 0.5 * GetMaxReservedCardsValue(card_costs, card_values, minion_flags, max_mp - mp_loss, MAX_FIELD_SIZE - field.size()); // scale it with 0.5 as there are many possible consequence that are uncertain
			}
			if (effective_ally_hp > leader->max_hp) // at this point the ally hp stat should not exceed leader's max hp (regarding the addition of reserved lifesteal attack in previous turn)
				effective_ally_hp = leader->max_hp;
			if (leader->is_shielded)
				effective_ally_hp += 1; // counts shield as 1 effective hp (very rough estimate), so that the AI knows to break/save shield when it is good to do so
			for (auto it = field.begin(); it != field.end(); it++)
			{
				Card* tmp_minion = *it;
				effective_ally_atk += tmp_minion->atk * tmp_minion->max_n_atks;
				if ((!leader->is_taunt || leader->is_stealth) && tmp_minion->is_taunt && !tmp_minion->is_stealth) // minion only contribute to effective hp if it has taunt (and the leader does not)
					effective_ally_hp += tmp_minion->max_hp - tmp_minion->hp_loss + (tmp_minion->is_shielded ? 1 : 0);
			}
			if (deck.empty() && turn_num < MAX_NUM_TURNS) // if we don't have next turn then we don't need to consider fatigue
				effective_ally_hp -= fatigue + 1;
			
			// collect opponent strength information
			double effective_oppo_atk = opponent->leader->atk * opponent->leader->max_n_atks;
			double effective_oppo_hp = opponent->leader->max_hp - opponent->leader->hp_loss; // note this is mostly considering leader's hp
			if (opponent->leader->is_shielded)
				effective_oppo_hp += 1; // counts shield as 1 effective hp (very rough estimate), so that the AI knows to break/save shield when it is good to do so
			if (opponent->field.size() > 0) // we need to check this for allying poison anyway so might as well do it here
			{
				double oppo_minion_total_damage = 0;
				for (auto it = opponent->field.begin(); it != opponent->field.end(); it++)
				{
					Card* tmp_minion = *it;
					oppo_minion_total_damage += tmp_minion->atk * tmp_minion->max_n_atks;
					if ((!opponent->leader->is_taunt || opponent->leader->is_stealth) && tmp_minion->is_taunt && !tmp_minion->is_stealth) // minion only contribute to effective hp if it has taunt (and the leader does not)
						effective_oppo_hp += tmp_minion->max_hp - tmp_minion->hp_loss + (tmp_minion->is_shielded ? 1 : 0);
				}
				double poisoned_oppo_minions = (ally_poison_count > (double)(opponent->field.size()) ? (double)(opponent->field.size()) : ally_poison_count);
				effective_oppo_atk += (double)oppo_minion_total_damage * ((double)(opponent->field.size()) - poisoned_oppo_minions)/(double)(opponent->field.size());
			}
			if (effective_oppo_hp <= reserved_ally_atk) // a check point for a potentially ALMOST gauranteed win, as the last turn our side could use the reserved attacks to finish off (combined with turn end and turn start events, include fatigues that already happened at the start of current turn)
				almost_win = true;
			// add the lifesteal consideration for opponent leader and minions
			if (opponent->leader->is_lifesteal)
				effective_oppo_hp += opponent->leader->atk * opponent->leader->max_n_atks;
			for (auto it = opponent->field.begin(); it != opponent->field.end(); it++)
                        {
				Card* tmp_minion = *it;
				if (tmp_minion->is_lifesteal)
					effective_oppo_hp += tmp_minion->atk * tmp_minion->max_n_atks;
			}
			if (opponent->deck.empty() && opponent->turn_num < MAX_NUM_TURNS) // again, apply fatigue when it is not the last turn
				effective_oppo_hp -= opponent->fatigue + 1;

			// opponent will attack in current turn (and then allied leader suffers fatigue) so this checks for a potentially ALMOST gauranteed loss
			if (effective_ally_hp <= effective_oppo_atk)
				almost_lose = true;
			// add the lifesteal consideration for leader and allied minions for the coming turn
			if (leader->is_lifesteal)
				effective_ally_hp += leader->atk * leader->max_n_atks;
			for (auto it = field.begin(); it != field.end(); it++)
			{
				Card* tmp_minion = *it;
                    		if (tmp_minion->is_lifesteal)
					effective_ally_hp += tmp_minion->atk * tmp_minion->max_n_atks;
			}

			double total_diff = (effective_ally_atk + effective_ally_hp + GetHeuristicAdjustment(1)) - (effective_oppo_atk + effective_oppo_hp + GetHeuristicAdjustment(0)) + (reserved_ally_atk + reserved_cards_score);
			double raw_eval = total_diff / 30.0; // the 30.0 is the HP of the default leaders (well it should be a reasonable scaling factor)

			if (almost_win) // check almost win before almost lose because the check point for almost win is earlier (use the reserved attacks from last turn, when the check is after an artifically ended turn)
                        {
				raw_eval = 0.9 + raw_eval * 0.1;
				if (raw_eval > 0.99)
					return 0.99;
				else if (raw_eval < 0.9)  // not very likely
					return 0.9;
				else
					return raw_eval;
                        }
			else if (almost_lose)
			{
				raw_eval = -0.9 + raw_eval * 0.1;
				if (raw_eval < -0.99)
					return -0.99;
				else if (raw_eval > -0.9)  // not very likely
					return -0.9;
				else
					return raw_eval;
			}
			else
			{
				if (raw_eval > 0.9)
					return 0.9;
				else if (raw_eval < -0.9)
					return -0.9;
				else
					return raw_eval;
			}
		}
	}
}

vector<ActionSetEntity*> Player::GetOptionSet() const
{
	vector<ActionSetEntity*> option_set(0);

	// possible play actions
	for (int i = field.size() + opponent->field.size() + 2; i <= field.size() + opponent->field.size() + hand.size() + 1; i++)
	{
		PlayActionSet* tmp_set = new PlayActionSet(i);
		if (tmp_set->CreateValidSet(this) > 0)
			option_set.push_back(tmp_set);
		else
			delete tmp_set;
	}

	// possible attack actions
	for (int i = 0; i <= field.size(); i++)
	{
		AttackActionSet* tmp_set = new AttackActionSet(i);
		if (tmp_set->CreateValidSet(this) > 0)
			option_set.push_back(tmp_set);
		else
			delete tmp_set;
	}

	// end turn action, which is always an option
	EndTurnActionSet* tmp_set = new EndTurnActionSet();
	tmp_set->CreateValidSet(this);
	option_set.push_back(tmp_set);

	return option_set;
}

vector<ActionEntity*> Player::GetActionList() const
{
	vector<ActionEntity*> action_list(0);
	vector<ActionSetEntity*> option_set = GetOptionSet();

	for (auto it = option_set.begin(); it != option_set.end(); it++)
	{
		ActionSetEntity* tmp_set = (*it); 
		action_list.insert(action_list.end(), tmp_set->action_set.begin(), tmp_set->action_set.end());
		tmp_set->UnownActions();
		delete tmp_set;
	}
	
	return action_list;	
}

void Player::TakeSearchAIInputs()
{
	while (is_turn_active)
		TakeSearchAIInput();
}

void Player::TakeSearchAIInput()
{
	queue<DeferredEvent*> event_queue;
	PtrRedirMap redir_map;
	KnowledgeState knowledge_state(this, event_queue, redir_map);
	knowledge_state.PerformAction();

	while (!event_queue.empty())
	{
		delete event_queue.front();
		event_queue.pop();
	}
}

void Player::TakeRandomAIInputs()
{
	while (is_turn_active)
		TakeRandomAIInput();
}

void Player::TakeRandomAIInput()
{
	vector<ActionSetEntity*> option_set = GetOptionSet();

	int i = GetRandInt(option_set.size());
	option_set[i]->PerformRandomAction(this);

	for (auto it = option_set.begin(); it != option_set.end(); it++)
		delete (*it);
}

void Player::TakeInputs()
{
	while (is_turn_active)
		TakeSingleInput();
}

void Player::TakeSingleInput()
{
	char command_ch;
	
	bool valid_type_found = false;
	
	int x, y, z;
	while (!valid_type_found)
	{
		cin >> command_ch;
		switch (command_ch)
		{
		case 'P':
		case 'p':
			if (cin >> x >> y >> z)
			{
				cout << "Playing card " << x << " at position " << y << " targetting at " << z << "." << endl << endl; //echo
				if (CheckPlayValid(x, y, z))
				{
					Play(x, y, z);
					valid_type_found = true;
				}
			}
			else
			{
				cout << "Format error." << endl << endl;
				cin.clear(); cin.ignore(); // ignore the ill-formed input (otherwise stuck)
				DisplayHelp();
			}
			break;
		case 'A':
		case 'a':
			if (cin >> x >> z)
			{
				cout << "Commanding " << x << " to attack " << z << "." << endl << endl; //echo
				if (CheckAttackValid(x, z))
				{
					Attack(x, z);
					valid_type_found = true;
				}
			}
			else
			{
				cout << "Format error." << endl << endl;
				cin.clear(); cin.ignore(); // ignore the ill-formed input (otherwise stuck)
				DisplayHelp();
			}
			break;
		case 'E':
		case 'e':
			cout << "Ending the turn." << endl << endl; //echo
			EndTurn();
			valid_type_found = true;
			break;
		case 'R':
		case 'r':
			cout << "Resigning." << endl << endl; //echo
			SetLose();
			valid_type_found = true;
			break;
		case 'B':
		case 'b':
			cout << "Printing the current board." << endl << endl; //echo
			PrintBoard();
			valid_type_found = true;
			break;
		case 'Q':
		case 'q':
			if (cin >> x)
			{
				cout << "Querying " << x << "." << endl << endl; //echo
				Query(x);
				valid_type_found = true;
			}
			else
			{
				cout << "Format error." << endl << endl;
				cin.clear(); cin.ignore(); // ignore the ill-formed input (otherwise stuck)
				DisplayHelp();
			}
			break;
		case 'O':
		case 'o':
			display_overheat_counts = !display_overheat_counts;
			cout << "Display of overheat counts switched " << (display_overheat_counts ? "on." : "off.") << endl << endl;
			break;
		case 'H':
		case 'h':
			cout << "Displaying command manual." << endl << endl; //echo
			DisplayHelp();
			valid_type_found = true;
			break;
		}
	}
}

void Player::DisplayHelp() const
{
	cout << "(Note that all numbering used below should be consistent to the numbering in the current board info, i.e. by the order of you leader, your minions, and then opponent minions, opponent leader, and finally your hand.)" << endl << endl;
	cout << "P x y z: play card x from hand at position y and target z, the positioning y is for the minion on the left (0 if the first one), and the target z should be specified with the numbering on current board, y and/or z are sometimes not used;" << endl;
	cout << "A x y: use your minion x to attack enemy leader/minion y, the ordering is the same as the target in play card command;" << endl;
	cout << "E: end turn;" << endl;
	cout << "R: resign;" << endl;
	cout << "B: print board;" << endl;
	cout << "Q x: query details on leader/minion/card x, the ordering is the same as the target in play card command;" << endl;
	cout << "O: switch on/off the display of overheat counts, currently " << (display_overheat_counts ? "on;" : "off;") << endl;
	cout << "H: help on commands." << endl << endl;
}

void Player::Query(int x) const
{
	Card* card = nullptr;
	if (x == 0)
		card = leader;
	else if (x > 0 && x <= field.size())
		card = field[x - 1];
	else if (x > field.size() && x <= field.size() + opponent->field.size())
		card = opponent->field[x - field.size() - 1];
	else if (x == field.size() + opponent->field.size() + 1)
		card = opponent->leader;
	else if (x > field.size() + opponent->field.size() + 1 && x <= field.size() + opponent->field.size() + hand.size() + 1)
		card = hand[x - field.size() - opponent->field.size() - 2];
	else
		cout << "Invalid Target for Querying" << endl;
	
	if (card)
		cout << card->DetailInfo() << endl;
}

string Player::BriefInfo() const
{
	return leader->BriefInfo();
}

string Player::DetailInfo() const
{
	return leader->DetailInfo();
}

void Player::PrintBoard() const
{
	const Player *host, *guest;
	if (is_guest)
	{
		host = opponent;
		guest = this;
	}
	else
	{
		host = this;
		guest = opponent;
	}
	cout << endl;
	cout << guest->name << "\'s Side" << endl;
	cout << "================================" << endl;
	cout << "Opponent Deck Size: " << guest->deck.size() << endl;
	cout << "Opponent Hand Size: " << guest->hand.size() << endl;
	cout << "Opponent MP: " << guest->max_mp - guest->mp_loss << " of " << guest->max_mp << endl;
	cout << "--------------------------------" << endl;
	cout << "Opponent Leader:" << endl;
	cout << host->field.size() + guest->field.size() + 1 << " -- " << guest->BriefInfo() << endl;
	cout << "Opponent Minions:" << endl;
	for (int i = guest->field.size() - 1; i >= 0 ; i--)
		cout << i + host->field.size() + 1 << " -- " << guest->field[i]->BriefInfo() << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "Your Minions:" << endl;
	for (int i = host->field.size() - 1; i >= 0; i--)
		cout << i + 1 << " -- " << host->field[i]->BriefInfo() << endl;
	cout << "Your Leader:" << endl;
	cout << 0 << " -- " << host->BriefInfo() << endl;
	cout << "--------------------------------" << endl;
	cout << "Your MP: " << host->max_mp - host->mp_loss << " of " << host->max_mp << endl;
	cout << "Your Hand: " << endl;
	for (int i = 0; i < host->hand.size(); i++)
		cout << i + host->field.size() + guest->field.size() + 2 << " -- " << host->hand[i]->BriefInfo() << endl;
	cout << "Your Deck Size: " << host->deck.size() << endl;
	cout << "================================" << endl;
	cout << host->name << "\'s Side" << endl << endl;
}

int Player::GetActualFieldSize() const
{
	return field.size() + field_size_adjust;
}

int Player::GetActualHandSize() const
{
	return hand.size() + hand_size_adjust;
}

int Player::GetActualDeckSize() const
{
	return deck.size() + deck_size_adjust;
}


/* Generator/Descriptor Section */


void IntersectRange(int& min_result, int& max_result, int min1, int max1, int min2, int max2)
{
	min_result = (min2 > min1 ? min2 : min1);
	max_result = (max2 < max1 ? max2 : max1);
}

void IntersectRangeInPlace(int& min_val, int& max_val, int other_min_val, int other_max_val)
{
	if (other_min_val > min_val)
		min_val = other_min_val;
	if (other_max_val < max_val)
		max_val = other_max_val;
}

void UnionRangeInPlace(int& min_val, int& max_val, int other_min_val, int other_max_val)
{
	if (other_min_val < min_val)
		min_val = other_min_val;
	if (other_max_val > max_val)
		max_val = other_max_val;
}


CondConfig::CondConfig(unsigned _flag, int _min_mp, int _max_mp, int _min_max_mp, int _max_max_mp, int _min_cost, int _max_cost, int _min_atk, int _max_atk, int _min_hp, int _max_hp, int _min_n_atks, int _max_n_atks)
	: flag(_flag), min_mp(_min_mp), max_mp(_max_mp), min_max_mp(_min_max_mp), max_max_mp(_max_max_mp), min_cost(_min_cost), max_cost(_max_cost), min_atk(_min_atk), max_atk(_max_atk), min_hp(_min_hp), max_hp(_max_hp), min_n_atks(_min_n_atks), max_n_atks(_max_n_atks)
{
}

CondConfig::CondConfig(unsigned _flag, int _min_cost, int _max_cost, int _min_atk, int _max_atk, int _min_hp, int _max_hp, int _min_n_atks, int _max_n_atks)
	: CondConfig(_flag, 0, 10, 0, 10, _min_cost, _max_cost, _min_atk, _max_atk, _min_hp, _max_hp, _min_n_atks, _max_n_atks)
{
}

CondConfig::CondConfig(unsigned _flag)
	: CondConfig(_flag, 0, 10, 0, 10, -9, 40, 0, 5)
{
	if (!(_flag & TARGET_IS_SPELL))
	{
		// if it is referring to a character and considers minions or only allowing minions then the upper bound for HP become lower
		if (_flag & TARGET_IS_MINION)
		{
			if (_flag & TARGET_IS_LEADER)
				max_hp = 20;
			else
				max_hp = 10;
		}
	}
}

CondConfig::CondConfig()
	: CondConfig(TARGET_TYPE_ANY, 0, 10, 0, 10, -9, 40, 0, 5)  // -9 is taking into consideration of on destroy (deathrattle) timing
{
}

CondConfig& CondConfig::operator &= (const CondConfig& config)
{
	flag &= config.flag;
	IntersectRangeInPlace(min_mp, max_mp, config.min_mp, config.max_mp);
	IntersectRangeInPlace(min_max_mp, max_max_mp, config.min_max_mp, config.max_max_mp);
	IntersectRangeInPlace(min_cost, max_cost, config.min_cost, config.max_cost);
	IntersectRangeInPlace(min_atk, max_atk, config.min_atk, config.max_atk);
	IntersectRangeInPlace(min_hp, max_hp, config.min_hp, config.max_hp);
	IntersectRangeInPlace(min_n_atks, max_n_atks, config.min_n_atks, config.max_n_atks);

	return *this;
}

CondConfig& CondConfig::operator &= (unsigned flag)
{
	this->flag &= flag;

	return *this;
}

CondConfig& CondConfig::operator |= (const CondConfig & config)
{
	UnionRangeInPlace(min_mp, max_mp, config.min_mp, config.max_mp);
	UnionRangeInPlace(min_max_mp, max_max_mp, config.min_max_mp, config.max_max_mp);
	UnionRangeInPlace(min_cost, max_cost, config.min_cost, config.max_cost);
	UnionRangeInPlace(min_atk, max_atk, config.min_atk, config.max_atk);
	UnionRangeInPlace(min_hp, max_hp, config.min_hp, config.max_hp);
	UnionRangeInPlace(min_n_atks, max_n_atks, config.min_n_atks, config.max_n_atks);

	return *this;
}

CondConfig& CondConfig::operator |= (unsigned flag)
{
	this->flag |= flag;

	return *this;
}

unsigned CondConfig::operator & (unsigned mask) const
{
	return flag & mask;
}

unsigned CondConfig::operator | (unsigned mask) const
{
	return flag | mask;
}

CondConfig GetInitConfigFromCard(const Card* card)
{
	unsigned card_pos_flag = TARGET_ANY_POS_TYPE;
	unsigned card_type_flag = TARGET_ANY_CARD_TYPE;
	unsigned minion_type_flag = TARGET_TYPE_NOTHING;

	switch (card->card_type)
	{
	case LEADER_CARD:
		card_type_flag = TARGET_IS_LEADER;
		break;
	case MINION_CARD:
		card_type_flag = TARGET_IS_MINION;
		switch (card->minion_type)
		{
		case BEAST_MINION:
			minion_type_flag = TARGET_IS_BEAST;
			break;
		case DRAGON_MINION:
			minion_type_flag = TARGET_IS_DRAGON;
			break;
		case DEMON_MINION:
			minion_type_flag = TARGET_IS_DEMON;
			break;
		}
		break;
	case SPELL_CARD:
		card_type_flag = TARGET_IS_SPELL;
		card_pos_flag = TARGET_POS_HAND_OR_DECK;
		break;
	}

	// card position
	unsigned flag = card_pos_flag | card_type_flag | minion_type_flag;

	return CondConfig(flag, card->mana, card->mana, card->atk, card->atk, card->max_hp, card->max_hp, card->max_n_atks, card->max_n_atks);
}

CondConfig GetInstantConfigFromInitConfig(const CondConfig& config)
{
	unsigned flag = config.flag | TARGET_ANY_MINION_ABIL_TYPE; // the minion type and ability field in instant config work differently from init config, so reset those bits (to 1's) first
	int min_cost = 0;
	int max_cost = 10;
	int min_atk = 0;
	int max_atk = 10;
	int min_hp = -9;
	int max_hp = 40;
	int min_n_atks = 0;
	int max_n_atks = 5;
	
	if (!(flag & TARGET_NOT_LEADER)) // must be leader
	{
		flag &= LEADER_COND_FILTER;
		min_cost = 8;
	}
	else if (!(flag & TARGET_IS_LEADER)) // must not be leader
	{
		max_cost = 7;
		max_hp = 10;
	}

	if (!(flag & TARGET_ANY_CHAR)) // must be spell
	{
		flag &= SPELL_COND_FILTER;
	}
	else if (!(flag & TARGET_NOT_MINION)) // must be minion
	{
		flag &= MINION_COND_FILTER;
	} 	

	return CondConfig(flag, min_cost, max_cost, min_atk, max_atk, min_hp, max_hp, min_n_atks, max_n_atks);
}

void AdjustInitConfigWithAnotherConfig(CondConfig& init_config, const CondConfig& config) // the other config could be init or instant config
{
	// only dealing with card type related stat ranges; other parts are either not used or sufficient with just the instant config
	if (!(config & TARGET_NOT_LEADER)) // must be leader
	{
		if (init_config.min_cost < 8)
			init_config.min_cost = 8;
		if (init_config.max_cost < init_config.min_cost)
			init_config.max_cost = init_config.min_cost;
	}
	else if (!(config & TARGET_IS_LEADER)) // must not be leader
	{
		if (init_config.max_cost > 7)
			init_config.max_cost = 7;
		if (init_config.min_cost > init_config.max_cost)
			init_config.min_cost = init_config.max_cost;

		if (init_config.max_hp > 10)
			init_config.max_hp = 10;
		if (init_config.min_hp > 10)
			init_config.min_hp = 10;
	}
}

CondConfig ExtractEffectIndependentConfig(const CondConfig& config)
{
	// remove the position flags that are effect dependent
	unsigned correct_pos_flag = (!(config & TARGET_ANY_CHAR) ? TARGET_POS_HAND_OR_DECK : TARGET_ANY_POS_TYPE);

	return CondConfig(config.flag | correct_pos_flag, config.min_mp, config.max_mp, config.min_max_mp, config.max_max_mp, config.min_cost, config.max_cost, config.min_atk, config.max_atk, config.min_hp, config.max_hp, config.min_n_atks, config.max_n_atks);
}

CondConfig GetDefaultInitConfig()
{
	return CondConfig(TARGET_ANY_POS_TYPE | TARGET_ANY_CARD_ALLE_MINION_TYPE);
}

CondConfig GetDefaultConfig()
{
	return CondConfig();
}

CondConfig GetFlagConfig(unsigned flag)
{
	return CondConfig(flag);
}

CondConfig GetMpConfig(int min_mp, int max_mp)
{
	return CondConfig(TARGET_TYPE_ANY, min_mp, max_mp, min_mp, 10, 0, 10, 0, 10, -9, 40, 0, 5);
}

CondConfig GetMaxMpConfig(int min_max_mp, int max_max_mp)
{
	return CondConfig(TARGET_TYPE_ANY, 0, max_max_mp, min_max_mp, max_max_mp, 0, 10, 0, 10, -9, 40, 0, 5);
}

CondConfig GetCostConfig(unsigned flag, int min_cost, int max_cost)
{
	return CondConfig(flag, min_cost, max_cost, 0, 10, -9, 40, 0, 5);
}

CondConfig GetAtkConfig(unsigned flag, int min_atk, int max_atk)
{
	return  CondConfig(flag, 0, 10, min_atk, max_atk, -9, 40, 0, 5);
}

CondConfig GetHpConfig(unsigned flag, int min_hp, int max_hp)
{
	return CondConfig(flag, 0, 10, 0, 10, min_hp, max_hp, 0, 5);
}

CondConfig GetAtkTimesConfig(unsigned flag, int min_n_atks, int max_n_atks)
{
	return CondConfig(flag, 0, 10, 0, 10, -9, 40, min_n_atks, max_n_atks);
}


bool display_overheat_counts = false;


string MinionTypeDescription(int type)
{
	switch (type)
	{
	case BEAST_MINION:
		return "Beast";
	case DRAGON_MINION:
		return "Dragon";
	case DEMON_MINION:
		return "Demon";
	}

	return "";
}

string AbilityDescriptionInline(bool is_charge, bool is_taunt, bool is_stealth, bool is_untargetable, bool is_shielded, bool is_poisonous, bool is_lifesteal)
{
	string desc =
		(is_charge ? string("Charge, ") : string("")) // the string() is needed at least for the first item otherwise it would have trouble figuring out the type (wouldn't compile)
		+ (is_taunt ? string("Taunt, ") : string(""))
		+ (is_stealth ? string("Stealth, ") : string(""))
		+ (is_untargetable ? string("Untargetable, ") : string(""))
		+ (is_shielded ? string("Shielded, ") : string(""))
		+ (is_poisonous ? string("Poisonous, ") : string(""))
		+ (is_lifesteal ? string("Lifesteal, ") : string(""));
	if (desc.length() > 1) // remove the last space and comma if non-empty
	{
		desc.pop_back();
		desc.pop_back();
		desc = " with " + desc;
	}
	return desc; // add a leading space
}

string AbilityDescriptionBrief(bool is_charge, bool is_taunt, bool is_stealth, bool is_untargetable, bool is_shielded, bool is_poisonous, bool is_lifesteal)
{
	string desc = 
		(is_charge ? string("Charge. ") : string("")) // the string() is needed at least for the first item otherwise it would have trouble figuring out the type (wouldn't compile)
		+ (is_taunt ? string("Taunt. ") : string(""))
		+ (is_stealth ? string("Stealth. ") : string(""))
		+ (is_untargetable ? string("Untargetable. ") : string(""))
		+ (is_shielded ? string("Shielded. ") : string(""))
		+ (is_poisonous ? string("Poisonous. ") : string(""))
		+ (is_lifesteal ? string("Lifesteal. ") : string(""));
	if (desc.length() > 0) // remove the last space if non-empty
		desc.pop_back();
	return desc;
}

string AbilityDescriptionDetail(bool is_charge, bool is_taunt, bool is_stealth, bool is_untargetable, bool is_shielded, bool is_poisonous, bool is_lifesteal, int indent_size)
{
	return (is_charge ? string("Charge.\n") + RepeatSpace(indent_size) : string("")) // the string() is needed at least for the first item otherwise it would have trouble figuring out the type (wouldn't compile)
		+ (is_taunt ? string("Taunt.\n") + RepeatSpace(indent_size) : string(""))
		+ (is_stealth ? string("Stealth.\n") + RepeatSpace(indent_size) : string(""))
		+ (is_untargetable ? string("Untargetable (Cannot be targeted by spells or battlecries).\n") + RepeatSpace(indent_size) : (""))
		+ (is_shielded ? string("Shielded (Divine Shield).\n") + RepeatSpace(indent_size) : string(""))
		+ (is_poisonous ? string("Poisonous.\n") + RepeatSpace(indent_size) : string(""))
		+ (is_lifesteal ? string("Lifesteal.\n") + RepeatSpace(indent_size) : string(""));
}


/* Simulator Section */

void GiglRandInit(unsigned seed)
{
	RandInit(seed);
}

int GetGiglRandInt()
{
	return GetRandInt();
}

int GetGiglRandInt(int n)
{
	return GetRandInt(n);
}

double GetGiglRandFloat()
{
	return GetRandFloat();
}

double GetGiglRandFloat(double max_val)
{
	return GetRandFloat(max_val);
}

double GetGiglRandFloat(double min_val, double max_val)
{
	return GetRandFloat(min_val, max_val);
}

vector<int> CreateRandomSelection(int n, int k)
{
	int* a = new int[n];
	for (int i = 0; i < n; i++)
		a[i] = i;
	RandShuffle(a, n, k);
	vector<int> selected_indices(a+n-k, a+n); // note: the ending reference should be just outside the range
	delete [] a;

	return selected_indices;
}

vector<int> CreateRandomSelectionSorted(int n, int k)
{
	vector<int> selected_list = CreateRandomSelection(n, k);
	sort(selected_list.begin(), selected_list.end());
	return selected_list;
}

Card* GenerateSingleCard(int seed)
{
	return GenerateCard(seed);
}

Card* GenerateSingleCardWithCost(int cost, int seed)
{
	return GenerateCardWithCost(cost, seed);
}

string GetCardName(Card* card)
{
	return card->name;
}

string GetCardBrief(Card* card)
{
	return card->BriefInfo();
}

string GetCardDetail(Card* card)
{
	return card->DetailInfo();
}

int GetCardOrigCost(Card* card)
{
	return card->orig_mana;
}

vector<Card*> GenerateCardSet(int n, int seed)
{
	vector<int> seed_list = GenerateCardSetSeeds(n, seed);

	vector<Card*> cards(n);
	for (int i = 0; i < n; i++)
		cards[i] = GenerateSingleCard(seed_list[i]);

	return cards;
}

vector<int> GenerateCardSetSeeds(int n, int seed)
{
	RandInit(seed);

	vector<int> seed_set(n);
	for (int i = 0; i < n; i++)
		seed_set[i] = GetRandInt();

	// remove duplicates (so it is easier for checking # of overlapped cards between deck through seeds), should rarely actually happen, but just in case, will also make the seeds be sorted (increaing) at the same time
	bool no_duplicate;
	do
	{
		no_duplicate = true;
		sort(seed_set.begin(), seed_set.end());
		for (int i = 1; i < n; i++)
		{
			if (seed_set[i] == seed_set[i - 1])
			{
				no_duplicate = false;
				int dup = seed_set[i - 1];
				seed_set[i] = GetRandInt();
				while(seed_set[i] == dup)
					seed_set[i] = GetRandInt();
				for (i++; i < n; i++)
				{
					if (seed_set[i] == dup)
					{
						seed_set[i] = GetRandInt();
						while (seed_set[i] == dup)
							seed_set[i] = GetRandInt();
					}
					else
						break;
				}
			}
		}
	}
	while(!no_duplicate);

	return seed_set;
}

void InitMatch(vector<Card*>& card_list, const vector<int>& deck_a_orig_indices, const vector<int>& deck_b_orig_indices, vector<int>& deck_a_shuffle, vector<int>& deck_b_shuffle, vector<int>& deck_a_indices, vector<int>& deck_b_indices, vector<Card*>& deck_a, vector<Card*>& deck_b) // card_list didn't use const only because GIGL currently doesn't allow node sections to have const functions (can be made const when GIGL takes this into account)
{
	int seed = GetRandInt();
	RandInit(seed);
	#ifndef SUPPRESS_TOURNAMENT_PROMPTS
	cout << "Match seed: " << seed << ". " << endl;
	#endif

	int size_a = deck_a_indices.size();
	int size_b = deck_b_indices.size();

	// shuffle and reproduce the orders to other info
	iota(deck_a_shuffle.begin(), deck_a_shuffle.end(), 0); // fill index with {0,1,2,...}
	RandShuffle(deck_a_shuffle.data(), size_a);
	for (int k = 0; k < size_a; k++)
	{
		deck_a_indices[k] = deck_a_orig_indices[deck_a_shuffle[k]];
		deck_a[k] = HardCopyCard(card_list[deck_a_indices[k]]);
	}
	iota(deck_b_shuffle.begin(), deck_b_shuffle.end(), 0); // fill index with {0,1,2,...}
	RandShuffle(deck_b_shuffle.data(), size_b);
	for (int k = 0; k < size_b; k++)
	{
		deck_b_indices[k] = deck_b_orig_indices[deck_b_shuffle[k]];
		deck_b[k] = HardCopyCard(card_list[deck_b_indices[k]]);
	}
}

void DecidePlayOrder(Player* player1, Player* player2, Player*& first_player, Player*& second_player)
{
	if (GetRandInt() % 2)
	{
		first_player = player1;
		second_player = player2;
	}
	else
	{
		first_player = player2;
		second_player = player1;
	}

	cout << first_player->name << " goes first." << endl << endl;
}

Card* HardCopyCard(Card* card) // note: this version cannot be used when we want to keep a consistent redir_map (as it is not passed in or out); use when we only care about the card itself
{
	PtrRedirMap redir_map;
	return card->CreateHardCopy(redir_map);
}

vector<Card*> HardCopyCards(vector<Card*> cards)
{
	vector<Card*> card_copies;
	for (Card* card : cards)
		card_copies.push_back(HardCopyCard(card));

	return card_copies;
}

void DeleteCard(Card* card)
{
	delete card;
}

void DeleteCards(vector<Card*>& cards)
{
	for (Card* card : cards)
		delete card;
}


vector<Card*> CreateDemoCardSet1()
{
	vector<Card*> cards(100);
	for (int i = 0; i < 100; i++)
		cards[i] = CreateDemoCard1();
	return cards;
}

vector<Card*> CreateDemoCardSet1Alt()
{
	vector<Card*> cards(100);
	for (int i = 0; i < 50; i++)
		cards[i] = CreateDemoCard1Alt(1);
	for (int i = 50; i < 100; i++)
		cards[i] = CreateDemoCard1Alt(5);
	return cards;
}

vector<Card*> CreateDemoCardSet1Alt2()
{
	vector<Card*> cards(100);
	for (int i = 0; i < 25; i++)
		cards[i] = CreateDemoCard1Alt(1);
	for (int i = 25; i < 50; i++)
		cards[i] = CreateDemoCard1Alt(5);
	for (int i = 50; i < 75; i++)
		cards[i] = CreateDemoCard3(1);
	for (int i = 75; i < 100; i++)
		cards[i] = CreateDemoCard3(5);
	return cards;
}

vector<Card*> CreateDemoCardSet2()
{
	vector<Card*> cards(100);
	for (int i = 0; i < 50; i++)
		cards[i] = CreateDemoCard2(1);
	for (int i = 50; i < 100; i++)
		cards[i] = CreateDemoCard2(5);
	return cards;
}

vector<Card*> CreateDemoCardSet2Alt()
{
	vector<Card*> cards(100);
	for (int i = 0; i < 90; i++)
		cards[i] = CreateDemoCard2(1);
	for (int i = 90; i < 100; i++)
		cards[i] = CreateDemoCard2(5);
	return cards;
}

vector<Card*> CreateDemoCardSet3()
{
	vector<Card*> cards(100);
	for (int i = 0; i < 50; i++)
		cards[i] = CreateDemoCard3(1);
	for (int i = 50; i < 100; i++)
		cards[i] = CreateDemoCard3(5);
	return cards;
}

vector<Card*> CreateDemoCardSet4()
{
	vector<Card*> cards(100);
	for (int i = 0; i < 50; i++)
		cards[i] = CreateDemoCard4(1);
	for (int i = 50; i < 100; i++)
		cards[i] = CreateDemoCard4(5);
	return cards;
}

vector<Card*> CreateDemoCardSet4Alt()
{
	vector<Card*> cards(100);
	for (int i = 0; i < 25; i++)
		cards[i] = CreateDemoCard4(1);
	for (int i = 25; i < 50; i++)
		cards[i] = CreateDemoCard4(5);
	for (int i = 50; i < 75; i++)
		cards[i] = CreateDemoCard4Alt(1);
	for (int i = 75; i < 100; i++)
		cards[i] = CreateDemoCard4Alt(5);
	return cards;
}

vector<Card*> CreateDemoCardSet5()
{
	vector<Card*> cards(100);
	for (int i = 0; i < 50; i++)
		cards[i] = CreateDemoCard5Beast();
	for (int i = 50; i < 100; i++)
		cards[i] = CreateDemoCard5Dragon();
	return cards;
}

vector<Card*> CreateDemoCardSet6()
{
	vector<Card*> cards(100);
	for (int i = 0; i < 25; i++)
		cards[i] = CreateDemoCard6Beast(1);
	for (int i = 25; i < 50; i++)
		cards[i] = CreateDemoCard6Beast(5);
	for (int i = 50; i < 75; i++)
		cards[i] = CreateDemoCard6Dragon(1);
	for (int i = 75; i < 100; i++)
		cards[i] = CreateDemoCard6Dragon(5);
	return cards;
}

vector<Card*> CreateDemoCardSet7()
{
	vector<Card*> cards(100);
	for (int i = 0; i < 10; i++)
		cards[i] = CreateDefaultLeader(30);
	for (int i = 10; i < 70; i++)
		cards[i] = CreateDemoCard1();
	for (int i = 70; i < 100; i++)
		cards[i] = CreateSndPlayerToken();
	return cards;
}

vector<Card*> CreateDemoCardSet8()
{
	vector<Card*> cards(100);
	for (int i = 0; i < 25; i++)
		cards[i] = CreateDemoCard8Beast();
	for (int i = 25; i < 50; i++)
		cards[i] = CreateDemoCard8Spell();
	for (int i = 50; i < 75; i++)
		cards[i] = CreateDemoCard8Dragon();
	for (int i = 75; i < 100; i++)
		cards[i] = CreateDemoCard8Leader();
	return cards;
}

vector<Card*> CreateDemoCardSetX()
{
	vector<Card*> cards(100);

	cards[0] = CreateDemoCardXDemon(0);
	cards[1] = CreateDemoCardXSpell(0);

	for (int i = 2; i < 5; i++)
		cards[i] = CreateDemoCardXBeast(1);
	for (int i = 5; i < 7; i++)
		cards[i] = CreateDemoCardXDragon(1);
	for (int i = 7; i < 10; i++)
		cards[i] = CreateDemoCardXDemon(1);
	for (int i = 10; i < 15; i++)
		cards[i] = CreateDemoCardXSpell(1);

	for (int i = 15; i < 20; i++)
		cards[i] = CreateDemoCardXBeast(2);
	for (int i = 20; i < 24; i++)
		cards[i] = CreateDemoCardXDragon(2);
	for (int i = 24; i < 28; i++)
		cards[i] = CreateDemoCardXDemon(2);
	for (int i = 28; i < 34; i++)
		cards[i] = CreateDemoCardXSpell(2);

	for (int i = 34; i < 39; i++)
		cards[i] = CreateDemoCardXBeast(3);
	for (int i = 39; i < 43; i++)
		cards[i] = CreateDemoCardXDragon(3);
	for (int i = 43; i < 47; i++)
		cards[i] = CreateDemoCardXDemon(3);
	for (int i = 47; i < 53; i++)
		cards[i] = CreateDemoCardXSpell(3);

	for (int i = 53; i < 57; i++)
		cards[i] = CreateDemoCardXBeast(4);
	for (int i = 57; i < 60; i++)
		cards[i] = CreateDemoCardXDragon(4);
	for (int i = 60; i < 63; i++)
		cards[i] = CreateDemoCardXDemon(4);
	for (int i = 63; i < 68; i++)
		cards[i] = CreateDemoCardXSpell(4);

	for (int i = 68; i < 71; i++)
		cards[i] = CreateDemoCardXBeast(5);
	for (int i = 71; i < 74; i++)
		cards[i] = CreateDemoCardXDragon(5);
	for (int i = 74; i < 76; i++)
		cards[i] = CreateDemoCardXDemon(5);
	for (int i = 76; i < 80; i++)
		cards[i] = CreateDemoCardXSpell(5);

	for (int i = 80; i < 82; i++)
		cards[i] = CreateDemoCardXBeast(6);
	for (int i = 82; i < 84; i++)
		cards[i] = CreateDemoCardXDragon(6);
	for (int i = 84; i < 85; i++)
		cards[i] = CreateDemoCardXDemon(6);
	for (int i = 85; i < 87; i++)
		cards[i] = CreateDemoCardXSpell(6);

	for (int i = 87; i < 88; i++)
		cards[i] = CreateDemoCardXBeast(7);
	for (int i = 88; i < 90; i++)
		cards[i] = CreateDemoCardXDragon(7);
	for (int i = 90; i < 91; i++)
		cards[i] = CreateDemoCardXDemon(7);
	for (int i = 91; i < 93; i++)
		cards[i] = CreateDemoCardXSpell(7);

	for (int i = 93; i < 96; i++)
		cards[i] = CreateDemoCardXLeader(8);

	for (int i = 96; i < 98; i++)
		cards[i] = CreateDemoCardXLeader(9);

	for (int i = 98; i < 100; i++)
		cards[i] = CreateDemoCardXLeader(10);

	return cards;
}

vector<Card*> CreateDemoCardSetH()
{
	vector<Card*> cards(100);
	
	// Basic Neutral Cards
	cards[0] = CreateElvenArcher();
	cards[1] = CreateGoldshireFootman();
	cards[2] = CreateMurlocRaider();
	cards[3] = CreateStonetuskBoar();
	cards[4] = CreateVoodooDoctor();
	cards[5] = CreateBloodfenRapter();
	cards[6] = CreateBluegillWarrior();
	cards[7] = CreateFrostwolfGrunt();
	cards[8] = CreateMurlocTidehunter();
	cards[9] = CreateNoviceEngineer();
	cards[10] = CreateRiverCrocolisk();
	cards[11] = CreateIronforgeRifleman();
	cards[12] = CreateIronfurGrizzly();
	cards[13] = CreateMagmaRager();
	cards[14] = CreateRazorfenHunter();
	cards[15] = CreateShatteredSunCleric();
	cards[16] = CreateSilverbackPatriarch();
	cards[17] = CreateWolfrider();
	cards[18] = CreateChillwindYeti();
	cards[19] = CreateDragonlingMechanic();
	cards[20] = CreateGnomishInventor();
	cards[21] = CreateOasisSnapjaw();
	cards[22] = CreateSenjinShieldmasta();
	cards[23] = CreateStormwindKnight();
	cards[24] = CreateBootyBayBodyguard();
	cards[25] = CreateDarkscaleHealer();
	cards[26] = CreateNightblade();
	cards[27] = CreateStormpikeCommando();
	cards[28] = CreateBoulderfistOgre();
	cards[29] = CreateLordOfTheArena();
	cards[30] = CreateRecklessRocketeer();
	cards[31] = CreateCoreHound();
	cards[32] = CreateWarGolem();

	// Basic Class Cards
	cards[33] = CreateChaosNova();
	cards[34] = CreateMoonfire();
	cards[35] = CreateHealingTouch();
	cards[36] = CreateStarfire();
	cards[37] = CreateArcaneShot();
	cards[38] = CreateMirrorImage();
	cards[39] = CreateArcaneExplosion();
	cards[40] = CreateArcaneIntellect();
	cards[41] = CreateFireball();
	cards[42] = CreatePolymorph();
	cards[43] = CreateFlamestrike();
	cards[44] = CreateBlessingOfMight();
	cards[45] = CreateHandOfProtection();
	cards[46] = CreateHolyLight();
	cards[47] = CreateBlessingOfKings();
	cards[48] = CreateConsecration();
	cards[49] = CreateHammerOfWrath();
	cards[50] = CreateGuardianOfKings();
	cards[51] = CreatePowerWordShield();
	cards[52] = CreateHolySmite();
	cards[53] = CreateMindVision();
	cards[54] = CreatePsychicConjurer();
	cards[55] = CreateRadiance();
	cards[56] = CreateShadowWordDeath();
	cards[57] = CreateShadowWordPain();
	cards[58] = CreateHolyNova();
	cards[59] = CreatePowerInfusion();
	cards[60] = CreateSinisterStrike();
	cards[61] = CreateSap();
	cards[62] = CreateShiv();
	cards[63] = CreateFanOfKnives();
	cards[64] = CreatePlaguebringer();
	cards[65] = CreateAssassinate();
	cards[66] = CreateHex();
	cards[67] = CreateFireElemental();
	cards[68] = CreateSoulfire();
	cards[69] = CreateFelstalker();
	cards[70] = CreateDrainLife();
	cards[71] = CreateShadowBolt();
	cards[72] = CreateHellfire();
	cards[73] = CreateWhirlwind();
	cards[74] = CreateKorkronElite();
	
	// Classic Neutral Cards
	cards[75] = CreateWisp();
	cards[76] = CreateLeperGnome();
	cards[77] = CreateWorgenInfiltrator();
	cards[78] = CreateFaerieDragon();
	cards[79] = CreateLootHoarder();
	cards[80] = CreateEarthenRingFarseer();
	cards[81] = CreateHarvestGolem();
	cards[82] = CreateJunglePanther();
	cards[83] = CreateScarletCrusader();
	cards[84] = CreateThrallmarFarseer();
	cards[85] = CreateStranglethornTiger();
	cards[86] = CreateWindfuryHarpy();
	cards[87] = CreateEmperorCobra();
	cards[88] = CreateImpMaster();
	cards[89] = CreateInjuredBlademaster();
	cards[90] = CreateAbomination();
	cards[91] = CreateStampedingKodo();
	cards[92] = CreateSunwalker();
	cards[93] = CreateRavenholdtAssassin();
	cards[94] = CreateBigGameHunter();
	cards[95] = CreateKingMukla();
	cards[96] = CreateCairneBloodhoof();
	cards[97] = CreateHogger();
	cards[98] = CreateTheBeast();
	cards[99] = CreateTheBlackKnight();

	return cards;
}

DeferredEvent::DeferredEvent(Card* _card, bool _is_owning_card, bool _start_of_batch) : card(_card), is_owning_card(_is_owning_card), is_start_of_batch(_start_of_batch)
{
}

DeferredEvent::~DeferredEvent()
{
	if (is_owning_card && card)
		delete card;
}

DestroyEvent::DestroyEvent(Card* _card, bool _start_of_batch) : DeferredEvent(_card, false, _start_of_batch)
{
}

void DestroyEvent::Process(Player* curr_player)
{
	if (card == curr_player->leader)
	{
		#ifndef SUPPRESS_MATCH_PROMPTS
		if (!curr_player->is_exploration)
			cout << curr_player->name << "\'s leader " << card->name << " destroyed." << endl << endl;
		#endif
		curr_player->SetLose();
	}
	else if (card == curr_player->opponent->leader)
	{
		#ifndef SUPPRESS_MATCH_PROMPTS
		if (!curr_player->is_exploration)
			cout << curr_player->opponent->name << "\'s leader " << card->name << " destroyed." << endl << endl;
		#endif
		curr_player->opponent->SetLose();
	}
	else
	{
		#ifndef SUPPRESS_MATCH_PROMPTS
		if (!curr_player->is_exploration)
			cout << card->owner->name << "\'s minion " << card->name << " destroyed." << endl << endl;
		#endif
		card->Destroy();
	}
}

CastEvent::CastEvent(Card* _card, bool _start_of_batch) : DeferredEvent(_card, false, _start_of_batch)
{
}

void CastEvent::Process(Player* curr_player)
{
	// do nothing
}

DiscardEvent::DiscardEvent(Card* _card, bool _start_of_batch) : DeferredEvent(_card, false, _start_of_batch)
{
}

void DiscardEvent::Process(Player* curr_player)
{
	#ifndef SUPPRESS_MATCH_PROMPTS
	if (!curr_player->is_exploration)
		cout << card->owner->name << "\'s " << card->name << " discarded." << endl << endl;
	#endif
	card->Discard();
}

FieldSummonEvent::FieldSummonEvent(Card* _card, bool _start_of_batch, Player* _owner) : DeferredEvent(_card, true, _start_of_batch), owner(_owner)
{
}

void FieldSummonEvent::Process(Player* curr_player)
{
	#ifndef SUPPRESS_MATCH_PROMPTS
	if (!curr_player->is_exploration) // don't print original card owner's (card->owner) info as theoretically cards that are in transfer or being spawned/copied does not have an owner until it reaches the destination
		cout << card->name << " summoned to " << owner->name << "\'s field." << endl << endl;
	#endif
	owner->SummonToField(card);
	owner->field_size_adjust--;
	is_owning_card = false;
}

HandPutEvent::HandPutEvent(Card* _card, bool _start_of_batch, Player* _owner) : DeferredEvent(_card, true, _start_of_batch), owner(_owner)
{
}

void HandPutEvent::Process(Player* curr_player)
{
	#ifndef SUPPRESS_MATCH_PROMPTS
	if (!curr_player->is_exploration) // don't print original card owner's (card->owner) info as theoretically cards that are in transfer or being spawned/copied does not have an owner until it reaches the destination
		cout << card->name << " put to " << owner->name << "\'s hand." << endl << endl;
	#endif
	owner->PutToHand(card);
	owner->hand_size_adjust--;
	is_owning_card = false;
}

DeckShuffleEvent::DeckShuffleEvent(Card* _card, bool _start_of_batch, Player* _owner) : DeferredEvent(_card, true, _start_of_batch), owner(_owner)
{
}

void DeckShuffleEvent::Process(Player* curr_player)
{
	#ifndef SUPPRESS_MATCH_PROMPTS
	if (!curr_player->is_exploration) // don't print original card owner's (card->owner) info as theoretically cards that are in transfer or being spawned/copied does not have an owner until it reaches the destination
		cout << card->name << " shuffled to " << owner->name << "\'s deck." << endl << endl;
	#endif
	owner->ShuffleToDeck(card);
	owner->deck_size_adjust--;
	is_owning_card = false;
}

CardTransformEvent::CardTransformEvent(Card* _card, bool _start_of_batch, Card* _replacement) : DeferredEvent(_card, false, _start_of_batch), replacement(_replacement), is_owning_replacement(true)
{
}

CardTransformEvent::~CardTransformEvent()
{
	if (is_owning_replacement && replacement)
		delete replacement;
}

void CardTransformEvent::Process(Player* curr_player)
{
	// well many of the following could be put into the FlagCardTransform function but its not much of a difference for now
	#ifndef SUPPRESS_MATCH_PROMPTS
	if (!curr_player->is_exploration)
		cout << card->owner->name << "\'s " << card->name << " transformed to " << replacement->name << "." << endl << endl;
	#endif
	replacement->card_pos = card->card_pos;
	replacement->owner = card->owner;
	replacement->opponent = card->opponent;
	if (card->replacement) // just in case
		delete card->replacement;
	card->replacement = replacement; // not actually replacing yet, leave it to clear corpse
	is_owning_replacement = false; // even it is not replaced yet, the ownership of the replacement is transferred to the card instead
}

CardResetEvent::CardResetEvent(Card* _card, bool _start_of_batch) : DeferredEvent(_card, false, _start_of_batch)
{
}

void CardResetEvent::Process(Player* curr_player)
{
	// do nothing
}


/* AI Section */


ActionEntity::ActionEntity()
{
}

ActionEntity::~ActionEntity()
{
}

PlayAction::PlayAction(int _src, int _pos, int _des) : src(_src), pos(_pos), des(_des)
{
}

bool PlayAction::CheckValid(const Player* player) const
{
	int tmp_des = des; // CheckPlayValid may modify the destination to -1 if it is untargeted, this is only meaningful for human console input, AI will check if it is Targeted before hand
	return player->CheckPlayValid(src, pos, tmp_des);
}

void PlayAction::PerformAction(Player* player) const
{
	player->Play(src, pos, des);
}

AttackAction::AttackAction(int _src, int _des) : src(_src), des(_des)
{
}

bool AttackAction::CheckValid(const Player* player) const
{
	return player->CheckAttackValid(src, des);
}

void AttackAction::PerformAction(Player* player) const
{
	player->Attack(src, des);
}

EndTurnAction::EndTurnAction()
{
}

bool EndTurnAction::CheckValid(const Player* player) const
{
	return true;
}

void EndTurnAction::PerformAction(Player* player) const
{
	player->EndTurn();
}

ActionSetEntity::ActionSetEntity() : action_set(0), is_owning_actions(true)
{
}

ActionSetEntity::~ActionSetEntity()
{
	if (is_owning_actions)
		for (auto it = action_set.begin(); it != action_set.end(); it++)
			delete (*it);
}

void ActionSetEntity::UnownActions()
{
	is_owning_actions = false;
}

PlayActionSet::PlayActionSet(int _card_index) : card_index(_card_index)
{
}

int PlayActionSet::CreateValidSet(const Player* player)
{
	if (card_index <= player->field.size() + player->opponent->field.size() + 1 || card_index > player->field.size() + player->opponent->field.size() + player->hand.size() + 1)
		return 0;

	int x = card_index - (player->field.size() + player->opponent->field.size() + 2);
	Card* tmp_card = player->hand[x];

	if (tmp_card->card_type == SPELL_CARD)
	{
		if (tmp_card->isTargetedAtPlay(card_index, -1))
		{
			for (int i = 0; i <= player->field.size() + player->opponent->field.size() + player->hand.size() + 1; i++)
			{
				PlayAction* tmp_action = new PlayAction(card_index, -1, i);
				if (tmp_action->CheckValid(player))
					action_set.push_back(tmp_action);
				else
					delete tmp_action;
			}
		}
		else
		{
			PlayAction* tmp_action = new PlayAction(card_index, -1, -1);
			if (tmp_action->CheckValid(player))
				action_set.push_back(tmp_action);
			else
				delete tmp_action;
		}
	}
	else
	{
		for (int j = 0; j <= player->field.size(); j++)
		{

			if (tmp_card->isTargetedAtPlay(card_index, j))
			{
				for (int i = 0; i <= player->field.size() + player->opponent->field.size() + player->hand.size() + 1; i++)
				{
					PlayAction* tmp_action = new PlayAction(card_index, j, i);
					if (tmp_action->CheckValid(player))
						action_set.push_back(tmp_action);
					else
						delete tmp_action;
				}
			}
			else
			{
				PlayAction* tmp_action = new PlayAction(card_index, j, -1);
				if (tmp_action->CheckValid(player))
					action_set.push_back(tmp_action);
				else
					delete tmp_action;
			}
		}
	}

	return action_set.size();
}

void PlayActionSet::PerformRandomAction(Player* player) const
{
	int i = GetRandInt(action_set.size());
	action_set[i]->PerformAction(player);
}

AttackActionSet::AttackActionSet(int _card_index) : card_index(_card_index)
{
}

int AttackActionSet::CreateValidSet(const Player* player)
{
	if (card_index < 0 || card_index > player->field.size())
		return 0;

	for (int i = player->field.size() + 1; i <= player->field.size() + player->opponent->field.size() + 1; i++)
	{
		AttackAction* tmp_action = new AttackAction(card_index, i);
		if (tmp_action->CheckValid(player))
			action_set.push_back(tmp_action);
		else
			delete tmp_action;
	}

	return action_set.size();
}

void AttackActionSet::PerformRandomAction(Player* player) const
{
	int i = GetRandInt(action_set.size());
	action_set[i]->PerformAction(player);
}

EndTurnActionSet::EndTurnActionSet()
{
}

int EndTurnActionSet::CreateValidSet(const Player* player)
{
	action_set.push_back(new EndTurnAction());
	return 1;
}

void EndTurnActionSet::PerformRandomAction(Player* player) const
{
	action_set[0]->PerformAction(player);
}

KnowledgeActionNode::KnowledgeActionNode(const ActionEntity* _action) : num_visits(0), sum_eval(0.0), ave_eval(0.0), action(_action)
{
}

const ActionEntity* KnowledgeActionNode::GetAction() const
{
	return action;
}

double KnowledgeActionNode::TestAction(Player* player)
{
	// here we only do a one step evaluation mainly because of performance concerns for large batch of simulations
	// doing an meaningful multistep evaluation would require much more iterations of running this function anyway
	// also, a good way of doing multistep evaluation is to end turn and start next at each of the intermediate step and take the max along the path, which would take even much more time, as it needs state copying
	action->PerformAction(player);
	double tmp_eval = player->ForwardToNextTurnAndEval();
	num_visits++;
	sum_eval += tmp_eval;
	ave_eval = sum_eval / (double)num_visits;
	return tmp_eval;
}

KnowledgeState::KnowledgeState(Player* _player, queue<DeferredEvent*>& event_queue, PtrRedirMap& redir_map) : num_visits(0), action_list(_player->GetActionList()), action_nodes(0), num_tests_scaling(_player->ai_level), player(_player), opponent(_player->opponent)
{
	for (auto it = action_list.begin(); it != action_list.end(); it++)
		action_nodes.push_back(new KnowledgeActionNode(*it));
}

KnowledgeState::~KnowledgeState()
{
	// note: don't delete the player or opponent as they are managed in the actual game
	for (auto it = action_nodes.begin(); it != action_nodes.end(); it++)
		delete (*it);
	for (auto it = action_list.begin(); it != action_list.end(); it++)
		delete (*it);
}

const ActionEntity* KnowledgeState::DecideAction() const
{
	if (action_list.size() == 1) // in this case it is the end turn action (note in this case we don't test so it wouldn't pass the num_visits > 0 check so it has to be specially treated)
		return action_list[0];

	vector<double> action_weights(0);
	vector<int> action_indices(0);	
	for (int i = 0; i < action_nodes.size(); i++)
	{
		const KnowledgeActionNode* tmp_node = action_nodes[i];
		if (tmp_node && tmp_node->num_visits > 0) // note: only visited nodes are considered (otherwise when all visited are negative then it'll break, could initialize ave_eval with negative infinity but in case we need the worst action at some point...), normally each node should be guaranteed to be tested but just in case (e.g. when the ai level is 1)
		{
			action_weights.push_back(exp(tmp_node->ave_eval / 0.01));
			action_indices.push_back(i);
		}
	}

	int n_actions = action_weights.size();
	NormalizeProbs(action_weights.data(), n_actions);
	int choice = GetRandChoiceFromProbs(action_weights.data(), n_actions);
	if (choice == n_actions) // shouldn't happen, but just in case
		choice = n_actions - 1;
	return action_nodes[action_indices[choice]]->GetAction();
}

double KnowledgeState::TestAction(Player* player)
{
	KnowledgeActionNode* selected_node = nullptr;

	// first see if there is any unvisited child
	auto it = action_nodes.begin();
	while (it != action_nodes.end() && (*it)->num_visits > 0)
		it++;
	// if not then it should be at the "end()" at this point, we do UCB-1 for a mult-armed bandit choice; if so then the "it" should be pointing to the first unvisited child at this point, we choose that node
	if (it == action_nodes.end())
	{
		double max_priority_weight = -1e10;
		double two_ln_visits = 2.0 * log(num_visits);
		for (it = action_nodes.begin(); it != action_nodes.end(); it++)
		{
			KnowledgeActionNode* tmp_node = (*it);
			double tmp_weight = tmp_node->ave_eval + 2.0 * sqrt(two_ln_visits / (double)tmp_node->num_visits); // UCB-1, the 2.0 is for scalling up for range -1 ~ 1 as opposed to 0 ~ 1
			if (tmp_weight > max_priority_weight)
			{
				max_priority_weight = tmp_weight;
				selected_node = tmp_node;
			}
		}
	}
	else
		selected_node = (*it);

	num_visits++;
	double tmp_eval = selected_node->TestAction(player);
	return tmp_eval;
}

void KnowledgeState::PerformAction()
{
	// search/test
	int total_num_tests = (action_list.size() - 1) * num_tests_scaling; // subtract one because if there were only one action there is no need to test
	for (int i = 0; i < total_num_tests; i++)
	{
		queue<DeferredEvent*> event_queue;
		PtrRedirMap redir_map;
		Player* ally_copy = player->CreateKnowledgeCopy(COPY_ALLY, event_queue, redir_map);
		Player* oppo_copy = opponent->CreateKnowledgeCopy(COPY_OPPO, event_queue, redir_map);
		ally_copy->opponent = oppo_copy;
		oppo_copy->opponent = ally_copy;
		ally_copy->SetAllCardAfflications();
		oppo_copy->SetAllCardAfflications();
		TestAction(ally_copy);
		while (!event_queue.empty())
		{
			delete event_queue.front();
			event_queue.pop();
		}
		delete ally_copy;
		delete oppo_copy;
	}

	// execute the optimal action
	DecideAction()->PerformAction(player);
}

/* Formatted Print Section */

DescToken::~DescToken()
{
}

WordToken::WordToken(const string& _lexeme, int _curr_indent, bool _is_bold) : lexeme(_lexeme), curr_indent(_curr_indent), is_bold(_is_bold)
{
}

void WordToken::Append(string& script, int& row_index, int& column_index) const
{
	if (is_bold)
                script += "<b>";
        script += lexeme;
        if (is_bold)
                script += "</b>";
        column_index += lexeme.length();
}

bool WordToken::StartLine(string& script, int& row_index, int& column_index) const
{
	for (int i = 0; i < curr_indent; i++)
		script += "&nbsp";
	column_index = curr_indent;
	Append(script, row_index, column_index);
	return false;
}

bool WordToken::AddToLine(string& script, int& row_index, int& column_index, int max_line_len) const
{
	if (column_index + 1 + lexeme.length() > max_line_len)
	{
		script += "<br>";
		row_index++;
		return StartLine(script, row_index, column_index);
	}
	script += ' '; // single literal space performs correctly in HTML (while multiple of literal will be merged into one, therefore having to use "&nbsp" instead)
	column_index++;
	Append(script, row_index, column_index);
	return false;
}

WordToken* mkWordToken(const string& lexeme, int curr_indent, bool is_bold)
{
	return new WordToken(lexeme, curr_indent, is_bold);
}

PunctToken::PunctToken(const string& _lexeme, int _curr_indent, bool _is_bold) : lexeme(_lexeme), curr_indent(_curr_indent), is_bold(_is_bold)
{
}

void PunctToken::Append(string& script, int& row_index, int& column_index) const
{
	if (is_bold)
                script += "<b>";
        script += lexeme;
        if (is_bold)
                script += "</b>";
        column_index += lexeme.length();
}

bool PunctToken::StartLine(string& script, int& row_index, int& column_index) const
{
	for (int i = 0; i < curr_indent; i++)
		script += "&nbsp";
	column_index = curr_indent;
	Append(script, row_index, column_index);
	return false;
}

bool PunctToken::AddToLine(string& script, int& row_index, int& column_index, int max_line_len) const
{
	if (column_index + lexeme.length() > max_line_len)
	{
		script += "<br>";
		row_index++;
		return StartLine(script, row_index, column_index);
	}
	Append(script, row_index, column_index);
	return false;
}

PunctToken* mkPunctToken(const string& lexeme, int curr_indent, bool is_bold)
{
	return new PunctToken(lexeme, curr_indent, is_bold);
}

NewLineToken::NewLineToken()
{
}

void NewLineToken::Append(string& script, int& row_index, int& column_index) const
{
	script += "<br>";
	row_index++;
	column_index = 0;
}

bool NewLineToken::StartLine(string& script, int& row_index, int& column_index) const
{
	Append(script, row_index, column_index);
	return true;
}

bool NewLineToken::AddToLine(string& script, int& row_index, int& column_index, int max_line_len) const
{
	Append(script, row_index, column_index);
	return true;
}

NewLineToken* mkNewLineToken()
{
	return new NewLineToken();
}

#define NUM_FONT_LEVELS 5
// note: the character counting stats below are experimented for 100% zoom level, changing zoom level will mess up things
// also note: the browser's min font size should be set to no bigger than the smallest font here
int Font_Size[NUM_FONT_LEVELS] = {12, 11, 10, 9, 8};
int Max_Line_Len[NUM_FONT_LEVELS] = {18, 20, 22, 25, 28};
int Max_Line_Num_Char[NUM_FONT_LEVELS] = {9, 11, 12, 13, 15};
int Max_Line_Num_Spell[NUM_FONT_LEVELS] = {11, 13, 14, 15, 17};

string CardDetailHtmlScript(Card* card, int& font_size)
{
	// collect tokens from the card hierarchy
	DescTokenVec token_list(0);
	card->FillCardDescTokens(token_list, 0, false);

	// turn tokens to an HTML script, and also trying out different font levels
	int font_level;
	string script = "";
	const int* max_line_len_arr = Max_Line_Len;
	const int* max_line_num_arr = (card->card_type != SPELL_CARD ? Max_Line_Num_Char : Max_Line_Num_Spell);
	for (font_level = 0; font_level < NUM_FONT_LEVELS; font_level++)
	{
		script = "";
		int row_index = 0, column_index = 0;
		bool is_starting_line = true;
		for (const DescToken* token : token_list)
		{
			if (is_starting_line)
				is_starting_line = token->StartLine(script, row_index, column_index);
			else
				is_starting_line = token->AddToLine(script, row_index, column_index, max_line_len_arr[font_level]);
			if (row_index == max_line_num_arr[font_level])
				break;
		}
		if (row_index < max_line_num_arr[font_level])
		{
			font_size = Font_Size[font_level];
			return script;
		}
	}

	// reaching this point means no font level can fit it
	size_t last_new_line_pos = script.rfind("<br>");
	size_t snd_last_new_line_pos = script.rfind("<br>", last_new_line_pos - 4);
	script.resize(snd_last_new_line_pos + 4);
	script += "......";
	font_size = Font_Size[NUM_FONT_LEVELS - 1];

	// deallocate the tokens (which were allocated on the heap)
	for (DescToken* token : token_list)
		delete token;	

	return script;	
}

string SingleCardHtmlTableScript(Card* card)
{
	// script for content in the detail cell
	int detail_font_size;
	string detail_script = CardDetailHtmlScript(card, detail_font_size);

	// type dependent styles and simple content
	string table_starter;
	string type_cell;
	string left_right_cell;
	string detail_cell_starter;
	switch (card->card_type)
	{
	case LEADER_CARD:
		table_starter = "<table class = \"leader_card\">\n"; // the newlines are not required for the final display but good to have if want to read the script
		type_cell = "<td class = \"leader_cell\">Leader</td>\n";
		left_right_cell = "<td class = \"left_right_cell_char\"></td>\n";
		detail_cell_starter = string("<td class = \"detail_cell_char\"") + " style = \"font-size: " + IntToStr(detail_font_size) + "px\">";
		break;
	case MINION_CARD:
		table_starter = "<table class = \"minion_card\">\n";
		type_cell = string("<td class = \"minion_cell\">") + MinionTypeDescription(card->minion_type) + "</td>\n";
		left_right_cell = "<td class = \"left_right_cell_char\"></td>\n";
		detail_cell_starter = string("<td class = \"detail_cell_char\"") + " style = \"font-size: " + IntToStr(detail_font_size) + "px\">";
		break;
	case SPELL_CARD:
		table_starter = "<table class = \"spell_card\">\n";
		type_cell = "<td class = \"spell_cell\">Spell</td>\n";
		left_right_cell = "<td class = \"left_right_cell_spell\"></td>\n";
		detail_cell_starter = string("<td class = \"detail_cell_spell\"") + " style = \"font-size: " + IntToStr(detail_font_size) + "px\">";
		break;
	}

	// construct the script for the table for the card
	string script = table_starter;
	script += "<tr>\n";
	script += string("<td class = \"mana_cell\">") + IntToStr(card->mana) + "</td>\n";
	script += type_cell;
	script += "<td class = \"unused_corner_cell\"></td>\n";
	script += "</tr>\n";
	script += "<tr>\n";
	script += left_right_cell;
	script += detail_cell_starter + detail_script + "</td>\n"; 
	script += left_right_cell;
	script += "</tr>\n";
	if (card->card_type != SPELL_CARD)
	{
		script += "<tr>\n";
		script += string("<td class = \"atk_cell\">") + IntToStr(card->atk) + "</td>\n";
		script += "<td class = \"bottom_cell\"></td>\n";
		script += string("<td class = \"hp_cell\">") + IntToStr(card->max_hp - card->hp_loss) + "</td>\n";
		script += "</tr>\n";
	}
	script += "</table>\n";

	return script;
}

void AbilityFillTokens(bool is_charge, bool is_taunt, bool is_stealth, bool is_untargetable, bool is_shielded, bool is_poisonous, bool is_lifesteal, DescTokenVec& tokens, int indent_size, int& need_starting_newline)
{
	if (is_charge || is_taunt || is_stealth || is_untargetable || is_shielded || is_poisonous || is_lifesteal)
	{
		if (need_starting_newline)
			tokens.push_back(mkNewLineToken());

		if (is_charge)
		{
			tokens.push_back(mkWordToken("Charge", indent_size, true));
			tokens.push_back(mkPunctToken(",", indent_size, true));
		}
		if (is_taunt)
		{
			tokens.push_back(mkWordToken("Taunt", indent_size, true));
			tokens.push_back(mkPunctToken(",", indent_size, true));
		}
		if (is_stealth)
		{
			tokens.push_back(mkWordToken("Stealth", indent_size, true));
			tokens.push_back(mkPunctToken(",", indent_size, true));
		}
		if (is_untargetable)
		{
			tokens.push_back(mkWordToken("Untargetable", indent_size, true));
			tokens.push_back(mkPunctToken(",", indent_size, true));
		}
		if (is_shielded)
		{
			tokens.push_back(mkWordToken("Shielded", indent_size, true));
			tokens.push_back(mkPunctToken(",", indent_size, true));
		}
		if (is_poisonous)
		{
			tokens.push_back(mkWordToken("Poisonous", indent_size, true));
			tokens.push_back(mkPunctToken(",", indent_size, true));
		}
		if (is_lifesteal)
		{
			tokens.push_back(mkWordToken("Lifesteal", indent_size, true));
			tokens.push_back(mkPunctToken(",", indent_size, true));
		}

		// replace the last "," with "."
		delete tokens.back();
		tokens.back() = mkPunctToken(".", indent_size, true);

		need_starting_newline = true;
	}	
}

void AbilityFillTokensInline(bool is_charge, bool is_taunt, bool is_stealth, bool is_untargetable, bool is_shielded, bool is_poisonous, bool is_lifesteal, DescTokenVec& tokens, int indent_size)
{
	if (is_charge || is_taunt || is_stealth || is_untargetable || is_shielded || is_poisonous || is_lifesteal)
	{

		tokens.push_back(mkWordToken("with", indent_size, false));

		if (is_charge)
		{
			tokens.push_back(mkWordToken("Charge", indent_size, false));
			tokens.push_back(mkPunctToken(",", indent_size, false));
		}
		if (is_taunt)
		{
			tokens.push_back(mkWordToken("Taunt", indent_size, false));
			tokens.push_back(mkPunctToken(",", indent_size, false));
		}
		if (is_stealth)
		{
			tokens.push_back(mkWordToken("Stealth", indent_size, false));
			tokens.push_back(mkPunctToken(",", indent_size, false));
		}
		if (is_untargetable)
		{
			tokens.push_back(mkWordToken("Untargetable", indent_size, false));
			tokens.push_back(mkPunctToken(",", indent_size, false));
		}
		if (is_shielded)
		{
			tokens.push_back(mkWordToken("Shielded", indent_size, false));
			tokens.push_back(mkPunctToken(",", indent_size, false));
		}
		if (is_poisonous)
		{
			tokens.push_back(mkWordToken("Poisonous", indent_size, false));
			tokens.push_back(mkPunctToken(",", indent_size, false));
		}
		if (is_lifesteal)
		{
			tokens.push_back(mkWordToken("Lifesteal", indent_size, false));
			tokens.push_back(mkPunctToken(",", indent_size, false));
		}

		// delete the last ","
		delete tokens.back();
		tokens.pop_back();
	}	
}


/* Machine Learning Section */

void GetCardRep(Card* card, CardRep& card_rep)
{
	card->FillRep(card_rep);
}

void GetCardsReps(vector<Card*>& card_list, vector<CardRep>& card_reps)
{
	int p = card_list.size();

	card_reps.resize(p);
	for (int i = 0; i < p; i++)
		GetCardRep(card_list[i], card_reps[i]);
}

Card* CreateCardFromRep(const string& name, CardRep& card_rep)
{
	NodeRep* start_rep = card_rep.data();
	return CreateNamedCardFromRep(name, start_rep);
}

vector<Card*> CreateCardsFromReps(vector<CardRep>& card_reps)
{
	int n = card_reps.size();
	vector<Card*> cards(n);

	for (int i = 0; i < n; i++)
		cards[i] = CreateCardFromRep("Anonymous", card_reps[i]);

	return cards;
}

Card* GenerateNamedCardWithCost(const string& name, int cost)
{
	return CreateNamedCardWithCost(name, cost);
}

Card* GenerateNamedMinionWithCost(const string& name, int cost)
{
	return CreateNamedMinionWithCost(name, cost);
}

// More neural network source are in other source files
