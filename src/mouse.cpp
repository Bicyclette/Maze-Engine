#include "mouse.hpp"

Mouse::Mouse(int pos[2], int size[2], std::string img_normal, std::string img_hover, int screenW, int screenH) :
    m_screen{ screenW, screenH },
    m_size{ size[0], size[1] },
    m_shader("shaders/mouse/vertex.glsl", "shaders/mouse/fragment.glsl", SHADER_TYPE::MOUSE),
    m_img_index(0),
    m_bloom_strength(1.0f),
    m_projection(glm::ortho(0.0f, static_cast<float>(screenW), 0.0f, static_cast<float>(screenH)))
{
    m_pos[0] = pos[0];
    float posYRatio{ 1.0f - (pos[1] / static_cast<float>(m_screen[1])) };
    m_pos[1] = posYRatio * m_screen[1];

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    float data[24] = {
        m_pos[0], m_pos[1], 0.0f, 1.0f,
        m_pos[0], m_pos[1] - m_size[1], 0.0f, 0.0f,
        m_pos[0] + m_size[0], m_pos[1] - m_size[1], 1.0f, 0.0f,
        m_pos[0], m_pos[1], 0.0f, 1.0f,
        m_pos[0] + m_size[0], m_pos[1] - m_size[1], 1.0f, 0.0f,
        m_pos[0] + m_size[0], m_pos[1], 1.0f, 1.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // mouse shape
    m_img.push_back(createTexture(img_normal, TEXTURE_TYPE::DIFFUSE, true));
    m_img.push_back(createTexture(img_hover, TEXTURE_TYPE::DIFFUSE, true));
}

Mouse::~Mouse()
{
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &m_vbo);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &m_vao);
    for (auto& img : m_img)
        glDeleteTextures(1, &img.id);
}

void Mouse::update_position()
{
    SDL_GetMouseState(&m_pos[0], &m_pos[1]);
    float posYRatio{ 1.0f - (m_pos[1] / static_cast<float>(m_screen[1])) };
    m_pos[1] = posYRatio * m_screen[1];
    float data[24] = {
        m_pos[0], m_pos[1], 0.0f, 1.0f,
        m_pos[0], m_pos[1] - m_size[1], 0.0f, 0.0f,
        m_pos[0] + m_size[0], m_pos[1] - m_size[1], 1.0f, 0.0f,
        m_pos[0], m_pos[1], 0.0f, 1.0f,
        m_pos[0] + m_size[0], m_pos[1] - m_size[1], 1.0f, 0.0f,
        m_pos[0] + m_size[0], m_pos[1], 1.0f, 1.0f
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 24 * sizeof(float), data);
}

void Mouse::set_bloom_strength(float strength)
{
    m_bloom_strength = strength;
}

void Mouse::use_normal()
{
    m_img_index = 0;
}

void Mouse::use_hover()
{
    m_img_index = 1;
}

void Mouse::draw()
{
    glBindVertexArray(m_vao);
    m_shader.use();
    m_shader.setMatrix("proj", m_projection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_img[m_img_index].id);
    m_shader.setInt("image", 0);
    m_shader.setFloat("bloom_strength", m_bloom_strength);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

int* Mouse::get_position()
{
    return m_pos;
}

int* Mouse::get_size()
{
    return m_size;
}