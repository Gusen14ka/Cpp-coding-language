// src/main.cpp
#include <SFML/Graphics.hpp>
#include "Game.hpp"

int main() {
    Game game;            // в ctor или init внутри вызывает loadFromFile
    return game.run();    // запускает цикл: update + render
}
