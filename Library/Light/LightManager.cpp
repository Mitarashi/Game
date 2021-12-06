#include <d3d11.h>

#include "LightManager.h"

void LightManager::Clear()
{
	//for (auto& spotLight : spotLights)
	//{
	//	auto it = std::find(spotLights.begin(), spotLights.end(), spotLight);
	//	if (it != spotLights.end()) spotLights.erase(it);
	//}

	//for (auto& pointLight : pointLights)
	//{
	//	auto it = std::find(pointLights.begin(), pointLights.end(), pointLight);
	//	if (it != pointLights.end()) pointLights.erase(it);
	//}
}

void LightManager::SetDirLight(Vec3 dir, Vec3 color)
{
	float d = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);

	//if (d > 0) { dir.x /= d; dir.y /= d; dir.z /= d; }
	if (d > 0) { dir /= d; }

	lightDir = Vec4(dir.x, dir.y, dir.z, 0);
	dirLightColor = Vec4(color.x, color.y, color.z, 1);
}

void LightManager::SetAmbient(Vec3 amb)
{
	ambient = { amb.x,amb.y,amb.z , 0 };
}

void LightManager::SetPointLight(int index, Vec3 pos, Vec3 color, float range)
{
	if (index < 0) return;

	std::unique_ptr<PointLight> pointLight = std::make_unique<PointLight>();
	pointLight->SetIndex(index);
	pointLight->SetRange(range);
	pointLight->SetType(1.0f);
	pointLight->SetPosition(pos);
	pointLight->SetColor(color);

	pointLights.emplace_back(std::move(pointLight));
}

void LightManager::SetSpotLight(int index, Vec3 pos, Vec3 color, Vec3 dir,
	float range, float innerCorn, float outerCorn)
{
	if (index < 0) return;
	float d = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
	if (d > 0) dir /= d;

	std::unique_ptr<SpotLight> spotLight = std::make_unique<SpotLight>();
	spotLight->SetIndex(index);
	spotLight->SetRange(range);
	spotLight->SetType(1.0f);
	spotLight->SetInnerCorn(innerCorn);
	spotLight->SetOuterCorn(outerCorn);
	spotLight->SetPosition(pos);
	spotLight->SetColor(color);
	spotLight->SetDir(dir);

	spotLights.emplace_back(std::move(spotLight));
}