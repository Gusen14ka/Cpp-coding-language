#include "ExpandPaddleBonus.hpp"

ExpandPaddleBonus::ExpandPaddleBonus(Paddle& paddle,
	const sf::Vector2f& pos)
	: Bonus(pos), paddle_(paddle)
{
	const auto& tex = ResourceHolder::getBonusTexture(BonusType::ExpandPaddle);
	sprite_.setTexture(tex);
	sprite_.setScale(SIZE / tex.getSize().x, SIZE / tex.getSize().y);
}

void ExpandPaddleBonus::apply() {
	paddle_.multiplyWidth(1.45f);
}