#include "Game/GameScene/Title/Title.h"
#include "EEnum.h"
#include "externals/imgui/imgui.h"
#include "Engine/Easing/Ease.h"

Title::Title(std::shared_ptr<Camera> camera)
{
	camera_ = camera;

	for (uint16_t index = 0u; index < 30u; index++) {
		enemy_.push_back(std::make_unique<Enemy>());
	}
}

void Title::Initialize()
{
	worldTransform.translation_ = Vector3(0.0f, 6.5f, 0.0f);

	for (uint16_t i = 0u; i < 30u; i++) {
		enemy_[i]->InitializeSP(15.0f + static_cast<float>(5.0f * i), Enemy::BottomType::kA, i, mobModels_type2);
	}
	parts_.resize(mobModels_.size());

	SetParts();

	chara.parent_ = &worldTransform;
	chara.rotation_ = Vector3(0.0f, -1.641f, 0.0f);

	//	カメラの設定
	//	カメラとの親子関係
	camera_->transform.parent_ = &worldTransform;
	camera_->transform.translation_ = Vector3(-25.0f, -0.5f, -5.0f);
	camera_->transform.rotation_ = Vector3(0.035f, 1.256f, 0.0f);
}

void Title::Update()
{
	ImGui::DragFloat3("camera", &camera_->transform.translation_.x, 0.1f);
	ImGui::DragFloat3("cameraRot", &camera_->transform.rotation_.x, AngleToRadian(1.0f));
	ImGui::DragFloat3("trans", &chara.rotation_.x, AngleToRadian(1.0f));

	worldTransform.UpdateMatrix();

	chara.UpdateMatrix();
	
	for (auto& ene : enemy_) {
		ene->Update();
	}
	for (auto& parts : parts_) {
		parts.UpdateMatrix();
	}

}

void Title::Draw(Matrix4x4 viewProjection)
{	
	for (uint16_t i = 0u; i < parts_.size(); i++)
	{
		Model::ModelDraw(parts_[i], viewProjection, 0xffffffff, mobModels_[i].get());
	}

	for (auto& ene : enemy_) {
		ene->Draw(viewProjection);
	}
}

void Title::Draw2D(Matrix4x4 viewProjection2d)
{

}

void Title::SetParts()
{
	parts_[Body].parent_ = &chara;

#pragma region パーツの親子関係と座標の初期設定
	parts_[Head].parent_ = &parts_[Body];
	parts_[BodyUnder].parent_ = &parts_[Body];

	parts_[LArm1].parent_ = &parts_[Body];
	parts_[LArm2].parent_ = &parts_[LArm1];
	parts_[LHand].parent_ = &parts_[LArm2];

	parts_[RArm1].parent_ = &parts_[Body];
	parts_[RArm2].parent_ = &parts_[RArm1];
	parts_[RHand].parent_ = &parts_[RArm2];

	parts_[LLeg1].parent_ = &parts_[BodyUnder];
	parts_[LLeg2].parent_ = &parts_[LLeg1];
	parts_[LFoot].parent_ = &parts_[LLeg2];

	parts_[RLeg1].parent_ = &parts_[BodyUnder];
	parts_[RLeg2].parent_ = &parts_[RLeg1];
	parts_[RFoot].parent_ = &parts_[RLeg2];

	//座標設定
	parts_[Body].translation_ = { 0.0f, 0.0f, 0.0f };
	parts_[BodyUnder].translation_ = { 0.0f, 0.0f, 0.0f };
	parts_[Head].translation_ = { 0.0f, 2.6f, 0.0f };

	parts_[LArm1].translation_ = { -0.8f, 1.57f, 0.0f };
	parts_[LArm2].translation_ = { -1.73f, 0.0f, 0.0f };
	parts_[LHand].translation_ = { -2.37f, 0.0f, 0.0f };

	parts_[RArm1].translation_ = { 0.8f, 1.57f, 0.0f };
	parts_[RArm2].translation_ = { 1.73f, 0.0f, 0.0f };
	parts_[RHand].translation_ = { 2.37f, 0.0f, 0.0f };

	parts_[LLeg1].translation_ = { -0.3f, -1.7f, 0.0f };
	parts_[LLeg2].translation_ = { 0.0f, -2.2f, 0.0f };
	parts_[LFoot].translation_ = { -0.12f, -2.2f, 0.0f };

	parts_[RLeg1].translation_ = { 0.3f, -1.7f, 0.0f };
	parts_[RLeg2].translation_ = { 0.0f, -2.2f, 0.0f };
	parts_[RFoot].translation_ = { 0.12f, -2.2f, 0.0f };
#pragma endregion
}
