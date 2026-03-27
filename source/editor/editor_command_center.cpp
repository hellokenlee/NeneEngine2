/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "editor_command_center.h"


namespace nene
{
	editor_command_center& editor_command_center::get()
	{
		static editor_command_center instance;
		return instance;
	}

	void editor_command_center::invoke(const std::shared_ptr<editor_command>& cmd)
	{
		if (cmd == nullptr)
		{
			return;
		}
		
		cmd->execute();
		m_undo_stack.emplace_back(cmd);
		m_redo_stack.clear();
	}

	bool editor_command_center::undo()
	{
		if (m_undo_stack.empty())
		{
			return false;
		}
		
		auto cmd = m_undo_stack.back();
		m_undo_stack.pop_back();
		cmd->undo();
		m_redo_stack.emplace_back(cmd);
		return true;
	}

	bool editor_command_center::redo()
	{
		if (m_redo_stack.empty())
		{
			return false;
		}
		
		auto cmd = m_redo_stack.back();
		m_redo_stack.pop_back();
		cmd->execute();
		m_undo_stack.emplace_back(cmd);
		return true;
	}

	bool editor_command_center::can_undo() const
	{
		return !m_undo_stack.empty();
	}

	bool editor_command_center::can_redo() const
	{
		return !m_redo_stack.empty();
	}

	void editor_command_center::clear_history()
	{
		m_undo_stack.clear();
		m_redo_stack.clear();
	}
}
