/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "archive.h"
#include <nlohmann/json.hpp>
#include <vector>


namespace nene::g
{
	class NENE_API json_writer : public archive
	{
	public:
		json_writer();

		enum direction direction() const override { return direction::write; }
		void read(const std::string& file_path) override {}
		void write(const std::string& file_path) const override;

		archive& operator<<(const nvp<uint8_t>& kv) override;
		archive& operator<<(const nvp<uint16_t>& kv) override;
		archive& operator<<(const nvp<uint32_t>& kv) override;
		archive& operator<<(const nvp<int8_t>& kv) override;
		archive& operator<<(const nvp<int16_t>& kv) override;
		archive& operator<<(const nvp<int32_t>& kv) override;
		archive& operator<<(const nvp<float>& kv) override;
		archive& operator<<(const nvp<uint3>& kv) override;
		archive& operator<<(const nvp<float2>& kv) override;
		archive& operator<<(const nvp<uuid>& kv) override;
		archive& operator<<(const nvp<std::string>& kv) override;

	protected:
		void enter_array(const char* name, size_t& size) override;
		void leave_array() override;
		void enter_object(const char* name) override;
		void leave_object() override;

	private:
		nlohmann::json m_root;
		std::vector<nlohmann::json*> m_stack;
	};

	class NENE_API json_reader : public archive
	{
	public:
		json_reader() = default;

		enum direction direction() const override { return direction::read; }
		void read(const std::string& file_path) override;
		void write(const std::string& file_path) const override {}

		archive& operator<<(const nvp<uint8_t>& kv) override;
		archive& operator<<(const nvp<uint16_t>& kv) override;
		archive& operator<<(const nvp<uint32_t>& kv) override;
		archive& operator<<(const nvp<int8_t>& kv) override;
		archive& operator<<(const nvp<int16_t>& kv) override;
		archive& operator<<(const nvp<int32_t>& kv) override;
		archive& operator<<(const nvp<float>& kv) override;
		archive& operator<<(const nvp<uint3>& kv) override;
		archive& operator<<(const nvp<float2>& kv) override;
		archive& operator<<(const nvp<uuid>& kv) override;
		archive& operator<<(const nvp<std::string>& kv) override;

	protected:
		void enter_array(const char* name, size_t& size) override;
		void leave_array() override;
		void enter_object(const char* name) override;
		void leave_object() override;

	private:
		nlohmann::json m_root;
		std::vector<nlohmann::json*> m_stack;
		std::vector<size_t> m_array_index_stack;
	};
}
