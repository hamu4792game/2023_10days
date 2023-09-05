#include "Player.h"
#include "Engine/Input/KeyInput/KeyInput.h"

Player::Player(std::shared_ptr<Camera> camera)
{
	camera_ = camera;
	for (uint16_t i = 0u; i < PARTS::Num; i++) {
		models_.push_back(std::make_unique<Model>());
	}
}

void Player::Initialize()
{
	parts_.resize(models_.size());
	for (auto& i : parts_) {
		i.parent_ = &parts_[Body];
	}
	parts_[Body].parent_ = &transform;
}

void Player::ModelLoad()
{
	//models_[Body]->Texture("Resources/player/body.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
}

void Player::Update()
{
	transform.UpdateMatrix();
	for (auto& i : parts_) {
		i.UpdateMatrix();
	}
}

void Player::Draw(const Matrix4x4& viewProjection)
{
	for (uint16_t i = 0u; i < parts_.size(); i++)
	{
		Model::ModelDraw(parts_[i], viewProjection, 0xffffffff, models_[i].get());
	}
}

void Player::Move()
{
	if (KeyInput::GetKey(DIK_W)) {
		transform.translation_.z += 0.1f;
	}
	if (KeyInput::GetKey(DIK_W)) {
		transform.translation_.z += 0.1f;
	}



}

void Player::CameraUpdate()
{

}
