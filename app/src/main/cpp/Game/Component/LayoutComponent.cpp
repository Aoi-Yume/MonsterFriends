//
// Created by 葵ユメ on 2018/08/25.
//


#include <stdio.h>
#include "LayoutComponent.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "CameraComponent.h"
#include "vector"

#define OPENGL_VERTION 2

#if OPENGL_VERTION >= 3
#define STR(s) #s
#define STRV(s) STR(s)

#define SHADER_ATTR_POS 0
#define SHADER_ATTR_UV 1
#define SHADER_UNIFORM_TRANSFORM 2
#define SHADER_UNIFORM_VIEW_PROJ 3
#define SHADER_UNIFORM_VIEWPORT_INV 4
#define SHADER_UNIFORM_SAMPLER 5
#define SHADER_UNIFORM_ALPHA 6
#define SHADER_UNIFORM_COLOR 7

static const char* s_vtxShaderSrc =
		"#version 300 es\n"
		"precision mediump float; \n"
  		"layout(location=" STRV(SHADER_ATTR_POS) ") in vec3 vPosition; \n"
		"layout(location=" STRV(SHADER_ATTR_UV) ") in vec2 attr_uv; \n"
		"layout(location=" STRV(SHADER_UNIFORM_TRANSFORM) ") uniform mat4 uni_transform; \n"
		"layout(location=" STRV(SHADER_UNIFORM_VIEW_PROJ)  ") uniform mat4 uni_viewProj; \n"
		"layout(location=" STRV(SHADER_UNIFORM_VIEWPORT_INV) ") uniform mat4 uni_viewportInv; \n"
		"out vec2 out_uv; \n"
		"void main() { \n"
		"gl_Position =  (uni_viewProj * uni_transform) * (uni_viewportInv * vec4(vPosition, 1.0f)); \n"
		"out_uv = attr_uv; \n"
		"}";

static const char* s_vtx2DShaderSrc =
		"#version 300 es\n"
		"precision mediump float; \n"
		"layout(location=" STRV(SHADER_ATTR_POS) ") in vec3 vPosition; \n"
		"layout(location=" STRV(SHADER_ATTR_UV) ") in vec2 attr_uv; \n"
		"layout(location=" STRV(SHADER_UNIFORM_TRANSFORM) ") uniform mat4 uni_transform; \n"
		"layout(location=" STRV(SHADER_UNIFORM_VIEW_PROJ) ") uniform mat4 uni_viewProj; \n"
		"out vec2 vary_uv; \n"
		"void main() { \n"
		"gl_Position =  (uni_viewProj * uni_transform) * vec4(vPosition, 1.0f); \n"
		"vary_uv = attr_uv; \n"
		"}";

static const char* s_fragShaderSrc =
		"#version 300 es\n"
		"precision mediump float; \n"
  		"layout(location=" STRV(SHADER_UNIFORM_SAMPLER) ") uniform sampler2D uni_sampler; \n"
		"layout(location=" STRV(SHADER_UNIFORM_ALPHA) ") uniform float uni_Alpha; \n"
		"in vec2 vary_uv; \n"
		"out vec4 out_color; \n"
		"void main() { \n"
		"out_color = texture(uni_sampler, vec2(vary_uv.x, 1.0f - vary_uv.y)); \n"
		"out_color.w *= uni_Alpha; \n"
		"if(out_color.w <= 0.1f) { discard; } \n"
		"}";

static const char* s_fragColorShaderSrc =
		"#version 300 es\n"
		"precision mediump float; \n"
		"layout(location=" STRV(SHADER_UNIFORM_COLOR) ") uniform vec4 uni_simpleColor; \n"
		"layout(location=" STRV(SHADER_UNIFORM_ALPHA) ") uniform float uni_Alpha; \n"
		"in vec2 vary_uv; \n"
  		"out vec4 out_color; \n"
		"void main() { \n"
		"out_color = uni_simpleColor; \n"
		"out_color.w *= uni_Alpha; \n"
		"if(out_color.w <= 0.1f) { discard; } \n"
		"}";
