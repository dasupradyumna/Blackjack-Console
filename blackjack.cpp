#include "blackjack.h"

#include <algorithm>        // std::shuffle
#include <ctime>            // std::time
#include <stdexcept>        // std::out_of_range

Card::Card() :
    __rank { },
    __suit { }
{}

void Card::print() const
{
    std::cout << __rank << __suit << ' ';
}

const size_t Card::value() const
{
    auto rank_numeric = static_cast<size_t>(__rank);
    if ( rank_numeric <= 8ull )
        return rank_numeric + 2ull;
    else if ( rank_numeric <= 11ull )
        return 10ull;
    else if ( rank_numeric == 12ull )
        return 11ull;
    else
        throw std::out_of_range { "error: `rank` out of Enum range.\n" };
}

std::ostream& operator<<( std::ostream& stream, Card::Rank rank )
{
    constexpr char RankEnum[]
    { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' };
    if ( static_cast<size_t>(rank) > 12ull )
        throw std::out_of_range { "error: `rank` out of Enum range.\n" };
    else
        return stream << RankEnum[ static_cast<size_t>(rank) ];
}

std::ostream& operator<<( std::ostream& stream, Card::Suit suit )
{
    constexpr char SuitEnum[] { 'C', 'D', 'H', 'S' };
    if ( static_cast<size_t>(suit) > 3ull )
        throw std::out_of_range { "error: `suit` out of Enum range.\n" };
    else
        return stream << SuitEnum[ static_cast<size_t>(suit) ];
}

Deck::Deck() :
    __deck { },
    __currentcard { 0ull },
    __shuffler { static_cast<std::mt19937_64::result_type>(std::time( nullptr )) }
{
    size_t card { 0ull };
    for ( size_t suit { 0ull }; suit < static_cast<size_t>(Card::Suit::TotalSuits); ++suit )
    {
        for ( size_t rank { 0ull }; rank < static_cast<size_t>(Card::Rank::TotalRanks); ++rank )
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
        __currentcard = 0ull;
    }

    return __deck[ __currentcard++ ];
}

Player::Player( const Deck& game ) :
    __hand { },
    __11Aces { 0ull },
    __count { 0ull },
    __game { &game }
{}

void Player::hit()
{
    auto newcard { __game->deal() };

    __hand.push_back( newcard );
    if ( newcard.value() == 11ull )
        ++__11Aces;

    __count += newcard.value();
    if ( __count > 21ull )
    {
        if ( __11Aces != 0ull )
        {
            --__11Aces;
            __count -= 10ull;
        }
        else
            __count = 0ull;
    }
}

void Player::view() const
{
    for ( const Card& card : __hand )
        card.print();
    std::cout << '\n';
}

const size_t Player::count() const { return __count; }
