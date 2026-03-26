#pragma once

#include <SDL3/SDL_events.h>

struct RenderContext;

class IScene
{
public:
    virtual ~IScene() = default;

    virtual void OnEnter() {}
    virtual void OnExit() {}

    virtual void HandleEvent(const SDL_Event& event) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render(const RenderContext& renderContext) = 0;
    virtual bool WantsToQuit() const = 0;
};
