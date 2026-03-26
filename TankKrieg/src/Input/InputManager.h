#pragma once
#include <SDL3/SDL.h>
#include "Math/Vector2.h"
#include "Math/Int2.h"

class InputManager {
public:
    /**
     * @brief Initialize input devices managed by the game.
     */
    void Initialize();
    /**
     * @brief Poll SDL input, refresh device state, and compose frame input values.
     * @param dt Elapsed time in seconds since the previous frame.
     */
    void Update(float dt);
    /**
     * @brief Release any input devices opened by the manager.
     */
    void Shutdown();

    /**
     * @brief Return the current turret or aiming direction.
     * @return The final aim vector for the current frame.
     */
    Vector2 GetAimVector() const { return aim; }
    /**
     * @brief Return the continuous movement direction requested this frame.
     * @return The normalized movement vector assembled from active input sources.
     */
    Vector2 GetMovementVector() const;// Continuous movement (keyboard arrows / left stick)
    /**
     * @brief Return the discrete cursor step requested this frame.
     * @return A single grid step derived from keyboard, D-pad, or analog repeat input.
     */
    Int2 GetCursorStep() const;// Discrete cursor movement (one grid step per press)

    /**
     * @brief Report whether SDL requested application shutdown during this frame.
     * @return True when a quit event was received; otherwise false.
     */
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

    /**
     * @brief Handle a single SDL event relevant to the input manager.
     * @param e Event polled from SDL for the current frame.
     */
    void ProcessEvent(const SDL_Event& e);
    /**
     * @brief Read keyboard state and update keyboard-driven movement inputs.
     */
    void UpdateKeyboardState();
    /**
     * @brief Read connected gamepad state and update analog and D-pad inputs.
     */
    void UpdateGamepadState();
    /**
     * @brief Merge per-device input state into the frame's final control values.
     */
    void ComposeFinalMove();
};
