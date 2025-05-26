#include "SlowBallBonus.hpp"

SlowBallBonus::SlowBallBonus(Ball& ball,
	const sf::Vector2f& pos)
	: Bonus(pos), ball_(ball)
{
	auto& tex = ResourceHolder::getBonusTexture(
		BonusType::SlowBall);
	sprite_.setTexture(tex);
	sprite_.setScale(SIZE / tex.getSize().x,
		SIZE / tex.getSize().y);
}

void SlowBallBonus::apply() {
	ball_.multiplyVelocity(0.8f);
}