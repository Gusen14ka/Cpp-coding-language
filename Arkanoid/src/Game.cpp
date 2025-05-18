#include "Game.hpp"
#include <iostream>

Game::Game() : 
    window_({ WINDOW_WIDTH, WINDOW_HEIGHT }, WINDOW_TITLE)
    , board_()
    , ball_({ WINDOW_WIDTH / 2.f,
        WINDOW_HEIGHT - PADDLE_BOTTOM_OFFSET
        - Paddle::HEIGHT - Ball::RADIUS - 0.1f})
    , paddle_({ WINDOW_WIDTH / 2.f,  WINDOW_HEIGHT - PADDLE_BOTTOM_OFFSET
        - (Paddle::HEIGHT / 2.f) })
{
    window_.setFramerateLimit(60);
    // загружаем карту уровня
    board_.loadFromFile("maps/level1.txt");
    // загружаем спрайты для бонусов
    ResourceHolder::loadBonusTexture("assets");
    ResourceHolder::loadFont("assets/gomarice_no_continue.ttf");
    ResourceHolder::loadHeartTexture("assets/heartSprite.png");

    startScreen_ = new StartScreen(window_.getSize());
    endScreen_ = new EndScreen(window_.getSize());

    initFloor();
    state_ = GameState::Start;
}

int Game::run() {
    while (window_.isOpen()) {
        
        //state_ = GameState::Playing; // для теста

        processInput();

        float dt = clock_.restart().asSeconds();
        dt = std::min(dt, 1.f / 30.f);
        updateSate(dt);

        render();
    }
    return 0;
}

void Game::updateSate(float dt) {
    if (state_ != GameState::Playing) {
        return;
    }

    paddle_.updateVelocity(dt);
    
    if (!ballFirstLaunched_) {
        // пока не дали первый вектор — мяч лежит
        ball_.setPosition({ paddle_.getPosition().x,
            paddle_.getPosition().y - Paddle::HEIGHT / 2 - Ball::RADIUS });
        return;
    }

    ball_.update(dt, window_);

    // Столкновения шар - блоки
    if (auto opt = board_.handleBallCollision(ball_)) {
        if (opt->first.has_value()) {
            bonuses_.emplace_back(opt->first.value(), opt->second);
        }
        score_ += 10;
    }

    // Столкновения шар - платформа
    sf::Vector2f ballCenter = ball_.getPosition();
    float ballRadius = Ball::RADIUS;
    sf::FloatRect paddleBounds = paddle_.getBounds();
    auto [normal, penetration] = computeCollisionCircleAABB(
        ballCenter, ballRadius, paddleBounds);
    bool collided = (penetration > 0.f);

    if (collided && !paddleCollisionLastFrame_)
    {
        if (paddle_.IsStickyEnabled() && !paddle_.hasStuckBall()) {
            paddle_.stickBall(ball_);
        }
        else {
            sf::Vector2f v0 = ball_.getVelocity();
            float speedBeforeCol = std::hypot(v0.x, v0.y);
            ball_.reflect(normal);
            ball_.setPosition(ballCenter 
                + normal * (penetration + 0.1f));

            // учитываем скорость платформы
            float paddleVelocity = paddle_.getVelocityX();
            float factor = 0.5f;
            sf::Vector2f ballVelocity = ball_.getVelocity();
            ballVelocity.x += paddleVelocity * factor;

            float newLen = std::hypot(ballVelocity.x, ballVelocity.y);
            if (newLen != 0.f) {
                ballVelocity *= (speedBeforeCol / newLen);
            }
            ball_.setVelocity(ballVelocity);
        }
    }

    paddleCollisionLastFrame_ = collided;

    // Обновляем и ловим + применяем бонусы
    for (auto it = bonuses_.begin(); it != bonuses_.end();) {
        it->update(dt);
        if (it->getBounds().intersects(paddle_.getBounds())) {
            applyBonus(it->getBonusType());
            it = bonuses_.erase(it);
        }
        else if (it->getPosition().y - Bonus::SIZE / 2 > WINDOW_HEIGHT) {
            it = bonuses_.erase(it);
        }
        else {
            it++;
        }
    }

    // Проверка на конец игры
    if (board_.isCleared()) {
        state_ = GameState::End;
        // чтобы показать финальный счёт на экране
        endScreen_->setScore(score_);
        return; // прекращаем дальнейший апдейт
    }

    // Взаимодействие шара с нижней границей окна
    float ballBottom = ball_.getPosition().y + Ball::RADIUS;
    if (ballBottom > WINDOW_HEIGHT - FLOOR_HEIGHT) 
    {
        if (oneTimefloorActive_) {
            ball_.reflect({ 0.f, -1.f });
            oneTimefloorActive_ = false;
        }
        else if (ballBottom > WINDOW_HEIGHT) {
            if (--lives_ <= 0) {
                state_ = GameState::End;
            }
            else {
                ballFirstLaunched_ = false;
                paddle_.reset();
                bonuses_.clear();
            }
        }
    }
}

