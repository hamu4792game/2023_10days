#include "Player.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include "externals/imgui/imgui.h"


#include "Game/Enemy/Enemy.h"
#include "Game/Score/Score.h"
#include "Game/GameScene/Battle/Battle.h"

#include "Engine/Input/KeyInput/KeyInput.h"
#include "Engine/Easing/Ease.h"

#include "GlobalVariables/GlobalVariables.h"


Player::Player(std::shared_ptr<Camera> camera)

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
	offset = Vector3(0.0f, 30.0f, -50.0f);
	camera_->transform.translation_ = offset;
	camera_->transform.rotation_.x = 0.4f;
	
	
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
	MAX_frame = 60.0f;
	

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

	state_ =Normal;

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

			T_ += AddT_;
			if (T_ >= 1.0f) {
				wave_A = ATK;
				T_ = 0;
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
			T_ += AddT_;
			//シーン切り替え処理
			if (T_ >= 1.0f) {
				wave_A = BACK;
				T_ = 0;
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
					AnimeType[Normal][i].st,
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
			}

		}
		break;
	default:
		break;
	}
	
}
#pragma endregion



void Player::ModelLoad()
{
	

	//models_[Body]->Texture("Resources/player/body.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
}

void Player::Update()
{

	//camera_->transform.translation_ = offset;
	//if (KeyInput::GetKey(DIK_SPACE)) {
	//	world_->rotation_.x += AngleToRadian(1.0f);
	//
	//
	//	Vector2 ran(0.0f, 0.0f);
	//	ran.x = static_cast<float>(std::rand() % 3 - 1);
	//	ran.y = static_cast<float>(std::rand() % 3 - 1);
	//	camera_->transform.translation_.x += ran.x;
	//	camera_->transform.translation_.y += ran.y;
	//}
	
	
	//ImGui::DragFloat("body", &parts_[Body].translation_.y, 0.1f);

	//	待機時間
	//	移動処理
	MoveType2();

	transform.UpdateMatrix();
	for (auto& i : parts_) {
		i.UpdateMatrix();
	}
}

void Player::HitTestInitialize() {
	evalutionCount_ = 0;
	intervalCount_ = 0;
	//score->ResetEvalution();
}

void Player::HitEvalution(Enemy* enemy) {

	if (evalutionCount_ <= kEvalutionframe_[kPerfect]) {

		enemy->Die(1);
		score_->AddPerfect();

		evalutionCount_ = 0;

	}
	else if (evalutionCount_ <= kEvalutionframe_[kGreat]) {

		enemy->Die(0);
		score_->AddGreat();

		evalutionCount_ = 0;

	}
	else if (evalutionCount_ <= kEvalutionframe_[kGood]) {

		enemy->Die(1);
		score_->AddGood();

		evalutionCount_ = 0;
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
		//	待機フレームの初期化
		waitFrame = 0.0f;

		Battle::masterSpeed = 0.3f;

		//	フラグを折る
		flag = false;
	}
}

void Player::MoveType2() {
	//	frame加算処理 通常加算速度 * 全体のframe速度
	//frame += 1.0f * Battle::masterSpeed;

	if (flag) {
		waitFrame++;

		//	仮 入力を受け付けたらフラグを建てる
		if (/*KeyInput::PushKey(DIK_SPACE) || */score_->GetEvaluation()) {
			waitFrame = MAX_frame;
		}

		if (/*KeyInput::PushKey(DIK_SPACE) || */score_->GetEvaluation()) {
			//	座標の更新
			oldPos = transform.translation_.z;
			//	敵の間隔分足す
			movePos += enemyDistance;
			//	frameの初期化
			frame = 0.0f;
			//	待機フレームの初期化
			waitFrame = 0.0f;

			flag = false;
		}
	}
	else {
		//	frame加算処理 通常加算速度 * 全体のframe速度
		frame += 1.0f * Battle::masterSpeed;
		//	攻撃をするまでの移動処理
		transform.translation_.z = Ease::UseEase(oldPos, movePos, frame, MAX_frame, Ease::EaseType::EaseOutSine);
		if (frame >= MAX_frame) {
			waitFrame = 0.0f;
			flag = true;
		}
	}
}

