#include "ResourceHolder.hpp"
#include <stdexcept>

std::map<BonusType, sf::Texture> ResourceHolder::bonusTextures_;

void ResourceHolder::loadBonusTexture(const std::string& folderPath) {
	struct Entry { BonusType type; const char* filename; };

	static const Entry entries[] = {
		{BonusType::ExpandPaddle, "ExpandPaddleSprite.png"},
		{BonusType::CompressPaddle, "CompressPaddleSprite.png"},
		{BonusType::SlowBall, "SlowBallSprite.png"},
		{BonusType::FastBall, "FastBallSprite.png"},
		{BonusType::StickyPaddle, "StickyPaddleSprite.png"},
		{BonusType::OneTimeFloor, "OneTimeFloorSprite.png"},
		{BonusType::RandomBounce, "RandomBounceSprite.png"}
	};

	for (auto& ent : entries) {
		sf::Texture tex;
		if (!tex.loadFromFile(folderPath + "/" + ent.filename)) {
			throw std::runtime_error("Failed to load texture: " + folderPath + "/" + ent.filename);
		}

		tex.setSmooth(true);
		bonusTextures_[ent.type] = std::move(tex);
	}
}

const sf::Texture& ResourceHolder::getBonusTexture(const BonusType& type) {
	return bonusTextures_.at(type);
}

void ResourceHolder::loadFont(const std::string& filename) {
	sf::Font f;
	if (!f.loadFromFile(filename)) {
		throw std::runtime_error("Cannot load font " + filename);
	}
	fonts_["main"] = f;
}

const sf::Font& ResourceHolder::getFont() {
	return fonts_.at("main");
}

void ResourceHolder::loadHeartTexture(const std::string& filename) {
	sf::Texture tex;
	if (!tex.loadFromFile(filename))
		throw std::runtime_error("Cannot load heart texture " + filename);
	heartTexture_ = tex;
}

const sf::Texture& ResourceHolder::getHeartTexture() {
	return heartTexture_;
}