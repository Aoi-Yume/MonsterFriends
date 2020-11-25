//
// Created by 葵ユメ on 2018/08/19.
//

#ifndef AOIYUME_MATH_VECTOR
#define  AOIYUME_MATH_VECTOR

#include <math.h>
#include "math_Util.h"

class VEC3 {
public:
    VEC3();
    VEC3(float fx, float fy, float fz);

    ~VEC3();

    static VEC3 Zero();

    void Set(float fx, float fy, float fz);

    void SetX(float fx);
    void SetY(float fy);
    void SetZ(float fz);

    float GetX() const;
    float GetY() const;
    float GetZ() const;

    VEC3 operator+(const VEC3& add) const;
    VEC3 operator-(const VEC3& sub) const;
    VEC3 operator*(const float multi) const;
    VEC3 operator/(const float div) const;

    void operator+=(const VEC3& add);
    void operator-=(const VEC3& sub);
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
    float m_fx;
    float m_fy;
    float m_fz;
};


class VEC4 {
public:
	VEC4();
	VEC4(float fx, float fy, float fz, float fw);

	~VEC4();

	static VEC4 Zero();

	void Set(float fx, float fy, float fz, float fw);

	void SetX(float fx);
	void SetY(float fy);
	void SetZ(float fz);
	void SetW(float fw);

	float GetX() const;
	float GetY() const;
	float GetZ() const;
	float GetW() const;

	VEC4 operator+(const VEC4& add) const;
	VEC4 operator-(const VEC4& sub) const;
	VEC4 operator*(const float multi) const;
	VEC4 operator/(const float div) const;

	void operator+=(const VEC4& add);
	void operator-=(const VEC4& sub);
	void operator*=(const float multi);
	void operator/=(const float div);

	float Length() const;
	float SqrtLength() const;

	void Normalize();

	VEC4 Normalize() const;
private:
	float m_fx;
	float m_fy;
	float m_fz;
	float m_fw;
};


#endif