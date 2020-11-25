//
// Created by 葵ユメ on 2018/08/25.
//


#ifndef AOIYUME_ENGINE_H
#define AOIYUME_ENGINE_H

#include "../../aoiyume_pch.h"
#include "engine_define.h"
#include <GLES2/gl2.h>
#include <android/asset_manager_jni.h>

struct TouchInputInfo{
	int		m_nPreTouchEvent;
	int		m_nTouchEvent;
	int 	m_nTouchCnt;
	float	m_fTouchX;
	float	m_fTouchY;
	
	TouchInputInfo()
	: m_nPreTouchEvent(0)
	, m_nTouchEvent(-1)
	, m_nTouchCnt(0)
	, m_fTouchX(0.0f)
	, m_fTouchY(0.0f)
	{
	}
};

struct ScreenInfo{
	int 	m_nScreenX;
	int 	m_nScreenY;
	float 	m_fAspect;

	ScreenInfo()
	: m_nScreenX(1920)
	, m_nScreenY(1080)
  	, m_fAspect(1920.0f / 1080.0f)
	{
	}
};

class Engine
{
public:
	Engine();
	virtual ~Engine();
	
	void AddRef();
	void SubRef();
	bool IsEmptyRef() const;

	void SetScreenInfo(int nWidth, int nHeight, float fAspect);
	const ScreenInfo& GetScreenInfo() const;

	void SetTouchInputInfo(int nEvent, float fTouchX, float fTouchY);
	const TouchInputInfo& GetTouchInputInfo() const;
	void CheckTouchUpdate();
	void ResetTouchEvent();

	void SetCameraComponent(void* pComponent);
	void* GetCameraComponent() const;

	void SetAssetManager(jobject assetMaangerObj);
	void DeleteAssetManager();
	void LoadAsset(const char* pFileName, void** pBuffer, long *pSize, bool bAllocate);

	void ShowSoftwareKeyboard();
	bool IsShowSofrwareKeyboard() const;
public:
	static void Create();
	static void Destroy();
	static Engine* GetEngine();

private:
	int				m_nRef;
	TouchInputInfo	m_touchInputInfo;
	ScreenInfo		m_screenInfo;

	void*	m_pCameraComponent;

	jobject 		m_AssetManagerObjRef;
	AAssetManager*	m_pAssetMaanger;
};

#endif