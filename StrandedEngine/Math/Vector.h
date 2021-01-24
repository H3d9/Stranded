// Stranded Engine
// ͨ����ѧ���� Vector 3D

#pragma once


struct Vector3 {
	Vector3() {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
	Vector3(float X, float Y, float Z) {
		x = X;
		y = Y;
		z = Z;
	}

	// Vector3�������ء�
	Vector3 operator-(const Vector3 &v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	Vector3 operator+(const Vector3 &v) const {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}
	Vector3 operator*(const Vector3 &v) const {
		return Vector3(x * v.x, y * v.y, z * v.z);
	}
	Vector3 operator/(const Vector3 &v) const {
		return Vector3(x / v.x, y / v.y, z / v.z);
	}

	// float�������ء�
	Vector3 operator+(float f) const {
		return Vector3(x + f, y + f, z + f);
	}
	Vector3 operator-(float f) const {
		return Vector3(x - f, y - f, z - f);
	}
	Vector3 operator*(float f) const {
		return Vector3(x * f, y * f, z * f);
	}
	Vector3 operator/(float f) const {
		f = 1 / f;
		return Vector3(x * f, y * f, z * f);
	}

	// ��Ŀ�������ء�
	void operator +=(const Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}
	void operator -=(const Vector3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}
	void operator *=(const Vector3& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
	}

	void operator /=(const Vector3& v) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
	}
	void operator +=(float f) {
		x += f;
		y += f;
		z += f;
	}
	void operator -=(float f) {
		x -= f;
		y -= f;
		z -= f;
	}
	void operator *=(float f) {
		x *= f;
		y *= f;
		z *= f;
	}
	void operator /=(float f) {
		f = 1 / f;
		x *= f;
		y *= f;
		z *= f;
	}

	float x;
	float y;
	float z;
}; // struct Vector 3D
