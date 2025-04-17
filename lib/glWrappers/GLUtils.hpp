#ifndef GL_UTILS
#define GL_UTILS

#include <GL/glew.h>
#include <GL/gl.h>

namespace ecco {
    namespace OpenGL {
        static GLuint GetCurrentBoundVAO() {

        }
        static GLuint GetCurrentBoundVBO() {

        }

        static size_t GetMaxAllowedVAOAttachments() {
            GLint maxAttribs = 0;
            glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
            return maxAttribs;
        }

    }
}


#endif
