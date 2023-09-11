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

	enum Parameter {
		kScore,
		kMemoHighScore,
		kCombo,
		kHighCombo,
		kMemoHighCombo,
		kPerfectNum,
		kGreatNum,
		kGoodNum,
		kMissNum,
	};

	static const int kParameterNum_ = 9;

public:

	Score();

	void Initialize();

	void SetNumberTexture(std::vector<std::shared_ptr<Texture2D>> numberTextures) { numberTextures_ = numberTextures; }

	// posは一番左の数字の真ん中の座標。
	void SetWorldTransform(const Vector2& screenPos, float scale, float rotate, int parameter);

	void SetColor(uint32_t color, int parameter) { colors_[parameter] = color; }

	void SetIsDraw(bool is, int parameter) { isDraw_[parameter] = is; }

	void DrawParameter(const Matrix4x4& viewProjectionMat, int parameter);

	void Draw2D(const Matrix4x4& viewProjectionMat);

public:

	void Memo();

	void ResetIsDraw();

	std::optional<Evaluation> GetEvaluation() { return evalutuin_; }

	void ResetEvalution() { evalutuin_ = std::nullopt; }

	// perfectの加算。スコア、コンボも加算する。
	void AddPerfect() {
		evalutuin_ = Evaluation::kPerfect;
		parameters_[kPerfectNum]++;
		AddCombo();
		AddScore(200 * (parameters_[kCombo] / 10 + 1));
	}

	// greatの加算。スコア、コンボも加算する。
	void AddGreat() {
		evalutuin_ = Evaluation::kGreat;
		parameters_[kGreatNum]++;
		AddCombo();
		AddScore(100 * (parameters_[kCombo] / 10 + 1));
	}

	// goodの加算。スコアの加算、コンボを途切れさせる。
	void AddGood() {
		evalutuin_ = Evaluation::kGood;
		parameters_[kGoodNum]++;
		ComboReset();
		AddScore(50);
	}

	// missの加算。コンボを途切れさせる。
	void AddMiss() {
		evalutuin_ = Evaluation::kMiss;
		parameters_[kMissNum]++;
		ComboReset();
	}

	int GetScore() { return parameters_[kScore]; }

	int GetCombo() { return parameters_[kCombo]; }

	int GetHighCombo() { return parameters_[kHighCombo]; }

	int GetPerfect() { return parameters_[kPerfectNum]; }

	int GetGreat() { return parameters_[kGreatNum]; }

	int GetGood() { return parameters_[kGoodNum]; }

	int GetMiss() { return parameters_[kMissNum]; }

	int GetHighScore() { return parameters_[kMemoHighScore]; }

	int GetMemoHighCombo() { return parameters_[kMemoHighCombo]; }

	bool IsFullCom() { return isFullCom_; }

private:

	// goodとmissが0ならフルコンになる。プレイが終わってからの更新でよさそう。
	void IsFullComUpdate();

	// すべての数値を0,falseにする
	void Reset();

	// scoreの加算
	void AddScore(int score) { parameters_[kScore] += score; }

	// comboの加算。hiComboとの比較と更新。
	void AddCombo();

	void ComboReset() { parameters_[kCombo] = 0; }

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

	int parameters_[kParameterNum_] = {};

	uint32_t colors_[kParameterNum_] = {};

	bool isDraw_[kParameterNum_] = {};

	bool isFullCom_ = false;

	std::vector<std::shared_ptr<Texture2D>> numberTextures_;

	std::vector<std::shared_ptr<WorldTransform>> worldTransforms_[kParameterNum_];


};
