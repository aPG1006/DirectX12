#include "IndexBuffer.h"
#include "DirectX12.h"
#include "d3dx12.h"

// コンストラクタ
IndexBuffer::IndexBuffer() :
	m_pBuffer(nullptr) {}

// 生成
void IndexBuffer::Create(void* data, size_t size, DXGI_FORMAT format)
{
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
		IID_PPV_ARGS(m_pBuffer.GetAddressOf()));

	// インデックスバッファリソースの生成に失敗
	if (FAILED(Result))
	{
		assert(SUCCEEDED(Result) && "インデックスバッファリソースの生成に失敗しました");
	}

	// インデックスバッファビューの設定
	m_bufferView.BufferLocation = m_pBuffer->GetGPUVirtualAddress();
	m_bufferView.Format = format;
	m_bufferView.SizeInBytes = static_cast<UINT>(size);

	// マッピング
	if (data != nullptr)
	{
		void* ptr = nullptr;
		Result = m_pBuffer->Map(0, nullptr, (void**)&ptr);

		// インデックスバッファマッピングに失敗しました
		if (FAILED(Result))
		{
			assert(SUCCEEDED(Result) && "インデックスバッファマッピングに失敗しました");
		}

		// インデックスデータをマッピング先に設定
		CopyMemory(ptr, data, size);

		// マッピング解除
		m_pBuffer->Unmap(0, nullptr);
	}
}

