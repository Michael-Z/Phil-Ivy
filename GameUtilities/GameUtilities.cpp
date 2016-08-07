#include <vector>
#include <string>
#include <algorithm> 	//for deck shuffling
#include <random>		//for deck shuffling
#include <chrono>		//for deck shuffling seed
#include <iostream>

#include "GameUtilities.h"
#include "../handEval/helper.h"
#include "../Nodes/ChoiceNode.h"
#include "../Nodes/OpponentNode.h"
#include "../Stage.h"
#include "Action.h"
#include "Decision.h"

class Node;
class ChoiceNode;
class OpponentNode;

static double smallBlind = 25.0;
static double bigBlind = 50.0;
static int smallBlindPosition = 0;

void allocateChips(int whoWon, Node &currentNode){
	if (whoWon == 0) { //when bot player wins
		currentNode.game.botPlayer.addChips(currentNode.getGame().getPot());
	} else if (whoWon == 1) { // when opp player wins
		currentNode.game.oppPlayer.addChips(currentNode.getGame().getPot());
	} else { //tie
		currentNode.game.oppPlayer.addChips(currentNode.getGame().getPot()/2);
		currentNode.game.botPlayer.addChips(currentNode.getGame().getPot()/2);
	}
}

std::vector<int> deal(
						std::vector<int>	&previousDeck,
						int					state) {
	unsigned seed =	std::chrono::system_clock::now().time_since_epoch().count();
	std::vector<int> dealtCards;
	std::shuffle(previousDeck.begin(), previousDeck.end(), 
			std::default_random_engine(seed));
	if (state == static_cast<int>(Stage::HOLECARDS) ){
		for (int i = 0; i < 2; ++i){
			dealtCards.push_back(previousDeck.back());
			previousDeck.pop_back();
		}
	} else if (state == static_cast<int>(Stage::PREFLOP) ) {
		for (int i = 0; i < 3; ++i) {
			dealtCards.push_back(previousDeck.back());
			previousDeck.pop_back();
		}
	} else if (state != static_cast<int>(Stage::RIVER) ) {
		dealtCards.push_back(previousDeck.back());
		previousDeck.pop_back();
	}
	return dealtCards;
}

void init_deck(std::vector<int>& deck) {
	deck.reserve(52);
	const char ranks[] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
	const char suits[] = {'s', 'h', 'd', 'c'};
	for (char i: ranks) {
		for (char j: suits) {
			std::string s {i, j};
			deck.push_back(cardToHex(s));
		}
	}
}

void playGame(){
	double initialChips = 1000.0;
	
	Player botPlayer(0, 0, initialChips, 0); //0, 0 represetn dummy cards
	Player oppPlayer(0, 0, initialChips, 0);
	
	while (botPlayer.getChips() && oppPlayer.getChips()){ // while both players have chips
		std::vector<Player> updatePlayers = playRound (botPlayer, oppPlayer);
		botPlayer = updatePlayers[0];
		oppPlayer = updatePlayers[1];
		smallBlindPosition = ~smallBlindPosition;
	}
}

std::vector<Player> playRound(Player botPlayer, Player oppPlayer){
	int smallBlindPosition = 0;
	std::vector<int> deck;
	init_deck(deck);
	botPlayer.setHoleCards(deal(deck, static_cast<int>(Stage::HOLECARDS) ));
	oppPlayer.setHoleCards(deal(deck, static_cast<int>(Stage::HOLECARDS) ));
	int currentStage = 1;
	std::shared_ptr<Node> root;
	if (smallBlindPosition == 0){
		root = std::make_shared<ChoiceNode>(1, bigBlind + smallBlind, std::vector<int>(), \
		botPlayer, oppPlayer, smallBlindPosition, std::shared_ptr<ChoiceNode> (NULL));
		
	
	} else if (smallBlindPosition == 1){
		root = std::make_shared<OpponentNode>(1, bigBlind + smallBlind, std::vector<int>(), \
		botPlayer, oppPlayer, smallBlindPosition, std::shared_ptr<OpponentNode> (NULL));
		
	} else {
		std::cout << "fuck owen, also why is smallblindposition not 1 or 0" << std::endl;
		return std::vector<Player>();
	}
	// currentNode infers type of node from root type
	auto currentNode = root;
	while (currentNode->getIsFolded() == 0) { 
		if (currentNode->getGame().getPlayerTurn() == 0)
			currentNode = playTurn(std::static_pointer_cast<ChoiceNode>(currentNode), deck);
		else
			currentNode = playTurn(std::static_pointer_cast<OpponentNode>(currentNode), deck);
		if (currentStage != currentNode->getGame().getState()){
			std::vector<int> oldBoard = currentNode->getGame().getBoardCards();
			std::vector<int> newCards = deal(deck, currentStage);
			currentStage++; //goes to the next stage in the game
			//adding current board cards to newly dealt cards
			for (auto i = newCards.begin(); i != newCards.end(); ++i){
				oldBoard.push_back(*i);
			}
			currentNode->game.boardCards = oldBoard;
			for (auto j = oldBoard.begin(); j != oldBoard.end(); ++j){
				std::cout << hexToCard(*j) << " ";
			}
			std::cout << std::endl << "current board cards: ";
			for (auto j = currentNode->game.getBoardCards().begin(); j != currentNode->game.getBoardCards().end(); ++j){
				std::cout << *j << " ";
				std::cout << hexToCard(*j) << " ";
			}
			std::cout << std::endl;
			std::cout << "oldboard cards: ";
for (auto j = oldBoard.begin(); j != oldBoard.end(); ++j){
				std::cout << hexToCard(*j) << " ";
			}
			std::cout << std::endl;
		}
	}
	// currentNode.getBotPlayer or something like that
	std::vector<Player> updatePlayers{currentNode->getGame().getBotPlayer(), currentNode->getGame().getOppPlayer() };
	return updatePlayers;
}

