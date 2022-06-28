
#ifndef VIEW_HEADER
#define VIEW_HEADER

    #include "Rasterizer.hpp"
    #include "Pixel_Buffer_Rgb565.hpp"
    #include "Pixel_Buffer_Rgba8888.hpp"

    namespace example
    {

        class View
        {
        private:

            typedef Pixel_Buffer_Rgb565 Pixel_Buffer;

        private:

            size_t width;
            size_t height;

            Pixel_Buffer               pixel_buffer;
            Rasterizer< Pixel_Buffer > rasterizer;

        public:

            View(size_t width, size_t height)
            :
                width       (width ),
                height      (height),
                pixel_buffer(width, height),
                rasterizer  (pixel_buffer )
            {
            }

            void paint ();

        };

    }

#endif
