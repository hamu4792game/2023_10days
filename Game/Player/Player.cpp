#include "Player.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Score/Score.h"

#include "Engine/Input/KeyInput/KeyInput.h"

void Player::Initialize()
{
	parts_.resize(models_.size());
	for (auto& i : parts_) {
		i.parent_ = &parts_[Body];
	}
	parts_[Body].parent_ = &transform;

	// スコアのポインタを先に取得しないとエラーになるから保留。
	//HitTestInitialize();
}

void Player::ModelLoad()
{
	//models_[Body]->Texture("Resources/player/body.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
}

void Player::Update()
{
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
		score->AddPer();

		evalutionCount_ = 0;

	}
	else if (evalutionCount_ <= kEvalutionframe_[kGreat]) {

		enemy->Die();
		score->AddGre();

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

void Player::HitTest(Enemy* enemy, Score* score) {

	// 毎フレーム1回のみの更新。カウントがフレーム数と一致しなくなるため。

	score->ResetEvalution();

	evalutionCount_++;

	if (KeyInput::GetInstance()->GetPadConnect()) {

		if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_A)) {
			if (enemy->GetBottomType() == Enemy::BottomTypeClass::kA) {

				HitEvalution(enemy, score);
			}
		}
		else if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_B)) {
			if (enemy->GetBottomType() == Enemy::BottomTypeClass::kB) {
				HitEvalution(enemy, score);
			}
		}
		else if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_X)) {
			if (enemy->GetBottomType() == Enemy::BottomTypeClass::kX) {
				HitEvalution(enemy, score);
			}
		}
		else if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_Y)) {
			if (enemy->GetBottomType() == Enemy::BottomTypeClass::kY) {
				HitEvalution(enemy, score);
			}
		}
	}

	if (evalutionCount_ > kEvalutionframe_[kMiss]) {
		//enemy->Die();
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
