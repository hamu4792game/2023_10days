#pragma once
#include "math/Matrix4x4.h"

#include "Game/UI/UI.h"

class Result
{
public:
	Result() = default;
	~Result() = default;

	void Initialize();

	void ModelLoad();

	void Update();

	void Draw(Matrix4x4 viewProjection);

public: // korone

	void SetUI(UI* ui) { ui_ = ui; }


private:

	UI* ui_ = nullptr;

};
