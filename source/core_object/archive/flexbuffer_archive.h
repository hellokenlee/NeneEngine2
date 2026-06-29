/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "archive.h"
#include <flatbuffers/flexbuffers.h>
#include <vector>


namespace nene::g
{
	class NENE_API flexbuffer_writer : public archive
	{
	public:
		flexbuffer_writer();

		enum direction direction() const override { return direction::write; }
		using archive::load;
		void load(const std::vector<uint8_t>& content) override { CHECK(false); }
		std::vector<uint8_t> dump() const override;

		archive& operator<<(const nvp<uint8_t>& kv) override;
		archive& operator<<(const nvp<uint16_t>& kv) override;
		archive& operator<<(const nvp<uint32_t>& kv) override;
		archive& operator<<(const nvp<int8_t>& kv) override;
		archive& operator<<(const nvp<int16_t>& kv) override;
		archive& operator<<(const nvp<int32_t>& kv) override;
		archive& operator<<(const nvp<float>& kv) override;
		archive& operator<<(const nvp<uint2>& kv) override;
		archive& operator<<(const nvp<uint3>& kv) override;
		archive& operator<<(const nvp<float2>& kv) override;
		archive& operator<<(const nvp<float3>& kv) override;
		archive& operator<<(const nvp<uuid>& kv) override;
		archive& operator<<(const nvp<std::string>& kv) override;

	protected:
		void enter_array(const char* name, size_t& size) override;
		void leave_array() override;
		void enter_object(const char* name) override;
		void leave_object() override;
		void write_blob(const char* name, const uint8_t* data, size_t bytes) override;

	private:
		enum class container_type : uint8_t { map, vector };

		struct container
		{
			container_type m_type;
			size_t m_start;
		};

		bool current_is_vector() const;
		void write_uint(const char* name, uint64_t value);
		void write_int(const char* name, int64_t value);
		void write_float(const char* name, float value);
		void write_string(const char* name, const std::string& value);
		void write_uint_vector(const char* name, const uint32_t* values, size_t size);
		void write_float_vector(const char* name, const float* values, size_t size);

		mutable flexbuffers::Builder m_builder;
		mutable std::vector<container> m_stack;
		mutable bool m_finished = false;
	};

	class NENE_API flexbuffer_reader : public archive
	{
	public:
		flexbuffer_reader() = default;

		enum direction direction() const override { return direction::read; }
		void load(const std::vector<uint8_t>& content) override;
		void load(std::vector<uint8_t>&& content) override;
		std::vector<uint8_t> dump() const override { CHECK(false); return {}; }

		archive& operator<<(const nvp<uint8_t>& kv) override;
		archive& operator<<(const nvp<uint16_t>& kv) override;
		archive& operator<<(const nvp<uint32_t>& kv) override;
		archive& operator<<(const nvp<int8_t>& kv) override;
		archive& operator<<(const nvp<int16_t>& kv) override;
		archive& operator<<(const nvp<int32_t>& kv) override;
		archive& operator<<(const nvp<float>& kv) override;
		archive& operator<<(const nvp<uint2>& kv) override;
		archive& operator<<(const nvp<uint3>& kv) override;
		archive& operator<<(const nvp<float2>& kv) override;
		archive& operator<<(const nvp<float3>& kv) override;
		archive& operator<<(const nvp<uuid>& kv) override;
		archive& operator<<(const nvp<std::string>& kv) override;

	protected:
		void enter_array(const char* name, size_t& size) override;
		void leave_array() override;
		void enter_object(const char* name) override;
		void leave_object() override;
		void read_blob(const char* name, std::vector<uint8_t>& bytes) override;

	private:
		void load_impl();
		flexbuffers::Reference next_value(const char* name);
		static size_t vector_size(const flexbuffers::Reference& ref);
		static flexbuffers::Reference vector_at(const flexbuffers::Reference& ref, size_t index);

		std::vector<uint8_t> m_content;
		flexbuffers::Reference m_root;
		std::vector<flexbuffers::Reference> m_stack;
		std::vector<size_t> m_array_index_stack;
	};
}
