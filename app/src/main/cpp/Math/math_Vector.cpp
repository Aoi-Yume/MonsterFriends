//
// Created by 葵ユメ on 2018/08/19.
//

#include "math_Vector.h"

#include "../aoiyume_pch.h"
#include <math.h>

VEC2::VEC2()
    : m_fx(0.0f)
    , m_fy(0.0f)
{
}

VEC2::VEC2(float fx, float fy)
    : m_fx(fx)
    , m_fy(fy)
{
}

VEC2::~VEC2()
{
}

VEC2 VEC2::Zero()
{
    return VEC2(0.0f, 0.0f);
}

void VEC2::Set(float fx, float fy)
{
    m_fx = fx;
    m_fy = fy;
}

void VEC2::SetX(float fx) { m_fx = fx; }
void VEC2::SetY(float fy) { m_fy = fy; }

float VEC2::GetX() const { return m_fx; }
float VEC2::GetY() const { return m_fy; }

VEC2 VEC2::operator+(const VEC2& add) const { return VEC2(m_fx + add.m_fx, m_fy + add.m_fy); }
VEC2 VEC2::operator-(const VEC2& sub) const { return VEC2(m_fx - sub.m_fx, m_fy - sub.m_fy); }
VEC2 VEC2::operator*(const float multi) const { return VEC2(m_fx * multi, m_fy * multi); }
VEC2 VEC2::operator/(const float div) const { return VEC2(m_fx / div, m_fy / div); }

void VEC2::operator+=(const VEC2& add) { *this = (*this) + add; }
void VEC2::operator-=(const VEC2& sub) { *this = (*this) - sub; }
void VEC2::operator*=(const float multi){ *this = (*this) * multi; }
void VEC2::operator/=(const float div){ *this = (*this) / div; }

float VEC2::Length() const { return sqrtf(m_fx * m_fx + m_fy * m_fy); }
float VEC2::SqrtLength() const { return m_fx * m_fx + m_fy * m_fy; }

void VEC2::Normalize()
{
    const float length = Length();
    if(length <= 0.0f){ return; }
    *this /= length;
}

VEC2 VEC2::Normalize() const
{
    const float length = Length();
    if(length <= 0.0f){  return Zero(); }
    return (*this) / length;
}

float VEC2::Dot(const VEC2& vec) const { return m_fx * vec.m_fx + m_fy * vec.m_fy; }
float VEC2::Cross(const VEC2& vec) const { return m_fx * vec.m_fy - m_fy * vec.m_fx; }

VEC3::VEC3()
        : m_fx(0.0f)
        , m_fy(0.0f)
        , m_fz(0.0f)
{
}

VEC3::VEC3(float fx, float fy, float fz)
        : m_fx(fx)
        , m_fy(fy)
        , m_fz(fz)
{
}

VEC3::~VEC3()
{
}

VEC3 VEC3::Zero()
{
    return VEC3(0.0f, 0.0f, 0.0f);
}

void VEC3::Set(float fx, float fy, float fz)
{
    m_fx = fx;
    m_fy = fy;
    m_fz = fz;
}

void VEC3::SetX(float fx) { m_fx = fx; }
void VEC3::SetY(float fy) { m_fy = fy; }
void VEC3::SetZ(float fz) { m_fz = fz; }

float VEC3::GetX() const { return m_fx; }
float VEC3::GetY() const { return m_fy; }
float VEC3::GetZ() const { return m_fz; }

VEC3 VEC3::operator+(const VEC3& add) const { return VEC3(m_fx + add.m_fx, m_fy + add.m_fy, m_fz + add.m_fz); }
VEC3 VEC3::operator-(const VEC3& sub) const { return VEC3(m_fx - sub.m_fx, m_fy - sub.m_fy, m_fz - sub.m_fz); }
VEC3 VEC3::operator*(const float multi) const { return VEC3(m_fx * multi, m_fy * multi, m_fz * multi); }
VEC3 VEC3::operator/(const float div) const { return VEC3(m_fx / div, m_fy / div, m_fz / div); }

void VEC3::operator+=(const VEC3& add) { *this = (*this) + add; }
void VEC3::operator-=(const VEC3& sub) { *this = (*this) - sub; }
void VEC3::operator*=(const float multi){ *this = (*this) * multi; }
void VEC3::operator/=(const float div){ *this = (*this) / div; }

