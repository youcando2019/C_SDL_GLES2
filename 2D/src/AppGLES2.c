#include "AppGLES2.h"

void AppGLES2_init(AppGLES2 * me)
{
    me->m_vuePrincipale = 0;
    me->m_largeur = 0;
    me->m_hauteur = 0;
    me->m_contexteOpenGL = 0;
}

void AppGLES2_destroy(AppGLES2 * me)
{
    // Destruction du contexte OpenGL, de la vue principale et fin de l'application
    SDL_GL_DeleteContext(me->m_contexteOpenGL);
    SDL_DestroyWindow(me->m_vuePrincipale);
    SDL_Quit();
}

int AppGLES2_initWindow(AppGLES2 * me)
{
    // Initialisation de la SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Erreur lors de l'initialisation de la SDL : %s \n", SDL_GetError());
        return -1;
    }

    // Récupération de la vue principale
    me->m_vuePrincipale = SDL_CreateWindow("", 0, 0, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if(me->m_vuePrincipale == 0)
    {
        printf("Erreur lors de la recuperation de la vue : %s \n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Paramètres OpenGL ES
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    // Création du contexte OpenGL
    me->m_contexteOpenGL = SDL_GL_CreateContext(me->m_vuePrincipale);
    if(me->m_contexteOpenGL == 0)
    {
        printf("Erreur lors de la creation du contexte OpenGL : %s \n", SDL_GetError());
        SDL_DestroyWindow(me->m_vuePrincipale);
        SDL_Quit();
        return 0;
    }

    // Récupération des dimensions de la vue principale
    SDL_GetWindowSize(me->m_vuePrincipale, &me->m_largeur, &me->m_hauteur);
    window_width = me->m_largeur;
    window_height = me->m_hauteur;
    if(window_height <= window_width)
    {
        pixel = 1.0f / (float) window_height;
    }
    else
    {
        pixel = 1.0f / (float) window_width;
    }

    return 1;
}

int AppGLES2_initGL(AppGLES2 * me)
{
    // Définition de la zone d'affichage
    glViewport(0, 0, me->m_largeur, me->m_hauteur);

    // Activation du Depth Buffer
    glEnable(GL_DEPTH_TEST);

    return 1;
}

void AppGLES2_bouclePrincipale(AppGLES2 * me)
{
    // Variables d'évènements
    int terminer = 0;

    // Matrices projection et modelview
    mat4 projection;
    glm_perspective(70.0, (float) me->m_largeur / (float) me->m_hauteur, 1.0, 100.0, projection);
    //glm_ortho(0.0f, window_width, window_height, 0.0f, 0.01f, 100.0f, projection);
    mat4 modelview;
    glm_mat4_identity(modelview);
    mat4 modelviewSauvgarde;
    glm_mat4_copy(modelview, modelviewSauvgarde);

    CubeColor cubeColor;
    CubeColor_init(1.0, "Shaders/couleur3D.vert", "Shaders/couleur3D.frag", &cubeColor);
    Cube cube;
    Cube_init(1.0, "Textures/caisse.jpg", 1.0, "Shaders/texture.vert", "Shaders/texture.frag", &cube);

    Button btn;
    Button_init(&btn, 200, 200, "Textures/caisse.jpg", "Shaders/texture2D.vert", "Shaders/texture2D.frag");

    // Boucle principale
    while(!terminer)
    {
        // Gestion des évènements
        SDL_PollEvent(&me->m_evenements);
        if(me->m_evenements.window.event == SDL_WINDOWEVENT_CLOSE)
            terminer = 1;

        // Nettoyage de la vue
        glClearColor(0.5, 0.5, 0.8, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Placement de la caméra
        glm_lookat((vec3){0, 0, 1}, (vec3){0, 0, 0}, (vec3){0, 1, 0}, modelview);
        glm_mat4_copy(modelview, modelviewSauvgarde);

        Button_afficher(&btn, &projection, &modelview);
        CubeColor_afficher(&projection, &modelview, &cubeColor);
        Cube_afficher(&projection, &modelview, &cube);

        // Actualisation de la vue
        SDL_GL_SwapWindow(me->m_vuePrincipale);
    }
    Button_destroy(&btn);
    CubeColor_destroy(&cubeColor);
    Cube_destroy(&cube);
}
