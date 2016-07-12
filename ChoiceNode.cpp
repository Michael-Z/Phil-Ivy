#include "Node.h"
#include "GameInfo.h"
#include "ChoiceNode.h"

ChoiceNode::ChoiceNode(Node* const parent, GameInfo game):
	parent(parent), game(game)
{
	visitCount = 0;
	isTerminal = false;
}

*Node ChoiceNode::fold() {
	ChoiceNode* choiceFold = new ChoiceNode(this, game);
	choiceFold->isTerminal = true;
	return choiceFold;
}

*Node ChoiceNode::raise(double raiseAmount) {

}

*Node ChoiceNode::check() {
	GameInfo checkGame(game.getState() + 1,
			game.getPot(),
			game.getSmallBlind(),
			game.getBigBlind(),
			game.getBoardCards(),
			game.getPlayerList());
	ChoiceNode* choiceCheck = new ChoiceNode(this, checkGame);
	return choiceCheck;
}