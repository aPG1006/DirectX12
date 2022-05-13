#pragma once
#include<d3d12.h>
#include<wrl.h>
#include<memory>
#include<vector>

// 頂点バッファクラス
class VertexBuffer
{
private:
	// バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>m_buffer;
	// バッファサイズ
	UINT m_bufferSize;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW m_bufferView;
public:
	// コンストラクタ
	VertexBuffer();
	// デストラクタ
	~VertexBuffer() = default;
	// 生成
	void Create(void* pData, size_t size, UINT stride);
	// 更新
	void Update(void* pData);

	// バッファビューの取得
	D3D12_VERTEX_BUFFER_VIEW GetBufferView() const { return m_bufferView; }
};