#include "Ground.h"
#include "externals/imgui/imgui.h"

Ground::Ground()
{
	ground = std::make_unique<Model>();
}

void Ground::Initialize()
{
	transform.scale_ = Vector3(200.0f, 200.0f, 200.0f);
	transform.rotation_ = Vector3(0.0f, AngleToRadian(90.0f), 0.0f);
}

void Ground::ModelLoad()
{
	ground->Texture("Resources/ground/ground.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
}

void Ground::Draw(const Matrix4x4& viewProjection)
{
	//ImGui::DragFloat3("cameraTrans", &transform.scale_.x, 0.1f);
	transform.UpdateMatrix();
	Model::ModelDraw(transform, viewProjection, 0x000000ff, ground.get());
}
