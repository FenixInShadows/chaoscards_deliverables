#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>

#include "Player.h"

// #define SUPPRESS_TOURNAMENT_PROMPTS

using namespace std;


struct TournamentState // note, when a card_list is managed through TournamentState, when a card_list is managed with a TournamentState, do not delete cards elsewhere, only delete through ResetState or the destructor here.
{
	TournamentState() : card_list(), card_reps(), card_stats(), all_decks(), active_decks(), all_deck_stats(), active_deck_stats(), card_stats_in_all_decks(), card_stats_in_active_decks(), snd_player_stat(), rec_list(), card_pair_diffs(nullptr), neibor_dists(), ave_neibor_dist(0.0), match_pair_count(0), turn_count(0)
	{
	}
	TournamentState(vector<Card*>& cards) : card_list(cards), card_stats(cards.size()), all_decks(), active_decks(), all_deck_stats(), active_deck_stats(), card_stats_in_all_decks(), card_stats_in_active_decks(), snd_player_stat(), rec_list(), match_pair_count(0), turn_count(0)
	{
		GetCardsReps(card_list, card_reps);
		AllocatePairDiffs();
		ComputePairDiffs();
			
	}
	TournamentState(const TournamentState& state) : card_list(HardCopyCards(state.card_list)), card_reps(state.card_reps), card_stats(state.card_stats), all_decks(state.all_decks), active_decks(state.active_decks), all_deck_stats(state.all_deck_stats), active_deck_stats(state.active_deck_stats), card_stats_in_all_decks(state.card_stats_in_all_decks), card_stats_in_active_decks(state.card_stats_in_active_decks), snd_player_stat(state.snd_player_stat), rec_list(state.rec_list), neibor_dists(state.neibor_dists), ave_neibor_dist(state.ave_neibor_dist), match_pair_count(state.match_pair_count), turn_count(state.turn_count)
	{
		AllocatePairDiffs();
		int n_cards = card_list.size();
		if (n_cards > 0)
		{
			for (int i = 0; i < n_cards; i++)
				for (int j = 0; j < n_cards; j++)
					card_pair_diffs[i][j] = state.card_pair_diffs[i][j];
		}
	}
	~TournamentState()
	{
		DeallocatePairDiffs();
		for (Card* card_ptr : card_list)
			DeleteCard(card_ptr);
	}
	TournamentState& operator = (const TournamentState& state)
	{
		DeallocatePairDiffs();
		for (Card* card_ptr : card_list)
			DeleteCard(card_ptr);
		card_list = HardCopyCards(state.card_list);
		card_reps = state.card_reps;
		AllocatePairDiffs();
		int n_cards = card_list.size();
		if (n_cards > 0)
		{
			for (int i = 0; i < n_cards; i++)
				for (int j = 0; j < n_cards; j++)
					card_pair_diffs[i][j] = state.card_pair_diffs[i][j];
		}
		neibor_dists = state.neibor_dists;
		ave_neibor_dist = state.ave_neibor_dist;
		card_stats = state.card_stats;
		all_decks = state.all_decks;
		active_decks = state.active_decks;
		all_deck_stats = state.all_deck_stats;
		active_deck_stats = state.active_deck_stats;
		card_stats_in_all_decks = state.card_stats_in_all_decks;
		card_stats_in_active_decks = state.card_stats_in_active_decks;
		snd_player_stat = state.snd_player_stat;
		rec_list = state.rec_list;
		match_pair_count = state.match_pair_count;
		turn_count = state.turn_count;

		return *this;
	}
	void ResetState() // this version does not update the card pool (using the existing one)
	{
		for (BalanceStat& stat : card_stats)
			stat.ResetStat();

		all_decks.clear();
		active_decks.clear();
		all_deck_stats.clear();
		active_deck_stats.clear();
		card_stats_in_all_decks.clear();
		card_stats_in_active_decks.clear();
		snd_player_stat.ResetStat();

		rec_list.clear();
		match_pair_count = 0;
		turn_count = 0;

	}
	void ResetState(vector<Card*>& cards)
	{
		DeallocatePairDiffs();
		for (Card* card_ptr : card_list)
			DeleteCard(card_ptr);
		card_list = cards;
		GetCardsReps(card_list, card_reps);
		AllocatePairDiffs();
		ComputePairDiffs();
		card_stats.resize(card_list.size());

		ResetState();
	}
	void AllocatePairDiffs() // call after the corresponding card list is assigned
	{
		int n_cards = card_list.size();
		if (n_cards > 0)
		{
			card_pair_diffs = new double* [n_cards];
			for (int i = 0; i < n_cards; i++)
				card_pair_diffs[i] = new double[n_cards];
		}
	}
	void DeallocatePairDiffs() // call before the corresponding card list is discarded
	{
		int n_cards = card_list.size();
		if (n_cards > 0)
		{
			for (int i = 0; i < card_list.size(); i++)
				delete [] card_pair_diffs[i];
			delete [] card_pair_diffs;
		}
	}
	void ComputePairDiffs() // call after the card list and reps are assigned and diffs is allocated; also updates ave_neibor_dist
	{
		int n_cards = card_list.size();
		// compute the pair distance table
		for (int i = 0; i < n_cards; i++)
		{
			card_pair_diffs[i][i] = 0.0; // diagonal is known, no need to compute
			for (int j = i + 1; j < n_cards; j++) // only compute the upper triangle as it is symmetric
				card_pair_diffs[j][i] = card_pair_diffs[i][j] = DiffCardRep(card_reps[i], card_reps[j]);
		}
		// compute the average of nearest neighbor distance for all cards
		neibor_dists.resize(n_cards);
		double sum_neibor_dist = 0.0;
		for (int i = 0; i < n_cards; i++)
		{
			double nearest = 1.0;
			for (int j = 0; j < n_cards; j++)
				if (i != j && card_pair_diffs[i][j] < nearest)
					nearest = card_pair_diffs[i][j];
			neibor_dists[i] = nearest;
			sum_neibor_dist += nearest;
		}
		ave_neibor_dist = sum_neibor_dist / (double)n_cards;
	}

	vector<Card*> card_list;
	vector<CardRep> card_reps;
	vector<BalanceStat> card_stats;
	vector<vector<int>> all_decks;
	vector<vector<int>> active_decks;
	vector<BalanceStat> all_deck_stats;
	vector<BalanceStat> active_deck_stats;
	vector<vector<BalanceStat>> card_stats_in_all_decks;
	vector<vector<BalanceStat>> card_stats_in_active_decks;
	BalanceStat snd_player_stat;
	vector<MatchRec> rec_list;
	double** card_pair_diffs; // will allocate a 2D table for storing the pairwise card diffs
	vector<double> neibor_dists;
	double ave_neibor_dist;
	int match_pair_count;
	int turn_count; // both sides of each matches are summed up
};

