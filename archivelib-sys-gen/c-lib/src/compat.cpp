

#include "compat.hpp"


extern "C" ALGreenleafEngine* AL_FUNCTION newALGreenleafEngine( int level )
{
    return new ALGreenleafEngine( (short int) level, 1 );
}
