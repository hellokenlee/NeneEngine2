/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once


namespace i
{
class NENE_API renderer
{
public:
	renderer() = default;
	virtual ~renderer() = default;

	virtual void render_view_family() = 0;
};
}