struct SimulationNode // an entity for maintaining stats for simulating the same card set
{
	SimulationNode(vector<Card*> _cards) :
		tournmnt_state(_cards),
		card_stats(_cards.size()),
		snd_player_stat(),
		sum_coverage(0.0),
		card_sum_coverages(_cards.size(), 0),
		sum_active_deck_diff(0.0),
		weighted_sum_interative_diff_sqr(0.0),
		card_weighted_sum_interactive_diff_sqr(_cards.size(), 0.0),
		n_visits(0),
		match_pair_count(0),
		active_deck_pair_card_matching_count(0),
		total_interactive_weight_scale(0.0),
		balance_score(0.0), symmetry_score(0.0), strength_score(0.0), coverage_score(0.0), diversity_score(0.0), interaction_score(0.0),
		card_balance_scores(_cards.size(), 0.0)
	{
		for (int i = 0; i < _cards.size(); i++)
			card_stats[i].SetCardCost(GetCardOrigCost(_cards[i]));
	}
	void ResetTournament()
	{
		tournmnt_state.ResetState();
	}
	void UpdateBalanceMetrics() // compute balance related metrics, according to (mostly) cumulative stats of the same card pool, potentially across multiple tournaments
	{
		int n_cards = tournmnt_state.card_reps.size();
		double total_partic = (double)(match_pair_count * 4);
		double ave_partic = total_partic / (double)n_cards;
		double half_ave_partic = 0.5 * ave_partic;
		vector<double> card_symmetry_scores(n_cards), card_strength_scores(n_cards), card_coverage_scores(n_cards), card_diversity_scores(n_cards), card_interaction_scores(n_cards);

		/* symmetry - from the second player stat */
		// second player effective win rate anaylsis; closer to 0.5 is better
		double effective_win_offset = snd_player_stat.eval - 0.5;
		double play_order_rating = 1.0 - 400.0 * effective_win_offset * effective_win_offset; // we consider second player winning less than 45% or more than 55% an unacceptable level
		// draw rate anaylsis; lower is better
		double draw_rate = snd_player_stat.draw_contribution / snd_player_stat.total_participation;
		double stalemate_rating = 1.0 - 400.0 * draw_rate * draw_rate; // we consider more than 5% draw being an unacceptable level
		// compute the score as (0.8 * min + 0.2 * ave)
		double min_symmetry_rating = (play_order_rating < stalemate_rating ? play_order_rating : stalemate_rating);
		symmetry_score = 0.8 * min_symmetry_rating + 0.1 * (play_order_rating + stalemate_rating);
		// per card symmetry score - similar formula; here the weighting is a bit nasty though, as the weighting in the global snd_player_stat was linear from each card and the global symmetry score was squared not on the card level but on the global level 
		for (int i = 0; i < n_cards; i++)
		{
			double card_weighted_snd_player_effective_win_offset = (card_stats[i].snd_player_eff_win_contrib - 0.5 * card_stats[i].effective_win_contribution) / half_ave_partic; // use half_ave_partic as scale, as on average, each card is with the second player half of the times
			double card_play_order_rating = 1.0 - 400.0 * card_weighted_snd_player_effective_win_offset * card_weighted_snd_player_effective_win_offset;
			double card_weighted_draw_rate = card_stats[i].draw_contribution / ave_partic;
			double card_stalemate_rating = 1.0 - 400.0 * card_weighted_draw_rate * card_weighted_draw_rate;
			double min_card_symmetry_rating = (card_play_order_rating < card_stalemate_rating ? card_play_order_rating : card_stalemate_rating);
			card_symmetry_scores[i] = 0.8 * min_card_symmetry_rating + 0.1 * (card_play_order_rating + card_stalemate_rating);
		}

		/* strength - from cards */
		// we want the card strengths to be not too drastic, by constraining the weighted
		// without this, card set with one or very few vastly overpowered cards are not penalized enough (e.g. those cards combined with any cards would be very powerful and other cards may not make much of a difference)
		// we do not want this to put similar contraints explicitly on decks, because it is good to have carefully built decks to be much stronger than carelessly built ones
		double sum_weighted_sqr_offset = 0.0;
		vector<double> card_weighted_sqr_offset(n_cards);
		for (int i = 0; i < n_cards; i++)
		{
			double tmp_offset = card_stats[i].eval - 0.5;
			double tmp_term = card_stats[i].total_participation * tmp_offset * tmp_offset;
			sum_weighted_sqr_offset += tmp_term;
			card_weighted_sqr_offset[i] = tmp_term / ave_partic;
		}
		// strictly speaking the average square offset is bounded in 0 ~ 0.25 (offset magnitude being in 0 ~ 0.5)
		// but we think weighted average square offset being 1/400 (offset magnitude at 0.05 roughly on average) as unacceptable
		strength_score = 1.0 - 400.0 * sum_weighted_sqr_offset / total_partic;
		// per card strength score - similar formula
		for (int i = 0; i < n_cards; i++)
			card_strength_scores[i] = 1.0 - 400.0 * card_weighted_sqr_offset[i];

		/* coverage - from the active decks */
		// note, for coverage, we do not compute the union of the covered cards in active decks from different tournaments, because this is going to systematically shift up the coverage with more tests
		// instead, we average the coverage from different tournaments
		double coverage_rate = sum_coverage / (double)n_visits;
		coverage_score = coverage_rate * coverage_rate; // use square to match the dimensionality of other scores
		// per card coverage score - coverage rate & adjusted participation
		double sum_adjusted_partic = 0.0;
		for (int i = 0; i < n_cards; i++)
			sum_adjusted_partic += card_stats[i].adjusted_participation;
		double ave_adjusted_partic = sum_adjusted_partic / (double)n_cards;
		for (int i = 0; i < n_cards; i++)
		{
			double card_coverage_rate = (double)card_sum_coverages[i] / (double)n_visits;
			double card_coverage_rate_rating = card_coverage_rate * card_coverage_rate;
			double normalized_card_adjusted_partic = card_stats[i].adjusted_participation / ave_adjusted_partic;
			double card_adjusted_partic_rating = (normalized_card_adjusted_partic > 1.0 ? 1.0 : normalized_card_adjusted_partic); // clamp at 1.0, as for this part we only want to exclude those getting used abnormally little
			double min_card_coverage_rating = (card_coverage_rate_rating < card_adjusted_partic_rating ? card_coverage_rate_rating : card_adjusted_partic_rating);
			card_coverage_scores[i] = 0.8 * min_card_coverage_rating + 0.1 * (card_coverage_rate_rating + card_adjusted_partic_rating); // (0.8 * min + 0.2 * ave)
		}

		/* diversity - from all decks and the active decks */
		// nearest card neighbor distance analysis (ave. neigbor distance is already computed in the state)
		double nearest_neibor_rating = 12.0 * tournmnt_state.ave_neibor_dist; // well the formula or scaling should theoretically depend on the number of cards and the the dimensionality of the card space, but its hard to say what is the effective dimensionality
		// active deck diff analysis
		double ave_deck_diff_per_card = sum_active_deck_diff / (double)active_deck_pair_card_matching_count;
		double deck_diff_rating = 3.0 * ave_deck_diff_per_card; // apply some scaling
		// compute the score as (0.8 * min + 0.2 * ave)
		double min_diversity_rating = (nearest_neibor_rating < deck_diff_rating ? nearest_neibor_rating : deck_diff_rating);
		diversity_score = 0.8 * min_diversity_rating + 0.1 * (nearest_neibor_rating + deck_diff_rating);
		// per card diversity score - nearest neibor distance only
		for (int i = 0; i < n_cards; i++)
			card_diversity_scores[i] = 12.0 * tournmnt_state.neibor_dists[i];

		/* interaction - from all decks */
		// we want it hard to estimate a card's win contribution in the deck by looking at how good each card in the deck is for a whole tournament
		interaction_score = 50.0 * weighted_sum_interative_diff_sqr / total_interactive_weight_scale;
		// per card interaction score - simular formula
		double card_interaction_score_factor = 50.0 * (double)n_cards / total_interactive_weight_scale; // the n_cards is here because the scale is distributed to cards
		for (int i = 0; i < n_cards; i++)
			card_interaction_scores[i] = card_weighted_sum_interactive_diff_sqr[i] * card_interaction_score_factor;

		// global (card set) balance score
		double min_score = symmetry_score;
		if (strength_score < min_score)
			min_score = strength_score;
		if (coverage_score < min_score)
			min_score = coverage_score;
		if (diversity_score < min_score)
			min_score = diversity_score;
		if (interaction_score < min_score)
			min_score = interaction_score;		
		balance_score = 0.8 * min_score + 0.04 * (symmetry_score + strength_score + coverage_score + diversity_score + interaction_score); // 0.8 * min + 0.2 * ave
		// per card balance score
		for (int i = 0; i < n_cards; i++)
		{
			double min_card_score = card_symmetry_scores[i];
			if (card_strength_scores[i] < min_card_score)
				min_card_score = card_strength_scores[i];
			if (card_coverage_scores[i] < min_card_score)
				min_card_score = card_coverage_scores[i];
			if (card_diversity_scores[i] < min_card_score)
				min_card_score = card_diversity_scores[i];
			if (card_interaction_scores[i] < min_card_score)
				min_card_score = card_interaction_scores[i];
			card_balance_scores[i] = 0.8 * min_card_score + 0.04 * (card_symmetry_scores[i] + card_strength_scores[i] + card_coverage_scores[i] + card_diversity_scores[i] + card_interaction_scores[i]); // 0.8 * min + 0.2 * ave
		}
	}
	void AppendTournamentStats() // call this after a tournament is done; this also calls UpdataBalanceMetrics to update balance related metrics
	{
		// stats
		AppendBalanceStats(card_stats, tournmnt_state.card_stats);
		snd_player_stat.AppendStat(tournmnt_state.snd_player_stat);

		// coverage
		int n_cards = tournmnt_state.card_reps.size();
		vector<int> flags(n_cards, 0);
		for (const vector<int>& deck : tournmnt_state.active_decks)
			for (int card_index : deck)
				flags[card_index] = 1;
		int coverage_count = 0;
		for (int i = 0; i < n_cards; i++)
		{
			coverage_count += flags[i];
			card_sum_coverages[i] += flags[i];
		}
		double coverage_rate = (double)coverage_count / (double)n_cards;
		sum_coverage += coverage_rate;

		// active deck diff
		int n_active_decks = tournmnt_state.active_decks.size();
		int deck_size = tournmnt_state.active_decks[0].size();
		active_deck_pair_card_matching_count += n_active_decks * (n_active_decks - 1) * deck_size / 2;
		double sum_diff = 0.0;
		for (int i = 0; i < n_active_decks - 1; i++)
		{
			for (int j = i + 1; j < n_active_decks; j++)
			{
				// prepare the diff table 
				vector<vector<double>> pair_diff_table(deck_size, vector<double>(deck_size));
				for (int ii = 0; ii < deck_size; ii++)
				{
					int card_index_a = tournmnt_state.active_decks[i][ii];
					for (int jj = 0; jj < deck_size; jj++)
					{
						int card_index_b = tournmnt_state.active_decks[j][jj];
						pair_diff_table[ii][jj] += tournmnt_state.card_pair_diffs[card_index_a][card_index_b];
					}
				}

				// use Hungarian algorithm to find the min diff matching, and use the total pair diff in that matching as the deck diff metric
				vector<int> matched_indices;
				HungarianAssignment(pair_diff_table, matched_indices);
				for (int ii = 0; ii < deck_size; ii++)
					sum_diff += pair_diff_table[ii][matched_indices[ii]];
			}
		}
		sum_active_deck_diff += sum_diff;

		// interaction
		double weighted_sum_diff_sqr = 0.0;
		int n_decks = tournmnt_state.all_decks.size();
		for (int i = 0; i < n_decks; i++)
		{
			double tmp_weight = tournmnt_state.all_deck_stats[i].total_participation;
			double real_deck_win_contrib = tournmnt_state.all_deck_stats[i].effective_win_contribution;
			vector<double> real_card_win_contribs(deck_size);
			double pseudo_deck_win_contrib = 0.0;
			vector<double> pseudo_card_win_contribs(deck_size);
			for (int j = 0; j < deck_size; j++)
			{
				real_card_win_contribs[j] = tournmnt_state.card_stats_in_all_decks[i][j].effective_win_contribution;
				// use the overal eval to get the pseudo contrib
				// not using the card eval from the accumulated stats, but using the current tournmnt, for 
				// 1. there are certain things that make more sense for the local environment for the current tournmnt, not 100% how much it applies here, but can be so to a certain degree;
				// 2. only applying the accumulated eval to current tournmnt but not retroactively is likely to make earlier tournmnts less reliable than later ones, applying it retroactively would require storing a lot more info which would give much pressure to memory. 
				double tmp_card_contrib = tournmnt_state.card_stats_in_all_decks[i][j].total_participation * tournmnt_state.card_stats[tournmnt_state.all_decks[i][j]].eval;
				pseudo_card_win_contribs[j] = tmp_card_contrib;
				pseudo_deck_win_contrib += tmp_card_contrib;
			}
			double tmp_sum_diff_sqr = 0.0;
			for (int j = 0; j < deck_size; j++)
			{
				double tmp_diff = (real_deck_win_contrib == 0.0 ? 1.0 / (double)deck_size : real_card_win_contribs[j] / real_deck_win_contrib) - (pseudo_deck_win_contrib == 0.0 ? 1.0 / (double)deck_size : pseudo_card_win_contribs[j] / pseudo_deck_win_contrib);
				double tmp_diff_sqr = tmp_diff * tmp_diff;
				tmp_sum_diff_sqr += tmp_diff_sqr;
				card_weighted_sum_interactive_diff_sqr[tournmnt_state.all_decks[i][j]] += tmp_weight * tmp_diff_sqr;
			}
			weighted_sum_diff_sqr += tmp_weight * tmp_sum_diff_sqr;
		}
		weighted_sum_interative_diff_sqr += weighted_sum_diff_sqr;
		total_interactive_weight_scale += (double)(tournmnt_state.match_pair_count * 4) / (double)deck_size; // because it is sqr diff and also the contribs are normalized to a total of 1.0 for each deck, so the sum_diff_sqr is likely inversely scaled by deck_size  

		// other counters etc.
		n_visits++;
		match_pair_count += tournmnt_state.match_pair_count;

		// update balance related metrics
		UpdateBalanceMetrics();
	}

