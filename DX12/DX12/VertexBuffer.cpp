#include"VertexBuffer.h"
#include "DirectX12.h"
#include "d3dx12.h"

// �R���X�g���N�^
VertexBuffer::VertexBuffer() :
	m_buffer(nullptr),
	m_bufferSize(0) {}

// ����
void VertexBuffer::Create(void* pData, size_t size, UINT stride)
{
	m_bufferSize = size;

	// �q�[�v�v���p�e�B�[
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
		IID_PPV_ARGS(m_buffer.GetAddressOf()));

	// ���_�o�b�t�@���\�[�X�̐����Ɏ��s
	if (FAILED(Result))
	{
		assert(SUCCEEDED(Result) && "���_�o�b�t�@���\�[�X�̐����Ɏ��s���܂���");
	}

	// ���_�o�b�t�@�r���[�̐ݒ�
	m_bufferView.BufferLocation = m_buffer->GetGPUVirtualAddress();
	m_bufferView.SizeInBytes    = static_cast<UINT>(size);
	m_bufferView.StrideInBytes  = static_cast<UINT>(stride);

	// �}�b�s���O
	if (pData == nullptr) { return; }

	void* ptr = nullptr;
	Result = m_buffer->Map(0, nullptr, (void**)&ptr);

	// ���_�o�b�t�@�}�b�s���O�Ɏ��s���܂���
	if (FAILED(Result))
	{
		assert(SUCCEEDED(Result) && "���_�o�b�t�@�}�b�s���O�Ɏ��s���܂���");
	}

	// ���_�f�[�^���}�b�s���O��ɐݒ�
	CopyMemory(ptr, pData, size);

	// �}�b�s���O����
	m_buffer->Unmap(0, nullptr);
}

// �X�V
void VertexBuffer::Update(void* pData)
{
	// �}�b�s���O
	if (pData == nullptr) { return; }

	void* ptr = nullptr;
	HRESULT Result = m_buffer->Map(0, nullptr, (void**)&ptr);

	// ���_�o�b�t�@�}�b�s���O�Ɏ��s���܂���
	if (FAILED(Result))
	{
		assert(SUCCEEDED(Result) && "���_�o�b�t�@�}�b�s���O�Ɏ��s���܂���");
	}

	// ���_�f�[�^���}�b�s���O��ɐݒ�
	CopyMemory(ptr, pData, m_bufferSize);

	// �}�b�s���O����
	m_buffer->Unmap(0, nullptr);
}

