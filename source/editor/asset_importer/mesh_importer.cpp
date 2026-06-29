/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "mesh_importer.h"

#include <filesystem>

#include "texture_importer.h"
#include "engine/asset/static_mesh_asset.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>

#include "core/slow_task.h"


namespace nene
{
	logger mesh_importer_("mesh_importer");
	
	static std::shared_ptr<g::texture_asset> parse_assimp_embedded_texture(const aiTexture* tex)
	{
		auto result = asset_importer::make_asset<g::texture_asset>();
		auto& image = result->m_mip_maps.emplace_back();

		if (tex->mHeight == 0)
		{
			// compressed format (PNG, JPG, etc.) stored in memory
			int image_width, image_height, image_channels;
			unsigned char* r8g8b8a8 = stbi_load_from_memory(
				reinterpret_cast<const unsigned char*>(tex->pcData),
				static_cast<int>(tex->mWidth),
				&image_width, &image_height, &image_channels, 4);

			if (!r8g8b8a8)
			{
				log(mesh_importer_, error, "failed to decode embedded texture");
				return nullptr;
			}

			image.m_extent.x = image_width;
			image.m_extent.y = image_height;
			image.m_extent.z = 1;
			image.m_format = gapi_pixel_format::r8g8b8a8;
			image.m_pixels.resize(image_width * image_height * 4);
			memcpy(image.m_pixels.data(), r8g8b8a8, image_width * image_height * 4);
			stbi_image_free(r8g8b8a8);
		}
		else
		{
			// uncompressed BGRA texel data, convert to RGBA
			image.m_extent.x = tex->mWidth;
			image.m_extent.y = tex->mHeight;
			image.m_extent.z = 1;
			image.m_format = gapi_pixel_format::r8g8b8a8;

			const uint32_t pixel_count = tex->mWidth * tex->mHeight;
			image.m_pixels.resize(pixel_count * 4);
			for (uint32_t i = 0; i < pixel_count; i++)
			{
				image.m_pixels[i * 4 + 0] = tex->pcData[i].r;
				image.m_pixels[i * 4 + 1] = tex->pcData[i].g;
				image.m_pixels[i * 4 + 2] = tex->pcData[i].b;
				image.m_pixels[i * 4 + 3] = tex->pcData[i].a;
			}
		}

		return result;
	}
	
	static std::shared_ptr<g::static_mesh_asset> parse_assimp_mesh(const aiMesh* mesh)
	{
		//
		auto out_static_mesh = asset_importer::make_asset<g::static_mesh_asset>();
		// positions
		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			
			out_static_mesh->m_vertex_positions.emplace_back(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		}
		
		// normals
		if (mesh->HasNormals())
		{
			for (uint32_t i = 0; i < mesh->mNumVertices; i++)
			{
				out_static_mesh->m_vertex_normals.emplace_back(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			}
		}
		
		// texcoords
		for (const auto& texcoords : mesh->mTextureCoords)
		{
			if (texcoords != nullptr)
			{
				out_static_mesh->m_vertex_uvs.emplace_back();
				for (uint32_t i = 0; i < mesh->mNumVertices; i++)
				{
					out_static_mesh->m_vertex_uvs.back().emplace_back(texcoords[i].x, texcoords[i].y);
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
				out_static_mesh->m_triangle_faces.emplace_back(face.mIndices[j]);
			}
		}
		//
		return out_static_mesh;
	}

	static void process_assimp_scene_node(const aiNode* node, const aiScene* scene, const std::filesystem::path& to_directory_abs_path, std::map<std::string, std::shared_ptr<g::asset>>& result_assets)
	{
		// TODO: 按 node 处理, 记录 transform 关系, 一个 scene 导出为一个 prefab 文件
		scoped_slow_task slow_task(100.0f);
		
		// process all meshes
		slow_task.begin_progress_scope(50.0f, "importing meshes");
		for (uint32_t i = 0; i < scene->mNumMeshes; i++)
		{
			auto mesh = scene->mMeshes[i];
			result_assets.emplace(mesh->mName.C_Str(), parse_assimp_mesh(mesh));
		}
		
		// process all textures
		slow_task.begin_progress_scope(50.0f, "importing textures");
		for (uint32_t i = 0; i < scene->mNumTextures; i++)
		{
			auto tex = scene->mTextures[i];
			result_assets.emplace(tex->mFilename.C_Str(), parse_assimp_embedded_texture(tex));
		}
		texture_importer importer;
		auto white_list_assimp_texture_types = {aiTextureType_NORMALS, aiTextureType_BASE_COLOR, aiTextureType_NORMAL_CAMERA, aiTextureType_EMISSION_COLOR, aiTextureType_METALNESS, aiTextureType_DIFFUSE_ROUGHNESS, aiTextureType_AMBIENT_OCCLUSION, aiTextureType_EMISSIVE};
		for (uint32_t i = 0; i < scene->mNumMaterials; i++)
		{
			const auto& assimp_material = scene->mMaterials[i];
			for (const auto& assimp_texture_type : white_list_assimp_texture_types)
			{
				uint32_t num_textures = assimp_material->GetTextureCount(assimp_texture_type);
				for (uint32_t j = 0; j < num_textures; j++)
				{
					aiString str;
					const auto& res = assimp_material->GetTexture(assimp_texture_type, j, &str);
					if (res == aiReturn_SUCCESS && !str.Empty() && str.C_Str()[0] != '*')
					{
						auto texture_abs_path = (to_directory_abs_path / std::filesystem::path(str.C_Str())).generic_string();
						auto textures = importer.import_asset(texture_abs_path);
						for (auto& [name, texture] : textures)
						{
							result_assets.emplace(name, std::move(texture));
						}
					}
				}
			}
		}
	}
	
	std::map<std::string, std::shared_ptr<g::asset>> mesh_importer::import_asset(const std::string& from_abs_path)
	{
		//
		NENE_PROFILER_ZONE();
		NENE_PROFILER_ZONE_TEXT(from_abs_path.c_str(), from_abs_path.size());
		//
		Assimp::Importer importer;
		// nene uses left handed coordinate system
		const aiScene* scene = importer.ReadFile(from_abs_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_MakeLeftHanded | aiProcess_GenNormals);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
		{
			log(mesh_importer_, error, "assimp error: {}", importer.GetErrorString());
			return {};
		}
		
		std::map<std::string, std::shared_ptr<g::asset>> result;
		process_assimp_scene_node(scene->mRootNode, scene, std::filesystem::path(from_abs_path).parent_path(), result);
		return result;
	}

	std::vector<std::string> mesh_importer::get_supported_asset_extensions()
	{
		return {".fbx", ".gltf"};
	}
}
