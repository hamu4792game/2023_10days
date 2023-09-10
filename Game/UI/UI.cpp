#include "UI.h"

#include "Game/Score/Score.h"
#include "Game/GameScene/GameScene.h"

#include "GlobalVariables/GlobalVariables.h"

UI::UI() {

	for (int i = 0; i < kUITexturesMaxNum_; i++) {
		worldTransforms_.push_back(std::make_shared<WorldTransform>());

		colors_[i] = 0xFFFFFFFF;

		isDraw_[i] = false;
	}
}

void UI::ResetIsDraw() {

	for (int i = 0; i < kUITexturesMaxNum_; i++) {
		isDraw_[i] = false;
	}

	score_->ResetIsDraw();
}

void UI::SetGlobalVariable() {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();


	for (int scene = 0; scene < kUseSceneNum_; scene++) {
		for (int useClass = 0; useClass < kClassNum_; useClass++) {

			globalVariables->CreateGroup(groupNames_[scene][useClass]);

			if (useClass == ClassEmum::kUIClass) {

				for (int name = 0; name < UI::kUITexturesMaxNum_; name++) {

					uiPos_[scene][name] = { float(WinApp::kWindowWidth) / 2.0f , float(WinApp::kWindowHeight) / 2.0f };
					uiScales_[scene][name] = 1.0f;

					globalVariables->AddItem(groupNames_[scene][useClass], uiItemNames_[ParameterNum::kPos][name], uiPos_[scene][name]);
					globalVariables->AddItem(groupNames_[scene][useClass], uiItemNames_[ParameterNum::kScale][name], uiScales_[scene][name]);

				}
			}
			else if (useClass == ClassEmum::kScoreClass) {

				for (int name = 0; name < UI::kScoreNum_; name++) {

					scoreNumPos_[scene][name] = { float(WinApp::kWindowWidth) / 2.0f , float(WinApp::kWindowHeight) / 2.0f };
					scoreNumScales_[scene][name] = 1.0f;

					globalVariables->AddItem(groupNames_[scene][useClass], scoreItemNames_[ParameterNum::kPos][name], scoreNumPos_[scene][name]);
					globalVariables->AddItem(groupNames_[scene][useClass], scoreItemNames_[ParameterNum::kScale][name], scoreNumScales_[scene][name]);

				}
			}
		}
	}

	/*globalVariables->CreateGroup("UIBattleScene");

	globalVariables->AddItem("UIBattleScene", "ScorePos", uiPos_[Scene::kBattleScene][UITextureNames::kScore]);
	globalVariables->AddItem("UIBattleScene", "ScoreScale", uiScales_[Scene::kBattleScene][UITextureNames::kScore]);
	globalVariables->AddItem("UIBattleScene", "HighScorePos", uiPos_[Scene::kBattleScene][UITextureNames::kHighScore]);
	globalVariables->AddItem("UIBattleScene", "HighScoreScale", uiScales_[Scene::kBattleScene][UITextureNames::kHighScore]);
	globalVariables->AddItem("UIBattleScene", "ComboPos", uiPos_[Scene::kBattleScene][UITextureNames::kCombo]);
	globalVariables->AddItem("UIBattleScene", "ComboScale", uiScales_[Scene::kBattleScene][UITextureNames::kCombo]);
	globalVariables->AddItem("UIBattleScene", "HighComboPos", uiPos_[Scene::kBattleScene][UITextureNames::kHighCombo]);
	globalVariables->AddItem("UIBattleScene", "HighComboScale", uiScales_[Scene::kBattleScene][UITextureNames::kHighCombo]);
	globalVariables->AddItem("UIBattleScene", "PerfectPos", uiPos_[Scene::kBattleScene][UITextureNames::kPerfect]);
	globalVariables->AddItem("UIBattleScene", "PerfectScale", uiScales_[Scene::kBattleScene][UITextureNames::kPerfect]);
	globalVariables->AddItem("UIBattleScene", "GreatPos", uiPos_[Scene::kBattleScene][UITextureNames::kGreat]);
	globalVariables->AddItem("UIBattleScene", "GreatScale", uiScales_[Scene::kBattleScene][UITextureNames::kGreat]);
	globalVariables->AddItem("UIBattleScene", "GoodPos", uiPos_[Scene::kBattleScene][UITextureNames::kGood]);
	globalVariables->AddItem("UIBattleScene", "GoodScale", uiScales_[Scene::kBattleScene][UITextureNames::kGood]);
	globalVariables->AddItem("UIBattleScene", "MissPos", uiPos_[Scene::kBattleScene][UITextureNames::kMiss]);
	globalVariables->AddItem("UIBattleScene", "MissScale", uiScales_[Scene::kBattleScene][UITextureNames::kMiss]);

	globalVariables->CreateGroup("UIResultScene");

	globalVariables->AddItem("UIResultScene", "ScorePos", uiPos_[Scene::kResultScene][UITextureNames::kScore]);
	globalVariables->AddItem("UIResultScene", "ScoreScale", uiScales_[Scene::kResultScene][UITextureNames::kScore]);
	globalVariables->AddItem("UIResultScene", "HighScorePos", uiPos_[Scene::kResultScene][UITextureNames::kHighScore]);
	globalVariables->AddItem("UIResultScene", "HighScoreScale", uiScales_[Scene::kResultScene][UITextureNames::kHighScore]);
	globalVariables->AddItem("UIResultScene", "ComboPos", uiPos_[Scene::kResultScene][UITextureNames::kCombo]);
	globalVariables->AddItem("UIResultScene", "ComboScale", uiScales_[Scene::kResultScene][UITextureNames::kCombo]);
	globalVariables->AddItem("UIResultScene", "HighComboPos", uiPos_[Scene::kResultScene][UITextureNames::kHighCombo]);
	globalVariables->AddItem("UIResultScene", "HighComboScale", uiScales_[Scene::kResultScene][UITextureNames::kHighCombo]);
	globalVariables->AddItem("UIResultScene", "PerfectPos", uiPos_[Scene::kResultScene][UITextureNames::kPerfect]);
	globalVariables->AddItem("UIResultScene", "PerfectScale", uiScales_[Scene::kResultScene][UITextureNames::kPerfect]);
	globalVariables->AddItem("UIResultScene", "GreatPos", uiPos_[Scene::kResultScene][UITextureNames::kGreat]);
	globalVariables->AddItem("UIResultScene", "GreatScale", uiScales_[Scene::kResultScene][UITextureNames::kGreat]);
	globalVariables->AddItem("UIResultScene", "GoodPos", uiPos_[Scene::kResultScene][UITextureNames::kGood]);
	globalVariables->AddItem("UIResultScene", "GoodScale", uiScales_[Scene::kResultScene][UITextureNames::kGood]);
	globalVariables->AddItem("UIResultScene", "MissPos", uiPos_[Scene::kResultScene][UITextureNames::kMiss]);
	globalVariables->AddItem("UIResultScene", "MissScale", uiScales_[Scene::kResultScene][UITextureNames::kMiss]);


	globalVariables->CreateGroup("ScoreBattleScene");

	globalVariables->AddItem("ScoreBattleScene", "ScorePos", scoreNumPos_[Scene::kBattleScene][Score::Parameter::kScore]);
	globalVariables->AddItem("ScoreBattleScene", "ScoreScale", scoreNumScales_[Scene::kBattleScene][Score::Parameter::kScore]);
	globalVariables->AddItem("ScoreBattleScene", "HighScorePos", scoreNumPos_[Scene::kBattleScene][Score::Parameter::kMemoHighScore]);
	globalVariables->AddItem("ScoreBattleScene", "HighScoreScale", scoreNumScales_[Scene::kBattleScene][Score::Parameter::kMemoHighScore]);
	globalVariables->AddItem("ScoreBattleScene", "ComboPos", scoreNumPos_[Scene::kBattleScene][Score::Parameter::kCombo]);
	globalVariables->AddItem("ScoreBattleScene", "ComboScale", scoreNumScales_[Scene::kBattleScene][Score::Parameter::kCombo]);
	globalVariables->AddItem("ScoreBattleScene", "HighComboPos", scoreNumPos_[Scene::kBattleScene][Score::Parameter::kHighCombo]);
	globalVariables->AddItem("ScoreBattleScene", "HighComboScale", scoreNumScales_[Scene::kBattleScene][Score::Parameter::kHighCombo]);
	globalVariables->AddItem("ScoreBattleScene", "TotalHighComboPos", scoreNumPos_[Scene::kBattleScene][Score::Parameter::kMemoHighCombo]);
	globalVariables->AddItem("ScoreBattleScene", "TotalHighComboScale", scoreNumScales_[Scene::kBattleScene][Score::Parameter::kMemoHighCombo]);
	globalVariables->AddItem("ScoreBattleScene", "PerfectPos", scoreNumPos_[Scene::kBattleScene][Score::Parameter::kPerfectNum]);
	globalVariables->AddItem("ScoreBattleScene", "PerfectScale", scoreNumScales_[Scene::kBattleScene][Score::Parameter::kPerfectNum]);
	globalVariables->AddItem("ScoreBattleScene", "GreatPos", scoreNumPos_[Scene::kBattleScene][Score::Parameter::kGreatNum]);
	globalVariables->AddItem("ScoreBattleScene", "GreatScale", scoreNumScales_[Scene::kBattleScene][Score::Parameter::kGreatNum]);
	globalVariables->AddItem("ScoreBattleScene", "GoodPos", scoreNumPos_[Scene::kBattleScene][Score::Parameter::kGoodNum]);
	globalVariables->AddItem("ScoreBattleScene", "GoodScale", scoreNumScales_[Scene::kBattleScene][Score::Parameter::kGoodNum]);
	globalVariables->AddItem("ScoreBattleScene", "MissPos", scoreNumPos_[Scene::kBattleScene][Score::Parameter::kMissNum]);
	globalVariables->AddItem("ScoreBattleScene", "MissScale", scoreNumScales_[Scene::kBattleScene][Score::Parameter::kMissNum]);

	globalVariables->CreateGroup("ScoreResultScene");

	globalVariables->AddItem("ScoreResultScene", "ScorePos", scoreNumPos_[Scene::kResultScene][Score::Parameter::kScore]);
	globalVariables->AddItem("ScoreResultScene", "ScoreScale", scoreNumScales_[Scene::kResultScene][Score::Parameter::kScore]);
	globalVariables->AddItem("ScoreResultScene", "HighScorePos", scoreNumPos_[Scene::kResultScene][Score::Parameter::kMemoHighScore]);
	globalVariables->AddItem("ScoreResultScene", "HighScoreScale", scoreNumScales_[Scene::kResultScene][Score::Parameter::kMemoHighScore]);
	globalVariables->AddItem("ScoreResultScene", "ComboPos", scoreNumPos_[Scene::kResultScene][Score::Parameter::kCombo]);
	globalVariables->AddItem("ScoreResultScene", "ComboScale", scoreNumScales_[Scene::kResultScene][Score::Parameter::kCombo]);
	globalVariables->AddItem("ScoreResultScene", "HighComboPos", scoreNumPos_[Scene::kResultScene][Score::Parameter::kHighCombo]);
	globalVariables->AddItem("ScoreResultScene", "HighComboScale", scoreNumScales_[Scene::kResultScene][Score::Parameter::kHighCombo]);
	globalVariables->AddItem("ScoreBattleScene", "TotalHighComboPos", scoreNumPos_[Scene::kBattleScene][Score::Parameter::kMemoHighCombo]);
	globalVariables->AddItem("ScoreBattleScene", "TotalHighComboScale", scoreNumScales_[Scene::kBattleScene][Score::Parameter::kMemoHighCombo]);
	globalVariables->AddItem("ScoreResultScene", "PerfectPos", scoreNumPos_[Scene::kResultScene][Score::Parameter::kPerfectNum]);
	globalVariables->AddItem("ScoreResultScene", "PerfectScale", scoreNumScales_[Scene::kResultScene][Score::Parameter::kPerfectNum]);
	globalVariables->AddItem("ScoreResultScene", "GreatPos", scoreNumPos_[Scene::kResultScene][Score::Parameter::kGreatNum]);
	globalVariables->AddItem("ScoreResultScene", "GreatScale", scoreNumScales_[Scene::kResultScene][Score::Parameter::kGreatNum ]);
	globalVariables->AddItem("ScoreResultScene", "GoodPos", scoreNumPos_[Scene::kResultScene][Score::Parameter::kGoodNum]);
	globalVariables->AddItem("ScoreResultScene", "GoodScale", scoreNumScales_[Scene::kResultScene][Score::Parameter::kGoodNum]);
	globalVariables->AddItem("ScoreResultScene", "MissPos", scoreNumPos_[Scene::kResultScene][Score::Parameter::kMissNum]);
	globalVariables->AddItem("ScoreResultScene", "MissScale", scoreNumScales_[Scene::kResultScene][Score::Parameter::kMissNum]);*/

	ApplyGlobalVariable();
}

