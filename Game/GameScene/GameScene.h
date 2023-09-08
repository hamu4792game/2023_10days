#pragma once
#include "Game/GameScene/Title/Title.h"
#include "Game/GameScene/Battle/Battle.h"
#include "Game/GameScene/Result/Result.h"

#include "Engine/Camera/Camera.h"

#include "Engine/Texture/Model.h"
#include "Engine/Texture/Texture2D.h"

#include "Game/Ground/Ground.h"

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

private: // モデル用変数
	// プレイヤーデータのモデル配列
	std::vector<std::shared_ptr<Model>> mobModels_;
	std::vector<std::shared_ptr<Model>> mobModels_type2;
	//	ボタンUIのモデル 4個
	std::vector<std::shared_ptr<Model>> bottonModels_;
	//	数字UIのtexture 0～9の10個
	std::vector<std::shared_ptr<Texture2D>> numberTextures_;
	// UIのperfectやcomboなど
	std::vector<std::shared_ptr<Texture2D>> UITextures_;

private:

public:

	void Initialize();


	void Update();
	
	void Draw();

private: // メンバ関数
	//	モデルのロード
	void ModelLoad();

};