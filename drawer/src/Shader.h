#ifndef SHADER_H
#define SHADER_H

// Include Mac
#ifdef __APPLE__
#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>


// Include Windows
#elif WIN32
    #include <glad/glad.h>
    #include <KHR/khrplatform.h>

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
