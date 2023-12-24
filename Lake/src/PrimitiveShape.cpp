#include "Lake/PrimitiveShape.h"

#include "Lake/Log.h"

#include <glad/glad.h>

u32 lake::PrimitiveShape::sQuadVBO = 0;
u32 lake::PrimitiveShape::sQuadEBO = 0;
u32 lake::PrimitiveShape::sQuadVAO = 0;

void lake::PrimitiveShape::init() {
    static bool inited = false;
    LK_ASSERT(!inited, "PrimitiveShape::init() called more than once");
    inited = true;

    // Data
    constexpr f32 quadVertexData[] = {
        -1.0f, -1.0f,  // bottom left
         1.0f, -1.0f,  // bottom right
         1.0f,  1.0f,  // top right
        -1.0f,  1.0f,  // top left
    };

    constexpr u32 quadIndexData[] = {
        0, 1, 2, // first triangle
        2, 3, 0  // second triangle
    };
    
    constexpr u32 stride = 2 * sizeof(f32);

    // Quad
    glCreateBuffers(1, &sQuadVBO);
    glNamedBufferData(sQuadVBO, sizeof(quadVertexData), quadVertexData, GL_STATIC_DRAW);

    glCreateBuffers(1, &sQuadEBO);
    glNamedBufferData(sQuadEBO, sizeof(quadIndexData), quadIndexData, GL_STATIC_DRAW);

    glCreateVertexArrays(1, &sQuadVAO);
    glVertexArrayVertexBuffer(sQuadVAO, 0, sQuadVBO, 0, stride);
    glVertexArrayAttribFormat(sQuadVAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(sQuadVAO, 0);
    glVertexArrayElementBuffer(sQuadVAO, sQuadEBO);
}
