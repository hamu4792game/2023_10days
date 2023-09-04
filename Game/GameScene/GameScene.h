#pragma once
#include "Game/GameScene/Title/Title.h"
#include "Game/GameScene/Battle/Battle.h"
#include "Game/GameScene/Result/Result.h"

#include "Engine/Camera/Camera.h"

#include "Engine/Texture/Model.h"

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
	std::unique_ptr<Camera> camera2d;
	//	カメラ行列の生成
	Matrix4x4 viewProjectionMatrix{};
	Matrix4x4 viewProjectionMatrix2d{};

	//	シーン用インスタンス
	std::unique_ptr<Title> title;
	std::unique_ptr<Battle> battle;
	std::unique_ptr<Result> result;

	std::unique_ptr<Model> model;
	std::unique_ptr<Model> model2;
	WorldTransform pos;
	WorldTransform pos2;

public:

	void Initialize();

	void Update();
	
	void Draw();

};