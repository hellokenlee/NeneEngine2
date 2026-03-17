/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"


namespace nene::g
{
	class object;
}

namespace nene
{
	/** Name-Value pair */
	template<typename data_t>
	struct nvp
	{
		const char* m_name;
		data_t& m_data;
	};
	// CDAT
	template<typename T>
	nvp(const char*, T&) -> nvp<T>;
	
	/***
	 *	The writer/reader abstract base class
	 */
	class NENE_API archive
	{
	public:
		
		//
		enum class direction { write, read };
		//
		virtual ~archive() = default;
		virtual direction direction() const = 0;
		virtual void read(const std::string& file_path) = 0;
		virtual void write(const std::string& file_path) const = 0;
		
		// entry
		virtual archive& operator<<(g::object& obj);
		
		// basic types
		virtual archive& operator<<(const nvp<uint8_t>& kv) = 0;
		virtual archive& operator<<(const nvp<uint3>& kv) = 0;
		virtual archive& operator<<(const nvp<std::string>& kv) = 0;
		
		// native types
		template<typename data_t>
		archive& operator<<(const nvp<data_t>& kv)
		{
			enter_object(kv.m_name);
			*this << kv.m_data;
			leave_object();
			return *this;
		}
		
		// stl's container types
		template<typename element_t>
		archive& operator<<(const nvp<std::vector<element_t>>& kvs)
		{
			//
			size_t len = kvs.m_data.size();
			enter_array(kvs.m_name, len);
			//
			for (auto i = 0; i < len; ++i)
			{
				(*this) << nvp<element_t>(kvs.m_name, kvs.m_data[i]);
			}
			leave_array();
			//
			return *this;
		}
		
	protected:
		virtual void enter_array(const char* name, size_t& size) = 0;
		virtual void leave_array() = 0;
		
		virtual void enter_object(const char* name) = 0;
		virtual void leave_object() = 0;
	};
}

/** auto nvp name get */
#define NVP(member_name) ::nene::nvp{#member_name, this->member_name}
