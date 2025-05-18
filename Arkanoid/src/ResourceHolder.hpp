#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "EnumTypes.hpp"

class ResourceHolder {
public:
	// Загружает текстуры бонусов из папки по переданному пути
	static void loadBonusTexture(const std::string& folderPath);

	// Даёт ссылку на текстуру для данного бонуса
	static const sf::Texture& getBonusTexture(const BonusType& type);

	// Загружает шрифт
	static void loadFont(const std::string& filename);

	// Загружает текстуру сердца для отображения жизней
	static void loadHeartTexture(const std::string& filename);

	// Геттер для шрифта
	static const sf::Font& getFont();
	
	//Геттер для текстуры сердца
	static const sf::Texture& getHeartTexture();
private:
	static std::map<BonusType, sf::Texture> bonusTextures_;
	static inline std::map<std::string, sf::Font> fonts_;
	static inline sf::Texture heartTexture_;
};