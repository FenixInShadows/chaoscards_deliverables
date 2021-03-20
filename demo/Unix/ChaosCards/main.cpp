#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>

#include "Player.h"

using namespace std;

void BrowseCardSet(vector<Card*> cards) // cards didn't use const only because GIGL currently doesn't allow node sections to have const functions (can be made const when GIGL takes this into account)
{
	int n = cards.size();

	for (int j = 0; j < n; j++)
		cout << j << ": " << GetCardBrief(cards[j]) << endl;
	cout << endl;
	while (true)
	{
		int i;
		cout << "Input card index to view detail (input large positive index for overview; input negative index to skip): " << endl;
		while (!(cin >> i)) { cin.clear(); cin.ignore(); }
		if (i < 0)
			break;
		if (i >= n)
			for (int j = 0; j < n; j++)
				cout << j << ": " << GetCardBrief(cards[j]) << endl;
		else
			cout << endl << GetCardDetail(cards[i]) << endl;
	}
}

void BrowseCardSet(vector<Card*> cards, const vector<int> indices) // this version only browses a subset of the cards
{
	vector<Card*> selected_cards;
	for (int i : indices)
		selected_cards.push_back(cards[i]);

	BrowseCardSet(selected_cards);
}

void InteractiveMatch(Player& player_a, Player& player_b, bool rand_play_order = true)  // this assume the players are already constructed
{
	player_a.opponent = &player_b;
	player_b.opponent = &player_a;

	player_a.SetAllCardAfflications();
	player_b.SetAllCardAfflications();

	Player* player1 = &player_a;
	Player* player2 = &player_b;

	if (rand_play_order)
		DecidePlayOrder(&player_a, &player_b, player1, player2);

	cout << "Game begins." << endl;
	cout << "Input H for help on commands" << endl;

	player1->InitialCardDraw(false);
	player2->InitialCardDraw(true);

	while (true)
	{
		player1->StartTurn();
		(player1->*(player1->input_func))();
		if (player1->CheckLose() || player2->CheckLose())
			break;

		player2->StartTurn();
		(player2->*(player2->input_func))();
		if (player1->CheckLose() || player2->CheckLose())
			break;
	}

	if (player1->CheckLose())
	{
		if (player2->CheckLose())
			cout << "It is a draw." << endl;
		else
			cout << player2->name << " won." << endl;
	}
	else
		cout << player1->name << " won." << endl;

	cout << "Game ends." << endl;
}

void TestAIs(int ai_level_a, int ai_level_b, vector<Card*>& card_list, const vector<vector<int>>& deck_list, int deck_num, int deck_size) // deck_list stores indices in the seed_list, not the seeds themselves
{
	BalanceStat ai_stat_a, ai_stat_b;
	for (int i = 0; i < deck_num; i++)
	{
		for (int j = 0; j < deck_num; j++)
		{
			cout << "AI_A Deck " << i << " VS AI_B Deck " << j << ". ";

			vector<int> deck_a_shuffle(deck_size);
			vector<int> deck_b_shuffle(deck_size);
			vector<int> deck_a_indices(deck_size);
			vector<int> deck_b_indices(deck_size);
			vector<Card*> deck_a(deck_size);
			vector<Card*> deck_b(deck_size);

			InitMatch(card_list, deck_list[i], deck_list[j], deck_a_shuffle, deck_b_shuffle, deck_a_indices, deck_b_indices, deck_a, deck_b);

			queue<DeferredEvent*> event_queue;
			Player player1("AI_A", 30, deck_a, true, event_queue, ai_level_a);
			Player player2("AI_B", 30, deck_b, true, event_queue, ai_level_b);

			player1.opponent = &player2;
			player2.opponent = &player1;

			player1.SetAllCardAfflications();
			player2.SetAllCardAfflications();

			player1.InitialCardDraw(false);
			player2.InitialCardDraw(true);

			while (true)
			{
				player1.StartTurn();
				(player1.*(player1.input_func))();
				if (player1.CheckLose() || player2.CheckLose())
					break;

				player2.StartTurn();
				(player2.*(player2.input_func))();
				if (player1.CheckLose() || player2.CheckLose())
					break;
			}

			while (!event_queue.empty())
			{
				delete event_queue.front(); // note: this is not deleting the actual card but the entity for flagging
				event_queue.pop();
			}

			if (player1.CheckLose())
			{
				if (player2.CheckLose())
				{
					// draw
					ai_stat_a.DrawUpdate(1.0);
					ai_stat_b.DrawUpdate(1.0);
					cout << "They drew." << endl;
				}
				else
				{
					// AI_A loses, deck AI_B wins
					ai_stat_a.LoseUpdate(1.0);
					ai_stat_b.WinUpdate(1.0);
					cout << "AI_B won." << endl;
				}
			}
			else
			{
				// AI_A wins, deck AI_B loses
				ai_stat_a.WinUpdate(1.0);
				ai_stat_b.LoseUpdate(1.0);
				cout << "AI_A won." << endl;
			}

		}
	}

	ai_stat_a.UpdateEval();
	cout << "AI_A level: " << ai_level_a << endl;
	cout << "AI_A total matches: " << ai_stat_a.total_num << endl;
	cout << "AI_A wins: " << ai_stat_a.num_wins << endl;
	cout << "AI_A losses: " << ai_stat_a.num_losses << endl;
	cout << "AI_A eval (win rate, counting draws as half a win): " << ai_stat_a.eval << endl;
	ai_stat_b.UpdateEval();
	cout << "AI_B level: " << ai_level_b << endl;
	cout << "AI_B total matches: " << ai_stat_b.total_num << endl;
	cout << "AI_B wins: " << ai_stat_b.num_wins << endl;
	cout << "AI_B losses: " << ai_stat_b.num_losses << endl;
	cout << "AI_B eval (win rate, counting draws as half a win): " << ai_stat_b.eval << endl;
}


int SimulateSingleMatchBetweenDecks(int ai_level, vector<Card*>& card_list, const vector<int>& deck_a_orig_indices, const vector<int>& deck_b_orig_indices, vector<BalanceStat>& card_stats, BalanceStat& deck_a_stat, BalanceStat& deck_b_stat, MatchRec& rec, bool is_inverted, int deck_size) // return number of turns when the match ends, both sides summed
{
	vector<int> deck_a_shuffle(deck_size);
	vector<int> deck_b_shuffle(deck_size);
	vector<int> deck_a_indices(deck_size);
	vector<int> deck_b_indices(deck_size);
	vector<Card*> deck_a(deck_size);
	vector<Card*> deck_b(deck_size);

	InitMatch(card_list, deck_a_orig_indices, deck_b_orig_indices, deck_a_shuffle, deck_b_shuffle, deck_a_indices, deck_b_indices, deck_a, deck_b);

	queue<DeferredEvent*> event_queue;
	Player player1("AI_Deck_A", 30, deck_a, true, event_queue, ai_level);
	Player player2("AI_Deck_B", 30, deck_b, true, event_queue, ai_level);

	vector<int> contribution_counters_a(deck_size, 0);
	player1.RegisterCardContributions(contribution_counters_a);
	vector<int> contribution_counters_b(deck_size, 0);
	player2.RegisterCardContributions(contribution_counters_b);

	player1.opponent = &player2;
	player2.opponent = &player1;

	player1.SetAllCardAfflications();
	player2.SetAllCardAfflications();

	player1.InitialCardDraw(false);
	player2.InitialCardDraw(true);

	while (true)
	{
		player1.StartTurn();
		(player1.*(player1.input_func))();
		if (player1.CheckLose() || player2.CheckLose())
			break;

		player2.StartTurn();
		(player2.*(player2.input_func))();
		if (player1.CheckLose() || player2.CheckLose())
			break;
	}

	while (!event_queue.empty())
	{
		delete event_queue.front(); // note: this is not deleting the actual card but the entity for flagging
		event_queue.pop();
	}

	int sum_contribution_a = 0;
	for (int k = 0; k < deck_size; k++)
		sum_contribution_a += contribution_counters_a[k];
	int sum_contribution_b = 0;
	for (int k = 0; k < deck_size; k++)
		sum_contribution_b += contribution_counters_b[k];

	vector<double> processed_contribution_a(deck_size); // recorded to orginal card order before shuffling, also normalize (only used for match records)
	for (int k = 0; k < deck_size; k++)
		processed_contribution_a[k] = (double)contribution_counters_a[deck_a_shuffle[k]] / (double)sum_contribution_a;
	vector<double> processed_contribution_b(deck_size);
	for (int k = 0; k < deck_size; k++)
		processed_contribution_b[k] = (double)contribution_counters_b[deck_b_shuffle[k]] / (double)sum_contribution_b;

	if (player1.CheckLose())
	{
		if (player2.CheckLose())
		{
			// draw
			rec.DrawUpdate(processed_contribution_a, processed_contribution_b);
			deck_a_stat.DrawUpdate(1.0);
			deck_b_stat.DrawUpdate(1.0);
			for (int k = 0; k < deck_size; k++)
			{
				card_stats[deck_a_indices[k]].DrawUpdate((double)contribution_counters_a[k] / (double)sum_contribution_a);
				card_stats[deck_b_indices[k]].DrawUpdate((double)contribution_counters_b[k] / (double)sum_contribution_b);
			}
			cout << "They drew." << endl;
		}
		else
		{
			// deck i loses, deck j wins
			if (is_inverted)
				rec.LoseUpdate(processed_contribution_b, processed_contribution_a);
			else
				rec.WinUpdate(processed_contribution_a, processed_contribution_b);
			deck_a_stat.LoseUpdate(1.0);
			deck_b_stat.WinUpdate(1.0);
			for (int k = 0; k < deck_size; k++)
			{
				card_stats[deck_a_indices[k]].LoseUpdate((double)contribution_counters_a[k] / (double)sum_contribution_a);
				card_stats[deck_b_indices[k]].WinUpdate((double)contribution_counters_b[k] / (double)sum_contribution_b);
			}
			cout << "The second deck won." << endl;
		}
	}
	else
	{
		// deck i wins, deck j loses
		if (is_inverted)
			rec.WinUpdate(processed_contribution_b, processed_contribution_a);
		else
			rec.LoseUpdate(processed_contribution_a, processed_contribution_b);
		deck_a_stat.WinUpdate(1.0);
		deck_b_stat.LoseUpdate(1.0);
		for (int k = 0; k < deck_size; k++)
		{
			card_stats[deck_a_indices[k]].WinUpdate((double)contribution_counters_a[k] / (double)sum_contribution_a);
			card_stats[deck_b_indices[k]].LoseUpdate((double)contribution_counters_b[k] / (double)sum_contribution_b);
		}
		cout << "The first deck won." << endl;
	}

	return player1.turn_num + player2.turn_num;
}