void UI::ApplyGlobalVariable() {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	for (int scene = 0; scene < kUseSceneNum_; scene++) {
		for (int useClass = 0; useClass < kClassNum_; useClass++) {

			if (useClass == ClassEmum::kUIClass) {

				for (int name = 0; name < UI::kUITexturesMaxNum_; name++) {

					uiPos_[scene][name] = globalVariables->GetVector2Value(groupNames_[scene][useClass], uiItemNames_[ParameterNum::kPos][name]);
					uiScales_[scene][name] = globalVariables->GetFloatValue(groupNames_[scene][useClass], uiItemNames_[ParameterNum::kScale][name]);
				}
			}
			else if (useClass == ClassEmum::kScoreClass) {

				for (int name = 0; name < UI::kScoreNum_; name++) {

					scoreNumPos_[scene][name] = globalVariables->GetVector2Value(groupNames_[scene][useClass], scoreItemNames_[ParameterNum::kPos][name]);
					scoreNumScales_[scene][name] = globalVariables->GetFloatValue(groupNames_[scene][useClass], scoreItemNames_[ParameterNum::kScale][name]);

				}
			}
		}
	}

}

void UI::SetAllTransform(int scene) {

	for (int name = 0; name < UI::kUITexturesMaxNum_; name++) {
		SetWorldTransform(uiPos_[scene][name], uiScales_[scene][name], 0.0f, name);
	}

	for (int name = 0; name < UI::kScoreNum_; name++) {
		score_->SetWorldTransform(scoreNumPos_[scene][name], scoreNumScales_[scene][name], 0.0f, name);
	}

}

