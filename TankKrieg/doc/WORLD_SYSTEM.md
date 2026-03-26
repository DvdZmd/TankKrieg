
---

# 📁 `/docs/WORLD_SYSTEM.md`

```markdown
# WORLD SYSTEM — TankKrieg

Defines the role of the World.

---

## 1. Responsibilities

- Own and manage entities
- Update all entities
- Act as the simulation container

---

## 2. Rules

- World must not contain gameplay logic
- World must not handle rendering directly
- World must not depend on SDL

---

## 3. Entity Management

World must:

- Add entities
- Remove entities
- Iterate entities safely

---

## 4. Update Flow

- World updates all entities each frame
- Order must be deterministic

---

## 5. Future Considerations

- Spatial partitioning
- Collision systems
- Event systems