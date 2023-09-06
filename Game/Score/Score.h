#pragma once

#include <optional>
#include "Engine/Texture/Texture2D.h"

class Score {

public:

	enum class Evaluation {
		kPerfect,
		kGreat,
		kGood,
		kMiss
	};

public:

	void Initialize(std::vector<std::shared_ptr<Texture2D>> numberTextures);

	// posは一番左の数字の真ん中の座標。たぶん。
	void DrawScore(WorldTransform& worldTransform, Matrix4x4 viewProjectionMat, uint32_t color);

	// posは一番左の数字の真ん中の座標。たぶん。
	void DrawCombo(WorldTransform& worldTransform, Matrix4x4 viewProjectionMat, uint32_t color);

	// posは一番左の数字の真ん中の座標。たぶん。
	void DrawHighCombo(WorldTransform& worldTransform, Matrix4x4 viewProjectionMat, uint32_t color);

	// posは一番左の数字の真ん中の座標。たぶん。
	void DrawPerfectNum(WorldTransform& worldTransform, Matrix4x4 viewProjectionMat, uint32_t color);

	// posは一番左の数字の真ん中の座標。たぶん。
	void DrawGreatNum(WorldTransform& worldTransform, Matrix4x4 viewProjectionMat, uint32_t color);

	// posは一番左の数字の真ん中の座標。たぶん。
	void DrawGoodNum(WorldTransform& worldTransform, Matrix4x4 viewProjectionMat, uint32_t color);

	// posは一番左の数字の真ん中の座標。たぶん。
	void DrawMissNum(WorldTransform& worldTransform, Matrix4x4 viewProjectionMat, uint32_t color);

public:

	// すべての数値を0,falseにする
	void Reset();

	// score,hiCombo,isFullComboを比較して更新する
	void SetUpdate(const Score& score);

	std::optional<Evaluation> GetEvaluation() { return evalutuin_; }

	void ResetEvalution() { evalutuin_ = std::nullopt; }

	// perfectの加算。スコア、コンボも加算する。
	void AddPer() {
		evalutuin_ = Evaluation::kPerfect;
		perfectNum_++;
		AddCom();
		AddScore(200 * (combo_ / 10 + 1));
	}

	// greatの加算。スコア、コンボも加算する。
	void AddGre() {
		evalutuin_ = Evaluation::kGreat;
		greatNum_++;
		AddCom();
		AddScore(100 * (combo_ / 10 + 1));
	}

	// goodの加算。スコアの加算、コンボを途切れさせる。
	void AddGood() {
		evalutuin_ = Evaluation::kGood;
		goodNum_++;
		ComboReset();
		AddScore(50);
	}

	// missの加算。コンボを途切れさせる。
	void AddMiss() {
		evalutuin_ = Evaluation::kMiss;
		missNum_++;
		ComboReset();
	}

	// goodとmissが0ならフルコンになる。プレイが終わってからの更新でよさそう。
	void IsFullComUpdate();

	int GetScore() { return score_; }

	int GetCombo() { return combo_; }

	int GetHighCombo() { return highCombo_; }

	int GetPerfect() { return perfectNum_; }

	int GetGreat() { return greatNum_; }

	int GetGood() { return goodNum_; }

	int GetMiss() { return missNum_; }

	bool IsFullCom() { return isFullCom_; }

private:

	// scoreの加算
	void AddScore(int score) { score_ += score; }

	// comboの加算。hiComboとの比較と更新。
	void AddCom();

	void ComboReset() { combo_ = 0; }


	void DrawParameter(int parameter, int digits, bool look, WorldTransform& worldTransform, Matrix4x4 viewProjectionMat, uint32_t color);

private:
	// 最大桁数
	static const int kScoreMaxDigits_ = 6;
	static const int kComboMaxDigits_ = 3;
	static const int kEvaluationMaxDigits_ = 3;

	// textureのサイズ
	static const int kNumberTextureWidth_ = 53;
	static const int kNumberTextureHeight_ = 61;

private:

	std::optional<Evaluation> evalutuin_ = std::nullopt;

	int score_ = 0;
	int combo_ = 0;
	int perfectNum_ = 0;
	int greatNum_ = 0;
	int goodNum_ = 0;
	int missNum_ = 0;

	int highCombo_ = 0;
	bool isFullCom_ = false;


	std::vector<std::shared_ptr<Texture2D>> numberTextures_;

	WorldTransform scoreWorldTransform_[kScoreMaxDigits_];
	WorldTransform combWorldTransform_[kComboMaxDigits_];
	WorldTransform highCombWorldTransform_[kComboMaxDigits_];
	WorldTransform perfectNumWorldTransform_[kComboMaxDigits_];
	WorldTransform greatNumWorldTransform_[kComboMaxDigits_];
	WorldTransform goodNumWorldTransform_[kComboMaxDigits_];
	WorldTransform missNumWorldTransform_[kComboMaxDigits_];

};
