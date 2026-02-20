#pragma once
#include <SDL3/SDL.h>
#include "Math/Vector2.h"
#include "Math/Int2.h"

class InputManager {
public:
    void Initialize();
    void Update(float dt);
    void Shutdown();

    Vector2 GetAimVector() const { return aim; }
    Vector2 GetMovementVector() const;// Continuous movement (keyboard arrows / left stick)
    Int2 GetCursorStep() const;// Discrete cursor movement (one grid step per press)

    bool QuitRequested() const;


private:
    // Continuous movement sources
    Vector2 keyboardMove{};
    Vector2 gamepadMove{};
    Vector2 finalMove{};

    Vector2 gamepadAim{};   // right stick
    Vector2 aim{};          // final aim vector (right stick or mouse in future)

    Int2 cursorStep{};

    float cursorRepeatTimer = 0.0f;
    float cursorRepeatDelay = 0.12f; // 120ms between repeated steps while holding stick
    Int2 analogStep{};

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
