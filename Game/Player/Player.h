#pragma once
#include "math/Matrix4x4.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"
#include "Engine/Camera/Camera.h"
#include"EEnum.h"


class Score;

class Enemy;

class Player
{
public:
	Player(std::shared_ptr<Camera> camera = nullptr);
	~Player() = default;

	//モデルデータ配列,パーツWのデータ配列、親の処理
	void Initialize(std::vector<std::shared_ptr<Model>> models,WorldTransform* world);

	void ModelLoad();

	//	更新処理
	void Update();

	//	描画
	void Draw(const Matrix4x4& viewProjection);

public: // Korone

	void HitTestInitialize();

	//void SetScore(Score* score) { score = score; }

	void HitTest(Enemy* enemy, Score* score);

private: // Korone

	//void HitTest(Enemy* enemy, Score* score);

	void HitEvalution(Enemy* enemy, Score* score);

private:

	
	//	世界の中心のptr
	WorldTransform* world_ = nullptr;
	
	//	元
	WorldTransform transform;

	//	モデルデータ配列
	std::vector<std::shared_ptr<Model>> models_;
	//	パーツ用データ
	std::vector<WorldTransform> parts_;
	//	カメラ共有ptr
	std::shared_ptr<Camera> camera_ = nullptr;
	//	カメラの初期位置
	Vector3 offset;

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

	//Score* score = nullptr;


};
