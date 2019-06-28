#include "Paper.h"

void Paper_init(Paper * paper, int width, int height)
{
    paper->m_width = width;
    paper->m_height = height;
    float rect[] = {-1.0, -1.0, -1.0, 1.0, 1.0, 1.0,
                  -1.0, -1.0, 1.0, -1.0, 1.0, 1.0};
    for(int i=0; i<12; i+=2)
    {
        paper->m_rect[i] = rect[i] / (float) width;
        paper->m_rect[i+1] = rect[i+1] / (float) height;
    }

    paper->m_total_rects = 0;
    paper->m_vertices = malloc(paper->m_total_rects * 12 * sizeof(float));
    paper->m_colors = malloc(paper->m_total_rects * 18 * sizeof(float));

    paper->m_position = (int **)malloc(paper->m_height * sizeof(int *));
    for(int i=0; i<paper->m_height; i++)
    {
        paper->m_position[i] = (int *)malloc(paper->m_width * sizeof(int));
        for(int j=0; j<paper->m_width; j++)
        {
            paper->m_position[i][j] = 0;
        }
    }
}

void Paper_destroy(Paper * paper)
{
    free(paper->m_vertices);
    free(paper->m_colors);
    for(int i=0; i<paper->m_height; i++)
    {
        free(paper->m_position[i]);
    }
    free(paper->m_position);
}

void Paper_add(Paper * paper, int x, int y, vec3 color)
{
    if(paper->m_position[y][x] == 0)
    {
        // Add the number of rects
        paper->m_total_rects += 1;
        // Add the rect
        paper->m_vertices = realloc(paper->m_vertices, paper->m_total_rects * 12 * sizeof(float));
        for(int i=0; i<12; i+=2)
        {
            paper->m_vertices[(paper->m_total_rects-1) * 12 + i] = ( paper->m_rect[i] + ((float)x / (float)paper->m_width) ) * 2.0f - 1.0f;
            paper->m_vertices[(paper->m_total_rects-1) * 12 + i + 1] = ( paper->m_rect[i+1] + ((float)y / (float)paper->m_height) ) * (-2.0f) + 1.0f;
        }
        // Add the Color
        paper->m_colors = realloc(paper->m_colors, paper->m_total_rects * 18 * sizeof(float));
        for(int i=(paper->m_total_rects-1)*18; i<paper->m_total_rects*18; i+=3)
        {
            for(int j=0; j<3; j++)
            {
                paper->m_colors[i+j] = color[j];
            }
        }
        // This position is in use and save the position of the rect in m_verticies
        paper->m_position[y][x] = paper->m_total_rects;
    }
}

void Paper_clear(Paper * paper)
{
    paper->m_total_rects = 0;
    paper->m_vertices = realloc(paper->m_vertices, paper->m_total_rects * 12 * sizeof(float));
    paper->m_colors = realloc(paper->m_colors, paper->m_total_rects * 18 * sizeof(float));
    for(int i=0; i<paper->m_height; i++)
    {
        for(int j=0; j<paper->m_width; j++)
        {
            paper->m_position[i][j] = 0;
        }
    }
}
