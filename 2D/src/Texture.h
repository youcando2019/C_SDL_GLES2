#ifndef TEXTURE_H

#define TEXTURE_H
// Include OpenGL ES iOS
#ifdef __IPHONE_NA
    #include <OpenGLES/ES2/gl.h>
// Include OpenGL ES Android
#elif __ANDROID__
    #include <GLES2/gl2.h>
#else
    #include <GLES2/gl2.h>
#endif
// Includes communs
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>

// Classe Texture
typedef struct
{
    GLuint m_id;
    char* m_fichierImage;
} Texture;

    void Texture_init(Texture * me, char* fichierImage);
    void Texture_copy(Texture * me, Texture * const textureACopier);
    //void operator=(Texture * me, Texture * const textureACopier);
    void Texture_destroy(Texture * me);
    int Texture_charger(Texture * me);
    GLuint const Texture_getID(Texture * me);
    void Texture_setFichierImage(Texture * me, char* fichierImage);
    SDL_Surface* const Texture_inverserPixels(SDL_Surface *imageSource);
#endif