	TournamentState tournmnt_state;
	vector<BalanceStat> card_stats;
	BalanceStat snd_player_stat;
	double sum_coverage;
	vector<int> card_sum_coverages; // for each card it is either covered or not in each tournament, so using int type would be sufficient
	double sum_active_deck_diff;
	double weighted_sum_interative_diff_sqr;
	vector<double> card_weighted_sum_interactive_diff_sqr;
	int n_visits;
	int match_pair_count;
	int active_deck_pair_card_matching_count; // the sum of following from each tournament: n_active_decks * (n_active_decks - 1) * deck_size / 2, which stands for the number of card matchings between each (ordered) pair of different active decks
	double total_interactive_weight_scale; // scale to divide by for the interaction score
	double balance_score, symmetry_score, strength_score, coverage_score, diversity_score, interaction_score;
	vector<double> card_balance_scores;
};


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
			cout << "AI_A Deck " << i << "(goes first) VS AI_B Deck " << j << "(goes second). ";

			vector<int> deck_a_shuffle(deck_size);
			vector<int> deck_b_shuffle(deck_size);
			vector<int> deck_a_indices(deck_size);
			vector<int> deck_b_indices(deck_size);
			vector<Card*> deck_a(deck_size);
			vector<Card*> deck_b(deck_size);

			InitMatch(card_list, deck_list[i], deck_list[j], deck_a_shuffle, deck_b_shuffle, deck_a_indices, deck_b_indices, deck_a, deck_b);

			queue<DeferredEvent*> event_queue;
			Player player1("AI_A", 30, deck_a, true, event_queue, ai_level_a, card_list);
			Player player2("AI_B", 30, deck_b, true, event_queue, ai_level_b, card_list);

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
				delete event_queue.front();
				event_queue.pop();
			}

			if (player1.CheckLose())
			{
				if (player2.CheckLose())
				{
					// draw
					ai_stat_a.DrawUpdate(1.0, false);
					ai_stat_b.DrawUpdate(1.0, true);
					cout << "They drew." << endl;
				}
				else
				{
					// AI_A loses, deck AI_B wins
					ai_stat_a.LoseUpdate(1.0, false);
					ai_stat_b.WinUpdate(1.0, true);
					cout << "AI_B won." << endl;
				}
			}
			else
			{
				// AI_A wins, deck AI_B loses
				ai_stat_a.WinUpdate(1.0, false);
				ai_stat_b.LoseUpdate(1.0, true);
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


int SimulateSingleMatchBetweenDecks(int ai_level, vector<Card*>& card_list, const vector<int>& deck_a_orig_indices, const vector<int>& deck_b_orig_indices,
	vector<BalanceStat>& card_stats, BalanceStat& deck_a_stat, BalanceStat& deck_b_stat, vector<BalanceStat>& card_stats_in_deck_a, vector<BalanceStat>& card_stats_in_deck_b, BalanceStat& snd_player_stat,
	MatchRec& rec, bool is_inverted, int deck_size) // return number of turns when the match ends, both sides summed
{
	vector<int> deck_a_shuffle(deck_size);
	vector<int> deck_b_shuffle(deck_size);
	vector<int> deck_a_indices(deck_size);
	vector<int> deck_b_indices(deck_size);
	vector<Card*> deck_a(deck_size);
	vector<Card*> deck_b(deck_size);

	InitMatch(card_list, deck_a_orig_indices, deck_b_orig_indices, deck_a_shuffle, deck_b_shuffle, deck_a_indices, deck_b_indices, deck_a, deck_b);

	queue<DeferredEvent*> event_queue;
	Player player1("AI_Deck_A", 30, deck_a, true, event_queue, ai_level, card_list);
	Player player2("AI_Deck_B", 30, deck_b, true, event_queue, ai_level, card_list);

	vector<int> contribution_counters_a(deck_size, 0);
	vector<int> cost_constrained_counters_a(deck_size, 0);
	player1.RegisterCardContributions(contribution_counters_a, cost_constrained_counters_a);
	vector<int> contribution_counters_b(deck_size, 0);
	vector<int> cost_constrained_counters_b(deck_size, 0);
	player2.RegisterCardContributions(contribution_counters_b, cost_constrained_counters_b);

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
		delete event_queue.front();
		event_queue.pop();
	}

	int num_turns = player1.turn_num + player2.turn_num;

	int sum_contribution_a = 0;
	for (int k = 0; k < deck_size; k++)
		sum_contribution_a += contribution_counters_a[k];
	int sum_contribution_b = 0;
	for (int k = 0; k < deck_size; k++)
		sum_contribution_b += contribution_counters_b[k];

	vector<double> processed_contribution_a(deck_size); // recorded to orginal card order before shuffling, also normalize (used for match records)
	for (int k = 0; k < deck_size; k++)
		processed_contribution_a[deck_a_shuffle[k]] = (double)contribution_counters_a[k] / (double)sum_contribution_a;
	vector<double> processed_contribution_b(deck_size);
	for (int k = 0; k < deck_size; k++)
		processed_contribution_b[deck_b_shuffle[k]] = (double)contribution_counters_b[k] / (double)sum_contribution_b;

	if (player1.CheckLose())
	{
		if (player2.CheckLose())
		{
			// draw
			rec.DrawUpdate(processed_contribution_a, processed_contribution_b);
			deck_a_stat.DrawUpdate(1.0, false);
			deck_b_stat.DrawUpdate(1.0, true);
			snd_player_stat.DrawUpdate(1.0, true);
			for (int k = 0; k < deck_size; k++)
			{
				int index_a = deck_a_indices[k];
				double partic_a = (double)contribution_counters_a[k] / (double)sum_contribution_a;
				double cost_constrained_partic_a = (double)cost_constrained_counters_a[k] / (double) sum_contribution_a;
				card_stats[index_a].DrawUpdate(partic_a, cost_constrained_partic_a, player1.turn_num, false);
				card_stats_in_deck_a[deck_a_shuffle[k]].DrawUpdate(partic_a, false); // currently don't need adjusted participation, so no need to update that (also btw the eval there won't be used either); also need to get back to the original index because the deck is shuffled
				int index_b = deck_b_indices[k];
				double partic_b = (double)contribution_counters_b[k] / (double)sum_contribution_b;
				double cost_constrained_partic_b = (double)cost_constrained_counters_b[k] / (double) sum_contribution_b;
				card_stats[index_b].DrawUpdate(partic_b, cost_constrained_partic_b, player2.turn_num, true);
				card_stats_in_deck_b[deck_b_shuffle[k]].DrawUpdate(partic_b, true); // currently don't need adjusted participation, so no need to update that (also btw the eval there won't be used either); also need to get back to the original index because the deck is shuffled
			}
			#ifndef SUPPRESS_TOURNAMENT_PROMPTS
			cout << "They drew." << endl;
			#endif
		}
		else
		{
			// 1st deck loses, 2nd deck wins
			if (is_inverted)
				rec.LoseUpdate(processed_contribution_b, processed_contribution_a);
			else
				rec.WinUpdate(processed_contribution_a, processed_contribution_b);
			deck_a_stat.LoseUpdate(1.0, false);
			deck_b_stat.WinUpdate(1.0, true);
			snd_player_stat.WinUpdate(1.0, true);
			for (int k = 0; k < deck_size; k++)
			{
				int index_a = deck_a_indices[k];
				double partic_a = (double)contribution_counters_a[k] / (double)sum_contribution_a;
				double cost_constrained_partic_a = (double)cost_constrained_counters_a[k] / (double) sum_contribution_a;
				card_stats[index_a].LoseUpdate(partic_a, cost_constrained_partic_a, player1.turn_num, false);
				card_stats_in_deck_a[deck_a_shuffle[k]].LoseUpdate(partic_a, false); // currently don't need adjusted participation, so no need to update that (also btw the eval there won't be used either); also need to get back to the original index because the deck is shuffled
				int index_b = deck_b_indices[k];
				double partic_b = (double)contribution_counters_b[k] / (double)sum_contribution_b;
				double cost_constrained_partic_b = (double)cost_constrained_counters_b[k] / (double) sum_contribution_b;
				card_stats[index_b].WinUpdate(partic_b, cost_constrained_partic_b, player2.turn_num, true);
				card_stats_in_deck_b[deck_b_shuffle[k]].WinUpdate(partic_b, true); // currently don't need adjusted participation, so no need to update that (also btw the eval there won't be used either); also need to get back to the original index because the deck is shuffled
			}
			#ifndef SUPPRESS_TOURNAMENT_PROMPTS
			cout << "The second deck won." << endl;
			#endif
		}
	}
	else
	{
		// 1st deck wins, 2nd deck loses
		if (is_inverted)
			rec.WinUpdate(processed_contribution_b, processed_contribution_a);
		else
			rec.LoseUpdate(processed_contribution_a, processed_contribution_b);
		deck_a_stat.WinUpdate(1.0, false);
		deck_b_stat.LoseUpdate(1.0, true);
		snd_player_stat.LoseUpdate(1.0, true);
		for (int k = 0; k < deck_size; k++)
		{
			int index_a = deck_a_indices[k];
			double partic_a = (double)contribution_counters_a[k] / (double)sum_contribution_a;
			double cost_constrained_partic_a = (double)cost_constrained_counters_a[k] / (double) sum_contribution_a;
			card_stats[index_a].WinUpdate(partic_a, cost_constrained_partic_a, player1.turn_num, false);
			card_stats_in_deck_a[deck_a_shuffle[k]].WinUpdate(partic_a, false); // currently don't need adjusted participation, so no need to update that (also btw the eval there won't be used either); also need to get back to the original index because the deck is shuffled
			int index_b = deck_b_indices[k];
			double partic_b = (double)contribution_counters_b[k] / (double)sum_contribution_b;
			double cost_constrained_partic_b = (double)cost_constrained_counters_b[k] / (double) sum_contribution_b;
			card_stats[index_b].LoseUpdate(partic_b, cost_constrained_partic_b, player2.turn_num, true);
			card_stats_in_deck_b[deck_b_shuffle[k]].LoseUpdate(partic_b, true); // currently don't need adjusted participation, so no need to update that (also btw the eval there won't be used either); also need to get back to the original index because the deck is shuffled
		}
		#ifndef SUPPRESS_TOURNAMENT_PROMPTS
		cout << "The first deck won." << endl;
		#endif
	}

	return num_turns;
}

int SimulatePairMatchBetweenDecks(int ai_level, vector<Card*>& card_list, const vector<int>& deck_a_orig_indices, const vector<int>& deck_b_orig_indices,
	vector<BalanceStat>& card_stats, BalanceStat& deck_a_stat, BalanceStat& deck_b_stat, vector<BalanceStat>& card_stats_in_deck_a, vector<BalanceStat>& card_stats_in_deck_b, BalanceStat& snd_player_stat,
	MatchRec& rec, int deck_size) // return total number of match turns (both sides summed)
{
	// test both the playing order (which player plays first and with plays second)
	int turn_count_1 = SimulateSingleMatchBetweenDecks(ai_level, card_list, deck_a_orig_indices, deck_b_orig_indices, card_stats, deck_a_stat, deck_b_stat, card_stats_in_deck_a, card_stats_in_deck_b, snd_player_stat, rec, false, deck_size);
	int turn_count_2 = SimulateSingleMatchBetweenDecks(ai_level, card_list, deck_b_orig_indices, deck_a_orig_indices, card_stats, deck_b_stat, deck_a_stat, card_stats_in_deck_b, card_stats_in_deck_a, snd_player_stat, rec, true, deck_size);
	return turn_count_1 + turn_count_2;
}