#else
static const char* s_vtxShaderSrc =
		"precision mediump float; \n"
		"attribute vec3 vPosition; \n"
		"attribute vec2 attr_uv; \n"
  		"uniform mat4 uni_transform; \n"
		"uniform mat4 uni_viewProj; \n"
  		"uniform mat4 uni_viewportInv; \n"
		"varying vec2 vary_uv; \n"
		"void main() { \n"
		"gl_Position =  (uni_viewProj * uni_transform) * (uni_viewportInv * vec4(vPosition, 1.0f)); \n"
		"vary_uv = attr_uv; \n"
		"}";

static const char* s_vtx2DShaderSrc =
		"precision mediump float; \n"
		"attribute vec3 vPosition; \n"
		"attribute vec2 attr_uv; \n"
  		"uniform mat4 uni_transform; \n"
		"uniform mat4 uni_viewProj; \n"
		"varying vec2 vary_uv; \n"
		"void main() { \n"
		"gl_Position =  (uni_viewProj * uni_transform) * vec4(vPosition, 1.0f); \n"
		"vary_uv = attr_uv; \n"
		"}";

static const char* s_fragShaderSrc =
		"precision mediump float; \n"
  		"uniform sampler2D uni_sampler; \n"
		"uniform float uni_Alpha; \n"
		"varying vec2 vary_uv; \n"
		"void main() { \n"
		"gl_FragColor = texture2D(uni_sampler, vec2(vary_uv.x, 1.0f - vary_uv.y)); \n"
		"gl_FragColor.w *= uni_Alpha; \n"
		"if(gl_FragColor.w <= 0.1f) { discard; } \n"
		"}";

static const char* s_fragColorShaderSrc =
		"precision mediump float; \n"
  		"uniform vec4 uni_simpleColor; \n"
		"uniform float uni_Alpha; \n"
		"varying vec2 vary_uv; \n"
		"void main() { \n"
		"gl_FragColor = uni_simpleColor; \n"
		"gl_FragColor.w *= uni_Alpha; \n"
		"if(gl_FragColor.w <= 0.1f) { discard; } \n"
		"}";
#endif

//------------------------------------------
//------------------------------------------
LayoutComponent::LayoutComponent(EntityBase* pEntityBase)
: Super(eComponentKind_Layout, pEntityBase)
, m_bCreated(false)
{
	DEBUG_LOG("Call LayoutComponent Constructor");
	memset(m_cResPath, 0, sizeof(m_cResPath));
}

//------------------------------------------
//------------------------------------------
LayoutComponent::~LayoutComponent()
{
    DEBUG_LOG("Call LayoutComponent Deconstructor");
}

//------------------------------------------
//------------------------------------------
void LayoutComponent::SetResPath(const char *pResPath){
	const size_t nStrlen = strlen(pResPath);
	assert(nStrlen < sizeof(m_cResPath));
	strcpy(m_cResPath, pResPath);
	SetUseTex(true);
}

//------------------------------------------
//------------------------------------------
GameMessageResult LayoutComponent::Update(GameMessage message, const void* param)
{
	switch(message)
	{
		case eGameMessage_Setup:{
			setup();
			break;
		}
		case eGameMessage_Draw:{
			draw();
			break;
		}
		case eGameMessage_Pause:{
			break;
		}
		case eGameMessage_Destroy:{
			destroy();
			break;
		}
		default:{
			break;
		}
	}
	return eGameMessageResult_Break;
}

//------------------------------------------
//------------------------------------------
void LayoutComponent::setup()
{
	setupShader();

	float fSizeW = 100.0f;
	float fSizeH = 100.0f;
	glGenTextures(1, &m_nTexId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, m_nTexId);

	setupTexture(fSizeW, fSizeH);

	// 設定しないとテクスチャが表示されないので注意
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint buffers[2];
	glGenBuffers(2, buffers);
	m_nVtxBuffer = buffers[0];
	m_nIdxBuffer = buffers[1];

	const uint vtxNum = 5 * 4;
	m_aVtxBuffer.clear();
	m_aVtxBuffer.resize(vtxNum);
	getVertex(fSizeW, fSizeH, m_aVtxBuffer.data());

	// 頂点データバインド
	glBindBuffer(GL_ARRAY_BUFFER, m_nVtxBuffer);
	glBufferData(GL_ARRAY_BUFFER, vtxNum * sizeof(float), m_aVtxBuffer.data(), GL_STATIC_DRAW);

	// バインド解除
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// インデックス
	const uint idxNum = 6;
	short idx[] = {0, 1, 2, 2, 1, 3};
	m_aIndexBuffer.clear();
	m_aIndexBuffer.resize(idxNum);
	for(int i = 0; i < idxNum; ++i) {
		m_aIndexBuffer.at(i) = idx[i];
	}

	// インデックスバインド
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIdxBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxNum * sizeof(short), m_aIndexBuffer.data(), GL_STATIC_DRAW);
	// バインド解除
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#if OPENGL_VERTION >= 3
	m_nViewportInvLocation = SHADER_UNIFORM_VIEWPORT_INV;
	m_nTransformLocation = SHADER_UNIFORM_TRANSFORM;
	m_nViewProjLocation = SHADER_UNIFORM_VIEW_PROJ;
	m_nSamplerLocation = SHADER_UNIFORM_SAMPLER;
	m_nAlphaLocation = SHADER_UNIFORM_ALPHA;
