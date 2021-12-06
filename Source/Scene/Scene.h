#pragma once

#include <string>

// シーン
class Scene
{
public:
	// コンストラクタ
	Scene() = default;

	// デストラクタ
	virtual ~Scene() = default;

	// 初期化
	virtual void Initialize() = 0;

	// 終了化
	virtual void Finalize() = 0;

	// 更新処理
	virtual void Update() = 0;

	// 描画処理
	virtual void Render() = 0;

	// ImGui描画
	virtual void OnGui();

	// 初期化終了済みかどうか
	bool IsReady() const { return ready; }

	// 初期化完了設定
	void SetReady() { ready = true; }

private:
	bool ready = false;
	bool isLoadScene = false;

protected:
	std::string sceneName = {};
	int nextScene = 0;
};