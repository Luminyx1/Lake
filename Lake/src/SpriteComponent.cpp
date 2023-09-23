#include "Lake/SpriteComponent.h"

#include <glad/glad.h>

namespace {
    struct GLObjects {
        GLuint vao;
        GLuint vbo;
        GLuint ebo;
    };

    static GLObjects getQuadObjects() {
        static bool inited = false;
        static GLObjects objects;
        if (!inited) {
            constexpr f32 vertices[] = {
                -1.0f, -1.0f,  // bottom left
                 1.0f, -1.0f,  // bottom right
                 1.0f,  1.0f,  // top right
                -1.0f,  1.0f,  // top left
            };

            constexpr u32 indices[] = {
                0, 1, 2, // first triangle
                2, 3, 0  // second triangle
            };

            constexpr u32 stride = 2 * sizeof(f32);

            glGenBuffers(1, &objects.vbo);
            glBindBuffer(GL_ARRAY_BUFFER, objects.vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

            glGenVertexArrays(1, &objects.vao);
            glBindVertexArray(objects.vao);

            glGenBuffers(1, &objects.ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objects.ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, nullptr);
            glEnableVertexAttribArray(0);

            inited = true;
        }

        return objects;
    }
}

lake::SpriteComponent::SpriteComponent(const std::string& texturePath, const Texture::FilterMode filterMode)
    : mMatrix(1.0f)
    , mTexture(texturePath, filterMode)
    , mShaderProgram("lake/assets/shaders/sprite.vsh", "lake/assets/shaders/sprite.fsh")
{ }

lake::SpriteComponent::~SpriteComponent() = default;

void lake::SpriteComponent::draw(const RenderInfo& renderInfo) {
    mTexture.bind(0);

    mShaderProgram.bind();
    mShaderProgram.setMat4(0, mMatrix);
    mShaderProgram.setMat4(1, renderInfo.camera->getViewProjection());

    const auto& [vao, vbo, ebo] = getQuadObjects();
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
