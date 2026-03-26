# GAMEPLAY RULES — TankKrieg

Defines gameplay logic and rules. This is strictly separated from engine systems.

---

## 1. Goals

- Define how the game behaves from the player's perspective
- Keep gameplay logic independent from engine implementation
- Allow iteration without breaking engine systems

---

## 2. Core Gameplay Concept

- Player controls a tank
- Movement: 8 directions
- Turret rotates independently from hull
- Player interacts with world objects (future)

---

## 3. Separation from Engine

Gameplay must:

- NOT depend on SDL
- NOT handle rendering
- NOT manage input directly

Gameplay consumes:

- Input actions
- World state

---

## 4. Tank Behavior

### Movement

- Tank moves in 8 directions
- Movement must be frame-rate independent
- Movement must respect collisions (future)

---

### Turret

- Turret rotation is independent
- Rotation may be:
  - Input-based (keyboard/joystick)
  - Future: mouse aiming

---

### Shooting (Future)

- Tank can fire projectiles
- Projectiles are independent entities

---

## 5. Rules

- Gameplay must be deterministic
- No hidden side effects
- No direct system manipulation (Renderer, SDL, etc.)

---

## 6. Extensibility

Gameplay must allow:

- Enemy AI tanks
- Multiple unit types
- Different movement behaviors

---

## 7. Anti-Patterns

- Mixing gameplay with rendering
- Hardcoding input inside entities
- Embedding engine logic into gameplay classes