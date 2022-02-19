#ifndef LIGHTNING_HPP
#define LIGHTNING_HPP

#include <algorithm>
#include <chrono>
#include <random>
#include "shader_light.hpp"

struct Lightning
{
	struct Arc
	{
		Arc(float amp, float thick) :
			m_dis(-amp/2.0f, amp/2.0f),
			m_thickness(thick)
		{}
		std::uniform_real_distribution<float> m_dis;
		float m_thickness;
	};

	Lightning(glm::vec3 from, glm::vec3 to, int step, glm::vec3 color, float intensity, std::vector<float> & arcs, bool dynamic, float refreshInterval) :
		m_from(from),
		m_to(to),
		m_step(step),
		m_modelFrom(glm::mat4(1.0f)),
		m_modelTo(glm::mat4(1.0f)),
		m_color(color),
		m_intensity(intensity),
		m_dynamic(dynamic),
		m_refreshTimer(0.0f),
		m_refreshInterval(refreshInterval),
		lightning("shaders/lightning/vertex.glsl", "shaders/lightning/fragment.glsl"),
		MAX_POINTS(24 * 3 * m_step * arcs.size() * 2),
		rng(std::chrono::steady_clock::now().time_since_epoch().count()),
		m_dis(0.01f, 0.99f)
	{
		// set arcs
		for(int i{0}; i < arcs.size(); i+=2)
			m_arcs.emplace_back(arcs[i], arcs[i+1]);

		// VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, MAX_POINTS * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Unbind VAO
		glBindVertexArray(0);
		
		// initial data
		genTriangles();
	}

