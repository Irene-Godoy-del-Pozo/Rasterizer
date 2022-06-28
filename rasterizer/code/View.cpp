


#include "View.hpp"

namespace example
{

    void View::paint ()
    {
        Point< int, 2 > points[6];

        points[0].coordinates[0] = 445; points[0].coordinates[1] = 100;
        points[1].coordinates[0] = 600; points[1].coordinates[1] = 240;
        points[2].coordinates[0] = 550; points[2].coordinates[1] = 440;
        points[3].coordinates[0] = 355; points[3].coordinates[1] = 500;
        points[4].coordinates[0] = 200; points[4].coordinates[1] = 360;
        points[5].coordinates[0] = 250; points[5].coordinates[1] = 160;

        int indices[] = { 0, 1, 2, 3, 4, 5 };

        rasterizer.set_color (0, 0, 255);


       // rasterizer.fill_convex_polygon (points, indices, indices + 6);

        rasterizer.fill_rectangle (100, 100, 200, 300,3);

       rasterizer.stroke_rectangle(100, 100, 200, 300,1);

        rasterizer.get_pixel_buffer ().gl_draw_pixels (0, 0);
    }

}
