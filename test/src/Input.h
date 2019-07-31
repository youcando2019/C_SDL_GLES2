#ifndef INPUT_H
#define INPUT_H

    // Include
    #include <SDL2/SDL.h>

    // Classe
    typedef struct
    {
        SDL_Event m_evenements;
        int m_touches[SDL_NUM_SCANCODES]; // BOOL
        int m_boutonsSouris[8]; // BOOL
        int m_x;
        int m_y;
        int m_xRel;
        int m_yRel;
        int m_terminer; // BOOL
    } Input;

    void Input_init(Input * me);
    void Input_destroy(Input * me);
    void Input_updateEvenements(Input * me);
    int const Input_terminer(Input * me); // BOOL
    int const Input_getTouche(Input * me, const SDL_Scancode touche); // BOOL
    int const Input_getBoutonSouris(Input * me, const Uint8 bouton); // BOOL
    int const Input_mouvementSouris(Input * me); // BOOL
    int const Input_getX(Input * me);
    int const Input_getY(Input * me);
    int const Input_getXRel(Input * me);
    int const Input_getYRel(Input * me);
    void const Input_afficherPointeur(Input * me, int reponse);
    void const Input_capturerPointeur(Input * me, int reponse);

#endif // INPUT_H_INCLUDED
