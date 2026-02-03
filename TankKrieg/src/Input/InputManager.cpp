#include "InputManager.h"
#include <cmath>

static float ApplyDeadzone(float v, float dz) {
    return (std::fabs(v) < dz) ? 0.0f : v;
}

void InputManager::Initialize() {
    // SDL must be initialized before this.
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

    // 1) mover now -> prev al inicio del frame
    upPrev = upNow; downPrev = downNow; leftPrev = leftNow; rightPrev = rightNow;
    dpadUpPrev = dpadUpNow; dpadDownPrev = dpadDownNow; dpadLeftPrev = dpadLeftNow; dpadRightPrev = dpadRightNow;

    // 2) procesar eventos
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        ProcessEvent(e);
    }

    UpdateKeyboardState();
    UpdateGamepadState();
    ComposeFinalMove();

	// 4) calcular step edge-trigger y cachearlo TODO (mejor en SetCursorStep?)
    cursorStep = { 0, 0 };

    if (upNow && !upPrev) cursorStep.y -= 1;
    if (downNow && !downPrev) cursorStep.y += 1;
    if (leftNow && !leftPrev) cursorStep.x -= 1;
    if (rightNow && !rightPrev) cursorStep.x += 1;

    // si querés que el D-pad también mueva el cursor:
    if (dpadUpNow && !dpadUpPrev) cursorStep.y -= 1;
    if (dpadDownNow && !dpadDownPrev) cursorStep.y += 1;
    if (dpadLeftNow && !dpadLeftPrev) cursorStep.x -= 1;
    if (dpadRightNow && !dpadRightPrev) cursorStep.x += 1;
}

void InputManager::ProcessEvent(const SDL_Event& e) {
    if (e.type == SDL_EVENT_QUIT) {
        quit = true;
    }

    // Optional (future): handle controller connect/disconnect events here.
}

void InputManager::UpdateKeyboardState() {
    keyboardMove = {};

    const bool* k = SDL_GetKeyboardState(nullptr);

    upNow = k[SDL_SCANCODE_UP];
    downNow = k[SDL_SCANCODE_DOWN];
    leftNow = k[SDL_SCANCODE_LEFT];
    rightNow = k[SDL_SCANCODE_RIGHT];

    if (upNow)    keyboardMove.y -= 1.0f;
    if (downNow)  keyboardMove.y += 1.0f;
    if (leftNow)  keyboardMove.x -= 1.0f;
    if (rightNow) keyboardMove.x += 1.0f;

    if (keyboardMove.Length() > 0.0f) {
        keyboardMove.Normalize();
    }
}

void InputManager::UpdateGamepadState() {
    gamepadMove = {};

    // Reset dpad state each frame
    dpadUpNow = dpadDownNow = dpadLeftNow = dpadRightNow = false;

    if (!gamepad) return;

    // Left stick for continuous movement
    float x = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTX) / 32767.0f;
    float y = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTY) / 32767.0f;

    x = ApplyDeadzone(x, 0.2f);
    y = ApplyDeadzone(y, 0.2f);

    gamepadMove = { x, y };
    if (gamepadMove.Length() > 1.0f) gamepadMove.Normalize();

    // D-pad for discrete cursor stepping (optional but nice)
    dpadUpNow = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP);
    dpadDownNow = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
    dpadLeftNow = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT);
    dpadRightNow = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
}

void InputManager::ComposeFinalMove() {
    // Priority: if stick is moved, use it; otherwise keyboard.
    finalMove = (gamepadMove.Length() > 0.0f) ? gamepadMove : keyboardMove;
}

Vector2 InputManager::GetMovementVector() const {
    return finalMove;
}

Int2 InputManager::GetCursorStep() const { return cursorStep; }

bool InputManager::QuitRequested() const {
    return quit;
}
