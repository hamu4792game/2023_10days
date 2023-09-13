#include "UI.h"

#include "Game/Score/Score.h"
#include "Game/GameScene/GameScene.h"

#include "GlobalVariables/GlobalVariables.h"
#include "Engine/Easing/Ease.h"

#include "Engine/Input/KeyInput/KeyInput.h"

UI::UI() {

	for (int i = 0; i < kUITexturesMaxNum_; i++) {
		worldTransforms_.push_back(std::make_shared<WorldTransform>());

		colors_[i] = 0xFFFFFFFF;

		isDraw_[i] = false;
	}
	pon.SoundLoadWave("Resources/sound/pon.wav");
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

	count_ = 0;

	score_->SetIsDraw(true, Score::kScore);
	score_->SetIsDraw(true, Score::kCombo);
	score_->SetIsDraw(true, Score::kMemoHighScore);

	score_->SetColor(0xCCCC11FF, Score::kScore);
	score_->SetColor(0xCCCC11FF, Score::kMemoHighScore);
	score_->SetColor(0x30CCDDFF, Score::kCombo);

	colors_[UITextureNames::kPerfect] = 0xFFFFFFFF;
	colors_[UITextureNames::kGreat] = 0xFFFFFFFF;
	colors_[UITextureNames::kGood] = 0xFFFFFFFF;
	colors_[UITextureNames::kMiss] = 0xFFFFFFFF;

	isDraw_[UITextureNames::kScore] = true;
	isDraw_[UITextureNames::kCombo] = true;
	isDraw_[UITextureNames::kHighScore] = true;

	SetAllTransform(Scene::kBattleScene);

}

