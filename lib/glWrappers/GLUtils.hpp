#ifndef GL_UTILS
#define GL_UTILS

#include <GL/gl.h>
#include <GL/glew.h>

#include "../ecco/EccoOutcome.hpp"

namespace ecco {
namespace OpenGL {

#define RETURN_ON_GL_FAIL(func)                                         \
  do {                                                                  \
  while(glGetError() != GL_NO_ERROR) {}                                 \
  func;                                                                 \
  GLenum err = glGetError();                                            \
  if (err != GL_NO_ERROR) {                                             \
    return ecco::StatusOutcome::Failure(std::string("GLError: " + err)); \
  }                                                                     \
  } while (false)


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
