#include "Score.h"

void Score::Initialize(std::vector<std::shared_ptr<Texture2D>> numberTextures) {
	
	numberTextures_ = numberTextures;

}

void Score::DrawScore(Vector2 pos, Vector2 scale, float rotate, Matrix4x4 viewProjectionMat, uint32_t color) {

	DrawParameter(score_, kScoreMaxDigits_, 0, pos, scale, rotate, viewProjectionMat, color);

}

void Score::DrawCombo(Vector2 pos, Vector2 scale, float rotate, Matrix4x4 viewProjectionMat, uint32_t color) {

	DrawParameter(combo_, kComboMaxDigits_, 1, pos, scale, rotate, viewProjectionMat, color);

}

void Score::DrawHighCombo(Vector2 pos, Vector2 scale, float rotate, Matrix4x4 viewProjectionMat, uint32_t color) {

	DrawParameter(highCombo_, kComboMaxDigits_, 0, pos, scale, rotate, viewProjectionMat, color);

}

void Score::DrawPerfectNum(Vector2 pos, Vector2 scale, float rotate, Matrix4x4 viewProjectionMat, uint32_t color) {

	DrawParameter(perfectNum_, kComboMaxDigits_, 0, pos, scale, rotate, viewProjectionMat, color);

}

void Score::DrawGreatNum(Vector2 pos, Vector2 scale, float rotate, Matrix4x4 viewProjectionMat, uint32_t color) {

	DrawParameter(greatNum_, kComboMaxDigits_, 0, pos, scale, rotate, viewProjectionMat, color);
}

void Score::DrawGoodNum(Vector2 pos, Vector2 scale, float rotate, Matrix4x4 viewProjectionMat, uint32_t color) {

	DrawParameter(goodNum_, kComboMaxDigits_, 0, pos, scale, rotate, viewProjectionMat, color);
}

void Score::DrawMissNum(Vector2 pos, Vector2 scale, float rotate, Matrix4x4 viewProjectionMat, uint32_t color) {

	DrawParameter(missNum_, kComboMaxDigits_, 0, pos, scale, rotate, viewProjectionMat, color);
}

void Score::DrawParameter(int parameter, int digits, bool look, Vector2 pos, Vector2 scale, float rotate, Matrix4x4 viewProjectionMat, uint32_t color) {


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

		Vector2 p = { pos.x + kNumberTextureWidth_ * i * scale.x,pos.y };

		if (rotate != 0) {
			Vector2 a = p - pos;
			p.x = a.x * std::cosf(rotate) - a.y * std::sinf(rotate) + pos.x;
			p.y = a.y * std::cosf(rotate) + a.x * std::sinf(rotate) + pos.y;
		}

		numberTextures_[num]->Draw(p, scale, rotate, viewProjectionMat, color);
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