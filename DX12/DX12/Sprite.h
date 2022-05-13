#pragma once
#include <memory>
#include "Math.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

// ���_�\����
struct Vertex
{
	Vector3 m_pos;    // ���W
	Vector2 m_uv;	    // UV
	Vector4 m_color;  // ���_�J���[
};

// �X�v���C�g�N���X
class Sprite
{
	// ���W
	Vector2 m_position;
	// �T�C�Y
	Vector2 m_size;
	// �J���[
	Vector4 m_color;
	// ��A���A�E�A���̍��W
	float m_top, m_bottom, m_right, m_left;
	// ���_
	std::unique_ptr<Vertex[]>m_pVertices;
	// �O�p�`�i���_���ѕ��j
	std::unique_ptr<DWORD[]>m_pIndices;
	// ���_�o�b�t�@
	std::unique_ptr<VertexBuffer>m_pVertexBuffer;
	// �C���f�b�N�X�o�b�t�@
	std::unique_ptr<IndexBuffer>m_pIndexBuffer;

	// ���_�X�V
	void UpdateVertices();
public:
	// �R���X�g���N�^
	Sprite();
	// �f�X�g���N�^
	~Sprite() = default;
	// �ǂݍ���
	void Load(const char* filename = nullptr);
	// �`��
	void Render();

	// ���W�̎擾
	Vector2 GetPosition() { return m_position; }
	// �T�C�Y�̎擾
	Vector2 GetSize() { return m_size; }

	// ���W�̃Z�b�g
	void SetPosition(Vector2 position) { m_position = position; }
	// �T�C�Y�̃Z�b�g
	void SetSize(Vector2 size) { m_size = size; }
	// �J���[�̃Z�b�g
	void SetColor(Vector4 color) { m_color = color; }
};