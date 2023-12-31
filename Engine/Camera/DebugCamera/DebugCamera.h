#pragma once
#include "math/Vector3.h"
#include "math/Matrix4x4.h"

/// <summary>
/// デバッグカメラ
/// </summary>
class DebugCamera
{
public:
	DebugCamera() = default;
	~DebugCamera() = default;

	//	初期化処理
	void Initialize();
	//	更新処理
	void Update();

private:
	//	X,Y,Z周りのローカル回転角
	Vector3 rotation = { 0.0f,0.0f,0.0f };
	//	ローカル座標
	Vector3 translation = { 0.0f,0.0f,-50.0f };
	//	ビュー行列
	Matrix4x4 viewMatrix;
	//	射影行列
	Matrix4x4 orthographicMatrix;



};

