#include "Player.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include "externals/imgui/imgui.h"

Player::Player(std::shared_ptr<Camera> camera)
{
	camera_ = camera;
	for (uint16_t i = 0u; i < PARTS::Num; i++) {
		models_.push_back(std::make_unique<Model>());
	}

	parts_.resize(models_.size());
}

void Player::Initialize(std::vector<std::unique_ptr<Model>> models,const WorldTransform& world)
{
  //  モデルの受け渡し
	models_ = models;

	//	世界との親子関係
	transform.parent_ = &world;
	transform.translation_.y = 1.0f * 30.0f;

	//	カメラとの親子関係
	camera_->transform.parent_ = &transform;
	camera_->transform.translation_.z = -20.0f;
	
	//親子関係
	parts_[Body].parent_ = &transform;
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
	parts_[Body].translation_ = { 0, 0, 0 };
	parts_[BodyUnder].translation_ = { 0, 0, 0 };
	parts_[Head].translation_ = { 0, 2.6f, 0 };

	parts_[LArm1].translation_ = { -0.8f, 1.57f, 0 };
	parts_[LArm2].translation_ = { -1.73f, 0, 0 };
	parts_[LHand].translation_ = { -2.37f, 0, 0 };

	parts_[RArm1].translation_ = { 0.8f, 1.57f, 0 };
	parts_[RArm2].translation_ = { 1.73f, 0, 0 };
	parts_[RHand].translation_ = { 2.37f, 0, 0 };

	parts_[LLeg1].translation_ = { -0.3f, -1.7f, 0 };
	parts_[LLeg2].translation_ = { 0, -2.2f, 0 };
	parts_[LFoot].translation_ = { -0.12f, -2.2f, 0 };

	parts_[RLeg1].translation_ = { 0.3f, -1.7f, 0 };
	parts_[RLeg2].translation_ = { 0, -2.2f, 0 };
	parts_[RFoot].translation_ = { 0.12f, -2.2f, 0 };
	
	
}

void Player::ModelLoad()
{
	

	//models_[Body]->Texture("Resources/player/body.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
}

void Player::Update()
{
	//Move();
	ImGui::DragFloat3("tra", &camera_->transform.translation_.x, 0.1f);
	ImGui::DragFloat3("rot", &camera_->transform.rotation_.x, AngleToRadian(1.0f));

	//CameraUpdate();

	transform.UpdateMatrix();
	for (auto& i : parts_) {
		i.UpdateMatrix();
	}
}

void Player::Draw(const Matrix4x4& viewProjection)
{
	for (uint16_t i = 0u; i < parts_.size(); i++)
	{
		Model::ModelDraw(parts_[i], viewProjection, 0xffffffff, models_[i].get());
	}
}
