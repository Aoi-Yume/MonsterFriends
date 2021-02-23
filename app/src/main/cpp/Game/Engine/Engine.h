//
// Created by 葵ユメ on 2018/08/25.
//


#ifndef AOIYUME_ENGINE_H
#define AOIYUME_ENGINE_H

#include "../../aoiyume_pch.h"
#include "engine_define.h"
#include <GLES2/gl2.h>
#include <android/asset_manager_jni.h>

struct TouchInputInfo;

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

	void SetDestrpyCallBack(const std::function<void()>& callBack);

	void AddRef();
	void SubRef();
	bool IsEmptyRef() const;

	void SetScreenInfo(int nWidth, int nHeight, float fAspect);
	const ScreenInfo& GetScreenInfo() const;

	void SetTouchInputInfo(int nEvent, float fTouchX, float fTouchY, int nPlayerId = -1);
	bool FindDelayTouchInfo(TouchInputInfo& info, int nEvent, int nPlayerId = -1) const;

	void SetCameraComponent(void* pComponent);
	void* GetCameraComponent() const;

	void SetAssetManager(jobject assetMaangerObj);
	void DeleteAssetManager();
	void LoadAsset(const char* pFileName, void** pBuffer, long *pSize, bool bAllocate);

	void ShowSoftwareKeyboard();
	bool IsShowSofrwareKeyboard() const;
	void GetInputText(char* pText, int nSize) const;

	void StartNearbyAdvertising(const char* pConnectName);
	void StopNearbyAdvertising();
	void StartNearbyDiscovery(const char* pConnectName);
	void StopNearbyDiscovery();
	void SendData(const char* pId, jbyte* pData, int nSize);

public:
	static void Create();
	static void Destroy();
	static Engine* GetEngine();

private:
	int fixNetPlayerId(int nPlayerId) const;

private:
	int				m_nRef;
	ScreenInfo		m_screenInfo;

	void*	m_pCameraComponent;

	jobject 		m_AssetManagerObjRef;
	AAssetManager*	m_pAssetMaanger;

	std::function<void()> m_DestroyCallBack;
};

#endif