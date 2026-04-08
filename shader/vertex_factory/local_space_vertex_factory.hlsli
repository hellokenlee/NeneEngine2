/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "vertex_factory/vertex_factory_common.hlsli"
#include "common/common.hlsli"
#include "cppshared/object_data.hlsli"

ConstantBuffer<SObjectData> ObjectData : register(b1, SPACE_VS);


struct SVertexShaderInput
{
	float4 WorldPosition;
	float2 UV;

	float4 GetWorldPosition()
	{
		return WorldPosition;
	}
	
	float2 GetUV()
	{
		return UV;
	}
};


struct SVertexFactoryInput
{
	float4 LocalPosition : POSITION0;
	float2 UV : TEXCOORD0;

	SVertexShaderInput GetVertexShaderInput()
	{
		SVertexShaderInput VertexShaderInput = (SVertexShaderInput)0;
		VertexShaderInput.WorldPosition = mul(LocalPosition, ObjectData.WorldMatrix);
		VertexShaderInput.UV = UV;
		return VertexShaderInput;
	}
};


