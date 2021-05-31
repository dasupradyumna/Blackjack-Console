#ifndef __blackjack_h__
#define __blackjack_h__

#include <array>            // std::array
#include <cstddef>          // std::size_t
#include <random>           // std::mt19937_64
#include <vector>           // std::vector

using std::size_t;

class Card
{
public:
    enum class Rank
    {
        Two, Three, Four, Five,
        Six, Seven, Eight, Nine,
        Ten, Jack, Queen, King,
        Ace,

        TotalRanks
    };

    enum class Suit
    {
        Club,
        Diamond,
        Heart,
        Spade,

        TotalSuits
    };

    Rank __rank;
    Suit __suit;

    Card();
    void print() const;

    // friend void printCard(const Card& card);
    // friend int handValue(const std::vector<Card>& hand);
};

class Deck
{
public:

    Deck();
    void shuffle();
    void view();
    Card deal();

    std::array < Card, static_cast<size_t>(Card::Rank::TotalRanks)*
        static_cast<size_t>(Card::Suit::TotalSuits)> __deck;
    std::mt19937_64 __shuffler;
};

void printCard(const Card& card);
int handValue(const std::vector<Card>& hand);

#endif