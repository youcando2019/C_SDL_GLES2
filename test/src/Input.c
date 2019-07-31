#include "Input.h"

void Input_init(Input * me)
{
    me->m_x = 0;
    me->m_y = 0;
    me->m_xRel = 0;
    me->m_yRel = 0;
    me->m_terminer = 0;

    // Initialisation du tableau m_touches[]
    for(int i=0; i < SDL_NUM_SCANCODES; i++)
    {
        me->m_touches[i] = 0;
    }

    // Initialisation du tableau m_boutonsSouris[]
    for(int i=0; i < 8; i++)
    {
        me->m_boutonsSouris[i] = 0;
    }
}

void Input_destroy(Input * me)
{

}

void Input_updateEvenements(Input * me)
{
    // Pour �viter des mouvements fictifs de la souris, on r�initialise les coordonn�es relatives
    me->m_xRel = 0;
    me->m_yRel = 0;

    // Boucle d'�v�nements
    while(SDL_PollEvent(&me->m_evenements))
    {
        // Switch sur le type d'�v�nement
        switch(me->m_evenements.type)
        {
            // Cas d'une touche enfonc�e
            case SDL_KEYDOWN:
                me->m_touches[me->m_evenements.key.keysym.scancode] = 1;
            break;

            // Cas d'une touche rel�ch�e
            case SDL_KEYUP:
                me->m_touches[me->m_evenements.key.keysym.scancode] = 0;
            break;

            // Cas de pression sur un bouton de la souris
            case SDL_MOUSEBUTTONDOWN:
                me->m_boutonsSouris[me->m_evenements.button.button] = 1;
            break;

            // Cas du rel�chement d'un bouton de la souris
            case SDL_MOUSEBUTTONUP:
                me->m_boutonsSouris[me->m_evenements.button.button] = 0;
            break;

            // Cas d'un mouvement de souris
            case SDL_MOUSEMOTION:
                me->m_x = me->m_evenements.motion.x;
                me->m_y = me->m_evenements.motion.y;
                me->m_xRel = me->m_evenements.motion.xrel;
                me->m_yRel = me->m_evenements.motion.yrel;
            break;

            // Cas de la fermeture de la fen�tre
            case SDL_WINDOWEVENT:
                if(me->m_evenements.window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    me->m_terminer = 1;
                }
            break;

            default:
            break;
        }
    }
}

int const Input_terminer(Input * me)
{
    return me->m_terminer;
}

int const Input_getTouche(Input * me, const SDL_Scancode touche)
{
    return me->m_touches[touche];
}

int const Input_getBoutonSouris(Input * me, const Uint8 bouton)
{
    return me->m_boutonsSouris[bouton];
}

int const Input_mouvementSouris(Input * me)
{
    if(me->m_xRel == 0 && me->m_yRel == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

// Getters concernant la position du curseur
int const Input_getX(Input * me)
{
    return me->m_x;
}

int const Input_getY(Input * me)
{
    return me->m_y;
}

int const Input_getXRel(Input * me)
{
    return me->m_xRel;
}

int const Input_getYRel(Input * me)
{
    return me->m_yRel;
}

void const Input_afficherPointeur(Input * me, int reponse)
{
    if(reponse)
    {
        SDL_ShowCursor(SDL_ENABLE);
    }
    else
    {
        SDL_ShowCursor(SDL_DISABLE);
    }
}

void const Input_capturerPointeur(Input * me, int reponse)
{
    if(reponse)
    {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }
    else
    {
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }
}
