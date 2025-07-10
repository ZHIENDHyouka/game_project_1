#include "Game.h"

Game::Game() : _is_running(true), _current_scene(nullptr),
               _window(nullptr), _renderer(nullptr),
               _window_width(800), _window_height(600), _fps(60),
               _frame_time(1000 / _fps), _delta_time(0.0f)
{
}

Game::~Game()
{
    clean();
}

Game &Game::Instance()
{
    static Game instance;
    return instance;
}

void Game::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        // Handle initialization error
        _is_running = false;
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init Error: %s", SDL_GetError());
        return;
    }
    _window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, _window_width, _window_height, SDL_WINDOW_SHOWN);
    if (_window == nullptr)
    {
        // Handle window creation error
        _is_running = false;
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow Error: %s", SDL_GetError());
        return;
    }

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (_renderer == nullptr)
    {
        // Handle renderer creation error
        _is_running = false;
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateRenderer Error: %s", SDL_GetError());
        return;
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        // Handle image initialization error
        _is_running = false;
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IMG_Init Error: %s", IMG_GetError());
        return;
    }

    _current_scene = new SceneMain();
    _current_scene->init();
}

void Game::run()
{
    while (_is_running)
    {
        auto start_time = SDL_GetTicks();
        SDL_Event event;
        HandleEvents(&event);
        update();
        render();
        auto end_time = SDL_GetTicks();
        auto diff = end_time - start_time;
        if (diff < _frame_time)
        {
            SDL_Delay(_frame_time - diff);
            _delta_time = static_cast<float>(_frame_time) / 1000.0f; // Convert milliseconds to seconds
        }
        else
        {
            _delta_time = static_cast<float>(diff) / 1000.0f; // Convert milliseconds to seconds
        }
    }
}

void Game::clean()
{
    if (_current_scene != nullptr)
    {
        _current_scene->clean();
        delete _current_scene;
        _current_scene = nullptr;
    }
    IMG_Quit();
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void Game::change_scene(Scene *new_scene)
{
    if (_current_scene)
    {
        _current_scene->clean();
        delete _current_scene;
    }
    _current_scene = new_scene;
    _current_scene->init();
}

void Game::HandleEvents(SDL_Event *event)
{

    while (SDL_PollEvent(event))
    {
        if (event->type == SDL_QUIT)
        {
            _is_running = false;
            break;
        }
        _current_scene->handleEvents(*event);
    }
}

void Game::update()
{
    _current_scene->update(_delta_time);
}

void Game::render()
{
    // clear
    SDL_RenderClear(_renderer);
    _current_scene->render();
    // present
    SDL_RenderPresent(_renderer);
}