void TestNewDeck(int ai_level, TournamentState& tournmnt_state, int deck_size, double temperature, int num_pair_matches)
{
	int p = tournmnt_state.card_list.size();
	int n_decks = tournmnt_state.active_decks.size();
	
	/* creating the new deck*/
	vector<int> new_deck(deck_size);
	double two_ln_visits = 2.0 * log(4.0 * tournmnt_state.match_pair_count * deck_size); // the natural logarithm of total number of times any card is tested so far
	vector<double> weights(p);
	vector<int> unvisited_indices;
	for (int i = 0; i < p; i++)
	{
		int tmp_visits = tournmnt_state.card_stats[i].total_num;
		if (tmp_visits == 0) // not tested at all, mark as -1
			unvisited_indices.push_back(i);
		else
			weights[i] = tournmnt_state.card_stats[i].eval + sqrt(two_ln_visits/(double)tmp_visits);
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
		if (new_deck == tournmnt_state.active_decks[i]) // comparing two vector of ints; as they are expected to be sorted so that makes sense here
		{
			duplicate_index = i;
			break;
		}

	/* perform tests, update the deck pool if necessary */
	if (duplicate_index >= 0) // if it is a duplicate, just re-test
	{
		for (int i = 0; i < n_decks; i++)
			if (i != duplicate_index)
			{
				MatchRec rec(tournmnt_state.active_decks[i], tournmnt_state.active_decks[duplicate_index]);
				for (int k = 0; k < num_pair_matches; k++)
				{
					tournmnt_state.match_pair_count++;
					#ifndef SUPPRESS_TOURNAMENT_PROMPTS
					cout << "Match Pair #" << tournmnt_state.match_pair_count << endl;
					#endif
					tournmnt_state.turn_count += SimulatePairMatchBetweenDecks(ai_level, tournmnt_state.card_list, tournmnt_state.active_decks[i], tournmnt_state.active_decks[duplicate_index], tournmnt_state.card_stats, tournmnt_state.active_deck_stats[i], tournmnt_state.active_deck_stats[duplicate_index],
						tournmnt_state.card_stats_in_active_decks[i], tournmnt_state.card_stats_in_active_decks[duplicate_index], tournmnt_state.snd_player_stat, rec, deck_size);
				}
				rec.UpdateStats();
				tournmnt_state.rec_list.push_back(rec);
			}
		UpdateStatEvals(tournmnt_state.active_deck_stats);
	}
	else // if it is not a duplicate, test and decide whether to replace the worst among other decks
	{
		BalanceStat tmp_deck_stat;
		vector<BalanceStat> tmp_card_stats_in_deck(deck_size);
		for (int i = 0; i < n_decks; i++)
		{
			MatchRec rec(tournmnt_state.active_decks[i], new_deck);
			for (int k = 0; k < num_pair_matches; k++)
			{
				tournmnt_state.match_pair_count++;
				#ifndef SUPPRESS_TOURNAMENT_PROMPTS
				cout << "Match Pair #" << tournmnt_state.match_pair_count << endl;
				#endif
				tournmnt_state.turn_count += SimulatePairMatchBetweenDecks(ai_level, tournmnt_state.card_list, tournmnt_state.active_decks[i], new_deck, tournmnt_state.card_stats, tournmnt_state.active_deck_stats[i], tmp_deck_stat,
					tournmnt_state.card_stats_in_active_decks[i], tmp_card_stats_in_deck, tournmnt_state.snd_player_stat, rec, deck_size);
			}
			rec.UpdateStats();
			tournmnt_state.rec_list.push_back(rec);
		}
		UpdateStatEvals(tournmnt_state.active_deck_stats);
		tmp_deck_stat.UpdateEval();

		// find the worst among other decks
		double min_eval = 10.0;
		int min_index = -1; // this should always be overwritten
		for (int i = 0; i < n_decks; i++)
			if (tournmnt_state.active_deck_stats[i].eval < min_eval)
			{
				min_eval = tournmnt_state.active_deck_stats[i].eval;
				min_index = i;
			}	
		
		// use Metropolis-MC like rejection scheme to decide whether to accept the replacement or not
		double accept_prob = exp((tmp_deck_stat.eval - min_eval)/temperature);
		double roll = GetGiglRandFloat();
		if (roll < accept_prob) // accept and replace, otherwise don't do the replacement
		{
			// when accept, archive the replaced deck
			tournmnt_state.all_decks.push_back(tournmnt_state.active_decks[min_index]);
			tournmnt_state.all_deck_stats.push_back(tournmnt_state.active_deck_stats[min_index]);
			tournmnt_state.card_stats_in_all_decks.push_back(tournmnt_state.card_stats_in_active_decks[min_index]);
			tournmnt_state.active_decks[min_index] = new_deck;
			tournmnt_state.active_deck_stats[min_index] = tmp_deck_stat;
			tournmnt_state.card_stats_in_active_decks[min_index] = tmp_card_stats_in_deck;
			#ifndef SUPPRESS_TOURNAMENT_PROMPTS
			cout << "Update accepted." << endl;
			#endif
		}
		else
		{
			// when reject, archive the attempted deck
			tournmnt_state.all_decks.push_back(new_deck);
			tournmnt_state.all_deck_stats.push_back(tmp_deck_stat);
			tournmnt_state.card_stats_in_all_decks.push_back(tmp_card_stats_in_deck);
			#ifndef SUPPRESS_TOURNAMENT_PROMPTS
			cout << "Update rejected." << endl;
			#endif
		}
	}
}

void TestCrossOver(int ai_level, TournamentState& tournmnt_state, int deck_size, double temperature, int num_pair_matches)
{
	int n_decks = tournmnt_state.active_decks.size();

	/* creating the cross over offsprings (two) */
	vector<int> selected_deck_indices = CreateRandomSelection(n_decks, 2);
	vector<vector<int>> deck_copies(2);
	for (int i = 0; i < 2; i++)
		deck_copies[i] = tournmnt_state.active_decks[selected_deck_indices[i]];

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
			if (deck_copies[i] == tournmnt_state.active_decks[j]) // comparing two vector of ints; as they are expected to be sorted so that makes sense here
			{
				duplicate_indices[i] = j;
				break;	
			}
	}
	
	/* perform tests */
	vector<BalanceStat> tmp_deck_stats(2);
	vector<vector<BalanceStat>> tmp_card_stats_in_decks(2);
	tmp_card_stats_in_decks[0].resize(deck_size);
	tmp_card_stats_in_decks[1].resize(deck_size);
	for (int i = 0; i < 2; i++)
	{
		if (duplicate_indices[i] >= 0) // if it is a duplicate
		{
			for (int j = 0; j < n_decks; j++)
				if (j != duplicate_indices[i])
				{
					MatchRec rec(tournmnt_state.active_decks[j], tournmnt_state.active_decks[duplicate_indices[i]]);
					for (int k = 0; k < num_pair_matches; k++)
					{
						tournmnt_state.match_pair_count++;
						#ifndef SUPPRESS_TOURNAMENT_PROMPTS
						cout << "Match Pair #" << tournmnt_state.match_pair_count << endl;
						#endif
						tournmnt_state.turn_count += SimulatePairMatchBetweenDecks(ai_level, tournmnt_state.card_list, tournmnt_state.active_decks[j], tournmnt_state.active_decks[duplicate_indices[i]], tournmnt_state.card_stats, tournmnt_state.active_deck_stats[j], tournmnt_state.active_deck_stats[duplicate_indices[i]],
							tournmnt_state.card_stats_in_active_decks[j], tournmnt_state.card_stats_in_active_decks[duplicate_indices[i]], tournmnt_state.snd_player_stat, rec, deck_size);
					}
					rec.UpdateStats();
					tournmnt_state.rec_list.push_back(rec);
				}
			UpdateStatEvals(tournmnt_state.active_deck_stats);
		}
		else // if it is not a duplicate
		{
			for (int j = 0; j < n_decks; j++)
			{
				MatchRec rec(tournmnt_state.active_decks[j], deck_copies[i]);
				for (int k = 0; k < num_pair_matches; k++)
				{
					tournmnt_state.match_pair_count++;
					#ifndef SUPPRESS_TOURNAMENT_PROMPTS
					cout << "Match Pair #" << tournmnt_state.match_pair_count << endl;
					#endif
					tournmnt_state.turn_count += SimulatePairMatchBetweenDecks(ai_level, tournmnt_state.card_list, tournmnt_state.active_decks[j], deck_copies[i], tournmnt_state.card_stats, tournmnt_state.active_deck_stats[j], tmp_deck_stats[i],
						tournmnt_state.card_stats_in_active_decks[j], tmp_card_stats_in_decks[i], tournmnt_state.snd_player_stat, rec, deck_size);
				}
				rec.UpdateStats();
				tournmnt_state.rec_list.push_back(rec);
			}
			UpdateStatEvals(tournmnt_state.active_deck_stats);
			tmp_deck_stats[i].UpdateEval();
		}	
	}
	// if both are not duplicates, also do a set of matches between them
	if (duplicate_indices[0] < 0 && duplicate_indices[1] < 0)
	{
		MatchRec rec(deck_copies[0], deck_copies[1]);
		for (int k = 0; k < num_pair_matches; k++)
		{
			tournmnt_state.match_pair_count++;
			#ifndef SUPPRESS_TOURNAMENT_PROMPTS
			cout << "Match Pair #" << tournmnt_state.match_pair_count << endl;
			#endif
			tournmnt_state.turn_count += SimulatePairMatchBetweenDecks(ai_level, tournmnt_state.card_list, deck_copies[0], deck_copies[1], tournmnt_state.card_stats, tmp_deck_stats[0], tmp_deck_stats[1],
				tmp_card_stats_in_decks[0], tmp_card_stats_in_decks[1], tournmnt_state.snd_player_stat, rec, deck_size);
		}
		rec.UpdateStats();
		tournmnt_state.rec_list.push_back(rec);
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
			double accept_prob = exp(((tmp_deck_stats[0].eval + tmp_deck_stats[1].eval) - (tournmnt_state.active_deck_stats[selected_deck_indices[0]].eval + tournmnt_state.active_deck_stats[selected_deck_indices[1]].eval))/temperature);
			double roll = GetGiglRandFloat();
			if (roll < accept_prob) // accept and replace, otherwise don't do the replacement
			{
				for (int i = 0; i < 2; i++)
				{
					// when accept, archive the replaced decks
					tournmnt_state.all_decks.push_back(tournmnt_state.active_decks[selected_deck_indices[i]]);
					tournmnt_state.all_deck_stats.push_back(tournmnt_state.active_deck_stats[selected_deck_indices[i]]);
					tournmnt_state.card_stats_in_all_decks.push_back(tournmnt_state.card_stats_in_active_decks[selected_deck_indices[i]]);
					tournmnt_state.active_decks[selected_deck_indices[i]] = deck_copies[i];
					tournmnt_state.active_deck_stats[selected_deck_indices[i]] = tmp_deck_stats[i];
					tournmnt_state.card_stats_in_active_decks[selected_deck_indices[i]] = tmp_card_stats_in_decks[i];
				}
				#ifndef SUPPRESS_TOURNAMENT_PROMPTS
				cout << "Update accepted." << endl;
				#endif
			}
			else
			{
				for (int i = 0; i < 2; i++)
				{
					// when reject, archive the attempted decks
					tournmnt_state.all_decks.push_back(deck_copies[i]);
					tournmnt_state.all_deck_stats.push_back(tmp_deck_stats[i]);
					tournmnt_state.card_stats_in_all_decks.push_back(tmp_card_stats_in_decks[i]);
				}
				#ifndef SUPPRESS_TOURNAMENT_PROMPTS
				cout << "Update rejected." << endl;
				#endif
			}
		}
		else // only the first deck is new
		{
			// first figure out the weaker of the two parents
			int replace_target = (tournmnt_state.active_deck_stats[selected_deck_indices[0]].eval <= tournmnt_state.active_deck_stats[selected_deck_indices[1]].eval ? selected_deck_indices[0] : selected_deck_indices[1]);
			// use Metropolis-MC like rejection scheme to decide whether to accept the replacement or not
			double accept_prob = exp((tmp_deck_stats[0].eval - tournmnt_state.active_deck_stats[replace_target].eval)/temperature);
			double roll = GetGiglRandFloat();
			if (roll < accept_prob) // accept and replace, otherwise don't do the replacement
			{
				// when accept, archive the replaced deck
				tournmnt_state.all_decks.push_back(tournmnt_state.active_decks[replace_target]);
				tournmnt_state.all_deck_stats.push_back(tournmnt_state.active_deck_stats[replace_target]);
				tournmnt_state.card_stats_in_all_decks.push_back(tournmnt_state.card_stats_in_active_decks[replace_target]);
				tournmnt_state.active_decks[replace_target] = deck_copies[0];
				tournmnt_state.active_deck_stats[replace_target] = tmp_deck_stats[0];
				tournmnt_state.card_stats_in_active_decks[replace_target] = tmp_card_stats_in_decks[0];
				#ifndef SUPPRESS_TOURNAMENT_PROMPTS
				cout << "Update accepted." << endl;
				#endif
			}
			else
			{
				// when reject, archive the attempted deck
				tournmnt_state.all_decks.push_back(deck_copies[0]);
				tournmnt_state.all_deck_stats.push_back(tmp_deck_stats[0]);
				tournmnt_state.card_stats_in_all_decks.push_back(tmp_card_stats_in_decks[0]);
				#ifndef SUPPRESS_TOURNAMENT_PROMPTS
				cout << "Update rejected." << endl;
				#endif
			}
		}
	}
	else
	{
		if (duplicate_indices[1] < 0) // only the second deck is new
		{
			// first figure out the weaker of the two parents
			int replace_target = (tournmnt_state.active_deck_stats[selected_deck_indices[0]].eval <= tournmnt_state.active_deck_stats[selected_deck_indices[1]].eval ? selected_deck_indices[0] : selected_deck_indices[1]);
			// use Metropolis-MC like rejection scheme to decide whether to accept the replacement or not
			double accept_prob = exp((tmp_deck_stats[1].eval - tournmnt_state.active_deck_stats[replace_target].eval)/temperature);
			double roll = GetGiglRandFloat();
			if (roll < accept_prob) // accept and replace, otherwise don't do the replacement
			{
				// when accept, archive the replaced deck
				tournmnt_state.all_decks.push_back(tournmnt_state.active_decks[replace_target]);
				tournmnt_state.all_deck_stats.push_back(tournmnt_state.active_deck_stats[replace_target]);
				tournmnt_state.card_stats_in_all_decks.push_back(tournmnt_state.card_stats_in_active_decks[replace_target]);
				tournmnt_state.active_decks[replace_target] = deck_copies[1];
				tournmnt_state.active_deck_stats[replace_target] = tmp_deck_stats[1];
				tournmnt_state.card_stats_in_active_decks[replace_target] = tmp_card_stats_in_decks[1];
				#ifndef SUPPRESS_TOURNAMENT_PROMPTS
				cout << "Update accepted." << endl;
				#endif
			}
			else
			{
				// when reject, archive the attempted deck
				tournmnt_state.all_decks.push_back(deck_copies[1]);
				tournmnt_state.all_deck_stats.push_back(tmp_deck_stats[1]);
				tournmnt_state.card_stats_in_all_decks.push_back(tmp_card_stats_in_decks[1]);
				#ifndef SUPPRESS_TOURNAMENT_PROMPTS
				cout << "Update rejected." << endl;
				#endif
			}
		}
		// else do nothing
	}
}

