#pragma once
#include "Game/GameScene/Title/Title.h"
#include "Game/GameScene/Battle/Battle.h"
#include "Game/GameScene/Result/Result.h"

#include "Engine/Camera/Camera.h"

#include "Engine/Texture/Model.h"
#include "Engine/Texture/Texture2D.h"

#include "Game/Ground/Ground.h"
#include "Game/Skydome/Skydome.h"

class GameScene
{
public:
	GameScene() = default;
	~GameScene() = default;

	enum class Scene {
		TITLE,
		BATTLE,
		RESULT
	};
	Scene scene = Scene::TITLE;
	Scene oldscene = Scene::TITLE;

	bool sceneChangeFlag = false;

	//	シングルトンインスタンス
	static GameScene* GetInstance();

	
private:
	
	//	カメラの生成
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Camera> camera2d;
	//	カメラ行列の生成
	Matrix4x4 viewProjectionMatrix{};
	Matrix4x4 viewProjectionMatrix2d{};

	//	シーン用インスタンス
	std::unique_ptr<Title> title;
	std::unique_ptr<Battle> battle;
	std::unique_ptr<Result> result;

	std::unique_ptr<Ground> ground;
	std::unique_ptr<Skydome> skydome;

private: // モデル用変数
	// プレイヤーデータのモデル配列
	std::vector<std::shared_ptr<Model>> mobModels_;
	std::vector<std::shared_ptr<Model>> mobModels_type2;
	//	ボタンUIのモデル 4個
	std::vector<std::shared_ptr<Model>> bottonModels_;
	std::vector<std::shared_ptr<Texture2D>> bottonTexture_;
	//	数字UIのtexture 0～9の10個
	std::vector<std::shared_ptr<Texture2D>> numberTextures_;
	// UIのperfectやcomboなど
	std::vector<std::shared_ptr<Texture2D>> UITextures_;

	std::vector<std::shared_ptr<Model>> shopModel_;

	// ゲージ用
	std::vector<std::shared_ptr<Texture2D>> gaugeTextures_;

private:
	//	パーツ用データの元
	std::vector<WorldTransform> parts_;

private:

public:

	void Initialize();


	void Update();
	
	void Draw();

private: // メンバ関数
	//	モデルのロード
	void ModelLoad();

	//	シーンチェンジ用
	std::shared_ptr<Texture2D> box;
	std::shared_ptr<Texture2D> boxP;
	WorldTransform boxtransform;
	float boxScale = 0.0f;
	float easeNum = 0.0f;
	bool flag = false;
	void SceneChange();

};