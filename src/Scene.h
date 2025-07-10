#ifndef _SCENE_H_
#define _SCENE_H_
#include "Object.h"
#include <SDL.h>
#include <SDL_image.h>
class Scene
{
private:
    int a;
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void init() = 0;
    virtual void update(float delta_time) = 0;
    virtual void render() = 0;
    virtual void clean() = 0;
    virtual void handleEvents(const SDL_Event &event) = 0;
};

#endif