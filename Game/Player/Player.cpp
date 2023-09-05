#include "Player.h"
#include "Engine/Input/KeyInput/KeyInput.h"

Player::Player(std::shared_ptr<Camera> camera)
{
	camera_ = camera;
	for (uint16_t i = 0u; i < PARTS::Num; i++) {
		models_.push_back(std::make_unique<Model>());
	}
}

void Player::Initialize()
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
	models_[Body]->Texture("Resources/player/Parts/pBody.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	models_[Head]->Texture("Resources/player/Parts/pHead.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	models_[BodyUnder]->Texture("Resources/player/Parts/pBodyUnder.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	//左腕１・左腕２・左手
	models_[LArm1]->Texture("Resources/player/Parts/pLArm1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	models_[LArm2]->Texture("Resources/player/Parts/pLArm2.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	models_[LHand]->Texture("Resources/player/Parts/pLHand.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	//右腕１・右腕２・右手
	models_[RArm1]->Texture("Resources/player/Parts/pRArm1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	models_[RArm2]->Texture("Resources/player/Parts/pRArm2.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	models_[RHand]->Texture("Resources/player/Parts/pRHand.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	//左足首１・左足首２・左足
	models_[LLeg1]->Texture("Resources/player/Parts/pLLeg1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	models_[LLeg2]->Texture("Resources/player/Parts/pLLeg.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	models_[LFoot]->Texture("Resources/player/Parts/pLFoot.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	//右足首１・右足首２・右足
	models_[RLeg1]->Texture("Resources/player/Parts/pRLeg1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	models_[RLeg2]->Texture("Resources/player/Parts/pRLeg.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	models_[RFoot]->Texture("Resources/player/Parts/pRFoot.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");


	//models_[Body]->Texture("Resources/player/body.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
}

void Player::Update()
{
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
	if (KeyInput::GetKey(DIK_W)) {
		transform.translation_.z += 0.1f;
	}
	if (KeyInput::GetKey(DIK_W)) {
		transform.translation_.z += 0.1f;
	}



}

void Player::CameraUpdate()
{

}
