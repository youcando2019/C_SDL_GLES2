#include "Shader.h"

// Constructeurs et Destructeur

void Shader_init(Shader* me, char* vertexSource, char* fragmentSource)
{
    me->m_vertexID = 0;
    me->m_fragmentID = 0;
    me->m_programID = 0;
    me->m_vertexSource = vertexSource;
    me->m_fragmentSource = fragmentSource;
}

void Shader_destroy(Shader* me)
{
    // Destruction du shader

    glDeleteShader(me->m_vertexID);
    glDeleteShader(me->m_fragmentID);
    glDeleteProgram(me->m_programID);
}

// Methodes

int Shader_charger(Shader * me)
{
    // Destruction d'un �ventuel ancien Shader

    if(glIsShader(me->m_vertexID) == GL_TRUE)
        glDeleteShader(me->m_vertexID);

    if(glIsShader(me->m_fragmentID) == GL_TRUE)
        glDeleteShader(me->m_fragmentID);

    if(glIsProgram(me->m_programID) == GL_TRUE)
        glDeleteProgram(me->m_programID);


    // Compilation des shaders
    if(!Shader_compilerShader(me, &me->m_vertexID, GL_VERTEX_SHADER, me->m_vertexSource))
    {
        return 0;
    }

    if(!Shader_compilerShader(me, &me->m_fragmentID, GL_FRAGMENT_SHADER, me->m_fragmentSource))
    {
        return 0;
    }

    // Creation du programme

    me->m_programID = glCreateProgram();


    // Association des shaders

    glAttachShader(me->m_programID, me->m_vertexID);
    glAttachShader(me->m_programID, me->m_fragmentID);

    // Verrouillage des entres shader

    glBindAttribLocation(me->m_programID, 0, "in_Vertex");
    glBindAttribLocation(me->m_programID, 1, "in_Color");
    glBindAttribLocation(me->m_programID, 2, "in_TexCoord0");
    glBindAttribLocation(me->m_programID, 3, "in_Normal");


    // Linkage du programme

    glLinkProgram(me->m_programID);



    // Verification du linkage

    GLint erreurLink = 0;
    glGetProgramiv(me->m_programID, GL_LINK_STATUS, &erreurLink);


    // S'il y a eu une erreur
    if(erreurLink != GL_TRUE)
    {
        // Recuperation de la taille de l'erreur
        GLint tailleErreur = 0;
        glGetProgramiv(me->m_programID, GL_INFO_LOG_LENGTH, &tailleErreur);


        // Allocation de memoire

        //char *erreur = new char[tailleErreur + 1];
        char *erreur = malloc(tailleErreur + 1 * sizeof(char));


        // R�cup�ration de l'erreur

        glGetShaderInfoLog(me->m_programID, tailleErreur, &tailleErreur, erreur);
        erreur[tailleErreur] = '\0';


        // Affichage de l'erreur
        printf("Error link : %s \n", erreur);


        // Lib�ration de la m�moire et retour du bool�en false

        //delete[] erreur;
        glDeleteProgram(me->m_programID);
        free(erreur);

        return 0;
    }



    // Sinon c'est que tout s'est bien pass�

    else
        return 1;
}


int Shader_compilerShader(Shader * me, GLuint * shader, GLenum type, char* fichierSource)
{
    // Creation du shader

    *shader = glCreateShader(type);


    // Verification du shader

    if(*shader == 0)
    {
        printf("Erreur, le __type__ de shader n'existe pas \n");
        return 0;
    }


    // Flux de lecture

    //std::ifstream fichier(fichierSource.c_str());
    FILE* fichier = NULL;
    fichier = fopen(fichierSource, "r");

    // Test d'ouverture

    if(fichier == NULL)
    {
        printf("Erreur le fichier %s est introuvable \n", fichierSource);
        glDeleteShader(*shader);

        return 0;
    }


    // Strings permettant de lire le code source

    //char* ligne;
    int n = 1;
    char* codeSource = malloc(n * sizeof(char));
    int caractereActuel = fgetc(fichier);;

    // Lecture

    //while(getline(fichier, ligne))
    //    codeSource += ligne + '\n';

    // Boucle de lecture des caractères un à un
    while (caractereActuel != EOF) // On continue tant que fgetc n'a pas retourné EOF (fin de fichier)
    {
        codeSource[n-1] = caractereActuel; // On Ajoute au codeSource
        n += 1;
        codeSource = realloc(codeSource, n * sizeof(char));
        caractereActuel = fgetc(fichier); // On lit le caractère
    }
    codeSource[n-1] = '\0';

    // Fermeture du fichier

    fclose(fichier);


    // R�cup�ration de la chaine C du code source

    const GLchar* chaineCodeSource = codeSource;

    // Envoi du code source au shader
    glShaderSource(*shader, 1, &chaineCodeSource, 0);

    // Compilation du shader

    glCompileShader(*shader);



    // V�rification de la compilation

    GLint erreurCompilation = 0;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &erreurCompilation);


    // S'il y a eu une erreur
    if(erreurCompilation != GL_TRUE)
    {
        // Recuperation de la taille de l'erreur
        GLint tailleErreur = 0;
        glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &tailleErreur);


        // Allocation de memoire

        //char *erreur = new char[tailleErreur + 1];
        char *erreur = malloc(tailleErreur + 1 * sizeof(char));


        // R�cup�ration de l'erreur

        glGetShaderInfoLog(*shader, tailleErreur, &tailleErreur, erreur);
        erreur[tailleErreur] = '\0';


        // Affichage de l'erreur

        printf("Error de compilation : %s \n", erreur);

        // Lib�ration de la m�moire et retour du bool�en false

        //delete[] erreur;
        free(erreur);
        glDeleteShader(*shader);
        free(codeSource);

        return 0;
    }


    // Sinon c'est que tout s'est bien pass�

    else
    {
        free(codeSource);
        return 1;
    }
}


// Getter

GLuint Shader_getProgramID(Shader * me)
{
    return me->m_programID;
}
