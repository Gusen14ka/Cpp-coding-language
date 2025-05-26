#pragma once

#include "Bonus.hpp"
#include "../../EnumTypes.hpp"
#include "../../ResourceHolder.hpp"
#include "../Ball.hpp"

class FastBallBonus : public Bonus {
public:
	FastBallBonus(Ball& ball, const sf::Vector2f& pos);

	void apply() override;

private:
	Ball& ball_;
};

