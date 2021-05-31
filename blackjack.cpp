#include "blackjack.h"

#include <algorithm>            // std::shuffles
#include <ctime>                // std::time
#include <iostream>             // std::cout

Deck::Deck() :
    __deck { },
    __shuffler { static_cast<std::mt19937_64::result_type>(std::time(nullptr)) }
{
    size_t card { 0ULL };
    for ( size_t suit { 0ULL }; suit < static_cast<size_t>(Deck::Suit::TotalSuits); ++suit )
    {
        for ( size_t rank { 0ULL }; rank < static_cast<size_t>(Deck::Rank::TotalRanks); ++rank )
        {
            __deck[card].rank = static_cast<Deck::Rank>(rank);
            __deck[card].suit = static_cast<Deck::Suit>(suit);
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
        printCard(card);
    std::cout << '\n';
}

Deck::Card Deck::deal()
{
    static size_t card { 0 };
    return __deck[card++];
}

void printCard(const Deck::Card card)
{
    switch ( card.rank )
    {
        case Deck::Rank::Two: std::cout << '2'; break;
        case Deck::Rank::Three: std::cout << '3'; break;
        case Deck::Rank::Four: std::cout << '4'; break;
        case Deck::Rank::Five: std::cout << '5'; break;
        case Deck::Rank::Six: std::cout << '6'; break;
        case Deck::Rank::Seven: std::cout << '7'; break;
        case Deck::Rank::Eight: std::cout << '8'; break;
        case Deck::Rank::Nine: std::cout << '9'; break;
        case Deck::Rank::Ten: std::cout << 'T'; break;
        case Deck::Rank::Jack: std::cout << 'J'; break;
        case Deck::Rank::Queen: std::cout << 'Q'; break;
        case Deck::Rank::King: std::cout << 'K'; break;
        case Deck::Rank::Ace: std::cout << 'A'; break;
    }

    switch ( card.suit )
    {
        case Deck::Suit::Club: std::cout << 'C' << ' '; break;
        case Deck::Suit::Diamond: std::cout << 'D' << ' '; break;
        case Deck::Suit::Heart: std::cout << 'H' << ' '; break;
        case Deck::Suit::Spade: std::cout << 'S' << ' '; break;
    }
}

int handValue(const std::vector<Deck::Card>& hand)
{
    auto addCardRank = [ ] (int accumulated, Deck::Card next) -> int
    {
        int rank_numeric = static_cast<int>(next.rank);
        if ( rank_numeric <= 8 )
            return accumulated + rank_numeric + 2;
        else if ( rank_numeric <= 11 )
            return accumulated + 10;
        else if ( rank_numeric == 12 )
            return accumulated + 11;
        else
            throw;
    };
    return std::accumulate(hand.begin(), hand.end(), 0, addCardRank);
}
