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
	//for (uint16_t i = 0u; i < PARTS::Num; i++) {
	//	models_.push_back(std::make_unique<Model>());
	//}

	//parts_.resize(models_.size());

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

	state_ = Normal;

	wave_A = ATKWAIT;

	T_ = 0;

	isAnimeStart_ = false;
	//サイズあわせ
	nowR.resize(parts_.size());
	ESALL.resize(parts_.size());

#pragma region 各アニメの身体パーツ
	AnimeType[Normal].resize(parts_.size());

	AnimeType[Normal][Body] = {
		{0.9f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f},
	};
	AnimeType[Normal][Head] = {
		{-0.7f, 0.0f, 0.0f},
		{-0.7f, 0.0f, 0.0f},
	};
	AnimeType[Normal][BodyUnder] = {
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
		{0.0f,0.0f,0.0f},
		{0.0f, -2.31f, 0},
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
		{-0.57f, -0.22f, 0.0f},
		{-0.57f, -0.22f, 0.0f},
	};
	AnimeType[1][LLeg2] = {
		{0.6f, 0.0f, 0.0f},
		{0.6f, 0.0f, 0.0f},
	};
	AnimeType[1][LFoot] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	AnimeType[1][RLeg1] = {
		{-0.6f, 0.9f, 0.0f},
		{-0.6f, 0.9f, 0.0f},
	};
	AnimeType[1][RLeg2] = {
		{0.7f, 0.0f, 0.0f},
		{0.7f, 0.0f, 0.0f},
	};
	AnimeType[1][RFoot] = {
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

//現在のプレイヤーの情報取得
void Player::GetplayerR() {
	for (int i = 0; i < PARTS::Num; i++) {
		nowR[i] = parts_[i].rotation_;
	}
}



void Player::Animetion() {

	switch (state_)
	{
	case Player::Normal:
		if (score_->GetEvaluation()) {
			state_ = ATK_R;
		}
		break;
	case Player::ATK_R:
		ATK_R_F();
		break;
	case Player::ATK_L:
		break;
	case Player::A_NUM:
		break;
	default:
		break;
	}
}

//右攻撃
void Player::ATK_R_F() {

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
					AnimeType[ATK_R][i].st
				};
			}
		}
		else {
			for (int i = 0; i < Num; i++) {
				parts_[i].rotation_ = ES(ESALL[i], T_);
			}

			//予備動作無し
			T_ = 1.0f;
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
					AnimeType[ATK_R][i].ed,
				};
			}
		}
		else {
			//イージング更新
			for (int i = 0; i < Num; i++) {
				parts_[i].rotation_ = ES(ESALL[i], T_);
			}
			//Tを加算
			T_ += AddT_*4.0f;
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
				//仮でいきなり手を広げた状態
				ESALL[i] = {
					nowR[i],
					AnimeType[ATK_R][i].st,
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
				state_ = Normal;
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
#pragma endregion



void Player::ModelLoad()
{


	//models_[Body]->Texture("Resources/player/body.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
}

void Player::Update()
{

	camera_->transform.rotation_ = Vector3(offset.x, offset.y, camera_->transform.rotation_.z);

	//ImGui::DragFloat("body", &parts_[Body].translation_.y, 0.1f);

	//	待機時間
	//	移動処理
	MoveType2();

	Animetion();

	transform.UpdateMatrix();
	for (auto& i : parts_) {
		i.UpdateMatrix();
	}
}

void Player::HitTestInitialize() {
	evalutionCount_ = 0.0f;
	intervalCount_ = 0;
	//score->ResetEvalution();
}

void Player::HitEvalution(Enemy* enemy) {

	float hitCount = std::abs(evalutionCount_ - kEvalutionframe_[kGood]);

	hitCount = std::clamp<float>(hitCount, 0.0f, float(kEvalutionframe_[kGood]));

	if (hitCount <= kEvalutionframe_[kPerfect]) {

		enemy->Die(1);
		score_->AddPerfect();

		evalutionCount_ = 0.0f;

	}
	else if (hitCount <= kEvalutionframe_[kGreat]) {

		enemy->Die(0);
		score_->AddGreat();

		evalutionCount_ = 0.0f;

	}
	else if (hitCount <= kEvalutionframe_[kGood]) {

		enemy->Die(1);
		score_->AddGood();

		evalutionCount_ = 0.0f;
	}

	//else {

	//	//enemy->Die();
	//	score_->AddMiss();

	//	evalutionCount_ = 0;
	//}
}

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

	if (flag) {
		
		if (/*KeyInput::PushKey(DIK_SPACE) || */score_->GetEvaluation()) {
			//	座標の更新
			oldPos = transform.translation_.z;
			//	敵の間隔分足す
			movePos += enemyDistance;
			//	frameの初期化
			frame = 0.0f;

			flag = false;
			if (score_->GetEvaluation() == Score::Evaluation::kPerfect || score_->GetEvaluation() == Score::Evaluation::kGreat) {
				shakeFlag = true;
			}
			else {
				shakeFlag = false;
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

	// 毎フレーム1回のみの更新。カウントがフレーム数と一致しなくなるため。

	score_->ResetEvalution();

	if (flag) {

		// ここの加算の割合を上げれば加速する
		evalutionCount_++;

		if (KeyInput::GetInstance()->GetPadConnect()) {

			if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_A)) {
				if (enemy->GetBottomType() == Enemy::BottomTypeClass::kA) {

					HitEvalution(enemy);
				}
				else {
					enemy->Die(0);
					score_->AddMiss();

					evalutionCount_ = 0.0f;
				}

			}
			else if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_B)) {
				if (enemy->GetBottomType() == Enemy::BottomTypeClass::kB) {
					HitEvalution(enemy);
				}
				else {
					enemy->Die(1);
					score_->AddMiss();

					evalutionCount_ = 0.0f;
				}
			}
			else if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_X)) {
				if (enemy->GetBottomType() == Enemy::BottomTypeClass::kX) {
					HitEvalution(enemy);
				}
				else {
					enemy->Die(1);
					score_->AddMiss();

					evalutionCount_ = 0.0f;
				}
			}
			else if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_Y)) {
				if (enemy->GetBottomType() == Enemy::BottomTypeClass::kY) {
					HitEvalution(enemy);
				}
				else {
					enemy->Die(0);
					score_->AddMiss();

					evalutionCount_ = 0.0f;
				}
			}
		}

		if (evalutionCount_ > kEvalutionframe_[kGood] * 2.0f) {

			evalutionCount_ = 0.0f;
		}

	}
}