	~Lightning()
	{
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &VBO);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &VAO);
	}

	static int cmp(const void* a, const void* b)
	{
		float A{*static_cast<const float*>(a)};
		float B{*static_cast<const float*>(b)};
		if(A < B)
			return -1;
		else if(A > B)
			return 1;
		return 0;
	}

	void setModelMatrix(glm::mat4 modelFrom, glm::mat4 modelTo)
	{
		m_modelFrom = modelFrom;
		m_modelTo = modelTo;
	}
	
	void genTriangles()
	{
		// update endpoints position
		glm::vec4 A(m_from, 1.0f);
		glm::vec4 B(m_to, 1.0f);
		A = m_modelFrom * A;
		B = m_modelTo * B;
		m_from = glm::vec3(A);
		m_to = glm::vec3(B);

		// get axis
		glm::vec3 front{m_to-m_from};
		glm::vec3 right{glm::cross(glm::normalize(front), glm::vec3(0.0f, 1.0f, 0.0f))};
		glm::vec3 up{glm::cross(right, glm::normalize(front))};

		// generate random arcs
		m_percentPos.clear();
		m_points.clear();
		m_triangles.clear();
		for(int j{0}; j < m_arcs.size(); ++j)
		{
			m_percentPos.push_back(std::vector<float>());
			for(int i{0}; i < m_step; ++i)
				m_percentPos[j].push_back(m_dis(rng));
			std::qsort(m_percentPos[j].data(), m_percentPos[j].size(), sizeof(float), Lightning::cmp);
		}
		for(int j{0}; j < m_arcs.size(); ++j)
		{
			m_points.push_back(std::vector<glm::vec3>());
			m_points[j].push_back(m_from);
			for(int i{0}; i < m_step; ++i)
			{
				glm::vec3 offsetFront = front * m_percentPos[j][i];
				glm::vec3 offsetRight = right * m_arcs[j].m_dis(rng);
				glm::vec3 offsetUp = up * m_arcs[j].m_dis(rng);
				m_points[j].push_back(glm::vec3(m_from + offsetFront + offsetRight + offsetUp));
			}
			m_points[j].push_back(m_to);

			// create triangles
			for(int i{0}; i < m_points[j].size()-1; ++i)
			{
				glm::vec3 p1 = m_points[j][i] + (up + right) * m_arcs[j].m_thickness;
				glm::vec3 p2 = m_points[j][i] + (-up + right) * m_arcs[j].m_thickness;
				glm::vec3 p3 = m_points[j][i+1] + (up + right) * m_arcs[j].m_thickness;
				glm::vec3 p4 = m_points[j][i+1] + (-up + right) * m_arcs[j].m_thickness;
				glm::vec3 p5 = m_points[j][i] + (up - right) * m_arcs[j].m_thickness;
				glm::vec3 p6 = m_points[j][i+1] + (up - right) * m_arcs[j].m_thickness;
				glm::vec3 p7 = m_points[j][i] + (-up - right) * m_arcs[j].m_thickness;
				glm::vec3 p8 = m_points[j][i+1] + (-up - right) * m_arcs[j].m_thickness;

				// front face
				m_triangles.push_back(p1[0]);
				m_triangles.push_back(p1[1]);
				m_triangles.push_back(p1[2]);
				
				m_triangles.push_back(p2[0]);
				m_triangles.push_back(p2[1]);
				m_triangles.push_back(p2[2]);
				
				m_triangles.push_back(p3[0]);
				m_triangles.push_back(p3[1]);
				m_triangles.push_back(p3[2]);
				
				m_triangles.push_back(p2[0]);
				m_triangles.push_back(p2[1]);
				m_triangles.push_back(p2[2]);
				
				m_triangles.push_back(p4[0]);
				m_triangles.push_back(p4[1]);
				m_triangles.push_back(p4[2]);
				
				m_triangles.push_back(p3[0]);
				m_triangles.push_back(p3[1]);
				m_triangles.push_back(p3[2]);
				
				// top face
				m_triangles.push_back(p1[0]);
				m_triangles.push_back(p1[1]);
				m_triangles.push_back(p1[2]);
				
				m_triangles.push_back(p3[0]);
				m_triangles.push_back(p3[1]);
				m_triangles.push_back(p3[2]);

				m_triangles.push_back(p5[0]);
				m_triangles.push_back(p5[1]);
				m_triangles.push_back(p5[2]);
				
				m_triangles.push_back(p5[0]);
				m_triangles.push_back(p5[1]);
				m_triangles.push_back(p5[2]);
				
				m_triangles.push_back(p3[0]);
				m_triangles.push_back(p3[1]);
				m_triangles.push_back(p3[2]);
				
				m_triangles.push_back(p6[0]);
				m_triangles.push_back(p6[1]);
				m_triangles.push_back(p6[2]);
				
				// back face
				m_triangles.push_back(p8[0]);
				m_triangles.push_back(p8[1]);
				m_triangles.push_back(p8[2]);
				
				m_triangles.push_back(p7[0]);
				m_triangles.push_back(p7[1]);
				m_triangles.push_back(p7[2]);

				m_triangles.push_back(p5[0]);
				m_triangles.push_back(p5[1]);
				m_triangles.push_back(p5[2]);
				
				m_triangles.push_back(p8[0]);
				m_triangles.push_back(p8[1]);
				m_triangles.push_back(p8[2]);
				
				m_triangles.push_back(p5[0]);
				m_triangles.push_back(p5[1]);
				m_triangles.push_back(p5[2]);
				
				m_triangles.push_back(p6[0]);
				m_triangles.push_back(p6[1]);
				m_triangles.push_back(p6[2]);
				
				// bottom face
				m_triangles.push_back(p2[0]);
				m_triangles.push_back(p2[1]);
				m_triangles.push_back(p2[2]);
				
				m_triangles.push_back(p8[0]);
				m_triangles.push_back(p8[1]);
				m_triangles.push_back(p8[2]);

				m_triangles.push_back(p4[0]);
				m_triangles.push_back(p4[1]);
				m_triangles.push_back(p4[2]);
				
				m_triangles.push_back(p2[0]);
				m_triangles.push_back(p2[1]);
				m_triangles.push_back(p2[2]);
				
				m_triangles.push_back(p7[0]);
				m_triangles.push_back(p7[1]);
				m_triangles.push_back(p7[2]);
				
				m_triangles.push_back(p8[0]);
				m_triangles.push_back(p8[1]);
				m_triangles.push_back(p8[2]);
			}
		}

		// update VBO
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		void * vbo_ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		memcpy(vbo_ptr, m_triangles.data(), m_triangles.size() * sizeof(float));
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	void draw(glm::mat4 & view, glm::mat4 & proj, float delta)
	{
		if(m_dynamic)
		{
			m_refreshTimer += delta;
			if(m_refreshTimer > m_refreshInterval)
			{
				m_refreshTimer = 0.0f;
				genTriangles();
			}
		}
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		lightning.use();
		lightning.setMatrix("view", view);
		lightning.setMatrix("proj", proj);
		lightning.setVec3f("color", m_color);
		lightning.setFloat("intensity", m_intensity);
		glDrawArrays(GL_TRIANGLES, 0, m_triangles.size()/3);
	}

	void setIntensity(float intensity)
	{
		m_intensity = intensity;
	}

	glm::vec3 m_from;
	glm::vec3 m_to;
	int m_step;
	glm::mat4 m_modelFrom;
	glm::mat4 m_modelTo;
	glm::vec3 m_color;
	float m_intensity;
	bool m_dynamic;
	float m_refreshTimer;
	const float m_refreshInterval;
	Shader lightning;
	const int MAX_POINTS;
	
	std::mt19937 rng;
	std::uniform_real_distribution<float> m_dis;
	std::vector<std::vector<float>> m_percentPos;
	std::vector<std::vector<glm::vec3>> m_points;
	std::vector<float> m_triangles;
	std::vector<struct Arc> m_arcs;

	GLuint VAO;
	GLuint VBO;
};

#endif
