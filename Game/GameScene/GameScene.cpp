#include "GameScene.h"
#include "externals/imgui/imgui.h"
#include "Engine/Input/KeyInput/KeyInput.h"

GameScene* GameScene::GetInstance()
{
	static GameScene instance;
	return &instance;
}

void GameScene::Initialize()
{
	//	モデルの読み込みと生成
	camera = std::make_shared<Camera>(2000.0f, true);
	camera2d = std::make_unique<Camera>();
	//	カメラ行列の更新
	viewProjectionMatrix = camera->GetViewProMat();
	viewProjectionMatrix2d = camera2d->GetViewProMat();

	//	初期化
	title = std::make_unique<Title>();
	title->Initialize();

	battle = std::make_unique<Battle>();
	battle->Initialize();

	result = std::make_unique<Result>();
	result->Initialize();

	title->ModelLoad();
	battle->ModelLoad();
	result->ModelLoad();

	model = std::make_unique<Model>();
	model2 = std::make_unique<Model>();
	model->Texture("Resources/player/limbs.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	model2->Texture("Resources/player/head.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	pos.translation_ = Vector3(0.0f, 0.0f, 0.0f);
	pos2.translation_ = Vector3(3.0f, 0.0f, 0.0f);

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

	ImGui::DragFloat("te", &pos.monocuro.pibot.x, 1.0f);
	//pos2.cMono = pos.cMono;

	pos.UpdateMatrix();
	pos2.UpdateMatrix();

	camera->transform.translation_.z = -20.0f;

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

	Model::ModelDraw(pos, viewProjectionMatrix, 0xffffffff, model.get());
	Model::ModelDraw(pos2, viewProjectionMatrix, 0xffffffff, model.get());
	Model::ModelDraw(pos, viewProjectionMatrix, 0xffffffff, model2.get());
	Model::ModelDraw(pos2, viewProjectionMatrix, 0xffffffff, model2.get());

}
