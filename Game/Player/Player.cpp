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

void Player::Initialize(const WorldTransform& world)
{

	
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
	//頭・身体・腰
	models_[Body]->Texture("Resources/player/Parts/pBody/pBody.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	models_[Head]->Texture("Resources/player/Parts/pHead/pHead.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	models_[BodyUnder]->Texture("Resources/player/Parts/pBodyUnder/pBodyUnder.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	//左腕１・左腕２・左手
	models_[LArm1]->Texture("Resources/player/Parts/pLArm1/pLArm1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	models_[LArm2]->Texture("Resources/player/Parts/pLArm2/pLArm2.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	models_[LHand]->Texture("Resources/player/Parts/pLHand/pLHand.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	//右腕１・右腕２・右手
	models_[RArm1]->Texture("Resources/player/Parts/pRArm1/pRArm1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	models_[RArm2]->Texture("Resources/player/Parts/pRArm2/pRArm2.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	models_[RHand]->Texture("Resources/player/Parts/pRHand/pRHand.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	//左足首１・左足首２・左足
	models_[LLeg1]->Texture("Resources/player/Parts/pLLeg1/pLLeg1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	models_[LLeg2]->Texture("Resources/player/Parts/pLLeg/pLLeg.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	models_[LFoot]->Texture("Resources/player/Parts/pLFoot/pLFoot.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	//右足首１・右足首２・右足
	models_[RLeg1]->Texture("Resources/player/Parts/pRLeg1/pRLeg1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	models_[RLeg2]->Texture("Resources/player/Parts/pRLeg2/pRLeg2.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	models_[RFoot]->Texture("Resources/player/Parts/pRFoot/pRFoot.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");


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

void Player::Move()
{
	Vector3 move = Vector3(0.0f, 0.0f, 0.0f);
	const float speed = 0.2f;

	if (KeyInput::GetKey(DIK_W)) {
		move.z += 0.1f;
	}
	if (KeyInput::GetKey(DIK_S)) {
		move.z -= 0.1f;
	}

	if (move.x != 0.0f || move.y != 0.0f || move.z != 0.0f)
	{
		//	移動量の正規化 * speed
		move = Normalize(move) * speed;

		//	移動ベクトルをカメラの角度だけ回転させる
		move = TransformNormal(move, MakeRotateMatrix(camera_->transform.rotation_));

		//	移動方向に見た目を合わせる
		parts_[Body].rotation_.y = std::atan2f(move.x, move.z);

	}
	transform.translation_ += move;

}

void Player::CameraUpdate()
{
	Vector3 offset(0.0f, 2.0f, -20.0f);

	//	カメラの角度から回転行列を計算する
	Matrix4x4 rotate = MakeRotateMatrix(camera_->transform.rotation_);

	offset = TransformNormal(offset, rotate);
	offset.x = 0.0f;

	//	座標をコピーしてオフセット分ずらす
	camera_->transform.translation_ = transform.translation_ + offset;
}
