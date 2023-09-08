#include "Game/GameScene/Title/Title.h"
#include "EEnum.h"
#include "externals/imgui/imgui.h"

Title::Title(std::shared_ptr<Camera> camera)
{
	camera_ = camera;
}

void Title::Initialize()
{
	worldTransform.translation_ = Vector3(0.0f, 6.5f, 0.0f);

	player_.parts_.resize(mobModels_.size());
	enemy_.resize(5);
	for (uint16_t i = 0u; i < enemy_.size(); i++) {
		enemy_[i].parts_.resize(mobModels_type2.size());
		enemy_[i].parts_ = SetParts(enemy_[i].parts_);
		enemy_[i].character.translation_.z = 10.0f + static_cast<float>(i * 10.0f);
	}

	//SetParts();

	//	カメラの設定
	//	カメラとの親子関係
	camera_->transform.parent_ = &worldTransform;
	camera_->transform.translation_ = Vector3(-6.0f, -0.5f, -35.0f);
	camera_->transform.rotation_ = Vector3(0.035f, 0.035f, 0.0f);
}

void Title::Update()
{
	ImGui::DragFloat3("camera", &camera_->transform.translation_.x, 0.1f);
	ImGui::DragFloat3("cameraRot", &camera_->transform.rotation_.x, AngleToRadian(1.0f));

	worldTransform.UpdateMatrix();
	
	for (uint16_t i = 0; i < 5; i++) {
		enemys_.push_back(std::make_unique<Enemy>());
		enemys_[i]->InitializeSP(10.0f + (i * 10.0f), Enemy::BottomType::kA, i, mobModels_type2);
	}
}

void Title::Draw(Matrix4x4 viewProjection)
{
	for (uint16_t i = 0u; i <5; i++)
	{
		Model::ModelDraw(player_.parts_[i], viewProjection, 0xffffffff, mobModels_[i].get());
		enemys_[i]->Draw(viewProjection);
	}
}

void Title::Draw2D(Matrix4x4 viewProjection2d)
{

}

void Title::SetParts()
{
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
