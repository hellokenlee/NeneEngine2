/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "mesh_importer.h"

#include <assimp/Importer.hpp>

namespace nene
{
	void import()
	{
		Assimp::Importer importer;
		
		importer.ReadFile();
	}
}
