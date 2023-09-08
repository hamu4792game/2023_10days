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
	//	カメラの読み込みと生成
	camera = std::make_shared<Camera>(2000.0f, true);
	camera2d = std::make_shared<Camera>();
	//	カメラ行列の更新
	viewProjectionMatrix = camera->GetViewProMat();
	viewProjectionMatrix2d = camera2d->GetViewProMat();

	//	シーンの生成
	title = std::make_unique<Title>();
	battle = std::make_unique<Battle>(camera);
	result = std::make_unique<Result>();

	//	ロード前の生成
	//プレイヤーモデルの初期化
	for (uint16_t i = 0u; i < PARTS::Num; i++) {
		mobModels_.push_back(std::make_shared<Model>());
		mobModels_type2.push_back(std::make_shared<Model>());
	}
	//ボタンモデルの初期化
	for (uint32_t i = 0u; i < 4; i++) {
		bottonModels_.push_back(std::make_shared<Model>());
	}
	//	数字テクスチャの初期化
	for (uint16_t i = 0u; i < 10; i++) {
		numberTextures_.push_back(std::make_shared<Texture2D>());
	}
	//	UIテクスチャの初期化
	for (uint16_t i = 0u; i < UI::kUITexturesMaxNum_; i++) {
		UITextures_.push_back(std::make_shared<Texture2D>());
	}


	//	モデルのロード
	ModelLoad();
	//	モデルのセット
	title->SetModels(mobModels_);
	title->SetModelsType2(mobModels_type2);
	title->SetBottonModels(bottonModels_);

	battle->SetModels(mobModels_);
	battle->SetModelsType2(mobModels_type2);
	battle->SetBottonModels(bottonModels_);
	battle->SetNumberTextures(numberTextures_);
	battle->SetUITextures(UITextures_);

	result->SetUI(battle->GetUI());

	//	シーンの初期化
	title->Initialize();
	battle->Initialize();
	result->Initialize();


	//	変数の初期化
	scene = Scene::BATTLE;
	oldscene = Scene::TITLE;

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

	ground->Draw(viewProjectionMatrix);
	//	3D描画
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

	//	2D描画
	switch (scene)
	{
	case GameScene::Scene::TITLE:
		title->Draw2D(viewProjectionMatrix2d);
		break;
	case GameScene::Scene::BATTLE:
		battle->Draw2D(viewProjectionMatrix2d);
		break;
	case GameScene::Scene::RESULT:
		result->Draw(viewProjectionMatrix2d);
		break;
	}

	//Model::ModelDraw(pos, viewProjectionMatrix, 0xffffffff, model.get());
}


