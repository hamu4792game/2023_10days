#include "Battle.h"
#include "externals/imgui/imgui.h"
#include "math/Vector4.h"

Battle::Battle(std::shared_ptr<Camera> camera)
{
	camera_ = camera;
	player_ = std::make_unique<Player>(camera_);
}

void Battle::Initialize()
{
	player_->Initialize(transform);
}

void Battle::ModelLoad()
{
	player_->ModelLoad();
}

void Battle::Update()
{
	transform.rotation_.x += AngleToRadian(1.0f);
	//ImGui::DragFloat("worldRo", &transform.rotation_.x, AngleToRadian(1.0f));
	transform.worldMatrix = MakeRotateMatrix(transform.rotation_);
	player_->Update();
}

void Battle::Draw(const Matrix4x4& viewProjection)
{
	player_->Draw(viewProjection);
}
