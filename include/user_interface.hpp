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
        void print(std::string txt, float x, float y, float zIndex, float scale, glm::vec3 color);

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
        Sprite(glm::vec2 pos, glm::vec2 size, float zIndex, int screenW, int screenH);
        ~Sprite();
        void set_background_img(std::string img);
        void set_background_color(glm::vec4 color);
        void set_bloom_strength(float strength);
        void resize_screen(int width, int height);
        void draw();

    private:
        glm::vec2 m_pos; // top left corner position
        glm::vec2 m_size;
        float m_zIndex;
        GLuint m_vao;
        GLuint m_vbo;
        struct Texture m_img;
        glm::vec4 m_color;
        bool m_use_bkg_img;
        float m_bloom_strength;
        Shader shader;
        glm::mat4 projection;
};

struct SpriteGroup
{
    SpriteGroup(int id) : m_id(id) {}
    int m_id;
    std::vector<std::unique_ptr<Sprite>> m_sprite;
};

#endif
