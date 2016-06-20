#ifndef SOMTD_MAP_LEVEL_H
#define SOMTD_MAP_LEVEL_H

#include <ijengine/game_object.h>
#include <ijengine/game_events_listener.h>
#include <ijengine/level.h>
#include <ijengine/texture.h>

#include <string>
#include <memory>
#include <fstream>
#include <vector>

#include "player.h"
#include "movable_unit.h"
#include "luascript.h"
#include "labyrinth.h"
#include "spawner.h"
#include "wave.h"

namespace SoMTD {
    class MapLevel : public ijengine::Level, public ijengine::GameEventsListener {
    public:
        MapLevel(const string& actual_map = "", const string& next_map = "", const string& audio_path = "");
        ~MapLevel();
        bool done() const;
        string next() const;
        string audio() const;
        void load_map_from_file();
        void load_tiles();
        bool on_event(const ijengine::GameEvent& event);
        void load_hud();
        void load_panels();
        void load_buttons();
        void load_spawners();

    protected:
        void draw_help_text(ijengine::Canvas *c);
        void update_self(unsigned now, unsigned last);
        void draw_self(ijengine::Canvas *canvas, unsigned now, unsigned last);
        void draw_self_after(ijengine::Canvas *, unsigned, unsigned);

    private:
        void build_tower(unsigned tower_id, int x, int y);
        std::string m_next;
        std::string m_current;
        std::string m_audio_path;
        bool m_done;
        Player *m_player;
        int m_start;
        std::shared_ptr< ijengine::Texture > m_texture;
        LuaScript *m_actions;
        std::pair<int, int> origin;
        std::pair<int, int> destiny;
        std::vector< std::pair<int, int> > m_unit_path;
        Labyrinth *m_labyrinth;
        std::vector< SoMTD::Spawner<MovableUnit> *> spawners;
        void fetch_waves_from_file();
        std::vector< SoMTD::Wave *> m_waves;
        int m_current_wave = 0;
        void start_wave();
    };
}

#endif

