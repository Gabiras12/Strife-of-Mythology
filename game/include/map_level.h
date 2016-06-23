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

        enum State {
            IDLE = 0x00,
            RESTING = 0x01,
            PLAYING = 0x02,
            OVER = 0x03
        };

        MapLevel(const string& actual_map = "", const string& next_map = "", const string& audio_path = "");
        ~MapLevel();

        bool done() const;
        string next() const;
        string audio() const;
        MapLevel::State actual_state() const;

        void load_map_from_file();
        bool on_event(const ijengine::GameEvent& event);
        void load_tiles();
        void load_hud();
        void load_panels();
        void load_buttons();
        void load_spawners();
        Player *player() const;

    protected:
        void draw_help_text(ijengine::Canvas *c);
        void update_self(unsigned now, unsigned last);
        void draw_self(ijengine::Canvas *canvas, unsigned now, unsigned last);
        void draw_self_after(ijengine::Canvas *, unsigned, unsigned);

    private:
        void update_current_wave(unsigned now, unsigned last);
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
        void start_wave(unsigned now);
        int m_wave_index = 0;
        SoMTD::Wave* current_wave();
        MapLevel::State m_actual_state;
        void handle_idle_state(unsigned now, unsigned last);
        unsigned m_state_started_at = 0;
        void transition_to(MapLevel::State from, MapLevel::State to, unsigned now, unsigned last);
        void handle_playing_state(unsigned now, unsigned last);
        void handle_resting_state(unsigned now, unsigned last);
        void draw_selected_panel(ijengine::Canvas *c, unsigned now, unsigned last);
    };
}

#endif

