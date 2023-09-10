#include "Game/GameScene/Title/Title.h"
#include "EEnum.h"
#include "Engine/Easing/Ease.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include "Game/GameScene/GameScene.h"

Title::Title(std::shared_ptr<Camera> camera)
{
	camera_ = camera;

	for (uint16_t index = 0u; index < 30u; index++) {
		enemy_.push_back(std::make_unique<Enemy>());
	}
	pushAtext_ = std::make_unique<Texture2D>();
	pushAtext_->Texture("Resources/hud/pushA.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	
}

void Title::Initialize()
{
	worldTransform.translation_ = Vector3(0.0f, 6.5f, 0.0f);

	for (uint16_t i = 0u; i < 30u; i++) {
		enemy_[i]->InitializeSP(15.0f + static_cast<float>(5.0f * i), Enemy::BottomType::kA, i, mobModels_type2);
	}
	parts_.resize(mobModels_.size());

	shopTrans.resize(shopModels_.size());

	SetParts();

	chara.parent_ = &worldTransform;
	chara.rotation_ = Vector3(0.0f, -1.641f, 0.0f);

	shopTrans[static_cast<uint16_t>(SHOPPARTS::Base)].parent_ = &worldTransform;
	shopTrans[static_cast<uint16_t>(SHOPPARTS::Base)].translation_ = Vector3(0.0f, 0.0f, 200.0f);
	shopTrans[static_cast<uint16_t>(SHOPPARTS::Base)].scale_ = Vector3(5.0f, 5.0f, 5.0f);

	shopTrans[static_cast<uint16_t>(SHOPPARTS::Signboard)].parent_ = &worldTransform;
	shopTrans[static_cast<uint16_t>(SHOPPARTS::Signboard)].translation_ = Vector3(0.0f, 8.0f, 157.5f);
	shopTrans[static_cast<uint16_t>(SHOPPARTS::Signboard)].rotation_ = Vector3(AngleToRadian(-90.0f), 0.0f, 0.0f);
	shopTrans[static_cast<uint16_t>(SHOPPARTS::Signboard)].scale_ = Vector3(1.6f, 0.9f, 1.0f);

	shopTrans[static_cast<uint16_t>(SHOPPARTS::BoardText)].parent_ = &shopTrans[static_cast<uint16_t>(SHOPPARTS::Signboard)];
	shopTrans[static_cast<uint16_t>(SHOPPARTS::BoardText)].translation_ = Vector3(0.0f, 0.0f, 0.0f);
	shopTrans[static_cast<uint16_t>(SHOPPARTS::BoardText)].rotation_ = Vector3(0.0f, 0.0f, 0.0f);
	//shopTrans[static_cast<uint16_t>(SHOPPARTS::BoardText)].scale_ = Vector3(1.0f, 1.0f, 1.0f);

	//	カメラの設定
	//	カメラとの親子関係
	camera_->transform.parent_ = &worldTransform;
	camera_->transform.translation_ = Vector3(-25.0f, -0.5f, -5.0f);
	camera_->transform.rotation_ = Vector3(0.035f, 1.256f, 0.0f);

	endPoint = Vector3(0.0f, 0.0f, 170.0f);

	pushAtrans_.translation_ = Vector3(0.0f, -230.0f, 0.0f);
	pushAtrans_.scale_ = Vector3(1.5f, 1.5f, 1.0f);

}

void Title::Update()
{
	CameraMove();

	worldTransform.UpdateMatrix();

	chara.UpdateMatrix();

	for (auto& shop : shopTrans) {
		shop.UpdateMatrix();
	}
	
	for (auto& ene : enemy_) {
		ene->Update();
	}
	for (auto& parts : parts_) {
		parts.UpdateMatrix();
	}


	if (cameraStep == Title::CAMERASTEP::Zero) {
		pushAtrans_.UpdateMatrix();
	}
	
}

void Title::Draw(Matrix4x4 viewProjection)
{	
	
	for (uint16_t i = 0u; i < shopModels_.size(); i++) {
		Model::ModelDraw(shopTrans[i], viewProjection, 0xffffffff, shopModels_[i].get());
	}

	for (uint16_t i = 0u; i < parts_.size(); i++) {
		Model::ModelDraw(parts_[i], viewProjection, 0xffffffff, mobModels_[i].get());
	}

	for (auto& ene : enemy_) {
		ene->Draw(viewProjection);
	}
}

void Title::Draw2D(Matrix4x4 viewProjection2d)
{
	if (cameraStep == Title::CAMERASTEP::Zero) {
		Texture2D::TextureDraw(pushAtrans_, viewProjection2d, 0x000000ff, pushAtext_.get());
	}
}

void Title::SetParts()
{
	parts_[Body].parent_ = &chara;

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

void Title::CameraMove()
{
	switch (cameraStep)
	{
	case Title::CAMERASTEP::Zero:

		if (KeyInput::PushKey(DIK_R)) {
			// 一旦初期座標に戻す
			camera_->transform.translation_ = Vector3(-25.0f, -0.5f, -5.0f);
			camera_->transform.rotation_ = Vector3(0.035f, 1.256f, 0.0f);
			chara.rotation_ = Vector3(0.0f, -1.641f, 0.0f);
		}


		if (KeyInput::PushKey(DIK_SPACE) || KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_A)) {
			cameraStep = CAMERASTEP::First;

			// Firstのためのセット
			startingPoint = camera_->transform.translation_;
			startingRotate = camera_->transform.rotation_;
			endPoint = Vector3(0.0f, 8.0f, -3.3f);
			endRotate = Vector3(0.35f, 0.0f, 0.0f);
			easeNowFrame = 0;
			easeMaxFrame = 60;

			//	突貫
			pushAtrans_.scale_ = Vector3(0.0f, 0.0f, 0.0f);

		}
		break;
	case Title::CAMERASTEP::First:
		easeNowFrame++;
		camera_->transform.translation_ = Ease::UseEase(startingPoint, endPoint, easeNowFrame, easeMaxFrame, Ease::EaseType::EaseInSine);
		camera_->transform.rotation_ = Ease::UseEase(startingRotate, endRotate, easeNowFrame, easeMaxFrame, Ease::EaseType::EaseInSine);
		if (easeNowFrame >= easeMaxFrame) {
			cameraStep = CAMERASTEP::Second;

			// Secondのためのセット
			startingPoint = camera_->transform.translation_;
			startingRotate = camera_->transform.rotation_;
			endPoint = Vector3(0.0f, 8.0f, 155.0f);
			endRotate = Vector3(0.0f, 0.0f, 0.0f);
			easeNowFrame = 0;
			easeMaxFrame = 120;

		}
		break;
	case Title::CAMERASTEP::Second:
		easeNowFrame++;
		camera_->transform.translation_ = Ease::UseEase(startingPoint, endPoint, easeNowFrame, easeMaxFrame, Ease::EaseType::EaseInSine);
		camera_->transform.rotation_ = Ease::UseEase(startingRotate, endRotate, easeNowFrame, easeMaxFrame, Ease::EaseType::EaseInSine);
		if (easeNowFrame >= easeMaxFrame) {
			cameraStep = CAMERASTEP::Bounce;

			// Bounceのためのセット
			startingPoint = camera_->transform.translation_;
			startingRotate = camera_->transform.rotation_;
			endPoint = camera_->transform.translation_ - Vector3(0.0f, 0.0f, 2.0f);
			endRotate = Vector3(0.35f, 0.0f, 0.0f);
			easeNowFrame = 0;
			easeMaxFrame = 30;

		}
		break;
	case Title::CAMERASTEP::Bounce:
		easeNowFrame++;

		if (easeNowFrame >= easeMaxFrame + 10) {
			cameraStep = CAMERASTEP::BounceFace;

			// BounceFaceのためのセット
			camera_->transform.rotation_ = Vector3(0.0f, AngleToRadian(-180.0f), 0.0f);
			camera_->transform.translation_ = Vector3(0.0f, 2.6f, 6.5f);
			chara.rotation_.y = 0.0f;

			startingPoint = camera_->transform.translation_ - Vector3(0.0f, 0.0f, 2.0f);
			endPoint = camera_->transform.translation_;
			easeNowFrame = 0;
			easeMaxFrame = 30;
		}
		else if (easeNowFrame <= easeMaxFrame) {
			camera_->transform.translation_ = Ease::UseEase(startingPoint, endPoint, easeNowFrame, easeMaxFrame, Ease::EaseType::EaseOutBounce);
		}
		break;
	case Title::CAMERASTEP::BounceFace:
		easeNowFrame++;
		if (easeNowFrame >= easeMaxFrame + 20) {
			cameraStep = CAMERASTEP::Zero;

			//	シーンチェンジ
			GameScene::GetInstance()->sceneChangeFlag = true;
			//	tirstReturnへの準備
			/*startingPoint = camera_->transform.translation_;
			startingRotate = camera_->transform.rotation_;
			endPoint = Vector3(0.0f, 30.0f, -50.0f);
			endRotate = Vector3(0.4f, 0.0f, 0.0f);
			easeNowFrame = 0;
			easeMaxFrame = 30;*/
		}
		else if (easeNowFrame <= easeMaxFrame) {
			camera_->transform.translation_ = Ease::UseEase(startingPoint, endPoint, easeNowFrame, easeMaxFrame, Ease::EaseType::EaseInOutBounce);
		}
		break;
case Title::CAMERASTEP::FirstReturn:
		easeNowFrame++;
		camera_->transform.translation_ = Ease::UseEase(startingPoint, endPoint, easeNowFrame, easeMaxFrame, Ease::EaseType::EaseInSine);
		camera_->transform.rotation_ = Ease::UseEase(startingRotate, endRotate, easeNowFrame, easeMaxFrame, Ease::EaseType::EaseInSine);
		if (easeNowFrame >= easeMaxFrame) {
			cameraStep = CAMERASTEP::Zero;

		}
		break;

	}
}
