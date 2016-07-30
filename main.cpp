#include "Player.h"
#include "GameObject.h"
#include "Nodes/Node.h"
#include "potInfo.h"
#include "Helper/helper.h"
#include <vector>
#include <iostream>

int main() {
	enum states {HOLECARDS, PREFLOP, FLOP, TURN, RIVER};
	std::vector<int> deck;
	init_deck(deck);
	double bigBlind = 50.0;
	double smallBlind = 25.0;
	int smallBlindPosition;
	std::cout << "small blind position: " << std::endl;
	std::cin >> smallBlindPosition;
	
	Player botPlayer(deal(deck, HOLECARDS), 1000.0, 25.0);
	Player oppPlayer(deal(deck, HOLECARDS), 1000.0, 50.0); //update with pot investment
	
	std::vector<int> boardCards;
	
	Node g(0, 75.0, boardCards, botPlayer, oppPlayer, 0);
	g.call(25.0);
	
}
