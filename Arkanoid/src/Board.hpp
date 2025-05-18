#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "entities/Block.hpp"
#include "entities/Ball.hpp"
#include "Pos.hpp"
#include "EnumTypes.hpp"
#include <optional>
#include "CollisionUtils.hpp"


/// Управление коллекцией блоков (уровнем)
class Board {
public:
    // Размер одного блока
    static constexpr float BLOCK_WIDTH = 50.f;
    static constexpr float BLOCK_HEIGHT = 50.f;

    // Базовые тступы от краёв окна
    static constexpr float OFFSET_Y_TOP = 100.f;

    // Размер логической сетки
    // Максимально вазможные значения с учётом отсупа снизу в высоту одного блока
    static constexpr int ROWS = 12;
    static constexpr int COLS = 12;


    Board() {
        // инициализируем пустую матрицу
        indexGrid_.assign(ROWS, std::vector<int>(COLS, -1));
    }

    /// Заполнить доску стандартным макетом (например, 12×8 зелёных блоков)
    void generateDefault();

    /// Загрузить уровень из текстового файла (матрица целых чисел)
    void loadFromFile(const std::string& filename);

    /// Перерисовать все блоки на экран
    void draw(sf::RenderWindow& window) const;

    /**
     *  Проверяет столкновение мяча со всеми блоками:
     *   - если было пересечение — вызвать block.hit(), ball.reverseY()
     *   - возвращает optional пару - тип бонуса и его начальное положение
     */
    std::optional<std::pair<std::optional<BonusType>, sf::Vector2f>> handleBallCollision(Ball& ball);

    /// Все ли блоки уничтожены?
    bool isCleared() const;

    /// Сбросить уровень (восстановить hp, флаги destroyed, и обновить indexGrid_)
    void reset();

private:
    std::vector<std::vector<int>> indexGrid_;  // размер rows×cols: -1 или индекс в blocks_
    std::vector<Block> blocks_;

    // Перевод логической позиции → пиксели
    sf::Vector2f logicalToPx(const Pos& pos) const;

    // Обновить indexGrid_ после загрузки или reset()
    void rebuildIndexGrid();
};
