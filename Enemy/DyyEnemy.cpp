#include <string>
#include "Scene/PlayScene.hpp"
#include "Enemy/PlaneEnemy.hpp"
#include "DyyEnemy.hpp"

DyyEnemy::DyyEnemy(int x, int y) : Enemy("play/enemy-4.png", x, y, 10, 50, 5, 5){
}

// 覆寫死亡行為，分裂出兩隻 PlaneEnemy
void DyyEnemy::OnExplode() {
    // 呼叫原本爆炸動畫、特效等
    Enemy::OnExplode();

    // 在左右兩邊產生 PlaneEnemy
    PlayScene* scene = getPlayScene();
    const float offset = 1; // 兩側的偏移量

    int gridX = static_cast<int>(Position.x / PlayScene::BlockSize);
    int gridY = static_cast<int>(Position.y / PlayScene::BlockSize);

    if (gridX - 1 >= 0) {
        auto* e1 = new PlaneEnemy((gridX - 1) * PlayScene::BlockSize + PlayScene::BlockSize / 2,
                                gridY * PlayScene::BlockSize + PlayScene::BlockSize / 2);
        e1->UpdatePath(scene->mapDistance);
        scene->EnemyGroup->AddNewObject(e1);
    }

    if (gridX + 1 < PlayScene::MapWidth) {
        auto* e2 = new PlaneEnemy((gridX + 1) * PlayScene::BlockSize + PlayScene::BlockSize / 2,
                                gridY * PlayScene::BlockSize + PlayScene::BlockSize / 2);
        e2->UpdatePath(scene->mapDistance);
        scene->EnemyGroup->AddNewObject(e2);
    }
}