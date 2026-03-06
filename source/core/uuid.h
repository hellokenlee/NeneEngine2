/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>


namespace nene
{
	using uuid = boost::uuids::uuid;
	
	inline uuid generate_random_uuid()
	{
		boost::uuids::random_generator generator;
		return generator();
	}
	
	inline std::string uuid_to_string(const uuid& uid)
	{
		return boost::uuids::to_string(uid);
	}
	
	inline uuid string_to_uuid(const std::string& str)
	{
		boost::uuids::string_generator gen;
		return gen(str);
	}
}
