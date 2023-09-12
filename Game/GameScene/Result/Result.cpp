#include "Result.h"

#include "Engine/Input/KeyInput/KeyInput.h"
#include "Game/GameScene/GameScene.h"

Result::Result(std::shared_ptr<Camera> camera) {
	camera_ = camera;
}

void Result::Initialize()
{
	ui_->Initialize();

	mobTransform_.resize(mobModels_.size());
	shopTransform_.resize(shopModels_.size());
	tentyoTransform_.resize(tentyoModels_.size());

	camera_->transform.translation_ = Vector3(0.0f, 3.0f, 0.0f);
	camera_->transform.rotation_ = Vector3(AngleToRadian(3.0f), 0.0f, 0.0f);

}

void Result::Update()
{
	ui_->Update();
	
	if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_A) ||
		KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_B) ||
		KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_X) ||
		KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_Y) ||
		KeyInput::PushKey(DIK_SPACE)) {
		GameScene::GetInstance()->sceneChangeFlag = true;
	}

	for (auto& i : mobTransform_) {
		i.UpdateMatrix();
	}
	for (auto& i : shopTransform_) {
		i.UpdateMatrix();
	}
	for (auto& i : tentyoTransform_) {
		i.UpdateMatrix();
	}

}

void Result::Draw(Matrix4x4 viewProjection)
{
	for (uint16_t i = 0u; i < mobModels_.size(); i++) {
		Model::ModelDraw(mobTransform_[i], viewProjection, 0xffffffff, mobModels_[i].get());
	}
	for (uint16_t i = 0u; i < shopModels_.size(); i++) {
		Model::ModelDraw(shopTransform_[i], viewProjection, 0xffffffff, shopModels_[i].get());
	}
	for (uint16_t i = 0u; i < tentyoModels_.size(); i++) {
		Model::ModelDraw(tentyoTransform_[i], viewProjection, 0xffffffff, tentyoModels_[i].get());
	}
}

void Result::Draw2D(const Matrix4x4& viewProjection) {

	ui_->Draw2D(viewProjection);
}
