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
    if (FT_Init_FreeType(&ft))
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
    if (FT_New_Face(ft, ttf_file.c_str(), 0, &face))
    {
        std::cerr << "ERROR::FREETYPE: Failed loading font" << std::endl;
        std::exit(-1);
    }

    FT_Set_Pixel_Sizes(face, 0, font_size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    Alphabet alphabet;
    police.emplace_back(ttf_file, alphabet);

    for (unsigned char c{ 0 }; c < 128; ++c)
    {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
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
        police[police.size() - 1].second.insert(std::pair<char, Glyph>(c, character));
    }

    // clean up
    FT_Done_Face(face);
}

void Text::use_police(int index)
{
    activePoliceIndex = index;
}

void Text::print(std::string txt, float x, float y, float scale, glm::vec3 color)
{
    if (activePoliceIndex == -1)
        return;
    Alphabet alphabet = police[activePoliceIndex].second;

    shader.use();
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

glm::vec3 Text::get_cursor_shape(std::string txt, float x, float y, float scale, int cursor_pos) // x,y => pos, z => height
{
    if (activePoliceIndex == -1)
        throw std::runtime_error("TEXT ERROR : WRONG POLICE INDEX SUPPLIED ! (-1)");
    Alphabet alphabet = police[activePoliceIndex].second;

    // get max glyph vertical size
    float height = 0.0f;
    // get vertical shift
    float yshift = 0.0f;
    for (unsigned char c{ 0 }; c < 128; ++c)
    {
        Glyph glyph = alphabet[c];
        if (glyph.size.y > height) {
            height = glyph.size.y;
        }
        if ((glyph.bearing.y - glyph.size.y) < yshift) {
            yshift = (glyph.bearing.y - glyph.size.y);
        }
    }

    // iterate through "cursor_pos" characters
    std::string::const_iterator c;
    for (c = txt.begin(); c < txt.begin() + cursor_pos; ++c)
    {
        Glyph glyph = alphabet[*c];
        // advance cursors for next glyph
        x += (glyph.advance >> 6) * scale;
    }
    return glm::vec3(x, y + yshift, height);
}

Sprite::Sprite(int id, glm::vec2 pos, glm::vec2 size, int screenW, int screenH) :
    m_id(id),
    m_layer_id(-1),
    m_pos(pos),
    m_size(size),
    m_color(0.0f, 0.0f, 0.0f, 1.0f),
    m_img_index(-1),
    m_img_gl(-1),
    m_bloom_strength(1.0f),
    m_shader("shaders/UI/vertex.glsl", "shaders/UI/fragment.glsl", SHADER_TYPE::UI),
    m_projection(glm::ortho(0.0f, static_cast<float>(screenW), 0.0f, static_cast<float>(screenH))),
    m_selectable(true),
    m_selected(false)
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    float data[24] = {
        m_pos.x, m_pos.y + m_size.y, 0.0f, 1.0f,
        m_pos.x, m_pos.y, 0.0f, 0.0f,
        m_pos.x + m_size.x, m_pos.y, 1.0f, 0.0f,
        m_pos.x, m_pos.y + m_size.y, 0.0f, 1.0f,
        m_pos.x + m_size.x, m_pos.y, 1.0f, 0.0f,
        m_pos.x + m_size.x, m_pos.y + m_size.y, 1.0f, 1.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
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
    for (auto& tex : m_img)
    {
        if (tex.id != -1)
            glDeleteTextures(1, &tex.id);
    }
}

void Sprite::translate(glm::vec2 shift)
{
    m_pos += shift;
    float data[24] = {
        m_pos.x, m_pos.y, 0.0f, 1.0f,
        m_pos.x, m_pos.y - m_size.y, 0.0f, 0.0f,
        m_pos.x + m_size.x, m_pos.y - m_size.y, 1.0f, 0.0f,
        m_pos.x, m_pos.y, 0.0f, 1.0f,
        m_pos.x + m_size.x, m_pos.y - m_size.y, 1.0f, 0.0f,
        m_pos.x + m_size.x, m_pos.y, 1.0f, 1.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 24 * sizeof(float), data);
}

void Sprite::set_pos(glm::vec2 pos)
{
    m_pos = pos;
    float data[24] = {
        m_pos.x, m_pos.y, 0.0f, 1.0f,
        m_pos.x, m_pos.y - m_size.y, 0.0f, 0.0f,
        m_pos.x + m_size.x, m_pos.y - m_size.y, 1.0f, 0.0f,
        m_pos.x, m_pos.y, 0.0f, 1.0f,
        m_pos.x + m_size.x, m_pos.y - m_size.y, 1.0f, 0.0f,
        m_pos.x + m_size.x, m_pos.y, 1.0f, 1.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 24 * sizeof(float), data);
}

void Sprite::set_size(glm::vec2 size)
{
    m_size = size;
    float data[24] = {
        m_pos.x, m_pos.y, 0.0f, 1.0f,
        m_pos.x, m_pos.y - m_size.y, 0.0f, 0.0f,
        m_pos.x + m_size.x, m_pos.y - m_size.y, 1.0f, 0.0f,
        m_pos.x, m_pos.y, 0.0f, 1.0f,
        m_pos.x + m_size.x, m_pos.y - m_size.y, 1.0f, 0.0f,
        m_pos.x + m_size.x, m_pos.y, 1.0f, 1.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 24 * sizeof(float), data);
}

void Sprite::set_background_img(std::string img)
{
    if (m_img[0].id != -1)
        glDeleteTextures(1, &m_img[0].id);
    m_img[0] = createTexture(img, TEXTURE_TYPE::DIFFUSE, true);
}

void Sprite::set_background_img_selected(std::string img)
{
    if (m_img[1].id != -1)
        glDeleteTextures(1, &m_img[1].id);
    m_img[1] = createTexture(img, TEXTURE_TYPE::DIFFUSE, true);
}

void Sprite::draw(glm::vec2 translate)
{
    glBindVertexArray(m_vao);
    m_shader.use();
    m_shader.setMatrix("proj", m_projection);
    if (m_img_index != -1)
        m_shader.setBool("use_bkg_img", true);
    else
        m_shader.setBool("use_bkg_img", false);
    glActiveTexture(GL_TEXTURE0);
    if (m_img_index > -1)
        glBindTexture(GL_TEXTURE_2D, m_img[m_img_index].id);
    else if (m_img_index == -2)
        glBindTexture(GL_TEXTURE_2D, m_img_gl);
    m_shader.setInt("image", 0);
    m_shader.setVec4f("bkg_color", m_color);
    m_shader.setFloat("bloom_strength", m_bloom_strength);
    m_shader.setVec2f("translate", translate);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

// top left corner of mouse pointer
bool Sprite::mouse_hover(int mouseX, int mouseY)
{
    return (mouseX >= m_pos.x && mouseX <= (m_pos.x + m_size.x)) && (mouseY <= m_pos.y && mouseY >= (m_pos.y - m_size.y));
}

void Sprite::resize_screen(int width, int height)
{
    m_projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
}