int SimulatePairMatchBetweenDecks(int ai_level, vector<Card*>& card_list, const vector<int>& deck_a_orig_indices, const vector<int>& deck_b_orig_indices, vector<BalanceStat>& card_stats, BalanceStat& deck_a_stat, BalanceStat& deck_b_stat, MatchRec& rec, int deck_size) // return total number of match turns (both sides summed)
{
	// test both the playing order (which player plays first and with plays second)
	int turn_count_1 = SimulateSingleMatchBetweenDecks(ai_level, card_list, deck_a_orig_indices, deck_b_orig_indices, card_stats, deck_a_stat, deck_b_stat, rec, false, deck_size);
	int turn_count_2 = SimulateSingleMatchBetweenDecks(ai_level, card_list, deck_b_orig_indices, deck_a_orig_indices, card_stats, deck_b_stat, deck_a_stat, rec, true, deck_size);
	return turn_count_1 + turn_count_2;
}

int TestNewDeck(int ai_level, vector<Card*>& card_list, vector<vector<int>>& deck_list, vector<BalanceStat>& card_stats, vector<BalanceStat>& deck_stats, vector<MatchRec>& rec_list, int deck_size, double temperature, int num_pair_matches, int& deck_count, int& match_pair_count) // return total number of match turns (both sides summed)
{
	int p = card_list.size();
	int n_decks = deck_list.size();
	
	/* creating the new deck*/
	vector<int> new_deck(deck_size);
	double two_ln_visits = 2.0 * log(4.0 * match_pair_count * deck_size); // the natural logarithm of total number of times any card is tested so far
	vector<double> weights(p);
	vector<int> unvisited_indices;
	for (int i = 0; i < p; i++)
	{
		int tmp_visits = card_stats[i].total_num;
		if (tmp_visits == 0) // not tested at all, mark as -1
			unvisited_indices.push_back(i);
		else
			weights[i] = card_stats[i].eval + sqrt(two_ln_visits/(double)tmp_visits);
	}

	int n_unvisited = unvisited_indices.size();
	if (n_unvisited >= deck_size) // if there are enough unvisited ones, choose among them
	{
		vector<int> selected_unvisited_indices = CreateRandomSelection(n_unvisited, deck_size);
		for (int i = 0; i < deck_size; i++)
			new_deck[i] = unvisited_indices[selected_unvisited_indices[i]];
		sort(new_deck.begin(), new_deck.end());	
	}
	else // otherwise first choose all unvisited ones, then choose the remaining with max UCB weights
	{
		for (int i = 0; i < n_unvisited; i++)
		{
			int tmp_index = unvisited_indices[i];
			new_deck[i] = tmp_index;
			weights[tmp_index] = -10.0; // mark as already selected
		}
		for (int i = n_unvisited; i < deck_size; i++)
		{
			double max_weight = -10.0;
			double selected_card_index = -1; // will be overwritten
			for (int j = 0; j < p; j++)
				if (weights[j] > max_weight)
				{
					max_weight = weights[j];
					selected_card_index = j;
				}
			new_deck[i] = selected_card_index;
			weights[selected_card_index] = -10.0; // mark as already selected
		}
		sort(new_deck.begin(), new_deck.end());
	}

	/* check if the mutated deck is a duplicate of any existing deck */
	int duplicate_index = -1;
	for (int i = 0; i < n_decks; i++)
		if (new_deck == deck_list[i]) // comparing two vector of ints; as they are expected to be sorted so that makes sense here
		{
			duplicate_index = i;
			break;	
		}

	/* perform tests, update the deck pool if necessary */
	int turn_count = 0;
	if (duplicate_index >= 0) // if it is a duplicate, just re-test
	{
		for (int i = 0; i < n_decks; i++)
			if (i != duplicate_index)
			{
				MatchRec rec(deck_list[i], deck_list[duplicate_index]);
				for (int k = 0; k < num_pair_matches; k++)
				{
					cout << "Match Pair #" << ++match_pair_count << endl;
					turn_count += SimulatePairMatchBetweenDecks(ai_level, card_list, deck_list[i], deck_list[duplicate_index], card_stats, deck_stats[i], deck_stats[duplicate_index], rec, deck_size);
				}
				rec.UpdateStats();
				rec_list.push_back(rec);
			}
		UpdateStatEvals(deck_stats);
	}
	else // if it is not a duplicate, test and decide whether to replace the worst among other decks
	{
		deck_count++;
		BalanceStat tmp_deck_stat;
		for (int i = 0; i < n_decks; i++)
		{
			MatchRec rec(deck_list[i], new_deck);
			for (int k = 0; k < num_pair_matches; k++)
			{
				cout << "Match Pair #" << ++match_pair_count << endl;
				turn_count += SimulatePairMatchBetweenDecks(ai_level, card_list, deck_list[i], new_deck, card_stats, deck_stats[i], tmp_deck_stat, rec, deck_size);
			}
			rec.UpdateStats();
			rec_list.push_back(rec);
		}
		UpdateStatEvals(deck_stats);
		tmp_deck_stat.UpdateEval();

		// find the worst among other decks
		double min_eval = 10.0;
		int min_index = -1; // this should always be overwritten
		for (int i = 0; i < n_decks; i++)
			if (deck_stats[i].eval < min_eval)
			{
				min_eval = deck_stats[i].eval;
				min_index = i;
			}	
		
		// use Metropolis-MC like rejection scheme to decide whether to accept the replacement or not
		double accept_prob = exp((tmp_deck_stat.eval - min_eval)/temperature);
		double roll = GetGiglRandFloat();
		if (roll < accept_prob) // accept and replace, otherwise don't do the replacement
		{
			deck_list[min_index] = new_deck;
			deck_stats[min_index] = tmp_deck_stat;
			cout << "Update accepted." << endl;
		}
		else
		{
			cout << "Update rejected." << endl;
		}
	}

	return turn_count;
}

