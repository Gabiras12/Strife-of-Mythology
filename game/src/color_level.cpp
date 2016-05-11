#include "color_level.h"

#include <ijengine/canvas.h>

using namespace ijengine;

ColorLevel::ColorLevel(int r, int g, int b, const string& next_level)
    : m_r(r), m_g(g), m_b(b), m_done(false), m_next(next_level), m_start(-1)
{

}

