#pragma once
#include <SDL3/SDL.h>
#include "Math/Vector2.h"
#include "Math/Int2.h"

class InputManager {
public:
    void Initialize();
    void Update();
    void Shutdown();

    // Continuous movement (keyboard arrows / left stick)
    Vector2 GetMovementVector() const;
    // Discrete cursor movement (one grid step per press)
    Int2 GetCursorStep() const;

    bool QuitRequested() const;

private:
    // Continuous movement sources
    Vector2 keyboardMove{};
    Vector2 gamepadMove{};
    Vector2 finalMove{};

    Int2 cursorStep{};

    // Discrete cursor (edge-trigger) state
    bool upNow = false, downNow = false, leftNow = false, rightNow = false;
    bool upPrev = false, downPrev = false, leftPrev = false, rightPrev = false;

    // Optional: D-pad edge state (if you want cursor with gamepad)
    bool dpadUpNow = false, dpadDownNow = false, dpadLeftNow = false, dpadRightNow = false;
    bool dpadUpPrev = false, dpadDownPrev = false, dpadLeftPrev = false, dpadRightPrev = false;

    bool quit = false;
    SDL_Gamepad* gamepad = nullptr;

    void ProcessEvent(const SDL_Event& e);
    void UpdateKeyboardState();
    void UpdateGamepadState();
    void ComposeFinalMove();
};
