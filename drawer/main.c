// Include OpenGL ES iOS
#ifdef __IPHONE_NA
#include <OpenGLES/ES2/gl.h>

// Include OpenGL ES Android
#elif __ANDROID__
#include <GLES2/gl2.h>

#elif WIN32
#include <glad/glad.h>
#include <KHR/khrplatform.h>

#else
#include <GLES2/gl2.h>

#endif

// Includes GLM
#include <cglm/cglm.h>

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "src/Shader.h"
#include "src/Paper.h"

int main(int argc, char **argv)
{
    // Initialisation of SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Error in the initialisation of SDL : %s \n", SDL_GetError());
        return -1;
    }

    // Create the window
    SDL_Window *window = 0;
    window = SDL_CreateWindow("", 0, 0, 600, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if(window == 0)
    {
        printf("Error in the creation of the window : %s \n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // OpenGL ES Parametres
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    // Create the contexte OpenGL
    SDL_GLContext contexteOpenGL = 0;
    contexteOpenGL = SDL_GL_CreateContext(window);
    if(contexteOpenGL == 0)
    {
        printf("Error in the creation of contexte OpenGL : %s \n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Get window dimensions
    int width = 0, height = 0;
    SDL_GetWindowSize(window, &width, &height);

    // Definition the viewport
    glViewport(0, 0, width, height);

    // Creation of Matrices
    mat4 projection;

    glm_ortho(0, width, 0, height, -0.5f, 0.5f, projection);

    // Shader
    Shader shader;
    Shader_init(&shader, "Shaders/color2D.vert", "Shaders/color2D.frag");
    Shader_charger(&shader);

    // Variables d'évènements
    int finish = 0;
    SDL_Event ev;

    // Colors
    vec3 color_background;
    glm_vec3_copy((vec3){1.0, 1.0, 1.0}, color_background);
    vec3 color;
    glm_vec3_copy((vec3){0.0, 0.0, 0.0}, color);
    // Paper
    Paper paper;
    Paper_init(&paper, width, height);

    int clicked = 0;

    // main loop
    while(finish == 0)
    {
        // Events test
        SDL_WaitEvent(&ev);
        if(ev.window.event == SDL_WINDOWEVENT_CLOSE)
        {
            finish = 1;
        }

        switch(ev.type)
        {
            case SDL_MOUSEBUTTONDOWN:
                if(ev.button.button == 1)
                {
                    clicked = 1;
                }
                else if(ev.button.button == 3)
                {
                    clicked = 3;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                clicked = 0;
                printf("SDL_MOUSEBUTTONUP, button %d clicks %d\n", ev.button.button, (int)ev.button.clicks);
                break;
            case SDL_MOUSEMOTION:
                if(clicked == 1)
                {
                    Paper_add(&paper, ev.motion.x, ev.motion.y, color);
                }
                else if(clicked == 3)
                {

                }
                break;
            case SDL_KEYUP:
                if(ev.key.keysym.sym == SDLK_q)
                {
					printf("You pressed Q - bye!\n");
                }
                if(ev.key.keysym.sym == SDLK_c)
                {
					Paper_clear(&paper);
                }
                break;
            default:
                break;
        }
        // Clear the screen
        glClearColor(color_background[0], color_background[1], color_background[2], 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate the shader
        glUseProgram(Shader_getProgramID(&shader));

        // On filling and activate the Vertex Attrib 0
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, paper.m_vertices);
        glEnableVertexAttribArray(0);

        // On filling and activate the Vertex Attrib 1
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, paper.m_colors);
        glEnableVertexAttribArray(1);

            // On draw on the screen
            glDrawArrays(GL_TRIANGLES, 0, paper.m_total_rects * 6);

        // Desactivate Vertex Attrib
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);

        // Desactivate the shader
        glUseProgram(0);

        // Refresh the screen
        SDL_GL_SwapWindow(window);
    }

    // Destroy contexte OpenGL, window and exit the software
    Paper_destroy(&paper);
    Shader_destroy(&shader);
    SDL_GL_DeleteContext(contexteOpenGL);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
