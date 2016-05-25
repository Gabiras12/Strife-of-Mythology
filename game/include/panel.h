#ifndef SOMTD_PANEL_H
#define SOMTD_PANEL_H

#include <ijengine/game_object.h>
#include <ijengine/game_events_listener.h>
#include <ijengine/texture.h>

#include "player.h"

#include <memory>

namespace SoMTD {
    class Panel : public ijengine::GameObject, public ijengine::GameEventsListener {
    public:
        Panel(std::string texture_name, unsigned id, int x, int y, Player *m = nullptr);
        ~Panel();

        void draw_self(ijengine::Canvas *canvas, unsigned now, unsigned last);
        bool on_event(const ijengine::GameEvent& event);
        void update_self(unsigned, unsigned);

    private:
        unsigned m_id;
        std::shared_ptr<ijengine::Texture> m_texture;
        int m_x;
        int m_y;
        int m_w;
        int m_h;
        int m_start;
        bool m_done;
        int m_priority;
        std::string texture_name_path;
        Player *m_player;
    };
}

#endif
