/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

enum class serialization : uint8
{
    serializing,
    unserializing,
};

namespace i
{
	class archive
	{
	public:
		archive() = default;
		archive(const archive&) = default;
		archive& operator=(const archive&) = default;
		virtual ~archive() = default;
		
    	virtual void serialize(void *data, const size_t& length) = 0;
    	
    	virtual void serialize(const int& data) = 0;
    	
    	virtual void serialize(const bool& data) = 0;

		virtual void serialize(const std::string& data) = 0;
	};
}