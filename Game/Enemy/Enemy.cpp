#include "Enemy.h"
#include<numbers>

void Enemy::InitializeSP(float pos, int type, int num,//	モデルデータ配列
	std::vector<std::shared_ptr<Model>> models)
{

	models_ = models;

	type_ = type;

	transform.translation_.z = pos;
	transform.translation_.y = 6.5f;
	//transform.scale_ = Vector3(0.3f, 0.3f, 0.3f);
	//transform.UpdateMatrix();

	switch (type)
	{
	case kA:
		bottomType_ = BottomTypeClass::kA;
		break;
	case kB:
		bottomType_ = BottomTypeClass::kB;
		break;
	case kX:
		bottomType_ = BottomTypeClass::kX;
		break;
	case kY:
		bottomType_ = BottomTypeClass::kY;
		break;
	default:
		bottomType_ = BottomTypeClass::kA;
		break;
	}

	num_ = num;

	parts_.resize(models_.size());
	parts_[Body].parent_ = &transform;

	//アニメ系統初期化
	AnimeInitialize();

	//ボタンの親を設定
	BottonW_.parent_ = &parts_[Body];
	BottonW_.translation_ = { 0,5,0 };

#pragma region 親子関係設定
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

	//	初期化　hamu
	lifespan = 0u;
	die_ = false;
}


