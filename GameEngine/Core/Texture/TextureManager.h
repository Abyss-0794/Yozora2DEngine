#pragma once

class GraphicsDevice;

class TextureManager
{
public:
	struct Texture
	{
		ComPtr<ID3D12Resource> textureBuffer = nullptr;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandle;
	};

	TextureManager(GraphicsDevice& graphicsDevice);

	bool LoadShader(const std::wstring& filePath);
	Texture GetTextureInfo(const std::wstring& filePath);

private:
	std::unordered_map<std::wstring, Texture> m_dynamicTextureMap;

	GraphicsDevice& m_graphicsDevice;
};