#ifndef GL_UTILS
#define GL_UTILS

#include <GL/gl.h>
#include <GL/glew.h>

namespace ecco {
namespace OpenGL {

static GLuint GetCurrentBoundVAO() {
  GLint vao = 0;
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);
  return static_cast<GLuint>(vao);
}

static GLuint GetCurrentBoundVBO() {
  GLint vbo = 0;
  glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo);
  return static_cast<GLuint>(vbo);
}

static size_t GetMaxAllowedVAOAttachments() {
  GLint maxAttribs = 0;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
  return maxAttribs;
}

} // namespace OpenGL
} // namespace ecco

#endif
