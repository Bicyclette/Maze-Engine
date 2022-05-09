#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <string>
#include <memory>
#include <utility>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <vector>
#include <array>
#include <exception>
#include "shader_light.hpp"


struct Glyph
{
    GLuint textureID; // handle of the glyph texture
    glm::ivec2 size; // size of glyph
    glm::ivec2 bearing; // offset from baseline to left/top of glyph
    int advance; // offset to advance to next glyph
};

class Text
{
public:
    using Alphabet = std::map<char, Glyph>;

public:
    Text(int width, int height);
    ~Text();
    void init();
    void resize_screen(int width, int height);
    void load_police(std::string ttf_file, int font_size);
    void use_police(int index);
    void print(std::string txt, float x, float y, float scale, glm::vec3 color);

private:
    FT_Library ft;
    std::vector<std::pair<std::string, Alphabet>> police;
    int activePoliceIndex;
    GLuint vao;
    GLuint vbo;
    Shader shader;
    glm::mat4 projection;
};

class Sprite
{
public:
    Sprite(int id, glm::vec2 pos, glm::vec2 size, int screenW, int screenH);
    ~Sprite();
    int get_id() { return m_id; }
    void translate(glm::vec2 shift);
    void set_pos(glm::vec2 pos);
    void set_size(glm::vec2 size);
    glm::vec2 get_position() { return m_pos; }
    glm::vec2 get_size() { return m_size; }
    void set_background_img(std::string img);
    void set_background_img_gl(GLuint id) { m_img_gl = id; }
    void set_background_img_selected(std::string img);
    void set_background_color(glm::vec4 color) { m_color = color; }
    void set_bloom_strength(float strength) { m_bloom_strength = strength; }
    void draw(glm::vec2 translate = glm::vec2(0.0f));
    void use_background_img() { m_img_index = 0; }
    void use_background_img_gl() { m_img_index = -2; }
    void use_background_img_selected() { m_img_index = 1; }
    void use_background_color() { m_img_index = -1; }
    bool mouse_hover(int mouseX, int mouseY);
    void resize_screen(int width, int height);
    void set_selectable(bool selectable) { m_selectable = selectable; }
    bool is_selectable() { return m_selectable; }
    void select() { m_selected = true; }
    void unselect() { m_selected = false; }
    bool is_selected() { return m_selected; }
    int get_layer_id() { return m_layer_id; }
    void set_layer_id(int id) { m_layer_id = id; }

private:

    int m_id;
    int m_layer_id;
    glm::vec2 m_pos; // top left corner position
    glm::vec2 m_size;
    GLuint m_vao;
    GLuint m_vbo;
    std::array<Texture, 2> m_img; // [0] = normal, [1] = selected
    GLuint m_img_gl;
    glm::vec4 m_color;
    int m_img_index; // -1 means it uses plain color, -2 means it uses a framebuffer color texture
    float m_bloom_strength;
    Shader m_shader;
    glm::mat4 m_projection;
    bool m_selectable;
    bool m_selected;
};

struct SpriteGroup
{
    SpriteGroup() : m_id(0) {}
    SpriteGroup(int id) : m_id(id) {}
    void translate(glm::vec2 shift)
    {
        for (auto& s : m_sprite)
            s->translate(shift);
    }

    int m_id;
    std::vector<std::shared_ptr<Sprite>> m_sprite;
};

struct Layer
{
    Layer() : m_id(0), m_visible(true) {}

    Layer(int id) : m_sg(id), m_id(id), m_visible(true)
    {
        if (m_id < 0)
		{
            throw std::runtime_error("Error creating layer : supplied ID is negative (must be positive or null)");
		}
    }

    void set_id(int id) { m_id = id; m_sg.m_id = id; }

    void set_visibility(bool visible) { m_visible = visible; }

    void add_sprite(int id, glm::vec2 pos, glm::vec2 size, int screenW, int screenH)
    {
        m_sg.m_sprite.emplace_back(std::make_shared<Sprite>(id, pos, size, screenW, screenH));
        m_sg.m_sprite[m_sg.m_sprite.size() - 1]->set_layer_id(m_id);
    }

    std::shared_ptr<Sprite> get_sprite(int id)
    {
        for (int i{ 0 }; i < m_sg.m_sprite.size(); ++i)
        {
            if (m_sg.m_sprite[i]->get_id() == id)
                return m_sg.m_sprite[i];
        }
        throw std::runtime_error("Error while fetching sprite : wrong ID");
    }

    void draw()
    {
        if (!m_visible)
            return;
        for (auto& sprite : m_sg.m_sprite)
            sprite->draw();
    }

    int m_id;
    bool m_visible;
    SpriteGroup m_sg;
};

struct Page
{
    Page(int id) : m_id(id) {}
    void add_layer(int id) { m_layer.emplace_back(id); }
    Layer& get_layer(int id)
    {
        for (auto& layer : m_layer)
        {
            if (layer.m_id == id)
            {
                return layer;
            }
        }
        throw std::runtime_error("Error while fetching layer : wrong ID");
    }
    void draw()
    {
        for (auto& layer : m_layer)
            layer.draw();
    }

    int m_id;
    std::vector<Layer> m_layer;
};

class UI
{
public:
    UI() : m_page_index(-1) {}

    int get_active_page() { return m_page_index; }

    void set_active_page(int page_index) { m_page_index = page_index; }

    std::shared_ptr<Sprite> get_hovered_sprite(int mouseX, int mouseY)
    {
        if (m_page_index == -1)
            return std::shared_ptr<Sprite>(nullptr);
        std::shared_ptr<Sprite> hovered(nullptr);
        for (auto& layer : m_page[m_page_index].m_layer)
        {
            if (layer.m_visible)
            {
                for (auto& sprite : layer.m_sg.m_sprite)
                {
                    if (!sprite->is_selectable())
                        continue;
                    glm::vec2 pos{ sprite->get_position() };
                    glm::vec2 size{ sprite->get_size() };
                    if (mouseX >= pos.x && mouseX <= (pos.x + size.x) && mouseY >= pos.y && mouseY <= (pos.y + size.y))
                        hovered = sprite;
                }
            }
        }
        return hovered;
    }

    void add_page() { m_page.emplace_back(m_page.size()); }

    Page& get_page(int index) { return m_page[index]; }

    void resize_screen(int width, int height)
    {
        for (auto& page : m_page)
        {
            for (auto& layer : page.m_layer)
            {
                for (auto& sprite : layer.m_sg.m_sprite)
                {
                    sprite->resize_screen(width, height);
                }
            }
        }
    }

private:
    std::vector<Page> m_page;
    int m_page_index; // active page (rendered)
};

#endif