void Player::GaugeInitialize() {

	gaugeIsDraw_[GaugeDrawEnum::kBack] = true;
	gaugeIsDraw_[GaugeDrawEnum::kGaugeBack] = true;
	gaugeIsDraw_[GaugeDrawEnum::kGaugePerfect] = true;
	gaugeIsDraw_[GaugeDrawEnum::kGaugeGreat] = true;
	gaugeIsDraw_[GaugeDrawEnum::kGaugeGood] = true;
	gaugeIsDraw_[GaugeDrawEnum::kMark] = true;

	gaugeColors_[GaugeDrawEnum::kBack] = 0x222222EE;
	gaugeColors_[GaugeDrawEnum::kGaugeBack] = 0x555555EE;

	gaugeColors_[GaugeDrawEnum::kGaugePerfect] = 0xEEEEEEEE;
	gaugeColors_[GaugeDrawEnum::kGaugeGreat] = 0xEEEE11EE;
	gaugeColors_[GaugeDrawEnum::kGaugeGood] = 0x33FF55EE;

	gaugeColors_[GaugeDrawEnum::kMark] = 0xFFFFFFFF;

	SetGaugeWorldTransform({ kBasePos_.x,kBasePos_.y }, { kBaseScale_ + kBaseWhiteSpace_,kBaseScaleY_ + kBaseWhiteSpace_ }, 0.0f, GaugeDrawEnum::kBack);
	SetGaugeWorldTransform({ kBasePos_.x,kBasePos_.y }, { kBaseScale_,kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeBack);
	SetGaugeWorldTransform(kBasePos_, { kGaugeScale_[Evalution::kPerfect],kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugePerfect);
	SetGaugeWorldTransform(kBasePos_, { kGaugeScale_[Evalution::kGreat],kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeGreat);
	SetGaugeWorldTransform(kBasePos_, { kGaugeScale_[Evalution::kGood],kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeGood);

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

	kEvalutionframe_[Evalution::kPerfect] = globalVariables->GetIntValue("EvalutionFrame", "perfect");
	kEvalutionframe_[Evalution::kGreat] = globalVariables->GetIntValue("EvalutionFrame", "great");
	kEvalutionframe_[Evalution::kGood] = globalVariables->GetIntValue("EvalutionFrame", "good");

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

	SetGaugeWorldTransform({ kBasePos_.x,kBasePos_.y }, { kBaseScale_ + kBaseWhiteSpace_,kBaseScaleY_ + kBaseWhiteSpace_ }, 0.0f, GaugeDrawEnum::kBack);
	SetGaugeWorldTransform({ kBasePos_.x,kBasePos_.y }, { kBaseScale_,kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeBack);

	kGaugeScale_[Evalution::kPerfect] = kBaseScale_ * kEvalutionframe_[Evalution::kPerfect] / kEvalutionframe_[Evalution::kGood];
	kGaugeScale_[Evalution::kGreat] = kBaseScale_ * kEvalutionframe_[Evalution::kGreat] / kEvalutionframe_[Evalution::kGood];
	kGaugeScale_[Evalution::kGood] = kBaseScale_;

	//kGaugeStartPos_[Evalution::kPerfect] = kGaugeStartPos_[Evalution::kGreat] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kGreat] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kPerfect];
	//kGaugeStartPos_[Evalution::kGreat] = kGaugeStartPos_[Evalution::kGood] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kGood] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kGreat];
	//kGaugeStartPos_[Evalution::kGood] = kBasePos_.x - kTextureSize_ / 2 * kBaseScale_ + kTextureSize_ / 2 * kGaugeScale_[Evalution::kGood];

	kGaugeMarkPos_[0] = { kBasePos_.x - kTextureSize_ / 2 * kBaseScale_ + kTextureSize_ / 2 * kGaugeMarkScale_, kBasePos_.y };
	kGaugeMarkPos_[1] = { kBasePos_.x + kTextureSize_ / 2 * kBaseScale_ - kTextureSize_ / 2 * kGaugeMarkScale_, kBasePos_.y };
	/*kGaugeEndPos_[Evalution::kPerfect] = kGaugeStartPos_[Evalution::kPerfect] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kPerfect];
	kGaugeEndPos_[Evalution::kGreat] = kGaugeStartPos_[Evalution::kGreat] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kGreat];
	kGaugeEndPos_[Evalution::kGood] = kGaugeStartPos_[Evalution::kGood] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kGood];*/

}

void Player::GaugeUpdate() {

	ApplyKoroneGlobalVariable();

	SetGaugeWorldTransform(kBasePos_, { kGaugeScale_[Evalution::kPerfect],kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugePerfect);
	SetGaugeWorldTransform(kBasePos_, { kGaugeScale_[Evalution::kGreat],kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeGreat);
	SetGaugeWorldTransform(kBasePos_, { kGaugeScale_[Evalution::kGood],kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeGood);

	if (flag) {

		gaugeIsDraw_[GaugeDrawEnum::kMark] = true;

		float easeFrame = std::clamp<float>(evalutionCount_, 0.0f, float(kEvalutionframe_[Evalution::kGood]) * 2.0f);

		SetGaugeWorldTransform(Ease::UseEase(kGaugeMarkPos_[0], kGaugeMarkPos_[1],easeFrame, kEvalutionframe_[Evalution::kGood] * 2.0f,Ease::Constant),
			{ kGaugeMarkScale_,kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kMark);

		if (score_->GetEvaluation()) {
			gaugeIsDraw_[GaugeDrawEnum::kMark] = false;
		}

	}
	else {

		gaugeIsDraw_[GaugeDrawEnum::kMark] = false;

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
