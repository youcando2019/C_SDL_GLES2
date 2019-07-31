#include "src/AppGLES2.h"

int main(int argc, char **argv)
{
    AppGLES2 app;
    AppGLES2_init(&app);
    if(!AppGLES2_initWindow(&app))
        return -1;
    if(!AppGLES2_initGL(&app))
        return -1;
    AppGLES2_bouclePrincipale(&app);

    AppGLES2_destroy(&app);

    return 0;
}
