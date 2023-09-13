#include "Game/GameScene/Title/Title.h"
#include "EEnum.h"
#include "Engine/Easing/Ease.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include "Game/GameScene/GameScene.h"
#include "externals/imgui/imgui.h"
#include <algorithm>

Title::Title(std::shared_ptr<Camera> camera)
{
	camera_ = camera;

	for (uint16_t index = 0u; index < 30u; index++) {
		enemy_.push_back(std::make_unique<Enemy>());
	}
	player = std::make_unique<Enemy>();
	pushAtext_ = std::make_unique<Texture2D>();
	pushAtext_->Texture("Resources/hud/pushA.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	
	//	音声のロード
	bgm.SoundLoadWave("Resources/sound/bgm.wav");
	bgm.SoundPlayWave(true);
	bgm.SetVolume(0.1f);

	sDawnsound.SoundLoadWave("Resources/sound/dawn.wav");
	sAppearance.SoundLoadWave("Resources/sound/appearance.wav");

}

void Title::Initialize()
{
	worldTransform.translation_ = Vector3(0.0f, 6.5f, 0.0f);
	int index = 0;
	for (uint16_t i = 0u; i < 30u; i++) {
		index++;
		if (index >= static_cast<int>(GameScene::ModelType::Max_Num)) {
			index = 1;
		}
		enemy_[i]->InitializeSP(15.0f + static_cast<float>(5.0f * i), Enemy::BottomType::kA, i, mobModels_[index]);
	}
	player->InitializeSP(0.0f, Enemy::BottomType::kA, 0, mobModels_[0]);
	player->SetParent(chara);
	parts_.resize(mobModels_[0].size());

	shopTrans.resize(shopModels_.size());
	tentyoTransform.resize(tentyoModels_.size());

	SetParts();

	cameraStep = CAMERASTEP::Zero;

	chara.parent_ = &worldTransform;
	chara.translation_ = Vector3(0.0f, -6.5f, 0.0f);
	chara.rotation_ = Vector3(0.0f, -1.641f, 0.0f);

	shopTrans[static_cast<uint16_t>(SHOPPARTS::Base)].parent_ = &worldTransform;
	shopTrans[static_cast<uint16_t>(SHOPPARTS::Base)].translation_ = Vector3(0.0f, -7.0f, 210.0f);
	shopTrans[static_cast<uint16_t>(SHOPPARTS::Base)].rotation_ = Vector3(0.0f, AngleToRadian(180.0f), 0.0f);
	shopTrans[static_cast<uint16_t>(SHOPPARTS::Base)].scale_ = Vector3(5.0f, 5.0f, 5.0f);

	shopTrans[static_cast<uint16_t>(SHOPPARTS::Signboard)].parent_ = &worldTransform;
	shopTrans[static_cast<uint16_t>(SHOPPARTS::Signboard)].translation_ = Vector3(0.0f, 3.0f, 180.5f);
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
	camera_->transform.scale_ = Vector3(1.0f, 1.0f, 1.0f);

	endPoint = Vector3(0.0f, 0.0f, 170.0f);

	pushAtrans_.translation_ = Vector3(0.0f, -230.0f, 0.0f);
	pushAtrans_.scale_ = Vector3(1.5f, 1.5f, 1.0f);

#pragma region 王騎将軍ポーズ
	okisyogun.resize(tentyoTransform.size());
	okisyogun[Body] = {
		{0.0f,0.0f,0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	okisyogun[BodyUnder] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	okisyogun[Head] = {
		{0.0f, 0.0f, 0.0f},
		{0.3f, 0.0f, 0.0f},
	};

	//ude
	okisyogun[LArm1] = {
		{0.0f, 0.6f, 1.1f},
		{0.0f, 0.6f, 1.1f},
	};
	okisyogun[LArm2] = {
		{0.0f, 2.7f, -0.95f},
		{0.0f, 2.7f, -0.95f},
	};
	okisyogun[LHand] = {
		{0.0f, 0.75f, 0.0f},
		{0.0f, 0.75f, 0.0f},
	};

	okisyogun[RArm1] = {
		{0.0f, -0.53f, -1.0f},
		{0.0f, -0.53f, -1.0f},
	};
	okisyogun[RArm2] = {
		{0.0f, -2.54f, 0.7f},
		{0.0f, -2.54f, 0.7f},
	};
	okisyogun[RHand] = {
		{0.0f, -0.5f, 0.0f},
		{0.0f, -0.5f, 0.0f},
	};

	//asi
	okisyogun[LLeg1] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	okisyogun[LLeg2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	okisyogun[LFoot] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	okisyogun[RLeg1] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	okisyogun[RLeg2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	okisyogun[RFoot] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
#pragma endregion

	tentyoTransform[Body].parent_ = &TenchoW_;
#pragma region パーツの親子関係と座標の初期設定
	tentyoTransform[Head].parent_ = &tentyoTransform[Body];
	tentyoTransform[BodyUnder].parent_ = &tentyoTransform[Body];

	tentyoTransform[LArm1].parent_ = &tentyoTransform[Body];
	tentyoTransform[LArm2].parent_ = &tentyoTransform[LArm1];
	tentyoTransform[LHand].parent_ = &tentyoTransform[LArm2];

	tentyoTransform[RArm1].parent_ = &tentyoTransform[Body];
	tentyoTransform[RArm2].parent_ = &tentyoTransform[RArm1];
	tentyoTransform[RHand].parent_ = &tentyoTransform[RArm2];

	tentyoTransform[LLeg1].parent_ = &tentyoTransform[BodyUnder];
	tentyoTransform[LLeg2].parent_ = &tentyoTransform[LLeg1];
	tentyoTransform[LFoot].parent_ = &tentyoTransform[LLeg2];

	tentyoTransform[RLeg1].parent_ = &tentyoTransform[BodyUnder];
	tentyoTransform[RLeg2].parent_ = &tentyoTransform[RLeg1];
	tentyoTransform[RFoot].parent_ = &tentyoTransform[RLeg2];

	tentyoTransform[Num].parent_ = &tentyoTransform[BodyUnder];
	//座標設定
	tentyoTransform[Body].translation_ = { 0.0f, 0.0f, 0.0f };
	tentyoTransform[BodyUnder].translation_ = { 0.0f, 0.0f, 0.0f };
	tentyoTransform[Head].translation_ = { 0.0f, 2.6f, 0.0f };

	tentyoTransform[LArm1].translation_ = { -0.8f, 1.57f, 0.0f };
	tentyoTransform[LArm2].translation_ = { -1.73f, 0.0f, 0.0f };
	tentyoTransform[LHand].translation_ = { -2.37f, 0.0f, 0.0f };

	tentyoTransform[RArm1].translation_ = { 0.8f, 1.57f, 0.0f };
	tentyoTransform[RArm2].translation_ = { 1.73f, 0.0f, 0.0f };
	tentyoTransform[RHand].translation_ = { 2.37f, 0.0f, 0.0f };

	tentyoTransform[LLeg1].translation_ = { -0.3f, -1.7f, 0.0f };
	tentyoTransform[LLeg2].translation_ = { 0.0f, -2.2f, 0.0f };
	tentyoTransform[LFoot].translation_ = { -0.12f, -2.2f, 0.0f };

	tentyoTransform[RLeg1].translation_ = { 0.3f, -1.7f, 0.0f };
	tentyoTransform[RLeg2].translation_ = { 0.0f, -2.2f, 0.0f };
	tentyoTransform[RFoot].translation_ = { 0.12f, -2.2f, 0.0f };


#pragma endregion
	
	Tpos[0] = Vector3(-3.6f, 6.6f, 190.0f);
	Trota[0]= Vector3(0.0f, -3.4f, 0.0f);

	Tpos[1] = Vector3(-16.11f,3.5f,-7.0f);
	Trota[1] = Vector3(0.0f,-1.4f,0.0f);

	Tpos[2] = Vector3(0.0f, 30.0f, 210.0f);
	Trota[2] = Vector3(0.0f, -3.14f, 0.0f);

	Tpos[3] = Vector3(0.0f,6.64f,179.8f);
	Trota[3] = Vector3(0.0f, -3.14f, 0.0f);

	int GetTpos = GetRandomNum(3, false);

	TenchoW_.translation_ = Tpos[GetTpos];
	TenchoW_.rotation_ = Trota[GetTpos];
}

//将軍ポーズ店長
void Title::OkiSyogun() {
	if (!isOkiStart_) {
		isOkiStart_ = true;
		//アニメーションポーズ設定
		for (int i = 0; i < PARTS::Num; i++) {
			tentyoTransform[i].rotation_ = okisyogun[i].st;
		}
		isL_ = false;
		OkiT_ = 0;
	}
	else {

		for (int i = 0; i < PARTS::Num; i++) {
			tentyoTransform[i].rotation_ = ES(okisyogun[i], OkiT_);
		}

		//以下ループT
		float ADD = 1.0f / 30.0f;
		if (!isL_) {
			OkiT_ += ADD;
			if (OkiT_ >= 1.0f) {
				isL_ = true;
				OkiT_ = 1.0f;
			}
		}
		else {
			OkiT_ -= ADD;
			if (OkiT_ <= 0.0f) {
				isL_ = false;
				OkiT_ = 0;
			}
		}
	}
}

void Title::Update()
{
#ifdef _DEBUG
	ImGui::Begin("tentyo");
	ImGui::DragFloat3("t pos", &TenchoW_.translation_.x,0.01f);
	ImGui::DragFloat3("t rota", &TenchoW_.rotation_.x,0.01f);
	ImGui::End();
#endif // _DEBUG

	OkiSyogun();

	//ImGui::DragFloat3("cameraTrans", &camera_->transform.translation_.x, 1.0f);
	//ImGui::DragFloat3("cameraRotate", &camera_->transform.rotation_.x, AngleToRadian(1.0f));
	//ImGui::DragFloat3("shopTra", &tentyoTransform[Body].translation_.x, 1.0f);
	//ImGui::DragFloat3("shopRota", &tentyoTransform[Body].rotation_.x, AngleToRadian(1.0f));
	

	if (cameraStep == CAMERASTEP::Zero) {
		if (KeyInput::GetInstance()->GetPadButton(XINPUT_GAMEPAD_DPAD_LEFT) || KeyInput::GetKey(DIK_LEFTARROW)) {
			camera_->transform.rotation_.y -= AngleToRadian(1.0f);
		}
		else if (KeyInput::GetInstance()->GetPadButton(XINPUT_GAMEPAD_DPAD_RIGHT) || KeyInput::GetKey(DIK_RIGHTARROW)) {
			camera_->transform.rotation_.y += AngleToRadian(1.0f);
		}
		camera_->transform.rotation_.y = std::clamp<float>(camera_->transform.rotation_.y, AngleToRadian(25.0f), AngleToRadian(90.0f));
	}
	

	CameraMove();

	worldTransform.UpdateMatrix();

	chara.UpdateMatrix();

	player->Update();

	for (auto& shop : shopTrans) {
		shop.UpdateMatrix();
	}
	
	for (auto& ene : enemy_) {
		ene->Update();
	}
	for (auto& parts : parts_) {
		parts.UpdateMatrix();
	}
	TenchoW_.UpdateMatrix();
	for (auto& tentyoParts : tentyoTransform) {
		tentyoParts.UpdateMatrix();
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

	for (uint16_t i = 0u; i < tentyoModels_.size(); i++) {
		Model::ModelDraw(tentyoTransform[i], viewProjection, 0xffffffff, tentyoModels_[i].get());
	}

	if (cameraStep == CAMERASTEP::BounceFace) {
		for (uint16_t i = 0u; i < parts_.size(); i++) {
			Model::ModelDraw(parts_[i], viewProjection, 0xffffffff, mobModels_[0][i].get());
		}
	}
	else {
		player->Draw(viewProjection);
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
	

	tentyoTransform[Body].translation_;

#pragma region 店長パーツの親子関係と座標の初期設定
	tentyoTransform[Head].parent_ = &tentyoTransform[Body];
	tentyoTransform[BodyUnder].parent_ = &tentyoTransform[Body];

	tentyoTransform[LArm1].parent_ = &tentyoTransform[Body];
	tentyoTransform[LArm2].parent_ = &tentyoTransform[LArm1];
	tentyoTransform[LHand].parent_ = &tentyoTransform[LArm2];

	tentyoTransform[RArm1].parent_ = &tentyoTransform[Body];
	tentyoTransform[RArm2].parent_ = &tentyoTransform[RArm1];
	tentyoTransform[RHand].parent_ = &tentyoTransform[RArm2];

	tentyoTransform[LLeg1].parent_ = &tentyoTransform[BodyUnder];
	tentyoTransform[LLeg2].parent_ = &tentyoTransform[LLeg1];
	tentyoTransform[LFoot].parent_ = &tentyoTransform[LLeg2];

	tentyoTransform[RLeg1].parent_ = &tentyoTransform[BodyUnder];
	tentyoTransform[RLeg2].parent_ = &tentyoTransform[RLeg1];
	tentyoTransform[RFoot].parent_ = &tentyoTransform[RLeg2];

	tentyoTransform[Num].parent_ = &tentyoTransform[BodyUnder];

	//座標設定
	tentyoTransform[Body].translation_ = { 0.0f, -2.0f, -3.0f };
	tentyoTransform[Body].rotation_ = { AngleToRadian(90.0f), 0.0f, AngleToRadian(180.0f)};
	tentyoTransform[BodyUnder].translation_ = { 0.0f, 0.0f, 0.0f };
	tentyoTransform[Head].translation_ = { 0.0f, 2.6f, 0.0f };

	tentyoTransform[LArm1].translation_ = { -0.8f, 1.57f, 0.0f };
	tentyoTransform[LArm2].translation_ = { -1.73f, 0.0f, 0.0f };
	tentyoTransform[LHand].translation_ = { -2.37f, 0.0f, 0.0f };

	tentyoTransform[RArm1].translation_ = { 0.8f, 1.57f, 0.0f };
	tentyoTransform[RArm2].translation_ = { 1.73f, 0.0f, 0.0f };
	tentyoTransform[RHand].translation_ = { 2.37f, 0.0f, 0.0f };

	tentyoTransform[LLeg1].translation_ = { -0.3f, -1.7f, 0.0f };
	tentyoTransform[LLeg2].translation_ = { 0.0f, -2.2f, 0.0f };
	tentyoTransform[LFoot].translation_ = { -0.12f, -2.2f, 0.0f };

	tentyoTransform[RLeg1].translation_ = { 0.3f, -1.7f, 0.0f };
	tentyoTransform[RLeg2].translation_ = { 0.0f, -2.2f, 0.0f };
	tentyoTransform[RFoot].translation_ = { 0.12f, -2.2f, 0.0f };
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

			sAppearance.SoundPlayWave();
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
			//endPoint = Vector3(0.0f, 8.0f, 155.0f);
			endPoint = shopTrans[static_cast<uint16_t>(SHOPPARTS::Signboard)].translation_;
			endPoint.z -= 2.5f;
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
			chara.translation_.y = 0.0f;

			sDawnsound.SoundPlayWave();

		}
		else if (easeNowFrame <= easeMaxFrame) {
			camera_->transform.translation_ = Ease::UseEase(startingPoint, endPoint, easeNowFrame, easeMaxFrame, Ease::EaseType::EaseOutBounce);
		}
		break;
	case Title::CAMERASTEP::BounceFace:
		easeNowFrame++;
		if (easeNowFrame >= easeMaxFrame + 20) {
			//cameraStep = CAMERASTEP::Zero;

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
