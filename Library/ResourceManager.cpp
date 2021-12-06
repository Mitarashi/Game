#include "../Library/Misc.h"
#include "../Library/DxSystem.h"

#include "ResourceManager.h"

// モデルリソース読み込み
std::shared_ptr<ModelResource> ResourceManager::LoadModelResource(const char* fileName)
{
	// モデルを検索
	std::map<std::string, std::weak_ptr<ModelResource>>::iterator it = models.find(fileName);
	if (it != models.end())
	{
		// リンク（寿命）が切れていないか確認
		if (!it->second.expired())
		{
			// 読み込み済みのモデルリソースを返す
			return it->second.lock();
		}
	}

	// 新規モデルリソース作成＆読み込み
	auto model = std::make_shared<ModelResource>();
	model->Load(fileName);

	// マップに登録
	models[fileName] = model;
	return model;
}

// テクスチャ読み込み
HRESULT ResourceManager::LoadTextureFromFile(
	const char*					fileName,
	ID3D11ShaderResourceView**	srv,
	D3D11_TEXTURE2D_DESC*		textureDesc)
{
	ID3D11Device* device = DxSystem::Instance().GetDevice();

	HRESULT hr = S_OK;

	Microsoft::WRL::ComPtr<ID3D11Resource> resource;

	// マルチバイト文字からワイド文字へ変換
	wchar_t wfileName[256];
	::MultiByteToWideChar(CP_ACP, 0, fileName, -1, wfileName, 256);

	// テクスチャ読み込み処理
	auto it = textures.find(wfileName);
	if (it != textures.end())
	{
		*srv = it->second.Get();
		(*srv)->AddRef();
		(*srv)->GetResource(resource.GetAddressOf());
	}
	else
	{
		hr = DirectX::CreateWICTextureFromFile(
			device,
			wfileName,
			resource.GetAddressOf(),
			srv
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		textures.insert(std::make_pair(wfileName, *srv));
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
	hr = resource.Get()->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	texture2d->GetDesc(textureDesc);

	return hr;
}

// ダミーテクスチャ生成
HRESULT ResourceManager::CreateDummyTexture(
	ID3D11ShaderResourceView**	srv,
	D3D11_TEXTURE2D_DESC*		desc)
{
	ID3D11Device* device = DxSystem::Instance().GetDevice();

	HRESULT hr = S_OK;

	const int width = 8;
	const int height = 8;
	UINT pixels[width * height];
	::memset(pixels, 0xFF, sizeof(pixels));

	// テクスチャ2Dデスク
	desc->Width					= width;
	desc->Height				= height;
	desc->MipLevels				= 1;
	desc->ArraySize				= 1;
	desc->Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	desc->SampleDesc.Count		= 1;
	desc->SampleDesc.Quality	= 0;
	desc->Usage					= D3D11_USAGE_DEFAULT;
	desc->BindFlags				= D3D11_BIND_SHADER_RESOURCE;
	desc->CPUAccessFlags		= 0;
	desc->MiscFlags				= 0;

	// リソースデータ
	D3D11_SUBRESOURCE_DATA data = { 0 };
	::memset(&data, 0, sizeof(data));
	data.pSysMem			= pixels;
	data.SysMemPitch		= width;

	// ダミーテクスチャ生成
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture = {};
	hr = device->CreateTexture2D(desc, &data, texture.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	hr = device->CreateShaderResourceView(texture.Get(), nullptr, srv);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	return hr;
}

// VertexShader読み込み
HRESULT ResourceManager::CreateVSFromCSO(
	const char*					csoName,
	ID3D11VertexShader**		vs,
	ID3D11InputLayout**			inputLayout,
	D3D11_INPUT_ELEMENT_DESC*	inputElementDesc,
	UINT						NumElements)
{
	ID3D11Device* device = DxSystem::Instance().GetDevice();

	HRESULT hr = S_OK;

	// 頂点シェーダー読み込み
	auto it = vertexShaders.find(csoName);
	if (it != vertexShaders.end())
	{
		*vs				= it->second.vertexShader.Get();
		(*vs)->AddRef();
		*inputLayout	= it->second.inputLayout.Get();
		(*inputLayout)->AddRef();
		return S_OK;
	}

	FILE* fp = nullptr;
	fopen_s(&fp, csoName, "rb");
	_ASSERT_EXPR(fp, "CSO File not found");

	fseek(fp, 0, SEEK_END);
	long csoSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	std::unique_ptr<unsigned char[]> csoData = std::make_unique<unsigned char[]>(csoSize);
	fread(csoData.get(), csoSize, 1, fp);
	fclose(fp);

	hr = device->CreateVertexShader(csoData.get(), csoSize, nullptr, vs);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	hr = device->CreateInputLayout(
		inputElementDesc,
		NumElements,
		csoData.get(),
		csoSize,
		inputLayout);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	vertexShaders.insert(std::make_pair(csoName, VSData(*vs, *inputLayout)));

	return hr;
}

// PixelShader読み込み
HRESULT ResourceManager::CreatePSFromCSO(
	const char*			csoName,
	ID3D11PixelShader** ps)
{
	ID3D11Device* device = DxSystem::Instance().GetDevice();

	HRESULT hr = S_OK;

	// ピクセルシェーダー読み込み
	auto it = pixelShaders.find(csoName);
	if (it != pixelShaders.end())
	{
		*ps = it->second.Get();
		(*ps)->AddRef();
		return S_OK;
	}

	FILE* fp = nullptr;
	fopen_s(&fp, csoName, "rb");
	_ASSERT_EXPR(fp, "CSO File not found");

	fseek(fp, 0, SEEK_END);
	long csoSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	std::unique_ptr<unsigned char[]> csoData = std::make_unique<unsigned char[]>(csoSize);
	fread(csoData.get(), csoSize, 1, fp);
	fclose(fp);

	hr = device->CreatePixelShader(csoData.get(), csoSize, nullptr, ps);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	pixelShaders.insert(std::make_pair(csoName, *ps));

	return hr;
}

// GeometryShader読み込み
HRESULT ResourceManager::CreateGSFromCSO(
	const char*				csoName,
	ID3D11GeometryShader**	gs)
{
	ID3D11Device* device = DxSystem::Instance().GetDevice();

	HRESULT hr = S_OK;

	// ジオメトリシェーダー読み込み
	auto it = geometryShaders.find(csoName);
	if (it != geometryShaders.end())
	{
		*gs = it->second.Get();
		(*gs)->AddRef();
		return S_OK;
	}

	FILE* fp = nullptr;
	fopen_s(&fp, csoName, "rd");
	_ASSERT_EXPR(fp, "CSO File not found");

	fseek(fp, 0, SEEK_END);
	long csoSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	std::unique_ptr<unsigned char[]> csoData = std::make_unique<unsigned char[]>(csoSize);
	fread(csoData.get(), csoSize, 1, fp);
	fclose(fp);

	hr = device->CreateGeometryShader(csoData.get(), csoSize, nullptr, gs);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	geometryShaders.insert(std::make_pair(csoName, *gs));

	return hr;
}
