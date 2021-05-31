#ifndef __blackjack_h__
#define __blackjack_h__

#include <array>            // std::array
#include <cstddef>          // std::size_t
#include <random>           // std::mt19937_64
#include <vector>           // std::vector

using std::size_t;

class Deck
{
    enum class Rank
    {
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        Ten,
        Jack,
        Queen,
        King,
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

public:

    struct Card
    {
        Rank rank;
        Suit suit;
    };

    Deck();
    void shuffle();
    void view();
    Card deal();

    friend void printCard(const Deck::Card card);
    friend int handValue(const std::vector<Card>& hand);

private:

    std::array < Card, static_cast<size_t>(Deck::Rank::TotalRanks)*
        static_cast<size_t>(Deck::Suit::TotalSuits)> __deck;
    std::mt19937_64 __shuffler;
};

void printCard(const Deck::Card card);
int handValue(const std::vector<Deck::Card>& hand);

#endif