#else
	m_nTransformLocation = glGetUniformLocation(getShaderProgram(), "uni_transform");
	m_nViewProjLocation = glGetUniformLocation(getShaderProgram(), "uni_viewProj");
	m_nViewportInvLocation = glGetUniformLocation(getShaderProgram(), "uni_viewportInv");
	m_nSamplerLocation = glGetUniformLocation(getShaderProgram(), "uni_sampler");
	m_nAlphaLocation = glGetUniformLocation(getShaderProgram(), "uni_Alpha");
#endif

	m_bCreated = true;
}

//------------------------------------------
//------------------------------------------
void LayoutComponent::setupShader()
{
	const char* pVtxShaderSrc = (IsOrtho()) ? s_vtx2DShaderSrc : s_vtxShaderSrc;
	const char* pFragShaderSrc = (IsUseTex()) ? s_fragShaderSrc : s_fragColorShaderSrc;
	createShader(pVtxShaderSrc, pFragShaderSrc);
}

//------------------------------------------
//------------------------------------------
void LayoutComponent::setupShaderParam()
{
#if OPENGL_VERTION >= 3
	m_nVtxLocation = SHADER_ATTR_POS;
	m_nUvLocation = SHADER_ATTR_UV;
#else
	m_nVtxLocation = 0;
	m_nUvLocation = 1;
	glBindAttribLocation(getShaderProgram(), m_nVtxLocation, "vPosition");
	glBindAttribLocation(getShaderProgram(), m_nUvLocation, "attr_uv");
#endif
}

//------------------------------------------
//------------------------------------------
void LayoutComponent::setupUniformShaderParam()
{
	TransformComponent *pTransform = reinterpret_cast<TransformComponent*>(GetEntityBase()->GetComponent(eComponentKind_Transform));
	CameraComponent *pCamera = reinterpret_cast<CameraComponent*>(Engine::GetEngine()->GetCameraComponent());
	// リンク情報で親を辿ってトランスフォーム設定
	if(GetEntityBase()->GetLinkTransform()) {
		glUniformMatrix4fv(m_nTransformLocation, 1, GL_FALSE, pTransform->GetWorldMatrix()->Get());
	}else{
		glUniformMatrix4fv(m_nTransformLocation, 1, GL_FALSE, pTransform->GetLocalMatrix()->Get());
	}
	if( !IsOrtho() ) {
		glUniformMatrix4fv(m_nViewportInvLocation, 1, GL_FALSE, pCamera->GetViewportInv().Get());
	}
	// 正確には正射影と2Dは違うけど、レイアウトなのでひとまず
	const float* pCamMtx = (IsOrtho()) ? pCamera->Get2D().Get() : pCamera->GetViewProjection().Get();
	glUniformMatrix4fv(m_nViewProjLocation, 1, GL_FALSE, pCamMtx);

	glUniform1f(m_nAlphaLocation, GetAlpha());
}

