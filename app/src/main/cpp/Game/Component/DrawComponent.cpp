//
// Created by 葵ユメ on 2018/09/05.
//


#include "DrawComponent.h"

DrawComponent::DrawComponent(ComponentKind nKind, EntityBase* pEntityBase)
: Super(nKind, pEntityBase)
, m_bOrtho(false)
, m_bUseTex(false)
, m_bVisible(true)
, m_fAlpha(1.0f)
, m_nVtxShader(0)
, m_nFragShader(0)
, m_nShaderProgram(0)
{
	DEBUG_LOG("Call DrawComponent Constructor");
	/*
    jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_LAYOUT_COMPONENT);
    jmethodID  methodID = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_LAYOUT_COMPONENT, "<init>");
    jobject pObj = GetEnv()->AllocObject(classID);

    m_pObj = GetEnv()->NewGlobalRef(pObj);
    jfieldID  fieldID = ObjectLoader::GetObjectLoader()->GetFieledID(CLASS_NAME_LAYOUT_COMPONENT, "m_nInstance");
    GetEnv()->SetLongField(m_pObj, fieldID, reinterpret_cast<long>(this));

    GetEnv()->CallVoidMethod(m_pObj, methodID);
	*/
}

DrawComponent::~DrawComponent()
{
    DEBUG_LOG("Call DrawComponent Deconstructor");
   // GetEnv()->DeleteGlobalRef(m_pObj);
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

void DrawComponent::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

bool DrawComponent::IsVisible() const
{
	return m_bVisible;
}

void DrawComponent::SetAlpha(float fAlpha)
{
	m_fAlpha = fAlpha;
}

float DrawComponent::GetAlpha() const
{
	return m_fAlpha;
}

GLuint DrawComponent::loadShader(GLenum type, const GLchar* pShaderCode)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &pShaderCode, NULL);
	glCompileShader(shader);
	return shader;
}

void DrawComponent::createShader(const char* pVtxSource, const char* pFragSource)
{
	m_nVtxShader = loadShader(GL_VERTEX_SHADER, pVtxSource);
	assert(m_nVtxShader != 0);
	m_nFragShader = loadShader(GL_FRAGMENT_SHADER, pFragSource);
	assert(m_nFragShader != 0);

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