#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
using namespace std;


//					FUNCTIONS FOR DECK/HAND BUILDING/PRINTING/MOD
//prints specific cards... ex: King of Hearts etc
void printCards(vector<int>& hand) {
	for (int i = 1; i < 6; i++) {
		cout << i << ". ";
		//switch statements for "King of " "5 of " etc
		switch (hand[i - 1] % 13) {
		case 0:
			cout << "King of ";
			break;
		case 1:
			cout << "Queen of ";
			break;
		case 2:
			cout << "Jack of ";
			break;
		case 3:
			cout << "10 of ";
			break;
		case 4:
			cout << "9 of ";
			break;
		case 5:
			cout << "8 of ";
			break;
		case 6:
			cout << "7 of ";
			break;
		case 7:
			cout << "6 of ";
			break;
		case 8:
			cout << "5 of ";
			break;
		case 9:
			cout << "4 of ";
			break;
		case 10:
			cout << "3 of ";
			break;
		case 11:
			cout << "2 of ";
			break;
		case 12:
			cout << "Ace of ";
			break;
		}
		//switch statements for "Clubs" "Spades" etc
		switch (hand[i - 1] / 13) {
		case 0:
			cout << "Clubs" << endl;
			break;
		case 1:
			cout << "Diamonds" << endl;
			break;
		case 2:
			cout << "Hearts" << endl;
			break;
		case 3:
			cout << "Spades" << endl;
			break;
		}
	}
	cout << "\n\n\n\n\n";
}

void shuff(vector<int>& deck) {
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	shuffle(deck.begin(), deck.end(), default_random_engine(seed));
}

//builds deck
void insertDeck(vector<int> &deck) {
	//inserting values into deck
	for (int i = 0; i < 52; ++i) {
		deck.push_back(i);
	}
	shuff(deck);
	cout << "\n\n\n\nThe Deck has been set!\n\n\n\n\n";
}

//build player hand
void buildHand(vector<int> &deck, vector<int> &hand) {
	if(deck.size() < 5){
		throw  invalid_argument("\n\n\n\nThe deck is near empty! Please set it before trying to draw another hand.\n\n\n\n\n");
	}
	else {
		cout << "\n\n\n\nThe deck is near empty! Please set it before trying to draw another hand.\n\n\n\n\n";
		for (int i = 0; i < 5; i++) {
			hand.push_back(deck[deck.size() - 1]);
			deck.pop_back();
		}

		cout << "\n\n\n\nCards have been successfully drawn!\n";
		printCards(hand);
	}
}





//					FUNCTIONS FOR RATING HAND
//finds size of hand after unique is performed on it. this assists in identifying what kind of hand it is. done in a seperate function to preserve hand contents
int findUniqueSize(vector<int> hand) {
	vector<int>::iterator it;
	it = unique(hand.begin(), hand.end());
	hand.resize(distance(hand.begin(), it));
	for (int i = 0; i < hand.size(); i++) {
		cout << hand[i] << " ";
	}
	cout << "\n\n";

	return hand.size();
}

//returns number of card matches
bool valCheck(vector<int> hand) {

	//changes hand locally to its i%13 - makes it easy to count pairs
	transform(hand.begin(), hand.end(), hand.begin(), bind2nd(modulus<int>(), 13));
	
	//sort mod then find unique
	sort(hand.begin(), hand.end());

	int uSize = findUniqueSize(hand);
	
	//depending on whats left in the unique vector, helps to reveal whats still in the hand
	if (uSize == 5) {
		//no pairs
		cout << "\n\n\n\nThere are no pairs in this hand.\n\n\n\n\n";
		return false;
	}
	else if (uSize == 4) {
		//4 left, 1 dup removed means 1 pair
		cout << "\n\n\n\nThere is 1 pair in this hand.\n\n\n\n\n";
	}
	else if (uSize == 3) {
		//find adjacent to identify first occuring match, then count to determine if 2pair or 3 of a kind.
		vector<int>::iterator it = adjacent_find(hand.begin(), hand.end());
		int occount = count(hand.begin(), hand.end(), *it);
		if (occount == 2){
			cout << "\n\n\n\nThis is a 2 pair!\n\n\n\n\n";
		}
		else {
			cout << "\n\n\n\nThis is a 3 of a kind!\n\n\n\n\n";
		}
	}
	else if (uSize == 2) {
		//find adjacent to identify first occuring match, then count to determine if 4 of a kind or full house
		vector<int>::iterator it = adjacent_find(hand.begin(), hand.end());
		int occount = count(hand.begin(), hand.end(), *it);
		
		if (occount == 4) {
			cout << "\n\n\n\nThis is 4 of a Kind!\n\n\n\n\n";
		}
		else {
			cout << "\n\n\n\nThis is a Full House!\n\n\n\n\n";
		}
	}
	return true;
}

//checks to see if the i/13 match
bool suitCheck(vector<int> hand) {
	//changes hand locally to i/13 - makes it easy to check for flush
	transform(hand.begin(), hand.end(), hand.begin(), bind2nd(divides<int>(), 13));

	//returns false if even one suit is not equal to another
	return find_if(hand.begin() + 1, hand.end(), bind1st(not_equal_to<int>(), hand.front())) == hand.end();
}

