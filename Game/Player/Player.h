#pragma once
#include "math/Matrix4x4.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"

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

private:

	enum PARTS
	{
		Body,
		BodyUnder,
		Head,

		LArm1,
		LArm2,
		LHand,

		RArm1,
		RArm2,
		RHand,

		LLeg1,
		LLeg2,
		LFoot,

		RLeg1,
		RLeg2,
		RFoot,
	};
	//	元
	WorldTransform transform;

	//	モデルデータ配列
	std::vector<std::unique_ptr<Model>> models_;
	//	パーツ用データ
	std::vector<WorldTransform> parts_;

};
