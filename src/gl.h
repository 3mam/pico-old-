#ifndef GL_H
#define GL_H
#include <SDL2/SDL.h>
#ifdef _WIN32
#define APIENTRY __stdcall
#else
#define APIENTRY
#endif

typedef char GLchar;
typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef int GLint;
typedef int GLsizei;
typedef float GLfloat;
typedef double GLdouble;
typedef float GLclampf;
typedef int GLsizei;
typedef ptrdiff_t GLsizeiptr;
typedef void GLvoid;
typedef unsigned char GLboolean;

typedef GLuint(APIENTRY *pCreateShader)(GLenum);
typedef void(APIENTRY *pShaderSource)(GLuint, GLsizei, const GLchar **,
                                      const GLint *);
typedef void(APIENTRY *pCompileShader)(GLuint);
typedef GLuint(APIENTRY *pCreateProgram)(void);
typedef void(APIENTRY *pAttachShader)(GLuint, GLuint);
typedef void(APIENTRY *pLinkProgram)(GLuint);
typedef GLuint(APIENTRY *pGetAttribLocation)(GLuint, const GLchar *);
typedef void(APIENTRY *pUseProgram)(GLuint);
typedef void(APIENTRY *pGenBuffers)(GLsizei, GLuint *);
typedef void(APIENTRY *pBindBuffer)(GLenum, GLuint);
typedef void(APIENTRY *pBufferData)(GLenum, GLsizeiptr, const GLvoid *, GLenum);
typedef void(APIENTRY *pEnableVertexAttribArray)(GLuint);
typedef void(APIENTRY *pVertexAttribPointer)(GLuint, GLint, GLenum, GLboolean,
                                             GLsizei, const GLvoid *);
typedef void(APIENTRY *pGenTextures)(GLsizei, GLuint *);
typedef void(APIENTRY *pBindTexture)(GLenum, GLuint);
typedef void(APIENTRY *pTexParameteri)(GLenum, GLenum, GLint);
typedef void(APIENTRY *pTexImage2D)(GLenum, GLint, GLint, GLsizei, GLsizei,
                                    GLint, GLenum, GLenum, const GLvoid *);
typedef void(APIENTRY *pViewport)(GLint, GLint, GLsizei, GLsizei);
typedef void(APIENTRY *pDrawArrays)(GLenum, GLint, GLsizei);
typedef void(APIENTRY *pEnable)(GLenum);
typedef void(APIENTRY *pClear)(GLbitfield);
typedef void(APIENTRY *pClearColor)(GLclampf, GLclampf, GLclampf, GLclampf);

#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_ARRAY_BUFFER 0x8892
#define GL_STATIC_DRAW 0x88E4
#define GL_FLOAT 0x1406
#define GL_FALSE 0x0000
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_REPEAT 0x2901
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_RGBA 0x1908
#define GL_RGB 0x1907
#define GL_UNSIGNED_BYTE 0x1401
#define GL_UNSIGNED_INT_8_8_8_8 0x8035
#define GL_TRIANGLES 0x0004
#define GL_POINTS 0x0000
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_BLEND 0x0BE2
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303

pCreateShader glCreateShader;
pShaderSource glShaderSource;
pCompileShader glCompileShader;
pCreateProgram glCreateProgram;
pAttachShader glAttachShader;
pLinkProgram glLinkProgram;
pGetAttribLocation glGetAttribLocation;
pUseProgram glUseProgram;
pGenBuffers glGenBuffers;
pBindBuffer glBindBuffer;
pBufferData glBufferData;
pEnableVertexAttribArray glEnableVertexAttribArray;
pVertexAttribPointer glVertexAttribPointer;
pGenTextures glGenTextures;
pBindTexture glBindTexture;
pTexParameteri glTexParameteri;
pTexImage2D glTexImage2D;
pViewport glViewport;
pDrawArrays glDrawArrays;
pEnable glEnable;
pClear glClear;
pClearColor glClearColor;

void glInit(void);
GLuint glShader(const GLchar *, GLenum);
GLuint glCreateShaders(const GLchar *, const GLchar *);
#endif // GL_H
