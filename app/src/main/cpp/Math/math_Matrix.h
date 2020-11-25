//
// Created by 葵ユメ on 2018/08/19.
//

#ifndef AOIYUME_MATH_MATRIX
#define  AOIYUME_MATH_MATRIX


#include "math_Vector.h"
#include "math_Util.h"
#include <math.h>


class MAT4 {
public:
    MAT4();
    ~MAT4();

    void SetIdentity();
	void SetScale(float x, float y, float z);
	void SetScale(const VEC3& scale);
	void SetRotate(float x, float y, float z);
	void SetRotate(const VEC3& rotate);
    void SetTranslate(float x, float y, float z);
    void SetTranslate(const VEC3& translate);
	void SetInverse();

    void Set(float* matrix);
	void Set(const VEC3& scale, const VEC3& rotate, const VEC3& translate);

	void SetView(VEC3& camPos, VEC3& eyePos, VEC3& upVec);
	void SetPerspective(float fFov, float fAspect, float fNear, float zFar);
	void SetOrtho(float fLeft, float fRight, float fTop, float fBottom, float fNear, float fFar);

    const float* Get() const;

public:
	MAT4 operator*(const MAT4 mtxA);
	VEC4 operator*(const VEC3& vecA);
	void operator*=(const MAT4 mtxA);

public:

	static MAT4 Multiple(const MAT4& mtxA, const MAT4& mtxB);
private:
    float m_fMatrix[4][4];
};

#endif