void Player::HitTest(Enemy* enemy) {

	// 毎フレーム1回のみの更新。カウントがフレーム数と一致しなくなるため。

	score_->ResetEvalution();

	if (flag) {

		evalutionCount_++;

		if (KeyInput::GetInstance()->GetPadConnect()) {

			if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_A)) {
				if (enemy->GetBottomType() == Enemy::BottomTypeClass::kA) {

					HitEvalution(enemy);
				}
				else {
					enemy->Die(0);
					score_->AddMiss();

					evalutionCount_ = 0;
				}

			}
			else if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_B)) {
				if (enemy->GetBottomType() == Enemy::BottomTypeClass::kB) {
					HitEvalution(enemy);
				}
				else {
					enemy->Die(1);
					score_->AddMiss();

					evalutionCount_ = 0;
				}
			}
			else if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_X)) {
				if (enemy->GetBottomType() == Enemy::BottomTypeClass::kX) {
					HitEvalution(enemy);
				}
				else {
					enemy->Die(1);
					score_->AddMiss();

					evalutionCount_ = 0;
				}
			}
			else if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_Y)) {
				if (enemy->GetBottomType() == Enemy::BottomTypeClass::kY) {
					HitEvalution(enemy);
				}
				else {
					enemy->Die(0);
					score_->AddMiss();

					evalutionCount_ = 0;
				}
			}
		}

		if (evalutionCount_ >= kEvalutionframe_[kMiss]) {
			enemy->Die(1);
			score_->AddMiss();

			evalutionCount_ = 0;
		}

	}
}

