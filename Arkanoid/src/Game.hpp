#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Board.hpp"
#include "entities/Ball.hpp"
#include "entities/Paddle.hpp"
#include "entities/Block.hpp"
#include "entities/bonuses/Bonus.hpp"
#include "ResourceHolder.hpp"
#include "ui/StartScreen.hpp"
#include "ui/EndScreen.hpp"
#include "CollisionUtils.hpp"


const unsigned int WINDOW_WIDTH = 600;
const unsigned int WINDOW_HEIGHT = 800;
static constexpr const char* WINDOW_TITLE = "Arkanoid";


enum class GameState {
    Start,
    Playing,
    End
};

class Game {
public:
    static constexpr float FLOOR_HEIGHT = 5.f;
    static constexpr float PADDLE_BOTTOM_OFFSET = 20;
    Game();

    int run();



private:
    GameState state_;
    sf::RenderWindow window_;
    sf::Clock  clock_;
    // прямоугольник для отрисовки пола
    sf::RectangleShape floorShape_;

    Board board_;
    Ball ball_;
    Paddle paddle_;
    std::vector<Bonus> bonuses_;

    StartScreen* startScreen_;
    EndScreen* endScreen_;

    int score_ = 0;
    int lives_ = 3;
    bool oneTimefloorActive_ = false;
    
    // Запущен ли шар после смерти/начала игры
    bool ballFirstLaunched_ = false;

    // Сработали ли коллизия с платформой недавно
    bool paddleCollisionLastFrame_ = false;

    void processInput();
    void updateSate(float dt);
    void render();

    // Применяет бонусы
    void applyBonus(const BonusType& type);

    // Инициализирует "пол" для соответствующего бонуса
    void initFloor();
};
