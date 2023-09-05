#include "Battle.h"

void Battle::Initialize()
{
}

void Battle::ModelLoad()
{
}

void Battle::EnemyGeneration() {

	while (enemyNum_ - enemyKillCount_ < kEnemyIntervalNum_) {

		if (enemyNum_ == kEnemyMaxNum_) {
			break;
		}
		else {
			Enemy* enmey = new Enemy();

			Vector3 pos = { 0,0,kEnemyPopPosLength_ * (enemyNum_ + 1) };


			enemyNum_++;
		}
	}

}

void Battle::Update()
{
}

void Battle::Draw(Matrix4x4 viewProjection)
{
}
