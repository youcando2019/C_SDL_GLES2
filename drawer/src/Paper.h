#ifndef PAPER_H_INCLUDED
#define PAPER_H_INCLUDED

    #include <cglm/cglm.h>
    #include <malloc.h>

    typedef struct Paper Paper;
    struct Paper
    {
        float m_rect[12];
        int m_total_rects;
        float * m_vertices;
        float * m_colors;
        int m_width;
        int m_height;
        int ** m_position;
    };
    void Paper_init(Paper * paper, int width, int height);
    void Paper_destroy(Paper * paper);
    void Paper_add(Paper * paper, int x, int y, vec3 color);
    void Paper_clear(Paper * paper);

#endif // PAPER_H_INCLUDED
