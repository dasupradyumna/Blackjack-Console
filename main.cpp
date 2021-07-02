#include "blackjack.h"

#include <cstdlib>          // EXIT_SUCCESS, EXIT_FAILURE
#include <stdexcept>        // std::out_of_range, std::invalid_argument

//  TODO  add support for multiplayer, with names and colors(?)
//  TODO  add support for online playing with friends who have the same exe

/* This function controls the central flow of the game, from start to gameover.
 * Calls all the functions needed for making player moves, changing player turns and calculating scores.
 */
bool playBlackJack()
{
  // initialize game-related objects

  Deck game { };
  Player dealer { game };
  Player player { game };
  initialSetup( game, dealer, player );         // sets up game and hands before play starts

  try
  {
    std::cout << "<<< Player's turn >>>\n\n";

    auto turn { Turn::Player };                 // keeps track of game state
    while ( turn != Turn::GameOver )            // play game until gameover state
    {
      // display all the player's cards
      std::cout << "Dealer Cards : ";
      dealer.view();
      std::cout << "Player Cards : ";
      player.view();

      // execute each turn's corresponding logic
      switch ( turn )
      {
        case Turn::Player:  turn = playerTurn( player ); break;
        case Turn::Dealer:  turn = dealerTurn( dealer ); break;
        default:
          throw std::invalid_argument { "main.cpp:: error: invalid argument for `turn` in playBlackJack()\n" };
      }
    }

    // use the scores of all players and determine the winner for final display
    char winner { checkHands( dealer, player ) };
    showWinner( dealer, player, winner );
    return true;
  }
  catch ( std::out_of_range& ex )               // out of range exception from indexing Rank or Suit enums
  {
    std::cout << "\n\n< Out of Range > \n" << ex.what() << '\n';
    return false;
  }
  catch ( std::invalid_argument& ex )           // out of range exception from indexing Rank or Suit enums
  {
    std::cout << "\n\n< Invalid Argument > \n" << ex.what() << '\n';
    return false;
  }
}

int main()
{
  // return value depending on whether or not game completed successfully
  if ( playBlackJack() )
    return EXIT_SUCCESS;
  else
    return EXIT_FAILURE;
}
