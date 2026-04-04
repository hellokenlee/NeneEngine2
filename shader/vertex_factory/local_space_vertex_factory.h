/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "/vertex_factory/vertex_factory_common.h"
#include "/common/common.h"
#include "/cppshared/object_data.h"

cbuffer ObjectConstantBuffer : register(b1, SPACE_VS) { SObjectData ObjectData; };


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
	float4 LocalPosition : POSITION0;

	SVertexShaderInput GetVertexShaderInput()
	{
		SVertexShaderInput VertexShaderInput = (SVertexShaderInput)0;
		VertexShaderInput.WorldPosition = mul(LocalPosition, ObjectData.WorldMatrix);
		return VertexShaderInput;
	}
};


