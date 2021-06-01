#include "blackjack.h"

#include <algorithm>        // std::shuffle
#include <ctime>            // std::time


/////////////////////////////////////////// CARD ///////////////////////////////////////////

Card::Card() :          // default zero-initializer
    __rank { },
    __suit { }
{}

// return value of rank of the card
const size_t Card::value() const
{
    auto rank_numeric = static_cast<size_t>(__rank);
    if ( rank_numeric <= 8ull )
        return rank_numeric + 2ull;             // numeric ranks have respective values
    else if ( rank_numeric <= 11ull )
        return 10ull;                           // face ranks have value 10
    else if ( rank_numeric == 12ull )
        return 11ull;                           // ace has default value 11
    else
        throw std::out_of_range { "blackjack.cpp:: error: `rank` out of Enum range in Card::value()\n" };
}

// overloads the insertion operator for Card objects
std::ostream& operator<<( std::ostream& stream, const Card card )
{
    // compile-time constant list of rank characters
    static constexpr char RankEnum[] { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' };

    // compile-time constant list of suit characters
    static constexpr char SuitEnum[] { 'C', 'D', 'H', 'S' };

    // check for index bounds for rank and suit, and print card code
    if ( static_cast<size_t>(card.__rank) > 12ull )
        throw std::out_of_range { "blackjack.cpp:: error: `rank` out of Enum range in overload<<()\n" };
    else if ( static_cast<size_t>(card.__suit) > 3ull )
        throw std::out_of_range { "blackjack.cpp:: error: `suit` out of Enum range in overload<<()\n" };
    else
        return stream << RankEnum[ static_cast<size_t>(card.__rank) ] << SuitEnum[ static_cast<size_t>(card.__suit) ];
}


/////////////////////////////////////////// DECK ///////////////////////////////////////////

Deck::Deck() :
    __deck { },                     // zero-initialization
    __currentcard { 0ull },         // starts at first card of deck
    __shuffler { static_cast<std::mt19937_64::result_type>(std::time( nullptr )) }  // seeding the MT engine
{
    // create an ordered deck by iterating through entire Rank and Suit enums
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

// shuffles entire deck using random MT engine
void Deck::shuffle() const
{
    std::shuffle( __deck.begin(), __deck.end(), __shuffler );
}

// prints the deck cards' codes
void Deck::view() const
{
    for ( const Card& card : __deck )
        std::cout << card << ' ';
    std::cout << '\n';
}

// returns a card from the deck
// shuffle the deck and start from first, if the card is the last in the deck
const Card Deck::deal() const
{
    if ( __currentcard == __deck.size() )
    {
        shuffle();
        __currentcard = 0ull;
    }

    return __deck[ __currentcard++ ];
}


////////////////////////////////////////// PLAYER //////////////////////////////////////////

Player::Player( const Deck& game ) :
    __hand { },             // empty hands
    __11Ace { false },      // no ace in an empty hand
    __count { 0ull },
    __game { game }
{}

// request a card from the linked deck
// update the count of the hand with each requested card
void Player::hit()
{
    auto newcard { __game.deal() };     // get card from deck

    __hand.push_back( newcard );        // add to hand
    __count += newcard.value();         // add value to count

    if ( newcard.value() == 11ull )     // if request is an ace, it will have value 11
        __11Ace = true;

    if ( __count > 21ull && __11Ace )   // if count is more than 21 and an ace has value 11
    {
        __11Ace = false;                // consider the ace value 1
        __count -= 10ull;               // correct the count
    }
}

// display the player's hand
void Player::view() const
{
    for ( const Card& card : __hand )
        std::cout << card << ' ';
    std::cout << '\n';
}

const size_t Player::count() const { return __count; }


//////////////////////////////////// Gameplay functions ////////////////////////////////////

// sets up game and player hands before play starts
void initialSetup( Deck& game, Player& dealer, Player& player )
{
    std::cout << "\n<<<<<< Game start >>>>>>\n\n";

    game.shuffle();         // shuffle the ordered deck
    dealer.hit();           // give the dealer one card
    player.hit();           // give the players two cards
    player.hit();
}

// contains the logic to be executed when it's the player's turn
Turn playerTurn( Player& player )
{
    // ask the player for their next move
    std::cout << "Player move >> ";
    std::string playerMove {};
    std::getline( std::cin, playerMove, '\n' );

    // convert the input string to lower case
    std::transform( playerMove.begin(), playerMove.end(), playerMove.begin(),
        [ ] ( unsigned char c )
        {
            return std::tolower( c );
        } );

    if ( playerMove == "hit" )              // if player chooses to hit
    {
        std::cout << "Player chose to hit!\n";
        player.hit();                       // take a card
        if ( player.count() > 21ull )       // if score is more than 21, player busts
        {
            std::cout << "Player busted!\n\n";
            return Turn::GameOver;          // skip all turns and gameover
        }
        std::cout << '\n';
        return Turn::Player;
    }
    else if ( playerMove == "stand" )       // if players stands, turn goes to dealer
    {
        std::cout << "Player chose to stand!\n\n"
            "<<< Dealer's turn >>>\n\n";
        return Turn::Dealer;
    }
    else                                    // handling invalid input
    {
        std::cout << "Invalid move! Type either 'hit' or 'stand'.\n\n";
        return Turn::Player;
    }
}

// contains the logic to be executed when it's the dealer's turn
Turn dealerTurn( Player& dealer )
{
    if ( 0ull < dealer.count() && dealer.count() < 17ull ) // otherwise, he chooses to hit
    {
        std::cout << "Dealer chose to hit!\n";
        dealer.hit();                       // take a card
        if ( dealer.count() > 21ull )       // dealer busts if his score goes above 21
        {
            std::cout << "Dealer busted!\n\n";
            return Turn::GameOver;
        }
        std::cout << '\n';
    }
    else if ( dealer.count() >= 17ull )     // if dealer's hand value is atleast 17, he stands
    {
        std::cout << "Dealer chose to stand!\n\n";
        return Turn::GameOver;
    }
    return Turn::Dealer;
}

// determine the winner using the scores of the players
const char checkHands( const Player& dealer, const Player& player )
{
    // if dealer busts or player has higher count, player wins
    if ( dealer.count() > 21ull || dealer.count() < player.count() )
        return 'p';
    // if player busts or dealer has higher count, dealer wins
    else if ( player.count() > 21ull || player.count() < dealer.count() )
        return 'd';
    // otherwise, it's a tie : player.count() == dealer.count()
    else
        return 0;
}

// display the winner and final player scores
void showWinner( const Player& dealer, const Player& player, const char winner )
{
    std::cout << "<<< Result >>>\n\n";
    switch ( winner )
    {
        case 0:     std::cout << "It's a Tie!\n"; break;
        case 'p':   std::cout << "Player won!\n"; break;
        case 'd':   std::cout << "Dealer won!\n"; break;
        default:
            throw std::invalid_argument { "blackjack.cpp:: error: invalid argument for `winner` in showWinner()\n" };
    }

    // display dealer's hand and score
    std::cout << "\nDealer's Hand :: [" << dealer.count() << "] ";
    dealer.view();

    // display player's hand and score
    std::cout << "\nPlayer's Hand :: [" << player.count() << "] ";
    player.view();

    std::cout << "\n<<<<<< Game over >>>>>>\n";
}
