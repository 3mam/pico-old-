#include "gl.h"

void glInit(void) {
	if (SDL_GL_LoadLibrary(NULL) < 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize OpenGL",
														 SDL_GetError(), NULL);
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize OpenGL: %s",
								 SDL_GetError());
		exit(-1);
	}

	glCreateShader = (pCreateShader)SDL_GL_GetProcAddress("glCreateShader");
	glShaderSource = (pShaderSource)SDL_GL_GetProcAddress("glShaderSource");
	glCompileShader = (pCompileShader)SDL_GL_GetProcAddress("glCompileShader");
	glCreateProgram = (pCreateProgram)SDL_GL_GetProcAddress("glCreateProgram");
	glAttachShader = (pAttachShader)SDL_GL_GetProcAddress("glAttachShader");
	glLinkProgram = (pLinkProgram)SDL_GL_GetProcAddress("glLinkProgram");
	glGetAttribLocation =
			(pGetAttribLocation)SDL_GL_GetProcAddress("glGetAttribLocation");
	glUseProgram = (pUseProgram)SDL_GL_GetProcAddress("glUseProgram");
	glGenBuffers = (pGenBuffers)SDL_GL_GetProcAddress("glGenBuffers");
	glBindBuffer = (pBindBuffer)SDL_GL_GetProcAddress("glBindBuffer");
	glBufferData = (pBufferData)SDL_GL_GetProcAddress("glBufferData");
	glEnableVertexAttribArray = (pEnableVertexAttribArray)SDL_GL_GetProcAddress(
			"glEnableVertexAttribArray");
	glVertexAttribPointer =
			(pVertexAttribPointer)SDL_GL_GetProcAddress("glVertexAttribPointer");
	glGenTextures = (pGenTextures)SDL_GL_GetProcAddress("glGenTextures");
	glBindTexture = (pBindTexture)SDL_GL_GetProcAddress("glBindTexture");
	glTexParameteri = (pTexParameteri)SDL_GL_GetProcAddress("glTexParameteri");
	glTexImage2D = (pTexImage2D)SDL_GL_GetProcAddress("glTexImage2D");
	glViewport = (pViewport)SDL_GL_GetProcAddress("glViewport");
	glDrawArrays = (pDrawArrays)SDL_GL_GetProcAddress("glDrawArrays");
	glEnable = (pEnable)SDL_GL_GetProcAddress("glEnable");
	glClear = (pClear)SDL_GL_GetProcAddress("glClear");
	glClearColor = (pClearColor)SDL_GL_GetProcAddress("glClearColor");

	int32_t error = 0;
	if (glCreateShader == NULL) {
		error = -1;
	}
	if (glShaderSource == NULL) {
		error = -1;
	}
	if (glCompileShader == NULL) {
		error = -1;
	}
	if (glCreateProgram == NULL) {
		error = -1;
	}
	if (glAttachShader == NULL) {
		error = -1;
	}
	if (glLinkProgram == NULL) {
		error = -1;
	}
	if (glGetAttribLocation == NULL) {
		error = -1;
	}
	if (glUseProgram == NULL) {
		error = -1;
	}
	if (glGenBuffers == NULL) {
		error = -1;
	}
	if (glBindBuffer == NULL) {
		error = -1;
	}
	if (glBufferData == NULL) {
		error = -1;
	}
	if (glEnableVertexAttribArray == NULL) {
		error = -1;
	}
	if (glVertexAttribPointer == NULL) {
		error = -1;
	}
	if (glGenTextures == NULL) {
		error = -1;
	}
	if (glBindTexture == NULL) {
		error = -1;
	}
	if (glTexParameteri == NULL) {
		error = -1;
	}
	if (glTexImage2D == NULL) {
		error = -1;
	}
	if (glViewport == NULL) {
		error = -1;
	}
	if (glDrawArrays == NULL) {
		error = -1;
	}
	if (glEnable == NULL) {
		error = -1;
	}
	if (glClear == NULL) {
		error = -1;
	}
	if (glClearColor == NULL) {
		error = -1;
	}

	if (error < 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize OpenGL",
														 "GPU not support OpenGL 2.1", NULL);
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "GPU not support OpenGL 2.1");
		exit(-1);
	}
}

GLuint glShader(const GLchar *str, GLenum typ) {
  GLuint shader = (GLuint)glCreateShader(typ);
  glShaderSource(shader, 1, &str, NULL);
  glCompileShader(shader);
	// GLint success = 0;
	// glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	// printf("%d\n", success);
  return shader;
}

GLuint glCreateShaders(const GLchar *ver, const GLchar *frag) {
  GLuint verShader = glShader(ver, GL_VERTEX_SHADER);
  GLuint fragShader = glShader(frag, GL_FRAGMENT_SHADER);
  GLuint id = (GLuint)glCreateProgram();
  glAttachShader(id, verShader);
  glAttachShader(id, fragShader);
  glLinkProgram(id);

  return id;
}
