#include "user_interface.hpp"

Text::Text(int width, int height) :
    activePoliceIndex(-1),
    shader("shaders/text/vertex.glsl", "shaders/text/fragment.glsl", SHADER_TYPE::TEXT),
    projection(glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height)))
{
    init();
}

void Text::init()
{
    if(FT_Init_FreeType(&ft))
    {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType library" << std::endl;
        std::exit(-1);
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Text::resize_screen(int width, int height)
{
    projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
}

Text::~Text()
{
    FT_Done_FreeType(ft);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vbo);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
}

void Text::load_police(std::string ttf_file, int font_size)
{
    FT_Face face;
    if(FT_New_Face(ft, ttf_file.c_str(), 0, &face))
    {
        std::cerr << "ERROR::FREETYPE: Failed loading font" << std::endl;
        std::exit(-1);
    }

    FT_Set_Pixel_Sizes(face, 0, font_size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    
    Alphabet alphabet;
    police.emplace_back(ttf_file, alphabet);
    
    for(unsigned char c{0}; c < 128; ++c)
    {
        // load character glyph
        if(FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cerr << "ERROR::FREETYPE: Failed to load glyph : " << c << std::endl;
            continue;
        }
        // generate texture
        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        // set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // store character
        Glyph character = {
            texID,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        police[police.size()-1].second.insert(std::pair<char, Glyph>(c, character));
    }

    // clean up
    FT_Done_Face(face);
}

void Text::use_police(int index)
{
    activePoliceIndex = index;
}

void Text::print(std::string txt, float x, float y, float zIndex, float scale, glm::vec3 color)
{
    if (activePoliceIndex == -1)
        return;
    Alphabet alphabet = police[activePoliceIndex].second;

    shader.use();
    shader.setFloat("zIndex", zIndex);
    shader.setVec3f("textColor", color);
    shader.setMatrix("proj", projection);
    glBindVertexArray(vao);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = txt.begin(); c != txt.end(); ++c)
    {
        Glyph glyph = alphabet[*c];
        float xpos = x + glyph.bearing.x * scale;
        float ypos = y - (glyph.size.y - glyph.bearing.y) * scale;

        float w = glyph.size.x * scale;
        float h = glyph.size.y * scale;

        // update VBO
        float vertices[24] = {
            xpos, ypos + h, 0.0f, 0.0f,
            xpos, ypos, 0.0f, 1.0f,
            xpos + w, ypos, 1.0f, 1.0f,
            xpos, ypos + h, 0.0f, 0.0f,
            xpos + w, ypos, 1.0f, 1.0f,
            xpos + w, ypos + h, 1.0f, 0.0f
        };
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        // bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, glyph.textureID);
        shader.setInt("text", 0);

        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // advance cursors for next glyph
        x += (glyph.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Sprite::Sprite(glm::vec2 pos, glm::vec2 size, float zIndex, int screenW, int screenH) :
    m_pos(pos),
    m_size(size),
    m_zIndex(zIndex),
    m_color(0.0f, 0.0f, 0.0f, 1.0f),
    m_use_bkg_img(false),
    m_bloom_strength(0.0f),
    shader("shaders/UI/vertex.glsl", "shaders/UI/fragment.glsl", SHADER_TYPE::UI),
    projection(glm::ortho(0.0f, static_cast<float>(screenW), 0.0f, static_cast<float>(screenH)))
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glm::vec2 posPx(m_pos.x*screenW, m_pos.y*screenH);
    glm::vec2 sizePx(m_size.x*screenW, m_size.y*screenH);
    float data[30] = {
        posPx.x, posPx.y, zIndex, 0.0f, 1.0f,
        posPx.x, posPx.y - sizePx.y, zIndex, 0.0f, 0.0f,
        posPx.x + sizePx.x, posPx.y - sizePx.y, zIndex, 1.0f, 0.0f,
        posPx.x, posPx.y, zIndex, 0.0f, 1.0f,
        posPx.x + sizePx.x, posPx.y - sizePx.y, zIndex, 1.0f, 0.0f,
        posPx.x + sizePx.x, posPx.y, zIndex, 1.0f, 1.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

Sprite::~Sprite()
{
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &m_vbo);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &m_vao);
    if(m_img.id != -1)
        glDeleteTextures(1, &m_img.id);
}

void Sprite::set_background_img(std::string img)
{
    m_img = createTexture(img, TEXTURE_TYPE::DIFFUSE, true);
    m_use_bkg_img = true;
}

void Sprite::set_background_color(glm::vec4 color)
{
    m_color = color;
}

void Sprite::set_bloom_strength(float strength)
{
    m_bloom_strength = strength;
}

void Sprite::resize_screen(int width, int height)
{
    projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
    
    glm::vec2 posPx(m_pos.x * width, m_pos.y * height);
    glm::vec2 sizePx(m_size.x * width, m_size.y * height);
    float data[30] = {
        posPx.x, posPx.y, m_zIndex, 0.0f, 1.0f,
        posPx.x, posPx.y - sizePx.y, m_zIndex, 0.0f, 0.0f,
        posPx.x + sizePx.x, posPx.y - sizePx.y, m_zIndex, 1.0f, 0.0f,
        posPx.x, posPx.y, m_zIndex, 0.0f, 1.0f,
        posPx.x + sizePx.x, posPx.y - sizePx.y, m_zIndex, 1.0f, 0.0f,
        posPx.x + sizePx.x, posPx.y, m_zIndex, 1.0f, 1.0f
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 30 * sizeof(float), data);
}

void Sprite::draw()
{
    glBindVertexArray(m_vao);
    shader.use();
    shader.setMatrix("proj", projection);
    if (m_use_bkg_img)
    {
        shader.setBool("use_bkg_img", true);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_img.id);
        shader.setInt("image", 0);
    }
    else
    {
        shader.setBool("use_bkg_img", false);
        shader.setVec4f("bkg_color", m_color);
    }
    shader.setFloat("bloom_strength", m_bloom_strength);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
