/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <memory>

namespace nene::t
{
	// Shared pointer holding both a mutable and a const view of the same object,
	// sharing one control block. Useful when exposing a stable const shared_ptr
	// while keeping a mutable one internally.
	template <typename data_t>
	class dual_shared_ptr
	{
	public:
		dual_shared_ptr() = default;

		dual_shared_ptr(std::shared_ptr<data_t> ptr) 
			: m_ptr(std::move(ptr)) 
		{
			sync();
		}

		dual_shared_ptr& operator=(std::shared_ptr<data_t> ptr) 
		{
			m_ptr = std::move(ptr);
			sync();
			return *this;
		}
		
		const std::shared_ptr<const data_t>& get() const 
		{
			return m_const_ptr;
		}
		
		data_t& operator*() { return *m_ptr; }
		const data_t& operator*() const { return *m_const_ptr; }
		data_t* operator->() { return m_ptr.get(); }
		const data_t* operator->() const { return m_const_ptr.get(); }

		explicit operator bool() const 
		{
			return static_cast<bool>(m_ptr);
		}
		
		void reset() 
		{
			m_ptr.reset();
			sync();
		}
		
	private:
		// Keep the const view in sync with the mutable pointer.
		void sync() 
		{
			m_const_ptr = m_ptr;
		}
		std::shared_ptr<data_t> m_ptr = {};
		std::shared_ptr<const data_t> m_const_ptr = {};
	};
}