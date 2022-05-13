#pragma once
#include<d3d12.h>
#include<wrl.h>
#include <vector>
#include "VertexBuffer.h"

// インデックスバッファクラス
class IndexBuffer
{
private:
	// バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>m_pBuffer;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW  m_bufferView;
public:
	// コンストラクタ
	IndexBuffer();
	// デストラクタ
	~IndexBuffer() = default;
public:
	// 生成
	void Create(void* pData, size_t size, DXGI_FORMAT format = DXGI_FORMAT_R32_UINT);
public:
	// バッファビューの取得
	D3D12_INDEX_BUFFER_VIEW  GetBufferView() const { return m_bufferView; }
};