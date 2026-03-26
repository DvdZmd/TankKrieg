# CAMERA SYSTEM — TankKrieg

Defines how the camera works.

---

## 1. Responsibilities

- Transform world coordinates into screen coordinates
- Control viewport position
- Follow targets (e.g., player tank)

---

## 2. Core Concepts

### Camera Position

- Represents the center of the view in world space

---

### Transform

Camera must provide:

- World → Screen transformation
- Screen → World (optional, for input)

---

## 3. Behavior

### Follow Mode

- Camera follows player entity
- Smooth movement (optional)

---

### Static Mode (debug)

- Fixed camera position

---

## 4. Rules

- Camera must not contain gameplay logic
- Camera must not depend on rendering implementation
- Camera must be reusable

---

## 5. Isometric Considerations

- Camera must work with isometric projection
- Transform must be consistent with render pipeline

---

## 6. Future Features

- Smooth interpolation
- Zoom
- Camera bounds (map limits)
- Screen shake