int TestCrossOver(int ai_level, vector<Card*>& card_list, vector<vector<int>>& deck_list, vector<BalanceStat>& card_stats, vector<BalanceStat>& deck_stats, vector<MatchRec>& rec_list, int deck_size, double temperature, int num_pair_matches, int& deck_count, int& match_pair_count) // return total number of match turns (both sides summed)
{
	int n_decks = deck_list.size();

	/* creating the cross over offsprings (two) */
	vector<int> selected_deck_indices = CreateRandomSelection(n_decks, 2);
	vector<vector<int>> deck_copies(2);
	for (int i = 0; i < 2; i++)
		deck_copies[i] = deck_list[selected_deck_indices[i]];

	// first figure out cards not shared by the two decks, then decide which ones to swap
	int i0 = 0, i1 = 0;
	vector<int> diff_indices0, diff_indices1;
	while (i0 < deck_size && i1 < deck_size)
	{
		if (deck_copies[0][i0] == deck_copies[1][i1])
		{
			i0++;
			i1++;
		}
		else if (deck_copies[0][i0] < deck_copies[1][i1])
		{
			do
			{
				diff_indices0.push_back(i0);
				i0++;
			}
			while (i0 < deck_size && deck_copies[0][i0] < deck_copies[1][i1]);
		}
		else
		{
			do
			{
				diff_indices1.push_back(i1);
				i1++;
			}
			while (i1 < deck_size && deck_copies[0][i0] > deck_copies[1][i1]);
		}
	}
	// don't forget the remaining unmatched indices
	for (; i0 < deck_size; i0++)
		diff_indices0.push_back(i0);
	for (; i1 < deck_size; i1++)
		diff_indices1.push_back(i1);
	int diff_size = diff_indices0.size(); // should be the same for 0 and 1
	vector<int> index_map = CreateRandomSelection(diff_size, diff_size); // essentially a random shuffle on indices
	for (int i = 0; i < diff_size; i++)
	{
		double roll = GetGiglRandFloat();
		if (roll < 0.5) // 50% chance to swap (if nothing ends up swapping we'll just test the two decks again at the end, not a problem)
		{
			int i0 = diff_indices0[i];
			int i1 = diff_indices1[index_map[i]];
			int tmp = deck_copies[0][i0];
			deck_copies[0][i0] = deck_copies[1][i1];
			deck_copies[1][i1] = tmp;
		}
	}
	for (int i = 0; i < 2; i++)
		sort(deck_copies[i].begin(), deck_copies[i].end());	
	
	/* check duplicates */
	vector<int> duplicate_indices(2, -1);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < n_decks; j++)
			if (deck_copies[i] == deck_list[j]) // comparing two vector of ints; as they are expected to be sorted so that makes sense here
			{
				duplicate_indices[i] = j;
				break;	
			}
	}
	
	/* perform tests */
	int turn_count = 0;
	vector<BalanceStat> tmp_deck_stats(2);
	for (int i = 0; i < 2; i++)
	{
		if (duplicate_indices[i] >= 0) // if it is a duplicate
		{
			for (int j = 0; j < n_decks; j++)
				if (j != duplicate_indices[i])
				{
					MatchRec rec(deck_list[j], deck_list[duplicate_indices[i]]);
					for (int k = 0; k < num_pair_matches; k++)
					{
						cout << "Match Pair #" << ++match_pair_count << endl;
						turn_count += SimulatePairMatchBetweenDecks(ai_level, card_list, deck_list[j], deck_list[duplicate_indices[i]], card_stats, deck_stats[j], deck_stats[duplicate_indices[i]], rec, deck_size);
					}
					rec.UpdateStats();
					rec_list.push_back(rec);
				}
			UpdateStatEvals(deck_stats);
		}
		else // if it is not a duplicate
		{
			deck_count++;
			for (int j = 0; j < n_decks; j++)
			{
				MatchRec rec(deck_list[j], deck_copies[i]);
				for (int k = 0; k < num_pair_matches; k++)
				{
					cout << "Match Pair #" << ++match_pair_count << endl;
					turn_count += SimulatePairMatchBetweenDecks(ai_level, card_list, deck_list[j], deck_copies[i], card_stats, deck_stats[j], tmp_deck_stats[i], rec, deck_size);
				}
				rec.UpdateStats();
				rec_list.push_back(rec);
			}
			UpdateStatEvals(deck_stats);
			tmp_deck_stats[i].UpdateEval();
		}	
	}
	// if both are not duplicates, do a set of matches between them
	if (duplicate_indices[0] < 0 && duplicate_indices[1] < 0)
	{
		MatchRec rec(deck_copies[0], deck_copies[1]);
		for (int k = 0; k < num_pair_matches; k++)
		{
			cout << "Match Pair #" << ++match_pair_count << endl;
			turn_count += SimulatePairMatchBetweenDecks(ai_level, card_list, deck_copies[0], deck_copies[1], card_stats, tmp_deck_stats[0], tmp_deck_stats[1], rec, deck_size);
		}
		rec.UpdateStats();
		rec_list.push_back(rec);
		tmp_deck_stats[0].UpdateEval();
		tmp_deck_stats[1].UpdateEval();
	}

	/* update the deck pool if necessary */
	// replacement logic:
	// if both offsprings are duplicates, no replacement needed
	// if one offspring is a duplicate, decide whether to replace the weaker of the two parents or not
	// if none of them is a duplicate, decide whether to replace both parents or not
	if (duplicate_indices[0] < 0)
	{
		if (duplicate_indices[1] < 0) // both decks are new
		{
			// use Metropolis-MC like rejection scheme to decide whether to accept the replacement or not
			double accept_prob = exp(((tmp_deck_stats[0].eval + tmp_deck_stats[1].eval) - (deck_stats[selected_deck_indices[0]].eval + deck_stats[selected_deck_indices[1]].eval))/temperature);
			double roll = GetGiglRandFloat();
			if (roll < accept_prob) // accept and replace, otherwise don't do the replacement
			{
				for (int i = 0; i < 2; i++)
				{
					deck_list[selected_deck_indices[i]] = deck_copies[i];
					deck_stats[selected_deck_indices[i]] = tmp_deck_stats[i];
				}
				cout << "Update accepted." << endl;
			}
			else
			{
				cout << "Update rejected." << endl;
			}
		}
		else // only the first deck is new
		{
			// first figure out the weaker of the two parents
			int replace_target = (deck_stats[selected_deck_indices[0]].eval <= deck_stats[selected_deck_indices[1]].eval ? selected_deck_indices[0] : selected_deck_indices[1]);
			// use Metropolis-MC like rejection scheme to decide whether to accept the replacement or not
			double accept_prob = exp((tmp_deck_stats[0].eval - deck_stats[replace_target].eval)/temperature);
			double roll = GetGiglRandFloat();
			if (roll < accept_prob) // accept and replace, otherwise don't do the replacement
			{
				deck_list[replace_target] = deck_copies[0];
				deck_stats[replace_target] = tmp_deck_stats[0];
				cout << "Update accepted." << endl;
			}
			else
			{
				cout << "Update rejected." << endl;
			}
		}
	}
	else
	{
		if (duplicate_indices[1] < 0) // only the second deck is new
		{
			// first figure out the weaker of the two parents
			int replace_target = (deck_stats[selected_deck_indices[0]].eval <= deck_stats[selected_deck_indices[1]].eval ? selected_deck_indices[0] : selected_deck_indices[1]);
			// use Metropolis-MC like rejection scheme to decide whether to accept the replacement or not
			double accept_prob = exp((tmp_deck_stats[1].eval - deck_stats[replace_target].eval)/temperature);
			double roll = GetGiglRandFloat();
			if (roll < accept_prob) // accept and replace, otherwise don't do the replacement
			{
				deck_list[replace_target] = deck_copies[1];
				deck_stats[replace_target] = tmp_deck_stats[1];
				cout << "Update accepted." << endl;
			}
			else
			{
				cout << "Update rejected." << endl;
			}
		}
		// else do nothing
	}

	return turn_count;
}

int TestMutation(int ai_level, vector<Card*>& card_list, vector<vector<int>>& deck_list, vector<BalanceStat>& card_stats, vector<BalanceStat>& deck_stats, vector<MatchRec>& rec_list, int deck_size, double temperature, int num_pair_matches, int deck_count, int& match_pair_count) // return total number of match turns (both sides summed)
{
	int p = card_list.size();
	int n_decks = deck_list.size();

	/* creating the mutated deck*/	
	int selected_deck_index = GetGiglRandInt(n_decks);
	vector<int> deck_copy = deck_list[selected_deck_index];
	double two_ln_visits = 2.0 * log(4.0 * match_pair_count * deck_size); // the natural logarithm of total number of times any card is tested so far
	vector<double> weights(p);
	vector<int> unvisited_indices;
	for (int i = 0; i < p; i++)
	{
		int tmp_visits = card_stats[i].total_num;
		if (tmp_visits == 0) // not tested at all, mark as -1
			unvisited_indices.push_back(i); // we can decide here (instead of needing to wait for checking/clearing for cards in the selected deck) because any cards in the existing decks should all be already tested
		else
			weights[i] = card_stats[i].eval + sqrt(two_ln_visits/(double)tmp_visits);
	}
	
	int selected_card_index;
	int n_unvisited = unvisited_indices.size();
	if (n_unvisited > 0) // there are unvisited cards: get one randomly from them
	{
		selected_card_index = unvisited_indices[GetGiglRandInt(n_unvisited)];
	}
	else // no unvisited cards: select one with max UCB weight (but not any card from the selected deck to mutate from)
	{
		for (int i = 0; i < deck_size; i++) // clearing the weights of any cards in the selected deck because we don't want duplicate or identity mutation
			weights[deck_copy[i]] = -10.0;  // using the copied deck as the original as it shouldn't have been modified up to this point
		double max_weight = -10.0;
		selected_card_index = -1; // will be overwritten
		for (int i = 0; i < p; i++)
			if (weights[i] > max_weight)
			{
				max_weight = weights[i];
				selected_card_index = i;
			}
	}
	
	int replaced_index_in_deck = GetGiglRandInt(deck_size);
	deck_copy[replaced_index_in_deck] = selected_card_index;
	// keep the card index in the deck sorted (increasing)
	int k = replaced_index_in_deck; 
	while(k > 0 && deck_copy[k] < deck_copy[k - 1])
	{
		int tmp = deck_copy[k];
		deck_copy[k] = deck_copy[k - 1];
		deck_copy[k - 1] = tmp;
		k--;
	}
	while(k < deck_size - 1 && deck_copy[k] > deck_copy[k + 1])
	{
		int tmp = deck_copy[k];
		deck_copy[k] = deck_copy[k + 1];
		deck_copy[k + 1] = tmp;
		k++;
	}

	/* check if the mutated deck is a duplicate of any existing deck */
	int duplicate_index = -1;
	for (int i = 0; i < n_decks; i++)
		if (deck_copy == deck_list[i]) // comparing two vector of ints; as they are expected to be sorted so that makes sense here
		{
			duplicate_index = i;
			break;	
		}

	/* perform tests, update the deck pool if necessary */
	int turn_count = 0;
	if (duplicate_index >= 0) // if it is a duplicate, just re-test
	{
		for (int i = 0; i < n_decks; i++)
			if (i != duplicate_index)
			{
				MatchRec rec(deck_list[i], deck_list[duplicate_index]);
				for (int k = 0; k < num_pair_matches; k++)
				{
					cout << "Match Pair #" << ++match_pair_count << endl;
					turn_count += SimulatePairMatchBetweenDecks(ai_level, card_list, deck_list[i], deck_list[duplicate_index], card_stats, deck_stats[i], deck_stats[duplicate_index], rec, deck_size);
				}
				rec.UpdateStats();
				rec_list.push_back(rec);
			}
		UpdateStatEvals(deck_stats);
	}
	else // if it is not a duplicate, test and decide whether to replace the original deck it mutates from
	{
		deck_count++;
		BalanceStat tmp_deck_stat;
		for (int i = 0; i < n_decks; i++)
		{
			MatchRec rec(deck_list[i], deck_copy);
			for (int k = 0; k < num_pair_matches; k++)
			{
				cout << "Match Pair #" << ++match_pair_count << endl;
				turn_count += SimulatePairMatchBetweenDecks(ai_level, card_list, deck_list[i], deck_copy, card_stats, deck_stats[i], tmp_deck_stat, rec, deck_size);
			}
			rec.UpdateStats();
			rec_list.push_back(rec);
		}
		UpdateStatEvals(deck_stats);
		tmp_deck_stat.UpdateEval();
		
		// use Metropolis-MC like rejection scheme to decide whether to accept the replacement or not
		double accept_prob = exp((tmp_deck_stat.eval - deck_stats[selected_deck_index].eval)/temperature);
		double roll = GetGiglRandFloat();
		if (roll < accept_prob) // accept and replace, otherwise don't do the replacement
		{
			deck_list[selected_deck_index] = deck_copy;
			deck_stats[selected_deck_index] = tmp_deck_stat;
			cout << "Update accepted." << endl;
		}
		else
		{
			cout << "Update rejected." << endl;
		}
	}

	return turn_count;
}

