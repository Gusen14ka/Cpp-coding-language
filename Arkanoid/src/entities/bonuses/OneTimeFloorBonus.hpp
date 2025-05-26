#pragma once

#include "Bonus.hpp"
#include "../../EnumTypes.hpp"
#include "../../ResourceHolder.hpp"

// Форвард-декларация
class Game;

class OneTimeFloorBonus : public Bonus {
public:
	OneTimeFloorBonus(Game& game, const sf::Vector2f& pos);

	void apply() override;

private:
	Game& game_;
};
