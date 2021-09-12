//
// Created by 葵ユメ on 2021/09/11.
//


#include "Button/ButtonManager.h"
#include "Button/SimpleButton.h"
#include "OptionMenu.h"

//==========================================
//==========================================
class OptionChild : public GameEntity
{
	typedef GameEntity Super;
public:
	OptionChild()
			: GameEntity()
			, m_pButtonManager(nullptr)
	{}

	~OptionChild(){
		delete m_pButtonManager;
	}

	void GameEntitySetup(const void* param)
	{
		const std::tuple<const char*, VEC3, const char*> btnList[] = {
				{"image/option_tutorial.png", VEC3(0, 150.0f, 0), SE_LABEL_DECIDE},
				{"image/option_license.png", VEC3(0, -150.0f, 0), SE_LABEL_DECIDE},
				{"image/button_back.png", VEC3(500.0f, 300.0f, 0), SE_LABEL_CANCEL}
		};
		m_pButtonManager = new ButtonManager();
		for(int i = 0; i < sizeof(btnList) / sizeof(btnList[0]); ++i) {
			auto pBtn = m_pButtonManager->CreateButton(std::get<0>(btnList[i]));
			pBtn->SetPosition(std::get<1>(btnList[i]));
			pBtn->SetSelectSELabel(std::get<2>(btnList[i]));
			pBtn->SetVisible(false);
		}
	}

	void EntityUpdate(GameMessage message, const void* param) override
	{
		Super::EntityUpdate(message, param);
		m_pButtonManager->Update(message, param);
	}

	void Open()
	{
		m_pButtonManager->Reset();
		m_pButtonManager->Unlock();
		m_pButtonManager->SetVisible(true);
	}

	void Close()
	{
		m_pButtonManager->Lock();
		m_pButtonManager->SetVisible(false);
	}

	int GetResult() const { return m_pButtonManager->GetDecide(); }

private:
	ButtonManager* m_pButtonManager;
};

//==========================================
//==========================================
OptionMenu::OptionMenu()
: GameEntity()
{
	DEBUG_LOG("Create OptionMenu");
}

OptionMenu::~OptionMenu()
{
}

void OptionMenu::GameEntitySetup(const void* param)
{
	Entity::CreateTransformComponent(this);
	Super::GameEntitySetup(param);

	{
		Entity* pChild = new Entity();
		Entity::CreateLayoutComponent(pChild, "image/option_menu.png")->SetOrtho(true);
		auto pAnimComponent = Entity::CreateAnimationComponent(pChild);
		pAnimComponent->AddAnimation("Open", new SimpleOpenAnimation());
		AddChild(pChild);
	}
	{
		AddChild(new OptionChild());
	}

	SetVisible(false);
}

void OptionMenu::GameEntityUpdate(const void* param)
{
	Super::GameEntityUpdate(param);
}

void OptionMenu::EntityUpdate(GameMessage message, const void* param)
{
	Super::EntityUpdate(message, param);
}

void OptionMenu::Open()
{
	GetChild(0)->SetScale(VEC3(1, 1, 1));
	GetChild(0)->GetComponent<AnimationComponent*>(eComponentKind_Animation)->Play("Open");
	GetChild<OptionChild*>(1)->Open();
}

void OptionMenu::Close()
{
	GetChild(0)->GetComponent<AnimationComponent*>(eComponentKind_Animation)->Play("Open", -1.0f);
	GetChild<OptionChild*>(1)->Close();
}

int OptionMenu::GetResult() const
{
	return GetChild<OptionChild*>(1)->GetResult();
}