#ifndef SOMTD_GAME_H
#define SOMTD_GAME_H

#include <ijengine/game.h>
#include <ijengine/engine.h>
#include <ijengine/events_translator.h>

#include "level_factory.h"

namespace SoMTD {
    class Game {
    public:
        Game(const string& title, int w, int h);
        ~Game();
        void load_keyboard_translations();
        int run(const string& level_id);

    private:
        ijengine::Game m_game;
        ijengine::Engine m_engine;
        ijengine::EventsTranslator m_translator;
        LevelFactory m_level_factory;
    };
}

#endif

