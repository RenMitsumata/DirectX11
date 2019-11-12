#define STB_IMAGE_IMPLEMENTATION
#include "main.h"
#include "renderer.h"
#include "stb-master/stb_image.h"
#include "texture.h"



static unsigned int texture;	// 2個以上管理したい場合は配列にする
static unsigned int count = 0;

void CTexture::Load(const char *FileName)
{
	
	unsigned char	header[18];
	unsigned char	*image;
	int	width, height;
	unsigned char	depth;
	int	bpp;
	unsigned int	size;

	image = stbi_load(FileName, &width, &height, &bpp, 0);
	/*
	unsigned char	header[18];
	unsigned char	*image;
	unsigned int	width, height;
	unsigned char	depth;
	unsigned int	bpp;
	unsigned int	size;
	
	// ヘッダ読み込み
	FILE* file;
	file = fopen(FileName, "rb");
	assert(file);

	fread(header, sizeof(header), 1, file);


	// 画像サイズ取得
	width = header[13] * 256 + header[12];
	height = header[15] * 256 + header[14];
	depth = header[16];

	if (depth == 32)
		bpp = 4;
	else if (depth == 24)
		bpp = 3;
	else
		bpp = 0;

	if ((bpp != 4) && (bpp != 3)) {
		assert(false);
	}

	size = width * height * bpp;
	int alloc = width * height * 4;

	// メモリ確保
	image = (unsigned char*)new unsigned char[alloc];

	// 画像読み込み
	fread(image, size, 1, file);

	// R<->B
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			unsigned char c;
			c = image[(y * width + x) * 4 + 0];
			image[(y * width + x) * 4 + 0] = image[(y * width + x) * 4 + 2];
			image[(y * width + x) * 4 + 2] = c;
			if (bpp == 3) {
				image[(y * width + x) * 4 + 3] = 255;
			}
		}
	}

	fclose(file);
	*/
	size = width * height * bpp;

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = image;
	initData.SysMemPitch = width * 4;
	initData.SysMemSlicePitch = size;

	auto hr = CRenderer::GetDevice()->CreateTexture2D(&desc, &initData, &m_Texture);
	if (FAILED(hr)) {
		assert(false);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	hr = CRenderer::GetDevice()->CreateShaderResourceView(m_Texture, &SRVDesc, &m_ShaderResourceView);
	if (FAILED(hr))
	{
		assert(false);
	}

	stbi_image_free(image);
	//delete image;
}


void CTexture::Unload()
{
	m_Texture->Release();
	m_ShaderResourceView->Release();
}



