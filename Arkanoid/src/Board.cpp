#include "Board.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

void Board::generateDefault() {
	blocks_.clear();

	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
            Pos pos{ row, col };
            // Конструктор: logicalPos, pxPos, type, width, height, hp, бонус, speedMul
            blocks_.emplace_back(
                pos,
                logicalToPx(pos),
				BlockType::Standard,
                BlockColor::Green,
                BLOCK_WIDTH,
                BLOCK_HEIGHT
			);
		} 
	}
    rebuildIndexGrid();
}

sf::Vector2f Board::logicalToPx(const Pos& pos) const {
	return { pos.col * BLOCK_WIDTH, OFFSET_Y_TOP + pos.row * BLOCK_HEIGHT };
}

void Board::loadFromFile(const std::string& filename) {
    blocks_.clear();
    indexGrid_.assign(ROWS, std::vector<int>(COLS, -1));

    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open level file: " + filename);
    }

    std::string line;
    int row = 0;
    while (row < ROWS && std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string token;
        int col = 0;

        // читаем токены
        while (col < COLS && (lineStream >> token)) {
            if (token.size() < 7 || token.front() != '(' || token.back() != ')') {
                throw std::runtime_error("Bad block syntax: " + token);
            }
            std::istringstream tokenStream(token);

            int typeInt, colorInt, bonusInt;
            char ch;
            tokenStream >> std::ws >> ch;            // '('
            tokenStream >> std::ws >> typeInt;
            tokenStream >> std::ws >> ch;            // ','
            tokenStream >> std::ws >> colorInt;
            tokenStream >> std::ws >> ch;            // ','
            tokenStream >> std::ws >> bonusInt;
            tokenStream >> std::ws >> ch;            // ')'
            if (tokenStream.fail()) {
                throw std::runtime_error("Bad block syntax: " + token);
            }
            if (typeInt < 0) {
                col++;
                continue;
            }

            BlockType type = static_cast<BlockType>(typeInt);
            BlockColor color = static_cast<BlockColor>(colorInt);
            std::optional<BonusType> bonus = (bonusInt >= 0 
                ? std::optional<BonusType>{static_cast<BonusType>(bonusInt)}
                : std::nullopt);

            Pos pos{ row, col };
            sf::Vector2f pxPos = logicalToPx(pos);
            int hp = (type == BlockType::MultiHP ? 3 : 1);

            blocks_.emplace_back(
                pos,
                pxPos,
                type,
                color,
                BLOCK_WIDTH,
                BLOCK_HEIGHT,
                hp,
                bonus
            );
            col++;
        }

        if (col != COLS) {
            throw std::runtime_error("Row " + std::to_string(row) +
                " has wrong block count: " + std::to_string(col));
        }
        row++;
    }

    if (row != ROWS) {
        throw std::runtime_error("Level file has wrong row count: " + std::to_string(row));
    }

    rebuildIndexGrid();
}

void Board::rebuildIndexGrid() {
    // Заполнить -1
    for (auto& row : indexGrid_)
        std::fill(row.begin(), row.end(), -1);

    // Для каждого блока запомним его индекс
    for (int i = 0; i < (int)blocks_.size(); ++i) {
        const Pos p = blocks_[i].getLogicalPos();
        indexGrid_[p.row][p.col] = i;
    }
}

bool Board::isCleared() const {
    for (const auto& b : blocks_) {
        if (!b.isDestroyed() && !b.isIndestructible())
            return false;
    }
    return true;
}

void Board::reset() {
    // Восстанавливаем каждый блок
    for (auto& b : blocks_)
        b.reset();
    // Восстанавливаем матрицу индексов
    rebuildIndexGrid();
}

void Board::draw(sf::RenderWindow& window) const {
    for (const auto& b : blocks_) {
        if (!b.isDestroyed())
            b.draw(window);
    }
}

std::optional<std::pair<std::optional<BonusType>, sf::Vector2f>> Board::handleBallCollision(Ball& ball) {
    const sf::Vector2f ballCenter = ball.getPosition();
    const float ballRadius = Ball::RADIUS;

    if (ballCenter.y < OFFSET_Y_TOP) {
        return std::nullopt;
    }

    int ballCol = static_cast<int>(ballCenter.x / BLOCK_WIDTH);
    int ballRow = static_cast<int>((ballCenter.y - OFFSET_Y_TOP) / BLOCK_HEIGHT);

    

    // Будем искать лучшего кандидата для отскока
    int resultIdx = -1;
    sf::Vector2f resultNormal = { 0.f, 0.f };
    float resultPenetration = std::numeric_limits<float>::infinity();
    Pos resultLogicalPos = { 0, 0 };

    // Перебираем соседние 3×3 ячейки
    for (int dRow = -1; dRow <= 1; dRow++) {
        for (int dCol = -1; dCol <= 1; dCol++) {
            int checkRow = ballRow + dRow, checkCol = ballCol + dCol;
            if (checkRow < 0 || checkRow >= ROWS
                || checkCol < 0 || checkCol >= COLS)
            {
                continue;
            }

            int idx = indexGrid_[checkRow][checkCol];
            if (idx < 0) {
                continue;
            }

            Block& block = blocks_[idx];
            sf::FloatRect blockBounds = block.getBounds();
            if (!ball.getBounds().intersects(blockBounds))
                continue;
            auto [normal, pen] = computeCollisionCircleAABB(
                ballCenter, ballRadius, blockBounds);
            if (pen > 0.f && pen < resultPenetration) {
                resultPenetration = pen;
                resultNormal = normal;
                resultIdx = idx;
                resultLogicalPos = { checkRow, checkCol };
            }
        }
    }
    if (resultIdx < 0) {
        return std::nullopt;
    }

    ball.reflect(resultNormal);
    ball.setPosition(ballCenter + resultNormal * (resultPenetration + 0.1f));

    Block& block = blocks_[resultIdx];
    if (block.hit(ball)) {
        indexGrid_[resultLogicalPos.row][resultLogicalPos.col] = -1;
        std::optional<BonusType> bonus = block.getBonus();
        if (bonus.has_value()) {
            return std::make_optional(std::make_pair(bonus, block.getPxPosition()));
        }
        else {
            return std::make_optional(std::make_pair(std::nullopt, block.getPxPosition()));
        }
    }
    return std::nullopt;
}
