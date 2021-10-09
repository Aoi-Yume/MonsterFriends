//
// Created by 葵ユメ on 2018/09/05.
//


#include "DrawComponent.h"

DrawComponent::DrawComponent(ComponentKind nKind, EntityBase* pEntityBase)
: Super(nKind, pEntityBase)
, m_bOrtho(false)
, m_bUseTex(false)
, m_bAnimation(false)
, m_bVisible(true)
, m_BlendMode(eBlend_Alpha)
, m_fDiscardFactor(0.1f)
, m_Color(1, 1, 1, 1)
, m_nVtxShader(0)
, m_nFragShader(0)
, m_nShaderProgram(0)
{
//	DEBUG_LOG("Call DrawComponent Constructor");
}

DrawComponent::~DrawComponent()
{
//    DEBUG_LOG("Call DrawComponent Deconstructor");
}

void DrawComponent::SetOrtho(bool bOrtho)
{
	m_bOrtho = bOrtho;
}

bool DrawComponent::IsOrtho() const
{
	return m_bOrtho;
}

void DrawComponent::SetUseTex(bool bUseTex)
{
	m_bUseTex = bUseTex;
}

bool DrawComponent::IsUseTex() const
{
	return m_bUseTex;
}

void DrawComponent::SetAnimation(bool bAnimation)
{
	m_bAnimation = bAnimation;
}

bool DrawComponent::IsAnimation() const
{
	return m_bAnimation;
}

void DrawComponent::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

bool DrawComponent::IsVisible() const
{
	return m_bVisible;
}

void DrawComponent::SetBlendMode(BlendMode mode)
{
	m_BlendMode = mode;
}
DrawComponent::BlendMode DrawComponent::GetBlendMode() const
{
	return m_BlendMode;
}

void DrawComponent::SetDiscardFactor(float fFactor)
{
	m_fDiscardFactor = fFactor;
}

float DrawComponent::GetDiscardFactor() const
{
	return m_fDiscardFactor;
}

void DrawComponent::SetUVOffset(const VEC2& offset)
{
	m_uvOffset = offset;
}

const VEC2& DrawComponent::GetUVOffset() const
{
	return m_uvOffset;
}

void DrawComponent::AddUVOffset(const VEC2& offset)
{
	m_uvOffset += offset;
}


void DrawComponent::SetColor(const VEC4& color)
{
	m_Color = color;
}

const VEC4& DrawComponent::GetColor() const
{
	return m_Color;
}

GLuint DrawComponent::loadShader(GLenum type, const GLchar* pShaderCode)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &pShaderCode, NULL);
	glCompileShader(shader);
#if _AOIYUME_DEBUG_
	char log[4096];
	GLsizei length = 0;
	glGetShaderInfoLog(shader, sizeof(log), &length, log);
	if(length > 0) {
		DEBUG_LOG("---- Shader Log ----\n");
		DEBUG_LOG_A("%s\n", log);
		DEBUG_LOG("--------------------\n");
	}
#endif
	return shader;
}

void DrawComponent::createShader(const char* pVtxSource, const char* pFragSource)
{
	m_nVtxShader = loadShader(GL_VERTEX_SHADER, pVtxSource);
	ASSERT(m_nVtxShader != 0);
	m_nFragShader = loadShader(GL_FRAGMENT_SHADER, pFragSource);
	ASSERT(m_nFragShader != 0);

	m_nShaderProgram = glCreateProgram();
	glAttachShader(m_nShaderProgram, m_nVtxShader);
	glAttachShader(m_nShaderProgram, m_nFragShader);
	setupShaderParam();
	glLinkProgram(m_nShaderProgram);
}

void DrawComponent::destroyShader()
{
	glDeleteProgram(m_nShaderProgram);
	glDeleteShader(m_nFragShader);
	glDeleteShader(m_nVtxShader);
}

void DrawComponent::setupShaderParam()
{
	// 継承先で実装
}

void DrawComponent::setupUniformShaderParam()
{
	// 継承先で実装
}

void DrawComponent::setupTexture(float &fSizeW, float &fSizeH)
{
	// 継承先で実装
}

GLuint DrawComponent::getVtxShader() const
{
	return m_nVtxShader;
}

GLuint DrawComponent::getFragShader() const
{
	return m_nFragShader;
}

GLuint DrawComponent::getShaderProgram() const
{
	return m_nShaderProgram;
}