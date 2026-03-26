# ARCHITECTURE — TankKrieg

This document describes the current and target architecture of the engine.

---

## 1. High-Level Structure

TankKrieg is structured into three main layers:

### Platform Layer
Handles:
- SDL initialization
- Window creation
- Input polling
- Timing

### Engine Core
Handles:
- World management
- Camera system
- Entity updates
- Rendering pipeline coordination

### Game Layer
Handles:
- Tank behavior
- Movement logic
- Turret logic
- Game rules

---

## 2. Current State (Transitional)

- `Krieg` currently acts as:
  - Bootstrapper
  - Game loop
  - Coordinator
  - Scene owner

This is temporary and will be refactored.

---

## 3. Target Architecture

### Core Components

- `Game`
  - Owns high-level flow
  - Calls update/render

- `World`
  - Contains entities
  - Responsible for updating them

- `Entity`
  - Base abstraction for all objects

- `Camera2D`
  - Handles world → screen transformation

- `Renderer`
  - Draws entities based on camera output

---

## 4. Data Flow

```text
Input → Game → World → Entities → Camera → Renderer → Screen