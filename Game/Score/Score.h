#pragma once


#include <optional>

class Score {

public:

	enum class Evaluation {
		kPerfect,
		kGreat,
		kGood,
		kMiss
	};

	// すべての数値を0,falseにする
	void Reset();

	// score,hiCombo,isFullComboを比較して更新する
	void SetUpdate(const Score& score);

	std::optional<Evaluation> GetEvaluation() { return evalutuin_; }

	void ResetEvalution() { evalutuin_ = std::nullopt; }

	// scoreの加算
	void AddScore(int score) { score_ += score; }

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

	// comboの加算。hiComboとの比較と更新。
	void AddCom();

	void ComboReset() { combo_ = 0; }

	// goodとmissが0ならフルコンになる。プレイが終わってからの更新でよさそう。
	void IsFullComUpdate();

	int GetScore() { return score_; }

	int GetCom() { return combo_; }

	int GetHiCombo() { return hiCombo_; }

	int GetPer() { return perfectNum_; }

	int GetGre() { return greatNum_; }

	int GetGood() { return goodNum_; }

	int GetMiss() { return missNum_; }

	bool IsFullCom() { return isFullCom_; }

private:

	std::optional<Evaluation> evalutuin_ = std::nullopt;

	int score_ = 0;
	int combo_ = 0;
	int perfectNum_ = 0;
	int greatNum_ = 0;
	int goodNum_ = 0;
	int missNum_ = 0;

	int hiCombo_ = 0;
	bool isFullCom_ = false;

};
