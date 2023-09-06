#include "Score.h"

void Score::Initialize(std::vector<std::shared_ptr<Texture2D>> numberTextures) {
	
	numberTextures_ = numberTextures;

}

void Score::DrawScore(const Vector2& screenPos, float scale, float rotate, const Matrix4x4& viewProjectionMat, uint32_t color) {

	Vector2 pos = { screenPos.x - WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2 - screenPos.y };

	int parameter = score_;
	int digits = kScoreMaxDigits_;
	int k = 0;

	for (int i = 0; i < digits; i++) {
		int num = parameter / int(pow(10, digits - 1 - i));
		parameter = parameter % int(pow(10, digits - 1 - i));

		if (num > 9) {
			num = 1;
		}

		scoreWorldTransform_[i].translation_.x = pos.x + kNumberTextureWidth_ * i * scale;
		scoreWorldTransform_[i].translation_.y = pos.y;
		scoreWorldTransform_[i].scale_.x = scale;
		scoreWorldTransform_[i].scale_.y = scale;
		scoreWorldTransform_[i].rotation_.z = rotate;


		if (rotate != 0) {
			float a = kNumberTextureWidth_ * i * scale;

			scoreWorldTransform_[i].translation_.x = a * std::cosf(rotate) + pos.x;
			scoreWorldTransform_[i].translation_.y = a * std::sinf(rotate) + pos.y;

		}

		scoreWorldTransform_[i].UpdateMatrix();

		Texture2D::TextureDraw(scoreWorldTransform_[i], viewProjectionMat, color, numberTextures_[num].get());

	}
}

void Score::DrawCombo(const Vector2& screenPos, float scale, float rotate, const Matrix4x4& viewProjectionMat, uint32_t color) {

	Vector2 pos = { screenPos.x - WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2 - screenPos.y };

	int parameter = combo_;
	int digits = kComboMaxDigits_;
	int k = 0;

	for (int i = 0; i < digits; i++) {
		int num = parameter / int(pow(10, digits - 1 - i));
		parameter = parameter % int(pow(10, digits - 1 - i));

		if (num > 9) {
			num = 1;
		}

		comboWorldTransform_[i].translation_.x = pos.x + kNumberTextureWidth_ * i * scale;
		comboWorldTransform_[i].translation_.y = pos.y;
		comboWorldTransform_[i].scale_.x = scale;
		comboWorldTransform_[i].scale_.y = scale;
		comboWorldTransform_[i].rotation_.z = rotate;


		if (rotate != 0) {
			float a = kNumberTextureWidth_ * i * scale;

			comboWorldTransform_[i].translation_.x = a * std::cosf(rotate) + pos.x;
			comboWorldTransform_[i].translation_.y = a * std::sinf(rotate) + pos.y;

		}

		comboWorldTransform_[i].UpdateMatrix();

		if (num != 0) {
			k++;
		}

		if (k != 0) {
			Texture2D::TextureDraw(comboWorldTransform_[i], viewProjectionMat, color, numberTextures_[num].get());
		}

	}
}

void Score::DrawHighCombo(const Vector2& screenPos, float scale, float rotate, const Matrix4x4& viewProjectionMat, uint32_t color) {

	Vector2 pos = { screenPos.x - WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2 - screenPos.y };

	int parameter = highCombo_;
	int digits = kComboMaxDigits_;

	for (int i = 0; i < digits; i++) {
		int num = parameter / int(pow(10, digits - 1 - i));
		parameter = parameter % int(pow(10, digits - 1 - i));

		if (num > 9) {
			num = 1;
		}

		highComboWorldTransform_[i].translation_.x = pos.x + kNumberTextureWidth_ * i * scale;
		highComboWorldTransform_[i].translation_.y = pos.y;
		highComboWorldTransform_[i].scale_.x = scale;
		highComboWorldTransform_[i].scale_.y = scale;
		highComboWorldTransform_[i].rotation_.z = rotate;


		if (rotate != 0) {
			float a = kNumberTextureWidth_ * i * scale;

			highComboWorldTransform_[i].translation_.x = a * std::cosf(rotate) + pos.x;
			highComboWorldTransform_[i].translation_.y = a * std::sinf(rotate) + pos.y;

		}

		highComboWorldTransform_[i].UpdateMatrix();

		Texture2D::TextureDraw(highComboWorldTransform_[i], viewProjectionMat, color, numberTextures_[num].get());

	}
}

