#include "Player.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include "externals/imgui/imgui.h"


#include "Game/Enemy/Enemy.h"
#include "Game/Score/Score.h"
#include "Game/GameScene/Battle/Battle.h"

#include "Engine/Input/KeyInput/KeyInput.h"
#include "Engine/Easing/Ease.h"

#include "GlobalVariables/GlobalVariables.h"

#include <algorithm>


Player::Player(std::shared_ptr<Camera> camera) : kMax_frame(60.0f)
{
	camera_ = camera;
	for (int i = 0; i < kGaugeDrawNum_; i++) {
		gaugeWorldTransforms_.push_back(std::make_shared<WorldTransform>());
	}
}

void Player::Initialize(std::vector<std::shared_ptr<Model>> models, WorldTransform* world)
{
	//  モデルの受け渡し
	models_ = models;

	parts_.resize(models_.size());

	//	世界との親子関係
	world_ = world;
	transform.parent_ = world;
	//	地面の半径 * scale
	transform.translation_.y = 6.5f;
	//transform.scale_ = Vector3(0.3f, 0.3f, 0.3f);

	//	カメラとの親子関係
	camera_->transform.parent_ = &transform;

	camera_->transform.translation_ = Vector3(18.0f, 10.0f, -20.0f);
	camera_->transform.rotation_ = Vector3(0.383f, -6.785f, 0.0f);
	
	offset = Vector2(camera_->transform.rotation_.x, camera_->transform.rotation_.y);


	//親子関係
	parts_[Body].parent_ = &transform;

	// スコアのポインタを先に取得しないとエラーになるから保留。
	//HitTestInitialize();

	//	変数の初期化
	flag = false;
	movePos = 0.0f;
	oldPos = 0.0f;
	//enemyDistance = 10.0f;
	frame = 0.0f;

	MAX_frame = kMax_frame;

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


	//アニメーションに使う値の設定
	AnimeInitialize();
}



