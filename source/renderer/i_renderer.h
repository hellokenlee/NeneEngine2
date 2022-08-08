/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

class i_renderer
{
public:
	i_renderer() = default;
	virtual ~i_renderer() = default;

	virtual void render_view_family() = 0;
};