void Score::DrawPerfectNum(const Vector2& screenPos, float scale, float rotate, const Matrix4x4& viewProjectionMat, uint32_t color) {

	Vector2 pos = { screenPos.x - WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2 - screenPos.y };

	int parameter = perfectNum_;
	int digits = kComboMaxDigits_;

	for (int i = 0; i < digits; i++) {
		int num = parameter / int(pow(10, digits - 1 - i));
		parameter = parameter % int(pow(10, digits - 1 - i));

		if (num > 9) {
			num = 1;
		}

		perfectNumWorldTransform_[i].translation_.x = pos.x + kNumberTextureWidth_ * i * scale;
		perfectNumWorldTransform_[i].translation_.y = pos.y;
		perfectNumWorldTransform_[i].scale_.x = scale;
		perfectNumWorldTransform_[i].scale_.y = scale;
		perfectNumWorldTransform_[i].rotation_.z = rotate;


		if (rotate != 0) {
			float a = kNumberTextureWidth_ * i * scale;

			perfectNumWorldTransform_[i].translation_.x = a * std::cosf(rotate) + pos.x;
			perfectNumWorldTransform_[i].translation_.y = a * std::sinf(rotate) + pos.y;

		}

		perfectNumWorldTransform_[i].UpdateMatrix();

		Texture2D::TextureDraw(perfectNumWorldTransform_[i], viewProjectionMat, color, numberTextures_[num].get());

	}

}

void Score::DrawGreatNum(const Vector2& screenPos, float scale, float rotate, const Matrix4x4& viewProjectionMat, uint32_t color) {

	Vector2 pos = { screenPos.x - WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2 - screenPos.y };

	int parameter = greatNum_;
	int digits = kComboMaxDigits_;

	for (int i = 0; i < digits; i++) {
		int num = parameter / int(pow(10, digits - 1 - i));
		parameter = parameter % int(pow(10, digits - 1 - i));

		if (num > 9) {
			num = 1;
		}

		greatNumWorldTransform_[i].translation_.x = pos.x + kNumberTextureWidth_ * i * scale;
		greatNumWorldTransform_[i].translation_.y = pos.y;
		greatNumWorldTransform_[i].scale_.x = scale;
		greatNumWorldTransform_[i].scale_.y = scale;
		greatNumWorldTransform_[i].rotation_.z = rotate;


		if (rotate != 0) {
			float a = kNumberTextureWidth_ * i * scale;

			greatNumWorldTransform_[i].translation_.x = a * std::cosf(rotate) + pos.x;
			greatNumWorldTransform_[i].translation_.y = a * std::sinf(rotate) + pos.y;

		}

		greatNumWorldTransform_[i].UpdateMatrix();

		Texture2D::TextureDraw(greatNumWorldTransform_[i], viewProjectionMat, color, numberTextures_[num].get());

	}
}

void Score::DrawGoodNum(const Vector2& screenPos, float scale, float rotate, const Matrix4x4& viewProjectionMat, uint32_t color) {

	Vector2 pos = { screenPos.x - WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2 - screenPos.y };

	int parameter = goodNum_;
	int digits = kComboMaxDigits_;

	for (int i = 0; i < digits; i++) {
		int num = parameter / int(pow(10, digits - 1 - i));
		parameter = parameter % int(pow(10, digits - 1 - i));

		if (num > 9) {
			num = 1;
		}

		goodNumWorldTransform_[i].translation_.x = pos.x + kNumberTextureWidth_ * i * scale;
		goodNumWorldTransform_[i].translation_.y = pos.y;
		goodNumWorldTransform_[i].scale_.x = scale;
		goodNumWorldTransform_[i].scale_.y = scale;
		goodNumWorldTransform_[i].rotation_.z = rotate;


		if (rotate != 0) {
			float a = kNumberTextureWidth_ * i * scale;

			goodNumWorldTransform_[i].translation_.x = a * std::cosf(rotate) + pos.x;
			goodNumWorldTransform_[i].translation_.y = a * std::sinf(rotate) + pos.y;

		}

		goodNumWorldTransform_[i].UpdateMatrix();

		Texture2D::TextureDraw(goodNumWorldTransform_[i], viewProjectionMat, color, numberTextures_[num].get());

	}
}

