#include <imgui.h>

#include "SceneGame.h"
#include "SceneTest.h"
#include "SceneTitle.h"
#include "SceneLoading.h"
#include "SceneManager.h"

#include "Scene.h"

void Scene::OnGui()
{
	ImGui::SetNextWindowPos(ImVec2(900, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin(sceneName.c_str(), nullptr, ImGuiWindowFlags_None))
	{
		ImGui::Combo("nextScene", &nextScene, "Title\0Game\0Test");
		ImGui::Checkbox("isLoadScene", &isLoadScene);
		if (ImGui::Button("ChangeScene"))
		{
			switch (nextScene)
			{
			case 0:
				if (!isLoadScene) SceneManager::Instance().ChangeScene(std::make_unique<SceneTitle>());
				else SceneManager::Instance().ChangeScene(std::make_unique<SceneLoading>(std::make_unique<SceneTitle>()));
				break;
			case 1:
				if (!isLoadScene) SceneManager::Instance().ChangeScene(std::make_unique<SceneGame>());
				else SceneManager::Instance().ChangeScene(std::make_unique<SceneLoading>(std::make_unique<SceneGame>()));
				break;
			case 2:
				if (!isLoadScene) SceneManager::Instance().ChangeScene(std::make_unique<SceneTest>());
				else SceneManager::Instance().ChangeScene(std::make_unique<SceneLoading>(std::make_unique<SceneTest>()));
				break;
			}
		}
	}
	ImGui::End();
}