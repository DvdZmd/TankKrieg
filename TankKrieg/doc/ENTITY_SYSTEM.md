# ENTITY SYSTEM — TankKrieg

Defines how entities are structured and managed.

---

## 1. Goals

- Represent all objects in the game world
- Keep entities simple and focused
- Allow future scalability

---

## 2. Core Concepts

### Entity

- Represents a game object
- Has position and orientation
- Contains behavior

---

### Types of Entities

Examples:
- PlayerTank
- EnemyTank
- StaticObject (walls, obstacles)

---

## 3. Responsibilities

Entities may handle:

- Movement
- State
- Interaction

---

## 4. Rules

- Entities must not depend on SDL
- Entities must not contain rendering code
- Entities must not manage global systems

---

## 5. World Relationship

- World owns and updates entities
- Entities do not own the world

---

## 6. Update Flow

```text
World.update()
    → Entity.update()