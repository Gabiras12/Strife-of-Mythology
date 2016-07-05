#ifndef SOMTD_MENU_LEVEL_H
#define SOMTD_MENU_LEVEL_H

#include <ijengine/level.h>
#include <ijengine/texture.h>
#include <ijengine/game_events_listener.h>

#include <string>

#include <memory>
#include "player.h"

namespace SoMTD {
    class MenuLevel : public ijengine::Level, public ijengine::GameEventsListener {
    public:
        MenuLevel(std::string current_map_name, std::string next_map_name, std::string p_audio);
        ~MenuLevel();
        void finish();
        void exit_game();

    protected:
        void draw_self(ijengine::Canvas *c, unsigned, unsigned);
        void draw_self_after(ijengine::Canvas *c, unsigned, unsigned);
        void update_self(unsigned now, unsigned last);
        bool on_event(const ijengine::GameEvent&);
        std::string audio() const;
        std::string next() const;
        bool done() const;
        Player *player() const;

    private:
        std::string m_next;
        std::string m_audio="";
        bool m_done;
        std::shared_ptr<ijengine::Texture> m_texture;
        void load_buttons();
        Player* m_player;
    };
}

#endif
