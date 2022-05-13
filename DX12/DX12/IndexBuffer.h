#pragma once
#include<d3d12.h>
#include<wrl.h>
#include <vector>
#include "VertexBuffer.h"

// �C���f�b�N�X�o�b�t�@�N���X
class IndexBuffer
{
private:
	// �o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource>m_pBuffer;
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW  m_bufferView;
public:
	// �R���X�g���N�^
	IndexBuffer();
	// �f�X�g���N�^
	~IndexBuffer() = default;
public:
	// ����
	void Create(void* pData, size_t size, DXGI_FORMAT format = DXGI_FORMAT_R32_UINT);
public:
	// �o�b�t�@�r���[�̎擾
	D3D12_INDEX_BUFFER_VIEW  GetBufferView() const { return m_bufferView; }
};