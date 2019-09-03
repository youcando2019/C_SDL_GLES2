#include "Texture.h"

// Constructeur
void Texture_init(Texture * me, char* fichierImage)
{
    me->m_id = 0;
    me->m_fichierImage = fichierImage;
}

void Texture_copy(Texture * me, Texture * const textureACopier)
{
    // Copie de la texture
    me->m_fichierImage = textureACopier->m_fichierImage;
    me->m_id = 0;
}

/*void operator=(Texture * me, Texture * const textureACopier)
{
    // Copie de la texture
    me->m_fichierImage = textureACopier->m_fichierImage;
    me->m_id = 0;
}
*/
// Destructeur
void Texture_destroy(Texture * me)
{
    // Destruction de la texture
    glDeleteTextures(1, &me->m_id);
}

// Méthodes
int Texture_charger(Texture * me)
{
    // SDL_Surface dans laquelle sera stockée l'image à charger
    SDL_Surface *imageSDL = 0;

    // Chargement de l'image
    imageSDL = IMG_Load(me->m_fichierImage);

    // Vérification du chargement
    if(imageSDL == 0)
    {
        printf("Erreur : %s \n", IMG_GetError());
        return 0;
    }

    // Conversion de l'image en un format unique
    SDL_Surface *imageSDLFormatee = SDL_ConvertSurfaceFormat(imageSDL, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(imageSDL);

    // Inversion des pixels
    SDL_Surface *imageSDLInversee = Texture_inverserPixels(imageSDLFormatee);
    SDL_FreeSurface(imageSDLFormatee);

    // Destruction d'une éventuelle ancienne texture
    if(glIsTexture(me->m_id) == GL_TRUE)
    {
        glDeleteTextures(1, &me->m_id);
    }

    GLenum formatInterne = 0;
    GLenum format = 0;
    // Determination du format et du format interne pour les images a 3 composantes
    if(imageSDLInversee->format->BytesPerPixel == 3)
    {
        // Format interne
        formatInterne = GL_RGB;
        // Format
        if(imageSDLInversee->format->Rmask == 0xff)
        {
            format = GL_RGB;
        }
        else
        {
            format = GL_RGB;
        }
    }
    // Determination du format et du format interne pour les images a 4 composantes
    else if(imageSDLInversee->format->BytesPerPixel == 4)
    {
        // Format interne
        formatInterne = GL_RGBA;
        // Format
        if(imageSDLInversee->format->Rmask == 0xff)
        {
            format = GL_RGBA;
        }
        else
        {
            format = GL_RGBA;
        }
    }
    // Dans les autres cas, on arrete le chargement
    else
    {
        printf("Erreur, format interne de l'image inconnu \n");
        SDL_FreeSurface(imageSDLInversee);
        return 0;
    }

    // Génération de l'ID
    glGenTextures(1, &me->m_id);
    // Verrouillage
    glBindTexture(GL_TEXTURE_2D, me->m_id);
        // Copie des pixels
        glTexImage2D(GL_TEXTURE_2D, 0, formatInterne, imageSDLInversee->w, imageSDLInversee->h, 0, format, GL_UNSIGNED_BYTE, imageSDLInversee->pixels);
        // Application des filtres
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Déverrouillage
    glBindTexture(GL_TEXTURE_2D, 0);
    // Libere la surface SDL
    SDL_FreeSurface(imageSDLInversee);
    return 1;
}

GLuint const Texture_getID(Texture * me)
{
    return me->m_id;
}

void Texture_setFichierImage(Texture * me, char* fichierImage)
{
    me->m_fichierImage = fichierImage;
}

SDL_Surface* const Texture_inverserPixels(SDL_Surface *imageSource)
{
    // Copie conforme de l'image source sans les pixels
    SDL_Surface *imageInversee = SDL_CreateRGBSurface(0, imageSource->w, imageSource->h, imageSource->format->BitsPerPixel,
            imageSource->format->Rmask, imageSource->format->Gmask, imageSource->format->Bmask, imageSource->format->Amask);

    // Tableau intermédiaires permettant de manipuler les pixels
    unsigned char* pixelsSources = (unsigned char*) imageSource->pixels;
    unsigned char* pixelsInverses = (unsigned char*) imageInversee->pixels;

    for(int i = 0; i < imageSource->h; i++)
    {
        for(int j = 0; j < imageSource->w * imageSource->format->BytesPerPixel; j++)
        {
            pixelsInverses[(imageSource->w * imageSource->format->BytesPerPixel * (imageSource->h - 1 - i)) + j] =
                                        pixelsSources[(imageSource->w * imageSource->format->BytesPerPixel * i) + j];
        }
    }

    // Retour de l'image inverse
    return imageInversee;
}
