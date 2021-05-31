#include "blackjack.h"

#include <cstdlib>          // EXIT_SUCCESS, EXIT_FAILURE
#include <iostream>         // std::cout

bool playBlackJack()
{
    Deck game { };
    game.shuffle();
    game.view();

    Player dealer { game };
    dealer.hit();
    Player player { game };
    player.hit();
    player.hit();

    dealer.view();
    std::cout << dealer.handValue() << '\n';
    player.view();
    std::cout << player.handValue() << '\n';

    return true;
}

int main()
{
    if ( playBlackJack() )
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}