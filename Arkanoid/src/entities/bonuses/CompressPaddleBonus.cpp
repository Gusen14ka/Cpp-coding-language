#include "CompressPaddleBonus.hpp"

CompressPaddleBonus::CompressPaddleBonus(Paddle& paddle,
	const sf::Vector2f& pos)
	: Bonus(pos), paddle_(paddle)
{
	const auto& tex = ResourceHolder::getBonusTexture(
		BonusType::CompressPaddle);
	sprite_.setTexture(tex);
	sprite_.setScale(SIZE / tex.getSize().x, SIZE / tex.getSize().y);
}

void CompressPaddleBonus::apply() {
	paddle_.multiplyWidth(0.67f);
}