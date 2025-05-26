#pragma once
#include <SFML/Graphics.hpp>

// circle = (center, radius), rect = axis‑aligned rectangle (FloatRect)
// Возвращает {normal, penetrationDepth}; penetrationDepth > 0 означает пересечение.
static std::pair<sf::Vector2f, float> computeCollisionCircleAABB(
    const sf::Vector2f& C, float radius, const sf::FloatRect& r)
{
    // 1. Крайние координаты
    float left = r.left;
    float right = r.left + r.width;
    float top = r.top;
    float bottom = r.top + r.height;

    // 2. Ближайшая к центру точка P на прямоугольнике
    float px = std::clamp(C.x, left, right);
    float py = std::clamp(C.y, top, bottom);
    sf::Vector2f P{ px, py };

    // 3. Вектор от P к центру
    sf::Vector2f D = C - P;
    float dist = std::hypot(D.x, D.y);

    sf::Vector2f normal;
    float penetration;
    if (dist == 0.f) {
        // Случай центр внутри rect — берём кратчайшую грань
        float dl = std::abs(C.x - left);
        float dr = std::abs(right - C.x);
        float dt = std::abs(C.y - top);
        float db = std::abs(bottom - C.y);
        float m = std::min({ dl, dr, dt, db });
        if (m == dl)      normal = { -1.f,  0.f };
        else if (m == dr) normal = { 1.f,  0.f };
        else if (m == dt) normal = { 0.f, -1.f };
        else              normal = { 0.f,  1.f };
        penetration = radius + m;
    }
    else {
        normal = { D.x / dist, D.y / dist };
        penetration = radius - dist;
    }

    return { normal, penetration };
}