void Player::GaugeInitialize() {

	gaugeIsDraw_[GaugeDrawEnum::kBack] = true;
	gaugeIsDraw_[GaugeDrawEnum::kGaugeBack] = true;
	gaugeIsDraw_[GaugeDrawEnum::kGaugePerfect] = false;
	gaugeIsDraw_[GaugeDrawEnum::kGaugeGreat] = false;
	gaugeIsDraw_[GaugeDrawEnum::kGaugeGood] = false;

	gaugeColors_[GaugeDrawEnum::kBack] = 0x222222EE;
	gaugeColors_[GaugeDrawEnum::kGaugeBack] = 0x555555EE;
	gaugeColors_[GaugeDrawEnum::kGaugePerfect] = 0xEEEEEEEE;
	gaugeColors_[GaugeDrawEnum::kGaugeGreat] = 0xEEEE11EE;
	gaugeColors_[GaugeDrawEnum::kGaugeGood] = 0x33FF55EE;

	SetGaugeWorldTransform({ kBasePos_.x,kBasePos_.y }, { kBaseScale_ + kBaseWhiteSpace_,kBaseScaleY_ + kBaseWhiteSpace_ }, 0.0f, GaugeDrawEnum::kBack);
	SetGaugeWorldTransform({ kBasePos_.x,kBasePos_.y }, { kBaseScale_,kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeBack);
	SetGaugeWorldTransform({ kGaugeStartPos_[Evalution::kPerfect],kBasePos_.y }, { kGaugeScale_[Evalution::kPerfect],kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugePerfect);
	SetGaugeWorldTransform({ kGaugeStartPos_[Evalution::kGreat],kBasePos_.y }, { kGaugeScale_[Evalution::kGreat],kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeGreat);
	SetGaugeWorldTransform({ kGaugeStartPos_[Evalution::kGood],kBasePos_.y }, { kGaugeScale_[Evalution::kGood],kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeGood);

	SetGaugeGlobalVariable();
}

void Player::SetGaugeGlobalVariable() {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "Gauge";

	globalVariables->CreateGroup(groupName);

	globalVariables->AddItem(groupName, "pos", kBasePos_);
	globalVariables->AddItem(groupName, "scaleX", kBaseScale_);
	globalVariables->AddItem(groupName, "scaleY", kBaseScaleY_);
	globalVariables->AddItem(groupName, "whiteSpace", kBaseWhiteSpace_);

	ApplyGuageGlobalVariable();
}

void Player::ApplyGuageGlobalVariable() {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Gauge";

	kBasePos_ = globalVariables->GetVector2Value(groupName, "pos");
	kBaseScale_ = globalVariables->GetFloatValue(groupName, "scaleX");
	kBaseScaleY_ = globalVariables->GetFloatValue(groupName, "scaleY");
	kBaseWhiteSpace_ = globalVariables->GetFloatValue(groupName, "whiteSpace");


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

	ApplyGuageGlobalVariable();


	if (flag) {

		if (evalutionCount_ == 1) {
			gaugeIsDraw_[GaugeDrawEnum::kGaugePerfect] = true;
			gaugeIsDraw_[GaugeDrawEnum::kGaugeGreat] = true;
			gaugeIsDraw_[GaugeDrawEnum::kGaugeGood] = true;
		}

		if (evalutionCount_ < kEvalutionframe_[Evalution::kPerfect]) {

			float pos = Ease::UseEase(kGaugeStartPos_[Evalution::kPerfect], kGaugeEndPos_[Evalution::kPerfect], evalutionCount_, kEvalutionframe_[kPerfect], Ease::EaseType::Constant);
			float scale = Ease::UseEase(kGaugeScale_[Evalution::kPerfect], 0.0f, evalutionCount_, kEvalutionframe_[Evalution::kPerfect], Ease::EaseType::Constant);

			SetGaugeWorldTransform({ pos,kBasePos_.y}, { scale,kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugePerfect);
		}
		else if (evalutionCount_ == kEvalutionframe_[kPerfect]) {
			gaugeIsDraw_[GaugeDrawEnum::kGaugePerfect] = false;
		}
		else if (evalutionCount_ < kEvalutionframe_[Evalution::kGreat]) {

			float pos = Ease::UseEase(kGaugeStartPos_[Evalution::kGreat], kGaugeEndPos_[Evalution::kGreat], evalutionCount_ - kEvalutionframe_[Evalution::kPerfect], kEvalutionframe_[Evalution::kGreat] - kEvalutionframe_[Evalution::kPerfect], Ease::EaseType::Constant);
			float scale = Ease::UseEase(kGaugeScale_[Evalution::kGreat], 0.0f, evalutionCount_ - kEvalutionframe_[Evalution::kPerfect], kEvalutionframe_[Evalution::kGreat] - kEvalutionframe_[Evalution::kPerfect], Ease::EaseType::Constant);

			SetGaugeWorldTransform({ pos,kBasePos_.y }, { scale,kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeGreat);
		}
		else if (evalutionCount_ == kEvalutionframe_[Evalution::kGreat]) {
			gaugeIsDraw_[GaugeDrawEnum::kGaugeGreat] = false;
		}
		else if (evalutionCount_ < kEvalutionframe_[Evalution::kGood]) {

			float pos = Ease::UseEase(kGaugeStartPos_[Evalution::kGood], kGaugeEndPos_[Evalution::kGood], evalutionCount_ - kEvalutionframe_[Evalution::kGreat], kEvalutionframe_[Evalution::kGood] - kEvalutionframe_[Evalution::kGreat], Ease::EaseType::Constant);
			float scale = Ease::UseEase(kGaugeScale_[Evalution::kGood], 0.0f, evalutionCount_ - kEvalutionframe_[Evalution::kGreat], kEvalutionframe_[Evalution::kGood] - kEvalutionframe_[Evalution::kGreat], Ease::EaseType::Constant);

			SetGaugeWorldTransform({ pos,kBasePos_.y }, { scale,kBaseScaleY_ }, 0.0f, GaugeDrawEnum::kGaugeGood);
		}
		else if (evalutionCount_ == kEvalutionframe_[Evalution::kGood]) {
			gaugeIsDraw_[GaugeDrawEnum::kGaugeGood] = false;
		}

	}
	else {

		gaugeIsDraw_[GaugeDrawEnum::kGaugePerfect] = false;
		gaugeIsDraw_[GaugeDrawEnum::kGaugeGreat] = false;
		gaugeIsDraw_[GaugeDrawEnum::kGaugeGood] = false;

		SetGaugeWorldTransform({ kGaugeStartPos_[Evalution::kPerfect],kBasePos_.y }, { kGaugeScale_[Evalution::kPerfect],18.0f }, 0.0f, GaugeDrawEnum::kGaugePerfect);
		SetGaugeWorldTransform({ kGaugeStartPos_[Evalution::kGreat],kBasePos_.y }, { kGaugeScale_[Evalution::kGreat],18.0f }, 0.0f, GaugeDrawEnum::kGaugeGreat);
		SetGaugeWorldTransform({ kGaugeStartPos_[Evalution::kGood],kBasePos_.y }, { kGaugeScale_[Evalution::kGood],18.0f }, 0.0f, GaugeDrawEnum::kGaugeGood);
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
