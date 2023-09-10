#include "math/Matrix4x4.h"
#include "Engine/Texture/Model.h"
#include "Engine/WorldTransform/WorldTransform.h"

class Skydome
{
public:
	Skydome();
	~Skydome() = default;

	void Initialize(const WorldTransform& world);
	void Initialize();

	void ModelLoad();

	void Draw(const Matrix4x4& viewProjection);

private:

	std::unique_ptr<Model> skydome;
	WorldTransform transform;
};
