#pragma once

// 2次元ベクトル構造体
struct Vector2
{
	float m_x, m_y;

	Vector2() : m_x(0.f), m_y(0.f) {}
	Vector2(const Vector2& vec)
	{
		m_x = vec.m_x;
		m_y = vec.m_y;
	}
	Vector2(float x, float y)
	{
		this->m_x = x;
		this->m_y = y;
	}
};

// 3次元ベクトル構造体
struct Vector3
{
	float m_x, m_y, m_z;

	Vector3() : m_x(0.f), m_y(0.f), m_z(0.f) {}
	Vector3(const Vector3& vec)
	{
		m_x = vec.m_x;
		m_y = vec.m_y;
		m_z = vec.m_z;
	}

	Vector3(float x, float y, float z)
	{
		this->m_x = x;
		this->m_y = y;
		this->m_z = z;
	}
};

// 4次元ベクトル構造体
struct  Vector4
{
	float m_x, m_y, m_z, m_w;

	Vector4() : m_x(0.f), m_y(0.f), m_z(0.f), m_w(0.f) {}
	Vector4(const Vector4& vec)
	{
		this->m_x = vec.m_x;
		this->m_y = vec.m_y;
		this->m_z = vec.m_z;
		this->m_w = vec.m_w;
	}

	Vector4(float x, float y, float z, float w)
	{
		this->m_x = x;
		this->m_y = y;
		this->m_z = z;
		this->m_w = w;
	}
};