void UI::Initialize() {

	SetGlobalVariable();

	switch (GameScene::GetInstance()->scene)
	{
	case GameScene::Scene::TITLE:

		TitleInitialize();
		break;
	case GameScene::Scene::BATTLE:

		BattleInitialize();
		break;
	case GameScene::Scene::RESULT:

		ResultInitialize();
		break;
	default:
		break;
	}
}

void UI::TitleInitialize() {
	
	ResetIsDraw();
	

}

void UI::BattleInitialize() {

	ResetIsDraw();



	//score_->SetWorldTransform({ 60.0f,60.0f }, 1.0f, 0.0f, Score::kScore);
	//score_->SetWorldTransform({ 960.0f,160.0f }, 1.5f, 0.0f, Score::kCombo);
	//score_->SetWorldTransform({ 960.0f,240.0f }, 0.8f, 0.0f, Score::kHighCombo);
	//score_->SetWorldTransform({ 960.0f,300.0f }, 0.8f, 0.0f, Score::kPerfectNum);
	//score_->SetWorldTransform({ 960.0f,360.0f }, 0.8f, 0.0f, Score::kGreatNum);
	//score_->SetWorldTransform({ 960.0f,420.0f }, 0.8f, 0.0f, Score::kGoodNum);
	//score_->SetWorldTransform({ 960.0f,480.0f }, 0.8f, 0.0f, Score::kMissNum);


	score_->SetIsDraw(true, Score::kScore);
	score_->SetIsDraw(true, Score::kCombo);
	score_->SetIsDraw(true, Score::kHighCombo);
	score_->SetIsDraw(true, Score::kPerfectNum);
	score_->SetIsDraw(true, Score::kGreatNum);
	score_->SetIsDraw(true, Score::kGoodNum);
	score_->SetIsDraw(true, Score::kMissNum);
	score_->SetIsDraw(true, Score::kMemoHighScore);
	score_->SetIsDraw(true, Score::kMemoHighCombo);

	for (int i = 0; i < kUITexturesMaxNum_; i++) {
		isDraw_[i] = true;

		//SetWorldTransform({ 960.0f,160.0f + 60.0f * i }, 1.0f, 0.0f, i);
	}

	SetAllTransform(Scene::kBattleScene);

}

