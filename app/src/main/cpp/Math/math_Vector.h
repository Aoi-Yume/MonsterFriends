//
// Created by 葵ユメ on 2018/08/19.
//

#ifndef AOIYUME_MATH_VECTOR
#define  AOIYUME_MATH_VECTOR

#include <math.h>
#include "math_Util.h"

class VEC2 {
public:
	VEC2();
	VEC2(float fx, float fy);

	~VEC2();

	static VEC2 Zero();

	void Set(float fx, float fy);

	void SetX(float fx);
	void SetY(float fy);

	float X() const;
	float Y() const;

	VEC2 operator+(const VEC2& add) const;
	VEC2 operator-(const VEC2& sub) const;
	VEC2 operator*(const float multi) const;
	VEC2 operator/(const float div) const;

	void operator+=(const VEC2& add);
	void operator-=(const VEC2& sub);
	void operator*=(const float multi);
	void operator/=(const float div);

	float Length() const;
	float SqrtLength() const;

	void Normalize();

	VEC2 Normalize() const;

	float Dot(const VEC2& vec) const;
	float Cross(const VEC2& vec) const;
private:
	float x;
	float y;
};

class VEC3 {
public:
    VEC3();
    VEC3(float fx, float fy, float fz);

    ~VEC3();

    static VEC3 Zero();
    static VEC3 One();

    void Set(float fx, float fy, float fz);

    void SetX(float fx);
    void SetY(float fy);
    void SetZ(float fz);

    float X() const;
    float Y() const;
    float Z() const;

    VEC3 operator+(const VEC3& add) const;
    VEC3 operator-(const VEC3& sub) const;
    VEC3 operator*(const VEC3& multi) const;
    VEC3 operator*(const float multi) const;
    VEC3 operator/(const float div) const;

    void operator+=(const VEC3& add);
    void operator-=(const VEC3& sub);
	void operator*=(const VEC3& multi);
    void operator*=(const float multi);
    void operator/=(const float div);

    float Length() const;
    float SqrtLength() const;

    void Normalize();

    VEC3 Normalize() const;

    float Dot(const VEC3& vec) const;
    float DotXY(const VEC3& vec) const;
    float DotXZ(const VEC3& vec) const;

    VEC3 Cross(const VEC3& vec) const;
    float CrossXY(const VEC3& vec) const;
    float CrossYZ(const VEC3& vec) const;
    float CrossZX(const VEC3& vec) const;
private:
    float x;
    float y;
    float z;
};


class VEC4 {
public:
	VEC4();
	VEC4(float fx, float fy, float fz, float fw);

	~VEC4();

	static VEC4 Zero();
	static VEC4 One();

	void Set(float fx, float fy, float fz, float fw);

	void SetX(float fx);
	void SetY(float fy);
	void SetZ(float fz);
	void SetW(float fw);

	float X() const;
	float Y() const;
	float Z() const;
	float W() const;

	VEC4 operator+(const VEC4& add) const;
	VEC4 operator-(const VEC4& sub) const;
	VEC4 operator*(const VEC4& multi) const;
	VEC4 operator*(const float multi) const;
	VEC4 operator/(const float div) const;

	void operator+=(const VEC4& add);
	void operator-=(const VEC4& sub);
	void operator*=(const VEC4& multi);
	void operator*=(const float multi);
	void operator/=(const float div);

	float Length() const;
	float SqrtLength() const;

	void Normalize();

	VEC4 Normalize() const;
private:
	float x;
	float y;
	float z;
	float w;
};


#endif