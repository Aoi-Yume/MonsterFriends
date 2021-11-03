//
// Created by 葵ユメ on 2018/08/19.
//

#include "math_Vector.h"

#include "../aoiyume_pch.h"
#include <math.h>

VEC2::VEC2()
    : x(0.0f)
    , y(0.0f)
{
}

VEC2::VEC2(float fx, float fy)
    : x(fx)
    , y(fy)
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
    x = fx;
    y = fy;
}

void VEC2::SetX(float fx) { x = fx; }
void VEC2::SetY(float fy) { y = fy; }

float VEC2::X() const { return x; }
float VEC2::Y() const { return y; }

VEC2 VEC2::operator+(const VEC2& add) const { return VEC2(x + add.x, y + add.y); }
VEC2 VEC2::operator-(const VEC2& sub) const { return VEC2(x - sub.x, y - sub.y); }
VEC2 VEC2::operator*(const float multi) const { return VEC2(x * multi, y * multi); }
VEC2 VEC2::operator/(const float div) const { return VEC2(x / div, y / div); }

void VEC2::operator+=(const VEC2& add) { *this = (*this) + add; }
void VEC2::operator-=(const VEC2& sub) { *this = (*this) - sub; }
void VEC2::operator*=(const float multi){ *this = (*this) * multi; }
void VEC2::operator/=(const float div){ *this = (*this) / div; }

float VEC2::Length() const { return sqrtf(x * x + y * y); }
float VEC2::SqrtLength() const { return x * x + y * y; }

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

float VEC2::Dot(const VEC2& vec) const { return x * vec.x + y * vec.y; }
float VEC2::Cross(const VEC2& vec) const { return x * vec.y - y * vec.x; }

VEC3::VEC3()
        : x(0.0f)
        , y(0.0f)
        , z(0.0f)
{
}

VEC3::VEC3(float fx, float fy, float fz)
        : x(fx)
        , y(fy)
        , z(fz)
{
}

VEC3::~VEC3()
{
}

VEC3 VEC3::Zero()
{
    return {0.0f, 0.0f, 0.0f};
}

VEC3 VEC3::One()
{
    return {1.0f, 1.0f, 1.0f};
}

void VEC3::Set(float fx, float fy, float fz)
{
    x = fx;
    y = fy;
    z = fz;
}

void VEC3::SetX(float fx) { x = fx; }
void VEC3::SetY(float fy) { y = fy; }
void VEC3::SetZ(float fz) { z = fz; }

float VEC3::X() const { return x; }
float VEC3::Y() const { return y; }
float VEC3::Z() const { return z; }

VEC3 VEC3::operator+(const VEC3& add) const { return VEC3(x + add.x, y + add.y, z + add.z); }
VEC3 VEC3::operator-(const VEC3& sub) const { return VEC3(x - sub.x, y - sub.y, z - sub.z); }
VEC3 VEC3::operator*(const VEC3& multi) const { return VEC3(x * multi.x, y * multi.y, z * multi.z); }
VEC3 VEC3::operator*(const float multi) const { return VEC3(x * multi, y * multi, z * multi); }
VEC3 VEC3::operator/(const float div) const { return VEC3(x / div, y / div, z / div); }

void VEC3::operator+=(const VEC3& add) { *this = (*this) + add; }
void VEC3::operator-=(const VEC3& sub) { *this = (*this) - sub; }
void VEC3::operator*=(const VEC3& multi){ *this = (*this) * multi; }
void VEC3::operator*=(const float multi){ *this = (*this) * multi; }
void VEC3::operator/=(const float div){ *this = (*this) / div; }

float VEC3::Length() const { return sqrtf(x * x + y * y + z * z); }
float VEC3::SqrtLength() const { return x * x + y * y + z * z; }

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

float VEC3::Dot(const VEC3& vec) const { return x * vec.x + y * vec.y + z * vec.z; }
float VEC3::DotXY(const VEC3& vec) const { return x + vec.x + y * vec.y; }
float VEC3::DotXZ(const VEC3& vec) const { return x * vec.x + z * vec.z; }

VEC3 VEC3::Cross(const VEC3& vec) const { return VEC3(CrossYZ(vec), CrossZX(vec), CrossXY(vec)); }
float VEC3::CrossXY(const VEC3& vec) const { return x * vec.y - y * vec.x; }
float VEC3::CrossYZ(const VEC3& vec) const { return y * vec.z - z * vec.y; }
float VEC3::CrossZX(const VEC3& vec) const { return z * vec.x - x * vec.z; }



VEC4::VEC4()
: x(0.0f)
, y(0.0f)
, z(0.0f)
, w(0.0f)
{
}

VEC4::VEC4(float fx, float fy, float fz, float fw)
: x(fx)
, y(fy)
, z(fz)
, w(fw)
{
}

VEC4::~VEC4()
{
}

VEC4 VEC4::Zero()
{
    return {0.0f, 0.0f, 0.0f, 0.0f};
}

VEC4 VEC4::One()
{
    return { 1.0f, 1.0f, 1.0f, 1.0f};
}

void VEC4::Set(float fx, float fy, float fz, float fw)
{
    x = fx;
    y = fy;
    z = fz;
    w = fw;
}

void VEC4::SetX(float fx) { x = fx; }
void VEC4::SetY(float fy) { y = fy; }
void VEC4::SetZ(float fz) { z = fz; }
void VEC4::SetW(float fw) { w = fw; }

float VEC4::X() const { return x; }
float VEC4::Y() const { return y; }
float VEC4::Z() const { return z; }
float VEC4::W() const { return w; }

VEC4 VEC4::operator+(const VEC4& add) const { return VEC4(x + add.x, y + add.y, z + add.z, w + add.w); }
VEC4 VEC4::operator-(const VEC4& sub) const { return VEC4(x - sub.x, y - sub.y, z - sub.z, w - sub.w); }
VEC4 VEC4::operator*(const VEC4& multi) const { return VEC4(x * multi.x, y * multi.y, z * multi.z, w * multi.w); }
VEC4 VEC4::operator*(const float multi) const { return VEC4(x * multi, y * multi, z * multi, w * multi); }
VEC4 VEC4::operator/(const float div) const { return VEC4(x / div, y / div, z / div, w / div); }

void VEC4::operator+=(const VEC4& add) { *this = (*this) + add; }
void VEC4::operator-=(const VEC4& sub) { *this = (*this) - sub; }
void VEC4::operator*=(const VEC4& multi) { *this = (*this) * multi; }
void VEC4::operator*=(const float multi){ *this = (*this) * multi; }
void VEC4::operator/=(const float div){ *this = (*this) / div; }

float VEC4::Length() const { return sqrtf(x * x + y * y + z * z); }
float VEC4::SqrtLength() const { return x * x + y * y + z * z; }

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