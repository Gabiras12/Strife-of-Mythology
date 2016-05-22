#ifndef SOMTD_TEXTURE_BAR_H
#define SOMTD_TEXTURE_BAR_H

#include <ijengine/game_object.h>
#include <ijengine/game_events_listener.h>
#include <ijengine/texture.h>

#include <memory>

#include "player.h"

namespace SoMTD {
    class TextureBar : public ijengine::GameObject, public ijengine::GameEventsListener {
    public:
        TextureBar(std::string texture_name, unsigned id, int x, int y, SoMTD::Player* p, int s_off, int e_off);
        ~TextureBar();

        void draw_self(ijengine::Canvas *canvas, unsigned now, unsigned last);
        bool on_event(const ijengine::GameEvent& event);
        void update_self(unsigned, unsigned);

    private:
        std::shared_ptr<ijengine::Texture> m_texture;
        unsigned m_id;
        int m_x;
        int m_y;
        int m_w;
        int m_h;
        int m_start;
        bool m_done;
        int m_priority;
        SoMTD::Player *m_player;
        double m_percentage;
        int m_start_offset;
        int m_end_offset;
    };
}

#endif
