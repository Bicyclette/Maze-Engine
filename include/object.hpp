#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_

#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>
#include <omp.h>
#include <memory>
#include <utility>
#include "mesh.hpp"
#include "shader_light.hpp"

glm::mat4 assimpMat4_to_glmMat4(aiMatrix4x4 & m);
glm::mat3 assimpMat3_to_glmMat3(aiMatrix3x3 & m);

struct AABB
{
	float xMax;
	float xMin;
	float yMax;
	float yMin;
	float zMax;
	float zMin;
};

class Object
{
	public:

		Object(glm::mat4 model = glm::mat4(1.0f));
		Object(const std::string & path, glm::mat4 model = glm::mat4(1.0f));
		virtual ~Object();
		virtual void draw(Shader& shader, DRAWING_MODE mode = DRAWING_MODE::SOLID);
        std::vector<std::shared_ptr<Mesh>>& getMeshes();
		void setInstancing(const std::vector<glm::mat4> & models);
		void resetInstancing();
		std::string getName();
		void setModel(glm::mat4 & matrix);
		struct AABB getAABB();
		
	protected:

		virtual void load(const std::string & path);
		void exploreNode(aiNode* node, const aiScene* scene);
		virtual std::shared_ptr<Mesh> getMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<struct Texture> loadMaterialTextures(
						const aiScene* scene,
						aiMaterial* mat,
						aiTextureType type,
						TEXTURE_TYPE t);
		void computeAABB();
		
		std::string name;

		std::string directory;
		std::vector<Texture> texturesLoaded;
		std::vector<std::shared_ptr<Mesh>> meshes;

		GLuint instanceVBO;
		std::vector<glm::mat4> instanceModel;
		glm::mat4 model;
		bool instancing;

		struct AABB aabb;
};

#endif
