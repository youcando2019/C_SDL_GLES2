#ifndef SHADER_H
#define SHADER_H


// Include Windows

#ifdef WIN32
#define GLEW_STATIC
#define GLEW_BUILD
#include <GL/glew.h>


// Include Mac

#elif __APPLE__
#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>


// Include UNIX/Linux

#else
#include <GLES2/gl2.h>

#endif


// Includes communs

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

// Classe Shader

typedef struct
{
    GLuint m_vertexID;
    GLuint m_fragmentID;
    GLuint m_programID;

    char* m_vertexSource;
    char* m_fragmentSource;
} Shader;

void Shader_init(Shader * me, char* vertexSource, char* fragmentSource);
void Shader_destroy(Shader * me);

int Shader_charger(Shader * me);
int Shader_compilerShader(Shader * me, GLuint * shader, GLenum type, char* fichierSource);
GLuint Shader_getProgramID(Shader * me);

#endif
