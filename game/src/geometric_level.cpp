#include "geometric_level.h"
#include "game.h"

#include <ijengine/engine.h>
#include <ijengine/canvas.h>
#include <ijengine/rectangle.h>

SoMTD::GeometricLevel::GeometricLevel(std::string) :
    m_done(false),
    m_next("")
{
}

SoMTD::GeometricLevel::~GeometricLevel()
{
}

void
SoMTD::GeometricLevel::draw_self(ijengine::Canvas *c, unsigned a1, unsigned)
{
    int aux = 0;
    if ((a1 % 50) == 0) {
        aux = 10;
    }
    c->clear();
    for (int i=0; i < 15; ++i) {
        if ((i % 3) == 0) {
            for (int w=0; w < 1000; ++w) {
                c->draw(ijengine::Rectangle(100+aux*20, 100, 15*i, 100));
            }
        } else {
            c->draw(ijengine::Rectangle(7*i, 100, 15, 20*aux));
        }
    }
}

void
SoMTD::GeometricLevel::update_self(unsigned, unsigned)
{
}

bool
SoMTD::GeometricLevel::done() const
{
    return m_done;
}

std::string
SoMTD::GeometricLevel::next() const
{
    return m_next;
}

std::string
SoMTD::GeometricLevel::audio() const
{
    return m_audio;
}

bool
SoMTD::GeometricLevel::on_event(const ijengine::GameEvent &)
{
    return false;
}
