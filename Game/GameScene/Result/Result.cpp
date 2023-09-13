#include "Result.h"

#include "Engine/Input/KeyInput/KeyInput.h"
#include "Game/GameScene/GameScene.h"
#include"externals/imgui/imgui.h"

Result::Result(std::shared_ptr<Camera> camera) {
	camera_ = camera;

}

void Result::Initialize()
{

	
	ui_->Initialize();

	//プレイヤー
	parts_.resize(mobModels_.size());
	//店
	shopTransform_.resize(shopModels_.size());
	//店長
	tentyoTransform_.resize(tentyoModels_.size());

	//親子関係
	camera_->transform.parent_ = &cameraR_target_;

	tentyoW_.parent_ = &shopTransform_[0];

	playerW_.parent_ = &shopTransform_[0];

	//店長ボディを店に設定
	tentyoTransform_[Body].parent_ = &tentyoW_;
	parts_[Body].parent_ = &playerW_;

#pragma region パーツの親子関係と座標の初期設定
	tentyoTransform_[Head].parent_ = &tentyoTransform_[Body];
	tentyoTransform_[BodyUnder].parent_ = &tentyoTransform_[Body];

	tentyoTransform_[LArm1].parent_ = &tentyoTransform_[Body];
	tentyoTransform_[LArm2].parent_ = &tentyoTransform_[LArm1];
	tentyoTransform_[LHand].parent_ = &tentyoTransform_[LArm2];

	tentyoTransform_[RArm1].parent_ = &tentyoTransform_[Body];
	tentyoTransform_[RArm2].parent_ = &tentyoTransform_[RArm1];
	tentyoTransform_[RHand].parent_ = &tentyoTransform_[RArm2];

	tentyoTransform_[LLeg1].parent_ = &tentyoTransform_[BodyUnder];
	tentyoTransform_[LLeg2].parent_ = &tentyoTransform_[LLeg1];
	tentyoTransform_[LFoot].parent_ = &tentyoTransform_[LLeg2];

	tentyoTransform_[RLeg1].parent_ = &tentyoTransform_[BodyUnder];
	tentyoTransform_[RLeg2].parent_ = &tentyoTransform_[RLeg1];
	tentyoTransform_[RFoot].parent_ = &tentyoTransform_[RLeg2];

	tentyoTransform_[Num].parent_ = &tentyoTransform_[BodyUnder];
	//座標設定
	tentyoTransform_[Body].translation_ = { 0.0f, 0.0f, 0.0f };
	tentyoTransform_[BodyUnder].translation_ = { 0.0f, 0.0f, 0.0f };
	tentyoTransform_[Head].translation_ = { 0.0f, 2.6f, 0.0f };

	tentyoTransform_[LArm1].translation_ = { -0.8f, 1.57f, 0.0f };
	tentyoTransform_[LArm2].translation_ = { -1.73f, 0.0f, 0.0f };
	tentyoTransform_[LHand].translation_ = { -2.37f, 0.0f, 0.0f };

	tentyoTransform_[RArm1].translation_ = { 0.8f, 1.57f, 0.0f };
	tentyoTransform_[RArm2].translation_ = { 1.73f, 0.0f, 0.0f };
	tentyoTransform_[RHand].translation_ = { 2.37f, 0.0f, 0.0f };

	tentyoTransform_[LLeg1].translation_ = { -0.3f, -1.7f, 0.0f };
	tentyoTransform_[LLeg2].translation_ = { 0.0f, -2.2f, 0.0f };
	tentyoTransform_[LFoot].translation_ = { -0.12f, -2.2f, 0.0f };

	tentyoTransform_[RLeg1].translation_ = { 0.3f, -1.7f, 0.0f };
	tentyoTransform_[RLeg2].translation_ = { 0.0f, -2.2f, 0.0f };
	tentyoTransform_[RFoot].translation_ = { 0.12f, -2.2f, 0.0f };

	
#pragma endregion
	
#pragma region パーツの親子関係と座標の初期設定(プレイヤー)
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



	//以下数値設定
	camera_->transform.translation_ = Vector3(0.0f, 3.0f, -40.0f);
	camera_->transform.rotation_ = Vector3(0.0f, 0.0f, 0.0f);
	camera_->transform.scale_ = Vector3(1.0f, 1.0f, 0.5f);


	shopTransform_[0].scale_ = { 10.0f,10.0f,10.0f };

	playerW_.translation_ = Vector3(1.7f, 1.0f, 1.0f);
	playerW_.scale_ = Vector3(0.2f, 0.2f, 0.2f);
	playerW_.rotation_ = Vector3(0.0f, 3.14f, 0.0f);

	tentyoW_.translation_ = Vector3(-1.7f, 1.3f, 2.0f);
	tentyoW_.scale_ = Vector3(0.2f, 0.2f, 0.2f);
	tentyoW_.rotation_ = Vector3(0.0f, 2.2f, 0.0f);

	cameraR_target_.translation_ = Vector3(0.0f, 7.7f, 0.0f);
	cameraR_target_.rotation_ = Vector3(0.7f, 0.0f, 0.0f);


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
		{-0.15f, 0.5f, 0.0f},
		{-0.15f, 0.5f, 0.0f},
	};
	eatRamen[LArm2] = {
		{0.0f, 1.76f, 0.0f},
		{0.0f, 1.76f, 0.0f},
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

#pragma region 王騎将軍ポーズ
	okisyogun.resize(tentyoTransform_.size());
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

#pragma region 悔しいモーション
	kuyasii.resize(parts_.size());
	kuyasii[Body] = {
		{1.8f, 0.0f, 0.0f},
		{1.8f, 0.0f, 0.0f},
	};
	kuyasii[Head] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	kuyasii[BodyUnder] = {
		{-0.45f, 0.0f, 0.0f},
		{-0.45f, 0.0f, 0.0f},
	};
	//
	kuyasii[LArm1] = {
		{0.0f, 1.5f, 0.0f},
		{0.0f, 1.5f, 0.0f},
	};
	kuyasii[LArm2] = {
		{0.0f, 0.0f, -1.7f},
		{0.0f, 0.0f, -2.5f},
	};
	kuyasii[LHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	//
	kuyasii[RArm1] = {
		{0.0f, -1.5f, 0.0f},
		{0.0f, -1.5f, 0.0f},
	};
	kuyasii[RArm2] = {
		{0.0f, 0.0f, 1.7f},
		{0.0f, 0.0f, 1.7f},
	};
	kuyasii[RHand] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	//
	kuyasii[LLeg1] = {
		{-1.2f, 0.0f, 0.0f},
		{-1.2f, 0.0f, 0.0f},
	};
	kuyasii[LLeg2] = {
		{2.0f, 0.0f, 0.0f},
		{2.0f, 0.0f, 0.0f},
	};
	kuyasii[LFoot] = {
		{-0.2f, 0.0f, 0.0f},
		{-0.2f, 0.0f, 0.0f},
	};

	//
	kuyasii[RLeg1] = {
		{-1.2f, 0.0f, 0.0f},
		{-1.2f, 0.0f, 0.0f},
	};
	kuyasii[RLeg2] = {
		{2.0f, 0.0f, 0.0f},
		{2.0f, 0.0f, 0.0f},
	};
	kuyasii[RFoot] = {
		{-0.2f, 0.0f, 0.0f},
		{-0.2f, 0.0f, 0.0f},
	};
#pragma endregion

	isAnimeStart_ = false;
	isOkiStart_ = false;

}

void Result::Update()
{
	//camera_->transform.scale_ = Vector3(1.0f, 1.0f, 0.5f);

	cameraR_target_.rotation_.y += (1.0f / 360.0f)*3.14f;

#ifdef _DEBUG
	ImGui::Begin("Result");
	ImGui::Text("camera");
	ImGui::DragFloat3("C pos", &camera_->transform.translation_.x, 0.1f);
	ImGui::DragFloat3("C rotate", &camera_->transform.rotation_.x, 0.1f);
	ImGui::DragFloat3("C scale", &camera_->transform.scale_.x, 0.01f);
	ImGui::Text("cameraR");
	ImGui::DragFloat3("CR pos", &cameraR_target_.translation_.x, 0.1f);
	ImGui::DragFloat3("CR rotate", &cameraR_target_.rotation_.x, 0.1f);
	ImGui::DragFloat3("CR scale", &cameraR_target_.scale_.x, 0.01f);


	ImGui::Text("shop");
	ImGui::DragFloat3("S pos", &shopTransform_[0].translation_.x, 0.1f);
	ImGui::DragFloat3("S rotate", &shopTransform_[0].rotation_.x, 0.1f);
	ImGui::DragFloat3("S scale", &shopTransform_[0].scale_.x, 0.1f);

	ImGui::Text("player");
	ImGui::DragFloat3("P pos", &playerW_.translation_.x, 0.1f);
	ImGui::DragFloat3("P rotate", &playerW_.rotation_.x, 0.1f);
	ImGui::DragFloat3("P scale", &playerW_.scale_.x, 0.1f);

	ImGui::Text("tentyo");
	ImGui::DragFloat3("T pos", &tentyoW_.translation_.x, 0.1f);
	ImGui::DragFloat3("T Rotate", &tentyoW_.rotation_.x, 0.1f);
	ImGui::End();

#endif // _DEBUG

	if (clearFlag_) {
		// 時間内に来た時


	}
	else {
		// 間に合わなかった時

	}

	EatRamen();
	//Kuyasii();
	OkiSyogun();
	
	ui_->Update();

	cameraR_target_.UpdateMatrix();
	camera_->transform.UpdateMatrix();

	//アップデート
	for (auto& i : shopTransform_) {
		i.UpdateMatrix();
	}
	playerW_.UpdateMatrix();
	tentyoW_.UpdateMatrix();

	for (auto& i : parts_) {
		i.UpdateMatrix();
	}
	for (auto& i : tentyoTransform_) {
		i.UpdateMatrix();
	}

}

void Result::Draw(Matrix4x4 viewProjection)
{
	for (uint16_t i = 0u; i < mobModels_.size(); i++) {
		Model::ModelDraw(parts_[i], viewProjection, 0xffffffff, mobModels_[i].get());
	}
	for (uint16_t i = 0u; i < shopModels_.size(); i++) {
		Model::ModelDraw(shopTransform_[i], viewProjection, 0xffffffff, shopModels_[i].get());
	}
	
	for (uint16_t i = 0u; i < tentyoModels_.size(); i++) {
		Model::ModelDraw(tentyoTransform_[i], viewProjection, 0xffffffff, tentyoModels_[i].get());
	}
}

void Result::Draw2D(const Matrix4x4& viewProjection) {

	ui_->Draw2D(viewProjection);
}




#pragma region animation
//ラーメン食べるモーション
void Result::EatRamen() {
	//アニメーション初期化
	if (!isAnimeStart_) {
		isAnimeStart_ = true;
		isLoop_ = false;

		//アニメーションポーズ設定
		for (int i = 0; i < PARTS::Num; i++) {
			parts_[i].rotation_ = eatRamen[i].st;
		}
		T_ = 0;

	}//更新
	else {

		for (int i = 0; i < PARTS::Num; i++) {
			parts_[i].rotation_ = ES(eatRamen[i], T_);
		}

		//以下ループT
		float ADD = 1.0f / 30.0f;
		if (!isLoop_) {
			T_ += ADD;
			if (T_ >= 1.0f) {
				isLoop_ = true;
				T_ = 1.0f;
			}
		}
		else {
			T_ -= ADD;
			if (T_ <= 0.0f) {
				isLoop_ = false;
				T_ = 0;
			}
		}

	}
}

//将軍ポーズ店長
void Result::OkiSyogun() {
	if (!isOkiStart_) {
		isOkiStart_ = true;
		//アニメーションポーズ設定
		for (int i = 0; i < PARTS::Num; i++) {
			tentyoTransform_[i].rotation_ = okisyogun[i].st;
		}
		isL_ = false;
		OkiT_ = 0;
	}
	else {

		for (int i = 0; i < PARTS::Num; i++) {
			tentyoTransform_[i].rotation_ = ES(okisyogun[i], OkiT_);
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

//悔しがりアニメ
void Result::Kuyasii() {
	//アニメーション初期化
	if (!isAnimeStart_) {
		isAnimeStart_ = true;
		isLoop_ = false;

		//アニメーションポーズ設定
		for (int i = 0; i < PARTS::Num; i++) {
			parts_[i].rotation_ = kuyasii[i].st;
		}
		T_ = 0;

	}//更新
	else {
		for (int i = 0; i < PARTS::Num; i++) {
			parts_[i].rotation_ = ES(kuyasii[i], T_);
		}
		//以下ループT
		float ADD = 1.0f / 30.0f;
		if (!isLoop_) {
			T_ += ADD;
			if (T_ >= 1.0f) {
				isLoop_ = true;
				T_ = 1.0f;
			}
		}
		else {
			T_ -= ADD;
			if (T_ <= 0.0f) {
				isLoop_ = false;
				T_ = 0;
			}
		}

	}
}
#pragma endregion
