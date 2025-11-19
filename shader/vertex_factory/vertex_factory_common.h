/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

/** `VertexShaderInput SVertexFactoryInput::GetVertexShaderInput()` */
template<typename SVertexFactoryInput, typename SVertexShaderInput>
void VertexFactoryGetVertexShaderInput(in SVertexFactoryInput VertexFactoryInput, out SVertexShaderInput VertexShaderInput)
{
	VertexShaderInput = VertexFactoryInput.GetVertexShaderInput();
}

