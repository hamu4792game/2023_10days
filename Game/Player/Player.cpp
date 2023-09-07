#include "Player.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include "externals/imgui/imgui.h"


#include "Game/Enemy/Enemy.h"
#include "Game/Score/Score.h"
#include "Game/GameScene/Battle/Battle.h"

#include "Engine/Input/KeyInput/KeyInput.h"
#include "Engine/Easing/Ease.h"


Player::Player(std::shared_ptr<Camera> camera)

{
	camera_ = camera;
	//for (uint16_t i = 0u; i < PARTS::Num; i++) {
	//	models_.push_back(std::make_unique<Model>());
	//}

	//parts_.resize(models_.size());
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
	//transform.translation_.y = 1.0f * 200.0f;
	transform.scale_ = Vector3(0.3f, 0.3f, 0.3f);

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
	enemyDistance = 10.0f;
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
	parts_[Body].translation_ = { 0.0f, 6.5f, 0.0f };
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
	Move();

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

void Player::HitEvalution(Enemy* enemy, Score* score) {

	if (evalutionCount_ <= kEvalutionframe_[kPerfect]) {

		enemy->Die();
		score->AddPerfect();

		evalutionCount_ = 0;

	}
	else if (evalutionCount_ <= kEvalutionframe_[kGreat]) {

		enemy->Die();
		score->AddGreat();

		evalutionCount_ = 0;

	}
	else if (evalutionCount_ <= kEvalutionframe_[kGood]) {

		enemy->Die();
		score->AddGood();

		evalutionCount_ = 0;
	}

	//else {

	//	//enemy->Die();
	//	score_->AddMiss();

	//	evalutionCount_ = 0;
	//}
}

void Player::Move() {
	//	frame加算処理 通常加算速度 * 全体のframe速度
	frame += 1.0f * Battle::masterSpeed;

	if (flag) {
		waitFrame++;

		//	仮 入力を受け付けたらフラグを建てる
		if (KeyInput::PushKey(DIK_SPACE)) {
			waitFrame = MAX_frame;
		}

		if (waitFrame >= MAX_frame) {
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
		if (frame >= MAX_frame)	{
			waitFrame = 0.0f;
			flag = true;
		}
	}

}

void Player::MoveType2() {
	//	frame加算処理 通常加算速度 * 全体のframe速度
	frame += 1.0f * Battle::masterSpeed;

	if (flag) {
		waitFrame++;

		//	仮 入力を受け付けたらフラグを建てる
		if (KeyInput::PushKey(DIK_SPACE)) {
			waitFrame = MAX_frame;
		}

		if (waitFrame >= MAX_frame) {
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

void Player::HitTest(Enemy* enemy, Score* score) {

	// 毎フレーム1回のみの更新。カウントがフレーム数と一致しなくなるため。

	score->ResetEvalution();

	evalutionCount_++;

	if (KeyInput::GetInstance()->GetPadConnect()) {

		if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_A)) {
			if (enemy->GetBottomType() == Enemy::BottomTypeClass::kA) {

				HitEvalution(enemy, score);
			}
			else {
				enemy->Die();
				score->AddMiss();

				evalutionCount_ = 0;
			}

		}
		else if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_B)) {
			if (enemy->GetBottomType() == Enemy::BottomTypeClass::kB) {
				HitEvalution(enemy, score);
			}
			else {
				enemy->Die();
				score->AddMiss();

				evalutionCount_ = 0;
			}
		}
		else if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_X)) {
			if (enemy->GetBottomType() == Enemy::BottomTypeClass::kX) {
				HitEvalution(enemy, score);
			}
			else {
				enemy->Die();
				score->AddMiss();

				evalutionCount_ = 0;
			}
		}
		else if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_Y)) {
			if (enemy->GetBottomType() == Enemy::BottomTypeClass::kY) {
				HitEvalution(enemy, score);
			}
			else {
				enemy->Die();
				score->AddMiss();

				evalutionCount_ = 0;
			}
		}
	}

	if (evalutionCount_ >= kEvalutionframe_[kMiss]) {
		enemy->Die();
		score->AddMiss();

		evalutionCount_ = 0;
	}
}

void Player::Draw(const Matrix4x4& viewProjection)
{
	for (uint16_t i = 0u; i < parts_.size(); i++)
	{
		Model::ModelDraw(parts_[i], viewProjection, 0xffffffff, models_[i].get());
	}
}
