#include "stdafx.h"
#include "Vector3D.h"


Vector3D::Vector3D() : x(0),y(0),z(0)
{
}

Vector3D::Vector3D(float x, float y, float z): x(x), y(y), z(z)
{
}

Vector3D::Vector3D(const Vector3D & vector): x(vector.x), y(vector.y), z(vector.z)
{
}


Vector3D::~Vector3D()
{
}

Vector3D Vector3D::operator+(const Vector3D & rhs)
{
	return Vector3D(x + rhs.x, y + rhs.y, z+rhs.y);
}

Vector3D Vector3D::operator-(const Vector3D & rhs)
{
	return  Vector3D(x - rhs.x, y - rhs.y, z - rhs.y);
}

Vector3D Vector3D::operator*(const float & rhs)
{
	return Vector3D(x * rhs, y * rhs, z * rhs);
}

Vector3D & Vector3D::operator+=(const Vector3D & rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}
/*
Vector3D & Vector3D::operator=(const Vector3D & rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;

	return *this;
}
*/