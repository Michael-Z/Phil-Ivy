#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

class Player {
	private:
		std::vector<int> holeCards;
		double chips;
		double potInvestment;
		bool isFolded;

	public:

		// Default Constructor, initialzes chips, potInvestment, isFolded
		Player();
		// Creates players with appropriate values
		Player(std::vector<int> hcards, double chip, double pi);
		Player(int c1, int c2, double chip, double pi);

		// Member accessibility fcns

		// Getters
		std::vector<int> getHoleCards() const { return holeCards; }
		double getChips() const { return chips; }
		double getPotInvestment() const { return potInvestment; }
		bool getIsFolded() const { return isFolded; }

		// Setters
		void setHoleCards(std::vector<int> hCards) { holeCards = hCards; }
		void setHoleCards(int c1, int c2);
		void setIsFolded(bool status) { isFolded = status; }
		void setChips(double c) { chips = c; }
		void setPotInvestment(double pi) { potInvestment = pi; }
		// No setter for holeCards bc its const
};

#endif
