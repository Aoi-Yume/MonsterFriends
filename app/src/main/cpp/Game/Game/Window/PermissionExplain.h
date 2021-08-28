//
// Created by 葵ユメ on 2021/08/27.
//

#ifndef MONSTERFRIENDS_PERMISSIONEXPLAIN_H
#define MONSTERFRIENDS_PERMISSIONEXPLAIN_H


#include "GameEntity.h"

class ButtonManager;

class PermissionExplain : public GameEntity
{
	typedef GameEntity Super;
public:

public:
	PermissionExplain();
	virtual ~PermissionExplain();

public:
	void GameEntitySetup(const void* param) override;
	void GameEntityUpdate(const void* param) override;
	void EntityUpdate(GameMessage message, const void* param) override;

public:
	void SetVisible(bool bVisible);

	void AddPermissionExplain(const char* pName, const char* pResPath);
	void Open(const char* pName);
	void Close(const char* pName);

	bool IsNext() const;
private:

private:
	ButtonManager*	m_pButtonManager;
	std::unordered_map<std::string, Entity*> m_aPermissionExplain;
};


#endif //MONSTERFRIENDS_PERMISSIONEXPLAIN_H