//------------------------------------------
//------------------------------------------
void LayoutComponent::setupTexture(float &fSizeW, float &fSizeH)
{
	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_UTILITY);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_UTILITY, "LoadImage");
	jmethodID  methodID_2 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_UTILITY, "GetImageSize");
	jstring resPath = GetEnv()->NewStringUTF(m_cResPath);
	jobject imageBufferObj = GetEnv()->CallStaticObjectMethod(classID, methodID_1, resPath);
	jobject imageSizeObj = GetEnv()->CallStaticObjectMethod(classID, methodID_2, resPath);
	jint  imageSize[2] = {0, 0};
	GetEnv()->GetIntArrayRegion((jintArray)imageSizeObj, 0, 2, imageSize);
	const void* pBuffer = GetEnv()->GetDirectBufferAddress(imageBufferObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageSize[0], imageSize[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, pBuffer);
	fSizeW = (float)imageSize[0];
	fSizeH = (float)imageSize[1];
}

//------------------------------------------
//------------------------------------------
void LayoutComponent::draw() {
	if(!IsVisible()){ return; }

	glUseProgram(getShaderProgram());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE, GL_ZERO);
	setupUniformShaderParam();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_nTexId);
	// GL_TEXTURE0 を使うように指定
	glUniform1i(m_nSamplerLocation, 0);

	glEnableVertexAttribArray(m_nVtxLocation);
	glEnableVertexAttribArray(m_nUvLocation);

	glBindBuffer(GL_ARRAY_BUFFER, m_nVtxBuffer);
	// 0固定, Float数, GL_FLOAT固定, 正規化, 一つ当たり頂点サイズ, オフセット
	glVertexAttribPointer(m_nVtxLocation, 3, GL_FLOAT, GL_FALSE, 20, (const void*)0);
	glVertexAttribPointer(m_nUvLocation, 2, GL_FLOAT, GL_FALSE, 20, (const void*)12);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIdxBuffer);
	// タイプ, 頂点数, GL_SUNSIGNED_SHORT固定, オフセット
	glDrawElements(GL_TRIANGLES, (uint)m_aIndexBuffer.size(), GL_UNSIGNED_SHORT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableVertexAttribArray(m_nVtxLocation);
	glDisableVertexAttribArray(m_nUvLocation);

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

//------------------------------------------
//------------------------------------------
void LayoutComponent::destroy()
{
	DEBUG_LOG("Destroy Layout Component");
	destroyShader();
	glDeleteTextures(1, &m_nTexId);
	glDeleteBuffers(1, &m_nVtxBuffer);
	glDeleteBuffers(1, &m_nIdxBuffer);
	m_aVtxBuffer.clear();
	m_aIndexBuffer.clear();
}

//------------------------------------------
//------------------------------------------
bool LayoutComponent::isCreated() const
{
	return m_bCreated;
}

//------------------------------------------
//------------------------------------------
void LayoutComponent::getVertex(float fWidth, float fHeight, void* pBuffer)
{
	const ScreenInfo& info = Engine::GetEngine()->GetScreenInfo();
	float vertices[] = {
			// x, y, z, u, v
			-fWidth * 0.5f, fHeight * 0.5f, 0.0f, 0.0f, 1.0f,
			-fWidth * 0.5f, -fHeight * 0.5f, 0.0f, 0.0f, 0.0f,
			fWidth * 0.5f, fHeight * 0.5f, 0.0f, 1.0f, 1.0f,
			fWidth * 0.5f, -fHeight * 0.5f, 0.0f, 1.0f, 0.0f,
	};
	memcpy(pBuffer, vertices, sizeof(vertices));
}

//------------------------------------------
//------------------------------------------
GLuint LayoutComponent::getVertexBufferId() const
{
	return m_nVtxBuffer;
}

//------------------------------------------
//------------------------------------------
GLuint LayoutComponent::getTextureId() const
{
	return m_nTexId;
}

//------------------------------------------
//------------------------------------------
std::vector<float>& LayoutComponent::getVertexBuffer()
{
	return m_aVtxBuffer;
}


//------------------------------------------
//------------------------------------------
TextImageComponent::TextImageComponent(EntityBase* pEntityBase)
: Super(pEntityBase)
, m_bOutLine(false)
, m_nFontSize(32)
, m_nOutlineSize(1)
, m_FontColor(0.0f, 0.0f, 0.0f, 1.0f)
, m_BackColor(0.0f, 0.0f, 0.0f, 0.0f)
, m_OutlineColor(0.0f, 0.0f, 0.0f, 1.0f)
, m_Text("")
{
}

//------------------------------------------
//------------------------------------------
TextImageComponent::~TextImageComponent()
{
}

//------------------------------------------
//------------------------------------------
void TextImageComponent::SetText(const char* pText, float fSizeMultiple)
{
	m_Text = pText;
	if(!isCreated()){ return; }

	float fSizeW = 100.0f;
	float fSizeH = 100.0f;
	// TODO 1フレームに一回呼ばれるように場所を変える
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, getTextureId());

	setupTexture(fSizeW, fSizeH);

	// 設定しないとテクスチャが表示されないので注意
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);

	auto aVtxBuffer = getVertexBuffer();
	const uint vtxNum = 5 * 4;
	aVtxBuffer.clear();
	aVtxBuffer.resize(vtxNum);
	getVertex(fSizeW * fSizeMultiple, fSizeH * fSizeMultiple, aVtxBuffer.data());

	// 頂点データバインド
	glBindBuffer(GL_ARRAY_BUFFER, getVertexBufferId());
	glBufferData(GL_ARRAY_BUFFER, vtxNum * sizeof(float), aVtxBuffer.data(), GL_STATIC_DRAW);

	// バインド解除
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//------------------------------------------
//------------------------------------------
void TextImageComponent::SetFontSize(const int nFontSize)
{
	m_nFontSize = nFontSize;
}

