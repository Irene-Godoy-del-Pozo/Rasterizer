

#ifndef RASTERIZER_HEADER
#define RASTERIZER_HEADER

    #include <stdint.h>
    #include "Point.hpp"
    #include <algorithm>

    namespace example
    {

        template< class PIXEL_BUFFER_TYPE >
        class Rasterizer
        {
        public:

            typedef PIXEL_BUFFER_TYPE Pixel_Buffer;

        private:

            Pixel_Buffer & pixel_buffer;

            int offset_cache0[1082];
            int offset_cache1[1082];

        public:

            Rasterizer(Pixel_Buffer & target)
            :
                pixel_buffer(target)
            {
            }

            const Pixel_Buffer & get_pixel_buffer () const
            {
                return (pixel_buffer);
            }

        public:

            void set_color (int r, int  g, int b)
            {
                pixel_buffer.set_color (r, g, b);
            }

            void fill_convex_polygon
            (
                const Point< int, 2 > * const vertices, 
                const int             * const indices_begin, 
                const int             * const indices_end
            );

            /// <summary>
            /// Fills a rectangle
            /// </summary>
            /// <param name="increment">Every how many lines a pixel will be drawn</param>
            void fill_rectangle (int left_x, int top_y, int width, int height, int increment)
            {
                int offset = 0;

                for (int y = top_y, bottom_y = top_y + height; y < bottom_y; y +=increment )
                {
                    offset = y * pixel_buffer.get_width() + left_x;

                    for (int x = left_x, right_x = left_x + width; x < right_x; ++x)
                    {
						
							pixel_buffer.set_pixel (offset++);
                    }
                   
                }
            }

            /// <summary>
            /// Draws the border of the rectangle 
            /// </summary>
            /// <param name="increment">Every how many lines a pixel will be drawn</param>
            void stroke_rectangle (int left_x, int top_y, int width, int height,int increment)
            {
                
                int offset = 0;

				for (int y = top_y, bottom_y = top_y + height; y < bottom_y; y += increment) 
				{
                    offset = y * pixel_buffer.get_width() + left_x;
                    if (y == bottom_y- increment || y == top_y) //Horizontal lines
                    {
                        
                        for (int x = left_x, right_x = left_x + width; x < right_x; x+= increment)
                        {
                           
                            pixel_buffer.set_pixel(offset+= increment);
                        }
                    }
                    else                                        //Vertical lines
                    {
                        pixel_buffer.set_pixel(offset);
                        pixel_buffer.set_pixel(offset + width);
                    }					
				}
			}           

        private:

            template< typename VALUE_TYPE, size_t SHIFT >
            void interpolate (int * cache, int v0, int v1, int y_min, int y_max);

        };

        template< class  PIXEL_BUFFER_TYPE >
        void Rasterizer< PIXEL_BUFFER_TYPE >::fill_convex_polygon
        (
            const Point< int, 2 > * const vertices, 
            const int             * const indices_begin, 
            const int             * const indices_end
        )
        {
            // Get pointers to important variables

                  int   pitch         = pixel_buffer.get_width ();
                  int * offset_cache0 = this->offset_cache0;
                  int * offset_cache1 = this->offset_cache1;
            const int * indices_back  = indices_end - 1;

            // Gets the starting point (lesser Y) and the ending point (more Y):

            const int * start_index   = indices_begin;
                  int   start_y       = vertices[*start_index].coordinates[1];
            const int * end_index     = indices_begin;
                  int   end_y         = start_y;

            for (const int * index_iterator = start_index; ++index_iterator < indices_end; )
            {
                int current_y = vertices[*index_iterator].coordinates[1];

                if (current_y < start_y)
                {
                    start_y     = current_y; 
                    start_index = index_iterator;
                }
                else
                if (current_y > end_y)
                {
                    end_y       = current_y;
                    end_index   = index_iterator;
                }
            }

            // The X coordinates of the sides from the vertex with the smallest Y to the vertex with the largest Y in the counterclockwise direction 
            
            const int * current_index = start_index;
            const int *    next_index = start_index > indices_begin ? start_index - 1 : indices_back;

            int y0 = vertices[*current_index].coordinates[1];
            int y1 = vertices[*   next_index].coordinates[1];
            int o0 = vertices[*current_index].coordinates[0] + y0 * pitch;
            int o1 = vertices[*   next_index].coordinates[0] + y1 * pitch;

            while (true)
            {
                interpolate< int64_t, 32 > (offset_cache0, o0, o1, y0, y1);

                if (current_index == indices_begin) current_index = indices_back; else current_index--;
                if (current_index == end_index    ) break;
                if (   next_index == indices_begin) next_index    = indices_back; else    next_index--;

                y0 = y1;
                y1 = vertices[*next_index].coordinates[1];
                o0 = o1;
                o1 = vertices[*next_index].coordinates[0] + y1 * pitch;
            }

            int end_offset = o1;

            // The X coordinates of the sides from the vertex with the smallest Y to the vertex with the largest Y in the clockwise  direction 

            current_index = start_index;
               next_index = start_index < indices_back ? start_index + 1 : indices_begin;

            y0 = vertices[*current_index].coordinates[1];
            y1 = vertices[*   next_index].coordinates[1];
            o0 = vertices[*current_index].coordinates[0] + y0 * pitch;
            o1 = vertices[*   next_index].coordinates[0] + y1 * pitch;

            while (true)
            {
                interpolate< int64_t, 32 > (offset_cache1, o0, o1, y0, y1);

                if (current_index == indices_back) current_index = indices_begin; else current_index++;
                if (current_index == end_index   ) break;
                if (   next_index == indices_back) next_index    = indices_begin; else next_index++;

                y0 = y1;
                y1 = vertices[*next_index].coordinates[1];
                o0 = o1;
                o1 = vertices[*next_index].coordinates[0] + y1 * pitch;
            }

            if (o1 > end_offset) end_offset = o1;

            // Fill the scanlines from the smallest Y to the largest Y:

            offset_cache0 += start_y;
            offset_cache1 += start_y;

            while (true)
            {
                o0 = *offset_cache0++;
                o1 = *offset_cache1++;

                if (o0 < o1)
                {
                    while (o0 < o1) pixel_buffer.set_pixel (o0++);

                    if (o0 >= end_offset) break;
                }
                else
                {
                    while (o1 < o0) pixel_buffer.set_pixel (o1++);

                    if (o1 >= end_offset) break;
                }
            }
        }

        template< class  PIXEL_BUFFER_TYPE >
        template< typename VALUE_TYPE, size_t SHIFT >
        void Rasterizer< PIXEL_BUFFER_TYPE >::interpolate (int * cache, int v0, int v1, int y_min, int y_max)
        {
            VALUE_TYPE value = (VALUE_TYPE(     v0) << SHIFT);
            VALUE_TYPE step  = (VALUE_TYPE(v1 - v0) << SHIFT) / (y_max - y_min);

            for (int * iterator = cache + y_min, * end = cache + y_max; iterator <= end; )
            {
               *iterator++ = int(value >> SHIFT);
                value += step;
               *iterator++ = int(value >> SHIFT);
                value += step;
            }
        }

    }

#endif
