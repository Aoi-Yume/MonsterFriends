//
// Created by 葵ユメ on 2021/01/04.
//


#include "Random.h"

static unsigned int 		s_AppSeed;
static unsigned int 		s_SyncSeed;
static std::random_device	s_RndDevice;
static std::mt19937			s_AppRandom;
static std::mt19937			s_SyncRandom;

void Random::Initialize()
{
	s_AppSeed = s_RndDevice();
	s_AppRandom.seed(s_AppSeed);
	s_SyncSeed = s_RndDevice();
	s_SyncRandom.seed(s_SyncSeed);
}

void Random::SetSeed(unsigned int uSeed)
{
	s_AppSeed = uSeed;
	s_AppRandom.seed(uSeed);
}
void Random::SetSyncSeed(unsigned int uSeed)
{
	s_SyncSeed = uSeed;
	s_SyncRandom.seed(uSeed);
}

int Random::GetInt()
{
	return s_AppRandom();
}

int Random::GetInt(int nMin, int nMax)
{
	auto temp = std::uniform_int_distribution<>(nMin, nMax);
	return temp(s_AppRandom);
}

float Random::GetFloat(float fMin, float fMax)
{
	auto temp = std::uniform_real_distribution<>(fMin, fMax);
	return temp(s_AppRandom);
}

int Random::GetSyncInt()
{
	return s_SyncRandom();
}

int Random::GetSyncInt(int nMin, int nMax)
{
	auto temp = std::uniform_int_distribution<>(nMin, nMax);
	return temp(s_SyncRandom);
}

float Random::GetSyncFloat(float fMin, float fMax)
{
	auto temp = std::uniform_real_distribution<>(fMin, fMax);
	return temp(s_SyncRandom);
}

unsigned int Random::GetSeed()
{
	return s_AppSeed;
}

unsigned int Random::GetSyncSeed()
{
	return s_SyncSeed;
}