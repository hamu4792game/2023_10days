#pragma once
#include "math/Matrix4x4.h"
#include "Engine/Camera/Camera.h"
#include "Engine/Texture/Model.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "Game/Player/Player.h"



#include "Game/Score/Score.h"
#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/UI/UI.h"

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

	void Draw2D(const Matrix4x4& viewProjection);

private://	必要なメンバ変数

	std::shared_ptr<Camera> camera_;

	std::unique_ptr<Player> player_;

	//	プレイヤーデータのモデル配列
	std::vector<std::shared_ptr<Model>> mobModels_;
	std::vector<std::shared_ptr<Model>> mobModels_type2;

	//	パーツ用ペアレントデータ
	std::vector<WorldTransform> mobparts_;
  
	//	中心座標
	std::shared_ptr<WorldTransform> worldTransform;


	std::vector<std::shared_ptr<Model>> bottonModels_;

public:
	//	マスターフレーム速度。リアルタイム加減速
	static float masterSpeed;

private: // Korone

	void EnemyGeneration();

	void EnemyReset();

	void ScoreDraw(const Matrix4x4& viewProjection);

private: //Korone

	int kEnemyMaxNum_ = 100;
	//int kEnemyIntervalNum_ = 5;
	int enemyNum_ = 0;
	int enemyKillCount_ = 0;

	// enemyの配置の間隔
	float kEnemyPopPosLength_ = 10.0f;

	int preEnemyType_ = 0;
	int typeCount_ = 0;

	std::unique_ptr<Score> score_;

	std::unique_ptr<UI> ui_;
	
	std::list<Enemy*> enemies_;

	std::vector<std::shared_ptr<Texture2D>> numberTextures_;

	std::vector<std::shared_ptr<Texture2D>> UITextures_;

};
