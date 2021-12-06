#pragma once

#include <memory>
#include <vector>

#include "SpotLight.h"
#include "PointLight.h"

// ライトマネージャー
class LightManager
{
public:
	// コンストラクタ
	LightManager() = default;

	// デストラクタ
	~LightManager() = default;

	static LightManager& Instance() { return *instance; }

	void Clear();

	void SetDirLight(Vec3 dir, Vec3 color);

	void SetAmbient(Vec3 amb);

	void SetPointLight(int index, Vec3 pos, Vec3 color, float range);

	void SetSpotLight(int index, Vec3 pos, Vec3 color, Vec3 dir, float range, float inner, float outer);

public:
	//static const int POINTMAX = 32;
	//static const int SPOTMAX = 32;

public:
	Vec4 lightDir;
	Vec4 dirLightColor;
	Vec4 ambient;

private:
	inline static LightManager* instance = nullptr;

	std::vector<std::unique_ptr<PointLight>> pointLights;
	std::vector<std::unique_ptr<SpotLight>> spotLights;
};