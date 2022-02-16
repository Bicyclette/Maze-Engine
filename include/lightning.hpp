#ifndef LIGHTNING_HPP
#define LIGHTNING_HPP

#include "shader_light.hpp"

struct Lightning
{
	struct LightningArc
	{
		LightningArc(float amp, float thick) :
			m_amplitude(amp),
			m_thickness(thick)
		{}
		float m_amplitude;
		float m_thickness;
	};

	Lightning(glm::vec3 from, glm::vec3 to, int step, glm::vec3 color, float intensity) :
		m_from(from),
		m_to(to),
		m_step(step),
		m_color(color),
		m_intensity(intensity),
		lightning("shaders/lightning/vertex.glsl", "shaders/lightning/geometry.glsl", "shaders/lightning/fragment.glsl")
	{
		// VAO
		float data[3] = {m_from.x, m_from.y, m_from.z};
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), data, GL_DYNAMIC_ARRAY);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Unbind VAO
		glBindVertexArray(0);
	}
	
	void addArc(float amplitude, float thickness)
	{
		m_arcs.emplace_back(amplitude, thickness);
	}

	void draw(glm::mat4 & modelFrom, glm::mat4 & modelTo)
	{
		// update endpoints position
		glm::vec4 A(m_from, 1.0f);
		glm::vec4 B(m_to, 1.0f);
		A = modelFrom * A;
		B = modelTo * B;
		m_from = A.xyz;
		m_to = B.xyz;

		// draw
		lightning.use();
		lightning.setVec3f("color", m_color);
		lightning.setFloat("intensity", m_intensity);
		lightning.setInt("num_arcs", m_arcs.size());
		lightning.setInt("step", m_step);
		for(int i{0}; i < m_arcs.size(); ++i)
		{
			lightning.setFloat("arc_amplitude[" + std::to_string(i) + "]", m_arcs[i].m_amplitude);
			lightning.setFloat("arc_thickness[" + std::to_string(i) + "]", m_arcs[i].m_thickness);
		}
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBufferData update data
		glDrawArrays(GL_POINTS, 0, 1);
	}

	glm::vec3 m_from;
	glm::vec3 m_to;
	int m_step;
	glm::vec3 m_color;
	float m_intensity;
	std::vector<struct LightningArc> m_arcs;
	Shader lightning;

	GLuint VAO;
	GLuint VBO;
};

#endif
