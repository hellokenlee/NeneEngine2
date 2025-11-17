#include "/engine/generated/vertex_factory.h"

struct SVertexShaderOutput
{
	float4 WorldPosition : SV_POSITION;
}


void Main(SVertexFactoryInput VertexFactoryInput, out SVertexShaderOutput VertexShaderOutput)
{
	SVertexShaderInput VertexShaderInput = VertexFactoryInput.GetVertexShaderInput();
	VertexShaderOutput.WorldPosition = VertexShaderInput.GetWorldPosition();
}