bool straightCheck(vector<int> hand) {
	//transforms hand to its i%13 form and then sorts it. easier to evaluate.
	transform(hand.begin(), hand.end(), hand.begin(), bind2nd(modulus<int>(), 13));
	sort(hand.begin(), hand.end());
	printCards(hand);

	//if it's a straight, the result is 4
	int diff = hand[4] - hand[0];
	if (diff == 4) {
		return true;
	}
	else {
		//checks to see if it's a broadway straight - a straight with the ace in a high card position
		if(hand[4] == 12){
			diff = hand[3] - hand[0];

			if (diff == 3) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
}

//identifies combos in hand 
void rateHand(vector<int> &hand) {

	//check matches first. cant have a flush/straight with pairs
	if (valCheck(hand) == true) {

	}
	else {
		//checks suit if match doesn't yield anything
		if (suitCheck(hand) == true) {
			if(straightCheck(hand) == true){
				//suit and straight are both true
				cout << "\n\n\n\nThis hand is a Straight Flush!!\n\n\n\n\n";
			}
			else {
				//suit is true but straight is not
				cout << "\n\n\n\nThis hand is a Flush!!\n\n\n\n\n";
			}
		}
		else {
			if (straightCheck(hand) == true) {
				//straight is true but suit is not
				cout << "\n\n\n\nThis hand is a Straight!!\n\n\n\n\n";
			}
			else {
				//neither straight nor suit is true.
				cout << "\n\n\n\nRely on high card, or swap cards if available.\n\n\n\n\n";
			}
		}
	}
}



//				FUNCTION FOR SWAPPING CARDS
void cardReplace(vector<int>& deck, vector<int>& hand) {
	int swapN;


	cout << "\n\n\n\n\nHow many cards would you like to swap? The max is 2: ";
	cin >> swapN;

	switch (swapN) {
	default:
		cout << "\n\nYou have not entered a correct number. Returning you to main menu. Feel free to try again.\n\n\n";
		break;
	case 0:
		cout << "\n\nkay! Returning to menu.\n\n\n\n\n";
		break;
	case 1:
		int swap1;

		printCards(hand);
		cout << "\n\nWhich card would you like to swap. Please enter the number and only the number next to the displayed card: ";
		cin >> swap1;

		while ((0 > swap1) || (swap1 > 5)) {
			cout << "\n\nYou have not entered a valid number. Try again.\n\n";

			printCards(hand);
			cout << "\n\nWhich card would you like to swap. Please enter the number and only the number next to the displayed card: ";
			cin >> swap1;
		}

		replace(hand.begin(), hand.end(), hand[swap1 - 1], deck[deck.size() - 1]);

		deck.pop_back();

		cout << "\n\n Okay! Here are your new cards:" << endl;
		printCards(hand);

		break;
	case 2:
		int swap2, swap3;
		printCards(hand);
		cout << "\n\nWhich card would you like to swap first. Please enter the number and only the number next to the displayed card: ";
		cin >> swap2;

		cout << "\n\nWhich card would you like to swap second. Please enter the number and only the number next to the displayed card: ";
		cin >> swap3;

		//making sure user entry is correct
		while (((0 > swap2) || (swap2 > 5)) || ((0 > swap3) || (swap3 > 5))) {
			cout << "\n\nYou have not entered a valid number. Try again.\n\n";

			printCards(hand);
			cout << "\n\nWhich card would you like to swap first. Please enter the number and only the number next to the displayed card: ";
			cin >> swap1;

			cout << "\n\nWhich card would you like to swap second. Please enter the number and only the number next to the displayed card: ";
			cin >> swap2;
		}

		//replacing the actual cards
		replace(hand.begin(), hand.end(), hand[swap2 - 1], deck[deck.size() - 1]);
		replace(hand.begin(), hand.end(), hand[swap3 - 1], deck[deck.size() - 2]);

		//removing pulled cards from deck
		deck.pop_back();
		deck.pop_back();

		cout << "\n\n Okay! Here are your new cards:" << endl;
		printCards(hand);

		break;
	}
}




//				FUNCTIONS FOR  MENU
int menuInput() {
	cout << "!!!	POKER PLAYER PALOOZA -- MAIN MENU	!!!\n\n"
		<< "\n1. Set Deck"
		<< "\n2. Draw 5 Cards"
		<< "\n3. Rate Hand"
		<< "\n4. Replace Cards in Hand"
		<< "\n5. Discard Hand"
		<< "\n5. Reset Deck & Hand"
		<< "\n7. ASCII ART CREDIT"
		<< "\n8. Exit"
		<< "\nPlease pick one of the above menu choices. Enter the number and only the number: ";

	int choice;
	cin >> choice;
	while ((choice < 0) || (choice > 8)) {
		cout << "You have not entered a valid menu choice. Please try again: ";
		cin >> choice;
	}
	return choice;
}



int main() {

	cout << R"(  
	!!!!!! WELCOME TO POKER PLAYER PALOOZA !!!!!!
	   ______________________________________
	  /   _______________________________    \
	 |   |            __ __              |   |
	 | _ |    /\     ) _) /''''',        |   |
	 |(.)|   <  >    \ ) // '  `,        |   |
	 | ` |    \/       \/// ~ |~:    +   |   |
	 |   |             ///*\  ' :    |   |   |
	 |   |            ///***\_~.'    |   |   |
	 |   |  .'.    __///````,,..\_   |   |   |
	 |   |   ` \ _/* +_\#\\~\ooo/ \  |   |   |
	 |   |     |/:\ + *\_\#\~\so/!!\ |   |   |
	 |   |    _\_::\ * + \-\#\\o/!!!\|   |   |
	 |   |   / <_=::\ + */_____@_!!!_|   |   |
	 |   |  <__/_____\ */( /\______ _|   |   |
	 |   |   |_   _ __\/_)/* \   ._/  >  |   |
	 |   |   | !!! @     /* + \::=_>_/   |   |
	 |   |   |\!!!/o\\#\_\ + * \::_\     |   |
	 |   |   | \!!/os\~\#_\_* + \:/|     |   |
	 |   |   |  \_/ooo\~\\#_\+_*/- \     |   |
	 |   |   |    \''``,,,,///      .`.  |   |
	 |   |   |     ,.- ***///        '   |   |
	 |   |   |    : ~  \*///             |   |
	 |   |   +    : |   \//\             |   |
	 |   |        ,~  ~ //_( \     /\    | ; |
	 |   |        ,'  ` /_(__(    <  >   |(_)|
	 |   |         `````           \/    |   |
	 |   |_______________________________|   |
	  \______________________________________/)" 
		<<"\n";

	int choice = menuInput();
	int replaceH = 0;
	vector<int> deck;
	vector<int> hand;

	while( choice != 8){
		switch (choice) {
		case 1:
			//set deck
			insertDeck(deck);
			break;
		case 2:
			//draw cards
			if (hand.size() != 0) {
				throw invalid_argument("Discard your hand first before drawing another!thruw");
			}
			else {
				buildHand(deck, hand);
			}
			break;
		case 3:
			//rate hand
			if (hand.size() == 0) {
				throw invalid_argument("You need to draw a hand first.throw");
			}
			else {
				rateHand(hand);
			}

			break;
		case 4:
			//replace hand
			if (hand.size() == 0) {
				throw invalid_argument("Draw a hand first!throw");
			}
			else {
				cardReplace(deck, hand);
			}
			break;
		case 5:
			//discard hand
			hand.clear();
			break;
		case 6:
			//reset deck and hand
			deck.clear();
			hand.clear();
			insertDeck(deck);
			buildHand(deck, hand);
			break;
		case 7:
			//art credit
			cout << "\n\n\n" << R"(
	   ______________________________________
	  /   _______________________________    \
	 |   |            __ __              |   |
	 | _ |    /\     ) _) /''''',        |   |
	 |(.)|   <  >    \ ) // '  `,        |   |
	 | ` |    \/       \/// ~ |~:    +   |   |
	 |   |             ///*\  ' :    |   |   |
	 |   |            ///***\_~.'    |   |   |
	 |   |  .'.    __///````,,..\_   |   |   |
	 |   |   ` \ _/* +_\#\\~\ooo/ \  |   |   |
	 |   |     |/:\ + *\_\#\~\so/!!\ |   |   |
	 |   |    _\_::\ * + \-\#\\o/!!!\|   |   |
	 |   |   / <_=::\ + */_____@_!!!_|   |   |
	 |   |  <__/_____\ */( /\______ _|   |   |
	 |   |   |_   _ __\/_)/* \   ._/  >  |   |
	 |   |   | !!! @     /* + \::=_>_/   |   |
	 |   |   |\!!!/o\\#\_\ + * \::_\     |   |
	 |   |   | \!!/os\~\#_\_* + \:/|     |   |
	 |   |   |  \_/ooo\~\\#_\+_*/- \     |   |
	 |   |   |    \''``,,,,///      .`.  |   |
	 |   |   |     ,.- ***///        '   |   |
	 |   |   |    : ~  \*///             |   |
	 |   |   +    : |   \//\             |   |
	 |   |        ,~  ~ //_( \     /\    | ; |
	 |   |        ,'  ` /_(__(    <  >   |(_)|
	 |   |         `````           \/    |   |
	 |   |_______________________________|   |
	  \______________________________________/)"
				<< endl;
			cout << "Artist Name: Bryan S. So\n"
				<< "Artist email: so@brownie.cs.wisc.edu\n"
				<< "Art found on: https://ascii.co.uk/art/cards \n\n\n\n";
			break;
		case 8:
			//exit
			cout << "\n\n\nYou are now closing the POKER PLAYER PALOOZA game. Bye!";
			exit;
			break;
		}
		choice = menuInput();
	}
};