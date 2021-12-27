/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "Windows.h"
#include "Core/Log.h"
#include "Engine/EngineLoop.h"

int main()
{
	EngineLoop::Initialize();

	EngineLoop::Update();

	EngineLoop::Shutdown();

	return 0;
}
