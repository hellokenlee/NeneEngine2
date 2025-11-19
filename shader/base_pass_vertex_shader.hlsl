/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "/engine/generated/vertex_factory.h"
#include "/vertex_factory/vertex_factory_common.h"
#include "/base_pass_common.h"


void Main(in SVertexFactoryInput VertexFactoryInput, out SVertexShaderOutput VertexShaderOutput)
{
	//
	SVertexShaderInput VertexShaderInput;
	VertexFactoryGetVertexShaderInput(VertexFactoryInput, VertexShaderInput);

	//
	VertexShaderOutput.NdcPosition = VertexShaderInput.GetWorldPosition();
	VertexShaderOutput.UV = float2(1, 0);
}