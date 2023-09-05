#pragma once
#include "math/Matrix4x4.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"

class Score;

class Enemy;

class Player
{
public:
	Player() = default;
	~Player() = default;

	void Initialize();

	void ModelLoad();

	//	更新処理
	void Update();

	//	描画
	void Draw(const Matrix4x4& viewProjection);

public: // Korone

	void HitTestInitialize();

	void SetScore(Score* score) { score_ = score; }

private: // Korone

	void HitTest(Enemy* enemy);

	void HitEvalution(Enemy* enemy);

private:

	enum PARTS
	{
		Body,
		Head,
		L_arm,
		R_arm,
		L_leg,
		R_leg,
		Weapon,

		Num
	};
	//	元
	WorldTransform transform;

	//	モデルデータ配列
	std::vector<std::unique_ptr<Model>> models_;
	//	パーツ用データ
	std::vector<WorldTransform> parts_;

private: // Korone

	enum Evalution {
		kPerfect,
		kGreat,
		kGood,
		kMiss
	};

	//Input* input_ = nullptr;

	// 一番遅いフレーム。これを基準にコンボ数によって早くする。
	const int kIntervalFrame_ = 30;

	int intervalCount_ = 0;

	int kEvalutionframe_[4] = { 20, 60, 120, 121 };

	int evalutionCount_ = 0;

	Score* score_ = nullptr;
};
