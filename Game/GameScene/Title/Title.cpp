#include "Game/GameScene/Title/Title.h"
#include <EEnum.h>

void Title::Initialize()
{
	parts_.resize(mobModels_.size());
	SetParts();
}

void Title::Update()
{
}

void Title::Draw(Matrix4x4 viewProjection)
{
	for (uint16_t i = 0u; i < parts_.size(); i++)
	{
		Model::ModelDraw(parts_[i], viewProjection, 0xffffffff, mobModels_[i].get());
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