void Enemy::AnimeInitialize() {
	//サイズあわせ
	nowR.resize(parts_.size());
	ESALL.resize(parts_.size());
	anoMotion.resize(parts_.size());
#pragma region 大の字
	//大の字の値設定
	//サイズ設定
	sprawled[0].resize(parts_.size());
	sprawled[0][Head] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	sprawled[0][Body] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	sprawled[0][BodyUnder] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	sprawled[0][LArm1] = {
		{0.0f, 0.0f, -0.5f},
		{0.0f, 0.0f, -0.5f},
	};
	sprawled[0][LArm2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	sprawled[0][LHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	sprawled[0][RArm1] = {
		{0.0f, 0.0f, 0.5f},
		{0.0f, 0.0f, 0.5f},
	};
	
	sprawled[0][RArm2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	sprawled[0][RHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};


	sprawled[0][LLeg1] = {
		{0.0f, 0.0f, -0.8f},
		{0.0f, 0.0f, -0.8f },
	};
	sprawled[0][LLeg2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	sprawled[0][LFoot] = {
		{1.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f},
	};

	sprawled[0][RLeg1] = {
		{0.0f, 0.0f, 0.8f},
		{0.0f, 0.0f, 0.8f},
	};
	sprawled[0][RLeg2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	sprawled[0][RFoot] = {
		{1.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f},
	};
#pragma endregion
#pragma region フィギア
	//サイズ設定
	sprawled[1].resize(parts_.size());
	sprawled[1][Head] = {
		{-0.9f, 0.0f, 0.0f},
		{-0.9f, 0.0f, 0.0f},
	};

	sprawled[1][Body] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	sprawled[1][BodyUnder] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	sprawled[1][LArm1] = {
		{0.0f, 0.0f, -1.4f},
		{0.0f, 0.0f, -1.4f},
	};
	sprawled[1][LArm2] = {
		{0.0f, 0.0f, 0.6f},
		{0.0f, 0.0f, 0.6f},
	};
	sprawled[1][LHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};


	sprawled[1][RArm1] = {
		{0.0f, 0.0f, 1.4f},
		{0.0f, 0.0f, 1.4f},
	};
	sprawled[1][RArm2] = {
		{0.0f, 0.0f, 0.6f},
		{0.0f, 0.0f, 0.6f},
	};
	sprawled[1][RHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};


	sprawled[1][LLeg1] = {
		{0.0f, -1.5f, -0.7f},
		{0.0f, -1.5f, -0.7f},
	};
	sprawled[1][LLeg2] = {
		{1.13f, 0.0f, 0.0f},
		{1.13f, 0.0f, 0.0f},
	};
	sprawled[1][LFoot] = {
		{1.5f, 0.0f, 0.0f},
		{1.5f, 0.0f, 0.0f},
	};

	sprawled[1][RLeg1] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	sprawled[1][RLeg2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	sprawled[1][RFoot] = {
		{1.5f, 0.0f, 0.0f},
		{1.5f, 0.0f, 0.0f},
	};
#pragma endregion
#pragma region リュウとび
	//大の字の値設定
	//サイズ設定
	sprawled[2].resize(parts_.size());
	sprawled[2][Head] = {
		{-0.5f, 0.7f, 0.0f},
		{-0.5f, 0.7f, 0.0f},
	};

	sprawled[2][Body] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	sprawled[2][BodyUnder] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	sprawled[2][LArm1] = {
		{1.5f,0.34f,1.36f},
		{1.5f,0.34f,1.36f},
	};
	sprawled[2][LArm2] = {
		{0.0f,0.53f,0.0f},
		{0.0f,0.53f,0.0f},

	};
	sprawled[2][LHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	sprawled[2][RArm1] = {
		{1.54f,-0.37f,-1.35f},
		{1.54f,-0.37f,-1.35f},
	};
	sprawled[2][RArm2] = {
		{0.0f, -0.6f, 0.0f},
		{0.0f, -0.6f, 0.0f},
	};
	sprawled[2][RHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};


	sprawled[2][LLeg1] = {
		{-0.06f,1.3f,0.0f},
		{-0.06f,1.3f,0.0f},
	};
	sprawled[2][LLeg2] = {
		{0.0f, -0.4f, 0.0f},
		{0.0f, -0.4f, 0.0f},
	};
	sprawled[2][LFoot] = {
		{1.3f, 0.0f, 0.0f},
		{1.3f, 0.0f, 0.0f},
	};

	sprawled[2][RLeg1] = {
		{-0.44f, -1.26f, 0.0f},
		{-0.44f, -1.26f, 0.0f},
	};
	sprawled[2][RLeg2] = {
		{0.24f, 0.0f, 0.0f},
		{0.24f, 0.0f, 0.0f},
	};
	sprawled[2][RFoot] = {
		{1.26f, 0.0f, 0.0f},
		{1.26f, 0.0f, 0.0f},
	};
#pragma endregion
#pragma region 吹っ飛び（左
	//大の字の値設定
	//サイズ設定
	sprawled[3].resize(parts_.size());
	sprawled[3][Head] = {
		{0.0f, 0.0f, -0.2f},
		{0.0f, 0.0f, -0.2f},
	};

	sprawled[3][Body] = {
		{0.0f, 0.0f, -1.0f},
		{0.0f, 0.0f, -1.0f},
	};

	sprawled[3][BodyUnder] = {
		{0.0f, 0.0f, 0.5f},
		{0.0f, 0.0f, 0.5f},
	};

	sprawled[3][LArm1] = {
		{0.0f, 2.4f, 0.34f},
		{0.0f, 2.4f, 0.34f},
	};
	sprawled[3][LArm2] = {
		{0.0f, 0.0f, 0.75f},
		{0.0f, 0.0f, 0.75f},
	};
	sprawled[3][LHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	sprawled[3][RArm1] = {
		{0.0f, 0.0f, 0.14f},
		{0.0f, 0.0f, 0.14f},
	};
	sprawled[3][RArm2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	sprawled[3][RHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};


	sprawled[3][LLeg1] = {
		{0.0f, 0.0f, 0.48f},
		{0.0f, 0.0f, 0.48f },
	};
	sprawled[3][LLeg2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	sprawled[3][LFoot] = {
		{1.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f},
	};

	sprawled[3][RLeg1] = {
		{0.0f, 0.0f, 0.66f},
		{0.0f, 0.0f, 0.66f},
	};
	sprawled[3][RLeg2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	sprawled[3][RFoot] = {
		{0.8f, 0.0f, 0.0f},
		{0.8f, 0.0f, 0.0f},
	};
#pragma endregion
#pragma region 吹っ飛び（右
	anoMotion[Body] = {
		{0.0f,0.0f,0.22f},
		{0.0f,0.0f,0.22f},
	};
	anoMotion[Head] = {
		{0.0f,0.0f,0.21f},
		{0.0f,0.0f,0.21f},
	};
	anoMotion[BodyUnder] = {
		{0.0f,0.0f,-0.53f},
		{0.0f,0.0f,-0.53f},
	};
	anoMotion[LArm1] = {
		{0.0f,0.0f,0.15f},
		{0.0f,0.0f,0.15f},
	};
	anoMotion[LArm2] = {
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
	anoMotion[LHand] = {
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	anoMotion[RArm1] = {
		{0.0f,-2.4f,-0.34f},
		{0.0f,-2.4f,-0.34f},
	};
	anoMotion[RArm2] = {
		{0.0f,-0.75f,0.0f},
		{0.0f,-0.75f,0.0f},
	};
	anoMotion[RHand] = {
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	anoMotion[LLeg1] = {
		{0.0f,0.0f,-1.1f},
		{0.0f,0.0f,-1.1f},
	};
	anoMotion[LLeg2] = {
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
	anoMotion[LFoot] = {
		{1.0f,0.0f,0.0f},
		{1.0f,0.0f,0.0f},
	};

	anoMotion[RLeg1] = {
		{0.0f,0.0f,-0.66f},
		{0.0f,0.0f,-0.66f},
	};
	anoMotion[RLeg2] = {
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
	anoMotion[RFoot] = {
		{0.79f,0.0f,0.0f},
		{0.79f,0.0f,0.0f},
	};
#pragma endregion


#pragma region 待機
	WAIT_[0].resize(parts_.size());
	WAIT_[0][Body] = {
		{0,0,0},
		{0,0,0},
	};
	WAIT_[0][BodyUnder] = {
		{0, 0, 0},
		{0, 0, 0},
	};
	WAIT_[0][Head] = {
		{0, 0, 0},
		{0, 0, 0},
	};


	//左腕
	WAIT_[0][LArm1] = {
		{0.8f, 0.8f, 1.0f},
		{0.8f, 0.8f, 1.0f},
	};
	WAIT_[0][LArm2] = {
		{0.0f, 2.0f, -3.4f},
		{0.0f, 2.0f, -3.4f},
	};
	WAIT_[0][LHand] = {
		{0,0,0},
		{0,0,0},
	};
	//右腕
	WAIT_[0][RArm1] = {
		{0.8f, -0.8f, -1.0f},
		{0.8f, -0.8f, -1.0f},
	};
	WAIT_[0][RArm2] = {
		{0.0f, -2.3f, 0.4f},
		{0.0f, -2.3f, 0.4f},
	};
	WAIT_[0][RHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	//足
	WAIT_[0][LLeg1] = {
		{0.0f, -0.18f, 0.0f},
		{0.0f, -0.18f, 0.0f},
	};
	WAIT_[0][LLeg2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	WAIT_[0][LFoot] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	WAIT_[0][RLeg1] = {
		{-0.3f, 1.0f, 0.0f},
		{-0.3f, 1.0f, 0.0f},
	};
	WAIT_[0][RLeg2] = {
		{0.3f, 0.0f, 0.0f},
		{0.3f, 0.0f, 0.0f},
	};
	WAIT_[0][RFoot] = {
		{0.13f,0.0f,0.0f},
		{-0.44f,0.0f,0.0f},
	};

	WAIT_[1].resize(parts_.size());
	WAIT_[1][Body] = {
		{0, 0, 0},
		{0, 0, 0},
	};
	WAIT_[1][BodyUnder] = {
		{0, 0, 0},
		{0, 0, 0},
	};
	WAIT_[1][Head] = {
		{0, 0, 0},
		{0, 0, 0},
	};

	WAIT_[1][LArm1] = {
		{0, 0, 1.2f},
		{0, 0, 1.2f},
	};
	WAIT_[1][LArm2] = {
		{0, 0, 0.3f},
		{0, 0, 0.3f},
	};
	WAIT_[1][LHand] = {
		{0, 0, 0},
		{0, 0, 0},
	};

	WAIT_[1][RArm1] = {
		{0, 0, -0.4f},
		{0, 0, -0.4f},
	};
	WAIT_[1][RArm2] = {
		{0, 0, -1.8f},
		{0, 0, -1.8f},
	};
	WAIT_[1][RHand] = {
		{0, 0, -1.85f},
		{0, 0, -1.85f},
	};
	WAIT_[1][LLeg1] = {
		{0,      -0.48f, -0.12f},
		{-0.39f, -0.48f, -0.12f},
	};
	WAIT_[1][LLeg2] = {
		{0.14f, 0, 0},
		{0.65f, 0, 0},
	};
	WAIT_[1][LFoot] = {
		{0,    0.03f, 0.13f},
		{0.2f, 0.03f, 0.13f},
	};

	WAIT_[1][RLeg1] = {
		{0, 0.03f, 0.13f},
		{0, 0.03f, 0.13f},
	};
	WAIT_[1][RLeg2] = {
		{0, 0, -0.15f},
		{0, 0, -0.15f},
	};
	WAIT_[1][RFoot] = {
		{0, 0.36f, 0},
		{0, 0.36f, 0},
	};

	WAIT_[2].resize(parts_.size());

	WAIT_[2][Body] = {
		{0, 0, 0},
		{0, 0, 0},
	};
	WAIT_[2][BodyUnder] = {
		{0, 0, 0},
		{0, 0, 0},
	};
	WAIT_[2][Head] = {
		{0.15f, -0.22f, 0},
		{0.15f, -0.22f, 0},
	};

	// 左腕
	WAIT_[2][LArm1] = {
		{0.01f, 0.45f, 0.03f},
		{0.01f, 0.45f, 0.03f},
	};
	WAIT_[2][LArm2] = {
		{0.0f,2.02f,0.0f},
		{0.0f,2.02f,0.0f},
	};
	WAIT_[2][LHand] = {
		{0, 0, 0.0f},
		{0, 0, 0.0f},
	};
	// 右腕
	WAIT_[2][RArm1] = {
		{0,0,-0.6f},
		{0,0,-0.6f},
	};
	WAIT_[2][RArm2] = {
		{0.0f,0.0f,-0.6f},
		{0.0f,0.0f,-0.6f},
	};
	WAIT_[2][RHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	// 足
	WAIT_[2][LLeg1] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	WAIT_[2][LLeg2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	WAIT_[2][LFoot] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	WAIT_[2][RLeg1] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	WAIT_[2][RLeg2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	WAIT_[2][RFoot] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};


	WAIT_[3].resize(parts_.size());

	WAIT_[3][Body] = {
		{0, 0, 0},
		{0, 0, 0},
	};
	WAIT_[3][BodyUnder] = {
		{0, 0, 0},
		{0, 0, 0},
	};
	WAIT_[3][Head] = {
		{0.15f, -0.22f, 0},
		{0.15f, -0.22f, 0},
	};

	// 左腕
	WAIT_[3][LArm1] = {
		{0.0f,0.0f,0.67f},
		{0.0f,0.0f,0.67f},
	};
	WAIT_[3][LArm2] = {
		{0.0f,0.0f,1.54f},
		{0.0f,0.0f,1.54f},
	};
	WAIT_[3][LHand] = {
		{0, 0, 0.0f},
		{0, 0, 0.0f},
	};
	// 右腕
	WAIT_[3][RArm1] = {
		{0.0f,0,-1.67f},
		{0.0f,0,-1.67f},
	};
	WAIT_[3][RArm2] = {
		{0.0f,-1.85f,0.0f},
		{0.0f,-1.85f,0.0f},
	};
	WAIT_[3][RHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	// 足
	WAIT_[3][LLeg1] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	WAIT_[3][LLeg2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	WAIT_[3][LFoot] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	WAIT_[3][RLeg1] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	WAIT_[3][RLeg2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	WAIT_[3][RFoot] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
#pragma endregion
#pragma region ノーマル状態
	normal_A.resize(parts_.size());

	normal_A[Body] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	normal_A[BodyUnder] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	normal_A[Head] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};


	//左腕
	normal_A[LArm1] = {
		{0.0f, -0.1f, 1.4f},
		{0.0f, 0.1f, 1.4f},
	};
	normal_A[LArm2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	normal_A[LHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	//右腕
	normal_A[RArm1] = {
		{0.0f, -0.1f, -1.4f},
		{0.0f, 0.1f, -1.4f},
	};
	normal_A[RArm2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	normal_A[RHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	//足
	normal_A[LLeg1] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	normal_A[LLeg2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	normal_A[LFoot] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	normal_A[RLeg1] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	normal_A[RLeg2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	normal_A[RFoot] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
#pragma endregion
	//初期ポーズ設定
	for (int i = 0; i < Num; i++) {

		parts_[i].rotation_ = ES(normal_A[i], 0);
	}

}


void Enemy::Die(bool left) {
	//デスフラグをON
	isDead_ = true;

	dire = left;

	//方向設定
	if (left) {
		blowVec = { 0.1f,0.1f,0.7f };
	}
	else {
		blowVec = { -0.1f,0.1f,0.7f };
	}

	//Tをゼロに
	T_ = 0;
	//WAVEの最初のはじめ
	SetAnimeStart = false;

	//animestate設定するところを通すように
	isStart_blow_away = false;
}

//イージングの処理、好きに中身かえてちょ
Vector3 ES(esing E, float t) {
	return {
		E.st.x * (1.0f - t) + E.ed.x * t,
		E.st.y * (1.0f - t) + E.ed.y * t,
		E.st.z * (1.0f - t) + E.ed.z * t,
	};
}

int GetRandomNum(int wideOrmax, bool isWide) {
	//unsigned int curtime = (unsigned int)time(nullptr);
	//srand(curtime);
	int num;
	if (isWide) {
		num = rand() % (wideOrmax * 2 + 1) - wideOrmax;
	}
	else {
		num = rand() % wideOrmax;
	}
	return num;
}

void Enemy::BlowAway() {

	
	//死んだときに吹っ飛びアニメーション
	if (isDead_) {

		if (!isStart_blow_away) {
			isStart_blow_away = true;
			//アニメ状態を最初にする
			animeState_ = MODE_A::NOMOTIAN;

			//	念のため、初期化する	hamu
			lifespan = 0u;

			ANIMETYPE = GetRandomNum(animeNUM, false);

			//ANIMETYPE = 3;

			//リュウとびの場合
			if (ANIMETYPE == 2) {
				float theta;
				if (dire) {
					theta = -(std::numbers::pi_v<float> / 4);
					transform.rotation_.y = -(std::numbers::pi_v<float> /6);
					
				}
				else {
					theta = (std::numbers::pi_v<float> / 4);
					transform.rotation_.y = (std::numbers::pi_v<float> / 6);
				}
				parts_[Body].rotation_.z = theta;
				
			}

			

			//現在の回転量の取得
			for (int i = 0; i < Num; i++) {
				//仮でいきなり手を広げた状態
				ESALL[i] = sprawled[ANIMETYPE][i];
			}

			//吹っ飛びの場合
			if (ANIMETYPE == 3) {
				if (dire) {
					for (int i = 0; i < Num; i++) {
						//仮でいきなり手を広げた状態
						ESALL[i] = anoMotion[i];
					}
				}
				else {
					for (int i = 0; i < Num; i++) {
						//仮でいきなり手を広げた状態
						ESALL[i] = sprawled[ANIMETYPE][i];
					}
				}
				

			}
		}
		else {
			switch (animeState_)
			{
			case MODE_A::WAVE1:
				break;
			case MODE_A::WAVE2:
				break;
			case MODE_A::WAVE3:
				break;
			case MODE_A::WAVE4:
				break;
			case MODE_A::NOMOTIAN:
				
				//アニメーション初期設定
				if (!SetAnimeStart) {
					SetAnimeStart = true;
					//更新
					for (int i = 0; i < Num; i++) {
						if (i != Body) {
							parts_[i].rotation_ = ES(ESALL[i], T_);
						}
					}
				}
				else {//以下アニメーション処理

					//移動量加算
					transform.translation_ += blowVec;
					//回転
					
					if (ANIMETYPE != 3) {


						if (ANIMETYPE == 1 || ANIMETYPE == 2) {
							parts_[Body].rotation_.y += (1.0f / 5.0f) * std::numbers::pi_v<float>;
						}
						else {
							parts_[Body].rotation_.z += (1.0f / 5.0f) * std::numbers::pi_v<float>;
						}
					}
				}
				break;			
			default:
				break;
			}
		}


		//	消えるまでのフレーム加算 hamu
		lifespan++;
		//	指定フレーム回したら削除フラグを建てる hamu
		if (lifespan >= 180u) {
			die_ = true;
		}


	}
	else {//生きているときのアニメーション
		switch (state_)
		{
		case Enemy::NONE:

			if (!SetAnimeStart) {
				T_ = 0;
				SetAnimeStart = true;
				//次シーンまでの空きカウント
				countAnimeMax_ = 60 + GetRandomNum(60, true);
				//加算する値
				countAnime_ = 0;
				for (int i = 0; i < Num; i++) {
					//ノーマル状態
					ESALL[i] = normal_A[i];
				}
				isLoop = false;
			
			}else{
			
				//更新
				for (int i = 0; i < Num; i++) {
					parts_[i].rotation_ = ES(ESALL[i], T_);
				}

				//ループ時の処理の変更
				if (!isLoop) {
					T_ += AddTtoWAIT_*1.0f/4.0f;
					if (T_ >= 1.0f) {
						T_ = 1.0f;
						isLoop = true;
						//
					}
				}
				else {
					T_ -= AddTtoWAIT_ * 1.0f / 3.0f;
					if (T_ <= 0.0f) {
						T_ = 0.0f;
						isLoop = false;
					}
				}

			
				if (++countAnime_ >= countAnimeMax_) {
					T_ = 0;
					SetAnimeStart = false;
					//状態をアニメーション状態に
					state_ = ONE;
					//アニメーションの段階を初期化
					mode_ = WAIT;
					//度のアニメーションをするか取得
					ANIMENUM = GetRandomNum(4, false);
				}
			}


			

			
			break;
		case Enemy::ONE:
			WaitAnimetion(ANIMENUM);
			break;
		case Enemy::TWO:
			break;
		case Enemy::TREE:
			break;
		default:
			break;
		}
	}
}

void Enemy::WaitAnimetion(int num) {
	switch (mode_)
	{
	case Enemy::WAIT:

		if (!SetAnimeStart) {
			T_ = 0;
			SetAnimeStart = true;

			GetER();
			for (int i = 0; i < Num; i++) {
				//仮でいきなり手を広げた状態
				ESALL[i] = {
					nowR[i],
					WAIT_[num][i].st,

				};
			}
		}
		else {
			//更新
			for (int i = 0; i < Num; i++) {
				if (i != Body) {
					parts_[i].rotation_ = ES(ESALL[i], T_);
				}
			}

			T_ += 1.0f / 60.0f;
			if (T_ >= 1.0f) {
				//state_ = NONE;
				T_ = 0;
				SetAnimeStart = false;
				mode_ = MOVE;
			}
		}

		break;
	case Enemy::MOVE:
		//初期化
		if (!SetAnimeStart) {
			T_ = 0;
			SetAnimeStart = true;
			for (int i = 0; i < Num; i++) {
				//仮でいきなり手を広げた状態
				ESALL[i] = WAIT_[num][i];
			}
			isLoop = false;
			countAnime_ = 0;
		}
		else {
			//更新
			for (int i = 0; i < Num; i++) {
				parts_[i].rotation_ = ES(ESALL[i], T_);
			}

			//ループ時の処理の変更
			if (!isLoop) {
				T_ += 1.0f / 30.0f;
				if (T_ >= 1.0f) {					
					T_ = 1.0f;
					isLoop = true;
					//
				}
			}
			else {
				T_ -= 1.0f / 30.0f;
				if (T_ <= 0.0f) {				
					T_ = 0.0f;
					isLoop = false;
				}
			}

			//シーン切り替え処理
			if (++countAnime_ >= 180) {
				SetAnimeStart = false;
				//state_ = NONE;
				mode_ = BACK;
			}

		}
		break;
	case Enemy::BACK:
		if (!SetAnimeStart) {
			T_ = 0;
			SetAnimeStart = true;

			GetER();
			for (int i = 0; i < Num; i++) {
				//仮でいきなり手を広げた状態
				ESALL[i] = {
					nowR[i],
					normal_A[i].st,
				};
			}
		}
		else {
			//更新
			for (int i = 0; i < Num; i++) {
				parts_[i].rotation_ = ES(ESALL[i], T_);
			}

			T_ += 1.0f / 60.0f;
			if (T_ >= 1.0f) {
				//アニメ状態をなしに
				state_ = NONE;
				T_ = 0;
				SetAnimeStart = false;
				mode_ = WAIT;
			}
		}

		break;
	default:
		break;
	}
	
}

void Enemy::ModelLoad()
{
	//models_[Body]->Texture("Resources/player/body.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
}

void Enemy::Update()
{
	
	//死んだときのアニメーション
	BlowAway();

	transform.UpdateMatrix();
	for (auto& i : parts_) {
		i.UpdateMatrix();
	}
	BottonW_.UpdateMatrix();
}


void Enemy::Draw(const Matrix4x4& viewProjection, std::vector<std::shared_ptr<Model>> botunModels)
{
	for (uint16_t i = 0u; i < parts_.size(); i++)
	{
		Model::ModelDraw(parts_[i], viewProjection, 0xffffffff, models_[i].get());
	}


	if (!isDead_) {
		//ボタン描画
		switch (type_)
		{
		case Enemy::kA:
			Model::ModelDraw(BottonW_, viewProjection, 0xffffffff, botunModels[0].get());
			break;
		case Enemy::kB:
			Model::ModelDraw(BottonW_, viewProjection, 0xffffffff, botunModels[1].get());

			break;
		case Enemy::kX:
			Model::ModelDraw(BottonW_, viewProjection, 0xffffffff, botunModels[2].get());

			break;
		case Enemy::kY:
			Model::ModelDraw(BottonW_, viewProjection, 0xffffffff, botunModels[3].get());
			break;
		default:
			break;
		}
	}
}

void Enemy::Draw(const Matrix4x4& viewProjection)
{
	for (uint16_t i = 0u; i < parts_.size(); i++)
	{
		Model::ModelDraw(parts_[i], viewProjection, 0xffffffff, models_[i].get());
	}
}