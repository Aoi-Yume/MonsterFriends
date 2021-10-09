//
// Created by 葵ユメ on 2018/09/05.
//


#ifndef AOIYUME_DRAW_COMPONENT_H
#define AOIYUME_DRAW_COMPONENT_H

#include <GLES2/gl2.h>
#include "../Engine/Engine.h"
#include "ComponentBase.h"
#include "../../Math/math_Vector.h"


class DrawComponent : public ComponentBase
{
    typedef ComponentBase Super;

public:
	enum BlendMode {
		eBlend_None = 0,
		eBlend_Alpha,
		eBlend_Multiply,
	};

public:
	DrawComponent(ComponentKind nKind, EntityBase* pEntityBase);
	virtual ~DrawComponent();

public:
	void SetOrtho(bool bOrtho);
	bool IsOrtho() const;

	void SetUseTex(bool bUseTex);
	bool IsUseTex() const;

	void SetAnimation(bool bAnimation);
	bool IsAnimation() const;

	void SetVisible(bool bVisible);
	bool IsVisible() const;

	void SetBlendMode(BlendMode mode);
	BlendMode GetBlendMode() const;

	void SetDiscardFactor(float fFactor);
	float GetDiscardFactor() const;

	void SetUVOffset(const VEC2& offset);
	const VEC2& GetUVOffset() const;
	void AddUVOffset(const VEC2& offset);

	void SetColor(const VEC4& color);
	const VEC4& GetColor() const;

protected:
	GLuint loadShader(GLenum type, const char* pShaderCode);
	void createShader(const char* pVtxSource, const char* pFragSource);
	void destroyShader();

	virtual void setupShaderParam();
	virtual void setupUniformShaderParam();
	virtual void setupTexture(float& fSizeW, float& fSizeH);

	GLuint getVtxShader() const;
	GLuint getFragShader() const;
	GLuint getShaderProgram() const;
private:
	bool 		m_bOrtho;
	bool 		m_bUseTex;
	bool 		m_bAnimation;
	bool 		m_bVisible;
	BlendMode	m_BlendMode;

	float 		m_fDiscardFactor;
	VEC2		m_uvOffset;
	VEC4		m_Color;

	GLuint		m_nVtxShader;
	GLuint		m_nFragShader;
	GLuint		m_nShaderProgram;
};

#endif