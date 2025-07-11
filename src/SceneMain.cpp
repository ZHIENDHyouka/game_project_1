#include "SceneMain.h"

SceneMain::SceneMain() : game(Game::Instance())
{
}

SceneMain::~SceneMain()
{
    clean();
}

void SceneMain::init()
{
    player.texture = IMG_LoadTexture(game.GetRenderer(), "assets/assets/image/SpaceShip.png");
    if (player.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s", IMG_GetError());
        return;
    }

    SDL_QueryTexture(player.texture, nullptr, nullptr, &player.width, &player.height);
    player.width /= 4;
    player.height /= 4;
    player.position = {
        static_cast<float>(game.GetWindowWidth() / 2 - player.width / 2),
        static_cast<float>(game.GetWindowHeight() - player.height)
    };

    template_bullet.texture = IMG_LoadTexture(game.GetRenderer(), "assets/assets/image/laser-3.png");
    if (template_bullet.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load bullet texture: %s", IMG_GetError());
        return;
    }
    SDL_QueryTexture(template_bullet.texture, nullptr, nullptr, &template_bullet.width, &template_bullet.height);
    template_bullet.width /= 4;
    template_bullet.height /= 4;

    template_enemy.texture = IMG_LoadTexture(game.GetRenderer(), "assets/assets/image/insect-2.png");
    if (template_enemy.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load enemy texture: %s", IMG_GetError());
        return;
    }
    SDL_QueryTexture(template_enemy.texture, nullptr, nullptr, &template_enemy.width, &template_enemy.height);
    template_enemy.width /= 4;
    template_enemy.height /= 4;

    template_enemy_bullet.texture = IMG_LoadTexture(game.GetRenderer(), "assets/assets/image/bullet-1.png");
    if (template_enemy_bullet.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load enemy bullet texture: %s", IMG_GetError());
        return;
    }
    SDL_QueryTexture(template_enemy_bullet.texture, nullptr, nullptr, &template_enemy_bullet.width,
                     &template_enemy_bullet.height);
    template_enemy_bullet.width /= 4;
    template_enemy_bullet.height /= 4;

    template_exp.texture = IMG_LoadTexture(game.GetRenderer(),"assets/assets/effect/explosion.png");
    if (template_exp.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load enemy bullet texture: %s", IMG_GetError());
        return;
    }
    SDL_QueryTexture(template_exp.texture, nullptr, nullptr, &template_exp.width,&template_exp.height);
    template_exp.total_frame = template_exp.width / template_exp.height;
    template_exp.width = template_exp.height;
}

void SceneMain::update(float delta_time)
{
    KeyboardControl(delta_time);
    UpdateBullets(delta_time);
    SpawEnemy();
    UpdateEnemyBullets(delta_time);
    UpdateEnemies(delta_time);
    UpdateExplosions(delta_time);
}

void SceneMain::render()
{
    RenderBullets();
    RenderEnemyBullets();
    const SDL_Rect dest_rect = {
        static_cast<int>(player.position.x), static_cast<int>(player.position.y),
        player.width, player.height
    };
    SDL_RenderCopy(game.GetRenderer(), player.texture, nullptr, &dest_rect);
    RenderEnemies();
    RenderExplosions();
}

void SceneMain::clean()
{
    for (auto& bullet : bullets)
    {
        delete bullet; // Delete the bullet object
        bullet = nullptr; // Set pointer to nullptr after deletion
    }
    bullets.clear(); // Clear the list of bullets
    for (auto& enemy : enemies)
    {
        delete enemy; // Delete the enemy object
        enemy = nullptr; // Set pointer to nullptr after deletion
    }
    enemies.clear(); // Clear the list of enemies
    for (auto& enemy_bullet : enemy_bullet_list)
    {
        delete enemy_bullet; // Delete the enemy bullet object
        enemy_bullet = nullptr; // Set pointer to nullptr after deletion
    }
    enemy_bullet_list.clear(); // Clear the list of enemy bullets
    for (auto& explosion : explosions)
    {
        if (explosion)
        {
            delete explosion;
            explosion=nullptr;
        }
    }
    explosions.clear();

    if (player.texture)
    {
        SDL_DestroyTexture(player.texture);
        player.texture = nullptr;
    }
    player.position = {0.0f, 0.0f};
    player.width = 0;
    player.height = 0;

    if (template_bullet.texture)
    {
        SDL_DestroyTexture(template_bullet.texture);
        template_bullet.texture = nullptr;
    }

    if (template_enemy.texture)
    {
        SDL_DestroyTexture(template_enemy.texture);
        template_enemy.texture = nullptr;
    }

    if (template_enemy_bullet.texture)
    {
        SDL_DestroyTexture(template_enemy_bullet.texture);
        template_enemy_bullet.texture = nullptr;
    }
    if (template_exp.texture)
    {
        SDL_DestroyTexture(template_exp.texture);
        template_exp.texture = nullptr;
    }
}

