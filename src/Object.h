#ifndef _OBJECT_H_
#define _OBJECT_H_
#include <SDL.h>
struct Player
{
    SDL_Texture *texture;   // Pointer to the texture for the player
    SDL_FPoint position;    // Position of the player in floating-point coordinates
    int width;              // Width of the player
    int height;             // Height of the player
    float speed = 500.0f;   // Speed of the player movement
    Uint32 cool_down = 300; // Cooldown time for player actions in milliseconds
    Uint32 last_shot = 0;   // Timestamp of the last shot taken by the player
    Player() = default;     // Default constructor
    Player(SDL_Texture *tex, SDL_FPoint pos, int w, int h, float s)
        : texture(tex), position(pos), width(w), height(h), speed(s) {}
};

struct Bullet
{
    SDL_Texture *texture; // Pointer to the texture for the bullet
    SDL_FPoint position;  // Position of the bullet in floating-point coordinates
    int width;            // Width of the bullet
    int height;           // Height of the bullet
    float speed = 800.0f; // Speed of the bullet movement
    Bullet() = default;   // Default constructor
    Bullet(SDL_Texture *tex, SDL_FPoint pos, int w, int h, float s)
        : texture(tex), position(pos), width(w), height(h), speed(s) {}
    Bullet(const Bullet &other)
        : texture(other.texture), position(other.position), width(other.width), height(other.height), speed(other.speed) {}
};

struct Enemy
{
    SDL_Texture *texture; // Pointer to the texture for the bullet
    SDL_FPoint position;  // Position of the bullet in floating-point coordinates
    int width;            // Width of the bullet
    int height;           // Height of the bullet
    float speed = 500.0f; // Speed of the bullet movement
    Uint32 cool_down = 300;
    Uint32 last_shot = 0;
};

struct EnemyBullet
{
    SDL_Texture *texture;
    SDL_FPoint position;
    SDL_FPoint direction;
    int width;
    int height;
    float speed = 800.0f; // Speed of the bullet movement
};

#endif