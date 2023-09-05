#include "HitTest.h"

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Score/Score.h"

#include "Engine/Input/KeyInput/KeyInput.h"

void HitTest::Initialize() {


}

void HitTest::Reset() {
	evalutionCount_ = 0;
	intervalCount_ = 0;
}

void HitTest::HitEvalution(Enemy* enemy) {

	if (evalutionCount_ <= kEvalutionframe_[kPerfect]) {
		
		enemy->Die();
		score_->AddPer();

		evalutionCount_ = 0;

	} else if (evalutionCount_ <= kEvalutionframe_[kGreat]) {

		enemy->Die();
		score_->AddGre();

		evalutionCount_ = 0;

	} else if (evalutionCount_ <= kEvalutionframe_[kGood]) {

		enemy->Die();
		score_->AddGood();

		evalutionCount_ = 0;
	} else {

		//enemy->Die();
		score_->AddMiss();

		evalutionCount_ = 0;
	}
}

void HitTest::Update(Enemy* enemy) {

	// 毎フレーム1回のみの更新。カウントがフレーム数と一致しなくなるため。

	score_->ResetEvalution();

	evalutionCount_++;

	if (KeyInput::GetInstance()->GetPadConnect()) {
	
		if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_A)) {
			
			if (enemy->GetBottomType() == Enemy::BottomTypeClass::kA) {
				
				HitEvalution(enemy);
			}

		}else if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_B)) {
			if (enemy->GetBottomType() == Enemy::BottomTypeClass::kB) {
				HitEvalution(enemy);
			}

		} else if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_X)) {
			if (enemy->GetBottomType() == Enemy::BottomTypeClass::kX) {
				HitEvalution(enemy);
			}

		}else if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_Y)) {
			if (enemy->GetBottomType() == Enemy::BottomTypeClass::kY) {
				HitEvalution(enemy);
			}

		}
	}


}