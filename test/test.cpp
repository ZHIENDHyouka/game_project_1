#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
int main(int, char **)
{
    std::cout << "Hello, World!" << std::endl;
    SDL_Log("程序启动");
    // SDL初始化
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // 创建窗口
    SDL_Window *window = SDL_CreateWindow("Hello World!", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    // 创建渲染器
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // SDL_Image_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG))
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // LODA IMAGE
    SDL_Texture *texture = IMG_LoadTexture(renderer, "assets/assets/image/bg.png");

    // SDL_MIXER
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cerr << "SDL_Mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    // read music
    Mix_Music *music = Mix_LoadMUS("assets/assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    // 播放
    Mix_PlayMusic(music, -1);

    // TTF_Init
    if (TTF_Init() != 0)
    {
        std::cerr << "SDL_TTF Error: " << TTF_GetError() << std::endl;
        return 1;
    }
    // 创建字体
    TTF_Font *font = TTF_OpenFont("assets/assets/font/VonwaonBitmap-12px.ttf", 24);
    SDL_Color textColor = {255, 255, 255, 255}; // 白色
    SDL_Surface *textSurface = TTF_RenderUTF8_Solid(font, "Hello天天, SDL2!", textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    // 渲染循环
    while (true)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                break;
            }
        }

        // 清屏
        SDL_RenderClear(renderer);
        // 画一个长方形
        SDL_Rect rect = {100, 100, 200, 200};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        // draw text
        SDL_Rect textRect = {150, 150, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        // draw a texture
        SDL_Rect dstRect = {200, 200, 300, 400};
        SDL_RenderCopy(renderer, texture, NULL, &dstRect);
        // 更新屏幕
        SDL_RenderPresent(renderer);
    }

    // 清理并退出
    SDL_DestroyTexture(texture);
    IMG_Quit();

    Mix_FreeMusic(music);
    Mix_CloseAudio();
    Mix_Quit();

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}