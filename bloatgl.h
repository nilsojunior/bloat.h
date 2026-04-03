#ifndef BLOAT_GL_H
#define BLOAT_GL_H

#include <stdio.h>
#include <stdbool.h>

#include <GL/glcorearb.h>

typedef void* (*gl_loader_proc)(const char *name);
typedef gl_loader_proc (*gl_get_proc_address)(const char *name);

#define GL_FUNCS(X)                                                 \
    X(PFNGLCLEARPROC,                    glClear)                   \
    X(PFNGLCLEARCOLORPROC,               glClearColor)              \
    X(PFNGLCREATESHADERPROC,             glCreateShader)            \
    X(PFNGLSHADERSOURCEPROC,             glShaderSource)            \
    X(PFNGLCOMPILESHADERPROC,            glCompileShader)           \
    X(PFNGLGETSHADERIVPROC,              glGetShaderiv)             \
    X(PFNGLGETSHADERINFOLOGPROC,         glGetShaderInfoLog)        \
    X(PFNGLCREATEPROGRAMPROC,            glCreateProgram)           \
    X(PFNGLATTACHSHADERPROC,             glAttachShader)            \
    X(PFNGLLINKPROGRAMPROC,              glLinkProgram)             \
    X(PFNGLGETPROGRAMIVPROC,             glGetProgramiv)            \
    X(PFNGLGETPROGRAMINFOLOGPROC,        glGetProgramInfoLog)       \
    X(PFNGLDELETESHADERPROC,             glDeleteShader)            \
    X(PFNGLUSEPROGRAMPROC,               glUseProgram)              \
    X(PFNGLDELETEPROGRAMPROC,            glDeleteProgram)           \
    X(PFNGLUNIFORM1IPROC,                glUniform1i)               \
    X(PFNGLUNIFORM1FPROC,                glUniform1f)               \
    X(PFNGLGETUNIFORMLOCATIONPROC,       glGetUniformLocation)      \
    X(PFNGLVIEWPORTPROC,                 glViewport)                \
    X(PFNGLGENVERTEXARRAYSPROC,          glGenVertexArrays)         \
    X(PFNGLGENBUFFERSPROC,               glGenBuffers)              \
    X(PFNGLBINDVERTEXARRAYPROC,          glBindVertexArray)         \
    X(PFNGLBINDBUFFERPROC    ,           glBindBuffer)              \
    X(PFNGLBUFFERDATAPROC,               glBufferData)              \
    X(PFNGLVERTEXATTRIBPOINTERPROC,      glVertexAttribPointer)     \
    X(PFNGLENABLEVERTEXATTRIBARRAYPROC,  glEnableVertexAttribArray) \
    X(PFNGLGENTEXTURESPROC,              glGenTextures)             \
    X(PFNGLBINDTEXTUREPROC,              glBindTexture)             \
    X(PFNGLTEXPARAMETERIPROC,            glTexParameteri)           \
    X(PFNGLTEXIMAGE2DPROC,               glTexImage2D)              \
    X(PFNGLGENERATEMIPMAPPROC,           glGenerateMipmap)          \
    X(PFNGLACTIVETEXTUREPROC,            glActiveTexture)           \
    X(PFNGLDRAWELEMENTSPROC,             glDrawElements)            \
    X(PFNGLDELETEVERTEXARRAYSPROC,       glDeleteVertexArrays)      \
    X(PFNGLDELETEBUFFERSPROC,            glDeleteBuffers)           \
    X(PFNGLUNIFORMMATRIX4FVPROC,         glUniformMatrix4fv)        \
    X(PFNGLDRAWARRAYSPROC,               glDrawArrays)              \
    X(PFNGLENABLEPROC,                   glEnable)                  \
    X(PFNGLPOLYGONMODEPROC,              glPolygonMode)


#define X(type, name) static type name;
GL_FUNCS(X);
#undef X

#endif // BLOAT_GL_H

#ifdef BLOAT_GL_IMPLEMENTATION

bool gl_load(gl_get_proc_address fn)
{
    // Load all functions before returning false
    bool result = true;
#define X(type, name)                                                   \
    name = (type)(void*)fn(#name);                                      \
    if (!name) {                                                        \
        fprintf(stderr, "%s:%d: ERROR: Failed to load function `%s`\n", __FILE__, __LINE__, #name); \
        result = false;                                                 \
    }
    GL_FUNCS(X);
#undef X
    return result;
}

#endif // BLOAT_GL_IMPLEMENTATION
