//
// Created by 葵ユメ on 2018/09/05.
//


#ifndef AOIYUME_DRAW_COMPONENT_H
#define AOIYUME_DRAW_COMPONENT_H

#include <GLES2/gl2.h>
#include "../Engine/Engine.h"
#include "ComponentBase.h"


class DrawComponent : public ComponentBase
{
    typedef ComponentBase Super;
public:
	DrawComponent(ComponentKind nKind, EntityBase* pEntityBase);
	virtual ~DrawComponent();

public:
	void SetOrtho(bool bOrtho);
	bool IsOrtho() const;

	void SetUseTex(bool bUseTex);
	bool IsUseTex() const;

	void SetVisible(bool bVisible);
	bool IsVisible() const;

	void SetAlpha(float fAlpha);
	float GetAlpha() const;

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
	bool 		m_bVisible;

	float 		m_fAlpha;

	GLuint		m_nVtxShader;
	GLuint		m_nFragShader;
	GLuint		m_nShaderProgram;
};

#endif