void Game::render() {
    window_.clear(sf::Color::Black);

    switch (state_)
    {
    case GameState::Start:
        startScreen_->draw(window_);
        break;
    case GameState::Playing: {
        // Очки как текст
        sf::Text scoreText;
        scoreText.setFont(ResourceHolder::getFont());
        scoreText.setString("Score: " + std::to_string(score_));
        scoreText.setCharacterSize(30);
        scoreText.setFillColor(sf::Color(102, 0, 255));
        scoreText.setPosition(10.f, 10.f);
        window_.draw(scoreText);

        // Жизни — спрайты сердечек
        const sf::Texture& heartTex = ResourceHolder::getHeartTexture();
        // допустим, хотим, чтобы каждое сердце рисовалось размером 24×24
        float targetSize = 30.f;
        float scaleX = targetSize / heartTex.getSize().x;
        float scaleY = targetSize / heartTex.getSize().y;

        // пространство справа: отступ 10px от правого края
        float startX = WINDOW_WIDTH - 10.f - targetSize;
        for (int i = 0; i < lives_; ++i) {
            sf::Sprite heart(heartTex);
            heart.setScale(scaleX, scaleY);
            // располагаем от правого края: каждое следующее — влево на (targetSize+5)
            heart.setPosition(startX - i * (targetSize + 5.f), 15.f);
            window_.draw(heart);
        }

        board_.draw(window_);
        for (auto const& bonus : bonuses_) {
            bonus.draw(window_);
        }
        if (oneTimefloorActive_) {
            window_.draw(floorShape_);
        }
        paddle_.draw(window_);
        ball_.draw(window_);
        break;
    }
    case GameState::End:
        endScreen_->setScore(score_);
        endScreen_->draw(window_);
        break;
    default:
        break;
    }

    window_.display();
}

void Game::processInput() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
            return;
        }
        // передаём инфу в UI или в логику
        switch (state_)
        {
        case GameState::Start:
            if (startScreen_->handleEvent(event)) {
                state_ = GameState::Playing;
            }
            break;
        case GameState::Playing:
            paddle_.handleEvent(event, window_);
            if (!ballFirstLaunched_
                && event.type == sf::Event::MouseButtonReleased
                && event.mouseButton.button == sf::Mouse::Left)
            {
                // запускаем шар
                ball_.initVelocity();
                ballFirstLaunched_ = true;
            }
            break;
        case GameState::End:
            if (endScreen_->handleEvent(event)) {
                // сбросить игру
                score_ = 0;
                lives_ = 3;
                board_.reset();
                paddle_.reset();
                ball_.reset();
                bonuses_.clear();
                ballFirstLaunched_ = false;
                paddleCollisionLastFrame_ = false;
                state_ = GameState::Start;
            }
            break;
        default:
            break;
        }
    }
}

void Game::applyBonus(const BonusType& type) {
    switch (type)
    {
    case BonusType::ExpandPaddle:
        paddle_.multiplyWidth(1.45f);
        break;
    case BonusType::CompressPaddle:
        paddle_.multiplyWidth(0.67f);
        break;
    case BonusType::SlowBall:
        if (!paddle_.hasStuckBall()) {
            ball_.multiplyVelocity(0.8f);
        }
        break;
    case BonusType::FastBall:
        if (!paddle_.hasStuckBall()) {
            ball_.multiplyVelocity(1.3f);
        }
        break;
    case BonusType::StickyPaddle:
        paddle_.enableSticky();
        break;
    case BonusType::OneTimeFloor:
        oneTimefloorActive_ = true;
        break;
    case BonusType::RandomBounce:
        if (!paddle_.hasStuckBall()) {
            ball_.scheduleRandBounce(1.f);
        }
        break;
    default:
        break;
    }
}

void Game::initFloor() {
    floorShape_.setSize({ float(WINDOW_WIDTH), FLOOR_HEIGHT });
    floorShape_.setFillColor(sf::Color(100, 100, 100));
    floorShape_.setPosition(0.f, WINDOW_HEIGHT - FLOOR_HEIGHT);
}
