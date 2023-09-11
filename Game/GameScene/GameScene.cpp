#include "GameScene.h"
#include "externals/imgui/imgui.h"

#include <stdlib.h>
#include <time.h>
#include "Engine/Input/KeyInput/KeyInput.h"
#include <algorithm>
#include "Engine/Easing/Ease.h"

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
	title = std::make_unique<Title>(camera);
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
	//	shopモデルの生成
	for (uint16_t i = 0u; i < static_cast<uint16_t>(SHOPPARTS::Num); i++) {
		shopModel_.push_back(std::make_shared<Model>());
	}
	
	// ゲージ用
	for (uint16_t i = 0u; i < Player::kGaugeResourceNum_; i++) {
		gaugeTextures_.push_back(std::make_shared<Texture2D>());
	}

	//	モデルのロード
	ModelLoad();
	//	モデルのセット
	title->SetModels(mobModels_);
	title->SetModelsType2(mobModels_type2);
	title->SetBottonModels(bottonModels_);
	title->SetShopModel(shopModel_);

	battle->SetModels(mobModels_);
	battle->SetModelsType2(mobModels_type2);
	battle->SetBottonModels(bottonModels_);
	battle->SetNumberTextures(numberTextures_);
	battle->SetUITextures(UITextures_);
	battle->SetGaugeTextures(gaugeTextures_);

	result->SetUI(battle->GetUI());

	//	シーンの初期化
	title->Initialize();
	battle->Initialize();
	result->Initialize();


	//	変数の初期化
	scene = Scene::TITLE;
	oldscene = Scene::RESULT;

	//camera->transform.translation_.z = -20.0f;

	ground = std::make_unique<Ground>();
	ground->Initialize();
	ground->ModelLoad();

	skydome = std::make_unique<Skydome>();
	skydome->Initialize();
	skydome->ModelLoad();


	//	シーンチェンジ用
	sceneChangeFlag = false;
	flag = false;
	easeNum = 0.0f;
	boxtransform.scale_ = Vector3(0.0f, 0.0f, 1.0f);

}

