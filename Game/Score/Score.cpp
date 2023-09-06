#include "Score.h"

void Score::Initialize(std::vector<std::shared_ptr<Texture2D>> numberTextures) {
	
	numberTextures_ = numberTextures;

}

void Score::DrawScore(WorldTransform& worldTransform, Matrix4x4 viewProjectionMat, uint32_t color) {

	DrawParameter(score_, kScoreMaxDigits_, 0, worldTransform, viewProjectionMat, color);

}

void Score::DrawCombo(WorldTransform& worldTransform, Matrix4x4 viewProjectionMat, uint32_t color) {

	DrawParameter(combo_, kComboMaxDigits_, 1, worldTransform, viewProjectionMat, color);

}

void Score::DrawHighCombo(WorldTransform& worldTransform, Matrix4x4 viewProjectionMat, uint32_t color) {

	DrawParameter(highCombo_, kComboMaxDigits_, 0, worldTransform, viewProjectionMat, color);

}

void Score::DrawPerfectNum(WorldTransform& worldTransform, Matrix4x4 viewProjectionMat, uint32_t color) {

	DrawParameter(perfectNum_, kComboMaxDigits_, 0, worldTransform, viewProjectionMat, color);

}

void Score::DrawGreatNum(WorldTransform& worldTransform, Matrix4x4 viewProjectionMat, uint32_t color) {

	DrawParameter(greatNum_, kComboMaxDigits_, 0, worldTransform, viewProjectionMat, color);
}

void Score::DrawGoodNum(WorldTransform& worldTransform, Matrix4x4 viewProjectionMat, uint32_t color) {

	DrawParameter(goodNum_, kComboMaxDigits_, 0, worldTransform, viewProjectionMat, color);
}

void Score::DrawMissNum(WorldTransform& worldTransform, Matrix4x4 viewProjectionMat, uint32_t color) {

	DrawParameter(missNum_, kComboMaxDigits_, 0, worldTransform, viewProjectionMat, color);
}

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

void Score::AddCom() {
	combo_++;
	if (highCombo_ < combo_) {
		highCombo_ = combo_;
	}
}