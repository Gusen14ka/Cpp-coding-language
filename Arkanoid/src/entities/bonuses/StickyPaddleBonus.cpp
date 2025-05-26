#include "StickyPaddleBonus.hpp"

StickyPaddleBonus::StickyPaddleBonus(Paddle& paddle,
    const sf::Vector2f& pos)
    : Bonus(pos), paddle_(paddle) {

    const auto& tex = ResourceHolder::getBonusTexture(
        BonusType::StickyPaddle);
    sprite_.setTexture(tex);
    sprite_.setScale(SIZE / tex.getSize().x,
        SIZE / tex.getSize().y);
}

void StickyPaddleBonus::apply() {
    paddle_.enableSticky();
}