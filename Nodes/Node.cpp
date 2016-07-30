#include "Node.h"

Node::Node(int              state,
		double              pot,
		std::vector<int>    boardCards,
		Player				botPlayer,
		Player				oppPlayer,
		int                 playerTurn) :
	GameObject(state,
			pot,
			boardCards,
			botPlayer,
			oppPlayer,
			playerTurn) { }

	// Action function implementation
	std::shared_ptr<Node> Node::fold() {
		//create child foldNode
		std::shared_ptr<Node> foldNode(new Node(*this));
		foldNode->isTerminal = true;
		foldChild = foldNode;
		return foldNode;
	}

std::shared_ptr<Node> Node::call(double callAmount) { //remove call amount later
	//creates a temporary playerlist and updates the player's potinvestment and chip count
	if (getPlayerTurn() == 0){
		Player tempPlayer = getBotPlayer();
		tempPlayer.setPotInvestment(callAmount + tempPlayer.getPotInvestment());
		tempPlayer.setChips(tempPlayer.getChips() - callAmount);
		auto callNode = std::make_shared<Node>( getState() + 1,
			getPot() + getCurrentRaise(),
			getBoardCards(),
			tempPlayer,
			getOppPlayer(),
			getPlayerTurn() + 1);
		callChild = callNode;
	} else {
		Player tempPlayer = getOppPlayer();
		tempPlayer.setPotInvestment(callAmount + tempPlayer.getPotInvestment());
		tempPlayer.setChips(tempPlayer.getChips() - callAmount);
		auto callNode = std::make_shared<Node>( getState() + 1,
			getPot() + getCurrentRaise(),
			getBoardCards(),
			getBotPlayer(),
			tempPlayer,
			getPlayerTurn() + 1);
		callChild = callNode;
	}
	return callChild;
}

std::shared_ptr<Node> Node::raise(double raiseAmount) {
	auto raiseNode = std::make_shared<Node>(state,
			pot + raiseAmount,
			boardCards,
			botPlayer,
			oppPlayer,
			playerTurn + 1);
	raiseChild = raiseNode;
	return raiseNode;
}