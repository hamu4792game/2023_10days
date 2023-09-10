#include "Result.h"

#include "Engine/Input/KeyInput/KeyInput.h"
#include "Game/GameScene/GameScene.h"

void Result::Initialize()
{

	ui_->Initialize();
}

void Result::ModelLoad()
{
}

void Result::Update()
{
	ui_->Update();
	
	if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_A) || 
		KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_B) ||
		KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_X) ||
		KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_Y)) {
		GameScene::GetInstance()->sceneChangeFlag = true;
	}

}

void Result::Draw(Matrix4x4 viewProjection)
{
}

void Result::Draw2D(const Matrix4x4& viewProjection) {

	ui_->Draw2D(viewProjection);
}