void UI::ResultInitialize() {

	ResetIsDraw();

	score_->Memo();

	score_->SetIsDraw(false, Score::kScore);
	score_->SetIsDraw(false, Score::kHighCombo);
	score_->SetIsDraw(false, Score::kPerfectNum);
	score_->SetIsDraw(false, Score::kGreatNum);
	score_->SetIsDraw(false, Score::kGoodNum);
	score_->SetIsDraw(false, Score::kMissNum);

	score_->SetColor(0xCCCC11FF, Score::kScore);
	score_->SetColor(0xCCCC11FF, Score::kMemoHighScore);
	score_->SetColor(0x30CCDDFF, Score::kHighCombo);
	score_->SetColor(0xCCCC11FF, Score::kPerfectNum);
	score_->SetColor(0xCC2222FF, Score::kGreatNum);
	score_->SetColor(0x77BB11FF, Score::kGoodNum);
	score_->SetColor(0x777777FF, Score::kMissNum);


	score_->SetIsDraw(false, Score::kMemoHighScore);

	colors_[UITextureNames::kPerfect] = 0xFFFFFFFF;
	colors_[UITextureNames::kGreat] = 0xFFFFFFFF;
	colors_[UITextureNames::kGood] = 0xFFFFFFFF;
	colors_[UITextureNames::kMiss] = 0xFFFFFFFF;

	colors_[UITextureNames::kBack] = 0xFFFFFFFF;

	isDraw_[UITextureNames::kBack] = false;
	isDraw_[UITextureNames::kScore] = false;
	isDraw_[UITextureNames::kCombo] = false;
	isDraw_[UITextureNames::kHighScore] = false;
	isDraw_[UITextureNames::kPerfect] = false;
	isDraw_[UITextureNames::kGreat] = false;
	isDraw_[UITextureNames::kGood] = false;
	isDraw_[UITextureNames::kMiss] = false;

	count_ = 0;

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

void UI::EvaluationUpdate() {

	int type = 0;

	if (isDraw_[UITextureNames::kPerfect]) {

		type = UITextureNames::kPerfect;

		Vector2 pos = Ease::UseEase(uiPos_[Scene::kBattleScene][type], { uiPos_[Scene::kBattleScene][type].x - 50,uiPos_[Scene::kBattleScene][type].y - 100 }, count_, 30, Ease::EaseIn, 3);

		float rotate = Ease::UseEase(0.0f, 0.3f, count_, 30, Ease::EaseIn, 3);

		SetWorldTransform(pos, uiScales_[Scene::kBattleScene][type], rotate, type);

		if (count_ > 20) {
			colors_[type] -= 0x00000010;
		}
		
		if (count_ == 30) {
			count_ = 0;
			isDraw_[type] = false;
		}
		else {
			count_++;
		}
	}
	else if (isDraw_[UITextureNames::kGreat]) {

		type = UITextureNames::kGreat;

		Vector2 pos = Ease::UseEase(uiPos_[Scene::kBattleScene][type], { uiPos_[Scene::kBattleScene][type].x - 30,uiPos_[Scene::kBattleScene][type].y - 60 }, count_, 30, Ease::EaseIn, 3);

		float rotate = Ease::UseEase(0.0f, 0.2f, count_, 30, Ease::EaseIn, 3);

		SetWorldTransform(pos, uiScales_[Scene::kBattleScene][type], rotate, type);

		if (count_ > 20) {
			colors_[type] -= 0x00000010;
		}

		if (count_ == 30) {
			count_ = 0;
			isDraw_[type] = false;
		}
		else {
			count_++;
		}
	}
	else if (isDraw_[UITextureNames::kGood]) {

		type = UITextureNames::kGood;

		Vector2 pos = Ease::UseEase(uiPos_[Scene::kBattleScene][type], { uiPos_[Scene::kBattleScene][type].x - 15,uiPos_[Scene::kBattleScene][type].y - 30 }, count_, 30, Ease::EaseIn, 3);

		float rotate = Ease::UseEase(0.0f, 0.2f, count_, 30, Ease::EaseIn, 3);

		SetWorldTransform(pos, uiScales_[Scene::kBattleScene][type], rotate, type);

		if (count_ > 20) {
			colors_[type] -= 0x00000010;
		}

		if (count_ == 30) {
			count_ = 0;
			isDraw_[type] = false;
		}
		else {
			count_++;
		}
	}
	else if (isDraw_[UITextureNames::kMiss]) {

		type = UITextureNames::kMiss;

		Vector2 pos = Ease::UseEase(uiPos_[Scene::kBattleScene][type], { uiPos_[Scene::kBattleScene][type].x + 2,uiPos_[Scene::kBattleScene][type].y + 30 }, count_, 30, Ease::EaseIn, 3);

		float rotate = Ease::UseEase(0.0f, -0.3f, count_, 30, Ease::EaseIn, 3);

		SetWorldTransform(pos, uiScales_[Scene::kBattleScene][type], rotate, type);

		if (count_ > 20) {
			colors_[type] -= 0x00000010;
		}

		if (count_ == 30) {
			count_ = 0;
			isDraw_[type] = false;
		}
		else {
			count_++;
		}
	}


}

void UI::ComboUpdate() {

	comboCountFrame_++;

	if (score_->GetEvaluation()) {
		comboCountFrame_ = 0;
	}

	if (comboCountFrame_ > kMaxFrameCombo_) {
		comboCountFrame_ = kMaxFrameCombo_;
	}

	float scale = Ease::UseEase(0.0f, scoreNumScales_[Scene::kBattleScene][Score::Parameter::kCombo], comboCountFrame_, kMaxFrameCombo_, Ease::EaseOutBounce);

	score_->SetWorldTransform(scoreNumPos_[Scene::kBattleScene][Score::Parameter::kCombo], scale, 0.0f, Score::Parameter::kCombo);

}

void UI::BattleUpdate() {
	ApplyGlobalVariable();
	SetAllTransform(Scene::kBattleScene);

	if (score_->GetEvaluation() == Score::Evaluation::kPerfect) {

		count_ = 0;

		isDraw_[UITextureNames::kPerfect] = true;
		isDraw_[UITextureNames::kGreat] = false;
		isDraw_[UITextureNames::kGood] = false;
		isDraw_[UITextureNames::kMiss] = false; 
		colors_[UITextureNames::kPerfect] = 0xFFFFFFFF;
	}
	else if (score_->GetEvaluation() == Score::Evaluation::kGreat) {

		count_ = 0;

		isDraw_[UITextureNames::kPerfect] = false;
		isDraw_[UITextureNames::kGreat] = true;
		isDraw_[UITextureNames::kGood] = false;
		isDraw_[UITextureNames::kMiss] = false;
		colors_[UITextureNames::kGreat] = 0xFFFFFFFF;
	}
	else if (score_->GetEvaluation() == Score::Evaluation::kGood) {

		count_ = 0;

		isDraw_[UITextureNames::kPerfect] = false;
		isDraw_[UITextureNames::kGreat] = false;
		isDraw_[UITextureNames::kGood] = true;
		isDraw_[UITextureNames::kMiss] = false;
		colors_[UITextureNames::kGood] = 0xFFFFFFFF;
	}
	else if (score_->GetEvaluation() == Score::Evaluation::kMiss) {

		count_ = 0;

		isDraw_[UITextureNames::kPerfect] = false;
		isDraw_[UITextureNames::kGreat] = false;
		isDraw_[UITextureNames::kGood] = false;
		isDraw_[UITextureNames::kMiss] = true;
		colors_[UITextureNames::kMiss] = 0xFFFFFFFF;
	}

	ComboUpdate();

	EvaluationUpdate();
}

void UI::ResultUpdate() {
	ApplyGlobalVariable();
	SetAllTransform(Scene::kResultScene);

	count_++;

	if (count_ <= kInterval_ * 2) {

		if (count_ == kInterval_) {
			isDraw_[UITextureNames::kBack] = true;
		}
		if (count_ >= kInterval_) {

			float scale = Ease::UseEase(0.0f, uiScales_[Scene::kResultScene][UITextureNames::kBack], count_ - kInterval_, kInterval_, Ease::EaseInSine);

			SetWorldTransform(uiPos_[Scene::kResultScene][UITextureNames::kBack], scale, 0.0f, UITextureNames::kBack);

		}

	}
	else if (count_ == kInterval_ * 3) {

		isDraw_[UITextureNames::kScore] = true;
		isDraw_[UITextureNames::kHighScore] = true;

		score_->SetIsDraw(true, Score::kScore);
		score_->SetIsDraw(true, Score::kMemoHighScore);
		pon.SoundStop();
		pon.SoundPlayWave();
	}
	else if (count_ == kInterval_ * 4) {

		isDraw_[UITextureNames::kCombo] = true;
		score_->SetIsDraw(true, Score::kHighCombo);
		pon.SoundStop();
		pon.SoundPlayWave();
	}
	else if (count_ == kInterval_ * 5) {

		isDraw_[UITextureNames::kPerfect] = true;
		score_->SetIsDraw(true, Score::kPerfectNum);
		pon.SoundStop();
		pon.SoundPlayWave();
	}
	else if (count_ == kInterval_ * 6) {

		isDraw_[UITextureNames::kGreat] = true;
		score_->SetIsDraw(true, Score::kGreatNum);
		pon.SoundStop();
		pon.SoundPlayWave();
	}
	else if (count_ == kInterval_ * 7) {

		isDraw_[UITextureNames::kGood] = true;
		score_->SetIsDraw(true, Score::kGoodNum);
		pon.SoundStop();
		pon.SoundPlayWave();
	}
	else if (count_ == kInterval_ * 8) {

		isDraw_[UITextureNames::kMiss] = true;
		score_->SetIsDraw(true, Score::kMissNum);
		pon.SoundStop();
		pon.SoundPlayWave();
	}

	if (count_ >= kInterval_ * 8) {
		if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_A) ||
			KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_B) ||
			KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_X) ||
			KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_Y) ||
			KeyInput::PushKey(DIK_SPACE)) {
			GameScene::GetInstance()->sceneChangeFlag = true;
		}
	}

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

	DrawUITextures(viewProjectionMat);
	score_->Draw2D(viewProjectionMat);
}
