#ifndef CUBECOLOR_H

#define CUBECOLOR_H

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

// Classe Cube
typedef struct
{
    // Attributs
    Shader m_shader;
    float m_vertices[108];
    float m_couleurs[108];
} CubeColor;

void CubeColor_init(float taille, char* const vertexShader, char* const fragmentShader, CubeColor * me);
void CubeColor_destroy(CubeColor * me);
void CubeColor_afficher(mat4 * projection, mat4 * modelview, CubeColor * me);

#endif