void GameScene::ModelLoad()
{
	//頭・身体・腰
	mobModels_[Body]->Texture("Resources/player/Parts/pBody/pBody.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	mobModels_[Head]->Texture("Resources/player/Parts/pHead/pHead.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	mobModels_[BodyUnder]->Texture("Resources/player/Parts/pBodyUnder/pBodyUnder.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	//左腕１・左腕２・左手
	mobModels_[LArm1]->Texture("Resources/player/Parts/pLArm1/pLArm1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	mobModels_[LArm2]->Texture("Resources/player/Parts/pLArm2/pLArm2.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	mobModels_[LHand]->Texture("Resources/player/Parts/pLHand/pLHand.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	//右腕１・右腕２・右手
	mobModels_[RArm1]->Texture("Resources/player/Parts/pRArm1/pRArm1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	mobModels_[RArm2]->Texture("Resources/player/Parts/pRArm2/pRArm2.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	mobModels_[RHand]->Texture("Resources/player/Parts/pRHand/pRHand.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	//左足首１・左足首２・左足
	mobModels_[LLeg1]->Texture("Resources/player/Parts/pLLeg1/pLLeg1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	mobModels_[LLeg2]->Texture("Resources/player/Parts/pLLeg/pLLeg.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	mobModels_[LFoot]->Texture("Resources/player/Parts/pLFoot/pLFoot.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	//右足首１・右足首２・右足
	mobModels_[RLeg1]->Texture("Resources/player/Parts/pRLeg1/pRLeg1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	mobModels_[RLeg2]->Texture("Resources/player/Parts/pRLeg2/pRLeg2.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	mobModels_[RFoot]->Texture("Resources/player/Parts/pRFoot/pRFoot.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");

	//頭・身体・腰
	mobModels_type2[Body]->Texture("Resources/player/Parts/pBody/pBody.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "uvChecker.png");
	mobModels_type2[Head]->Texture("Resources/player/Parts/pHead/pHead.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "uvChecker.png");
	mobModels_type2[BodyUnder]->Texture("Resources/player/Parts/pBodyUnder/pBodyUnder.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "uvChecker.png");
	//左腕１・左腕２・左手
	mobModels_type2[LArm1]->Texture("Resources/player/Parts/pLArm1/pLArm1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "uvChecker.png");
	mobModels_type2[LArm2]->Texture("Resources/player/Parts/pLArm2/pLArm2.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "uvChecker.png");
	mobModels_type2[LHand]->Texture("Resources/player/Parts/pLHand/pLHand.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "uvChecker.png");
	//右腕１・右腕２・右手
	mobModels_type2[RArm1]->Texture("Resources/player/Parts/pRArm1/pRArm1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "uvChecker.png");
	mobModels_type2[RArm2]->Texture("Resources/player/Parts/pRArm2/pRArm2.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "uvChecker.png");
	mobModels_type2[RHand]->Texture("Resources/player/Parts/pRHand/pRHand.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "uvChecker.png");
	//左足首１・左足首２・左足
	mobModels_type2[LLeg1]->Texture("Resources/player/Parts/pLLeg1/pLLeg1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "uvChecker.png");
	mobModels_type2[LLeg2]->Texture("Resources/player/Parts/pLLeg/pLLeg.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "uvChecker.png");
	mobModels_type2[LFoot]->Texture("Resources/player/Parts/pLFoot/pLFoot.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "uvChecker.png");
	//右足首１・右足首２・右足
	mobModels_type2[RLeg1]->Texture("Resources/player/Parts/pRLeg1/pRLeg1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "uvChecker.png");
	mobModels_type2[RLeg2]->Texture("Resources/player/Parts/pRLeg2/pRLeg2.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "uvChecker.png");
	mobModels_type2[RFoot]->Texture("Resources/player/Parts/pRFoot/pRFoot.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "uvChecker.png");

	//	数字texture
	numberTextures_[0]->Texture("Resources/number/0.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	numberTextures_[1]->Texture("Resources/number/1.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	numberTextures_[2]->Texture("Resources/number/2.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	numberTextures_[3]->Texture("Resources/number/3.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	numberTextures_[4]->Texture("Resources/number/4.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	numberTextures_[5]->Texture("Resources/number/5.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	numberTextures_[6]->Texture("Resources/number/6.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	numberTextures_[7]->Texture("Resources/number/7.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	numberTextures_[8]->Texture("Resources/number/8.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	numberTextures_[9]->Texture("Resources/number/9.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");

	// UIのperfectやcomboなど
	UITextures_[UI::kScore]->Texture("Resources/hud/ScoreTextures/SCORE.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	UITextures_[UI::kHighScore]->Texture("Resources/hud/ScoreTextures/HIGHSCORE.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	UITextures_[UI::kCombo]->Texture("Resources/hud/ScoreTextures/COMBO.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	UITextures_[UI::kHighCombo]->Texture("Resources/hud/ScoreTextures/HIGHCOMBO.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	UITextures_[UI::kPerfect]->Texture("Resources/hud/ScoreTextures/PERFECT.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	UITextures_[UI::kGreat]->Texture("Resources/hud/ScoreTextures/GREAT.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	UITextures_[UI::kGood]->Texture("Resources/hud/ScoreTextures/GOOD.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	UITextures_[UI::kMiss]->Texture("Resources/hud/ScoreTextures/MISS.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");

	//ボタンオブジェクトロード
	bottonModels_[0]->Texture("Resources/hud/A/A.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	bottonModels_[1]->Texture("Resources/hud/B/B.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	bottonModels_[2]->Texture("Resources/hud/X/X.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	bottonModels_[3]->Texture("Resources/hud/Y/Y.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
}
