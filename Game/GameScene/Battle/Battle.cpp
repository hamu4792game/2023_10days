#include "Battle.h"

Battle::~Battle() {
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
}

void Battle::Initialize()
{
	for (Enemy* enemy : enemies_) {
		enemy->Die();
	}
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	EnemyGeneration();

	score_->Reset();
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
			Enemy* enemy = new Enemy();

			Vector3 pos = { 0,0,kEnemyPopPosLength_ * (enemyNum_ + 1) };

			int type = rand() % 4;

			if (type == preEnemyType_) {

				typeCount_++;
				if (typeCount_ == 4) {

					while (type == preEnemyType_)
					{
						type = rand() % 4;
						if (type != preEnemyType_) {
							break;
						}
					}

					typeCount_ = 0;
				}
			}
			else {
				typeCount_ = 0;
			}

			// Initializeを変える必要がある
			enemy->Initialize();

			enemies_.push_back(enemy);

			enemyNum_++;

			preEnemyType_ = type;
		}
	}

}

void Battle::Update()
{
	for(Enemy* enemy : enemies_){
		if (enemy->GetNum() == enemyKillCount_) {
			player_->HitTest(enemy, score_.get());
		}

		enemy->Update();
	}
	
	// 評価
	if (score_->GetEvaluation()) {
		EnemyGeneration();
	}

	player_->Update();

}

void Battle::Draw(Matrix4x4 viewProjection)
{
}
