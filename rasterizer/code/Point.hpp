
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Started by Ángel on december of 2013                                       *
 *                                                                             *
 *  This is free software released into the public domain.                     *
 *                                                                             *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <cstddef>

namespace example
{

    template< typename NUMERIC_TYPE, size_t DIMENSION >
    struct Point
    {
    public:

        typedef NUMERIC_TYPE numeric_type;
        static  const size_t dimension = DIMENSION;

    public:

        numeric_type coordinates[dimension];

    };

}
