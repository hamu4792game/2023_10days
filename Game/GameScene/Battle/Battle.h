#pragma once
#include "math/Matrix4x4.h"
#include "Engine/Camera/Camera.h"
#include "Engine/Texture/Model.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "Game/Player/Player.h"
#include "Engine/Input/AudioInput/AudioInput.h"


#include "Game/Score/Score.h"
#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/UI/UI.h"
#include "Game/Timer/Timer.h"

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

	bool* GetClearFlag() { return &clearFlag_; }


	void SetModels(std::vector<std::vector<std::shared_ptr<Model>>> modeldate) { mobModels_ = modeldate; }
	void SetBottonModels(std::vector<std::shared_ptr<Model>> modeldate) { bottonModels_= modeldate; }
	void SetBottonTextures(std::vector<std::shared_ptr<Texture2D>> modeldate) { bottonTexture_= modeldate; }
	void SetNumberTextures(std::vector<std::shared_ptr<Texture2D>> texturedate) { score_->SetNumberTexture(texturedate); }
	void SetUITextures(std::vector<std::shared_ptr<Texture2D>> texturedate) { ui_->SetUITexture(texturedate); }
	void SetGaugeTextures(std::vector<std::shared_ptr<Texture2D>> textures) { player_->SetGaugeTextures(textures); }
	void SetTimerNumTextures(std::vector<std::shared_ptr<Texture2D>> texturedate) { timer_->SetNumberTexture(texturedate); }
	void SetBlackBoxTexture(std::shared_ptr<Texture2D > texturedate) { blackBox_ = texturedate; }

	const WorldTransform& GetPlayerTransform() { return player_->GetPlayerTransform(); }

public: // korone

	UI* GetUI() { return ui_.get(); }

	Score* GetScore() { return score_.get(); }

private://	必要なメンバ変数

	std::shared_ptr<Camera> camera_;

	std::unique_ptr<Player> player_;

private: // シーンで必要なモデル配列
	//	プレイヤーデータのモデル配列
	std::vector<std::vector<std::shared_ptr<Model>>> mobModels_;

	std::vector<std::shared_ptr<Model>> bottonModels_;
	std::vector<std::shared_ptr<Texture2D>> bottonTexture_;
	
	//	ただの背景
	std::shared_ptr<Texture2D> blackBox_;
	WorldTransform blackTrans_;

private:
	
	//	中心座標
	std::shared_ptr<WorldTransform> worldTransform;

	WorldTransform bottonTransform_;

	uint16_t type_;

	int rand_ = 0;
	int randCount_ = 0;
	void ButtonRand();

	bool tutorialFlag_ = false;

	bool clearFlag_ = false;

private: // sound
	//	こづき
	AudioInput sLittlePunch;
	//	パーフェクトで
	AudioInput sBigPunch;
	//	倒れる
	AudioInput sFallDown;
	//	合言葉は「四肢爆散」
	AudioInput sHitToKill;

public:
	//	マスターフレーム速度。リアルタイム加減速
	static float masterSpeed;

private: // Korone

	void EnemyGeneration();

	void EnemyReset();

private: //Korone

	int kEnemyMaxNum_ = 50;
	int kEnemyIntervalNum_ = 5;

	int enemyCount_ = 0;
	int subCount_ = 0;

	int enemyNum_ = 0;
	int enemyKillCount_ = 0;

	// enemyの配置の間隔
	float kEnemyPopPosLength_ = 20.0f;

	int preEnemyType_ = 0;
	int typeCount_ = 0;

	std::unique_ptr<Score> score_;

	std::unique_ptr<UI> ui_;

	std::unique_ptr<Timer> timer_;
	
	std::list<Enemy*> enemies_;

};