//------------------------------------------
//------------------------------------------
void TextImageComponent::SetFontColor(const VEC4 &fontColor)
{
	m_FontColor = fontColor;
}

//------------------------------------------
//------------------------------------------
void TextImageComponent::SetBackColor(const VEC4 &backColor)
{
	m_BackColor = backColor;
}

//------------------------------------------
//------------------------------------------
void TextImageComponent::SetOutline(bool bOutline)
{
	m_bOutLine = bOutline;
}

//------------------------------------------
//------------------------------------------
void TextImageComponent::SetOutlineSize(int nOutlineSize)
{
	m_nOutlineSize = nOutlineSize;
}

//------------------------------------------
//------------------------------------------
void TextImageComponent::SetOutlineColor(const VEC4& outlineColor)
{
	m_OutlineColor = outlineColor;
}

//------------------------------------------
//------------------------------------------
void TextImageComponent::setupShader()
{
	SetUseTex(true);
	Super::setupShader();
}


//------------------------------------------
//------------------------------------------
void TextImageComponent::setupUniformShaderParam()
{
	Super::setupUniformShaderParam();
}

//------------------------------------------
//------------------------------------------
void TextImageComponent::setupTexture(float &fSizeW, float &fSizeH)
{
	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_UTILITY);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_UTILITY, "CreateFontImage");
	jmethodID  methodID_2 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_UTILITY, "GetTextImageSize");

	jstring text = GetEnv()->NewStringUTF(m_Text.c_str());
	jobject imageBufferObj = GetEnv()->CallStaticObjectMethod(classID, methodID_1, text, m_nFontSize);
	jfloatArray imageInfoArrayObj = (jfloatArray)GetEnv()->CallStaticObjectMethod(classID, methodID_2, text, m_nFontSize);
	jboolean jTrue = JNI_TRUE;
	jfloat* pTextImageInfo = GetEnv()->GetFloatArrayElements(imageInfoArrayObj, &jTrue);
	jint imageSize[2] = {(int)*(pTextImageInfo + 0), (int)*(pTextImageInfo + 1)};

	void* pBuffer = GetEnv()->GetDirectBufferAddress(imageBufferObj);
	uint size = (uint)GetEnv()->GetDirectBufferCapacity(imageBufferObj);
	unsigned char* pColorData = reinterpret_cast<unsigned char*>(pBuffer);
	// インデックス計算ラムダ式
	auto getIndex = [&imageSize](uint i, uint j)->uint{
		return (i * (uint)imageSize[1] + j) * 4U;
	};
	// アウトライン判定ラムダ式
	auto isOutline = [&getIndex, &pColorData, &size, this](uint i, uint j)->bool{
		for(int offset = m_nOutlineSize; offset > 0; offset--) {
			const uint upIdx = std::min(getIndex(i - offset, j), size - 1);
			const uint leftIdx = std::min(getIndex(i, j - offset), size - 1);
			const uint rightIdx = std::min(getIndex(i, j + offset), size - 1);
			const uint downIdx = std::min(getIndex(i + offset, j), size - 1);
			if(pColorData[upIdx] > 0 || pColorData[leftIdx] > 0 ||
			   pColorData[rightIdx] > 0 || pColorData[downIdx] > 0){
				return true;
			}
		}
		return false;
	};
	for(int i = 0; i < imageSize[1]; ++i){
		for(int j = 0; j < imageSize[0]; ++j){
			const uint baseIdx = getIndex((uint)i, (uint)j);
			unsigned char alpha = pColorData[baseIdx + 0];
			// 透明部分は背景色またはアウトライン
			if(alpha <= 0){
				// アウトライン
				// TODO アウトライン色のRに0以外が入るとバグるので修正
				if(m_bOutLine && isOutline((uint)i, (uint)j)){
					pColorData[baseIdx + 0] = static_cast<unsigned char>(m_OutlineColor.GetX() * 255.0f);
					pColorData[baseIdx + 1] = static_cast<unsigned char>(m_OutlineColor.GetY() * 255.0f);
					pColorData[baseIdx + 2] = static_cast<unsigned char>(m_OutlineColor.GetZ() * 255.0f);
					pColorData[baseIdx + 3] = static_cast<unsigned char>(m_OutlineColor.GetW() * 255.0f);
				}
				// 背景色
				else{
					pColorData[baseIdx + 0] = static_cast<unsigned char>(m_BackColor.GetX() * 255.0f);
					pColorData[baseIdx + 1] = static_cast<unsigned char>(m_BackColor.GetY() * 255.0f);
					pColorData[baseIdx + 2] = static_cast<unsigned char>(m_BackColor.GetZ() * 255.0f);
					pColorData[baseIdx + 3] = static_cast<unsigned char>(m_BackColor.GetW() * 255.0f);
				}
			}
			// 文字部分
			else{
				pColorData[baseIdx + 0] = static_cast<unsigned char>(m_FontColor.GetX() * 255.0f);
				pColorData[baseIdx + 1] = static_cast<unsigned char>(m_FontColor.GetY() * 255.0f);
				pColorData[baseIdx + 2] = static_cast<unsigned char>(m_FontColor.GetZ() * 255.0f);
				pColorData[baseIdx + 3] = static_cast<unsigned char>(m_FontColor.GetW() * 255.0f);
			}
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageSize[0], imageSize[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, pBuffer);
	fSizeW = (float)imageSize[0];
	fSizeH = (float)imageSize[1];
	//DEBUG_LOG_A("TextSize:[%.2f, %.2f]", fSizeW, fSizeH);
}

//------------------------------------------
//------------------------------------------
DebugSquareImageComponent::DebugSquareImageComponent(EntityBase* pEntityBase)
: Super(pEntityBase)
, m_fSizeW(100.0f)
, m_fSizeH(100.0f)
, m_Color(0.0f, 0.0f, 0.0f, 1.0f)
{

}

//------------------------------------------
//------------------------------------------
DebugSquareImageComponent::~DebugSquareImageComponent()
{
}

//------------------------------------------
//------------------------------------------
void DebugSquareImageComponent::SetSize(float fSizeW, float fSizeH)
{
	m_fSizeW = fSizeW;
	m_fSizeH = fSizeH;
}

//------------------------------------------
//------------------------------------------
void DebugSquareImageComponent::SetColor(const VEC4& color)
{
	m_Color = color;
}

//------------------------------------------
//------------------------------------------
void DebugSquareImageComponent::setupShader()
{
	SetUseTex(false);
	Super::setupShader();
}

//------------------------------------------
//------------------------------------------
void DebugSquareImageComponent::setupUniformShaderParam()
{
	Super::setupUniformShaderParam();
#if OPENGL_VERTION >= 3
	glUniform4fv(SHADER_UNIFORM_COLOR, 1, (const GLfloat*)&m_Color);
#else
	glUniform4fv(glGetUniformLocation(getShaderProgram(), "uni_simpleColor"), 1, (const GLfloat*)&m_Color);
#endif
}

//------------------------------------------
//------------------------------------------
void DebugSquareImageComponent::setupTexture(float &fSizeW, float &fSizeH)
{
	fSizeW = m_fSizeW;
	fSizeH = m_fSizeH;
}