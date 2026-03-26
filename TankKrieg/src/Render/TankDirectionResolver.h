#pragma once

class TankDirectionResolver
{
public:
    static constexpr int kDirectionCount = 8;

    static int AngleToDirectionIndex(float radians);
};
