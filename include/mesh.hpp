#ifndef _MESH_HPP_
#define _MESH_HPP_

#include <GL/glew.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "shader_light.hpp"
#include "IBL.hpp"

enum class DRAWING_MODE
{
	SOLID,
	WIREFRAME
};

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 biTangent;
	glm::ivec4 bonesID;
	glm::vec4 weights;

	Vertex()
	{
		position = glm::vec3(0.0f);
		normal = glm::vec3(0.0f);
		texCoords = glm::vec2(0.0f);
		tangent = glm::vec3(0.0f);
		biTangent = glm::vec3(0.0f);
		bonesID = glm::ivec4(0);
		weights = glm::vec4(0.0f);
	}

	Vertex(
		glm::vec3 pos,
		glm::vec3 norm,
		glm::vec2 tex,
		glm::vec3 tang = glm::vec3(0.0f),
		glm::vec3 bTang = glm::vec3(0.0f),
		glm::ivec4 idBones = glm::ivec4(0),
		glm::vec4 bonesWeights = glm::vec4(0.0f))
	{
		position = pos;
		normal = norm;
		texCoords = tex;
		tangent = tang;
		biTangent = bTang;
		bonesID = idBones;
		weights = bonesWeights;
	}
};

class Mesh
{
	public:

		Mesh(std::vector<Vertex> aVertices, std::vector<int> aIndices, Material m, std::string aName, glm::vec3 center);
        ~Mesh();
		std::string getName();
		std::vector<Vertex> const& getVertices() const;
		std::vector<int> const& getIndices() const;
		Material & getMaterial();
		void bindVAO() const;
		void draw(Shader & s, struct IBL_DATA * iblData = nullptr, bool instancing = false, int amount = 1, DRAWING_MODE mode = DRAWING_MODE::SOLID);
		void recreate(std::vector<Vertex> aVertices, std::vector<int> aIndices, bool dynamicDraw);
		void updateVBO(std::vector<Vertex> aVertices, std::vector<int> aIndices);
		bool getVertex(glm::vec3 pos, glm::vec3 normal, glm::vec3 lastPos, Vertex & out);
        glm::vec3 getCenter();
        glm::vec3 getCenterUpdate();
        void setCenterUpdate(glm::vec3 center);

	private:

		GLuint vao;
		GLuint vbo;
		GLuint ebo;

		std::string name;
		std::vector<Vertex> vertices;
		std::vector<int> indices;
        glm::vec3 m_center;
        glm::vec3 m_center_update;
		Material material;

		void shaderProcessing(Shader & s, struct IBL_DATA * iblData); // set proper uniforms according to shader type
};

#endif
