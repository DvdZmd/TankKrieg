# INPUT SYSTEM — TankKrieg

Defines how input is handled in the engine.

---

## 1. Goals

- Decouple input from SDL
- Provide a unified interface for keyboard and joystick
- Support both discrete (press) and continuous (hold) actions
- Be deterministic and frame-consistent

---

## 2. Architecture

### Layers

1. Platform Input (SDL)
   - Polls raw input events

2. Input System
   - Translates raw input into actions

3. Game Layer
   - Consumes actions, not raw input

---

## 3. Core Concepts

### Input State

Each frame must track:

- Key/Button pressed
- Key/Button released
- Key/Button held

---

### Actions

Input must be mapped to actions:

Examples:
- MoveUp
- MoveDown
- MoveLeft
- MoveRight
- RotateTurret
- Fire

---

## 4. Rules

- No SDL input calls inside gameplay code
- Gameplay must only consume high-level actions
- Input mappings must be configurable (future)

---

## 5. Continuous vs Discrete

- Press → single event
- Hold → continuous behavior

Both must be supported explicitly.

---

## 6. Future Considerations

- Gamepad support (analog sticks)
- Dead zones and smoothing
- Input remapping
- Network input (optional future)