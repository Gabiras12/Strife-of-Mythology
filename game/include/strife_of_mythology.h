#ifndef STRIFE_OF_MYTHOLOGY_H
#define STRIFE_OF_MYTHOLOGY_H

#include <ijengine/game.h>
#include <ijegine/engine.h>
#include <ijengine/events_translator.h>

#include "strife_of_mythology_level_factory.h"

using namespace ijengine;

class StrifeOfMythology {
public:
    StrifeOfMythology(const string& title, int w, int h);
    ~StrifeOfMythology;

    int run(const string& level_id);
private:
    Game m_game;
    Engine m_engine;
    EventsTranslator m_translator;
    StrifeOfMythologyLevelFactory m_factory;
};

#endif

