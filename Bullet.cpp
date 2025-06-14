#include "Bullet.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Scene/PlayScene.hpp"
#include <allegro5/allegro_primitives.h>

PlayScene *Bullet::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}
void Bullet::OnExplode(Enemy *enemy) {
}
Bullet::Bullet(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent) : Sprite(img, position.x, position.y), speed(speed), damage(damage), parent(parent) {
    PlayScene* scene = getPlayScene();
    if (scene && scene->potionEffectActive) {
        this->damage = damage * 1.5f; 
    } else {
        this->damage = damage;
    }
    
    Velocity = forwardDirection.Normalize() * speed;
    Rotation = rotation;
    CollisionRadius = 4;
}


void Bullet::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    PlayScene *scene = getPlayScene();

    // 子彈撞牆判斷
    int tileX = static_cast<int>(Position.x / PlayScene::BlockSize);
    int tileY = static_cast<int>(Position.y / PlayScene::BlockSize);
    if (scene->GetTileType(tileX, tileY) == PlayScene::TILE_FLOOR) {
        scene->BulletGroup->RemoveObject(objectIterator);
        return;
    }

    // 碰撞敵人
    for (auto &it : scene->EnemyGroup->GetObjects()) {
        Enemy *enemy = dynamic_cast<Enemy *>(it);
        if (!enemy->Visible)
            continue;
        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, enemy->Position, enemy->CollisionRadius)) {
            OnExplode(enemy);
            enemy->Hit(damage);
            scene->BulletGroup->RemoveObject(objectIterator);
            return;
        }
    }

    // 出界判斷
    if (!Engine::Collider::IsRectOverlap(Position - Size / 2, Position + Size / 2, Engine::Point(0, 0), PlayScene::GetClientSize())) {
        scene->BulletGroup->RemoveObject(objectIterator);
        return;
    }
}




