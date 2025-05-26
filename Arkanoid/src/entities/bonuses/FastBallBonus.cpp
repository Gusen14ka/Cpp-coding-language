#include "FastBallBonus.hpp"

FastBallBonus::FastBallBonus(Ball& ball,
	const sf::Vector2f& pos)
	: Bonus(pos), ball_(ball)
{
	auto& tex = ResourceHolder::getBonusTexture(
		BonusType::SlowBall);
	sprite_.setTexture(tex);
	sprite_.setScale(SIZE / tex.getSize().x,
		SIZE / tex.getSize().y);
}

void FastBallBonus::apply() {
	ball_.multiplyVelocity(1.3f);
}