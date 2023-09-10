#include "Score.h"

Score::Score() {

	for (int i = 0; i < kParameterNum_; i++) {
		if (i == kScore || i == kMemoHighScore) {
			for (int j = 0; j < kScoreMaxDigits_; j++) {
				worldTransforms_[i].push_back(std::make_shared<WorldTransform>());
			}
		}
		else {
			for (int j = 0; j < kComboMaxDigits_; j++) {
				worldTransforms_[i].push_back(std::make_shared<WorldTransform>());
			}
		}

		colors_[i] = 0xFFFFFFFF;

		parameters_[i] = 0;

		isDraw_[i] = false;
	}

}

void Score::Initialize() {
	
	Reset();

}

void Score::ResetIsDraw() {
	for (int i = 0; i < kParameterNum_; i++) {
		isDraw_[i] = false;
	}
}

void Score::SetWorldTransform(const Vector2& screenPos, float scale, float rotate, int parameter) {

	Vector2 pos = { screenPos.x - WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2 - screenPos.y };

	int i = 0;

	for (std::shared_ptr<WorldTransform> worldTransform : worldTransforms_[parameter]) {

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

void Score::DrawParameter(const Matrix4x4& viewProjectionMat, int parameter) {


	if (isDraw_[parameter]) {
		if (parameter == kCombo) {
			if (parameters_[parameter] < 2) {
				return;
			}
		}

		int i = 0;
		int num = 0;
		int parameterNum = parameters_[parameter];
		int k = 0;

		int digits = 0;

		if (parameter == kScore || parameter == kMemoHighScore) {
			digits = kScoreMaxDigits_;
		}
		else {
			digits = kComboMaxDigits_;
		}

		for (std::shared_ptr<WorldTransform> worldTransform : worldTransforms_[parameter]) {

			num = parameterNum / int(pow(10, digits - 1 - i));
			parameterNum = parameterNum % int(pow(10, digits - 1 - i));

			if (num > 9) {
				num = 1;
			}

			if (parameter == kCombo) {
				if (num != 0) {
					k++;
				}
			}

			if (parameter == kCombo) {
				if (k != 0) {
					Texture2D::TextureDraw(*(worldTransform.get()), viewProjectionMat, colors_[parameter], numberTextures_[num].get());
				}
			}
			else {
				Texture2D::TextureDraw(*(worldTransform.get()), viewProjectionMat, colors_[parameter], numberTextures_[num].get());
			}

			i++;
		}
	}
}

void Score::Draw2D(const Matrix4x4& viewProjectionMat) {

	for (int i = 0; i < kParameterNum_; i++) {
		DrawParameter(viewProjectionMat, i);
	}

}

void Score::Reset() {

	for (int i = 0; i < 7; i++) {
		parameters_[i] = 0;
	}

	isFullCom_ = false;


	ResetEvalution();
}


void Score::Memo() {
	
	IsFullComUpdate();

	if (parameters_[kMemoHighScore] < parameters_[kScore]) {
		parameters_[kMemoHighScore] = parameters_[kScore];
	}

	if (parameters_[kMemoHighCombo] < parameters_[kHighCombo]) {
		parameters_[kMemoHighCombo] = parameters_[kHighCombo];
	}
}

void Score::IsFullComUpdate() {
	if (parameters_[kMissNum] == 0 && parameters_[kGoodNum] == 0) {
		isFullCom_ = true;
	} else {
		isFullCom_ = false;
	}
}

void Score::AddCombo() {
	parameters_[kCombo]++;
	if (parameters_[kHighCombo] < parameters_[kCombo]) {
		parameters_[kHighCombo] = parameters_[kCombo];
	}
}