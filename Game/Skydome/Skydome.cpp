#include "Skydome.h"

Skydome::Skydome()
{
	skydome = std::make_unique<Model>();
}

void Skydome::Initialize()
{
	transform.scale_ = Vector3(400.0f, 400.0f, 400.0f);
	transform.translation_ = Vector3(0.0f, 0.0f, 300.0f);
}

void Skydome::ModelLoad()
{
	skydome->Texture("Resources/skydome/skydome.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "skydome/skydome.png");
}

void Skydome::Draw(const Matrix4x4& viewProjection)
{
	transform.UpdateMatrix();
	Model::ModelDraw(transform, viewProjection, 0xffffffff, skydome.get());
}
