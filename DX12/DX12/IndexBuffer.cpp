#include "IndexBuffer.h"
#include "DirectX12.h"
#include "d3dx12.h"

// �R���X�g���N�^
IndexBuffer::IndexBuffer() :
	m_pBuffer(nullptr) {}

// ����
void IndexBuffer::Create(void* data, size_t size, DXGI_FORMAT format)
{
	auto Properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	auto ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(size);

	// ���\�[�X�ݒ�
	HRESULT Result = DIRECTX().GetDevice()->CreateCommittedResource(
		&Properties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_pBuffer.GetAddressOf()));

	// �C���f�b�N�X�o�b�t�@���\�[�X�̐����Ɏ��s
	if (FAILED(Result))
	{
		assert(SUCCEEDED(Result) && "�C���f�b�N�X�o�b�t�@���\�[�X�̐����Ɏ��s���܂���");
	}

	// �C���f�b�N�X�o�b�t�@�r���[�̐ݒ�
	m_bufferView.BufferLocation = m_pBuffer->GetGPUVirtualAddress();
	m_bufferView.Format = format;
	m_bufferView.SizeInBytes = static_cast<UINT>(size);

	// �}�b�s���O
	if (data != nullptr)
	{
		void* ptr = nullptr;
		Result = m_pBuffer->Map(0, nullptr, (void**)&ptr);

		// �C���f�b�N�X�o�b�t�@�}�b�s���O�Ɏ��s���܂���
		if (FAILED(Result))
		{
			assert(SUCCEEDED(Result) && "�C���f�b�N�X�o�b�t�@�}�b�s���O�Ɏ��s���܂���");
		}

		// �C���f�b�N�X�f�[�^���}�b�s���O��ɐݒ�
		CopyMemory(ptr, data, size);

		// �}�b�s���O����
		m_pBuffer->Unmap(0, nullptr);
	}
}

