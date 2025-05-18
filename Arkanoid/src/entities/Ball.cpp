#include "Ball.hpp"
#include <random>
#include <chrono>

Ball::Ball(const sf::Vector2f& pos) : initialPosition_(pos)
{
    shape_.setRadius(RADIUS);
    shape_.setOrigin(RADIUS, RADIUS);
    shape_.setPosition(initialPosition_);
    shape_.setFillColor(sf::Color::White);

    rng_ = std::mt19937(static_cast<unsigned>(std::time(nullptr)));

    initVelocity();
}

void Ball::initVelocity() {
    std::uniform_real_distribution<float> dist(30.f, 150.f);
    float angleRad = dist(rng_) * static_cast<float>(PI / 180.0);

    velocity_.x = std::cos(angleRad) * INITIAL_SPEED;
    velocity_.y = - std::sin(angleRad) * INITIAL_SPEED;
}

void Ball::update(float dt, const sf::RenderWindow& window) {
    // добавляем перемещение
    sf::Vector2f position = shape_.getPosition() + velocity_ * dt;

    // применяем отскок, если нужно
    auto size = window.getSize();

    if (position.x - RADIUS <= 0.f) {
        position.x = RADIUS;
        reflect({ 1.f, 0.f });
    }
    else if (position.x + RADIUS >= size.x) {
        position.x = size.x - RADIUS;
        reflect({ -1.f, 0.f });
    }
    else if (position.y - RADIUS <= 0.f) {
        position.y = RADIUS;
        reflect({ 0.f, 1.f });
    }

    //Таймер для бонуса RandomBounce
    if (randomBounceTimer_ > 0.f) {
        randomBounceTimer_ -= dt;
        if (randomBounceTimer_ <= 0.f) {
            performRandomBounce();
            randomBounceTimer_ = -1.f;
        }
    }

    shape_.setPosition(position);
}

void Ball::reflect(const sf::Vector2f& normal) {
    float dot = velocity_.x * normal.x + velocity_.y * normal.y;
    velocity_.x -= 2.f * dot * normal.x;
    velocity_.y -= 2.f * dot * normal.y;

    // Гарантируем минимальную вертикальную составляющую
    float speed = std::hypot(velocity_.x, velocity_.y);
    float vy = velocity_.y;
    float signY = (vy >= 0.f ? 1.f : -1.f);

    if (std::abs(vy) < MIN_VY) {
        float newVy = signY * MIN_VY;
        float newVx = std::sqrt(std::max(0.f, speed * speed - newVy * newVy));
        newVx = (velocity_.x >= 0.f ? 1.f : -1.f) * newVx;

        velocity_.x = newVx;
        velocity_.y = newVy;
    }
}

void Ball::reset() {
    shape_.setPosition(initialPosition_);
}

void Ball::draw(sf::RenderWindow& window) const {
    window.draw(shape_);
}

sf::Vector2f Ball::getPosition() const {
    return shape_.getPosition();
}

sf::FloatRect Ball::getBounds() const {
    return shape_.getGlobalBounds();
}

sf::Vector2f Ball::getVelocity() const {
    return velocity_;
}

void Ball::multiplyVelocity(float multiplier) {
    sf::Vector2f newVelocity = velocity_ * multiplier;
    float speed = std::hypot(newVelocity.x, newVelocity.y);
    if (speed > INITIAL_SPEED * 3.5f || speed < INITIAL_SPEED / 4.f) {
        newVelocity = newVelocity / speed;
        speed = (multiplier >= 1 ? INITIAL_SPEED * 3.5f 
            : INITIAL_SPEED / 4.f);
        newVelocity *= speed;
    }
    velocity_ = newVelocity;
}

void Ball::setVelocity(const sf::Vector2f& newVelocity) {
    velocity_ = newVelocity;
}

void Ball::setPosition(const sf::Vector2f& pos) {
    shape_.setPosition(pos);
}

void Ball::performRandomBounce() {
    float speed = std::hypot(velocity_.x, velocity_.y);

    std::uniform_real_distribution<float> dist(0.f, 360.f);
    float angle = dist(rng_) * static_cast<float>(PI / 180.f);

    velocity_.x = std::cos(angle) * speed;
    velocity_.y = std::sin(angle) * speed;
}

void Ball::scheduleRandBounce(float delay) {
    randomBounceTimer_ = delay;
}