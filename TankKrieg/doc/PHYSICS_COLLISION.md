# PHYSICS & COLLISION — TankKrieg

Defines how movement and collision will work.

---

## 1. Goals

- Provide predictable and simple collision system
- Support tank movement constraints
- Be lightweight and deterministic

---

## 2. Scope (Current)

- Basic collision (tank vs obstacles)
- No complex physics simulation
- No rigid body system

---

## 3. Collision Types

### Static Colliders
- Walls
- Terrain obstacles

### Dynamic Colliders
- Tanks
- Future entities

---

## 4. Collision Model

Initial approach:

- Axis-Aligned Bounding Boxes (AABB)
- Simple overlap detection

---

## 5. Movement Resolution

- Movement is attempted
- Collision is checked
- If collision occurs:
  - Movement is adjusted or blocked

---

## 6. Rules

- Physics must be deterministic
- No physics logic inside rendering
- No SDL dependency

---

## 7. Future Evolution

- Spatial partitioning (quadtrees, grids)
- More precise collision shapes
- Pathfinding integration

---

## 8. Anti-Patterns

- Mixing collision with rendering
- Embedding physics inside entities
- Non-deterministic movement