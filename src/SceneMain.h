#ifndef _SCENE_MAIN_H_
#define _SCENE_MAIN_H_
#include "Game.h"
#include "Scene.h"
#include <cmath>
#include <list>
#include <random>
class Game;
class SceneMain : public Scene
{
private:
    Game &game;    // Reference to the Game instance
    Player player; // Example player object, assuming Player is defined in Object.h
    std::list<Bullet *> bullets;
    std::list<Enemy *> enemies; // List to hold bullets
    Bullet template_bullet;     // Template for creating bullets
    Enemy template_enemy;       // Template for creating enemies
    EnemyBullet template_enemy_bullet;
    std::list<EnemyBullet *> enemy_bullet_list;        // Template for enemy bullets
    std::mt19937 random_gen;                           // Random number generator
    std::uniform_real_distribution<float> random_dist; // Distribution for random numbers

public:
    SceneMain();
    ~SceneMain();
    void init() override;
    void update(float delta_time) override;
    void render() override;
    void clean() override;
    void handleEvents(const SDL_Event &event) override;
    void KeyboardControl(float delta_time);
    void ShootBullet();
    void UpdateBullets(float delta_time);
    void UpdateEnemies(float delta_time);
    void RenderBullets() const;
    void InitRandom();
    void SpawEnemy();
    void RenderEnemies() const;
    void RenderEnemyBullets() const;
    void UpdateEnemyBullets(float delta_time);
    void ShootEnemy(Enemy *enemy);
    SDL_FPoint GetDirection(const Enemy *enemy) const;
    void EnemyExplode(Enemy *enemy);
};

#endif