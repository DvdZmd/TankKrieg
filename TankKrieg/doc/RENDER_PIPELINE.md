
---

# 📁 `/docs/RENDER_PIPELINE.md`

```markdown
# RENDER PIPELINE — TankKrieg

Defines how rendering works in the engine.

---

## 1. Coordinate Spaces

- World Space (game logic)
- Screen Space (rendering)

---

## 2. Isometric Transformation

The engine uses an isometric projection.

Rules:
- X and Y world coordinates are transformed into isometric screen coordinates
- Z-order must be handled carefully (important for occlusion)

---

## 3. Rendering Flow

1. Entities exist in world space
2. Camera transforms world → screen
3. Renderer draws entities

---

## 4. Z-Ordering

- Objects further "down" the screen must render on top
- Sorting strategy must be consistent

---

## 5. Debug Rendering

- Current tank rendering is debug-only (primitive shapes)
- This must be replaceable with sprite rendering

---

## 6. Future Goals

- Sprite system
- Layering (ground, objects, units)
- Occlusion handling (e.g., walls in front/behind tank)