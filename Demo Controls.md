This file contains the instruction for controls when running the demo. The information is also included on the website https://sites.google.com/a/umn.edu/chaos-cards/downloads-instructions/demo-controls (the website version has more demonstrations).


## Demo Modes

When running the demo, the first thing you are prompted with is selecting the mode to run. Each mode provide a different functionality of the program. Here is the list.

0. Randomly generate and display cards.

1.  Generate cards with specified seeds. The seed to generate a card is used in the name of card, so this mode can help revisiting some cards seen in other modes. Note that as RNG is system/compiler dependent. If a card is generated on a machine and read into the program from another machine (e.g. ones in mode 6 and 7), the number in the card name may not guarantee recovering the same card.

2. Play against AI using decks generated freely or with specified seeds (card pool unknown; directly generating cards for decks).

3. Play against yourself with decks generated freely or with specified seeds (card pool unknown; directly generating cards for decks).

4. Simulate AI Vs AI matches between (different) AI's on the same set of random decks based on a card pool (Warning: this can be long simulation depending on spec).

5. Simulate AI Vs AI matches (with same AI) for evaluating cards (decks) on the same card pool with both random and evolutionary deck building/matching scheme (Warning: this can be long simulation depending on spec). The output data files from one of the runs is provided, so that other modes (6 and 7) may directly use them without re-running this long simulation. Data files containing "human" in the name may make sense to be read. They contains the testing results for cards in the random environment and meta environment respectively.

6. Simulate AI Vs AI matches (with same AI) between different two sets of given decks on the same card pool (Warning: this can be long simulation depending on spec). Both set of decks contains 10 decks, and the decks are matched pairwise and each pair is repeated for multiple times, and the pairwise win rate gets output to a text file like a matrix. The result is expected to be an overwhelming win for the meta decks. 

7. Playing with AI using either the 10 meta decks (from mode 5) or the top 10 random decks (from mode 5) for the optimized set in the thesis. This give a directly feeling on how much better the evolutionary scheme does on evaluating good cards. This is related to one of the claims in the poster paper and the thesis, i.e. game environment/context is very important for evaluating cards (and decks).

8. Test a card pool in terms of balance (Warning: this can be long simulation depending on spec).

9.  Produce an html file for a given card pool. The output contains an html file which can be viewed by browsers.

We recommend trying out mode 0, 2, 7, 9 and they are short and interactive to run. In Mode 7 you would generally expect to feel meta decks to be stronger (may be of similar strengths in rare cases) than top random decks, even given that the the random environment tried much more decks and matches.


## Game Control Commands

For most of the parts, you will only need to follow the prompts and input whatever is requested. If a format error occurs, the program is not likely to crash, but mostly would expect you to input the same information again. For the inputs for actually playing the matches (in mode 2, 3, and 7), there are quite a few actions you might want to choose from, so here we demonstrate a little. Note, the following inputs/commands are only valid inside an actual match (x, y, z below indicate integers, they refers to or is related to the numbering labeled on the board, ordered as your leader -> your minions -> opponents minions -> your hand, see the figures below).
P x y z: play card x from hand at position y and target z, the positioning y is for the minion on the left (0 if the first one), y and/or z are sometimes not used. (Spells and leaders does not use position y, cards without targeted effects does not use z. A leader/minion card with a targeted battlecry but having no valid target is treated as not having targeted effects when playing, while a spell with a targeted cast effect but having not valid target cannot be played. It is important to include all three integers even if some are not used, in which case the value of the unused components does not matter.)

* A x y: use your minion x to attack enemy leader/minion y, the ordering is the same as the target in play card command.

* E: end turn.

* R: resign.

* B: print board.

* Q x: query details on leader/minion/card x.

* O: switch on/off the display of overheat counts, currently off. (This command isn't needed often).

* H: help on commands (echo).
