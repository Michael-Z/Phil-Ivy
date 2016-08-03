#include "ChoiceNode.h"
#include "OpponentNode.h"

#include <memory>

ChoiceNode::ChoiceNode(
							int							state,
							double						pot,
							std::vector<int>			boardCards,
							Player						botPlayer,
							Player						oppPlayer,
							int							playerTurn,
							std::shared_ptr<ChoiceNode>	const parent) :
	Node(	state,
			pot,
			boardCards,
			botPlayer,
			oppPlayer,
			playerTurn,
			parent) { }

ChoiceNode::ChoiceNode(
							int								state,
							double							pot,
							std::vector<int>				boardCards,
							Player							botPlayer,
							Player							oppPlayer,
							int								playerTurn,
							std::shared_ptr<OpponentNode>	const parent) :
	Node(	state,
			pot,
			boardCards,
			botPlayer,
			oppPlayer,
			playerTurn,
			parent) { }
