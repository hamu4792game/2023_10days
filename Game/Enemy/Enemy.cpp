#include "Enemy.h"
#include<numbers>
void Enemy::Initialize()
{
	parts_.resize(models_.size());
	for (auto& i : parts_) {
		i.parent_ = &parts_[Body];
	}
	parts_[Body].parent_ = &transform;
}

void Enemy::Initialize(int type, int num)
{
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
	for (auto& i : parts_) {
		i.parent_ = &parts_[Body];
	}
	parts_[Body].parent_ = &transform;
}

void Enemy::InitializeSP(float pos, int type, int num,//	モデルデータ配列
	std::vector<std::shared_ptr<Model>> models)
{

	models_ = models;

	type_ = type;

	transform.translation_.z = pos;
	transform.translation_.y = 6.5f;
	transform.scale_ = Vector3(0.3f, 0.3f, 0.3f);
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

	
}


void Enemy::AnimeInitialize() {
	//サイズあわせ
	nowR.resize(parts_.size());
	ESALL.resize(parts_.size());
#pragma region 大の字
	//大の字の値設定
	//サイズ設定
	sprawled.resize(parts_.size());
	sprawled[Head] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	sprawled[Body] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	sprawled[BodyUnder] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	sprawled[LArm1] = {
		{0.0f, 0.0f, -0.5f},
		{0.0f, 0.0f, -0.5f},
	};
	sprawled[LArm2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	sprawled[LHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	sprawled[RArm1] = {
		{0.0f, 0.0f, 0.5f},
		{0.0f, 0.0f, 0.5f},
	};
	sprawled[RArm2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	sprawled[RHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};


	sprawled[LLeg1] = {
		{0.0f, 0.0f, -0.8f},
		{0.0f, 0.0f, -0.8f },
	};
	sprawled[LLeg2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	sprawled[LFoot] = {
		{1.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f},
	};

	sprawled[RLeg1] = {
		{0.0f, 0.0f, 0.8f},
		{0.0f, 0.0f, 0.8f},
	};
	sprawled[RLeg2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	sprawled[RFoot] = {
		{1.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f},
	};
#pragma endregion


}

//イージングの処理、好きに中身かえてちょ
Vector3 ES(esing E, float t) {
	return {
		E.st.x * (1.0f - t) + E.ed.x * t,
		E.st.y * (1.0f - t) + E.ed.y * t,
		E.st.z * (1.0f - t) + E.ed.z * t,
	};
}

void Enemy::BlowAway() {
	//死んだときに吹っ飛びアニメーション
	if (isDead_) {
		if (!isStart_blow_away) {
			isStart_blow_away = true;
			//仮でベクトル指定
			blowVec = { 0.5f,0.5f,0 };
			//アニメ状態を最初にする
			animeState_ = MODE_A::NOMOTIAN;
			//Tをゼロに
			T_ = 0;
			SetAnimeStart = false;

			//現在の回転量の取得
			for (int i = 0; i < Num; i++) {
				//仮でいきなり手を広げた状態
				ESALL[i] = sprawled[i];
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
					parts_[Body].rotation_.z += (1.0f / 3.0f) * (float)std::numbers::pi;

				}
				break;			
			default:
				break;
			}
		}
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
