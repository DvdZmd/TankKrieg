# ENGINE RULES — TankKrieg

This document defines the non-negotiable engineering rules for the TankKrieg engine.

All contributors (human or AI) MUST follow these rules.

---

## 1. Architecture Principles

- No God Objects
- Systems must have a single responsibility
- Prefer composition over inheritance
- Explicit dependencies over hidden coupling

---

## 2. Separation of Concerns

The engine must be divided into clear layers:

- Platform Layer (SDL, input, window, timing)
- Engine Core (world, systems, camera)
- Game Layer (tank logic, gameplay rules)

Game logic MUST NOT depend directly on SDL.

---

## 3. Game Loop

- The game loop must be deterministic
- Game logic must be executed through a `Tick(deltaTime)` or equivalent
- Rendering must be separated from simulation

---

## 4. Rendering Rules

- Rendering must be stateless
- No game logic inside render code
- World space must be transformed to screen space via camera systems

---

## 5. Input System

- Input must be abstracted from SDL
- No direct SDL input calls inside gameplay logic
- Support for keyboard and joystick must be unified

---

## 6. Memory & Ownership

- Prefer RAII
- Avoid raw pointers unless strictly necessary
- Use smart pointers with clear ownership semantics

---

## 7. Filesystem & Assets

- No hardcoded OS-specific paths
- Asset loading must be abstracted
- Prepare for future web compatibility (Emscripten)

---

## 8. Code Quality

- Code must be readable and maintainable
- Avoid clever hacks
- Use clear naming (no abbreviations unless standard)

---

## 9. Scalability Awareness

Even if the current scope is small:

- Design systems that can scale
- Avoid tightly coupled logic
- Prepare for multiple entities and larger maps

---

## 10. Strict Prohibitions

- No mixing of responsibilities in the same class
- No direct SDL calls in gameplay logic
- No quick-and-dirty patches
- No hidden side effects

---

## 11. AI Agent Behavior

AI-generated code MUST:

- Follow all rules in this document
- Prefer architecture over shortcuts
- Suggest improvements when detecting bad design