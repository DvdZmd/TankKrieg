#include <cmath>

#include "Render/TankDirectionResolver.h"

int TankDirectionResolver::AngleToDirectionIndex(float radians)
{
    constexpr float kTau = 6.28318530718f;
    constexpr float kSectorSize = kTau / static_cast<float>(kDirectionCount);

    float wrapped = std::fmod(radians, kTau);
    if (wrapped < 0.0f)
    {
        wrapped += kTau;
    }

    const float shifted = wrapped + (kSectorSize * 0.5f);
    return static_cast<int>(std::floor(shifted / kSectorSize)) % kDirectionCount;
}
