#ifndef SOMTD_TOWER_H
#define SOMTD_TOWER_H

#include <ijengine/game_object.h>
#include <ijengine/game_events_listener.h>
#include <ijengine/texture.h>

#include <memory>

namespace SoMTD {
    class Tower : public ijengine::GameObject, public ijengine::GameEventsListener {
    public:
        Tower(std::string texture_name, unsigned id, int x, int y);
        ~Tower();

        void draw_self(ijengine::Canvas *canvas, unsigned now, unsigned last);
        void draw_self_after(ijengine::Canvas *, unsigned, unsigned) {}
        bool on_event(const ijengine::GameEvent& event);
        void update_self(unsigned, unsigned);
        std::pair<int, int> screen_coordinates(int map_x, int map_y, int tw, int th);
        double m_range = 200.0;
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
        int canvas_x = 1;
        int canvas_y = 1;
        bool m_mouseover = false;
    };
}

#endif
