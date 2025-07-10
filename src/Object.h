#ifndef _OBJECT_H_
#define _OBJECT_H_
#include <SDL.h>

struct Player
{
    SDL_Texture* texture; // Pointer to the texture for the player
    SDL_FPoint position; // Position of the player in floating-point coordinates
    int width; // Width of the player
    int height; // Height of the player
    float speed = 500.0f; // Speed of the player movement
    Uint32 cool_down = 300; // Cooldown time for player actions in milliseconds
    Uint32 last_shot = 0; // Timestamp of the last shot taken by the player
    int current_health = 5;
    Player() = default; // Default constructor
    Player(SDL_Texture* tex, SDL_FPoint pos, int w, int h, float s)
        : texture(tex), position(pos), width(w), height(h), speed(s)
    {
    }
    SDL_Rect GetRect() const
    {
        return  SDL_Rect(static_cast<int>(position.x), static_cast<int>(position.y),width, height);
    }
};

struct Bullet
{
    SDL_Texture* texture; // Pointer to the texture for the bullet
    SDL_FPoint position; // Position of the bullet in floating-point coordinates
    int width; // Width of the bullet
    int height; // Height of the bullet
    float speed = 800.0f; // Speed of the bullet movement
    int damage = 1;
    Bullet() = default; // Default constructor
    Bullet(SDL_Texture* tex, SDL_FPoint pos, int w, int h, float s)
        : texture(tex), position(pos), width(w), height(h), speed(s)
    {
    }

    Bullet(const Bullet& other)
        : texture(other.texture), position(other.position), width(other.width), height(other.height), speed(other.speed)
    {
    }
    SDL_Rect GetRect() const
    {
        return  SDL_Rect(static_cast<int>(position.x), static_cast<int>(position.y),width, height);
    }
};

struct Enemy
{
    SDL_Texture* texture; // Pointer to the texture for the bullet
    SDL_FPoint position; // Position of the bullet in floating-point coordinates
    int width; // Width of the bullet
    int height; // Height of the bullet
    int current_health = 2;
    float speed = 500.0f; // Speed of the bullet movement
    Uint32 cool_down = 300;
    Uint32 last_shot = 0;
    SDL_Rect GetRect() const
    {
        return  SDL_Rect(static_cast<int>(position.x), static_cast<int>(position.y),width, height);
    }
};

struct EnemyBullet
{
    SDL_Texture* texture;
    SDL_FPoint position;
    SDL_FPoint direction;
    int width;
    int height;
    float speed = 800.0f; // Speed of the bullet movement
    int damage = 1;
    SDL_Rect GetRect() const
    {
        return  SDL_Rect(static_cast<int>(position.x), static_cast<int>(position.y),width, height);
    }
};

struct Explosion
{
    SDL_Texture* texture;
    SDL_FPoint position;
    int width;
    int height;
    int current_time;
    int total_frame;
    Uint32 start_time;
    Uint32 FPS;
};

#endif
