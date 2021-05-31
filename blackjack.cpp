#include "blackjack.h"

#include <algorithm>            // std::shuffles
#include <ctime>                // std::time
#include <iostream>             // std::cout

Card::Card() :
    __rank { },
    __suit { }
{}

void Card::print() const
{
    switch ( __rank )
    {
        case Rank::Two:     std::cout << '2'; break;
        case Rank::Three:   std::cout << '3'; break;
        case Rank::Four:    std::cout << '4'; break;
        case Rank::Five:    std::cout << '5'; break;
        case Rank::Six:     std::cout << '6'; break;
        case Rank::Seven:   std::cout << '7'; break;
        case Rank::Eight:   std::cout << '8'; break;
        case Rank::Nine:    std::cout << '9'; break;
        case Rank::Ten:     std::cout << 'T'; break;
        case Rank::Jack:    std::cout << 'J'; break;
        case Rank::Queen:   std::cout << 'Q'; break;
        case Rank::King:    std::cout << 'K'; break;
        case Rank::Ace:     std::cout << 'A'; break;
    }

    switch ( __suit )
    {
        case Suit::Club:    std::cout << 'C'; break;
        case Suit::Diamond: std::cout << 'D'; break;
        case Suit::Heart:   std::cout << 'H'; break;
        case Suit::Spade:   std::cout << 'S'; break;
    }

    std::cout << ' ';
}

Deck::Deck() :
    __deck { },
    __shuffler { static_cast<std::mt19937_64::result_type>(std::time(nullptr)) }
{
    size_t card { 0ULL };
    for ( size_t suit { 0ULL }; suit < static_cast<size_t>(Card::Suit::TotalSuits); ++suit )
    {
        for ( size_t rank { 0ULL }; rank < static_cast<size_t>(Card::Rank::TotalRanks); ++rank )
        {
            __deck[card].__rank = static_cast<Card::Rank>(rank);
            __deck[card].__suit = static_cast<Card::Suit>(suit);
            ++card;
        }
    }
}

void Deck::shuffle()
{
    std::shuffle(__deck.begin(), __deck.end(), __shuffler);
}

void Deck::view()
{
    for ( const Card& card : __deck )
        card.print();
    std::cout << '\n';
}

Card Deck::deal() const
{
    static size_t card { 0 };
    return __deck[card++];
}

Player::Player(const Deck& game) :
    __hand { },
    __game { &game }
{}

void Player::hit()
{
    __hand.push_back(__game->deal());
}

void Player::view()
{
    for ( const Card& card : __hand )
        card.print();
    std::cout << '\n';
}

int Player::handValue()
{
    auto addCardRank = [ ] (int accumulated, Card next) -> int
    {
        int rank_numeric = static_cast<int>(next.__rank);
        if ( rank_numeric <= 8 )
            return accumulated + rank_numeric + 2;
        else if ( rank_numeric <= 11 )
            return accumulated + 10;
        else if ( rank_numeric == 12 )
            return accumulated + 11;
        else
            throw;
    };

    return std::accumulate(__hand.begin(), __hand.end(), 0, addCardRank);
}
