#include "Ground.h"
#include "externals/imgui/imgui.h"

Ground::Ground()
{
	ground = std::make_unique<Model>();
}

void Ground::Initialize()
{
	transform.scale_ = Vector3(50.0f, 1.0f, 300.0f);
	transform.translation_ = Vector3(0.0f, 0.0f, 250.0f);
	transform.rotation_ = Vector3(0.0f, AngleToRadian(0.0f), 0.0f);
}

void Ground::ModelLoad()
{
	ground->Texture("Resources/plane/plane.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "ground/ground.png");
}

void Ground::Draw(const Matrix4x4& viewProjection)
{
	//ImGui::DragFloat3("cameraTrans", &transform.scale_.x, 0.1f);
	transform.UpdateMatrix();
	Model::ModelDraw(transform, viewProjection, 0xffffffff, ground.get());
}
