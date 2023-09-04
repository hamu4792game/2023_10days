#pragma once
#include "math/Matrix4x4.h"

class Battle
{
public:
	Battle() = default;
	~Battle() = default;

	void Initialize();

	void ModelLoad();

	void Update();

	void Draw(Matrix4x4 viewProjection);

private:

};