float VEC3::Length() const { return sqrtf(m_fx * m_fx + m_fy * m_fy + m_fz * m_fz); }
float VEC3::SqrtLength() const { return m_fx * m_fx + m_fy * m_fy + m_fz * m_fz; }

void VEC3::Normalize()
{
    const float length = Length();
    if(length <= 0.0f){ return; }
    *this /= length;
}

VEC3 VEC3::Normalize() const
{
    const float length = Length();
    if(length <= 0.0f){  return Zero(); }
    return (*this) / length;
}

float VEC3::Dot(const VEC3& vec) const { return m_fx * vec.m_fx + m_fy * vec.m_fy + m_fz * vec.m_fz; }
float VEC3::DotXY(const VEC3& vec) const { return m_fx + vec.m_fx + m_fy * vec.m_fy; }
float VEC3::DotXZ(const VEC3& vec) const { return m_fx * vec.m_fx + m_fz * vec.m_fz; }

VEC3 VEC3::Cross(const VEC3& vec) const { return VEC3(CrossYZ(vec), CrossZX(vec), CrossXY(vec)); }
float VEC3::CrossXY(const VEC3& vec) const { return m_fx * vec.m_fy - m_fy * vec.m_fx; }
float VEC3::CrossYZ(const VEC3& vec) const { return m_fy * vec.m_fz - m_fz * vec.m_fy; }
float VEC3::CrossZX(const VEC3& vec) const { return m_fz * vec.m_fx - m_fx * vec.m_fz; }



VEC4::VEC4()
: m_fx(0.0f)
, m_fy(0.0f)
, m_fz(0.0f)
, m_fw(0.0f)
{
}

VEC4::VEC4(float fx, float fy, float fz, float fw)
: m_fx(fx)
, m_fy(fy)
, m_fz(fz)
, m_fw(fw)
{
}

VEC4::~VEC4()
{
}

VEC4 VEC4::Zero()
{
    return VEC4(0.0f, 0.0f, 0.0f, 0.0f);
}

void VEC4::Set(float fx, float fy, float fz, float fw)
{
    m_fx = fx;
    m_fy = fy;
    m_fz = fz;
    m_fw = fw;
}

void VEC4::SetX(float fx) { m_fx = fx; }
void VEC4::SetY(float fy) { m_fy = fy; }
void VEC4::SetZ(float fz) { m_fz = fz; }
void VEC4::SetW(float fw) { m_fw = fw; }

float VEC4::GetX() const { return m_fx; }
float VEC4::GetY() const { return m_fy; }
float VEC4::GetZ() const { return m_fz; }
float VEC4::GetW() const { return m_fw; }

VEC4 VEC4::operator+(const VEC4& add) const { return VEC4(m_fx + add.m_fx, m_fy + add.m_fy, m_fz + add.m_fz, m_fw + add.m_fw); }
VEC4 VEC4::operator-(const VEC4& sub) const { return VEC4(m_fx - sub.m_fx, m_fy - sub.m_fy, m_fz - sub.m_fz, m_fw - sub.m_fw); }
VEC4 VEC4::operator*(const float multi) const { return VEC4(m_fx * multi, m_fy * multi, m_fz * multi, m_fw * multi); }
VEC4 VEC4::operator/(const float div) const { return VEC4(m_fx / div, m_fy / div, m_fz / div, m_fw / div); }

void VEC4::operator+=(const VEC4& add) { *this = (*this) + add; }
void VEC4::operator-=(const VEC4& sub) { *this = (*this) - sub; }
void VEC4::operator*=(const float multi){ *this = (*this) * multi; }
void VEC4::operator/=(const float div){ *this = (*this) / div; }

float VEC4::Length() const { return sqrtf(m_fx * m_fx + m_fy * m_fy + m_fz * m_fz); }
float VEC4::SqrtLength() const { return m_fx * m_fx + m_fy * m_fy + m_fz * m_fz; }

void VEC4::Normalize()
{
    const float length = Length();
    if(length <= 0.0f){ return; }
    *this /= length;
}

VEC4 VEC4::Normalize() const
{
    const float length = Length();
    if(length <= 0.0f){  return Zero(); }
    return (*this) / length;
}