void WriteCardData(vector<Card*>& cards, const vector<CardRep>& card_reps, const vector<BalanceStat>& card_stats, const char* filename)
{
	int p = card_reps.size();

	ofstream fs(filename);
	fs << p << endl;
	for (int i = 0; i < p; i++)
	{
		fs << GetCardName(cards[i]) << endl;
		int tmp_rep_len = card_reps[i].size();
		fs << tmp_rep_len << ":: ";
		for (int j = 0; j < tmp_rep_len; j++)
			fs << card_reps[i][j].GetPrintVersion();
		fs << endl << card_stats[i].eval << " "
			<< card_stats[i].num_wins << " " << card_stats[i].num_losses << " " << card_stats[i].total_num << " "
			<< card_stats[i].win_contribution << " " << card_stats[i].total_participation << endl;
	}
	fs.close();
	fs.clear();
}

void WriteCardDataHuman(vector<Card*>& cards, const vector<CardRep>& card_reps, const vector<BalanceStat>& card_stats, const char* filename) // more human readable verion, with card info
{
	int p = card_reps.size();

	ofstream fs(filename);
	fs << p << endl << endl << endl;
	for (int i = 0; i < p; i++)
	{
		fs << GetCardDetail(cards[i]) << endl << endl;
		fs << "Eval (win rate): " << card_stats[i].eval << endl;
		fs << "Num wins: " << card_stats[i].num_wins << endl;
		fs << "Num losses: " << card_stats[i].num_losses << endl;
		fs << "Total num: " << card_stats[i].total_num << endl;
		fs << "Win contribution: " << card_stats[i].win_contribution << endl;
		fs << "Total participation: " << card_stats[i].total_participation << endl << endl << endl;
	}
	fs.close();
	fs.clear();
}

void WriteDataDeckSorted(const vector<vector<int>>& deck_list, const vector<BalanceStat>& deck_stats, const vector<int>& sorted_deck_indices, int start_rank, int rank_step, int num_outputs, const char* filename) // Note: the rank passed in to the function should be zero-indexed (while in the file output it is one-indexed
{
	int deck_num = deck_list.size();
	int deck_size = deck_list[0].size();

	ofstream fs(filename);
	fs << num_outputs << endl;
	fs << deck_size << endl;
	int rank = start_rank;
	int i = 0;
	for (; i < num_outputs && rank < deck_num; i++, rank += rank_step)
	{
		int index = sorted_deck_indices[rank];
		// index in the card seed list
		for (int j = 0; j < deck_size; j++)
			fs << deck_list[index][j] << " ";
		fs << endl;
		// other info
		fs << rank + 1 << " " << deck_stats[index].eval << " "
			<< deck_stats[index].num_wins << " " << deck_stats[index].num_losses << " " << deck_stats[index].total_num << " " << endl << endl;
	}
	if (i < num_outputs && rank >= deck_num)
		cout << "Warning: not enough decks for outputting." << endl;
	fs.close();
	fs.clear();		
}

#define MAX_STR_LEN 256

void ReadCardData(vector<Card*>& cards, vector<CardRep>& card_reps, vector<BalanceStat>& card_stats, const char* filename)
{
	// often the match data file and read from it
	ifstream fs(filename);
	if (!fs.is_open())
	{
		cout << "Error occurred when opening data file (due to file existence or permission issues)" << endl;
		fs.clear();
		exit(1);
	}
	int p;
	fs >> p;
	cards.resize(p);
	card_reps.resize(p);
	card_stats.resize(p);
	for (int i = 0; i < p; i++)
	{
		fs.ignore(MAX_STR_LEN, '\n'); // skip the trailing newline from the previous line
		char tmp_name[MAX_STR_LEN];
		fs.getline(tmp_name, MAX_STR_LEN); // get the name
		int tmp_rep_len;
		fs >> tmp_rep_len;
		char tmp_ch;
		fs >> tmp_ch >> tmp_ch; // read two colons
		for (int j = 0; j < tmp_rep_len; j++)
		{
			int tmp_choice;
			fs >> tmp_choice;
			fs >> tmp_ch; // read a comma
			int tmp_term_len;
			fs >> tmp_term_len;
			fs >> tmp_ch; // read a colon
			vector<double> tmp_term_info(tmp_term_len);
			for (int k = 0; k < tmp_term_len; k++)
				fs >> tmp_term_info[k];
			fs >> tmp_ch; // read a semicolon
			card_reps[i].push_back(NodeRep(tmp_choice, tmp_term_info));
		}

		cards[i] = CreateCardFromRep(string(tmp_name), card_reps[i]);

		fs >> card_stats[i].eval
			>> card_stats[i].num_wins >> card_stats[i].num_losses >> card_stats[i].total_num
			>> card_stats[i].win_contribution >> card_stats[i].total_participation;
	}
	fs.close();
	fs.clear();
}

void ReadDeckData(vector<vector<int>>& deck_list, vector<BalanceStat>& deck_stats, const char* filename)
{
	ifstream fs(filename);
	if (!fs.is_open())
	{
		cout << "Error occurred when opening data file (due to file existence or permission issues)" << endl;
		fs.clear();
		exit(1);
	}
	int deck_num;
	int deck_size; 
	fs >> deck_num >> deck_size;
	deck_list.resize(deck_num);
	deck_stats.resize(deck_num);
	for (int i = 0; i < deck_num; i++)
	{
		// index in the card seed list
		deck_list[i].resize(deck_size);
		for (int j = 0; j < deck_size; j++)
			fs >> deck_list[i][j];
		// other info
		int rank;
		fs >> rank // not recorded
			>> deck_stats[i].eval
			>> deck_stats[i].num_wins >> deck_stats[i].num_losses >> deck_stats[i].total_num;
		deck_stats[i].win_contribution = deck_stats[i].num_wins - deck_stats[i].num_losses;
		deck_stats[i].total_participation = deck_stats[i].total_num;
	}
	fs.close();
	fs.clear();
}

void PrepareCardData(vector<Card*>& cards, const vector<BalanceStat>& card_stats, vector<CardRep>& card_reps, vector<double>& card_strengths, vector<double>& card_weights, const char* filename, const char* filename_human) // pass in nullptrs to the filenames to indicate not saving to files
{
	int p = cards.size();

	GetCardsReps(cards, card_reps);

	card_strengths.resize(p);
	card_weights.resize(p);
	for (int i = 0; i < p; i++)
	{
		card_strengths[i] = card_stats[i].eval;
		card_weights[i] = card_stats[i].total_participation;
	}

	if (filename && filename_human)
	{
		// save the match card data to a file (no card info)
		WriteCardData(cards, card_reps, card_stats, filename);

		// save the match card data with card info to a file in a human readable format
		WriteCardDataHuman(cards, card_reps, card_stats, filename_human);
	}
}

