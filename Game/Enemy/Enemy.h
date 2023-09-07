#pragma once
#include "math/Matrix4x4.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"
#include"EEnum.h"

class Enemy
{
public:

	enum class BottomTypeClass {
		kA,
		kB,
		kX,
		kY
	};

	enum BottomType {
		kA,
		kB,
		kX,
		kY
	};

public:
	Enemy() = default;
	~Enemy() = default;

	void Initialize();

	void ModelLoad();

	//	更新処理
	void Update();

	//	描画
	void Draw(const Matrix4x4& viewProjection, std::vector<std::shared_ptr<Model>> botunModels);

public: // Korone

	void Initialize(int type, int num);

	//修正版
	void InitializeSP(int type, int num,
		std::vector<std::shared_ptr<Model>> models);

	int type_;

	BottomTypeClass GetBottomType() { return bottomType_; }

	int GetNum() { return num_; }

	void Die() { isDead_ = true; }

	bool IsDead() { return isDead_; }

private:

	
	//	元
	WorldTransform transform;

	//	モデルデータ配列
	std::vector<std::shared_ptr<Model>> models_;
	//	パーツ用データ
	std::vector<WorldTransform> parts_;


	//ボタンのワールド
	WorldTransform BottonW_;


private: // Korone
	BottomTypeClass bottomType_ = BottomTypeClass::kA;

	int num_ = 0;

	bool isDead_ = false;



};