void SceneMain::handleEvents(const SDL_Event& event)
{
}

void SceneMain::KeyboardControl(float delta_time)
{
    const float delta_value = delta_time * player.speed;
    const auto key_state = SDL_GetKeyboardState(nullptr);
    if (key_state[SDL_SCANCODE_W])
    {
        player.position.y -= delta_value; // Move up
    }
    if (key_state[SDL_SCANCODE_S])
    {
        player.position.y += delta_value; // Move down
    }
    if (key_state[SDL_SCANCODE_A])
    {
        player.position.x -= delta_value; // Move left
    }
    if (key_state[SDL_SCANCODE_D])
    {
        player.position.x += delta_value; // Move right
    }

    if (player.position.x < 0)
    {
        player.position.x = 0; // Prevent moving out of bounds
    }
    if (player.position.x > game.GetWindowWidth() - player.width)
    {
        player.position.x = static_cast<float>(game.GetWindowWidth() - player.width); // Prevent moving out of bounds
    }
    if (player.position.y < 0)
    {
        player.position.y = 0; // Prevent moving out of bounds
    }
    if (player.position.y > game.GetWindowHeight() - player.height)
    {
        player.position.y = static_cast<float>(game.GetWindowHeight() - player.height); // Prevent moving out of bounds
    }

    // control the bullet
    if (key_state[SDL_SCANCODE_SPACE])
    {
        auto curr_time = SDL_GetTicks();
        if (curr_time - player.last_shot >= player.cool_down)
        {
            ShootBullet();
            player.last_shot = curr_time; // Update the last shot time
        }
    }
}

void SceneMain::ShootBullet()
{
    const auto bullet = new Bullet(template_bullet);
    bullet->position.x = player.position.x + (player.width - bullet->width) / 2;
    bullet->position.y = player.position.y;
    bullets.push_back(bullet);
}

void SceneMain::UpdateBullets(float delta_time)
{
    constexpr int margin = 32; // Margin to keep bullets within bounds
    constexpr int damage = 1;
    for (auto it = bullets.begin(); it != bullets.end();)
    {
        Bullet* bullet = *it;
        bullet->position.y -= bullet->speed * delta_time; // Move the bullet up
        if (bullet->position.y + margin < 0)
        {
            delete bullet; // Delete the bullet if it goes out of bounds
            it = bullets.erase(it); // Remove the bullet from the list
            // SDL_Log("Bullet removed from the list due to going out of bounds.");
        }
        else
        {
            bool is_hit = false;
            SDL_Rect bullet_rect = bullet->GetRect();
            for (const auto enemy : enemies)
            {
                SDL_Rect enemy_rect = enemy->GetRect();
                if (SDL_HasIntersection(&bullet_rect, &enemy_rect))
                {
                    enemy->current_health -= damage;
                    delete bullet;
                    bullet = nullptr;
                    it = bullets.erase(it);
                    is_hit = true;
                    break;
                }
            }
            if (!is_hit)
                ++it;
        }
    }
}

void SceneMain::UpdateEnemies(float delta_time)
{
    const auto curr_time = SDL_GetTicks();
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        Enemy* enemy = *it;
        enemy->position.y += enemy->speed * delta_time; // Move the enemy down
        if (enemy->position.y > game.GetWindowHeight())
        {
            delete enemy; // Delete the enemy if it goes out of bounds
            it = enemies.erase(it); // Remove the enemy from the list
        }
        else
        {
            if (enemy->current_health <= 0)
            {
                EnemyExplode(enemy);
                it = enemies.erase(it);
            }else
            {
                ++it;
            }
            if (curr_time - enemy->last_shot >= enemy->cool_down)
            {
                ShootEnemy(enemy); // Shoot an enemy bullet if cooldown is met
                enemy->last_shot = curr_time; // Update the last shot time
            }
        }
    }
}

void SceneMain::RenderBullets() const
{
    for (const auto& bullet : bullets)
    {
        SDL_Rect dest_rect = {
            static_cast<int>(bullet->position.x), static_cast<int>(bullet->position.y),
            bullet->width, bullet->height
        };
        SDL_RenderCopy(game.GetRenderer(), bullet->texture, nullptr, &dest_rect);
    }
}

void SceneMain::InitRandom()
{
    std::random_device rd;
    random_gen = std::mt19937(rd()); // Initialize random number generator with a random seed
    random_dist = std::uniform_real_distribution<float>(0.0f, 1.0f);
}

