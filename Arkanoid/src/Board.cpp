#include "Board.hpp"
#include <fstream>
#include <sstream>
#include <iostream>


Board::Board() {
    // инициализируем пустую сетку
    indexGrid_.assign(ROWS, std::vector<int>(COLS, -1));
}

sf::Vector2f Board::logicalToPx(const Pos& pos) const {
	return { pos.col * BLOCK_WIDTH,
        OFFSET_Y_TOP + pos.row * BLOCK_HEIGHT };
}

void Board::loadFromFile(const std::string& filename) {
    blocks_.clear();
    for (auto& row : indexGrid_)
        std::fill(row.begin(), row.end(), -1);

    std::ifstream file(filename);
    if (!file) throw std::runtime_error("Cannot open level file: " + filename);

    std::string line;
    int row = 0;
    while (row < ROWS && std::getline(file, line)) {
        std::istringstream ls(line);
        std::string token;
        int col = 0;
        while (col < COLS && (ls >> token)) {
            if (token.front() != '(' || token.back() != ')')
                throw std::runtime_error("Bad syntax: " + token);
            std::istringstream ts(token.substr(1, token.size() - 2));
            int typeInt, colorInt, bonusInt;
            char c1, c2;
            ts >> typeInt >> c1 >> colorInt >> c2 >> bonusInt;
            if (ts.fail() || c1 != ',' || c2 != ',')
                throw std::runtime_error("Bad syntax: " + token);

            if (typeInt >= 0) {
                Pos p{ row,col };
                auto px = logicalToPx(p);
                std::unique_ptr<Block> blk;
                switch (static_cast<BlockType>(typeInt)) {
                case BlockType::Standard:
                    blk = std::make_unique<StandardBlock>(p, px, BLOCK_WIDTH, BLOCK_HEIGHT);
                    break;
                case BlockType::MultiHP:
                    blk = std::make_unique<MultiHPBlock>(p, px, BLOCK_WIDTH, BLOCK_HEIGHT);
                    break;
                case BlockType::Indestructible:
                    blk = std::make_unique<IndestructibleBlock>(p, px, BLOCK_WIDTH, BLOCK_HEIGHT);
                    break;
                case BlockType::SpeedUp:
                    blk = std::make_unique<SpeedUpBlock>(p, px, BLOCK_WIDTH, BLOCK_HEIGHT);
                    break;
                case BlockType::WithBonus:
                    blk = std::make_unique<BonusBlock>(
                        static_cast<BonusType>(bonusInt),
                        p, px, BLOCK_WIDTH, BLOCK_HEIGHT);
                    break;
                default:
                    throw std::runtime_error("Unknown block type");
                }
                indexGrid_[row][col] = (int)blocks_.size();
                blocks_.push_back(std::move(blk));
            }
            ++col;
        }
        if (col != COLS) throw std::runtime_error("Wrong cols in row " + std::to_string(row));
        ++row;
    }
    if (row != ROWS) throw std::runtime_error("Wrong row count: " + std::to_string(row));
    rebuildIndexGrid();
}

void Board::rebuildIndexGrid() {
    // Заполнить -1
    for (auto& row : indexGrid_)
        std::fill(row.begin(), row.end(), -1);

    // Для каждого блока запомним его индекс
    for (int i = 0; i < (int)blocks_.size(); ++i) {
        const Pos p = blocks_[i]->getLogicalPos();
        indexGrid_[p.row][p.col] = i;
    }
}

bool Board::isCleared() const {
    for (const auto& b : blocks_) {
        if (!b->isDestroyed() && !b->isIndestructible())
            return false;
    }
    return true;
}

void Board::reset() {
    // Восстанавливаем каждый блок
    for (auto& b : blocks_)
        b->reset();
    // Восстанавливаем матрицу индексов
    rebuildIndexGrid();
}

void Board::draw(sf::RenderWindow& window) const {
    for (const auto& b : blocks_) {
        if (!b->isDestroyed())
            b->draw(window);
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

            auto& block = blocks_[idx];
            auto blockBounds = block->getBounds();
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

    auto bonus = blocks_[resultIdx]->onHit(ball);
    if (blocks_[resultIdx]->isIndestructible()) {
        return std::nullopt;
    }
    if (blocks_[resultIdx]->isDestroyed())
        indexGrid_[resultLogicalPos.row][resultLogicalPos.col] = -1;

    sf::Vector2f topLeft = blocks_[resultIdx]->getPxPosition();

    // Размер блока (константы Board::BLOCK_WIDTH/HEIGHT)
    constexpr float w = Board::BLOCK_WIDTH, h = Board::BLOCK_HEIGHT;

    // Центр блока — вниз-вправо на половину размера
    sf::Vector2f center{ topLeft.x + w / 2.f, topLeft.y + h / 2.f };

    return std::make_optional(std::make_pair(bonus, center));
}
