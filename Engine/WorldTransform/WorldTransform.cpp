#include "WorldTransform.h"

decltype(WorldTransform::monocuro) WorldTransform::monocuro;

WorldTransform::WorldTransform()
{
	*cMat = MakeIdentity4x4();
	*cColor = { 1.0f,1.0f,1.0f,1.0f };
	cMono->pibot = { 0.0f,0.0f };
	cMono->rate = 1.0f;
}

WorldTransform& WorldTransform::operator=(const WorldTransform& trans)
{
	this->translation_ = trans.translation_;
	this->rotation_ = trans.rotation_;
	this->scale_ = trans.rotation_;
	this->worldMatrix = trans.worldMatrix;
	this->parent_ = trans.parent_;
	return *this;
}

Matrix4x4 WorldTransform::UpdateMatrix()
{
	//	monocuroを動かせばcMonoに代入される
	this->cMono->pibot = monocuro.pibot;

	//	スケール、回転、平行移動を合成して行列を計算する
	this->worldMatrix = MakeAffineMatrix(scale_, rotation_, translation_);

	//	親があれば親のワールド行列を掛ける
	if (parent_)
	{
		this->worldMatrix = this->worldMatrix * parent_->worldMatrix;
	}

	return this->worldMatrix;
}
