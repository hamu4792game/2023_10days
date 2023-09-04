#pragma once
#include "math/Matrix4x4.h"

class Title
{
public:
	Title() = default;
	~Title() = default;

	void Initialize();

	void ModelLoad();

	void Update();

	void Draw(Matrix4x4 viewProjection);

private:

};
