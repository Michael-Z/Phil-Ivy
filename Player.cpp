#include "Player.h"

// Default Constructor
Player::Player() :
	chips(0),
	potInvestment(0)
	{ }

Player::Player(std::vector<int> hcards, double chip, double pi) : 
	holeCards(hcards),
	chips(chip),
	potInvestment(pi) { }

Player::Player(int c1, int c2, double chip, double pi) :
	holeCards {c1, c2},
	chips(chip),
	potInvestment(pi) { }

Player& Player::operator=(const Player& rhs) {
  holeCards = rhs.holeCards;
  chips = rhs.chips;
  potInvestment = rhs.potInvestment;

  return *this;
}
