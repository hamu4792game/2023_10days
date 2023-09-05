#include "Battle.h"
#include "externals/imgui/imgui.h"
#include "math/Vector4.h"

Battle::Battle(std::shared_ptr<Camera> camera)
{
	camera_ = camera;
	player_ = std::make_unique<Player>();
}

void Battle::Initialize()
{
	player_->Initialize();
}

void Battle::ModelLoad()
{
	player_->ModelLoad();
}

void Battle::Update()
{
	player_->Update();
}

void Battle::Draw(const Matrix4x4& viewProjection)
{
	player_->Draw(viewProjection);
}
