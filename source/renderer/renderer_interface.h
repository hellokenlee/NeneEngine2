/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

class renderer_interface
{
public:
	renderer_interface() = default;
	virtual ~renderer_interface() = default;

	virtual void render_view_family() = 0;
};
