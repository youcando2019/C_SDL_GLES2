#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

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

#include "shared.h"
#include "Shader.h"
#include "Texture.h"

// Classe Cube
typedef struct
{
    // Attributs
    Shader m_shader;
    Texture m_texture;
    float m_vertices[12];
    float m_coordTexture[12];
    int m_width;
    int m_height;
} Button;

void Button_init(Button * self, int width, int height, char* const image, char* const vertexShader, char* const fragmentShader);
void Button_destroy(Button * self);
void Button_afficher(Button * self, mat4 * projection, mat4 * modelview);

#endif // BUTTON_H_INCLUDED
