This file contains the instruction for controls when running the demo. The information is also included on the website https://sites.google.com/a/umn.edu/chaos-cards/downloads-instructions/demo-controls.

Demo Modes

When running the demo, the first thing you are prompted with is selecting the mode to run. Each mode provide a different functionality of the program. Here is the list.

0. Randomly generate and display cards.

1. Generate cards with specified seeds. The seed to generate a card is used in the name of card, so this mode can help revisiting some cards seen in other modes. Note that as RNG is system/compiler dependent. If a card is generated on a machine and read into the program from another machine (e.g. ones in mode 6 and 7), the number in the card name may not guarantee recovering the same card.

2. Playing with AI using randomly generated decks.

3. Playing with yourself using randomly generated decks.

4. Simulating AI vs AI matches on the same set of random decks, used to test relative AI strengths. This takes very long (hours to days depending on the spec).

5. Simulating AI (same AI) matches to evaluate card and decks (starting with a random card pool of 1000 cards), with both a random deck building scheme (random environment) or an evolutionary deck building scheme (meta environment). This is one main part of simulation done for the paper. This takes very long (hours to days depending on the spec). The output data files from one of the runs is provided, so that other modes (6 and 7) may directly use them without re-running this long simulation. Data files containing "human" in the name may make sense to be read. They contains the testing results for cards in the random environment and meta environment respectively.

6. Simulating AI (same AI) matches between the meta decks (produced by the evolutionary simulation in mode 5) and the top 1% random decks (produced by the random simulation in mode 5). Both set of decks contains 30 decks, and the decks are matched pairwise and each pair is repeated for multiple times. The result is expected to be an overwhelming win for the meta decks. This takes very long (hours to days depending on the spec).

7. Playing with AI using either the meta decks (from mode 5) or the top 1% random decks (from mode 5). This give a directly feeling on how much better the evolutionary scheme does on evaluating good cards, as the meta decks isn't a simple concentration of what's good from the random decks. Certain utility centered or strategic cards are just not present in the top random decks, but is involved in the meta decks a lot, especially the top meta decks. This is related to one of the claims in the paper, i.e. game environment/context is very important for evaluating cards (and decks).

We recommend trying out mode 0, 2 and 7, and they are short to run. Mode 7 would be fun in that you can feel what it means to overwhelm your opponent, when you select a meta deck and the AI selects a top random deck (if you played some Hearthstone or other such games alike), or feel what it means to be overwhelmed by your opponent, when you do the opposite (if the ai level is decent, say >= 2).



Game Control Commands

For most of the parts, you will only need to follow the prompts and input whatever is requested. If a format error occurs, the program is not likely to crash, but mostly would expect you to input the same information again. For the inputs for actually playing the matches (in mode 2, 3, and 7), there are quite a few actions you might want to choose from, so here we demonstrate a little. Note, the following inputs/commands are only valid inside an actual match (x, y, z below indicate integers, they refers to or is related to the numbering labeled on the board, ordered as your leader -> your minions -> opponents minions -> your hand, see the figures below).
P x y z: play card x from hand at position y and target z, the positioning y is for the minion on the left (0 if the first one), y and/or z are sometimes not used. (Spells and leaders does not use position y, cards without targeted effects does not use z. A leader/minion card with a targeted battlecry but having no valid target is treated as not having targeted effects when playing, while a spell with a targeted cast effect but having not valid target cannot be played. It is important to include all three integers even if some are not used, in which case the value of the unused components does not matter.)

* A x y: use your minion x to attack enemy leader/minion y, the ordering is the same as the target in play card command.

* E: end turn.

* R: resign.

* B: print board.

* Q x: query details on leader/minion/card x.

* O: switch on/off the display of overheat counts, currently off. (This command isn't needed often).

* H: help on commands (echo).
