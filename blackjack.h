#ifndef __blackjack_h__
#define __blackjack_h__

#include <array>            // std::array
#include <cstdlib>          // std::size_t
#include <iostream>         // std::cout, std::ostream
#include <random>           // std::mt19937_64
#include <vector>           // std::vector

using std::size_t;

/* [ Rank, Suit enums ]
 * This class is to represent a card with its rank and suit
 * and helps in printing and getting a value for each card.
 */
class Card
{
  // enumeration of ranks of a card
  enum class Rank
  {
    Two, Three, Four, Five, Six, Seven,
    Eight, Nine, Ten, Jack, Queen, King,
    Ace,

    TotalRanks
  };

  // enumeration of suits of a card
  enum class Suit
  {
    Club, Diamond, Heart, Spade,

    TotalSuits
  };

  Rank __rank;
  Suit __suit;

  Card() = default;                             // only Deck objects can create Card objects

public:

  Card( const Rank rank, const Suit suit );     // for creating R-value objects in Deck constructor
  const size_t value() const;                   // gives the value of a card's rank

  friend class Deck;                            // for Deck::Deck() to use Rank and Suit enums
  friend std::ostream& operator<<( std::ostream& stream, const Card card );   // to access Card's (private) attributes
};

// overload insertion operator for Card objects
std::ostream& operator<<( std::ostream& stream, const Card card );

/* This class holds a set of 52 cards, that can be shuffled when required.
 * This is the central class for the entire game.
 */
class Deck
{
  std::array<
    Card,
    static_cast<size_t>(Card::Rank::TotalRanks)* static_cast<size_t>(Card::Suit::TotalSuits)
  > __deck;
  size_t __currentcard;

public:

  Deck();                                       // creates a default ordered deck

  void shuffle();                               // shuffles the deck using a mersenne-twister
  void view() const;                            // displays all the cards in the deck (mainly for debugging)
  const Card deal();                            // deals a single card from a shuffled deck
};

/* This class is used to create players who can hold a hand of cards,
 * and participate in the game. Player can also request a card from the deck.
 */
class Player
{
  std::vector<Card> __hand;                     // contains cards that player currently holds 
  bool __11Ace;                                 // indicates whether there is an ace worth 11 points
  size_t __score;                               // overall value of the current hand
  Deck& __game;                                 // link to the Deck to receive cards

public:
  Player( Deck& game );
  void hit();                                   // receive a card from the linked deck
  void view() const;                            // display current hand
  const size_t score() const;                   // outputs __score attribute
};

// enumeration of possible game states
enum class Turn
{
  Player,
  Dealer,
  GameOver
};

void initialSetup( Deck& game, Player& dealer, Player& player );                    // shuffles deck and deals cards
Turn playerTurn( Player& player );                                                  // wait for player's move and make changes
Turn dealerTurn( Player& dealer );                                                  // make dealer's move automatically
const char checkHands( const Player& dealer, const Player& player );                // determine winner from player counts
void showWinner( const Player& dealer, const Player& player, const char winner );   // display end result and scores

#endif
