#include "Unit.h"

void Unit::SetDirection(const Vector2& dir) {
    direction = dir;
}

void Unit::Update(float dt) {
    position += direction * speed * dt;
}
