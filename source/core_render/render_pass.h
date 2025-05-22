/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"

/**
 *	A render node is a set of render passes implement some render features.
 *	All render passes in a render node are guaranteed to be executed within the same thread.
 *	Different render node could be executed in different thread.
 */
class render_node
{
protected:
	/**
	 *	Implement this with in a render node class.
	 *	Do not use this directly.
	 */
	class render_pass
	{
	public:
		render_pass(const render_node& node)
			: m_parent_node(node)
		{}

		virtual ~render_pass() = default;

		const render_pass& get_parent_render_node() const { return m_parent_node; }

		virtual void render() = 0;

	protected:
		const render_node& m_parent_node;
	};

public:
	render_node() = default;
	
	virtual ~render_node() = default;
	
	void emplace_render_pass(const std::unique_ptr<render_pass>& new_render_pass)
	{
		m_render_passes.push_back(new_render_pass);
	}

protected:
	std::vector<std::unique_ptr<render_pass>> m_render_passes;
};


/// 
///		Example:
///		
///

class shadow_render_node : public render_node
{
protected:
	class shadow_depth_render_pass : public render_pass
	{
		shadow_depth_render_pass(const shadow_render_node& node)
			: render_pass(node)
		{
		}
	};

	class shadow_projection_render_pass : public render_pass
	{
		shadow_projection_render_pass(const shadow_render_node& node)
			: render_pass(node)
		{
		}
	};

public:
	shadow_render_node()
	{
		emplace_render_pass(std::make_unique<shadow_depth_render_pass>(*this));
		emplace_render_pass(std::make_unique<shadow_projection_render_pass>(*this));
	}
};
