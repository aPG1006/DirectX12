#pragma once
#include<d3d12.h>
#include<wrl.h>
#include<memory>
#include<vector>

// ���_�o�b�t�@�N���X
class VertexBuffer
{
private:
	// �o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource>m_buffer;
	// �o�b�t�@�T�C�Y
	UINT m_bufferSize;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW m_bufferView;
public:
	// �R���X�g���N�^
	VertexBuffer();
	// �f�X�g���N�^
	~VertexBuffer() = default;
	// ����
	void Create(void* pData, size_t size, UINT stride);
	// �X�V
	void Update(void* pData);

	// �o�b�t�@�r���[�̎擾
	D3D12_VERTEX_BUFFER_VIEW GetBufferView() const { return m_bufferView; }
};