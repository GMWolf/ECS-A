#pragma once
class Vector3D
{
public:
	Vector3D();
	Vector3D(float x, float y, float z);
	Vector3D(const Vector3D& vector);
	~Vector3D();

	union {
		struct {
			float x;
			float y;
			float z;
			float pad;
		};
		float array[4];
	};

	Vector3D operator+(const Vector3D& rhs);
	Vector3D operator-(const Vector3D& rhs);
	Vector3D operator*(const float& rhs);
	Vector3D& operator+=(const Vector3D& rhs);
	//Vector3D& operator=(const Vector3D& rhs);
};