void GameScene::Update()
{
	//	シーン切替わり時の初期化
	if (oldscene != scene) {
		switch (scene)
		{
		case GameScene::Scene::TITLE:
			title->Initialize();
			skydome->NullParent();
			break;
		case GameScene::Scene::BATTLE:
			battle->Initialize();
			skydome->SetParent(battle->GetPlayerTransform());
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
		if (KeyInput::PushKey(DIK_P)) {
			//scene = Scene::BATTLE;
			sceneChangeFlag = true;
		}
		break;
	case GameScene::Scene::BATTLE:
		battle->Update();
		if (KeyInput::PushKey(DIK_P)) {
			scene = Scene::TITLE;
		}
		break;
	case GameScene::Scene::RESULT:
		result->Update();
		break;
	}

	//	シーンチェンジの処理
	if (sceneChangeFlag) {
		SceneChange();
	}

	//	カメラ行列の更新
	viewProjectionMatrix = camera->GetViewProMat();
	viewProjectionMatrix2d = camera2d->GetViewProMat();
}

void GameScene::Draw()
{

	skydome->Draw(viewProjectionMatrix);
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
		result->Draw2D(viewProjectionMatrix2d);
		break;
	}

	if (sceneChangeFlag) {
		Texture2D::TextureDraw(boxtransform, viewProjectionMatrix2d, 0x000000ff, &box);
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
	mobModels_type2[Body]->Texture("Resources/player/Parts/pBody/pBody.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "player/happakun.png");
	mobModels_type2[Head]->Texture("Resources/player/Parts/pHead/pHead.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "player/tomatokun.png");
	mobModels_type2[BodyUnder]->Texture("Resources/player/Parts/pBodyUnder/pBodyUnder.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "player/tomatokun.png");
	//左腕１・左腕２・左手
	mobModels_type2[LArm1]->Texture("Resources/player/Parts/pLArm1/pLArm1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "player/bluepro.png");
	mobModels_type2[LArm2]->Texture("Resources/player/Parts/pLArm2/pLArm2.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "player/bluepro.png");
	mobModels_type2[LHand]->Texture("Resources/player/Parts/pLHand/pLHand.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "player/bluepro.png");
	//右腕１・右腕２・右手
	mobModels_type2[RArm1]->Texture("Resources/player/Parts/pRArm1/pRArm1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "player/bluepro.png");
	mobModels_type2[RArm2]->Texture("Resources/player/Parts/pRArm2/pRArm2.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "player/bluepro.png");
	mobModels_type2[RHand]->Texture("Resources/player/Parts/pRHand/pRHand.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "player/bluepro.png");
	//左足首１・左足首２・左足
	mobModels_type2[LLeg1]->Texture("Resources/player/Parts/pLLeg1/pLLeg1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "player/Yostar.png");
	mobModels_type2[LLeg2]->Texture("Resources/player/Parts/pLLeg/pLLeg.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "player/Yostar.png");
	mobModels_type2[LFoot]->Texture("Resources/player/Parts/pLFoot/pLFoot.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "player/Yostar.png");
	//右足首１・右足首２・右足
	mobModels_type2[RLeg1]->Texture("Resources/player/Parts/pRLeg1/pRLeg1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "player/Yostar.png");
	mobModels_type2[RLeg2]->Texture("Resources/player/Parts/pRLeg2/pRLeg2.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "player/Yostar.png");
	mobModels_type2[RFoot]->Texture("Resources/player/Parts/pRFoot/pRFoot.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "player/Yostar.png");

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

	// ゲージ用
	gaugeTextures_[Player::GaugeResource::kBackResource]->Texture("Resources/gauge/white.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	gaugeTextures_[Player::GaugeResource::kGaugeResource]->Texture("Resources/gauge/white.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");

	//ボタンオブジェクトロード
	bottonModels_[0]->Texture("Resources/hud/A/A.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	bottonModels_[1]->Texture("Resources/hud/B/B.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	bottonModels_[2]->Texture("Resources/hud/X/X.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	bottonModels_[3]->Texture("Resources/hud/Y/Y.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");

	//	

	shopModel_[0]->Texture("Resources/shop/shop.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "shop/shop.png");
	shopModel_[1]->Texture("Resources/plane/plane.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "hud/board.png");
	shopModel_[2]->Texture("Resources/plane/plane.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "hud/onetime.png");

	box.Texture("Resources/hud/block.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
}


void GameScene::SceneChange()
{
	//80
	boxtransform.rotation_.z += AngleToRadian(15.0f);
	easeNum += 0.02f;
	easeNum = std::clamp(easeNum, 0.0f, 1.0f);
	if (!flag) {
		boxScale = Ease::UseEase(0.0f, 100.0f, easeNum, 1.0f, Ease::EaseType::EaseOutCirc);
		
		if (easeNum == 1.0f) {
			flag = true;
			easeNum = 0.0f;
			boxScale = 100.0f;
			//	あくまでもここはシーン切り替え処理のみ
			switch (scene)
			{
			case GameScene::Scene::TITLE:
				scene = GameScene::Scene::BATTLE;
				break;
			case GameScene::Scene::BATTLE:
				scene = GameScene::Scene::RESULT;
				break;
			case GameScene::Scene::RESULT:
				scene = GameScene::Scene::TITLE;
				break;
			}
		}
	}
	else if (flag) {
		boxScale = Ease::UseEase(100.0f, 0.0f, easeNum, 1.0f, Ease::EaseType::EaseOutCirc);
		if (easeNum == 1.0f) {
			flag = false;
			sceneChangeFlag = false;
			easeNum = 0.0f;
			boxScale = 0.0f;
			boxtransform.rotation_.z = 0.0f;
		}
	}

	boxtransform.scale_ = Vector3(boxScale, boxScale, 1.0f);

	boxtransform.UpdateMatrix();
}