void TestMutation(int ai_level, TournamentState& tournmnt_state, int deck_size, double temperature, int num_pair_matches)
{
	int p = tournmnt_state.card_list.size();
	int n_decks = tournmnt_state.active_decks.size();

	/* creating the mutated deck*/	
	int selected_deck_index = GetGiglRandInt(n_decks);
	vector<int> deck_copy = tournmnt_state.active_decks[selected_deck_index];
	double two_ln_visits = 2.0 * log(4.0 * tournmnt_state.match_pair_count * deck_size); // the natural logarithm of total number of times any card is tested so far
	vector<double> weights(p);
	vector<int> unvisited_indices;
	for (int i = 0; i < p; i++)
	{
		int tmp_visits = tournmnt_state.card_stats[i].total_num;
		if (tmp_visits == 0) // not tested at all, mark as -1
			unvisited_indices.push_back(i); // we can decide here (instead of needing to wait for checking/clearing for cards in the selected deck) because any cards in the existing decks should all be already tested
		else
			weights[i] = tournmnt_state.card_stats[i].eval + sqrt(two_ln_visits/(double)tmp_visits);
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
		if (deck_copy == tournmnt_state.active_decks[i]) // comparing two vector of ints; as they are expected to be sorted so that makes sense here
		{
			duplicate_index = i;
			break;	
		}

	/* perform tests, update the deck pool if necessary */
	if (duplicate_index >= 0) // if it is a duplicate, just re-test
	{
		for (int i = 0; i < n_decks; i++)
			if (i != duplicate_index)
			{
				MatchRec rec(tournmnt_state.active_decks[i], tournmnt_state.active_decks[duplicate_index]);
				for (int k = 0; k < num_pair_matches; k++)
				{
					tournmnt_state.match_pair_count++;
					#ifndef SUPPRESS_TOURNAMENT_PROMPTS
					cout << "Match Pair #" << tournmnt_state.match_pair_count << endl;
					#endif
					tournmnt_state.turn_count += SimulatePairMatchBetweenDecks(ai_level, tournmnt_state.card_list, tournmnt_state.active_decks[i], tournmnt_state.active_decks[duplicate_index], tournmnt_state.card_stats, tournmnt_state.active_deck_stats[i], tournmnt_state.active_deck_stats[duplicate_index],
						tournmnt_state.card_stats_in_active_decks[i], tournmnt_state.card_stats_in_active_decks[duplicate_index], tournmnt_state.snd_player_stat, rec, deck_size);
				}
				rec.UpdateStats();
				tournmnt_state.rec_list.push_back(rec);
			}
		UpdateStatEvals(tournmnt_state.active_deck_stats);
	}
	else // if it is not a duplicate, test and decide whether to replace the original deck it mutates from
	{
		BalanceStat tmp_deck_stat;
		vector<BalanceStat> tmp_card_stats_in_deck(deck_size);
		for (int i = 0; i < n_decks; i++)
		{
			MatchRec rec(tournmnt_state.active_decks[i], deck_copy);
			for (int k = 0; k < num_pair_matches; k++)
			{
				tournmnt_state.match_pair_count++;
				#ifndef SUPPRESS_TOURNAMENT_PROMPTS
				cout << "Match Pair #" << tournmnt_state.match_pair_count << endl;
				#endif
				tournmnt_state.turn_count += SimulatePairMatchBetweenDecks(ai_level, tournmnt_state.card_list, tournmnt_state.active_decks[i], deck_copy, tournmnt_state.card_stats, tournmnt_state.active_deck_stats[i], tmp_deck_stat,
					tournmnt_state.card_stats_in_active_decks[i], tmp_card_stats_in_deck, tournmnt_state.snd_player_stat, rec, deck_size);
			}
			rec.UpdateStats();
			tournmnt_state.rec_list.push_back(rec);
		}
		UpdateStatEvals(tournmnt_state.active_deck_stats);
		tmp_deck_stat.UpdateEval();
		
		// use Metropolis-MC like rejection scheme to decide whether to accept the replacement or not
		double accept_prob = exp((tmp_deck_stat.eval - tournmnt_state.active_deck_stats[selected_deck_index].eval)/temperature);
		double roll = GetGiglRandFloat();
		if (roll < accept_prob) // accept and replace, otherwise don't do the replacement
		{
			// when accept, archive the replaced deck
			tournmnt_state.all_decks.push_back(tournmnt_state.active_decks[selected_deck_index]);
			tournmnt_state.all_deck_stats.push_back(tournmnt_state.active_deck_stats[selected_deck_index]);
			tournmnt_state.card_stats_in_all_decks.push_back(tournmnt_state.card_stats_in_active_decks[selected_deck_index]);
			tournmnt_state.active_decks[selected_deck_index] = deck_copy;
			tournmnt_state.active_deck_stats[selected_deck_index] = tmp_deck_stat;
			tournmnt_state.card_stats_in_active_decks[selected_deck_index] = tmp_card_stats_in_deck;
			#ifndef SUPPRESS_TOURNAMENT_PROMPTS
			cout << "Update accepted." << endl;
			#endif
		}
		else
		{
			// when reject, archive the attempted deck
			tournmnt_state.all_decks.push_back(deck_copy);
			tournmnt_state.all_deck_stats.push_back(tmp_deck_stat);
			tournmnt_state.card_stats_in_all_decks.push_back(tmp_card_stats_in_deck);
			#ifndef SUPPRESS_TOURNAMENT_PROMPTS
			cout << "Update rejected." << endl;
			#endif
		}
	}
}

void EvolutionSimulation(TournamentState& tournmnt_state, int deck_pool_size, int deck_size)
{
	int ai_level = 2;

	int card_pool_size = tournmnt_state.card_list.size();
	
	tournmnt_state.active_deck_stats.resize(deck_pool_size);
	tournmnt_state.card_stats_in_active_decks.resize(deck_pool_size);
	for (int i = 0; i < deck_pool_size; i++)
		tournmnt_state.card_stats_in_active_decks[i].resize(deck_size);

	int num_pair_matches_init = 2; // initial number of pair matches between each two decks
	int num_pair_matches_final = 8; // final number of pair matches between each two decks

	// initialization
	#ifndef SUPPRESS_TOURNAMENT_PROMPTS
	cout << "Creating and testing initial deck pool." << endl;
	#endif
	for (int i = 0; i < deck_pool_size; i++)
	{
		vector<int> tmp_deck = CreateRandomSelectionSorted(card_pool_size, deck_size);
		bool is_duplicate;
		do // make sure we don't create duplicate decks
		{
			is_duplicate = false;
			for (int j = 0; j < i; j++)
				if (tmp_deck == tournmnt_state.active_decks[j])
				{
					is_duplicate = true;
					break;
				}
		} while (is_duplicate);
		tournmnt_state.active_decks.push_back(tmp_deck);
	}
	for (int i = 0; i < deck_pool_size - 1; i++)
		for (int j = i + 1; j < deck_pool_size; j++)
		{
			MatchRec rec(tournmnt_state.active_decks[i], tournmnt_state.active_decks[j]);
			for (int k = 0; k < num_pair_matches_init; k++)
			{
				tournmnt_state.match_pair_count++;
				#ifndef SUPPRESS_TOURNAMENT_PROMPTS
				cout << "Match Pair #" << tournmnt_state.match_pair_count << endl;
				#endif
				tournmnt_state.turn_count += SimulatePairMatchBetweenDecks(ai_level, tournmnt_state.card_list, tournmnt_state.active_decks[i], tournmnt_state.active_decks[j], tournmnt_state.card_stats, tournmnt_state.active_deck_stats[i], tournmnt_state.active_deck_stats[j],
					tournmnt_state.card_stats_in_active_decks[i], tournmnt_state.card_stats_in_active_decks[j], tournmnt_state.snd_player_stat, rec, deck_size);
			}
			rec.UpdateStats();
			tournmnt_state.rec_list.push_back(rec);
		}
	#ifndef SUPPRESS_TOURNAMENT_PROMPTS
	cout << endl;
	#endif
	UpdateStatEvals(tournmnt_state.card_stats);
	UpdateStatEvals(tournmnt_state.active_deck_stats);

	// evolving the deck pool and continue testing
	int num_evolve_iters = 100;
	double t_init = 1.0; // intial "temperature"
	double ln_t_init = log(t_init);
	double t_final = 0.001;
	double ln_t_final = log(t_final);
	double action_probs_init[] = { 0.7, 0.1, 0.2 }; // new deck; cross-over; mutation
	double action_probs_final[] = { 0.2, 0.5, 0.3 }; // new deck; cross-over; mutation 	
	for (int iter = 1; iter <= num_evolve_iters; iter++)
	{
		#ifndef SUPPRESS_TOURNAMENT_PROMPTS
		cout << "Evolution iteration #" << iter << ": ";
		#endif
		double progress = (double)iter / (double)num_evolve_iters;
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
			#ifndef SUPPRESS_TOURNAMENT_PROMPTS
			cout << "testing a new deck" << endl;
			#endif
			TestNewDeck(ai_level, tournmnt_state, deck_size, t, num_pair_matches);
		}
		else if (roll < action_probs[1]) // cross-over
		{
			#ifndef SUPPRESS_TOURNAMENT_PROMPTS
			cout << "testing the cross-overs of two existing decks" << endl;
			#endif
			TestCrossOver(ai_level, tournmnt_state, deck_size, t, num_pair_matches);
		}
		else // mutation
		{
			#ifndef SUPPRESS_TOURNAMENT_PROMPTS
			cout << "testing the mutation of an existing deck" << endl;
			#endif
			TestMutation(ai_level, tournmnt_state, deck_size, t, num_pair_matches);
		}
		UpdateStatEvals(tournmnt_state.card_stats); // deck stats should already be updated inside those "Test" functions
		#ifndef SUPPRESS_TOURNAMENT_PROMPTS
		cout << endl;
		#endif
	}

	// stop evolving and do the final round of tests
	for (int i = 0; i < deck_pool_size - 1; i++)
		for (int j = i + 1; j < deck_pool_size; j++)
		{
			MatchRec rec(tournmnt_state.active_decks[i], tournmnt_state.active_decks[j]);
			for (int k = 0; k < num_pair_matches_final; k++)
			{
				tournmnt_state.match_pair_count++;
				#ifndef SUPPRESS_TOURNAMENT_PROMPTS
				cout << "Match Pair #" << tournmnt_state.match_pair_count << endl;
				#endif
				tournmnt_state.turn_count += SimulatePairMatchBetweenDecks(ai_level, tournmnt_state.card_list, tournmnt_state.active_decks[i], tournmnt_state.active_decks[j], tournmnt_state.card_stats, tournmnt_state.active_deck_stats[i], tournmnt_state.active_deck_stats[j],
					tournmnt_state.card_stats_in_active_decks[i], tournmnt_state.card_stats_in_active_decks[j], tournmnt_state.snd_player_stat, rec, deck_size);
			}
			rec.UpdateStats();
			tournmnt_state.rec_list.push_back(rec);
		}
	#ifndef SUPPRESS_TOURNAMENT_PROMPTS
	cout << endl;
	#endif
	UpdateStatEvals(tournmnt_state.card_stats);
	UpdateStatEvals(tournmnt_state.active_deck_stats);

	// add the final active decks to the list of all decks (previous ones were added when they were rejected or replaced)
	for (int i = 0; i < deck_pool_size; i++)
	{
		tournmnt_state.all_decks.push_back(tournmnt_state.active_decks[i]);
		tournmnt_state.all_deck_stats.push_back(tournmnt_state.active_deck_stats[i]);
		tournmnt_state.card_stats_in_all_decks.push_back(tournmnt_state.card_stats_in_active_decks[i]);
	}

	tournmnt_state.snd_player_stat.UpdateEval();
	double draw_rate = tournmnt_state.snd_player_stat.draw_contribution / tournmnt_state.snd_player_stat.total_participation;
	cout << "Second player effective win rate (current tournament): " << tournmnt_state.snd_player_stat.eval << endl;
	cout << "Draw rate (current tournament: " << draw_rate << endl;
	cout << "Average match length in turn number (each side; current tournament): " << 0.25 * (double)tournmnt_state.turn_count / (double)tournmnt_state.match_pair_count << endl;
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
			<< card_stats[i].effective_win_contribution << " " << card_stats[i].draw_contribution << " " << card_stats[i].snd_player_eff_win_contrib << " " << card_stats[i].total_participation << " "
			<< card_stats[i].card_cost << " " << card_stats[i].sum_weighted_num_turns << " " << card_stats[i].cost_constrained_participation << " " << card_stats[i].adjusted_participation << endl;
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
		// note, the adjusted participation related info are not displayed in the human readable version, as sometimes they are not recorded or computed
		fs << GetCardDetail(cards[i]) << endl << endl;
		fs << "Eval (win rate): " << card_stats[i].eval << endl;
		fs << "Num wins: " << card_stats[i].num_wins << endl;
		fs << "Num losses: " << card_stats[i].num_losses << endl;
		fs << "Total num: " << card_stats[i].total_num << endl;
		fs << "Effective win contribution: " << card_stats[i].effective_win_contribution << endl;
		fs << "Draw contribution: " << card_stats[i].draw_contribution << endl;
		fs << "Second player effective win contribution: " << card_stats[i].snd_player_eff_win_contrib << endl;
		fs << "Total participation: " << card_stats[i].total_participation << endl << endl << endl;
	}
	fs.close();
	fs.clear();
}

void WriteDeckData(const vector<vector<int>>& deck_list, const vector<BalanceStat>& deck_stats, const char* filename) // we still output the "rank" but here it might not mean anything (may just be an index)
{
	int deck_num = deck_list.size();
	int deck_size = deck_list[0].size();

	ofstream fs(filename);
	fs << deck_num << endl;
	fs << deck_size << endl;
	for (int i = 0; i < deck_num; i++)
	{
		// index in the card seed list
		for (int j = 0; j < deck_size; j++)
			fs << deck_list[i][j] << " ";
		fs << endl;
		// other info
		fs << i + 1 << " " << deck_stats[i].eval << " "
			<< deck_stats[i].num_wins << " " << deck_stats[i].num_losses << " " << deck_stats[i].total_num << " " << endl << endl;
	}
	fs.close();
	fs.clear();
}

