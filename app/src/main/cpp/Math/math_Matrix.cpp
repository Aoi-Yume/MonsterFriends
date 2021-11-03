//
// Created by 葵ユメ on 2018/08/19.
//

#include "math_Matrix.h"
#include "../aoiyume_pch.h"
#include <math.h>
#include <memory>

MAT4::MAT4()
{
    SetIdentity();
}

MAT4::~MAT4() 
{
}

void MAT4::SetIdentity()
{
    float matValI[16] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };
    memcpy(m_fMatrix, matValI, sizeof(matValI));
}

void MAT4::SetScale(float x, float y, float z)
{
    m_fMatrix[0][0] = x;
    m_fMatrix[1][1] = y;
    m_fMatrix[2][2] = z;
}

void MAT4::SetScale(const VEC3& scale)
{
    SetScale(scale.X(), scale.Y(), scale.Z());
}

void MAT4::SetRotate(float x, float y, float z)
{
	const float fRadX = DEGTORAD(x);
	const float fRadY = DEGTORAD(y);
	const float fRadZ = DEGTORAD(z);
	MAT4 rX, rY, rZ;
	rX.m_fMatrix[1][1] = cosf(fRadX);
	rX.m_fMatrix[1][2] = -sinf(fRadX);
	rX.m_fMatrix[2][1] = sinf(fRadX);
	rX.m_fMatrix[2][2] = cosf(fRadX);

	rY.m_fMatrix[0][0] = cosf(fRadY);
	rY.m_fMatrix[0][2] = sinf(fRadY);
	rY.m_fMatrix[2][0] = -sinf(fRadY);
	rY.m_fMatrix[2][2] = cosf(fRadY);

	rZ.m_fMatrix[0][0] = cosf(fRadZ);
	rZ.m_fMatrix[0][1] = -sinf(fRadZ);
	rZ.m_fMatrix[1][0] = sinf(fRadZ);
	rZ.m_fMatrix[1][1] = cosf(fRadZ);

	*this = rX * rZ * rY;
}

void MAT4::SetRotate(const VEC3 &rotate)
{
    SetRotate(rotate.X(), rotate.Y(), rotate.Z());
}

void MAT4::SetTranslate(float x, float y, float z)
{
    m_fMatrix[3][0] = x;
    m_fMatrix[3][1] = y;
    m_fMatrix[3][2] = z;
}

void MAT4::SetTranslate(const VEC3 &translate)
{
    memcpy(&m_fMatrix[3][0], &translate, sizeof(translate));
}

void MAT4::SetInverse() {
	MAT4 ret;
	for(int i = 0; i < 4; ++i)
	{
		float div = m_fMatrix[i][i];
		if(int(div * 10000.0f) != 0) {
			for (int j = 0; j < 4; ++j) {
				m_fMatrix[i][j] /= div;
				ret.m_fMatrix[i][j] /= div;
			}
		}
		for(int j = 0; j < 4; ++j){
			if(i == j){ continue; }
			float multi = m_fMatrix[ j ][ i ];
			for(int k = 0; k < 4; ++k){
				if(j == k){ continue; }
				m_fMatrix[j][k] -= m_fMatrix[i][k] * multi;
				ret.m_fMatrix[j][k] -= ret.m_fMatrix[i][k] * multi;
			}
		}
	}
	(*this) = ret;
}

void MAT4::Set(float* matrix)
{
    memcpy(&m_fMatrix[0][0], matrix, sizeof(m_fMatrix));
}

void MAT4::Set(const VEC3 &scale, const VEC3 &rotate, const VEC3& translate)
{
    MAT4 s, r, t;
    s.SetScale(scale);
    r.SetRotate(rotate);
    t.SetTranslate(translate);
    (*this) = s;
    (*this) = r * (*this);
    (*this) = t * (*this);
}

