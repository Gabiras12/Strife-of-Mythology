#ifndef SOMTD_GAME_H
#define SOMTD_GAME_H

#include <ijengine/game.h>
#include <ijengine/engine.h>
#include <ijengine/events_translator.h>

#include "level_factory.h"
#include "translator.h"

namespace SoMTD {
    enum {
        MOUSEOVER = 4,
        CLICK = 5,
        BUILD_TOWER = 6,
        SELECT_TOWER = 11,
        UPGRADE_TOWER = 12,
        SPAWN_UNIT = 14
    };
    class Game {
    public:
        Game(const string& title, int w, int h);
        ~Game();
        int run(const string& level_id);

    private:
        ijengine::Game m_game;
        ijengine::Engine m_engine;
        Translator m_translator;
        LevelFactory m_level_factory;
    };
}

#endif

