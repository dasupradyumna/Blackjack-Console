#include "blackjack.h"

#include <cstdlib>          // EXIT_SUCCESS, EXIT_FAILURE
#include <iostream>         // std::cout

bool playBlackJack()
{
    Deck game { };
    game.view();
    game.shuffle();
    game.view();

    std::vector<Deck::Card> dealer { };
    dealer.push_back(game.deal());
    std::vector<Deck::Card> player { };
    player.push_back(game.deal());
    player.push_back(game.deal());

    for ( auto x : dealer ) printCard(x);
    std::cout << handValue(dealer) << '\n';
    for ( auto x : player ) printCard(x);
    std::cout << handValue(player) << '\n';

    return true;
}

int main()
{
    if ( playBlackJack() )
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}