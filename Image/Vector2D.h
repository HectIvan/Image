#pragma once
#include <cmath>
#include <cassert>

using std::sqrt;
class Vector2D
{
public:
	Vector2D() = default;
	Vector2D(float _x, float _y = 0.0f) : x(_x), y(_y) {}
	Vector2D(const Vector2D& other) { x = other.x; y = other.y; }
	~Vector2D() = default;

	Vector2D& operator = (const Vector2D& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	// ADD 
	Vector2D operator+(const Vector2D& other)
	{
		return Vector2D(x + other.x, y + other.y);
	}

	Vector2D operator+(const float other)
	{
		return Vector2D(x + other, y + other);
	}

	Vector2D operator+=(const Vector2D& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2D operator+=(const float other)
	{
		x += other;
		y += other;
		return *this;
	}

	// SUBSTRACT
	Vector2D operator-(const Vector2D& other)
	{
		return Vector2D(x - other.x, y - other.y);
	}

	Vector2D operator-(const float other)
	{
		return Vector2D(x - other, y - other);
	}

	Vector2D operator-=(const Vector2D& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vector2D operator-=(const float other)
	{
		x -= other;
		y -= other;
		return *this;
	}

	// MULTIPLY
	Vector2D operator*(const Vector2D& other)
	{
		return Vector2D(x * other.x, y * other.y);
	}

	Vector2D operator*(const float other)
	{
		return Vector2D(x * other, y * other);
	}

	Vector2D operator*=(const Vector2D& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	Vector2D operator*=(const float other)
	{
		x *= other;
		y *= other;
		return *this;
	}

	// DIVIDE
	Vector2D operator/(const Vector2D& other)
	{
		return Vector2D(x / other.x, y / other.y);
	}

	Vector2D operator/(const float other)
	{
		return Vector2D(x / other, y / other);
	}

	Vector2D operator/=(const Vector2D& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	Vector2D operator/=(const float other)
	{
		x /= other;
		y /= other;
		return *this;
	}

	float operator|(const Vector2D& other) const
	{
		return (x * other.x) + (y * other.y);
	}

	float DotProd(const Vector2D& other) const
	{
		return this->operator|(other);
	}

	inline float MagnitudeSquare() const
	{
		return x * x + y * y;
	}

	float Magnitude() const
	{
		return sqrt(MagnitudeSquare());
	}

	void Normalize()
	{
		float mag = Magnitude();
		assert(mag > 0.0f && "trying to Normalize a Zero size Vector");
		mag = 1.0f / mag;
		x *= mag;
		y *= mag;
	}

	float VecAngle() const
	{
		return atan2(y, x);
	}

public:
	float x, y;
};