void SceneMain::SpawEnemy()
{
    if (random_dist(random_gen) > 1 / 60.0f) // 1% chance to spawn an enemy each frame
    {
        return;
    }
    Enemy* enemy = new Enemy(template_enemy);
    enemy->position.x = random_dist(random_gen) * (game.GetWindowWidth() - enemy->width);
    enemy->position.y = -static_cast<float>(enemy->height); // Start above the screen
    enemies.push_back(enemy);
}

void SceneMain::RenderEnemies() const
{
    for (const auto& enemy : enemies)
    {
        SDL_Rect dest_rect = {
            static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y),
            enemy->width, enemy->height
        };
        SDL_RenderCopy(game.GetRenderer(), enemy->texture, nullptr, &dest_rect);
    }
}

void SceneMain::RenderEnemyBullets() const
{
    for (auto e_bullet : enemy_bullet_list)
    {
        SDL_Rect rec = {
            static_cast<int>(e_bullet->position.x),
            static_cast<int>(e_bullet->position.y),
            e_bullet->width,
            e_bullet->height
        };
        auto ancle = atan2(e_bullet->direction.y, e_bullet->direction.x) * 180 / M_PI - 90;
        SDL_RenderCopyEx(game.GetRenderer(), e_bullet->texture, nullptr, &rec, ancle, nullptr, SDL_FLIP_NONE);
    }
}

void SceneMain::UpdateEnemyBullets(const float delta_time)
{
    constexpr auto margin = 32;
    const int bullet_damage = template_bullet.damage;
    for (auto it = enemy_bullet_list.begin(); it != enemy_bullet_list.end();)
    {
        auto temp_bullet = *it;
        temp_bullet->position.x += temp_bullet->speed * delta_time * temp_bullet->direction.x;
        temp_bullet->position.y += temp_bullet->speed * delta_time * temp_bullet->direction.y;
        if (temp_bullet->position.y > game.GetWindowHeight() + margin ||
            temp_bullet->position.x > game.GetWindowWidth() + margin ||
            temp_bullet->position.x < -margin)
        {
            delete temp_bullet;
            it = enemy_bullet_list.erase(it);
        }
        else
        {
            SDL_Rect player_rect = player.GetRect();
            SDL_Rect bullet_enemy_rect = template_enemy_bullet.GetRect();
            if (SDL_HasIntersection(&player_rect, &bullet_enemy_rect))
            {
                player.current_health -= bullet_damage;
                delete temp_bullet;
                temp_bullet = nullptr;
                enemy_bullet_list.emplace(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void SceneMain::ShootEnemy(Enemy* enemy)
{
    auto temp_bullet = new EnemyBullet(template_enemy_bullet);
    temp_bullet->position.x = enemy->position.x + (enemy->width - temp_bullet->width) / 2;
    temp_bullet->position.y = enemy->position.y + enemy->height;
    temp_bullet->direction = GetDirection(enemy);
    enemy_bullet_list.push_back(temp_bullet);
}

SDL_FPoint SceneMain::GetDirection(const Enemy* enemy) const
{
    auto x = (player.position.x + player.width / 2) - (enemy->position.x + enemy->width / 2);
    auto y = (player.position.y + player.height / 2) - (enemy->position.y + enemy->height / 2);
    auto vec_length = sqrt(x * x + y * y);
    return SDL_FPoint(x / vec_length, y / vec_length);
}

void SceneMain::EnemyExplode(Enemy* enemy)
{
    auto current_time = SDL_GetTicks();
    auto explosion =  new Explosion(template_exp);
    explosion->position.x = enemy->position.x + enemy->width / 2 - explosion->width / 2;
    explosion->position.y = enemy->position.y + enemy->height / 2 - explosion->height / 2;
    explosion->start_time = current_time;
    explosions.push_back(explosion);
    delete enemy;
}

void SceneMain::UpdateExplosions(float delta_time)
{
    const auto current_time = SDL_GetTicks();
    for (auto it = explosions.begin(); it != explosions.end(); )
    {
        auto explosion = *it;
        explosion->current_frame = (current_time-explosion->start_time)*explosion->FPS/1000;//100ms 每帧的前提
        if (explosion->current_frame>=explosion->total_frame)
        {
            delete explosion;
            explosion=nullptr;
            it = explosions.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void SceneMain::RenderExplosions() const
{
    for (const auto explosion : explosions)
    {
        SDL_Rect ex_rect_dst = explosion->GetRect();
        SDL_Rect ex_rect_src = {explosion->current_frame*explosion->width,0,
                                explosion->width,explosion->height};
        SDL_RenderCopy(game.GetRenderer(),explosion->texture,&ex_rect_src,&ex_rect_dst);
    }
}
