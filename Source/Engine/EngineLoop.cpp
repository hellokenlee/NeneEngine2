/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "EngineLoop.h"

DEFINE_LOG_CATEGORY(Engine)


void EngineLoop::Initialize()
{
	LOG(Engine, Info, "Engine Init!");
}

void EngineLoop::Update()
{

}

void EngineLoop::Shutdown()
{
	LOG(Engine, Info, "Engine Shutdown!");
}