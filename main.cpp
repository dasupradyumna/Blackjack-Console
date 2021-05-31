#include "blackjack.h"

#include <cstdlib>          // EXIT_SUCCESS, EXIT_FAILURE
#include <iostream>         // std::cout
#include<stdexcept>         // std::out_of_range

bool playBlackJack()
{
    Deck game { };
    Player dealer { game };
    Player player { game };
    initialSetup( game, dealer, player );
    // game.view();

    try
    {
        std::cout << "<<< Player's turn >>>\n\n";

        auto turn { Turn::Player };
        char winner {};
        while ( turn != Turn::GameOver )
        {
            std::cout << "Dealer Cards : ";
            dealer.view();
            std::cout << "Player Cards : ";
            player.view();

            switch ( turn )
            {
                case Turn::Player:      turn = playerTurn( player ); break;
                case Turn::Dealer:      turn = dealerTurn( dealer ); break;
                default:throw;
            }
        }

        winner = checkHands( dealer, player );
        showWinner( dealer, player, winner );
        return true;
    }
    catch ( std::out_of_range& ex )
    {
        std::cout << ex.what();
        return false;
    }
}

int main()
{
    if ( playBlackJack() )
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}
