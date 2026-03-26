# SPRITE SYSTEM — TankKrieg

Defines how sprites will be integrated.

---

## 1. Goals

- Replace debug rendering
- Support 2D isometric sprites
- Keep rendering decoupled from entities

---

## 2. Core Concept

Entities must NOT render themselves.

Instead:

- Entities provide render data
- Renderer draws based on that data

---

## 3. Sprite Representation

A sprite should include:

- Texture reference
- Source rectangle
- Size
- Anchor point

---

## 4. Rules

- No SDL calls inside entities
- No texture loading inside entities
- Renderer handles all drawing

---

## 5. Tank Specific

Tank must be split into:

- Hull sprite
- Turret sprite

Both must be independently rotatable.

---

## 6. Rendering Order

- Must respect isometric depth
- Sorting must be consistent

---

## 7. Future Features

- Animation system
- Sprite batching
- Texture atlas