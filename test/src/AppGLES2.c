#include "AppGLES2.h"

void AppGLES2_init(AppGLES2 * me)
{
    me->m_vuePrincipale = 0;
    me->m_largeur = 0;
    me->m_hauteur = 0;
    me->m_contexteOpenGL = 0;
    Input_init(&me->m_input);
}

void AppGLES2_destroy(AppGLES2 * me)
{
    // Destruction du contexte OpenGL, de la vue principale et fin de l'application
    Input_destroy(&me->m_input);
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
    me->m_vuePrincipale = SDL_CreateWindow("", 0, 0, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
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

    return 1;
}

int AppGLES2_initGL(AppGLES2 * me)
{
    #ifdef WIN32
        // If you use SDL you can use: https://wiki.libsdl.org/SDL_GL_GetProcAddress
        if (!gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            printf("Failed to initialize OpenGL context\n");
            return -1;
        }

        /*
        // Alternative use the builtin loader, e.g. if no other loader function is available
        if (!gladLoadGL())
        {
            printf("Failed to initialize OpenGL context\n");
            return -1;
        }
        */
    #endif

    // Définition de la zone d'affichage
    glViewport(0, 0, me->m_largeur, me->m_hauteur);

    // Activation du Depth Buffer
    glEnable(GL_DEPTH_TEST);

    return 1;
}

void AppGLES2_bouclePrincipale(AppGLES2 * me)
{
    // Matrices projection et modelview
    mat4 projection;
    //glm_perspective(70.0, (float) me->m_largeur / me->m_hauteur, 1.0, 1000.0, projection);
    glm_ortho(-1.0f, -1.0f, 1.0f, 1.0f, 1.0, 10.0, projection);
    mat4 view;
    glm_mat4_identity(view);
    mat4 model;
    glm_mat4_identity(model);
    mat4 modelSauvgarde;
    glm_mat4_copy(model, modelSauvgarde);

    // Cam�ra mobile
    Camera camera;
    Camera_init(&camera, (vec3){0.001, 3, 0.001}, (vec3){0, 0, 0}, (vec3){0, 1, 0}, 0.5, 0.5);
    Input_afficherPointeur(&me->m_input, 0);
    Input_capturerPointeur(&me->m_input, 1);

    Obj button;
    Obj_init(&button,
                  "plane.obj",
                  "Textures/caisse.jpg",
                  (vec3){0.5, 0.2, 0.8},
                  (vec3){0.0, 0.0, 0.0},
                  (vec3){0.0, 0.0, 0.0},
                  (vec3){1.0, 1.0, 1.0},
                  "Shaders/texture.vert",
                  "Shaders/texture.frag");

    // Boucle principale
    while(!Input_terminer(&me->m_input))
    {
        // Gestion des evenements
        Input_updateEvenements(&me->m_input);
        if(Input_getTouche(&me->m_input, SDL_SCANCODE_ESCAPE))
        {
            break;
        }

        Camera_deplacer(&camera, &me->m_input);

        // Nettoyage de la vue
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Placement de la caméra
        Camera_lookat(&camera, &view);

        Obj_afficher(&button, projection, view, model, camera.m_position);

        // Actualisation de la vue
        SDL_GL_SwapWindow(me->m_vuePrincipale);
    }

    Obj_destroy(&button);
    Camera_destroy(&camera);
}
