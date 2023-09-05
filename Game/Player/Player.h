#pragma once
#include "math/Matrix4x4.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"
#include "Engine/Camera/Camera.h"
#include"EEnum.h"


class Player
{
public:
	Player(std::shared_ptr<Camera> camera = nullptr);
	~Player() = default;

	void Initialize(std::vector<std::unique_ptr<Model>> models,const WorldTransform& world);

	void ModelLoad();

	//	更新処理
	void Update();

	//	描画
	void Draw(const Matrix4x4& viewProjection);

private:

	
	
	//	元
	WorldTransform transform;

	//	モデルデータ配列
	std::vector<std::unique_ptr<Model>> models_;
	//	パーツ用データ
	std::vector<WorldTransform> parts_;

	std::shared_ptr<Camera> camera_ = nullptr;

};
