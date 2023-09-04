#pragma once
#include "math/Matrix4x4.h"

class Result
{
public:
	Result() = default;
	~Result() = default;

	void Initialize();

	void ModelLoad();

	void Update();

	void Draw(Matrix4x4 viewProjection);

private:

};
