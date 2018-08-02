#include "quad.h"
#include "iostream"

Quad::Quad(OpenGLContext *context) : Drawable(context)
{}

void Quad::create()
{
    GLuint idx[6]{0, 1, 2, 0, 2, 3};
    glm::vec4 vert_pos[4] {
                glm::vec4(0.f, yvals[0], 0.f, 1.f), // (0,0)
                glm::vec4(1.f, yvals[1], 0.f, 1.f), // (1, 0)
                glm::vec4(1.f, yvals[2], 1.f, 1.f), // (1, 1)
                glm::vec4(0.f, yvals[3], 1.f, 1.f)};//, // (0, 1)                

    glm::vec4 nor0 = glm::vec4(glm::normalize(glm::cross(glm::vec3(vert_pos[2] - vert_pos[0]), glm::vec3(vert_pos[1] - vert_pos[0]))), 0);
    glm::vec4 nor1 = glm::vec4(glm::normalize(glm::cross(glm::vec3(vert_pos[3] - vert_pos[0]), glm::vec3(vert_pos[2] - vert_pos[0]))), 0);

    glm::vec4 vert_nor[6] {nor0, nor0, nor0, nor1, nor1, nor1};    

    count = 6;

    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // CYL_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec4), vert_pos, GL_STATIC_DRAW);

    generateNor();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    context->glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec4), vert_nor, GL_STATIC_DRAW);
}
