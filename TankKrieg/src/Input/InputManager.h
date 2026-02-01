#pragma once
#include <SDL3/SDL.h>
#include "Math/Vector2.h"

class InputManager {
public:
    void Initialize();
    void Update();
    void Shutdown();

    Vector2 GetMovementVector() const;
    bool QuitRequested() const;

private:
    Vector2 keyboardMove{};
    Vector2 gamepadMove{};
    Vector2 finalMove{};

    bool quit = false;
    SDL_Gamepad* gamepad = nullptr;

    void ProcessEvent(const SDL_Event& e);
    void UpdateKeyboardState();
    void UpdateGamepadState();
    void ComposeFinalMove();
};
