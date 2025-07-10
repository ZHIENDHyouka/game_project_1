#ifndef _GAME_H_
#define _GAME_H_
#include "Scene.h"
#include "SceneMain.h"
#include <SDL.h>

class Game
{
private:
    bool _is_running;
    Scene *_current_scene;
    SDL_Window *_window;
    SDL_Renderer *_renderer;
    int _window_width;
    int _window_height;
    int _fps;
    Uint32 _frame_time;
    float _delta_time;
    Game();
    Game(Game &&) = delete;
    Game &operator=(Game &&) = delete;
    Game(const Game &) = delete;            // Disable copy constructor
    Game &operator=(const Game &) = delete; // Disable copy assignment operator

public:
    ~Game();
    static Game &Instance();

    void init();
    void run();
    void clean();
    void change_scene(Scene *new_scene);

    void HandleEvents(SDL_Event *event);
    void update();
    void render();

    inline SDL_Renderer *GetRenderer() const { return _renderer; }
    inline SDL_Window *GetWindow() const { return _window; }
    inline int GetWindowWidth() const { return _window_width; }
    inline int GetWindowHeight() const { return _window_height; }
};

#endif