#pragma region Animation関数
void Player::AnimeInitialize() {

	state_ =NOMOTION;

	wave_A = ATKWAIT;

	T_ = 0;

	isAnimeStart_ = false;
	//サイズあわせ
	nowR.resize(parts_.size());
	ESALL.resize(parts_.size());

#pragma region 各アニメの身体パーツ
	AnimeType[TACLE].resize(parts_.size());

	AnimeType[TACLE][Body] = {
		{0.9f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f},
	};
	AnimeType[TACLE][Head] = {
		{-0.7f, 0.0f, 0.0f},
		{-0.7f, 0.0f, 0.0f},
	};
	AnimeType[TACLE][BodyUnder] = {
		{-0.35f, 0.0f, 0.0f},
		{-0.45f, 0.0f, 0.0f},
	};

	AnimeType[0][LArm1] = {
		{0.0f, 0.7f, 1.0f},
		{0.0f, 0.8f, 1.0f},
	};
	AnimeType[0][LArm2] = {
		{0.0f, 2.4f, 0.0f},
		{0.0f, 2.4f, 0.0f},
	};
	AnimeType[0][LHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	AnimeType[0][RArm1] = {
		{0.0f, -0.7f, -1.0f},
		{0.0f, -0.8f, -1.0f},
	};
	AnimeType[0][RArm2] = {
		{0.0f, -2.4f, 0.0f},
		{0.0f, -2.4f, 0.0f},
	};
	AnimeType[0][RHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};


	AnimeType[0][LLeg1] = {
		{-2.11f,  0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	AnimeType[0][LLeg2] = {
		{2.15f,  0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	AnimeType[0][LFoot] = {
		{0.45f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	AnimeType[0][RLeg1] = {
		{0.0f,   0.0f, 0.0f},
		{-2.11f, 0.0f, 0.0f},
	};
	AnimeType[0][RLeg2] = {
		{0.0f,  0.0f, 0.0f},
		{2.15f, 0.0f, 0.0f},
	};
	AnimeType[0][RFoot] = {
		{0.0f,  0.0f, 0.0f},
		{0.45f, 0.0f, 0.0f},
	};

	AnimeType[ATK_R].resize(parts_.size());
	// 右に飛ばすパンチ
	AnimeType[1][Body] = {
		{0.0f,-0.8f,0},
		{0.0f, 1.5f, 0.0f},
	};
	AnimeType[1][Head] = {
		{0.0f, 0.95f, 0.0f},
		{0.0f, -1.11f, 0.0f},
	};
	AnimeType[1][BodyUnder] = {
		 {0.0f,0.8f,0.0f},
		{0.4f, -1.5f, 0},
	};

	AnimeType[1][LArm1] = {
		{0.0f, -0.13f, 0.0f},
		{0.0f, 0.48f, 0.0f},
	};
	AnimeType[1][LArm2] = {
		{0.0f, 2.5f, 0.0f},
		{0.0f, 1.14f, 0.0f},
	};
	AnimeType[1][LHand] = {
		{0.0f, 0.23f, 0.0f},
		{0.0f, 0.23f, 0.0f},
	};

	AnimeType[1][RArm1] = {
		{0.0f, 0.73f, -0.9f},
		{0.0f, -0.23f, 0.0f},
	};
	AnimeType[1][RArm2] = {
		{0.0f, -2.64f, 0.0f},
		{0.0f, -2.63f, 0.0f},
	};
	AnimeType[1][RHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	AnimeType[1][LLeg1] = {
	  {0.0f, 0.0f, -0.3f},
		{-2.0f, 0.0f, 0.0f},
	};
	AnimeType[1][LLeg2] = {
		  {0.0f, 0.0f, 0.2f},
		{2.5f, 0.0f, 0.0f},
	};
	AnimeType[1][LFoot] = {
	   {0.0f, 0.0f, 0.1f},
		{0.5f, 0.0f, 0.0f},
	};

	AnimeType[1][RLeg1] = {
		 {0.0f, 0.0f, 0.3f},
		{0.0f, 0.0f, 0.0f},
	};
	AnimeType[1][RLeg2] = {
		 {0.0f, 0.0f, -0.2f},
		{0.0f, 0.0f, 0.0f},
	};
	AnimeType[1][RFoot] = {
		  {0.0f, 0.0f, -0.1f},
		{0.0f, 0.0f, 0.0f},
	};

	AnimeType[KICK].resize(parts_.size());

	AnimeType[2][Body] = {
		{0.2f, 0.0f, 0.0f},
		{-0.15f, 0.0f, 0.0f},
	};
	AnimeType[2][Head] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	AnimeType[2][BodyUnder] = {
		{-0.2f, 0.0f, 0.0f},
		{0.35f, 0.0f, 0.0f},
	};

	AnimeType[2][RArm1] = {
		{0.0f, 0.0f, -1.0f},
		{0.0f, 0.9f, -1.0f},
	};
	AnimeType[2][RArm2] = {
		{0.0f, -2.3f, 0.0f},
		{0.0f, -1.7f, 0.0f},
	};
	AnimeType[2][RHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	AnimeType[2][LArm1] = {
		{0.0f, 0.0f, 1.0f},
		{0.0f, -0.9f, 1.0f},
	};
	AnimeType[2][LArm2] = {
		{0.0f, 2.3f, 0.0f},
		{0.0f, 1.7f, 0.0f},
	};
	AnimeType[2][LHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	AnimeType[2][RLeg1] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	AnimeType[2][RLeg2] = {
		{0.0f, 0.0f, 0.0f},
		{0.25f, 0.0f, 0.0f},
	};
	AnimeType[2][RFoot] = {
		{0.0f, 0.0f, 0.0f},
		{-0.35f, 0.0f, 0.0f},
	};

	AnimeType[2][LLeg1] = {
		{-2.5f, 0.0f, 0.0f},
		{-2.2f, 0.0f, 0.0f},
	};
	AnimeType[2][LLeg2] = {
		{2.6f, 0.0f, 0.0f},
		{0.3f, 0.0f, 0.0f},
	};
	AnimeType[2][LFoot] = {
		{0.8f, 0.0f, 0.0f},
		{0.1f, 0.0f, 0.0f},
	};

#pragma endregion

#pragma region 武器構え

	ATK_W.resize(parts_.size());

	ATK_W[Body] = { 0.1f, 0.0f, 0.0f };
	ATK_W[Head] = { 0.0f, 0.0f, 0.0f };
	ATK_W[BodyUnder] = { -0.1f, 0.0f, 0.0f };

	ATK_W[LArm1] = { 0.0f, 0.0f, 1.0f };
	ATK_W[LArm2] = { 0.0f, 2.3f, 0.0f };
	ATK_W[LHand] = { 0.0f, 0.0f, 0.0f };

	ATK_W[RArm1] = { 0.0f, 0.0f, -1.0f };
	ATK_W[RArm2] = { 0.0f, -2.3f, 0.0f };
	ATK_W[RHand] = { 0.0f, 0.0f, 0.0f };


	ATK_W[LLeg1] = { 0.0f, 0.0f, -0.1f };
	ATK_W[LLeg2] = { 0.0f, 0.0f, 0.1f };
	ATK_W[LFoot] = { 0.0f, 0.0f, 0.0f };

	ATK_W[RLeg1] = { 0.0f, 0.0f, 0.1f };
	ATK_W[RLeg2] = { 0.0f, 0.0f, -0.1f };
	ATK_W[RFoot] = { 0.0f, 0.0f, 0.0f };
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

#pragma region ダウン
	pDown.resize(parts_.size());
	bodyEsing = {
		{0.0f,-6.0f,0.0f},
		{0.0f, -3.0f, 0.0f},
	};
	pDown[Body] = {
		{1.75f,0.0f,0.0f},
		{0.95f,0.0f,0.0f},
	};
	pDown[Head] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},

	};
	pDown[BodyUnder] = {
		{0.0f, 0.0f, 0.0f},
		{-0.58f, 0.0f, 0.0f},
	};
	//
	pDown[LArm1] = {
		{0.0f, -0.3f, -1.4f},
		{0.58f, 0.56f,  0.0f },
	};
	pDown[LArm2] = {
		{0.0f, 0.0f, -0.3f},
		{0.59f, 2.19f, 0.0f },
	};
	pDown[LHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	//
	pDown[RArm1] = {
		{0.0f, 0.3f, 1.4f},
		{0.0f, -1.0f,  -1.3f},
	};
	pDown[RArm2] = {
		{0.0f, 0.0f, 0.3f},
		{0.0f, 0.0f, 0.0f},
	};
	pDown[RHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	//
	pDown[LLeg1] = {
		{0.2f, 0.0f, 0.0f},
		{-2.27f, 0.0f, 0.0f},
	};
	pDown[LLeg2] = {
		{0.09f, 0.0f, 0.0f},
		{1.9f,  0.0f, 0.0f},
	};
	pDown[LFoot] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	pDown[RLeg1] = {
		{0.2f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	pDown[RLeg2] = {
		{0.4f, 0.0f, 0.0f},
		{1.58f, 0.0f, 0.0f},

	};
	pDown[RFoot] = {
		{1.0f, 0.0f, 0.0f},
		{-0.17f, 0.0f, 0.0f},
	};

	bodyEsing = {
		{0.0f,-6.0f,0.0f},
		{0.0f,-3.0f,0.0f},
	};
#pragma endregion

#pragma region ラーメン食べる

	eatRamen.resize(parts_.size());
	eatRamen[Body] = {
		{0.3f, 0.0f, 0.0f},
		{0.4f, 0.0f, 0.0f},
	};
	eatRamen[Head] = {
		{0.4f, 0.0f, 0.0f},
		{0.4f, 0.0f, 0.0f},
	};
	eatRamen[BodyUnder] = {
		{-0.3f, 0.0f, 0.0f},
		{-0.4f, 0.0f, 0.0f},
	};
	//
	eatRamen[LArm1] = {
		{0.15f, 0.5f, 0.0f},
		{0.15f, 0.5f, 0.0f},
	};
	eatRamen[LArm2] = {
		{0.0f, 0.76f, 0.0f},
		{0.0f, 0.76f, 0.0f},
	};
	eatRamen[LHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	//
	eatRamen[RArm1] = {
		{0.1f, -0.13f, 0.0f},
		{-0.5f, -0.13f, 0.0f},
	};

	eatRamen[RArm2] = {
		{0.0f, -2.39f, 0.0f},
		{0.0f, -2.39f, 0.0f},
	};
	eatRamen[RHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};


	//
	eatRamen[LLeg1] = {
		{-1.5f, -0.3f, 0.0f},
		{-1.5f, -0.3f, 0.0f},
	};

	eatRamen[LLeg2] = {
		{1.5f, 0.0f, 0.0f},
		{1.5f, 0.0f, 0.0f},
	};
	eatRamen[LFoot] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	//
	eatRamen[RLeg1] = {
		{-1.5f, 0.3f, 0.0f},
		{-1.5f, 0.3f, 0.0f},
	};

	eatRamen[RLeg2] = {
		{1.5f, 0.0f, 0.0f},
		{1.5f, 0.0f, 0.0f},
	};
	eatRamen[RFoot] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
#pragma endregion




	//初期ポーズ設定
	for (int i = 0; i < Num; i++) {
		parts_[i].rotation_ = ES(normal_A[i], 0);
	}
}

//現在のプレイヤーの情報取得
void Player::GetplayerR() {
	for (int i = 0; i < PARTS::Num; i++) {
		nowR[i] = parts_[i].rotation_;
	}
}



void Player::Animetion() {

	switch (state_)
	{
	case Player::NOMOTION:
		//ミスだった場合
		if (score_->GetEvaluation() == Score::Evaluation::kMiss) {
			state_ = MISS_;
			isAnimeStart_ = false;
			wave_A = ATKWAIT;
			//ミスじゃなっかった場合
		}else if (score_->GetEvaluation()) {
			state_ = PUNCH;
			isAnimeStart_ = false;
			wave_A = ATKWAIT;
			ANIMENUM = GetRandomNum(A_NUM, false);
		}
		break;
	case Player::PUNCH://パンチまとめ
		ATK_R_F(ANIMENUM);
		break;
	case Player::MISS_:
		PDown();
		break;
	default:
		break;
	}

	
}

//パンチ攻撃
void Player::ATK_R_F(int num) {

	switch (wave_A)
	{
	case Player::ATKWAIT:
		if (!isAnimeStart_) {
			isAnimeStart_ = true;

			GetplayerR();

			//現在の回転量の取得
			for (int i = 0; i < Num; i++) {
				//仮でいきなり手を広げた状態
				ESALL[i] = {
					nowR[i],
					AnimeType[num][i].st
				};
			}
		}
		else {
			for (int i = 0; i < Num; i++) {
				parts_[i].rotation_ = ES(ESALL[i], T_);
			}

			//予備動作無し
			T_ += 1.0f/3.0f;
			if (T_ >= 1.0f) {
				wave_A = ATK;
				T_ = 0;
				isAnimeStart_ = false;
			}

		}

		break;
	case Player::ATK:
		if (!isAnimeStart_) {
			isAnimeStart_ = true;

			GetplayerR();

			//現在の回転量の取得
			for (int i = 0; i < Num; i++) {
				//仮でいきなり手を広げた状態
				ESALL[i] = {
					nowR[i],
					AnimeType[num][i].ed,
				};
			}
		}
		else {
			//イージング更新
			for (int i = 0; i < Num; i++) {
				parts_[i].rotation_ = ES(ESALL[i], T_);
			}
			//Tを加算
			T_ += AddT_*scaleSPD;
			//シーン切り替え処理
			if (T_ >= 1.0f) {
				wave_A = BACK;
				T_ = 0;
				isAnimeStart_ = false;
			}

		}
		break;
	case Player::BACK:
		if (!isAnimeStart_) {
			isAnimeStart_ = true;

			GetplayerR();

			//現在の回転量の取得
			for (int i = 0; i < Num; i++) {
				
				ESALL[i] = {
					nowR[i],
					ATK_W[i]
				};
			}
		}
		else {
			//イージング更新
			for (int i = 0; i < Num; i++) {
				parts_[i].rotation_ = ES(ESALL[i], T_);
			}
			//Tを加算
			T_ += AddT_;
			//シーン切り替え処理
			if (T_ >= 1.0f) {
				wave_A = ATKWAIT;
				state_ = NOMOTION;
				T_ = 0;
				isAnimeStart_ = false;
			}

		}
		break;
	default:
		break;
	}

}

//
void Player::PDown() {
	switch (wave_A)
	{
	case Player::ATKWAIT:
		if (!isAnimeStart_) {
			isAnimeStart_ = true;
			T_ = 0;

			//現在の回転量の取得
			GetplayerR();
			for (int i = 0; i < Num; i++) {
				//ダウンポーズ設定
				ESALL[i] = {
					nowR[i],
					pDown[i].st
				};
			}
			//ボディの座標も動かすので設定
			BDE = {
				parts_[Body].translation_,
				bodyEsing.st
			};

			easeNowFrame = 0.0f;
			easeMaxFrame = MAX_frame / 5.0f;
		}
		break;
	case Player::ATK:
		if (!isAnimeStart_) {
			isAnimeStart_ = true;
			T_ = 0;

			//現在の回転量の取得
			//GetplayerR();
			for (int i = 0; i < Num; i++) {
				//ダウンポーズ設定
				ESALL[i] = pDown[i];			
			}
			//ボディの座標も動かすので設定
			BDE = bodyEsing;

			easeMaxFrame = MAX_frame / 3.0f - easeNowFrame;
			easeNowFrame = 0.0f;
		}
		break;
	case Player::BACK:
		if (!isAnimeStart_) {
			isAnimeStart_ = true;
			T_ = 0;

			//現在の回転量の取得
			//GetplayerR();
			for (int i = 0; i < Num; i++) {
				//ダウンポーズ設定
				ESALL[i] = { 
					pDown[i].ed,
					ATK_W[i]
				};
			}
			//ボディの座標も動かすので設定
			BDE = { 
				bodyEsing.ed,
				{0.0f,0.0f,0.0f}
			};

			easeMaxFrame = MAX_frame / 3.0f;
			easeNowFrame = 0.0f;
		}
		break;
	default:
		break;
	}

	easeNowFrame += 1.0f * Battle::masterSpeed;
	float tn = Ease::MakeEaseT(easeNowFrame, easeMaxFrame, Ease::EaseType::Constant);
	//更新
	for (int i = 0; i < Num; i++) {
		parts_[i].rotation_ = Ease::UseEase(ESALL[i].st, ESALL[i].ed, tn);
	}
	parts_[Body].translation_ = Ease::UseEase(BDE.st, BDE.ed, tn);

	switch (wave_A)
	{
	case Player::ATKWAIT:
		//シーン切り替え処理
		if (easeNowFrame >= easeMaxFrame) {
			wave_A = ATK;
			//state_ = NOMOTION;
			T_ = 0;
			isAnimeStart_ = false;
		}
		break;
	case Player::ATK:
		//シーン切り替え処理
		if (easeNowFrame >= easeMaxFrame) {
			wave_A = BACK;
			//state_ = NOMOTION;
			T_ = 0;
			isAnimeStart_ = false;
		}
		break;
	case Player::BACK:
		//シーン切り替え処理
		if (easeNowFrame >= easeMaxFrame) {
			wave_A = ATKWAIT;
			state_ = NOMOTION;
			T_ = 0;
			isAnimeStart_ = false;
			parts_[Body].translation_ = { 0.0f,0.0f,0.0f };
		}
		break;
	}

}
#pragma endregion

void Player::Update()
{

	camera_->transform.rotation_ = Vector3(offset.x, offset.y, camera_->transform.rotation_.z);

	ImGui::DragFloat3("cameraTrans", &camera_->transform.translation_.x, 1.0f);
	ImGui::DragFloat3("cameraRotate", &camera_->transform.rotation_.x, 0.1f);


	//	待機時間
	//	移動処理
	MoveType2();

	Animetion();

	transform.UpdateMatrix();
	for (auto& i : parts_) {
		i.UpdateMatrix();
	}

	//camera_->transform.UpdateMatrix();
}

void Player::HitTestInitialize() {
	evalutionCount_ = 0.0f;
	intervalCount_ = 0;
	//score->ResetEvalution();
}

void Player::HitEvalution(Enemy* enemy) {


	if (evalutionCount_ <= kEvalutionframe_[kPerfect]) {

		enemy->Die(1);
		score_->AddPerfect();
		memoFrame_ = evalutionCount_;
		evalutionCount_ = 0;

	}
	else if (evalutionCount_ <= kEvalutionframe_[kGreat]) {

		enemy->Die(0);
		score_->AddGreat();
		memoFrame_ = evalutionCount_;
		evalutionCount_ = 0;

	}
	else if (evalutionCount_ <= kEvalutionframe_[kGood]) {

		enemy->Die(1);
		score_->AddGood();
		memoFrame_ = evalutionCount_;
		evalutionCount_ = 0;
	}
	else if (evalutionCount_ > kEvalutionframe_[kGood]) {
		enemy->Die(1);
		score_->AddMiss();
		memoFrame_ = float(kEvalutionframe_[kGood]);
		evalutionCount_ = 0;
	}

}

//	使ってない
void Player::Move() {

	//	攻撃(入力)された時
	if ((KeyInput::PushKey(DIK_SPACE) && !flag) || score_->GetEvaluation()) {
		flag = true;
	}
	if (flag) {
		//	フレームを最大にする
		//frame = MAX_frame;
		Battle::masterSpeed = 1.0f;
	}
	//	frame加算処理 通常加算速度 * 全体のframe速度
	frame += 1.0f * Battle::masterSpeed;


	//	攻撃をするまでの移動処理
	transform.translation_.z = Ease::UseEase(oldPos, movePos, frame, MAX_frame, Ease::EaseType::EaseOutSine);
	//	最大まで移動したら初期化(次の敵への準備)処理
	if (frame >= MAX_frame) {
		//	座標の更新
		oldPos = transform.translation_.z;
		//	敵の間隔分足す
		movePos += enemyDistance;
		//	frameの初期化
		frame = 0.0f;

		Battle::masterSpeed = 0.3f;

		//	フラグを折る
		flag = false;
	}
}

void Player::MoveType2() {
	//	frame加算処理 通常加算速度 * 全体のframe速度
	//frame += 1.0f * Battle::masterSpeed;

	//	最大フレーム数をコンボに応じて減少
	MAX_frame = kMax_frame - static_cast<float>(score_->GetCombo());
	MAX_frame = std::clamp<float>(MAX_frame, 30.0f, 60.0f);

	if (flag) {
		
		if (/*KeyInput::PushKey(DIK_SPACE) || */score_->GetEvaluation()) {
			//	座標の更新
			oldPos = transform.translation_.z;
			//	敵の間隔分足す
			movePos += enemyDistance;
			//	frameの初期化
			frame = 0.0f;

			flag = false;
			Battle::masterSpeed = 1.0f;
			shakeFlag = false;

			if (score_->GetEvaluation() == Score::Evaluation::kPerfect || score_->GetEvaluation() == Score::Evaluation::kGreat) {
				shakeFlag = true;
			}
			else if (score_->GetEvaluation() == Score::Evaluation::kMiss) {
				Battle::masterSpeed = 0.6f;
			}
		}
	}
	else {
		//	frame加算処理 通常加算速度 * 全体のframe速度
		frame += 1.0f * Battle::masterSpeed;
		//	攻撃をするまでの移動処理
		transform.translation_.z = Ease::UseEase(oldPos, movePos, frame, MAX_frame, Ease::EaseType::EaseOutSine);
		if (frame >= MAX_frame) {
			flag = true;
		}
		//	パーフェクトの瞬間だけシェイク
		else if (shakeFlag && frame < 5.0f) {
			CameraShake();
		}
	}
}

void Player::CameraShake()
{

	Vector2 ran(0.0f, 0.0f);
	ran.x = static_cast<float>(std::rand() % 3 - 1);
	ran.y = static_cast<float>(std::rand() % 3 - 1);

	ran.x = AngleToRadian(ran.x);
	ran.y = AngleToRadian(ran.y);

	camera_->transform.rotation_.x = offset.x + ran.x;
	camera_->transform.rotation_.y = offset.y + ran.y;

}

void Player::HitTest(Enemy* enemy) {

	score_->ResetEvalution();

	if (flag) {

		int combo = std::clamp(score_->GetCombo(), 0, kMaxSpeedCombNum_);

		// ここの加算の割合を上げれば加速する
		evalutionCount_ += Ease::UseEase(1.0f, kMaxAddFrame_, combo, kMaxSpeedCombNum_, Ease::Constant);

		if (KeyInput::GetInstance()->GetPadConnect()) {

			if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_A) ||
				KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_DPAD_DOWN)) {
				if (enemy->GetBottomType() == Enemy::BottomTypeClass::kA) {

					HitEvalution(enemy);
				}
				else {
					enemy->Die(0);
					score_->AddMiss();
					memoFrame_ = evalutionCount_;
					evalutionCount_ = 0;
				}

			}
			else if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_B) || 
				KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) {
				if (enemy->GetBottomType() == Enemy::BottomTypeClass::kB) {
					HitEvalution(enemy);
				}
				else {
					enemy->Die(1);
					score_->AddMiss();
					memoFrame_ = evalutionCount_;
					evalutionCount_ = 0;
				}
			}
			else if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_X) ||
				KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)) {
				if (enemy->GetBottomType() == Enemy::BottomTypeClass::kX) {
					HitEvalution(enemy);
				}
				else {
					enemy->Die(1);
					score_->AddMiss();
					memoFrame_ = evalutionCount_;
					evalutionCount_ = 0;
				}
			}
			else if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_Y) ||
				KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_DPAD_UP)) {
				if (enemy->GetBottomType() == Enemy::BottomTypeClass::kY) {
					HitEvalution(enemy);
				}
				else {
					enemy->Die(0);
					score_->AddMiss();
					memoFrame_ = evalutionCount_;
					evalutionCount_ = 0;
				}
			}
		}

		if (KeyInput::PushKey(DIK_UPARROW)) {
			if (enemy->GetBottomType() == Enemy::BottomTypeClass::kY) {
				HitEvalution(enemy);
			}
			else {
				enemy->Die(0);
				score_->AddMiss();
				memoFrame_ = evalutionCount_;
				evalutionCount_ = 0.0f;
			}
		}
		else if (KeyInput::PushKey(DIK_DOWNARROW)) {
			if (enemy->GetBottomType() == Enemy::BottomTypeClass::kA) {

				HitEvalution(enemy);
			}
			else {
				enemy->Die(0);
				score_->AddMiss();
				memoFrame_ = evalutionCount_;
				evalutionCount_ = 0.0f;
			}
		}
		else if (KeyInput::PushKey(DIK_LEFTARROW)) {
			if (enemy->GetBottomType() == Enemy::BottomTypeClass::kX) {
				HitEvalution(enemy);
			}
			else {
				enemy->Die(1);
				score_->AddMiss();
				memoFrame_ = evalutionCount_;
				evalutionCount_ = 0.0f;
			}
		}
		else if (KeyInput::PushKey(DIK_RIGHTARROW)) {
			if (enemy->GetBottomType() == Enemy::BottomTypeClass::kB) {
				HitEvalution(enemy);
			}
			else {
				enemy->Die(1);
				score_->AddMiss();
				memoFrame_ = evalutionCount_;
				evalutionCount_ = 0.0f;
			}
		}

		if (evalutionCount_ > kEvalutionframe_[kGood] && *tutorialFlag_ == false) {

			score_->AddMiss();
			enemy->Die(1);
			memoFrame_ = evalutionCount_;
			evalutionCount_ = 0;
		}
		else if (evalutionCount_ > kEvalutionframe_[kGood]) {

			evalutionCount_ = float(kEvalutionframe_[kGood]);
		}

	}
}

void Player::GaugeInitialize() {

	gaugeIsDraw_[GaugeDrawEnum::kBack] = true;
	gaugeIsDraw_[GaugeDrawEnum::kGaugeBack] = true;
	gaugeIsDraw_[GaugeDrawEnum::kGaugePerfect] = true;
	gaugeIsDraw_[GaugeDrawEnum::kGaugeGreat] = true;
	gaugeIsDraw_[GaugeDrawEnum::kGaugeGood] = true;
	gaugeIsDraw_[GaugeDrawEnum::kMark] = false;

	gaugeColors_[GaugeDrawEnum::kBack] = 0x222222EE;
	gaugeColors_[GaugeDrawEnum::kGaugeBack] = 0x555555EE;

	gaugeColors_[GaugeDrawEnum::kGaugePerfect] = 0xEEEEEEEE;
	gaugeColors_[GaugeDrawEnum::kGaugeGreat] = 0xEEEE11EE;
	gaugeColors_[GaugeDrawEnum::kGaugeGood] = 0x33FF55EE;

	gaugeColors_[GaugeDrawEnum::kMark] = 0x000000FF;

	evalutionCount_ = 0;

	memoF_ = 0.0f;

	SetGaugeWorldTransform({ kBasePos_.x,kBasePos_.y }, { kBaseScale_ + kBaseWhiteSpace_,kBaseScaleY_ + kBaseWhiteSpace_ }, 0.0f, GaugeDrawEnum::kBack);
	SetGaugeWorldTransform({ kBasePos_.x,kBasePos_.y }, { kBaseScale_,kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeBack);
	SetGaugeWorldTransform({ kGaugeStartPos_[Evalution::kPerfect],kBasePos_.y }, { kGaugeScale_[Evalution::kPerfect],kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugePerfect);
	SetGaugeWorldTransform({ kGaugeStartPos_[Evalution::kGreat],kBasePos_.y }, { kGaugeScale_[Evalution::kGreat],kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeGreat);
	SetGaugeWorldTransform({ kGaugeStartPos_[Evalution::kGood],kBasePos_.y }, { kGaugeScale_[Evalution::kGood],kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeGood);

	SetGaugeWorldTransform(kGaugeMarkPos_[0], { kGaugeMarkScale_,kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kMark);

	SetKoroneGlobalVariable();
}

void Player::SetKoroneGlobalVariable() {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "Gauge";

	globalVariables->CreateGroup(groupName);

	globalVariables->AddItem(groupName, "pos", kBasePos_);
	globalVariables->AddItem(groupName, "scaleX", kBaseScale_);
	globalVariables->AddItem(groupName, "scaleY", kBaseScaleY_);
	globalVariables->AddItem(groupName, "whiteSpace", kBaseWhiteSpace_);
	globalVariables->AddItem(groupName, "markScale", kGaugeMarkScale_);
	
	globalVariables->AddItem(groupName, "maxSpeed", kMaxAddFrame_);
	globalVariables->AddItem(groupName, "maxSpeedCombo", kMaxSpeedCombNum_);
	
	globalVariables->CreateGroup("EvalutionFrame");

	globalVariables->AddItem("EvalutionFrame", "perfect", kEvalutionframe_[Evalution::kPerfect]);
	globalVariables->AddItem("EvalutionFrame", "great", kEvalutionframe_[Evalution::kGreat]);
	globalVariables->AddItem("EvalutionFrame", "good", kEvalutionframe_[Evalution::kGood]);

	ApplyKoroneGlobalVariable();
}

void Player::ApplyKoroneGlobalVariable() {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Gauge";

	kBasePos_ = globalVariables->GetVector2Value(groupName, "pos");
	kBaseScale_ = globalVariables->GetFloatValue(groupName, "scaleX");
	kBaseScaleY_ = globalVariables->GetFloatValue(groupName, "scaleY");
	kBaseWhiteSpace_ = globalVariables->GetFloatValue(groupName, "whiteSpace");
	kGaugeMarkScale_ = globalVariables->GetFloatValue(groupName, "markScale");

	kMaxAddFrame_ = globalVariables->GetFloatValue(groupName, "maxSpeed");
	kMaxSpeedCombNum_ = globalVariables->GetIntValue(groupName, "maxSpeedCombo");

	kEvalutionframe_[Evalution::kPerfect] = globalVariables->GetIntValue("EvalutionFrame", "perfect");
	kEvalutionframe_[Evalution::kGreat] = globalVariables->GetIntValue("EvalutionFrame", "great");
	kEvalutionframe_[Evalution::kGood] = globalVariables->GetIntValue("EvalutionFrame", "good");
	kEvalutionframe_[Evalution::kMiss] = kEvalutionframe_[Evalution::kGood] + 1;

	SetFloatTransform();
}

void Player::SetGaugeWorldTransform(const Vector2& screenPos, const Vector2& scale, float rotate, int gaugeEnumName) {

	Vector2 pos = { screenPos.x - WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2 - screenPos.y };

	gaugeWorldTransforms_[gaugeEnumName]->translation_.x = pos.x;
	gaugeWorldTransforms_[gaugeEnumName]->translation_.y = pos.y;

	gaugeWorldTransforms_[gaugeEnumName]->rotation_.z = rotate;

	gaugeWorldTransforms_[gaugeEnumName]->scale_.x = scale.x;
	gaugeWorldTransforms_[gaugeEnumName]->scale_.y = scale.y;

	gaugeWorldTransforms_[gaugeEnumName]->UpdateMatrix();
}

void Player::SetFloatTransform() {

	kGaugeMarkPos_[0] = { kBasePos_.x - kTextureSize_ / 2 * kBaseScale_ + kTextureSize_ / 2 * kGaugeMarkScale_, kBasePos_.y };
	kGaugeMarkPos_[1] = { kBasePos_.x + kTextureSize_ / 2 * kBaseScale_ - kTextureSize_ / 2 * kGaugeMarkScale_, kBasePos_.y };

	SetGaugeWorldTransform({ kBasePos_.x,kBasePos_.y }, { kBaseScale_ + kBaseWhiteSpace_,kBaseScaleY_ + kBaseWhiteSpace_ }, 0.0f, GaugeDrawEnum::kBack);
	SetGaugeWorldTransform({ kBasePos_.x,kBasePos_.y }, { kBaseScale_,kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeBack);

	kGaugeScale_[Evalution::kPerfect] = kBaseScale_ * kEvalutionframe_[Evalution::kPerfect] / kEvalutionframe_[Evalution::kGood];
	kGaugeScale_[Evalution::kGreat] = kBaseScale_ * (kEvalutionframe_[Evalution::kGreat] - kEvalutionframe_[Evalution::kPerfect]) / kEvalutionframe_[Evalution::kGood];
	kGaugeScale_[Evalution::kGood] = kBaseScale_ * (kEvalutionframe_[Evalution::kGood] - kEvalutionframe_[Evalution::kGreat]) / kEvalutionframe_[Evalution::kGood];

	kGaugeStartPos_[Evalution::kPerfect] = kBasePos_.x - kTextureSize_ / 2 * kBaseScale_ + kTextureSize_ / 2 * kGaugeScale_[Evalution::kPerfect];
	kGaugeStartPos_[Evalution::kGreat] = kGaugeStartPos_[Evalution::kPerfect] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kPerfect] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kGreat];
	kGaugeStartPos_[Evalution::kGood] = kGaugeStartPos_[Evalution::kGreat] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kGreat] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kGood];

	kGaugeEndPos_[Evalution::kPerfect] = kGaugeStartPos_[Evalution::kPerfect] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kPerfect];
	kGaugeEndPos_[Evalution::kGreat] = kGaugeStartPos_[Evalution::kGreat] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kGreat];
	kGaugeEndPos_[Evalution::kGood] = kGaugeStartPos_[Evalution::kGood] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kGood];


}

void Player::GaugeUpdate() {

	ApplyKoroneGlobalVariable();


	if (flag) {

		if (score_->GetEvaluation()) {

			memoF_ = memoFrame_;

			if (memoFrame_ < kEvalutionframe_[Evalution::kPerfect]) {

				float pos = Ease::UseEase(kGaugeStartPos_[Evalution::kPerfect], kGaugeEndPos_[Evalution::kPerfect], memoFrame_, kEvalutionframe_[kPerfect], Ease::EaseType::Constant);
				float scale = Ease::UseEase(kGaugeScale_[Evalution::kPerfect], 0.0f, memoFrame_, kEvalutionframe_[Evalution::kPerfect], Ease::EaseType::Constant);

				SetGaugeWorldTransform({ pos,kBasePos_.y }, { scale,kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugePerfect);
			}
			else if (memoFrame_ >= kEvalutionframe_[kPerfect]) {
				gaugeIsDraw_[GaugeDrawEnum::kGaugePerfect] = false;
			}
			if (memoFrame_ < kEvalutionframe_[Evalution::kGreat]) {

				float pos = Ease::UseEase(kGaugeStartPos_[Evalution::kGreat], kGaugeEndPos_[Evalution::kGreat], memoFrame_ - kEvalutionframe_[Evalution::kPerfect], kEvalutionframe_[Evalution::kGreat] - kEvalutionframe_[Evalution::kPerfect], Ease::EaseType::Constant);
				float scale = Ease::UseEase(kGaugeScale_[Evalution::kGreat], 0.0f, memoFrame_ - kEvalutionframe_[Evalution::kPerfect], kEvalutionframe_[Evalution::kGreat] - kEvalutionframe_[Evalution::kPerfect], Ease::EaseType::Constant);

				SetGaugeWorldTransform({ pos,kBasePos_.y }, { scale,kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeGreat);
			}
			else if (memoFrame_ >= kEvalutionframe_[Evalution::kGreat]) {
				gaugeIsDraw_[GaugeDrawEnum::kGaugeGreat] = false;
			}
			if (memoFrame_ < kEvalutionframe_[Evalution::kGood]) {

				float pos = Ease::UseEase(kGaugeStartPos_[Evalution::kGood], kGaugeEndPos_[Evalution::kGood], memoFrame_ - kEvalutionframe_[Evalution::kGreat], kEvalutionframe_[Evalution::kGood] - kEvalutionframe_[Evalution::kGreat], Ease::EaseType::Constant);
				float scale = Ease::UseEase(kGaugeScale_[Evalution::kGood], 0.0f, memoFrame_ - kEvalutionframe_[Evalution::kGreat], kEvalutionframe_[Evalution::kGood] - kEvalutionframe_[Evalution::kGreat], Ease::EaseType::Constant);

				SetGaugeWorldTransform({ pos,kBasePos_.y }, { scale,kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeGood);
			}
			else if (memoFrame_ >= kEvalutionframe_[Evalution::kGood]) {
				gaugeIsDraw_[GaugeDrawEnum::kGaugeGood] = false;
			}
		}
		else {

			SetGaugeWorldTransform({ kGaugeStartPos_[Evalution::kPerfect],kBasePos_.y }, { kGaugeScale_[Evalution::kPerfect],kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugePerfect);
			SetGaugeWorldTransform({ kGaugeStartPos_[Evalution::kGreat],kBasePos_.y }, { kGaugeScale_[Evalution::kGreat],kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeGreat);
			SetGaugeWorldTransform({ kGaugeStartPos_[Evalution::kGood],kBasePos_.y }, { kGaugeScale_[Evalution::kGood],kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeGood);

			int combo = std::clamp(score_->GetCombo(), 0, kMaxSpeedCombNum_);

			if (evalutionCount_ == Ease::UseEase(1.0f, kMaxAddFrame_, combo, kMaxSpeedCombNum_, Ease::Constant)) {
				gaugeIsDraw_[GaugeDrawEnum::kGaugePerfect] = true;
				gaugeIsDraw_[GaugeDrawEnum::kGaugeGreat] = true;
				gaugeIsDraw_[GaugeDrawEnum::kGaugeGood] = true;
			}

			if (evalutionCount_ < kEvalutionframe_[Evalution::kPerfect]) {

				float pos = Ease::UseEase(kGaugeStartPos_[Evalution::kPerfect], kGaugeEndPos_[Evalution::kPerfect], evalutionCount_, kEvalutionframe_[kPerfect], Ease::EaseType::Constant);
				float scale = Ease::UseEase(kGaugeScale_[Evalution::kPerfect], 0.0f, evalutionCount_, kEvalutionframe_[Evalution::kPerfect], Ease::EaseType::Constant);

				SetGaugeWorldTransform({ pos,kBasePos_.y }, { scale,kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugePerfect);
			}
			else if (evalutionCount_ >= kEvalutionframe_[kPerfect]) {
				gaugeIsDraw_[GaugeDrawEnum::kGaugePerfect] = false;
			}
			if (evalutionCount_ < kEvalutionframe_[Evalution::kGreat]) {

				float pos = Ease::UseEase(kGaugeStartPos_[Evalution::kGreat], kGaugeEndPos_[Evalution::kGreat], evalutionCount_ - kEvalutionframe_[Evalution::kPerfect], kEvalutionframe_[Evalution::kGreat] - kEvalutionframe_[Evalution::kPerfect], Ease::EaseType::Constant);
				float scale = Ease::UseEase(kGaugeScale_[Evalution::kGreat], 0.0f, evalutionCount_ - kEvalutionframe_[Evalution::kPerfect], kEvalutionframe_[Evalution::kGreat] - kEvalutionframe_[Evalution::kPerfect], Ease::EaseType::Constant);

				SetGaugeWorldTransform({ pos,kBasePos_.y }, { scale,kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeGreat);
			}
			else if (evalutionCount_ >= kEvalutionframe_[Evalution::kGreat]) {
				gaugeIsDraw_[GaugeDrawEnum::kGaugeGreat] = false;
			}
			if (evalutionCount_ < kEvalutionframe_[Evalution::kGood]) {

				float pos = Ease::UseEase(kGaugeStartPos_[Evalution::kGood], kGaugeEndPos_[Evalution::kGood], evalutionCount_ - kEvalutionframe_[Evalution::kGreat], kEvalutionframe_[Evalution::kGood] - kEvalutionframe_[Evalution::kGreat], Ease::EaseType::Constant);
				float scale = Ease::UseEase(kGaugeScale_[Evalution::kGood], 0.0f, evalutionCount_ - kEvalutionframe_[Evalution::kGreat], kEvalutionframe_[Evalution::kGood] - kEvalutionframe_[Evalution::kGreat], Ease::EaseType::Constant);

				SetGaugeWorldTransform({ pos,kBasePos_.y }, { scale,kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeGood);
			}
			else if (evalutionCount_ >= kEvalutionframe_[Evalution::kGood]) {
				gaugeIsDraw_[GaugeDrawEnum::kGaugeGood] = false;
			}


		}

	}
	else {

		memoFrame_ = Ease::UseEase(memoF_, 0.0f, frame, MAX_frame, Ease::Constant);


		if (memoFrame_ < kEvalutionframe_[Evalution::kPerfect]) {
			gaugeIsDraw_[GaugeDrawEnum::kGaugePerfect] = true;
			float pos = Ease::UseEase(kGaugeStartPos_[Evalution::kPerfect], kGaugeEndPos_[Evalution::kPerfect], memoFrame_, kEvalutionframe_[kPerfect], Ease::EaseType::Constant);
			float scale = Ease::UseEase(kGaugeScale_[Evalution::kPerfect], 0.0f, memoFrame_, kEvalutionframe_[Evalution::kPerfect], Ease::EaseType::Constant);

			SetGaugeWorldTransform({ pos,kBasePos_.y }, { scale,kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugePerfect);
		}
		else if (memoFrame_ >= kEvalutionframe_[kPerfect]) {
			gaugeIsDraw_[GaugeDrawEnum::kGaugePerfect] = false;
		}
		if (memoFrame_ < kEvalutionframe_[Evalution::kGreat]) {
			gaugeIsDraw_[GaugeDrawEnum::kGaugeGreat] = true;
			float pos = Ease::UseEase(kGaugeStartPos_[Evalution::kGreat], kGaugeEndPos_[Evalution::kGreat], memoFrame_ - kEvalutionframe_[Evalution::kPerfect], kEvalutionframe_[Evalution::kGreat] - kEvalutionframe_[Evalution::kPerfect], Ease::EaseType::Constant);
			float scale = Ease::UseEase(kGaugeScale_[Evalution::kGreat], 0.0f, memoFrame_ - kEvalutionframe_[Evalution::kPerfect], kEvalutionframe_[Evalution::kGreat] - kEvalutionframe_[Evalution::kPerfect], Ease::EaseType::Constant);

			SetGaugeWorldTransform({ pos,kBasePos_.y }, { scale,kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeGreat);
		}
		else if (memoFrame_ >= kEvalutionframe_[Evalution::kGreat]) {
			gaugeIsDraw_[GaugeDrawEnum::kGaugeGreat] = false;
		}
		if (memoFrame_ < kEvalutionframe_[Evalution::kGood]) {
			gaugeIsDraw_[GaugeDrawEnum::kGaugeGood] = true;
			float pos = Ease::UseEase(kGaugeStartPos_[Evalution::kGood], kGaugeEndPos_[Evalution::kGood], memoFrame_ - kEvalutionframe_[Evalution::kGreat], kEvalutionframe_[Evalution::kGood] - kEvalutionframe_[Evalution::kGreat], Ease::EaseType::Constant);
			float scale = Ease::UseEase(kGaugeScale_[Evalution::kGood], 0.0f, memoFrame_ - kEvalutionframe_[Evalution::kGreat], kEvalutionframe_[Evalution::kGood] - kEvalutionframe_[Evalution::kGreat], Ease::EaseType::Constant);

			SetGaugeWorldTransform({ pos,kBasePos_.y }, { scale,kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeGood);
		}
		else if (memoFrame_ >= kEvalutionframe_[Evalution::kGood]) {
			gaugeIsDraw_[GaugeDrawEnum::kGaugeGood] = false;
		}
	}

}

void Player::MarkUpdate() {

	int combo = std::clamp(score_->GetCombo(), 0, kMaxMarkFrame_);

	markCount_ += Ease::UseEase(1.0f, kMaxAddFrame_, combo, kMaxMarkFrame_, Ease::Constant);

	float easeFrame = std::clamp<float>(memoFrame_, 0.0f, float(kEvalutionframe_[Evalution::kGood]));

	if (score_->GetEvaluation()) {
		markCount_ = 0;

		SetGaugeWorldTransform(Ease::UseEase(kGaugeMarkPos_[0], kGaugeMarkPos_[1], easeFrame, kEvalutionframe_[Evalution::kGood], Ease::Constant),
			{ kGaugeMarkScale_,kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kMark);
	}

	if (markCount_ > kMaxMarkFrame_) {
		markCount_ = float(kMaxMarkFrame_);
	}

}

void Player::GaugeDraw(const Matrix4x4& viewProjection, int gaugeEnumName) {

	if (gaugeIsDraw_[gaugeEnumName]) {

		if (gaugeEnumName == Player::GaugeDrawEnum::kBack) {
			Texture2D::TextureDraw(*(gaugeWorldTransforms_[gaugeEnumName].get()), viewProjection, gaugeColors_[gaugeEnumName], gaugeTextures_[GaugeResource::kBackResource].get());
		}
		else {
			Texture2D::TextureDraw(*(gaugeWorldTransforms_[gaugeEnumName].get()), viewProjection, gaugeColors_[gaugeEnumName], gaugeTextures_[GaugeResource::kGaugeResource].get());
		}

	}

}

void Player::GaugeDraw2D(const Matrix4x4& viewProjection) {

	for (int i = 0; i < kGaugeDrawNum_; i++) {
		GaugeDraw(viewProjection, i);
	}
}

void Player::Draw(const Matrix4x4& viewProjection)
{
	for (uint16_t i = 0u; i < parts_.size(); i++)
	{
		Model::ModelDraw(parts_[i], viewProjection, 0xffffffff, models_[i].get());
	}
}
