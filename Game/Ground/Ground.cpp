#include "Ground.h"
#include "externals/imgui/imgui.h"

Ground::Ground()
{
	ground = std::make_unique<Model>();
}

void Ground::Initialize()
{
	transform.scale_ = Vector3(1.0f, 1.0f, 1.0f);
}

void Ground::ModelLoad()
{
	ground->Texture("Resources/ground/ground.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
}

void Ground::Draw(const Matrix4x4& viewProjection)
{
	Model::ModelDraw(transform, viewProjection, 0xffffffff, ground.get());
}
