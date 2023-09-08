#include "UI.h"

#include "Game/Score/Score.h"
#include "Game/GameScene/GameScene.h"

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

void UI::Initialize() {

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

	score_->SetWorldTransform({ 60.0f,60.0f }, 1.0f, 0.0f, Score::kScore);
	score_->SetWorldTransform({ 960.0f,160.0f }, 1.5f, 0.0f, Score::kCombo);
	score_->SetWorldTransform({ 960.0f,240.0f }, 0.8f, 0.0f, Score::kHighCombo);
	score_->SetWorldTransform({ 960.0f,300.0f }, 0.8f, 0.0f, Score::kPerfectNum);
	score_->SetWorldTransform({ 960.0f,360.0f }, 0.8f, 0.0f, Score::kGreatNum);
	score_->SetWorldTransform({ 960.0f,420.0f }, 0.8f, 0.0f, Score::kGoodNum);
	score_->SetWorldTransform({ 960.0f,480.0f }, 0.8f, 0.0f, Score::kMissNum);


	score_->SetIsDraw(true, Score::kScore);
	score_->SetIsDraw(true, Score::kCombo);
	score_->SetIsDraw(true, Score::kHighCombo);
	score_->SetIsDraw(true, Score::kPerfectNum);
	score_->SetIsDraw(true, Score::kGreatNum);
	score_->SetIsDraw(true, Score::kGoodNum);
	score_->SetIsDraw(true, Score::kMissNum);

	for (int i = 0; i < kUITexturesMaxNum_; i++) {
		isDraw_[i] = true;

		SetWorldTransform({ 960.0f,160.0f + 60.0f * i }, 1.0f, 0.0f, i);
	}



}

void UI::ResultInitialize() {

	ResetIsDraw();

}

void UI::SetWorldTransform(const Vector2& screenPos, float scale, float rotate, int textureName) {

	Vector2 pos = { screenPos.x - WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2 - screenPos.y };

	worldTransforms_[textureName].get()->translation_.x = pos.x;
	worldTransforms_[textureName].get()->translation_.y = pos.y;

	worldTransforms_[textureName].get()->rotation_.z = rotate;

	worldTransforms_[textureName].get()->scale_.x = scale;
	worldTransforms_[textureName].get()->scale_.y = scale;

	worldTransforms_[textureName].get()->UpdateMatrix();
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

void UI::BattleUpdate() {

}

void UI::ResultUpdate() {

}

void UI::DrawUITexture(const Matrix4x4& viewProjectionMat, int textureName) {

	Texture2D::TextureDraw(*(worldTransforms_[textureName].get()), viewProjectionMat, colors_[textureName], UITextures_[textureName].get());
}

void UI::DrawUITextures(const Matrix4x4& viewProjectionMat) {

	for (int i = 0; i < kUITexturesMaxNum_; i++) {
		if (isDraw_[i]) {
			DrawUITexture(viewProjectionMat, i);
		}
	}
}

void UI::Draw2D(const Matrix4x4& viewProjectionMat) {

	score_->Draw2D(viewProjectionMat);
	DrawUITextures(viewProjectionMat);
}
