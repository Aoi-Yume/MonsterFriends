//
// Created by 葵ユメ on 2018/08/25.
//


#ifndef AOIYUME_LAYOUT_COMPONENT_H
#define AOIYUME_LAYOUT_COMPONENT_H

#include <vector>
#include "../Engine/Engine.h"
#include "ComponentBase.h"
#include "DrawComponent.h"
#include "../../Math/math_Vector.h"


class LayoutComponent : public DrawComponent
{
    typedef DrawComponent Super;
public:
	LayoutComponent(EntityBase* pEntityBase);
	virtual ~LayoutComponent();

	void SetResPath(const char* pResPath);
	void ChangeTexture(const char* pResPath);
	
	virtual GameMessageResult Update(GameMessage message, const void* param) override;

protected:
	virtual void setupShader();
	virtual void setupShaderParam() override;
	virtual void setupUniformShaderParam() override;
	virtual void setupTexture(float& fSizeW, float& fSizeH) override;
protected:
	void setup();
	void updateAnim();
	void draw();
	void destroy();

	bool isCreated() const;
	void getVertex(float fWidth, float fHeight, void* pBuffer);

	GLuint getVertexBufferId() const;
	GLuint getTextureId() const;
	std::vector<float>& getVertexBuffer();
protected:
	GLint 		m_nTransformLocation;
	GLint 		m_nViewportInvLocation;
	GLint 		m_nViewProjLocation;
	GLint 		m_nColorLocation;

private:
	bool		m_bCreated;
	char 		m_cResPath[64];
	GLuint 		m_nVtxBuffer;
	GLuint 		m_nIdxBuffer;

	GLuint 		m_nTexId;

	GLuint 		m_nVtxLocation;
	GLuint 		m_nUvLocation;

	GLint 		m_nSamplerLocation;

	std::vector<float>		m_aVtxBuffer;
	std::vector<short>		m_aIndexBuffer;
};

class TextImageComponent : public LayoutComponent
{
	typedef LayoutComponent Super;
public:
	TextImageComponent(EntityBase* pEnttiyBase);
	virtual ~TextImageComponent();

	void SetText(const char* pText, float fSizeMultiple = 1.0f);
	void SetFontSize(const int nFontSize);
	void SetFontColor(const VEC4& fontColor);
	void SetBackColor(const VEC4& backColor);
	void SetOutline(bool bOutline);
	void SetOutlineSize(int nOutlineSize);
	void SetOutlineColor(const VEC4& outlineColor);
protected:
	void setupShader() override;
	//virtual void setupShaderParam() override;
	void setupUniformShaderParam() override;
	virtual void setupTexture(float& fSizeW, float& fSizeH) override;

private:
	bool 				m_bOutLine;
	int 				m_nFontSize;
	int 				m_nOutlineSize;
	VEC4				m_FontColor;
	VEC4				m_BackColor;
	VEC4				m_OutlineColor;
	std::string			m_Text;
};

class DebugSquareImageComponent : public LayoutComponent{
	typedef LayoutComponent Super;
public:
	DebugSquareImageComponent(EntityBase* pEntityBase);
	virtual ~DebugSquareImageComponent();

	void SetSize(float fSizeW, float fSizeH);
protected:
	void setupShader() override;
	void setupUniformShaderParam() override;
	virtual void setupTexture(float& fSizeW, float& fSizeH) override;

private:
	float 		m_fSizeW;
	float 		m_fSizeH;
};
#endif