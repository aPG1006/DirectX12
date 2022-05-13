#pragma once
#include <memory>
#include "Math.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

// 頂点構造体
struct Vertex
{
	Vector3 m_pos;    // 座標
	Vector2 m_uv;	    // UV
	Vector4 m_color;  // 頂点カラー
};

// スプライトクラス
class Sprite
{
	// 座標
	Vector2 m_position;
	// サイズ
	Vector2 m_size;
	// カラー
	Vector4 m_color;
	// 上、下、右、左の座標
	float m_top, m_bottom, m_right, m_left;
	// 頂点
	std::unique_ptr<Vertex[]>m_pVertices;
	// 三角形（頂点結び方）
	std::unique_ptr<DWORD[]>m_pIndices;
	// 頂点バッファ
	std::unique_ptr<VertexBuffer>m_pVertexBuffer;
	// インデックスバッファ
	std::unique_ptr<IndexBuffer>m_pIndexBuffer;

	// 頂点更新
	void UpdateVertices();
public:
	// コンストラクタ
	Sprite();
	// デストラクタ
	~Sprite() = default;
	// 読み込み
	void Load(const char* filename = nullptr);
	// 描画
	void Render();

	// 座標の取得
	Vector2 GetPosition() { return m_position; }
	// サイズの取得
	Vector2 GetSize() { return m_size; }

	// 座標のセット
	void SetPosition(Vector2 position) { m_position = position; }
	// サイズのセット
	void SetSize(Vector2 size) { m_size = size; }
	// カラーのセット
	void SetColor(Vector4 color) { m_color = color; }
};