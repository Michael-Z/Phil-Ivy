#include "Decision.h"
#include "../Nodes/ChoiceNode.h"
#include "../Nodes/OpponentNode.h"
#include "../Action.h"
#include "../Config.h"

#include <memory>
#include <iostream>


//Decision Decision::makeDecision() {
//	std::cout << "Enter Action bot: Call(0), Raise(1), Fold(2)" << std::endl;
//	Decision decision;
//	int temp;
//	std::cin >> temp;
//    std::vector<int> deck;
//    init_deck(deck);
//    decision.action = static_cast<Action>(temp);
//	//decision.action = currentNode->monteCarlo(monteCarloDuration, deck);
//    std::cout << "Bot Decision: " << static_cast<int>(decision.action) << std::endl;
//	if (decision.action == Action::RAISE) {
//		std::cout << "Enter Raise amount" << std::endl;
//		double amount;
//		std::cin >> amount;
//		decision.raiseAmount = amount;
//	}
//	return decision;
//}
//
//Decision Decision::makeDecision(std::shared_ptr<OpponentNode> currentNode) {
//	std::cout << "Enter Action opp: Call(0), Raise(1), Fold(2)" << std::endl;
//	Decision decision;
//	int temp;
//	std::cin >> temp;
//	decision.action = static_cast<Action> (temp);
//	if (decision.action == Action::RAISE) {
//		std::cout << "Enter Raise amount" << std::endl;
//		double amount;
//		std::cin >> amount;
//		decision.raiseAmount = amount;
//	}
//	return decision;
//}
