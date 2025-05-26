#pragma once

#include "Bonus.hpp"
#include "../../EnumTypes.hpp"
#include "../../ResourceHolder.hpp"
#include "../Ball.hpp"

class SlowBallBonus : public Bonus {
public:
	SlowBallBonus(Ball& ball, const sf::Vector2f& pos);

	void apply() override;

private:
	Ball& ball_;
};
