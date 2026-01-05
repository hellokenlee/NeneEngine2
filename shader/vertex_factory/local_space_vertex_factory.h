/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "/vertex_factory/vertex_factory_common.h"

struct SVertexShaderInput
{
	float4 WorldPosition;

	float4 GetWorldPosition()
	{
		return WorldPosition;
	}
};


struct SVertexFactoryInput
{
	float4 Position : POSITION0;

	SVertexShaderInput GetVertexShaderInput()
	{
		SVertexShaderInput VertexShaderInput = (SVertexShaderInput)0;
		VertexShaderInput.WorldPosition = Position;
		return VertexShaderInput;
	}
};


