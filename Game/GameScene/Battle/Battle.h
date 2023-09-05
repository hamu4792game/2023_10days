#pragma once
#include "math/Matrix4x4.h"

#include "Game/Score/Score.h"
#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"

class Battle
{
public:
	Battle() = default;
	~Battle() = default;

	void Initialize();

	void ModelLoad();

	void Update();

	void Draw(Matrix4x4 viewProjection);

private:

private: // Korone

	void EnemyGeneration();

private: //Korone

	int kEnemyMaxNum_ = 100;
	int kEnemyIntervalNum_ = 5;
	int enemyNum_ = 0;
	int enemyKillCount_ = 0;
	float kEnemyPopPosLength_ = 5.0f;

	int preEnemyType_ = 0;
	int typeCount_ = 0;

	std::unique_ptr<Score> score_;

};
