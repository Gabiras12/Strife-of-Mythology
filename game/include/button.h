#ifndef SOMTD_BUTTON_H
#define SOMTD_BUTTON_H

#include <ijengine/game_object.h>
#include <ijengine/game_events_listener.h>
#include <ijengine/texture.h>

#include "player.h"
#include <memory>

namespace SoMTD {
    class Button : public ijengine::GameObject, public ijengine::GameEventsListener {
    public:
        Button(std::string texture_name, unsigned id, int x, int y, std::string mouseover_texture, Player *m = nullptr);
        ~Button();

        void draw_self(ijengine::Canvas *canvas, unsigned now, unsigned last);
        bool on_event(const ijengine::GameEvent& event);
        void update_self(unsigned, unsigned);
        std::pair<int, int> screen_coordinates(int map_x, int map_y, int tw, int th);
    private:
        std::shared_ptr<ijengine::Texture> m_texture;
        std::shared_ptr<ijengine::Texture> m_mouseover_texture;
        unsigned m_id;
        int m_x;
        int m_y;
        int m_w;
        int m_h;
        int m_start;
        bool m_done;
        int m_priority;
        bool m_mouseover;
        Player *m_player;
    };
}

#endif
