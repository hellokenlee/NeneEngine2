/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "mesh_importer.h"
#include "engine/asset/static_mesh_asset.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace nene
{
	logger mesh_importer_("mesh_importer");
	
	void parse_assimp_mesh_node(const aiMesh* mesh, g::static_mesh_asset& out_static_mesh)
	{
		// positions
		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			
			out_static_mesh.m_vertex_positions.emplace_back(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		}
		
		// normals
		if (mesh->HasNormals())
		{
			for (uint32_t i = 0; i < mesh->mNumVertices; i++)
			{
				out_static_mesh.m_vertex_normals.emplace_back(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			}
		}
		
		// texcoords
		for (const auto& texcoords : mesh->mTextureCoords)
		{
			if (texcoords != nullptr)
			{
				out_static_mesh.m_vertex_uvs.emplace_back();
				for (uint32_t i = 0; i < mesh->mNumVertices; i++)
				{
					out_static_mesh.m_vertex_uvs.back().emplace_back(texcoords[i].x, texcoords[i].y);
				}
			}
		}

		// index per face
		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			CHECK(face.mNumIndices == 3);
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				out_static_mesh.m_face_vertex_indices.emplace_back(face.mIndices[j]);
			}
		}
	}

	void process_assimp_scene_node(const aiNode* node, const aiScene* scene, std::vector<std::shared_ptr<g::static_mesh_asset>>& out_static_mesh_assets)
	{
		// process all meshes
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			out_static_mesh_assets.emplace_back(std::make_shared<g::static_mesh_asset>());
			parse_assimp_mesh_node(mesh, *(out_static_mesh_assets.back()));
		}

		// process other node in scene
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			process_assimp_scene_node(node->mChildren[i], scene, out_static_mesh_assets);
		}
	}
	
	std::shared_ptr<g::asset> mesh_importer::import_asset(const std::string& from_abs_path)
	{
		Assimp::Importer importer;
		
		const aiScene* scene = importer.ReadFile(from_abs_path, aiProcess_Triangulate | aiProcess_GenNormals);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
		{
			log(mesh_importer_, error, "assimp error: {}", importer.GetErrorString());
			return nullptr;
		}
		
		std::vector<std::shared_ptr<g::static_mesh_asset>> result;
		process_assimp_scene_node(scene->mRootNode, scene, result);
		return result.size() > 0 ? result[0] : nullptr;
	}

	std::vector<std::string> mesh_importer::get_supported_asset_extensions()
	{
		return {".fbx", ".gltf"};
	}
}
