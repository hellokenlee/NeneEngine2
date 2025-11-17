#include "vertex_factory_common.h"

struct SVertexShaderInput
{
	float4 Color;
	float4 WorldPosition;

	float4 GetWorldPosition()
	{
		return WorldPosition;
	}
}


struct SVertexFactoryInput
{
	float4 Position : POSITION0;
	float4 Color : COLOR0;

	SVertexShaderInput GetVertexShaderInput()
	{
		SVertexShaderInput VertexShaderInput = (SVertexShaderInput)0;
		return VertexShaderInput;
	}
}