void Score::DrawMissNum(Vector2 screenPos, float scale, float rotate, Matrix4x4 viewProjectionMat, uint32_t color) {

	Vector2 pos = { screenPos.x - WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2 - screenPos.y };

	int parameter = missNum_;
	int digits = kComboMaxDigits_;

	for (int i = 0; i < digits; i++) {
		int num = parameter / int(pow(10, digits - 1 - i));
		parameter = parameter % int(pow(10, digits - 1 - i));

		if (num > 9) {
			num = 1;
		}

		missNumWorldTransform_[i].translation_.x = pos.x + kNumberTextureWidth_ * i * scale;
		missNumWorldTransform_[i].translation_.y = pos.y;
		missNumWorldTransform_[i].scale_.x = scale;
		missNumWorldTransform_[i].scale_.y = scale;
		missNumWorldTransform_[i].rotation_.z = rotate;
		

		if (rotate != 0) {
			float a = kNumberTextureWidth_ * i * scale;

			missNumWorldTransform_[i].translation_.x = a * std::cosf(rotate) + pos.x;
			missNumWorldTransform_[i].translation_.y = a * std::sinf(rotate) + pos.y;

		}

		missNumWorldTransform_[i].UpdateMatrix();

		Texture2D::TextureDraw(missNumWorldTransform_[i], viewProjectionMat, color, numberTextures_[num].get());

	}
}

// gomi
void Score::DrawParameter(int parameter, int digits, bool look, WorldTransform& worldTransform, Matrix4x4 viewProjectionMat, uint32_t color) {


	int para = parameter;
	int k = 0;

	for (int i = 0; i < digits; i++) {
		int num = para / int(pow(10, digits - 1 - i));
		para = para % int(pow(10, digits - 1 - i));

		if (num > 9) {
			num = 1;
		}

		if (look) {
			if (num != 0) {
				k++;
			}
		}

		WorldTransform tmp = worldTransform;

		tmp.translation_.x = worldTransform.translation_.x + kNumberTextureWidth_ * i * worldTransform.scale_.x;

		if (tmp.rotation_.z != 0) {
			Vector2 a = Vector2{ tmp.translation_.x,worldTransform.translation_.y } - Vector2{ worldTransform.translation_.x, worldTransform.translation_.y };
			tmp.translation_.x = a.x * std::cosf(worldTransform.rotation_.z) - a.y * std::sinf(worldTransform.rotation_.z) + worldTransform.translation_.x;
			tmp.translation_.y = a.y * std::cosf(worldTransform.rotation_.z) + a.x * std::sinf(worldTransform.rotation_.z) + worldTransform.translation_.y;

		}

		tmp.UpdateMatrix();

		Texture2D::TextureDraw(tmp, viewProjectionMat, color, numberTextures_[num].get());
		
	}

	
}

void Score::Reset() {

	score_ = 0;
	combo_ = 0;
	perfectNum_ = 0;
	greatNum_ = 0;
	goodNum_ = 0;
	missNum_ = 0;
	isFullCom_ = false;
	highCombo_ = 0;

	ResetEvalution();
}

void Score::SetUpdate(const Score& score) {
	
	if (score_ < score.score_) {
		score_ = score.score_;
	}

	if (highCombo_ < score.highCombo_) {
		highCombo_ = score.highCombo_;
	}

	if (!isFullCom_) {
		isFullCom_ = score.isFullCom_;
	}
}

void Score::IsFullComUpdate() {
	if (missNum_ == 0 && goodNum_ == 0) {
		isFullCom_ = true;
	} else {
		isFullCom_ = false;
	}
}

void Score::AddCombo() {
	combo_++;
	if (highCombo_ < combo_) {
		highCombo_ = combo_;
	}
}