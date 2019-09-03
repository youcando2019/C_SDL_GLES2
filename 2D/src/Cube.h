#ifndef CUBE_H

#define CUBE_H

// Include OpenGL ES iOS
#ifdef __IPHONE_NA
#include <OpenGLES/ES2/gl.h>

// Include OpenGL ES Android
#elif __ANDROID__
#include <GLES2/gl2.h>

#else
#include <GLES2/gl2.h>

#endif

// Includes CGLM
#include <cglm/cglm.h>

#include "Shader.h"
#include "Texture.h"

// Classe Cube
typedef struct
{
    // Attributs
    Shader m_shader;
    Texture m_texture;
    float m_vertices[108];
    float m_coordTexture[72];
} Cube;

void Cube_init(float taille, char* const image, float repetition, char* const vertexShader, char* const fragmentShader, Cube * me);
void Cube_destroy(Cube * me);
void Cube_afficher(mat4 * projection, mat4 * modelview, Cube * me);

#endif
