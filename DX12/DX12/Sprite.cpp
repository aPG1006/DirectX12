#include "Sprite.h"
#include "DirectX12.h"
#include "Window.h"

// コンストラクタ
Sprite::Sprite() :
	m_position(0.f, 0.f),
	m_size(256.f, 256.f),
	m_color(1.f, 1.f, 1.f, 1.f),
	m_top(0.f), m_bottom(0.f), m_right(0.f), m_left(0.f) {}

void Sprite::Load(const char* filename)
{
	Vertex vertices[4] = {};
	// 頂点座標
	vertices[0].m_pos = Vector3(1.f, 0.f, 0.f);
	vertices[1].m_pos = Vector3(0.f, 0.f, 0.f);
	vertices[2].m_pos = Vector3(1.f, -1.f, 0.f);
	vertices[3].m_pos = Vector3(0.f, -1.f, 0.f);

	// UV座標
	vertices[0].m_uv = Vector2(1.f, 0.f);
	vertices[1].m_uv = Vector2(0.f, 0.f);
	vertices[2].m_uv = Vector2(1.f, 1.f);
	vertices[3].m_uv = Vector2(0.f, 1.f);

	// カラー
	for (int i = 0; i < 4; i++)
	{
		vertices[i].m_color = m_color;
	}

	// 頂点バッファ生成
	m_pVertexBuffer = std::make_unique<VertexBuffer>();
	m_pVertexBuffer->Create(vertices, sizeof(vertices), sizeof(Vertex));

	// インデックス番号
	m_pIndices = std::make_unique<DWORD[]>(6);
	m_pIndices[0] = 0;
	m_pIndices[1] = 1;
	m_pIndices[2] = 2;
	m_pIndices[3] = 2;
	m_pIndices[4] = 1;
	m_pIndices[5] = 3;

	m_pIndexBuffer = std::make_unique<IndexBuffer>();
	m_pIndexBuffer->Create(m_pIndices.get(), sizeof(DWORD) * 6);

	// 初期化
	SetPosition(Vector2(500.f, 0.f));
	SetSize(Vector2(256, 256));
}

// 描画
void Sprite::Render()
{
	UpdateVertices();

	auto CommandList = DIRECTX().GetCommandList();
	auto VertexBufferView = m_pVertexBuffer->GetBufferView();
	auto IndexBufferView = m_pIndexBuffer->GetBufferView();

	CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CommandList->IASetVertexBuffers(0, 1, &VertexBufferView);
	CommandList->IASetIndexBuffer(&IndexBufferView);
	CommandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

// 頂点更新
void Sprite::UpdateVertices()
{
	Vertex vertices[4] = {};

	// 頂点座標
	vertices[0].m_pos = Vector3(m_size.m_x, 0.f, 0.f);
	vertices[1].m_pos = Vector3(0.f, 0.f, 0.f);
	vertices[2].m_pos = Vector3(m_size.m_x, m_size.m_y, 0.f);
	vertices[3].m_pos = Vector3(0.f, m_size.m_y, 0.f);

	// UV座標
	vertices[0].m_uv = Vector2(m_right, m_top);
	vertices[1].m_uv = Vector2(m_left,  m_top);
	vertices[2].m_uv = Vector2(m_right, m_bottom);
	vertices[3].m_uv = Vector2(m_left,  m_bottom);

	for (int i = 0; i < 4; i++)
	{
		// カラー
		vertices[i].m_color = m_color;

		//スクリーン変換
		vertices[i].m_pos.m_x = (vertices[i].m_pos.m_x) + m_position.m_x - 0.5f;
		vertices[i].m_pos.m_y = (vertices[i].m_pos.m_y) + m_position.m_y - 0.5f;

		vertices[i].m_pos.m_x -=  WindowWidth  / 2.f;
		vertices[i].m_pos.m_y -=  WindowHeight / 2.f;
		vertices[i].m_pos.m_x /=  WindowWidth  / 2.f;
		vertices[i].m_pos.m_y /= -WindowHeight / 2.f;
	}

	// 頂点バッファ更新
	m_pVertexBuffer->Update(vertices);
}