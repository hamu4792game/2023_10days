#pragma once
#include "math/Matrix4x4.h"
#include "Engine/Camera/Camera.h"
#include "Engine/Texture/Model.h"

class Battle
{
public:
	Battle(std::shared_ptr<Camera> camera = nullptr);
	~Battle() = default;

	//	初期化
	void Initialize();

	//	モデルのロード
	void ModelLoad();

	//	更新処理
	void Update();

	//	3D描画処理
	void Draw(const Matrix4x4& viewProjection);

private://	必要なメンバ変数

	std::shared_ptr<Camera> camera_;

	//std::unique_ptr<Model> road;
	//WorldTransform roadPos;
	//WorldTransform wallPos;
	//WorldTransform wallPos2;

};
