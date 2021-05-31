#include "blackjack.h"

#include <algorithm>        // std::shuffle
#include <ctime>            // std::time

Card::Card() :
    __rank { },
    __suit { }
{}

void Card::print() const
{
    std::cout << __rank << __suit << ' ';
}

const int Card::value() const
{
    int rank_numeric = static_cast<int>(__rank);
    if ( rank_numeric <= 8 )
        return (rank_numeric + 2);
    else if ( rank_numeric <= 11 )
        return 10;
    else if ( rank_numeric == 12 )
        return 11;
    else
        throw;
}

std::ostream& operator<<( std::ostream& stream, Card::Rank rank )
{
    constexpr char RankEnum[]
    { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' };
    if ( static_cast<size_t>(rank) > 12ULL )
        throw;
    else
        return stream << RankEnum[ static_cast<size_t>(rank) ];
}

std::ostream& operator<<( std::ostream& stream, Card::Suit suit )
{
    constexpr char SuitEnum[] { 'C', 'D', 'H', 'S' };
    if ( static_cast<size_t>(suit) > 3ULL )
        throw;
    else
        return stream << SuitEnum[ static_cast<size_t>(suit) ];
}

Deck::Deck() :
    __deck { },
    __shuffler { static_cast<std::mt19937_64::result_type>(std::time( nullptr )) }
{
    size_t card { 0ULL };
    for ( size_t suit { 0ULL }; suit < static_cast<size_t>(Card::Suit::TotalSuits); ++suit )
    {
        for ( size_t rank { 0ULL }; rank < static_cast<size_t>(Card::Rank::TotalRanks); ++rank )
        {
            __deck[ card ].__rank = static_cast<Card::Rank>(rank);
            __deck[ card ].__suit = static_cast<Card::Suit>(suit);
            ++card;
        }
    }
}

void Deck::shuffle() const
{
    std::shuffle( __deck.begin(), __deck.end(), __shuffler );
}

void Deck::view() const
{
    for ( const Card& card : __deck )
        card.print();
    std::cout << '\n';
}

const Card Deck::deal() const
{
    if ( __currentcard == __deck.size() )
    {
        shuffle();
        __currentcard = 0;
    }

    return __deck[ __currentcard++ ];
}

Player::Player( const Deck& game ) :
    __hand { },
    __game { &game }
{}

void Player::hit()
{
    __hand.push_back( __game->deal() );
}

void Player::view() const
{
    for ( const Card& card : __hand )
        card.print();
    std::cout << '\n';
}

const int Player::handValue() const
{
    return std::accumulate( __hand.begin(), __hand.end(), 0,
        [ ] ( int accumulated, Card next ) -> int
        {
            return accumulated + next.value();
        }
    );
}
