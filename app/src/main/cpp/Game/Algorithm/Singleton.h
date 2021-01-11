//
// Created by 葵ユメ on 2020/12/06.
//


#ifndef AOIYUME_SINGLETON_H
#define AOIYUME_SINGLETON_H

template <class T>
class Singleton
{
protected:
	Singleton(){}
	virtual ~Singleton(){}
	
private:
	Singleton(const Singleton&) = delete;
	void operator=(const Singleton&) = delete;
	
	
public:
	static void Initialize(){
		s_pSingleton = new T();
	}
	
	static void Destroy(){
		delete s_pSingleton;
	}
	
	static T* Get(){
		if(!s_pSingleton){
			Initialize();
		}
		return s_pSingleton;
	}
	
private:
	static T* s_pSingleton;
};

#define USE_SINGLETON(classtype) \
private: \
	friend class Singleton<classtype>; \
	classtype(const classtype&) = delete; \
	void operator=(const classtype&) = delete; \
	\
protected: \
	classtype(); \
	virtual ~classtype(); \
	\
private: \


#define USE_SINGLETON_VARIABLE(classtype) \
template<> classtype* Singleton<classtype>::s_pSingleton = nullptr;
#endif