void WriteMatchRecords(const vector<MatchRec>& recs, int deck_size, const char* filename)
{
	int n_recs = recs.size();

	ofstream fs(filename);
	fs << n_recs << endl << endl;
	for (int i = 0; i < n_recs; i++)
	{
		for (int j = 0; j < deck_size; j++)
			fs << recs[i].deck_a_indices[j] << " ";
		fs << endl;
		for (int j = 0; j < deck_size; j++)
			fs << recs[i].deck_b_indices[j] << " ";
		fs << endl;
		fs << recs[i].win_weight << endl;
		fs << recs[i].weight << endl;
		fs << recs[i].win_rate << endl;
		for (int j = 0; j < deck_size; j++)
			fs << recs[i].sum_win_contrib_a[j] << " ";
		fs << endl;
		for (int j = 0; j < deck_size; j++)
			fs << recs[i].ave_win_contrib_a[j] << " ";
		fs << endl;
		for (int j = 0; j < deck_size; j++)
			fs << recs[i].sum_total_partic_a[j] << " ";
		fs << endl;
		for (int j = 0; j < deck_size; j++)
			fs << recs[i].ave_total_partic_a[j] << " ";
		fs << endl;
		for (int j = 0; j < deck_size; j++)
			fs << recs[i].sum_win_contrib_b[j] << " ";
		fs << endl;
		for (int j = 0; j < deck_size; j++)
			fs << recs[i].ave_win_contrib_b[j] << " ";
		fs << endl;
		for (int j = 0; j < deck_size; j++)
			fs << recs[i].sum_total_partic_b[j] << " ";
		fs << endl;
		for (int j = 0; j < deck_size; j++)
			fs << recs[i].ave_total_partic_b[j] << " ";
		fs << endl;
		fs << endl;
	}
	fs.close();
	fs.clear();
}

void ReadMatchRecords(vector<MatchRec>& recs, int deck_size, const char* filename)
{
	ifstream fs(filename);
	int n_recs;
	fs >> n_recs;
	recs.resize(n_recs);
	for (int i = 0; i < n_recs; i++)
	{
		recs[i].deck_a_indices.resize(deck_size);
		for (int j = 0; j < deck_size; j++)
			fs >> recs[i].deck_a_indices[j];
		recs[i].deck_b_indices.resize(deck_size);
		for (int j = 0; j < deck_size; j++)
			fs >> recs[i].deck_b_indices[j];
		fs >> recs[i].win_weight;
		fs >> recs[i].weight;
		fs >> recs[i].win_rate;
		recs[i].sum_win_contrib_a.resize(deck_size);
		for (int j = 0; j < deck_size; j++)
			fs >> recs[i].sum_win_contrib_a[j];
		recs[i].ave_win_contrib_a.resize(deck_size);
		for (int j = 0; j < deck_size; j++)
			fs >> recs[i].ave_win_contrib_a[j];
		recs[i].sum_total_partic_a.resize(deck_size);
		for (int j = 0; j < deck_size; j++)
			fs >> recs[i].sum_total_partic_a[j];
		recs[i].ave_total_partic_a.resize(deck_size);
		for (int j = 0; j < deck_size; j++)
			fs >> recs[i].ave_total_partic_a[j];
		recs[i].sum_win_contrib_b.resize(deck_size);
		for (int j = 0; j < deck_size; j++)
			fs >> recs[i].sum_win_contrib_b[j];
		recs[i].ave_win_contrib_b.resize(deck_size);
		for (int j = 0; j < deck_size; j++)
			fs >> recs[i].ave_win_contrib_b[j];
		recs[i].sum_total_partic_b.resize(deck_size);
		for (int j = 0; j < deck_size; j++)
			fs >> recs[i].sum_total_partic_b[j];
		recs[i].ave_total_partic_b.resize(deck_size);
		for (int j = 0; j < deck_size; j++)
			fs >> recs[i].ave_total_partic_b[j];
	}
	fs.close();
	fs.clear();
}

template <typename T>
void SplitTrainValidate(const vector<T>& data, vector<T>& train_data, vector<T>& validate_data, double train_ratio)
{
	int total_size = data.size();
	int train_size = (int)(train_ratio * total_size);

	train_data.resize(train_size);
	int i;
	for (i = 0; i < train_size; i++)
		train_data[i] = data[i];
	validate_data.resize(total_size - train_size);
	for (int j = 0; i < total_size; j++, i++)
		validate_data[j] = data[i];
}



