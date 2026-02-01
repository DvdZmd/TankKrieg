#include "InputManager.h"
#include <algorithm>
#include <cmath>

static float ApplyDeadzone(float v, float dz) {
    if (std::fabs(v) < dz) return 0.0f;
    return v;
}

void InputManager::Initialize() {
    if (SDL_HasGamepad()) {
        gamepad = SDL_OpenGamepad(0);
    }
}

void InputManager::Shutdown() {
    if (gamepad) {
        SDL_CloseGamepad(gamepad);
        gamepad = nullptr;
    }
}

void InputManager::Update() {
    quit = false;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        ProcessEvent(e);
    }

    UpdateKeyboardState();
    UpdateGamepadState();
    ComposeFinalMove();
}

void InputManager::ProcessEvent(const SDL_Event& e) {
    if (e.type == SDL_EVENT_QUIT) {
        quit = true;
    }

    // (Opcional pro) conectar/desconectar gamepad por eventos más adelante
}

void InputManager::UpdateKeyboardState() {
    keyboardMove = {};

    const bool up = SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_UP];
    const bool down = SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_DOWN];
    const bool left = SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_LEFT];
    const bool right = SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_RIGHT];

    if (up)    keyboardMove.y -= 1.0f;
    if (down)  keyboardMove.y += 1.0f;
    if (left)  keyboardMove.x -= 1.0f;
    if (right) keyboardMove.x += 1.0f;

    if (keyboardMove.Length() > 0.0f) keyboardMove.Normalize();
}

void InputManager::UpdateGamepadState() {
    gamepadMove = {};
    if (!gamepad) return;

    float x = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTX) / 32767.0f;
    float y = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTY) / 32767.0f;

    x = ApplyDeadzone(x, 0.2f);
    y = ApplyDeadzone(y, 0.2f);

    gamepadMove = { x, y };
    if (gamepadMove.Length() > 1.0f) gamepadMove.Normalize();
}

void InputManager::ComposeFinalMove() {
    // Prioridad “pro”: si hay gamepad, domina; si no, teclado.
    finalMove = (gamepadMove.Length() > 0.0f) ? gamepadMove : keyboardMove;
}

Vector2 InputManager::GetMovementVector() const {
    return finalMove;
}

bool InputManager::QuitRequested() const {
    return quit;
}