void WriteDeckDataSorted(const vector<vector<int>>& deck_list, const vector<BalanceStat>& deck_stats, const vector<int>& sorted_deck_indices, int start_rank, int rank_step, int num_outputs, const char* filename) // Note: the rank passed in to the function should be zero-indexed (while in the file output it is one-indexed
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
			>> card_stats[i].effective_win_contribution >> card_stats[i].draw_contribution >> card_stats[i].snd_player_eff_win_contrib >> card_stats[i].total_participation
			>> card_stats[i].card_cost >> card_stats[i].sum_weighted_num_turns >> card_stats[i].cost_constrained_participation >> card_stats[i].adjusted_participation;
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
		int index;
		fs >> index // not recorded (may or may not mean rank, depending on whether the list is sorted)
			>> deck_stats[i].eval
			>> deck_stats[i].num_wins >> deck_stats[i].num_losses >> deck_stats[i].total_num;
		deck_stats[i].effective_win_contribution = 0.5 * (double)(deck_stats[i].total_num + deck_stats[i].num_wins - deck_stats[i].num_losses); // win + 0.5 * draw
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

void WriteSingleMatchRecord(const MatchRec& rec, int deck_size, ostream& s)
{
	for (int j = 0; j < deck_size; j++)
		s << rec.deck_a_indices[j] << " ";
	s << endl;
	for (int j = 0; j < deck_size; j++)
		s << rec.deck_b_indices[j] << " ";
	s << endl;
	s << rec.win_weight << endl;
	s << rec.weight << endl;
	s << rec.win_rate << endl;
	for (int j = 0; j < deck_size; j++)
		s << rec.sum_win_contrib_a[j] << " ";
	s << endl;
	for (int j = 0; j < deck_size; j++)
		s << rec.ave_win_contrib_a[j] << " ";
	s << endl;
	for (int j = 0; j < deck_size; j++)
		s << rec.sum_total_partic_a[j] << " ";
	s << endl;
	for (int j = 0; j < deck_size; j++)
		s << rec.ave_total_partic_a[j] << " ";
	s << endl;
	for (int j = 0; j < deck_size; j++)
		s << rec.sum_win_contrib_b[j] << " ";
	s << endl;
	for (int j = 0; j < deck_size; j++)
		s << rec.ave_win_contrib_b[j] << " ";
	s << endl;
	for (int j = 0; j < deck_size; j++)
		s << rec.sum_total_partic_b[j] << " ";
	s << endl;
	for (int j = 0; j < deck_size; j++)
		s << rec.ave_total_partic_b[j] << " ";
	s << endl;
}

