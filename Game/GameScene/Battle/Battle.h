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

	//	更新処理
	void Update();

	//	3D描画処理
	void Draw(const Matrix4x4& viewProjection);

	void Draw2D(const Matrix4x4& viewProjection);


	void SetModels(std::vector<std::shared_ptr<Model>> modeldate) { mobModels_ = modeldate; }
	void SetModelsType2(std::vector<std::shared_ptr<Model>> modeldate) { mobModels_type2 = modeldate; }
	void SetBottonModels(std::vector<std::shared_ptr<Model>> modeldate) { bottonModels_= modeldate; }
	void SetNumberTextures(std::vector<std::shared_ptr<Texture2D>> texturedate) { score_->SetNumberTexture(texturedate); }
	void SetUITextures(std::vector<std::shared_ptr<Texture2D>> texturedate) { ui_->SetUITexture(texturedate); }
	void SetGaugeTextures(std::vector<std::shared_ptr<Texture2D>> textures) { player_->SetGaugeTextures(textures); }

public: // korone

	UI* GetUI() { return ui_.get(); }

private://	必要なメンバ変数

	std::shared_ptr<Camera> camera_;

	std::unique_ptr<Player> player_;

private: // シーンで必要なモデル配列
	//	プレイヤーデータのモデル配列
	std::vector<std::shared_ptr<Model>> mobModels_;
	std::vector<std::shared_ptr<Model>> mobModels_type2;

	std::vector<std::shared_ptr<Model>> bottonModels_;
	


private:
	
	//	中心座標
	std::shared_ptr<WorldTransform> worldTransform;

public:
	//	マスターフレーム速度。リアルタイム加減速
	static float masterSpeed;

private: // Korone

	void EnemyGeneration();

	void EnemyReset();

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

};