int main(int argc, char* argv[]) // argument order, if supplied: mode, seed, file paths (with different semantics among modes)
{
	int seed = 0;
	char ch = 'y';

	unsigned n = 20; // cards in a deck

	cout << "Select Mode: " << endl;
	cout << "0 (Default): Generate random cards freely." << endl;
	cout << "1 : Generate random cards with specified seeds." << endl;
	cout << "2 : Play against AI using decks generated freely or with specified seeds." << endl;
	cout << "3 : Play against yourself with decks generated with specified seeds." << endl;
	cout << "4 : Simulate AI Vs AI matches between (different) AI's on the same set of random decks (Warning: this may take hours to days)." << endl;
	cout << "5 : Simulate AI Vs AI matches (with same AI) for evaluating cards (decks) on the same random card pool with both random and evolutionary deck building/matching scheme (Warning: this may take hours to days)." << endl;
	cout << "6 : Simulate AI Vs AI matches (with same AI) between different two small sets of given decks (Warning: this may take hours to days)." << endl;
	cout << "7 : Play against AI using decks in either the meta environment (generated by the evolutionary simulation) or the random environment (generated by the random simulation)." << endl;

	int mode;
	if (argc > 1)
	{
		mode = atoi(argv[1]);
		cout << "Mode selected: " << mode << endl;
	}
	else
	{
		cin >> mode;
	}

	switch (mode)
	{
	case 7:
		{
			string Card_Path = "match_card_data_evolved.txt";
			string Deck_Paths[2] = {"match_deck_data_evolved.txt", "match_deck_data_top_random.txt"};

			// read card data
			vector<Card*> card_list;
			vector<CardRep> card_reps;
			vector<BalanceStat> card_stats; // it is questionable whether we should clear this before simulation, but these are not used here so it should be fine for now
			ReadCardData(card_list, card_reps, card_stats, Card_Path.c_str());

			// read deck data
			vector<vector<int>> deck_lists[2];
			vector<BalanceStat> deck_stat_lists[2]; // not actually used here, the original stats from when the deck where initially created and tested
			for (int i = 0; i < 2; i++)
				ReadDeckData(deck_lists[i], deck_stat_lists[i], Deck_Paths[i].c_str());
			int deck_size = deck_lists[0][0].size();

			while (true)
			{
				int selection;
				int your_deck_set = 0;
				int your_deck_rank = 1;
				int ai_deck_set = 1;
				int ai_deck_rank = 10;
				vector<int> your_deck_indices;
				vector<int> ai_deck_indices;
				cout << "The default setting is you choose one of the meta decks (stronger) and ai chooses one of the top random decks (weaker). Change default? (y/n)" << endl;
				cin >> ch;
				cout << endl;
				if (ch == 'Y' || ch == 'y')
				{
					cout << "Select your deck set: 0 - meta decks, 1 - top random decks" << endl;
					while (!(cin >> selection)) { cin.clear(); cin.ignore(); }
					if (selection < 0)
						selection = 0;
					else if (selection > 1)
						selection = 1;
					your_deck_set = selection;
					if (selection == 0)
						cout << "You selected meta decks." << endl << endl;
					else
						cout << "You selected top random decks" << endl << endl;

					cout << "Select ai deck set: 0 - meta decks, 1 - top random decks" << endl;
					while (!(cin >> selection)) { cin.clear(); cin.ignore(); }
					if (selection < 0)
						selection = 0;
					else if (selection > 1)
						selection = 1;
					ai_deck_set = selection;
					if (selection == 0)
						cout << "AI selected meta decks." << endl << endl;
					else
						cout << "AI selected top random decks" << endl << endl;
				}
				cout << "Choose, within the set, the rank of the deck for you to play (1 ~ 10)" << endl;
				while (!(cin >> selection)) { cin.clear(); cin.ignore(); }
				if (selection <= 0)
					selection = 1;
				else if (selection > 10)
					selection = 10;
				if (your_deck_set == 0)
					cout << "You selected #" << selection << " meta deck." << endl << endl;
				else
					cout << "You selected #" << selection << " random deck." << endl << endl;
				your_deck_indices = deck_lists[your_deck_set][selection - 1];
				BrowseCardSet(card_list, your_deck_indices);
				cout << endl;
				cout << "Choose, within the set, the rank of the deck for ai to play (1 ~ 10)" << endl;
				while (!(cin >> selection)) { cin.clear(); cin.ignore(); }
				if (selection <= 0)
					selection = 1;
				else if (selection > 10)
					selection = 10;
				if (ai_deck_set == 0)
					cout << "AI selected #" << selection << " meta deck." << endl << endl;
				else
					cout << "AI selected #" << selection << " random deck." << endl << endl;
				ai_deck_indices = deck_lists[ai_deck_set][selection - 1];

				cout << "Input AI level (integer, clamped to 0 ~ 5; 2 or above is recommended)" << endl;
				unsigned ai_level;
				while (!(cin >> ai_level)) { cin.clear(); cin.ignore(); }
				cout << endl;

				GiglRandInit(time(NULL));
				vector<int> your_deck_shuffle(deck_size);
				vector<int> ai_deck_shuffle(deck_size);
				vector<int> your_shuffled_indices(deck_size);
				vector<int> ai_shuffled_indices(deck_size);
				vector<Card*> your_deck(deck_size);
				vector<Card*> ai_deck(deck_size);
				InitMatch(card_list, your_deck_indices, ai_deck_indices, your_deck_shuffle, ai_deck_shuffle, your_shuffled_indices, ai_shuffled_indices, your_deck, ai_deck);

				queue<DeferredEvent*> event_queue;
				Player human_player("Player", 30, your_deck, false, event_queue);
				Player ai_player("AI", 30, ai_deck, true, event_queue, ai_level);

				InteractiveMatch(human_player, ai_player);

				while (!event_queue.empty())
				{
					delete event_queue.front();  // note: this is not deleting the actual card but the entity for flagging
					event_queue.pop();
				}

				cout << "Repeat? (y/n)" << endl;
				cin >> ch;
				if (ch != 'Y' && ch != 'y')
					break;
			}
			
		}
		break;
	case 6:
		{
			int num_pair_matches = 64; // number of match pairs between each pair of decks

			if (argc > 2)
			{
				seed = atoi(argv[2]);
			}
			else
			{
				cout << "Input seed" << endl;
				cin >> seed;
			}
			cout << "Seed for simulation: " << seed << endl;

			string Card_Path = "match_card_data_evolved.txt";
			if (argc > 3)
				Card_Path = argv[3];

			string Deck_A_Path = "match_deck_data_evolved.txt";
			if (argc > 4)
				Deck_A_Path = argv[4];

			string Deck_B_Path = "match_deck_data_top_random.txt";
			if (argc > 5)
				Deck_B_Path = argv[5];

			string Deck_Post_Simulation_Path = "deck_match_post_simulation.txt";
			if (argc > 6)
				Deck_Post_Simulation_Path = argv[6];
			
			// read card data
			vector<Card*> card_list;
			vector<CardRep> card_reps;
			vector<BalanceStat> card_stats; // it is questionable whether we should clear this before simulation, but these are not used here so it should be fine for now
			ReadCardData(card_list, card_reps, card_stats, Card_Path.c_str());

			// read deck data
			vector<vector<int>> deck_list_a, deck_list_b;
			vector<BalanceStat> deck_stats_a, deck_stats_b; // not actually used here, the original stats from when the deck where initially created and tested
			ReadDeckData(deck_list_a, deck_stats_a, Deck_A_Path.c_str());
			ReadDeckData(deck_list_b, deck_stats_b, Deck_B_Path.c_str());
			int deck_num_a = deck_list_a.size();
			int deck_num_b = deck_list_b.size();
			int deck_size = deck_list_a[0].size();

			unsigned ai_level = 2;

			time_t timer_0 = time(NULL);

			int turn_count = 0;

			ofstream fs(Deck_Post_Simulation_Path.c_str());
			fs << deck_num_a << " " << deck_num_b << endl;
			for (int i = 0; i < deck_num_a; i++)
			{
				for (int j = 0; j < deck_num_b; j++)
				{
					BalanceStat tmp_stat_a, tmp_stat_b; // only tmp_stat_a is recorded to output (well they should be symmetric)
					cout << "Deck " << i << " from the first list VS Deck " << j << " from the second list." << endl;
					for (int k = 0; k < num_pair_matches; k++)
					{
						cout << "Match Pair " << k << ":" << endl;
						MatchRec rec(deck_list_a[i], deck_list_b[j]); // not actually used
						turn_count += SimulatePairMatchBetweenDecks(ai_level, card_list, deck_list_a[i], deck_list_b[j], card_stats, tmp_stat_a, tmp_stat_b, rec, deck_size);
						rec.UpdateStats();
					}
					tmp_stat_a.UpdateEval();
					tmp_stat_b.UpdateEval(); // not actually necessary
					fs << tmp_stat_a.eval << " ";
				}
				fs << endl;
			}
			fs.close();
			fs.clear();	
			
			time_t timer_1 = time(NULL);
			
			/* performance report */
			cout << endl;
			cout << "Total number of decks tested: " << deck_num_a + deck_num_b << endl;
			cout << "Total number of match pairs tested: " << deck_num_a * deck_num_b * num_pair_matches << endl; 
			cout << "Total number of match turns simulated: " << turn_count << endl; 
			double total_time = difftime(timer_1, timer_0);
			cout << "Total testing time (including writing results to the file): " << total_time << endl;

			DeleteCards(card_list);
		}
		break;
	case 5:
		{
			int p = 100; // size of the card pool

			if (argc > 2)
			{
				seed = atoi(argv[2]);
			}
			else
			{
				cout << "Input Seed" << endl;
				cin >> seed;
			}
			cout << "Seed for card pool and simulation: " << seed << endl;

			string Match_Card_Data_Path = "match_card_data.txt";
			if (argc > 3)
				Match_Card_Data_Path = argv[3];

			string Match_Deck_Data_Path = "match_deck_data.txt";
			if (argc > 4)
				Match_Deck_Data_Path = argv[4];

			string Match_Match_Data_Path = "match_match_data.txt";
			if (argc > 5)
				Match_Deck_Data_Path = argv[5];

			// the card pool is shared between the two simulation methods
			vector<Card*> card_list = GenerateCardSet(p, seed);

			// random environment benchmark related parameters
			int deck_num_random = 1000; // total number of decks
			int match_pair_num_random = 10000; // total number of pair matches
			int turn_count_random = 0; // number of match turns, not known before hand
			double total_time_random;

			// meta environment benchmark related parameters
			int deck_pool_size = 10; // number of active decks (will evolve during the simulation)
			int num_pair_matches_init = 2; // initial number of pair matches between each two decks
			int num_pair_matches_final = 8; // final number of pair matches between each two decks
			int num_evolve_iter = 100;
			int deck_count_evolved = deck_pool_size; // it starts with the initial deck pool size, but the total number is not known before hand
			int match_pair_count_evolved = 0; // this is not known before hand
			int turn_count_evolved = 0; // number of match turns, not known before hand
			double init_time;
			double evolve_time;
			double final_time;
			
			int display_num = 5; // the number of top and bottom cards to be displayed
			vector<int> top_card_indices_random, bottom_card_indices_random;
			vector<int> top_card_indices_evolved, bottom_card_indices_evolved;

			/* random environment simulation */
			{
				string Match_Card_Data_Path_Random, Match_Card_Data_Path_Human_Random;
				size_t post_fix_pos = Match_Card_Data_Path.rfind(".");
				if (post_fix_pos == string::npos)
				{
					Match_Card_Data_Path_Random = Match_Card_Data_Path + "_random";
					Match_Card_Data_Path_Human_Random = Match_Card_Data_Path + "_human_random";
				}
				else
				{
					Match_Card_Data_Path_Random = Match_Card_Data_Path;
					Match_Card_Data_Path_Random.insert(post_fix_pos, "_random");
					Match_Card_Data_Path_Human_Random = Match_Card_Data_Path;
					Match_Card_Data_Path_Human_Random.insert(post_fix_pos, "_human_random");
				}

				string Match_Deck_Data_Path_All_Random, Match_Deck_Data_Path_Top_Random, Match_Deck_Data_Path_Skip_Random;
				post_fix_pos = Match_Deck_Data_Path.rfind(".");
				if (post_fix_pos == string::npos)
				{
					Match_Deck_Data_Path_All_Random = Match_Deck_Data_Path + "_all_random";
					Match_Deck_Data_Path_Top_Random = Match_Deck_Data_Path + "_top_random";
					Match_Deck_Data_Path_Skip_Random = Match_Deck_Data_Path + "_skip_random";
				}
				else
				{
					Match_Deck_Data_Path_All_Random = Match_Deck_Data_Path;
					Match_Deck_Data_Path_All_Random.insert(post_fix_pos, "_all_random");
					Match_Deck_Data_Path_Top_Random = Match_Deck_Data_Path;
					Match_Deck_Data_Path_Top_Random.insert(post_fix_pos, "_top_random");
					Match_Deck_Data_Path_Skip_Random = Match_Deck_Data_Path;
					Match_Deck_Data_Path_Skip_Random.insert(post_fix_pos, "_skip_random");
				}

				string Match_Match_Data_Path_Random;
				post_fix_pos = Match_Match_Data_Path.rfind(".");
				if (post_fix_pos == string::npos)
				{
					Match_Match_Data_Path_Random = Match_Match_Data_Path + "_random";
				}
				else
				{
					Match_Match_Data_Path_Random = Match_Match_Data_Path;
					Match_Match_Data_Path_Random.insert(post_fix_pos, "_random");
				}

				unsigned ai_level = 2;

				vector<BalanceStat> card_stats(p);
				vector<BalanceStat> deck_stats(deck_num_random);

				time_t timer_0 = time(NULL);

				vector<vector<int>> deck_list; // storing card indices in the seed list (not the seeds themselves)
				for (int i = 0; i < deck_num_random; i++)
					deck_list.push_back(CreateRandomSelection(p, n)); // problematic as some cards may not get selected

				vector<MatchRec> rec_list;
				for (int i = 0; i < match_pair_num_random; i++)
				{
					int index_a = GetGiglRandInt(deck_num_random);
					int index_b = GetGiglRandInt(deck_num_random);
					cout << "Match Pair " << i << ": Deck " << index_a << " VS Deck " << index_b << ". " << endl;
					MatchRec rec(deck_list[index_a], deck_list[index_b]);
					turn_count_random += SimulatePairMatchBetweenDecks(ai_level, card_list, deck_list[index_a], deck_list[index_b], card_stats, deck_stats[index_a], deck_stats[index_b], rec, n);
					rec.UpdateStats();
					rec_list.push_back(rec);
				}
				UpdateStatEvals(card_stats);
				UpdateStatEvals(deck_stats);

				time_t timer_1 = time(NULL);

				total_time_random = difftime(timer_1, timer_0);

				// prepare for displaying
				vector<int> card_indices(p);
				SortStatInIndices(card_stats, card_indices);
				for (int i = 0; i < display_num; i++)
				{
					top_card_indices_random.push_back(card_indices[i]);
					bottom_card_indices_random.push_back(card_indices[p - i - 1]);
				}

				// prepare data for training/post processing
				// cards
				vector<CardRep> card_reps;
				vector<double> card_strengths;
				vector<double> card_weights;
				PrepareCardData(card_list, card_stats, card_reps, card_strengths, card_weights, Match_Card_Data_Path_Random.c_str(), Match_Card_Data_Path_Human_Random.c_str());
				// decks
				vector<int> deck_indices(deck_num_random);
				SortStatInIndices(deck_stats, deck_indices);
				WriteDataDeckSorted(deck_list, deck_stats, deck_indices, 0, 1, 1000, Match_Deck_Data_Path_All_Random.c_str());
				WriteDataDeckSorted(deck_list, deck_stats, deck_indices, 0, 1, 10, Match_Deck_Data_Path_Top_Random.c_str());
				WriteDataDeckSorted(deck_list, deck_stats, deck_indices, 49, 100, 10, Match_Deck_Data_Path_Skip_Random.c_str());
				// matches
				WriteMatchRecords(rec_list, n, Match_Match_Data_Path_Random.c_str());
			}
			/* evolved environment simulation */
			{
				string Match_Card_Data_Path_Evolved, Match_Card_Data_Path_Human_Evolved;
				size_t post_fix_pos = Match_Card_Data_Path.rfind(".");
				if (post_fix_pos == string::npos)
				{
					Match_Card_Data_Path_Evolved = Match_Card_Data_Path + "_evolved";
					Match_Card_Data_Path_Human_Evolved = Match_Card_Data_Path + "_human_evolved";
				}
				else
				{
					Match_Card_Data_Path_Evolved = Match_Card_Data_Path;
					Match_Card_Data_Path_Evolved.insert(post_fix_pos, "_evolved");
					Match_Card_Data_Path_Human_Evolved = Match_Card_Data_Path;
					Match_Card_Data_Path_Human_Evolved.insert(post_fix_pos, "_human_evolved");
				}

				string Match_Deck_Data_Path_Evolved;
				post_fix_pos = Match_Deck_Data_Path.rfind(".");
				if (post_fix_pos == string::npos)
				{
					Match_Deck_Data_Path_Evolved = Match_Deck_Data_Path + "_evolved";
				}
				else
				{
					Match_Deck_Data_Path_Evolved = Match_Deck_Data_Path;
					Match_Deck_Data_Path_Evolved.insert(post_fix_pos, "_evolved");
				}

				string Match_Match_Data_Path_Evolved;
				post_fix_pos = Match_Match_Data_Path.rfind(".");
				if (post_fix_pos == string::npos)
				{
					Match_Match_Data_Path_Evolved = Match_Match_Data_Path + "_evolved";
				}
				else
				{
					Match_Match_Data_Path_Evolved = Match_Match_Data_Path;
					Match_Match_Data_Path_Evolved.insert(post_fix_pos, "_evolved");
				}

				unsigned ai_level = 2;

				vector<BalanceStat> card_stats(p);
				vector<BalanceStat> deck_stats(deck_pool_size);

				time_t timer_0 = time(NULL);

				vector<vector<int>> deck_list; // storing card indices in the seed list (not the seeds themselves)
				
				vector<MatchRec> rec_list;

				// initialization
				cout << "Creating and testing initial deck pool." << endl;
				for (int i = 0; i < deck_pool_size; i++)
				{
					vector<int> tmp_deck = CreateRandomSelectionSorted(p, n);
					bool is_duplicate;
					do // make sure we don't create duplicate decks
					{
						is_duplicate = false;
						for (int j = 0; j < i; j++)
							if (tmp_deck == deck_list[j])
							{
								is_duplicate = true;
								break;
							}
					} while (is_duplicate);
					deck_list.push_back(tmp_deck);
				}
				for (int i = 0; i < deck_pool_size - 1; i++)
					for (int j = i + 1; j < deck_pool_size; j++)
					{
						MatchRec rec(deck_list[i], deck_list[j]);
						for (int k = 0; k < num_pair_matches_init; k++)
						{
							cout << "Match Pair #" << ++match_pair_count_evolved << endl;
							turn_count_evolved += SimulatePairMatchBetweenDecks(ai_level, card_list, deck_list[i], deck_list[j], card_stats, deck_stats[i], deck_stats[j], rec, n);
						}
						rec.UpdateStats();
						rec_list.push_back(rec);
					}
				cout << endl;
				UpdateStatEvals(card_stats);
				UpdateStatEvals(deck_stats);

				time_t timer_1 = time(NULL);

				// evolving the deck pool and continue testing
				double t_init = 1.0; // intial "temperature"
				double ln_t_init = log(t_init);
				double t_final = 0.001;
				double ln_t_final = log(t_final);
				double action_probs_init[] = { 0.7, 0.1, 0.2 }; // new deck; cross-over; mutation
				double action_probs_final[] = { 0.2, 0.5, 0.3 }; // new deck; cross-over; mutation 	
				for (int iter = 1; iter <= num_evolve_iter; iter++)
				{
					cout << "Evolution iteration #" << iter << ": ";
					double progress = iter / (double)num_evolve_iter;
					double ln_t = ln_t_init + progress * (ln_t_final - ln_t_init); // temperature interpolated in log scale
					double t = exp(ln_t);
					int num_pair_matches = (int)(num_pair_matches_init + progress * (double)(num_pair_matches_final - num_pair_matches_init));
					double action_probs[2]; // the third one is not used
					for (int i = 0; i < 2; i++)
						action_probs[i] = action_probs_init[i] + progress * (action_probs_final[i] - action_probs_init[i]);
					action_probs[1] += action_probs[0]; // cumulate
					double roll = GetGiglRandFloat();
					if (roll < action_probs[0]) // new deck
					{
						cout << "testing a new deck" << endl;
						turn_count_evolved += TestNewDeck(ai_level, card_list, deck_list, card_stats, deck_stats, rec_list, n, t, num_pair_matches, deck_count_evolved, match_pair_count_evolved);
					}
					else if (roll < action_probs[1]) // cross-over
					{
						cout << "testing the cross-overs of two existing decks" << endl;
						turn_count_evolved += TestCrossOver(ai_level, card_list, deck_list, card_stats, deck_stats, rec_list, n, t, num_pair_matches, deck_count_evolved, match_pair_count_evolved);
					}
					else // mutation
					{
						cout << "testing the mutation of an existing deck" << endl;
						turn_count_evolved += TestMutation(ai_level, card_list, deck_list, card_stats, deck_stats, rec_list, n, t, num_pair_matches, deck_count_evolved, match_pair_count_evolved);
					}
					UpdateStatEvals(card_stats); // deck stats should already be updated inside those "Test" functions
					cout << endl;
				}

				time_t timer_2 = time(NULL);

				// stop evolving and do the final round of tests
				cout << "Testing the final deck pool." << endl;
				for (int i = 0; i < deck_pool_size - 1; i++)
					for (int j = i + 1; j < deck_pool_size; j++)
					{
						MatchRec rec(deck_list[i], deck_list[j]);
						for (int k = 0; k < num_pair_matches_final; k++)
						{
							cout << "Match Pair #" << ++match_pair_count_evolved << endl;
							turn_count_evolved += SimulatePairMatchBetweenDecks(ai_level, card_list, deck_list[i], deck_list[j], card_stats, deck_stats[i], deck_stats[j], rec, n);
						}
						rec.UpdateStats();
						rec_list.push_back(rec);
					}
				cout << endl;
				UpdateStatEvals(card_stats);
				UpdateStatEvals(deck_stats);

				time_t timer_3 = time(NULL);

				init_time = difftime(timer_1, timer_0);
				evolve_time = difftime(timer_2, timer_1);
				final_time = difftime(timer_3, timer_2);

				// prepare for displaying
				vector<int> card_indices(p);
				SortStatInIndices(card_stats, card_indices);
				for (int i = 0; i < display_num; i++)
				{
					top_card_indices_evolved.push_back(card_indices[i]);
					bottom_card_indices_evolved.push_back(card_indices[p - i - 1]);
				}

				// prepare data for training/post processing
				// cards
				vector<CardRep> card_reps;
				vector<double> card_strengths;
				vector<double> card_weights;
				PrepareCardData(card_list, card_stats, card_reps, card_strengths, card_weights, Match_Card_Data_Path_Evolved.c_str(), Match_Card_Data_Path_Human_Evolved.c_str());
				// decks
				vector<int> deck_indices(deck_pool_size);
				SortStatInIndices(deck_stats, deck_indices);
				WriteDataDeckSorted(deck_list, deck_stats, deck_indices, 0, 1, 10, Match_Deck_Data_Path_Evolved.c_str());
				// matches
				WriteMatchRecords(rec_list, n, Match_Match_Data_Path_Evolved.c_str());
			}

			/* performance report */
			cout << endl;
			cout << "Random game environment simulation summary" << endl;
			cout << "------------------------------------------" << endl;
			cout << "Total number of decks tested: " << deck_num_random << endl;
			cout << "Total number of match pairs tested: " << match_pair_num_random << endl;
			cout << "Total number of match turns simulated: " << turn_count_random << endl;
			cout << "Total testing time: " << total_time_random << endl;
			cout << "------------------------------------------" << endl << endl;
			cout << "Meta game environment simulation summary" << endl;
			cout << "------------------------------------------" << endl;
			cout << "Total number of decks tested: " << deck_count_evolved << endl;
			cout << "Total number of match pairs tested: " << match_pair_count_evolved << endl;
			cout << "Total number of match turns simulated: " << turn_count_evolved << endl;
			cout << "Initial deck pool testing time: " << init_time << endl;
			cout << "Evolving time: " << evolve_time << endl;
			cout << "Final deck pool testing time: " << final_time << endl;
			cout << "Total testing time: " << init_time + evolve_time + final_time << endl;
			cout << "------------------------------------------" << endl << endl;

			/* example cards */
			cout << endl;
			cout << "Top cards rated by random game environment" << endl;
			cout << "------------------------------------------" << endl;
			BrowseCardSet(card_list, top_card_indices_random);
			cout << endl;
			cout << "Top cards rated by meta game environment" << endl;
			cout << "------------------------------------------" << endl;
			BrowseCardSet(card_list, top_card_indices_evolved);
			cout << endl;
			cout << "Bottom cards rated by random game environment" << endl;
			cout << "------------------------------------------" << endl;
			BrowseCardSet(card_list, bottom_card_indices_random);
			cout << endl;
			cout << "Bottom cards rated by meta game environment" << endl;
			cout << "------------------------------------------" << endl;
			BrowseCardSet(card_list, bottom_card_indices_evolved);

			DeleteCards(card_list);
		}
		break;
	case 4:
		{
			int p = 100;
			int deck_num = 100;

			if (argc > 2)
			{
				seed = atoi(argv[2]);
			}
			else
			{
				cout << "Input seed" << endl;
				cin >> seed;
			}
			cout << "Seed for simulation: " << seed << endl;

			vector<Card*> card_list = GenerateCardSet(p, seed);

			vector<vector<int>> deck_list; // storing card indices in the seed list (not the seeds themselves)
			for (int i = 0; i < deck_num; i++)
				deck_list.push_back(CreateRandomSelection(p, n));

			// pair-wise matches (duplicate the same pair twice for a different player go first)
			unsigned ai_level_a, ai_level_b;
			cout << "Input AI level for the first AI (integer, clamped to 0 ~ 5)" << endl;
			cin >> ai_level_a;
			cout << "Input AI level for the second AI (integer, clamped to 0 ~ 5)" << endl;
			cin >> ai_level_b;
			TestAIs(ai_level_a, ai_level_b, card_list, deck_list, deck_num, n);

			DeleteCards(card_list);
		}
		break;
	case 3:
		while (true)
		{
			cout << endl << "Input seed (negative to quit) for Deck 1 (playing first)" << endl;
			while (!(cin >> seed)) { cin.clear(); cin.ignore(); } // the clear and ignore prevents stuck when the input is ill-formatted
			if (seed < 0)
				break;
			vector<Card*> deck1 = GenerateCardSet(n, seed);
			BrowseCardSet(deck1);

			cout << "Input seed (negative to quit) for Deck 2 (playing second)" << endl;
			while (!(cin >> seed)) { cin.clear(); cin.ignore(); } // the clear and ignore prevents stuck when the input is ill-formatted
			if (seed < 0)
			{
				DeleteCards(deck1);
				break;
			}
			vector<Card*> deck2 = GenerateCardSet(n, seed);
			BrowseCardSet(deck2);

			cout << "Shuffle decks for the match? (y/n; default y; cards are drawn in reverse order from the deck)" << endl;
			cin >> ch;
			if (ch != 'N' && ch != 'n')
			{
				srand(time(NULL));
				random_shuffle(deck1.begin(), deck1.end());
				random_shuffle(deck2.begin(), deck2.end());
			}

			queue<DeferredEvent*> event_queue;
			Player player1("Player1", 30, deck1, false, event_queue);
			Player player2("Player2", 30, deck2, false, event_queue);

			InteractiveMatch(player1, player2, false);

			while (!event_queue.empty())
			{
				delete event_queue.front();  // note: this is not deleting the actual card but the entity for flagging
				event_queue.pop();
			}

			cout << "Repeat? (y/n)" << endl;
			cin >> ch;
			if (ch != 'Y' && ch != 'y')
				break;
		}
		break;
	case 2:
		while (true)
		{
			cout << "Input seed for your deck (negative to randomize) " << endl;
			while (!(cin >> seed)) { cin.clear(); cin.ignore(); } // the clear and ignore prevents stuck when the input is ill-formatted
			if (seed < 0)
				seed = time(NULL);
			cout << "Deck seed: " << seed << endl; // echo
			vector<Card*> deck1 = GenerateCardSet(n, seed);
			BrowseCardSet(deck1);

			cout << "Input seed for AI's deck (negative to randomize) " << endl;
			while (!(cin >> seed)) { cin.clear(); cin.ignore(); } // the clear and ignore prevents stuck when the input is ill-formatted
			if (seed < 0)
			{
				seed = time(NULL) + GetGiglRandInt();
				// the addition may overflow to negative value, but we want seed to be positive
				if (seed < 0)
					seed = abs(seed + 1); // +1 is to avoid the problem of INT_MIN not having a proper absolute value
			}
			cout << "Deck seed: " << seed << endl; // echo
			vector<Card*> deck2 = GenerateCardSet(n, seed);

			cout << "Shuffle decks for the match? (y/n; default y; cards are drawn in reverse order from the deck)" << endl;
			cin >> ch;
			if (ch != 'N' && ch != 'n')
			{
				srand(time(NULL));
				random_shuffle(deck1.begin(), deck1.end());
				random_shuffle(deck2.begin(), deck2.end());
			}

			cout << "Input AI level (integer, clamped to 0 ~ 5; 2 or above is recommended)" << endl;
			unsigned ai_level;
			while (!(cin >> ai_level)) { cin.clear(); cin.ignore(); } // the clear and ignore prevents stuck when the input is ill-formatted

			queue<DeferredEvent*> event_queue;
			Player human_player("Player", 30, deck1, false, event_queue);
			Player ai_player("AI", 30, deck2, true, event_queue, ai_level);

			InteractiveMatch(human_player, ai_player);

			while (!event_queue.empty())
			{
				delete event_queue.front();  // note: this is not deleting the actual card but the entity for flagging
				event_queue.pop();
			}

			cout << "Repeat? (y/n)" << endl;
			cin >> ch;
			if (ch != 'Y' && ch != 'y')
				break;
		}
		break;
	case 1:
		while (seed >= 0)
		{
			cout << "Input Seed (negative to quit)" << endl;
			while (!(cin >> seed)) { cin.clear(); cin.ignore(); } // the clear and ignore prevents stuck when the input is ill-formatted
			if (seed < 0)
				break;

			Card* card = GenerateSingleCard(seed);
			cout << endl << GetCardDetail(card) << endl;
			DeleteCard(card);
		}
		break;
	default:
		seed = time(NULL);
		while (ch == 'y' || ch == 'Y')
		{
			Card* card = GenerateSingleCard(seed);
			cout << endl << GetCardDetail(card) << endl;
			DeleteCard(card);

			cout << "Continue? (y/n)" << endl;
			cin >> ch;
			seed = GetGiglRandInt();
		}
	}

	return 0;
}