void UI::ResultInitialize() {

	ResetIsDraw();

	SetAllTransform(Scene::kResultScene);
}

void UI::SetWorldTransform(const Vector2& screenPos, float scale, float rotate, int textureName) {

	Vector2 pos = { screenPos.x - WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2 - screenPos.y };

	worldTransforms_[textureName]->translation_.x = pos.x;
	worldTransforms_[textureName]->translation_.y = pos.y;

	worldTransforms_[textureName]->rotation_.z = rotate;

	worldTransforms_[textureName]->scale_.x = scale;
	worldTransforms_[textureName]->scale_.y = scale;

	worldTransforms_[textureName]->UpdateMatrix();
}

void UI::Update() {

	ApplyGlobalVariable();
	SetAllTransform(Scene::kBattleScene);

	switch (GameScene::GetInstance()->scene)
	{
	case GameScene::Scene::TITLE:

		TitleUpdate();
		break;
	case GameScene::Scene::BATTLE:

		BattleUpdate();
		break;
	case GameScene::Scene::RESULT:

		ResultUpdate();
		break;
	default:
		break;
	}
}

void UI::TitleUpdate() {

}

void UI::BattleUpdate() {

}

void UI::ResultUpdate() {

}

void UI::DrawUITexture(const Matrix4x4& viewProjectionMat, int textureName) {
	if (isDraw_[textureName]) {
		Texture2D::TextureDraw(*(worldTransforms_[textureName].get()), viewProjectionMat, colors_[textureName], uiTextures_[textureName].get());
	}
}

void UI::DrawUITextures(const Matrix4x4& viewProjectionMat) {

	for (int i = 0; i < kUITexturesMaxNum_; i++) {
		DrawUITexture(viewProjectionMat, i);
	}
}

void UI::Draw2D(const Matrix4x4& viewProjectionMat) {

	score_->Draw2D(viewProjectionMat);
	DrawUITextures(viewProjectionMat);
}
