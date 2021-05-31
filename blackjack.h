#ifndef __blackjack_h__
#define __blackjack_h__

#include <array>            // std::array
#include <cstddef>          // std::size_t
#include <iostream>         // std::cout, std::ostream
#include <random>           // std::mt19937_64
#include <vector>           // std::vector

using std::size_t;

class Card
{
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

public:

    Card();
    void print() const;
    const size_t value() const;

    friend class Deck;
    friend std::ostream& operator<<( std::ostream& stream, Rank rank );
    friend std::ostream& operator<<( std::ostream& stream, Suit suit );
};

std::ostream& operator<<( std::ostream& stream, Card::Rank rank );
std::ostream& operator<<( std::ostream& stream, Card::Suit suit );

class Deck
{
    mutable std::array< Card,
        static_cast<size_t>(Card::Rank::TotalRanks)
        * static_cast<size_t>(Card::Suit::TotalSuits) > __deck;
    mutable size_t __currentcard;
    mutable std::mt19937_64 __shuffler;

public:

    Deck();
    void shuffle() const;
    void view() const;
    const Card deal() const;
};

class Player
{
    std::vector<Card> __hand;
    size_t __11Aces;
    size_t __count;
    const Deck* __game;

public:
    Player( const Deck& game );
    void hit();
    void view() const;
    const size_t count() const;
};

#endif