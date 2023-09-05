#include "Enemy.h"

void Enemy::Initialize()
{
	parts_.resize(models_.size());
	for (auto& i : parts_) {
		i.parent_ = &parts_[Body];
	}
	parts_[Body].parent_ = &transform;
}

void Enemy::ModelLoad()
{
	//models_[Body]->Texture("Resources/player/body.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
}

void Enemy::Update()
{
	transform.UpdateMatrix();
	for (auto& i : parts_) {
		i.UpdateMatrix();
	}
}

void Enemy::Draw(const Matrix4x4& viewProjection)
{
	for (uint16_t i = 0u; i < parts_.size(); i++)
	{
		Model::ModelDraw(parts_[i], viewProjection, 0xffffffff, models_[i].get());
	}
}
