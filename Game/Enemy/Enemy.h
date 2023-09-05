#pragma once
#include "math/Matrix4x4.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"

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
	void Draw(const Matrix4x4& viewProjection);

public: // Korone

	BottomTypeClass GetBottomType() { return bottomType_; }

	int GetNum() { return num_; }

	void Die() { isDead_ = true; }

	bool IsDead() { return isDead_; }

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
	BottomTypeClass bottomType_ = BottomTypeClass::kA;

	int num_ = 0;

	bool isDead_ = false;

};
