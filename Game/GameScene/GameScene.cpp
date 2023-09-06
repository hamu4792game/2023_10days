#include "GameScene.h"
#include "externals/imgui/imgui.h"

#include <stdlib.h>
#include <time.h>

GameScene* GameScene::GetInstance()
{
	static GameScene instance;
	return &instance;
}

void GameScene::Initialize()
{
	//	モデルの読み込みと生成
	camera = std::make_shared<Camera>(2000.0f, true);
	camera2d = std::make_shared<Camera>();
	//	カメラ行列の更新
	viewProjectionMatrix = camera->GetViewProMat();
	viewProjectionMatrix2d = camera2d->GetViewProMat();

	//	シーンの生成と初期化
	title = std::make_unique<Title>();
	title->Initialize();

	battle = std::make_unique<Battle>(camera);
	battle->Initialize();

	result = std::make_unique<Result>();
	result->Initialize();

	//	変数の初期化
	scene = Scene::BATTLE;
	oldscene = Scene::TITLE;

	//	モデルのロード
	title->ModelLoad();
	battle->ModelLoad();
	result->ModelLoad();

	//camera->transform.translation_.z = -20.0f;

	ground = std::make_unique<Ground>();
	ground->Initialize();
	ground->ModelLoad();


	unsigned int currentTime = static_cast<unsigned int>(time(nullptr));
	srand(currentTime);
}

void GameScene::Update()
{
	//	シーン切替わり時の初期化
	if (oldscene != scene) {
		switch (scene)
		{
		case GameScene::Scene::TITLE:
			title->Initialize();
			break;
		case GameScene::Scene::BATTLE:
			battle->Initialize();
			break;
		case GameScene::Scene::RESULT:
			result->Initialize();
			break;
		}
	}
	oldscene = scene;

	switch (scene)
	{
	case GameScene::Scene::TITLE:
		title->Update();
		break;
	case GameScene::Scene::BATTLE:
		battle->Update();
		break;
	case GameScene::Scene::RESULT:
		result->Update();
		break;
	}

	

	//	カメラ行列の更新
	viewProjectionMatrix = camera->GetViewProMat();
	viewProjectionMatrix2d = camera2d->GetViewProMat();
}

void GameScene::Draw()
{
	switch (scene)
	{
	case GameScene::Scene::TITLE:
		title->Draw(viewProjectionMatrix);
		break;
	case GameScene::Scene::BATTLE:
		battle->Draw(viewProjectionMatrix);
		break;
	case GameScene::Scene::RESULT:
		result->Draw(viewProjectionMatrix);
		break;
	}

	ground->Draw(viewProjectionMatrix);
	battle->Draw2D(viewProjectionMatrix2d);
	//Model::ModelDraw(pos, viewProjectionMatrix, 0xffffffff, model.get());
}
