#include"VertexBuffer.h"
#include "DirectX12.h"
#include "d3dx12.h"

// コンストラクタ
VertexBuffer::VertexBuffer() :
	m_buffer(nullptr),
	m_bufferSize(0) {}

// 生成
void VertexBuffer::Create(void* pData, size_t size, UINT stride)
{
	m_bufferSize = size;

	// ヒーププロパティー
	auto Properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	auto ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(size);

	// リソース設定
	HRESULT Result = DIRECTX().GetDevice()->CreateCommittedResource(
		&Properties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_buffer.GetAddressOf()));

	// 頂点バッファリソースの生成に失敗
	if (FAILED(Result))
	{
		assert(SUCCEEDED(Result) && "頂点バッファリソースの生成に失敗しました");
	}

	// 頂点バッファビューの設定
	m_bufferView.BufferLocation = m_buffer->GetGPUVirtualAddress();
	m_bufferView.SizeInBytes    = static_cast<UINT>(size);
	m_bufferView.StrideInBytes  = static_cast<UINT>(stride);

	// マッピング
	if (pData == nullptr) { return; }

	void* ptr = nullptr;
	Result = m_buffer->Map(0, nullptr, (void**)&ptr);

	// 頂点バッファマッピングに失敗しました
	if (FAILED(Result))
	{
		assert(SUCCEEDED(Result) && "頂点バッファマッピングに失敗しました");
	}

	// 頂点データをマッピング先に設定
	CopyMemory(ptr, pData, size);

	// マッピング解除
	m_buffer->Unmap(0, nullptr);
}

// 更新
void VertexBuffer::Update(void* pData)
{
	// マッピング
	if (pData == nullptr) { return; }

	void* ptr = nullptr;
	HRESULT Result = m_buffer->Map(0, nullptr, (void**)&ptr);

	// 頂点バッファマッピングに失敗しました
	if (FAILED(Result))
	{
		assert(SUCCEEDED(Result) && "頂点バッファマッピングに失敗しました");
	}

	// 頂点データをマッピング先に設定
	CopyMemory(ptr, pData, m_bufferSize);

	// マッピング解除
	m_buffer->Unmap(0, nullptr);
}

