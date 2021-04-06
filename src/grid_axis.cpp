#include "grid_axis.hpp"

GridAxis::GridAxis(int gridDim) :
	dim(gridDim),
	nbPoints((dim+1)*(dim+1)),
	nbIndices(2*(dim+1)*(2*dim)),
	gridShader("../shaders/grid/vertex.glsl", "../shaders/grid/fragment.glsl"),
	axisShader("../shaders/axis/vertex.glsl", "../shaders/axis/fragment.glsl")
{
	grid = new float[nbPoints * 3];
	indices = new int[nbIndices];

	for(int line{0}; line < (dim + 1); ++line)
	{
		for(int col{0}; col < (dim + 1); ++col)
		{
			int index = line * (dim + 1) + col;
			grid[(index * 3)] = static_cast<float>(-dim) / 2.0f + static_cast<float>(col);		// x
			grid[(index * 3) + 1] = 0.0f;														// y
			grid[(index * 3) + 2] = static_cast<float>(-dim) / 2.0f + static_cast<float>(line);	// z
		}
	}

	int a{0}, b{1};
	for(int i{0}, j{1}; i < (nbIndices/2); i+=2, j+=2)
	{
		indices[i] = a++;
		indices[j] = b++;
		
		if(b % (dim+1) == 0)
		{
			a++;
			b++;
		}
	}
	
	a = 0;
	b = dim+1;
	int aInit = a;
	int bInit = b;
	int modulo = (dim+1) * (dim+1);
	for(int i{nbIndices/2}, j{(nbIndices/2)+1}; i < nbIndices; i+=2, j+=2)
	{
		if(b % (modulo) == 0)
		{
			modulo++;
			a = ++aInit;
			b = ++bInit;
		}
		
		indices[i] = a;
		indices[j] = b;
		
		a += (dim+1);
		b += (dim+1);	
	}

	glGenVertexArrays(1, &vaoG);
	glGenBuffers(1, &vboG);
	glGenBuffers(1, &eboG);

	glBindVertexArray(vaoG);
	glBindBuffer(GL_ARRAY_BUFFER, vboG);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboG);

	glBufferData(GL_ARRAY_BUFFER, (nbPoints * 3) * sizeof(float), grid, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbIndices * sizeof(int), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	axis = new float[18]{
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, // x
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, // y
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f  // z
	};
	
	glGenVertexArrays(1, &vaoA);
	glGenBuffers(1, &vboA);

	glBindVertexArray(vaoA);
	glBindBuffer(GL_ARRAY_BUFFER, vboA);

	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), axis, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	gridShader.use();
	gridShader.setMatrix("model", glm::mat4(1.0f));

	axisShader.use();
	axisShader.setMatrix("model", glm::mat4(1.0f));
}

GridAxis::~GridAxis()
{
	glBindVertexArray(vaoG);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vboG);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &eboG);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vaoG);
	
	glBindVertexArray(vaoA);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vboA);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vaoA);

	delete grid;
	delete indices;
	delete axis;
}

void GridAxis::draw(glm::mat4 view, glm::mat4 projection)
{	
	// start wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1.0f);

	// draw grid
	glBindVertexArray(vaoG);
	gridShader.use();
	gridShader.setMatrix("view", view);
	gridShader.setMatrix("proj", projection);

	glDrawElements(GL_LINES, nbIndices, GL_UNSIGNED_INT, 0);
	
	// draw axis
	glLineWidth(2.0f);
	glBindVertexArray(vaoA);
	axisShader.use();
	axisShader.setMatrix("view", view);
	axisShader.setMatrix("proj", projection);

	axisShader.setVec3f("color", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_LINE_STRIP, 0, 2);
	axisShader.setVec3f("color", glm::vec3(0.0f, 1.0f, 0.0f));
	glDrawArrays(GL_LINE_STRIP, 2, 2);
	axisShader.setVec3f("color", glm::vec3(0.0f, 0.0f, 1.0f));
	glDrawArrays(GL_LINE_STRIP, 4, 2);

	// end wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