void WriteMatchRecords(const vector<MatchRec>& recs, int deck_size, const char* filename)
{
	int n_recs = recs.size();

	ofstream fs(filename);
	fs << n_recs << endl << endl;
	for (int i = 0; i < n_recs; i++)
		WriteSingleMatchRecord(recs[i], deck_size, fs);
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

string GetAppendedFilePath(const string& path, const string& suffix)
{
	string result_path;
	size_t suffix_pos = path.rfind(".");
	if (suffix_pos == string::npos)
	{	
		result_path = path + suffix;
	}
	else
	{
		result_path = path;
		result_path.insert(suffix_pos, suffix);
	}
	return result_path;
}

template <typename T>
void SplitTrainValidate(const vector<T>& data, vector<T>& train_data, vector<T>& validate_data, double train_ratio)
{
	int total_size = data.size();
	int train_size = (int)(train_ratio * total_size);

	// do a random shuffle
	vector<int> index_map = CreateRandomSelection(total_size, total_size);

	train_data.resize(train_size);
	int i;
	for (i = 0; i < train_size; i++)
		train_data[i] = data[index_map[i]];
	validate_data.resize(total_size - train_size);
	for (int j = 0; i < total_size; j++, i++)
		validate_data[j] = data[index_map[i]];
}

double Cum_Mana_Curve[11] = { 0.0152, 0.1508, 0.3361, 0.5386, 0.6803, 0.7989, 0.8724, 0.9255, 0.9624, 0.9836, 100.0}; // the last number is just a sentinel for index bound, as long as it is >= 1.0 it should be fine

vector<Card*> GenerateInitCardSet(int n, int seed)  // generate a card set consisting of cards generated by the initial generator, according to the specified mana curve
{
	std::vector<Card*> cards(n);

	// create the list of costs 
	vector<int> costs(n);
	int curr_cost = 0;
	for (int i = 0; i < n; i++)
	{
		while (round(Cum_Mana_Curve[curr_cost] * n) <= i) // we want to maintain i < round(Cum_Mana_Curve[curr_cost] * n)
			curr_cost++;
		costs[i] = curr_cost;
	}
	// for better comparsion between different versions, set all the random seeds here (even though not every seed here is necessarily relevant here), and shuffle the costs
	srand(seed);
	GiglRandInit(seed);
	random_shuffle(costs.begin(), costs.end());

	// for better comparsion between different versions, set all the random seeds here (even though not every seed here is necessarily relevant here)
	srand(seed);
	GiglRandInit(seed);
	for (int i = 0; i < n; i++)
		cards[i] = GenerateSingleCardWithCost(costs[i], GetGiglRandInt());

	return cards;
}

void RunSimulationNode(SimulationNode& node, int deck_pool_size, int deck_size,
	int& n_total_visits, int& num_total_match_pairs, int& num_total_turns, int& num_total_decks, 
	time_t start_time) // note, this assumes the tournament state in the node is in a state that is ready to run (already reset)
{
	
	EvolutionSimulation(node.tournmnt_state, deck_pool_size, deck_size);
	n_total_visits++;
	num_total_match_pairs += node.tournmnt_state.match_pair_count;
	num_total_turns += node.tournmnt_state.turn_count;
	num_total_decks += node.tournmnt_state.all_decks.size();
	
	// accumulate stats on the node
	node.AppendTournamentStats(); // this also update the balance related metrics on the node
	cout << "Balance Score: " << node.balance_score << "; Symmetry Score: " << node.symmetry_score << "; Strength Score: " << node.strength_score << "; Coverage Score: " << node.coverage_score << "; Diversity Score: " << node.diversity_score << "; Interaction Score: " << node.interaction_score << endl;
	time_t curr_time = time(NULL);
	double elapsed_time = difftime(curr_time, start_time);
	cout << "Timer: " << elapsed_time << endl;

	// deallocate unnecessary data to save some space
	node.ResetTournament();
}

void PrintNodeLog(const SimulationNode* node, ofstream& fs) // note, no endl at the end
{
	fs << node->n_visits << "  "
		<< node->balance_score << "  "
		<< node->symmetry_score << "  "
		<< node->strength_score << "  "
		<< node->coverage_score << "  "
		<< node->diversity_score << "  "
		<< node->interaction_score << "  ";
}

void PrintCardSetHtml(vector<Card*>& card_list, const char* filename)
{	
	int n_cards = card_list.size();
	int n_cards_per_row = 4;

	ofstream fs(filename);

	fs << "<!DOCTYPE html>" << endl;
	fs << "<html>" << endl;
	fs << "<head>" << endl;
	fs << "<link rel=\"stylesheet\" href=\"styles.css\">" << endl;
	fs << "</head>" << endl;
	fs << "<body>" << endl;
	fs << "<table class = \"grid\">" << endl;
	for (int i = 0; i < n_cards; )
	{
		fs << "<tr>" << endl;
		for (int j = 0; i < n_cards && j < n_cards_per_row; i++, j++)
		{
			fs << "<td>" << endl;
			fs << SingleCardHtmlTableScript(card_list[i]);
			fs << "</td>" << endl;
		}
		fs << "</tr>" << endl;
	}
	fs << "</table>" << endl;
	fs << "</body>" << endl;
	fs << "</html>" << endl;

	fs.close();
	fs.clear();
}

vector<Card*> PickCardSet(vector<CardRep>& card_reps, vector<BalanceStat>& card_stats, string& output_card_data_path, string& output_card_data_path_human)
{
	cout << "Pick Card Set: " << endl;
	cout << "0: Set H in the thesis." << endl;
	cout << "1 : Set S1 in the thesis." << endl;
	cout << "2 : Set S2 in the thesis." << endl;
	cout << "3 : Set S2* in the thesis." << endl;
	cout << "4 : Set S3 in the thesis." << endl;
	cout << "5 : Set S4 in the thesis." << endl;
	cout << "6 : Set S5 in the thesis." << endl;
	cout << "7 : Set S6 in the thesis." << endl;
	cout << "8: An optimized set starting from H in the thesis." << endl;
	cout << "9: Read from specified file path." << endl;
	cout << "10: Generate with specific seed." << endl;
	cout << "Any other integer: Randomly generate freely." << endl;

	int choice;
	vector<Card*> card_list;
	cin >> choice;

	const int p = 100;

	switch (choice)
	{
	case 0:
		{
			card_list = CreateDemoCardSetH();
			GetCardsReps(card_list, card_reps);
			card_stats.resize(p);
			output_card_data_path = "demo_card_set_h.txt";
			output_card_data_path_human = "demo_card_set_h_human.txt";
		}
		break;
	case 1:
		{
			card_list = CreateDemoCardSet1();
			GetCardsReps(card_list, card_reps);
			card_stats.resize(p);
			output_card_data_path = "demo_card_set_1.txt";
			output_card_data_path_human = "demo_card_set_1_human.txt";
		}
		break;
	case 2:
		{
			card_list = CreateDemoCardSet2();
			GetCardsReps(card_list, card_reps);
			card_stats.resize(p);
			output_card_data_path = "demo_card_set_2.txt";
			output_card_data_path_human = "demo_card_set_2_human.txt";
		}
		break;
	case 3:
		{
			card_list = CreateDemoCardSet2Alt();
			GetCardsReps(card_list, card_reps);
			card_stats.resize(p);
			output_card_data_path = "demo_card_set_2_star.txt";
			output_card_data_path_human = "demo_card_set_2_star_human.txt";
		}
		break;
	case 4:
		{
			card_list = CreateDemoCardSet3();
			GetCardsReps(card_list, card_reps);
			card_stats.resize(p);
			output_card_data_path = "demo_card_set_3.txt";
			output_card_data_path_human = "demo_card_set_3_human.txt";
		}
		break;
	case 5:
		{
			card_list = CreateDemoCardSet4();
			GetCardsReps(card_list, card_reps);
			card_stats.resize(p);
			output_card_data_path = "demo_card_set_4.txt";
			output_card_data_path_human = "demo_card_set_4_human.txt";
		}
		break;
	case 6:
		{
			card_list = CreateDemoCardSet5();
			GetCardsReps(card_list, card_reps);
			card_stats.resize(p);
			output_card_data_path = "demo_card_set_5.txt";
			output_card_data_path_human = "demo_card_set_5_human.txt";
		}
		break;
	case 7:
		{
			card_list = CreateDemoCardSet6();
			GetCardsReps(card_list, card_reps);
			card_stats.resize(p);
			output_card_data_path = "demo_card_set_6.txt";
			output_card_data_path_human = "demo_card_set_6_human.txt";
		}
		break;
	case 8:
		{
			string Card_Path = "demo_card_set_h_optimized.txt";
			ReadCardData(card_list, card_reps, card_stats, Card_Path.c_str());
			output_card_data_path = "demo_card_set_h_optimized.txt";
			output_card_data_path_human = "demo_card_set_h_optimized_human.txt";
		}
		break;
	case 9:
		{
			cout << "Input card path: " << endl;
			string input_card_path;
			cin >> input_card_path;
			ReadCardData(card_list, card_reps, card_stats, input_card_path.c_str());
			output_card_data_path = input_card_path;
			output_card_data_path_human = GetAppendedFilePath(output_card_data_path, "_human");
		}
		break;
	case 10:
		{
			cout << "Input seed: " << endl;
			int seed;
			cin >> seed;
			card_list = GenerateCardSet(p, seed);
			GetCardsReps(card_list, card_reps);
			card_stats.resize(p);
			output_card_data_path = string("demo_card_set_seed_") + to_string(seed) + string(".txt");
			output_card_data_path_human = string("demo_card_set_seed_") + to_string(seed) + string("_human.txt");
		}
		break;
	default:
		{
			card_list = GenerateCardSet(p, time(NULL));
			GetCardsReps(card_list, card_reps);
			card_stats.resize(p);
			output_card_data_path = "demo_card_set_random.txt";
			output_card_data_path_human = "demo_card_set_random_human.txt";
		}
		break;
	}

	cout << endl << "Browsing the card set? (y/n)" << endl;
	char ch;
	cin >> ch;
	if (ch == 'y' || ch == 'Y')
		BrowseCardSet(card_list);

	return card_list;
}

int main(int argc, char* argv[]) // argument order, if supplied: mode, seed, file paths (with different semantics among modes)
{
	int seed = 0;
	char ch = 'y';

	int n = Deck_Size; // # cards in a deck

	cout << "Select Mode: " << endl;
	cout << "0 (Default): Generate random cards freely." << endl;
	cout << "1 : Generate random cards with specified seeds." << endl;
	cout << "2 : Play against AI using decks generated freely or with specified seeds (card pool unknown; directly generating cards for decks); random play order." << endl;
	cout << "3 : Play against yourself with decks generated freely or with specified seeds (card pool unknown; directly generating cards for decks); fixed play order." << endl;
	cout << "4 : Simulate AI Vs AI matches between (different) AI's on the same set of random decks based on a card pool (Warning: this may take hours to days)." << endl;
	cout << "5 : Simulate AI Vs AI matches (with same AI) for evaluating cards (decks) on the same card pool with both random and evolutionary deck building/matching scheme (Warning: this may take hours to days)." << endl;
	cout << "6 : Simulate AI Vs AI matches (with same AI) between different two sets of given decks (Warning: this may take hours to days)." << endl;
	cout << "7 : Play against AI using decks in either the meta environment (generated by the evolutionary simulation) or the random environment (generated by the random simulation) for the optimized set in the thesis." << endl;
	cout << "8 : Test a card pool in terms of balance (Warning: this may take hours to days)." << endl;
	cout << "9 : Produce an html file for a given card pool." << endl;

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
	case 9:
		{
			string output_card_data_path;
			string output_card_data_path_human;

			vector<CardRep> card_reps;
			vector<BalanceStat> card_stats;
			vector<Card*> card_list = PickCardSet(card_reps, card_stats, output_card_data_path, output_card_data_path_human);

			size_t post_fix_pos = output_card_data_path_human.rfind('.');
			string card_html_path;
			if (post_fix_pos == string::npos)
			{
				card_html_path = output_card_data_path_human + ".html";
			}
			else
			{
				card_html_path = output_card_data_path_human.substr(0, post_fix_pos) + ".html";
			}

			cout << endl << "Card raw data will be written (overwrite) to " << output_card_data_path << " (can be used as input for other purposes)." << endl;
			cout << "A slightly more human readable (but less machine readable) text version will be written (overwrite) to " << output_card_data_path_human << "." << endl;
			cout << "The html file will be written (overwrite) to " << card_html_path << "." << endl;
			cout << "Note1: The html should be viewed when the provided style.css is in the folder with it." << endl;
			cout << "Note2: Please set the min font size of your browser (or whatever html renderer) to no bigger than 8 when viewing the html, in case certain small font sizes are not supported." << endl;
			cout << "Input any character to continue: " << endl;
			cin >> ch;

			// print card data files
			WriteCardData(card_list, card_reps, card_stats, output_card_data_path.c_str());
			WriteCardDataHuman(card_list, card_reps, card_stats, output_card_data_path_human.c_str());

			// print html
			PrintCardSetHtml(card_list, card_html_path.c_str());

			DeleteCards(card_list);	
		}
		break;
	case 8:
		{
			string output_card_data_path;
			string output_card_data_path_human;

			vector<CardRep> card_reps;  // will be ignored (will use ones in the simulation node to manage)
			vector<BalanceStat> card_stats;  // will be ignored (will use ones in the simulation node to manage)
			vector<Card*> card_list = PickCardSet(card_reps, card_stats, output_card_data_path, output_card_data_path_human);

			cout << endl << "Card raw data will be written (overwrite) to " << output_card_data_path << " (can be used as input for other purposes)." << endl;
			cout << "A slightly more human readable (but less machine readable) text version will be written (overwrite) to " << output_card_data_path_human << "." << endl;
			cout << "Input any character to continue: " << endl;
			cin >> ch;

			if (argc > 2)
			{
				seed = atoi(argv[2]);
			}
			else
			{
				cout << "Input seed" << endl;
				cin >> seed;
			}
			cout << "Seed for simulation (not for card set generation): " << seed << endl;
			srand(seed);
			GiglRandInit(seed);
			
			int num_repeats = 10;
			int deck_pool_size = 10; // number of active decks (will evolve during the simulation)
			int n_total_visits = 0;
			int num_total_match_pairs = 0, num_total_turns = 0, num_total_decks = 0;
			time_t start_time = time(NULL);

			cout << "Simulation starts, balance stats are cumulatively considered over rounds of simulation." << endl;
			SimulationNode node(card_list);
			for (int k = 0; k < num_repeats; k++)
				RunSimulationNode(node, deck_pool_size, n, n_total_visits, num_total_match_pairs, num_total_turns, num_total_decks, start_time);

			WriteCardData(node.tournmnt_state.card_list, node.tournmnt_state.card_reps, node.card_stats, output_card_data_path.c_str());
			WriteCardDataHuman(node.tournmnt_state.card_list, node.tournmnt_state.card_reps, node.card_stats, output_card_data_path_human.c_str());
		}
		break;
	case 7:
		{
			string Card_Path = "demo_card_set_h_optimized.txt";
			string Deck_Paths[2] = {"demo_card_set_h_optimized_deck_data_active_evolved.txt", "demo_card_set_h_optimized_deck_data_top_random.txt"};

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
				int ai_deck_set = 1;
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

				cout << "Input AI level (integer, clamped to 0 ~ 2; 2 is recommended)" << endl;
				int ai_level;
				while (!(cin >> ai_level)) { cin.clear(); cin.ignore(); }
				cout << endl;

				srand(time(NULL));
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
				Player ai_player("AI", 30, ai_deck, true, event_queue, ai_level, card_list);

				InteractiveMatch(human_player, ai_player);

				while (!event_queue.empty())
				{
					delete event_queue.front();
					event_queue.pop();
				}

				cout << "Repeat? (y/n)" << endl;
				cin >> ch;
				if (ch != 'Y' && ch != 'y')
					break;
			}

			DeleteCards(card_list);
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
			cout << "Seed for simulation (not for card set generation): " << seed << endl << endl;

			cout << "Note that the default two deck lists are the active decks from the evolution simulation and the top decks from the random simulation for the given card pool." << endl;
			cout << "If requiring custom deck set, please use commandline arguments, with format:" << endl;
			cout << "<exe> <mode> <simulation seed> <card pool> <deck list 1> <deck list 2> <output file>" << endl;

			string Card_Path;
			vector<Card*> card_list;
			vector<CardRep> card_reps;
			vector<BalanceStat> card_stats; // it is questionable whether we should clear this before simulation, but it doesn't affect the process and is not output in this mode
			if (arc > 3)
			{
				Card_Path = argv[3];
				cout << "Using card pool: " << Card_Path << "." << endl;
				ReadCardData(card_list, card_reps, card_stats, Card_Path.c_str());
			}
			else
			{	
				string card_data_path_human; // this variable is not used in this mode
				card_list = PickCardSet(card_reps, card_stats, Card_Path, card_data_path_human);
			}
			
			string Deck_A_Path = GetAppendedFilePath(Card_Path, "_deck_data_active_evolved");
			if (argc > 4)
			{
				Deck_A_Path = argv[4];
				cout << "Using custom deck set for the first deck list: " << Deck_A_Path << "." << endl;
			}
			else
			{
				cout << "Using default deck set for the first deck list: " << Deck_A_Path << "." << endl;
				
			}

			string Deck_B_Path = GetAppendedFilePath(Card_Path, "_deck_data_top_random");
			if (argc > 5)
			{
				Deck_B_Path = argv[5];
				cout << "Using custom deck set for the second deck list: " << Deck_B_Path << "." << endl;
			}
			else
			{
				cout << "Using default deck set for the second deck list: " << Deck_B_Path << "." << endl;
			}

			string Deck_Post_Simulation_Path = GetAppendedFilePath(Card_Path, "deck_match_post_simulation.txt");
			if (argc > 6)
				Deck_Post_Simulation_Path = argv[6];

			cout << "Input any character to continue: " << endl;
			cin >> ch;

			// read deck data
			vector<vector<int>> deck_list_a, deck_list_b;
			vector<BalanceStat> deck_stats_a, deck_stats_b; // not actually used here, the original stats from when the deck where initially created and tested
			ReadDeckData(deck_list_a, deck_stats_a, Deck_A_Path.c_str());
			ReadDeckData(deck_list_b, deck_stats_b, Deck_B_Path.c_str());
			int deck_num_a = deck_list_a.size();
			int deck_num_b = deck_list_b.size();
			int deck_size = deck_list_a[0].size();

			int ai_level = 2;

			srand(seed);
			GiglRandInit(seed);

			time_t timer_0 = time(NULL);

			int turn_count = 0;

			ofstream fs(Deck_Post_Simulation_Path.c_str());
			fs << deck_num_a << " " << deck_num_b << endl;
			BalanceStat snd_player_stat;
			for (int i = 0; i < deck_num_a; i++)
			{
				for (int j = 0; j < deck_num_b; j++)
				{
					BalanceStat tmp_stat_a, tmp_stat_b; // only tmp_stat_a is recorded to output (well they should be symmetric)
					vector<BalanceStat> tmp_card_stats_in_deck_a(deck_size), tmp_card_stats_in_deck_b(deck_size); // not recorded
					cout << "Deck " << i << " from the first list VS Deck " << j << " from the second list." << endl;
					for (int k = 0; k < num_pair_matches; k++)
					{
						cout << "Match Pair " << k << ":" << endl;
						MatchRec rec(deck_list_a[i], deck_list_b[j]); // not actually used
						turn_count += SimulatePairMatchBetweenDecks(ai_level, card_list, deck_list_a[i], deck_list_b[j], card_stats, tmp_stat_a, tmp_stat_b,
							tmp_card_stats_in_deck_a, tmp_card_stats_in_deck_b, snd_player_stat, rec, deck_size);
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
			snd_player_stat.UpdateEval(); // not actually reported but just in case
			
			time_t timer_1 = time(NULL);
			
			/* performance report */
			cout << endl;
			cout << "Total number of decks tested: " << deck_num_a + deck_num_b << endl;
			cout << "Total number of match pairs tested: " << deck_num_a * deck_num_b * num_pair_matches << endl; 
			cout << "Total number of match turns (both sides added) simulated: " << turn_count << endl;
			double total_time = difftime(timer_1, timer_0);
			cout << "Total testing time (including writing results to the file): " << total_time << endl;

			DeleteCards(card_list);
		}
		break;
	case 5:
		{
			int p = 100; // size of the card pool

			string output_card_data_path;
			string output_card_data_path_human; // this variable is not used in this mode

			vector<CardRep> card_reps;
			vector<BalanceStat> card_stats; // this one is not actually used
			vector<Card*> card_list = PickCardSet(card_reps, card_stats, output_card_data_path, output_card_data_path_human);

			string Match_Card_Data_Path = GetAppendedFilePath(output_card_data_path, "_card_data");
			string Match_Deck_Data_Path = GetAppendedFilePath(output_card_data_path, "_deck_data");
			string Match_Match_Data_Path = GetAppendedFilePath(output_card_data_path, "_match_data");

			cout << "This mode will produce various output data files, for cards, decks, matches etc., for different matching schemes" << endl;
			cout << "Input any character to continue: " << endl;
			cin >> ch;

			if (argc > 2)
			{
				seed = atoi(argv[2]);
			}
			else
			{
				cout << "Input seed" << endl;
				cin >> seed;
			}
			cout << "Seed for simulation (not for card set generation): " << seed << endl;
			srand(seed);
			GiglRandInit(seed);

			// random environment benchmark related parameters
			int deck_num_random = 1000; // total number of decks
			int match_pair_num_random = 10000; // total number of pair matches
			int turn_count_random = 0; // number of match turns, not known before hand
			double total_time_random;

			// meta environment benchmark related parameters
			int deck_pool_size = 10; // number of active decks (will evolve during the simulation)
			int deck_count_evolved; // this is not known before hand
			int match_pair_count_evolved; // this is not known before hand
			int turn_count_evolved; // number of match turns, not known before hand
			double total_time_evolved;

			int display_num = 5; // the number of top and bottom cards to be displayed
			vector<int> top_card_indices_random, bottom_card_indices_random;
			vector<int> top_card_indices_evolved, bottom_card_indices_evolved;

			/* random environment simulation */
			{
				string Match_Card_Data_Path_Random = GetAppendedFilePath(Match_Card_Data_Path, "_random");
				string Match_Card_Data_Path_Human_Random = GetAppendedFilePath(Match_Card_Data_Path, "_human_random");
				string Match_Deck_Data_Path_All_Random = GetAppendedFilePath(Match_Deck_Data_Path, "_all_random");
				string Match_Deck_Data_Path_Top_Random = GetAppendedFilePath(Match_Deck_Data_Path, "_top_random");
				string Match_Deck_Data_Path_Skip_Random = GetAppendedFilePath(Match_Deck_Data_Path, "_skip_random");
				string Match_Match_Data_Path_Random = GetAppendedFilePath(Match_Match_Data_Path, "_random");

				int ai_level = 2;

				vector<BalanceStat> card_stats(p);
				vector<BalanceStat> deck_stats(deck_num_random);
				BalanceStat snd_player_stat;

				time_t timer_0 = time(NULL);

				vector<vector<int>> deck_list; // storing card indices in the seed list (not the seeds themselves)
				for (int i = 0; i < deck_num_random; i++)
					deck_list.push_back(CreateRandomSelection(p, n));

				vector<MatchRec> rec_list;
				for (int i = 0; i < match_pair_num_random; i++)
				{
					int index_a = GetGiglRandInt(deck_num_random);
					int index_b = GetGiglRandInt(deck_num_random);
					cout << "Match Pair " << i << ": Deck " << index_a << " VS Deck " << index_b << ". " << endl;
					vector<BalanceStat> tmp_card_stats_in_deck_a(n), tmp_card_stats_in_deck_b(n); // not recorded
					MatchRec rec(deck_list[index_a], deck_list[index_b]);
					turn_count_random += SimulatePairMatchBetweenDecks(ai_level, card_list, deck_list[index_a], deck_list[index_b], card_stats, deck_stats[index_a], deck_stats[index_b],
						tmp_card_stats_in_deck_a, tmp_card_stats_in_deck_b, snd_player_stat, rec, n);
					rec.UpdateStats();
					rec_list.push_back(rec);
				}
				UpdateStatEvals(card_stats);
				UpdateStatEvals(deck_stats);
				snd_player_stat.UpdateEval(); // not actually reported but just in case

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
				WriteDeckDataSorted(deck_list, deck_stats, deck_indices, 0, 1, 1000, Match_Deck_Data_Path_All_Random.c_str());
				WriteDeckDataSorted(deck_list, deck_stats, deck_indices, 0, 1, 10, Match_Deck_Data_Path_Top_Random.c_str());
				WriteDeckDataSorted(deck_list, deck_stats, deck_indices, 49, 100, 10, Match_Deck_Data_Path_Skip_Random.c_str());
				// matches
				WriteMatchRecords(rec_list, n, Match_Match_Data_Path_Random.c_str());
			}
			/* evolved environment simulation */
			{
				string Match_Card_Data_Path_Evolved = GetAppendedFilePath(Match_Card_Data_Path, "_evolved");
				string Match_Card_Data_Path_Human_Evolved = GetAppendedFilePath(Match_Card_Data_Path, "_human_evolved");
				string Match_Deck_Data_Path_All_Evolved = GetAppendedFilePath(Match_Deck_Data_Path, "_all_evolved");
				string Match_Deck_Data_Path_Active_Evolved = GetAppendedFilePath(Match_Deck_Data_Path, "_active_evolved");
				string Match_Match_Data_Path_Evolved = GetAppendedFilePath(Match_Match_Data_Path, "_evolved");
				
				time_t timer_0 = time(NULL);
	
				vector<Card*> card_list_copy = HardCopyCards(card_list); // this is just to overcome the artifact that we share the card list over the two simulation method and still use it after the block (as TournamentState will delete the cards it manages in its destructor
				TournamentState tournmnt_state(card_list_copy);
				EvolutionSimulation(tournmnt_state, deck_pool_size, n);

				time_t timer_1 = time(NULL);

				total_time_evolved = difftime(timer_1, timer_0);

				// prepare for displaying
				vector<int> card_indices(p);
				SortStatInIndices(tournmnt_state.card_stats, card_indices);
				for (int i = 0; i < display_num; i++)
				{
					top_card_indices_evolved.push_back(card_indices[i]);
					bottom_card_indices_evolved.push_back(card_indices[p - i - 1]);
				}

				// prepare data for training/post processing/displaying
				// cards
				vector<CardRep> card_reps;
				vector<double> card_strengths;
				vector<double> card_weights;
				PrepareCardData(card_list_copy, tournmnt_state.card_stats, card_reps, card_strengths, card_weights, Match_Card_Data_Path_Evolved.c_str(), Match_Card_Data_Path_Human_Evolved.c_str());
				// decks
				deck_count_evolved = tournmnt_state.all_decks.size();
				vector<int> active_deck_indices(deck_pool_size);
				SortStatInIndices(tournmnt_state.active_deck_stats, active_deck_indices);
				WriteDeckData(tournmnt_state.all_decks, tournmnt_state.all_deck_stats, Match_Deck_Data_Path_All_Evolved.c_str());
				WriteDeckDataSorted(tournmnt_state.active_decks, tournmnt_state.active_deck_stats, active_deck_indices, 0, 1, 10, Match_Deck_Data_Path_Active_Evolved.c_str());
				// matches
				match_pair_count_evolved = tournmnt_state.match_pair_count;
				turn_count_evolved = tournmnt_state.turn_count;
				WriteMatchRecords(tournmnt_state.rec_list, n, Match_Match_Data_Path_Evolved.c_str());
			}

			/* performance report */
			cout << endl;
			cout << "Random game environment simulation summary" << endl;
			cout << "------------------------------------------" << endl;
			cout << "Total number of decks tested: " << deck_num_random << endl;
			cout << "Total number of match pairs tested: " << match_pair_num_random << endl;
			cout << "Total number of match turns (both sides added) simulated: " << turn_count_random << endl;
			cout << "Total testing time: " << total_time_random << endl;
			cout << "------------------------------------------" << endl << endl;
			cout << "Evolution game environment simulation summary" << endl;
			cout << "------------------------------------------" << endl;
			cout << "Total number of decks tested: " << deck_count_evolved << endl;
			cout << "Total number of match pairs tested: " << match_pair_count_evolved << endl;
			cout << "Total number of match turns (both sides added) simulated: " << turn_count_evolved << endl;
			cout << "Total testing time: " << total_time_evolved << endl;
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

			string output_card_data_path;
			string output_card_data_path_human;

			vector<CardRep> card_reps;
			vector<BalanceStat> card_stats;
			vector<Card*> card_list = PickCardSet(card_reps, card_stats, output_card_data_path, output_card_data_path_human);

			cout << endl << "Card raw data will be written (overwrite) to " << output_card_data_path << " (can be used as input for other purposes)." << endl;
			cout << "A slightly more human readable (but less machine readable) text version will be written (overwrite) to " << output_card_data_path_human << "." << endl;
			cout << "Note: however, in this mode, no card stats will be computed during the AI testing matches, i.e., they will be default values or original values in the file." << endl;
			cout << "Input any character to continue: " << endl;
			cin >> ch;

			if (argc > 2)
			{
				seed = atoi(argv[2]);
			}
			else
			{
				cout << "Input seed for deck creation and match simulation (not for card set generation)" << endl;
				cin >> seed;
			}
			cout << "Seed for deck creation and match simulation " << seed << endl;
			srand(seed);
			GiglRandInit(seed);
			
			vector<vector<int>> deck_list; // storing card indices in the seed list (not the seeds themselves)
			for (int i = 0; i < deck_num; i++)
				deck_list.push_back(CreateRandomSelection(p, n));

			// pair-wise matches (duplicate the same pair twice for a different player go first)
			int ai_level_a, ai_level_b;
			cout << "Input AI level for the first AI (integer, clamped to 0 ~ 2)" << endl;
			cin >> ai_level_a;
			cout << "Input AI level for the second AI (integer, clamped to 0 ~ 2)" << endl;
			cin >> ai_level_b;
			TestAIs(ai_level_a, ai_level_b, card_list, deck_list, deck_num, n);

			WriteCardData(card_list, card_reps, card_stats, output_card_data_path.c_str());
			WriteCardDataHuman(card_list, card_reps, card_stats, output_card_data_path_human.c_str());

			DeleteCards(card_list);
		}
		break;
	case 3:
		while (true)
		{
			cout << "Input seed for Deck 1 (negative to randomize); note deck 1 always goes first" << endl;
			while (!(cin >> seed)) { cin.clear(); cin.ignore(); } // the clear and ignore prevents stuck when the input is ill-formatted
			if (seed < 0)
				seed = time(NULL);
			cout << "Deck 1 seed: " << seed << endl; // echo
			vector<Card*> deck1 = GenerateCardSet(n, seed);
			BrowseCardSet(deck1);

			cout << "Input seed for Deck 2 (negative to randomize); note deck 2 always goes second" << endl;
			while (!(cin >> seed)) { cin.clear(); cin.ignore(); } // the clear and ignore prevents stuck when the input is ill-formatted
			if (seed < 0)
			{
				seed = time(NULL) + GetGiglRandInt();
				// the addition may overflow to negative value, but we want seed to be positive
				if (seed < 0)
					seed = abs(seed + 1); // +1 is to avoid the problem of INT_MIN not having a proper absolute value
			}
			vector<Card*> deck2 = GenerateCardSet(n, seed);
			BrowseCardSet(deck2);

			// making the match process always random
			srand(time(NULL));
			GiglRandInit(time(NULL));

			cout << "Shuffle decks for the match? (y/n; default y; cards are drawn in reverse order from the deck)" << endl;
			cin >> ch;
			if (ch != 'N' && ch != 'n')
			{
				random_shuffle(deck1.begin(), deck1.end());
				random_shuffle(deck2.begin(), deck2.end());
			}

			queue<DeferredEvent*> event_queue;
			Player player1("Player1", 30, deck1, false, event_queue);
			Player player2("Player2", 30, deck2, false, event_queue);

			InteractiveMatch(player1, player2, false);

			while (!event_queue.empty())
			{
				delete event_queue.front();
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
			cout << "Input seed for your deck (negative to randomize); note player order is decided randomly " << endl;
			while (!(cin >> seed)) { cin.clear(); cin.ignore(); } // the clear and ignore prevents stuck when the input is ill-formatted
			if (seed < 0)
				seed = time(NULL);
			cout << "Deck 1 seed: " << seed << endl; // echo
			vector<Card*> deck1 = GenerateCardSet(n, seed);
			BrowseCardSet(deck1);

			cout << "Input seed for AI's deck (negative to randomize); note player order is decided randomly" << endl;
			while (!(cin >> seed)) { cin.clear(); cin.ignore(); } // the clear and ignore prevents stuck when the input is ill-formatted
			if (seed < 0)
			{
				seed = time(NULL) + GetGiglRandInt();
				// the addition may overflow to negative value, but we want seed to be positive
				if (seed < 0)
					seed = abs(seed + 1); // +1 is to avoid the problem of INT_MIN not having a proper absolute value
			}
			cout << "Deck 2 seed: " << seed << endl; // echo
			vector<Card*> deck2 = GenerateCardSet(n, seed);

			// making the match process always random
			srand(time(NULL));
			GiglRandInit(time(NULL)); 

			cout << "Shuffle decks for the match? (y/n; default y; cards are drawn in reverse order from the deck)" << endl;
			cin >> ch;
			if (ch != 'N' && ch != 'n')
			{
				random_shuffle(deck1.begin(), deck1.end());
				random_shuffle(deck2.begin(), deck2.end());
			}

			cout << "Input AI level (integer, clamped to 0 ~ 2; 2 is recommended)" << endl;
			int ai_level;
			while (!(cin >> ai_level)) { cin.clear(); cin.ignore(); } // the clear and ignore prevents stuck when the input is ill-formatted

			queue<DeferredEvent*> event_queue;
			Player human_player("Player", 30, deck1, false, event_queue);
			Player ai_player("AI", 30, deck2, true, event_queue, ai_level);

			InteractiveMatch(human_player, ai_player);

			while (!event_queue.empty())
			{
				delete event_queue.front();
				event_queue.pop();
			}

			cout << "Repeat? (y/n)" << endl;
			cin >> ch;
			if (ch != 'Y' && ch != 'y')
				break;
		}
		break;
	case 1:
		while (true)
		{
			cout << "Input seed (negative to quit)" << endl;
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
		break;
	}

	return 0;
}
