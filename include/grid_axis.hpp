#ifndef GRID_AXIS_HPP
#define GRID_AXIS_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_light.hpp"

class GridAxis
{
	public:

		GridAxis(int gridDim = 4);
		~GridAxis();
		void draw(glm::mat4 view, glm::mat4 projection);

	private:

		// Grid data
		GLuint vaoG;
		GLuint vboG;
		GLuint eboG;
		Shader gridShader;
		float* grid;
		int* indices;
		int dim;
		int nbPoints;
		int nbIndices;

		// Axis Data
		GLuint vaoA;
		GLuint vboA;
		Shader axisShader;
		float* axis;
};

#endif
