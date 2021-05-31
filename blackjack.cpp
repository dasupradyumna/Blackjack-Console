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
        throw std::out_of_range { "blackjack.cpp:: error: `rank` out of Enum range in Card::value()\n" };
}

std::ostream& operator<<( std::ostream& stream, Card::Rank rank )
{
    constexpr char RankEnum[]
    { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' };
    if ( static_cast<size_t>(rank) > 12ull )
        throw std::out_of_range { "blackjack.cpp:: error: `rank` out of Enum range in overload<<()\n" };
    else
        return stream << RankEnum[ static_cast<size_t>(rank) ];
}

std::ostream& operator<<( std::ostream& stream, Card::Suit suit )
{
    constexpr char SuitEnum[] { 'C', 'D', 'H', 'S' };
    if ( static_cast<size_t>(suit) > 3ull )
        throw std::out_of_range { "blackjack.cpp:: error: `suit` out of Enum range in overload<<()\n" };
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
    __11Ace { false },
    __count { 0ull },
    __game { &game }
{}

void Player::hit()
{
    auto newcard { __game->deal() };

    __hand.push_back( newcard );
    __count += newcard.value();

    if ( newcard.value() == 11ull )
        __11Ace = true;

    if ( __count > 21ull && __11Ace )
    {
        __11Ace = false;
        __count -= 10ull;
    }
}

void Player::view() const
{
    for ( const Card& card : __hand )
        card.print();
    std::cout << '\n';
}

const size_t Player::count() const { return __count; }

void initialSetup( Deck& game, Player& dealer, Player& player )
{
    std::cout << "\n<<<<<< Game start >>>>>>\n\n";

    game.shuffle();
    dealer.hit();
    player.hit();
    player.hit();
}

Turn playerTurn( Player& player )
{
    std::cout << "Player turn >> ";
    std::string playerMove;
    std::getline( std::cin, playerMove, '\n' );
    std::transform( playerMove.begin(), playerMove.end(), playerMove.begin(),
        [ ] ( unsigned char c )
        {
            return std::tolower( c );
        } );

    if ( playerMove == "hit" )
    {
        std::cout << "Player chose to hit!\n";
        player.hit();
        if ( player.count() > 21ull )
        {
            std::cout << "Player busted!\n\n";
            return Turn::GameOver;
        }
        std::cout << '\n';
        return Turn::Player;
    }
    else if ( playerMove == "stand" )
    {
        std::cout << "Player chose to stand!\n\n"
            "<<< Dealer's turn >>>\n\n";
        return Turn::Dealer;
    }
    else
    {
        std::cout << "Invalid move! Type either 'hit' or 'stand'.\n\n";
        return Turn::Player;
    }
}

Turn dealerTurn( Player& dealer )
{
    if ( dealer.count() >= 17ull )
    {
        std::cout << "Dealer chose to stand!\n\n";
        return Turn::GameOver;
    }
    else if ( 0ull < dealer.count() && dealer.count() < 17ull )
    {
        std::cout << "Dealer chose to hit!\n";
        dealer.hit();
        if ( dealer.count() > 21ull )
        {
            std::cout << "Dealer busted!\n\n";
            return Turn::GameOver;
        }
        std::cout << '\n';
        return Turn::Dealer;
    }
    else    // dealer.count() == 0ull
        return Turn::GameOver;
}

const char checkHands( const Player& dealer, const Player& player )
{
    if ( dealer.count() > 21ull || dealer.count() < player.count() )
        return 'p';
    else if ( player.count() > 21ull || player.count() < dealer.count() )
        return 'd';
    else    // player.count() == dealer.count()
        return 0;
}

void showWinner( const Player& dealer, const Player& player, const char& winner )
{
    std::cout << "<<< Result >>>\n\n";
    switch ( winner )
    {
        case 0:     std::cout << "It's a Tie!\n"; break;
        case 'p':   std::cout << "Player won!\n"; break;
        case 'd':   std::cout << "Dealer won!\n"; break;
        default:throw;
    }

    std::cout << "\nDealer's Hand :: [" << dealer.count() << "] ";
    dealer.view();

    std::cout << "\nPlayer's Hand :: [" << player.count() << "] ";
    player.view();

    std::cout << "\n<<<<<< Game over >>>>>>\n";
}