std::shared_ptr<OpponentNode> playTurn(std::shared_ptr<ChoiceNode> currentNode, std::vector<int> deck) {
	if (currentNode->getGame().getState() != static_cast<int>(Stage::SHOWDOWN)
			&& !currentNode->getIsAllIn()
			&& !currentNode->getIsFolded()) {
		Decision decision = Decision::makeDecision(currentNode);
		switch(decision.action) {
			case Action::CALL: {
				auto returnNode = currentNode->call();
				return returnNode;
				break; 
			}
			case Action::RAISE:{
				auto returnNode = currentNode->raise(decision.raiseAmount);
				return returnNode;
				break;
			}
			case Action::FOLD:{
				allocateChips(0, (*currentNode));
				auto returnNode = currentNode->fold();
				return returnNode;
				break;
			}
			default:
				std::cout << "Invalid action" << std::endl;
		}
	} else if (currentNode->getIsAllIn()) {
		for (int i = currentNode->getGame().getState(); i < static_cast<int>(Stage::SHOWDOWN); ++i) {
			std::vector<int> oldBoard = currentNode->getGame().getBoardCards();
			std::vector<int> newCards = deal(deck, i);
			//adding current board cards to newly dealt cards
			for (auto j = newCards.begin(); j != newCards.end(); ++j){
				oldBoard.push_back(*j);
			}
			std::cout << std::endl;
			currentNode->game.setBoardCards(oldBoard);
		}
		std::cout << "number of cards: " << currentNode->getGame().getBoardCards().size() << std::endl;
		std::cout << "botCards: " << hexToCard(currentNode->getGame().getBotPlayer().getHoleCards()[0]) << " " << hexToCard(currentNode->getGame().getBotPlayer().getHoleCards()[1]) << std::endl;
		std::cout << "oppCards: " << hexToCard(currentNode->getGame().getOppPlayer().getHoleCards()[0]) << " " << hexToCard(currentNode->getGame().getOppPlayer().getHoleCards()[1]) << std::endl;
		int winner = showdown(currentNode->getGame().getBotPlayer().getHoleCards(),
									  currentNode->getGame().getOppPlayer().getHoleCards(),
									  currentNode->getGame().getBoardCards());
		allocateChips(winner, (*currentNode));
		auto returnNode = currentNode->fold();
		return returnNode;
		
	} else if (currentNode->getGame().getState() == static_cast<int> (Stage::SHOWDOWN)) {
		int winner = showdown(currentNode->getGame().getBotPlayer().getHoleCards(),
									  currentNode->getGame().getOppPlayer().getHoleCards(),
									  currentNode->getGame().getBoardCards());
		allocateChips(winner, (*currentNode));
		auto returnNode = currentNode->fold();
		return returnNode;
			
	} else {
		std::cout << "What the fuck kind of Node is this" << std::endl;
		return std::shared_ptr<OpponentNode>(NULL);
	}
	return std::shared_ptr<OpponentNode>(NULL);
}

std::shared_ptr<ChoiceNode> playTurn(std::shared_ptr<OpponentNode> currentNode, std::vector<int> deck) {
	if (currentNode->getGame().getState() != static_cast<int>(Stage::SHOWDOWN)
			&& !currentNode->getIsAllIn()
			&& !currentNode->getIsFolded()) {
		Decision decision = Decision::makeDecision(currentNode);
		switch(decision.action) {
			case Action::CALL: {
				auto returnNode = currentNode->call();
				return returnNode;
				break; 
			}
			case Action::RAISE:{
				auto returnNode = currentNode->raise(decision.raiseAmount);
				return returnNode;
				break;
			}
			case Action::FOLD:{
				allocateChips(1, (*currentNode));
				auto returnNode = currentNode->fold();
				return returnNode;
				break;
			}
			default:
				std::cout << "Invalid action" << std::endl;
		}
	} else if (currentNode->getIsAllIn()) {
		for (int i = currentNode->getGame().getState(); i < static_cast<int>(Stage::SHOWDOWN); ++i) {
			std::vector<int> oldBoard = currentNode->getGame().getBoardCards();
			std::vector<int> newCards = deal(deck, i);
			//adding current board cards to newly dealt cards
			for (auto j = newCards.begin(); j != newCards.end(); ++j){
				oldBoard.push_back(*j);
			}
			for (auto j = oldBoard.begin(); j != oldBoard.end(); ++j){
				std::cout << hexToCard(*j) << " ";
			}
			std::cout << std::endl;
			currentNode->getGame().setBoardCards(oldBoard);
		}
		int winner = showdown(currentNode->getGame().getBotPlayer().getHoleCards(),
									  currentNode->getGame().getOppPlayer().getHoleCards(),
									  currentNode->getGame().getBoardCards());
									  std::cout << "winner :" << winner << std::endl;
		allocateChips(winner, (*currentNode));
		auto returnNode = currentNode->fold();
		return returnNode;
	} else if (currentNode->getGame().getState() == static_cast<int> (Stage::SHOWDOWN)) {
		int winner = showdown(currentNode->getGame().getBotPlayer().getHoleCards(),
									  currentNode->getGame().getOppPlayer().getHoleCards(),
									  currentNode->getGame().getBoardCards());
		allocateChips(winner, (*currentNode));
		auto returnNode = currentNode->fold();
		return returnNode;
	} else {
		std::cout << "What the fuck kind of Node is this" << std::endl;
		return std::shared_ptr<ChoiceNode>(NULL);
	}
	return std::shared_ptr<ChoiceNode>(NULL);
}

