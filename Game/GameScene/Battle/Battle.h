#pragma once
#include "math/Matrix4x4.h"
#include "Engine/Camera/Camera.h"
#include "Engine/Texture/Model.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "Game/Player/Player.h"



#include "Game/Score/Score.h"
#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"

class Battle
{
public:

	//~Battle() = default;
	~Battle();

	Battle(std::shared_ptr<Camera> camera = nullptr);


	//	初期化
	void Initialize();

	//	モデルのロード
	void ModelLoad();

	//	更新処理
	void Update();

	//	3D描画処理
	void Draw(const Matrix4x4& viewProjection);

private://	必要なメンバ変数

	std::shared_ptr<Camera> camera_;

	std::unique_ptr<Player> player_;

  //	プレイヤーデータのモデル配列
	std::vector<std::shared_ptr<Model>> mobModels_;

	//	パーツ用ペアレントデータ
	std::vector<WorldTransform> mobparts_;
  
  //	中心座標
	std::shared_ptr<WorldTransform> worldTransform;

private: // Korone

	void EnemyGeneration();

	void EnemyReset();

private: //Korone

	int kEnemyMaxNum_ = 100;
	int kEnemyIntervalNum_ = 5;
	int enemyNum_ = 0;
	int enemyKillCount_ = 0;
	float kEnemyPopPosLength_ = 5.0f;

	int preEnemyType_ = 0;
	int typeCount_ = 0;

	std::unique_ptr<Score> score_;
	//std::unique_ptr<Player> player_;
	std::list<Enemy*> enemies_;

};