void MAT4::SetView(VEC3& camPos, VEC3& eyePos, VEC3& upVec)
{
	VEC3 upAngle = upVec;
	upAngle.Normalize();

	VEC3 zAngle = camPos - eyePos;
	zAngle.Normalize();
	VEC3 xAngle = upAngle.Cross(zAngle);
	VEC3 yAngle = zAngle.Cross(xAngle);

	m_fMatrix[0][0] = xAngle.X();
	m_fMatrix[0][1] = yAngle.X();
	m_fMatrix[0][2] = zAngle.X();

	m_fMatrix[1][0] = xAngle.Y();
	m_fMatrix[1][1] = yAngle.Y();
	m_fMatrix[1][2] = zAngle.Y();

	m_fMatrix[2][0] = xAngle.Z();
	m_fMatrix[2][1] = yAngle.Z();
	m_fMatrix[2][2] = zAngle.Z();
	VEC3 invCamPos = camPos * -1.0f;
	memcpy(m_fMatrix[3], &invCamPos, sizeof(VEC3));
}

void MAT4::SetPerspective(float fFov, float fAspect, float fNear, float fFar)
{
    const float fFovCot = 1.0f / tanf(fFov * 0.5f);

    m_fMatrix[0][0] = fFovCot / fAspect;
    m_fMatrix[1][1] = fFovCot;
    m_fMatrix[2][2] = -fFar * 1.0f / (fFar - fNear);
    m_fMatrix[3][2] =  fFar * -fNear / (fFar - fNear);
    m_fMatrix[2][3] = -1.0f;
    m_fMatrix[3][3] = 0.0f;
	/*
	m_fMatrix[0][0] = fFovCot / fAspect;
	m_fMatrix[1][1] = fFovCot;
	m_fMatrix[2][2] = 1.0f / (fFar - fNear);
	m_fMatrix[3][2] = -fNear / (fFar - fNear);
	m_fMatrix[2][3] = 0.0f;
	m_fMatrix[3][3] = 1.0f;
	 */
}

void MAT4::SetOrtho(float fLeft, float fRight, float fTop, float fBottom, float fNear, float fFar)
{
	const float fSubRL = fRight - fLeft;
	const float fAddRL = fRight + fLeft;
	const float fSubBT = fBottom - fTop;
	const float fAddBT = fBottom + fTop;
	const float fSubFN = fFar - fNear;
	const float fAddFN = fFar + fNear;

	m_fMatrix[0][0] = 2.0f / fSubRL;
	m_fMatrix[1][1] = 2.0f / fSubBT;
	m_fMatrix[2][2] = -2.0f / fSubFN;
//	m_fMatrix[3][0] = -(fAddRL / fSubRL);
//	m_fMatrix[3][1] = -(fAddBT / fSubBT);
//	m_fMatrix[3][2] = -(fAddFN / fSubFN);
	m_fMatrix[3][3] = 1.0f;
}


const float* MAT4::Get() const
{
    return &m_fMatrix[0][0];
}

MAT4 MAT4::operator*(const MAT4 mtxA)
{
    return Multiple((*this), mtxA);
}

VEC4 MAT4::operator*(const VEC3& vecA)
{
	VEC4 ret;
	float x = m_fMatrix[0][0] * vecA.X() + m_fMatrix[1][0] * vecA.Y() + m_fMatrix[2][0] * vecA.Z() + m_fMatrix[3][0];
	float y = m_fMatrix[0][1] * vecA.X() + m_fMatrix[1][1] * vecA.Y() + m_fMatrix[2][1] * vecA.Z() + m_fMatrix[3][1];
	float z = m_fMatrix[0][2] * vecA.X() + m_fMatrix[1][2] * vecA.Y() + m_fMatrix[2][2] * vecA.Z() + m_fMatrix[3][2];
	float w = m_fMatrix[0][3] * vecA.X() + m_fMatrix[1][3] * vecA.Y() + m_fMatrix[2][3] * vecA.Z() + m_fMatrix[3][3];
	ret.Set(x, y, z, w);
	return ret;
}

void MAT4::operator*=(const MAT4 mtxA)
{
    (*this) = (*this) * mtxA;
}

MAT4 MAT4::Multiple(const MAT4 &mtxA, const MAT4 &mtxB)
{
    MAT4 ret;
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 4; ++j){
            float param = 0.0f;
            for(int k = 0; k < 4; ++k){
                param += mtxA.m_fMatrix[k][i] * mtxB.m_fMatrix[j][k];
            }
            ret.m_fMatrix[j][i] = param;
        }
    }
    return ret;
}