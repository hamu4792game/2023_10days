#include "Timer.h"

#include "GlobalVariables/GlobalVariables.h"

Timer::Timer() {

	for (int i = 0; i < kMaxDigits_; i++) {
		worldTransforms_.push_back(std::make_shared<WorldTransform>());
	}
}

void Timer::Initialize() {

	time_ = kMaxTime_;

	countFrame_ = 0;

	color_ = 0xFFFFFFFF;

	isDraw_ = true;

	SetGlobalVariable();

	ApplyGlobalVariable();
}

void Timer::SetGlobalVariable() {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();


	const char* groupName = "Timer";

	globalVariables->CreateGroup(groupName);

	globalVariables->AddItem(groupName, "pos", pos_);
	globalVariables->AddItem(groupName, "scale", scale_);
}

void Timer::ApplyGlobalVariable() {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "Timer";

	pos_ = globalVariables->GetVector2Value(groupName, "pos");
	scale_ = globalVariables->GetFloatValue(groupName, "scale");


	SetWorldTransform(pos_, scale_, 0.0f);
}

void Timer::Update() {

	ApplyGlobalVariable();

	countFrame_++;

	if (countFrame_ == 60) {
		countFrame_ = 0;
		time_--;
	}

}

// posは一番左の数字の真ん中の座標。
void Timer::SetWorldTransform(const Vector2& screenPos, float scale, float rotate) {

	Vector2 pos = { screenPos.x - WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2 - screenPos.y };

	int i = 0;

	for (std::shared_ptr<WorldTransform> worldTransform : worldTransforms_) {

		worldTransform->translation_.x = pos.x + kNumberTextureWidth_ * i * scale;
		worldTransform->translation_.y = pos.y;
		worldTransform->scale_.x = scale;
		worldTransform->scale_.y = scale;
		worldTransform->rotation_.z = rotate;


		if (rotate != 0) {
			float a = kNumberTextureWidth_ * i * scale;

			worldTransform->translation_.x = a * std::cosf(rotate) + pos.x;
			worldTransform->translation_.y = a * std::sinf(rotate) + pos.y;

		}

		worldTransform->UpdateMatrix();

		i++;
	}


}

void Timer::Draw2D(const Matrix4x4& viewProjectionMat) {

	if (isDraw_) {

		int i = 0;
		int num = 0;
		int parameterNum = time_;

		int digits = kMaxDigits_;

		for (std::shared_ptr<WorldTransform> worldTransform : worldTransforms_) {

			num = parameterNum / int(pow(10, digits - 1 - i));
			parameterNum = parameterNum % int(pow(10, digits - 1 - i));

			if (num > 9) {
				num = 1;
			}

			Texture2D::TextureDraw(*(worldTransform.get()), viewProjectionMat, color_, numberTextures_[num].get());

			i++;
		}
	}



}