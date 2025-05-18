#pragma once

// Типы блоков
enum class BlockType {
	Standard,
	Indestructible,
	SpeedUp,
	MultiHP
};

// Возможные бонусы
enum class BonusType {
	ExpandPaddle,
	CompressPaddle,
	SlowBall,
	FastBall,
	StickyPaddle,
	OneTimeFloor,
	RandomBounce
};

// Цвета блоков
enum class BlockColor {
	Green = 0,
	Red = 1, // MultiHP
	Blue = 2,
	Yellow = 3, // MultiHP
	Orange = 4,
	Gray = 5, // Indestructible
	Purple = 6, // SpeedUP
	Cyan = 7,
	Magenta = 8,
};
