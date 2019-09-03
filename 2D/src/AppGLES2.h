#ifndef APPGLES2_H

#define APPGLES2_H

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

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "shared.h"
#include "Button.h"
#include "CubeColor.h"
#include "Cube.h"

// Classe AppGLES2
typedef struct
{
    SDL_Window *m_vuePrincipale;
    int m_largeur;
    int m_hauteur;
    SDL_Event m_evenements;
    SDL_GLContext m_contexteOpenGL;
} AppGLES2;

void AppGLES2_init(AppGLES2 * me);
void AppGLES2_destroy(AppGLES2 * me);
int AppGLES2_initWindow(AppGLES2 * me);
int AppGLES2_initGL(AppGLES2 * me);
void AppGLES2_bouclePrincipale(AppGLES2 * me);

#endif
