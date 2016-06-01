#ifndef SOMTD_LEVEL_AREA_H
#define SOMTD_LEVEL_AREA_H

#include <ijengine/game_object.h>
#include <ijengine/game_events_listener.h>
#include <ijengine/texture.h>

#include <memory>

namespace SoMTD {
    class LevelArea : public ijengine::GameObject, public ijengine::GameEventsListener {
    public:
        LevelArea(std::string texture_name, unsigned id, int x, int y, int item_priority = 0);
        ~LevelArea();

        void draw_self_after(ijengine::Canvas *, unsigned, unsigned) { }
        void draw_self(ijengine::Canvas *canvas, unsigned now, unsigned last);
        bool on_event(const ijengine::GameEvent& event);
        void update_self(unsigned, unsigned);

    private:
        std::shared_ptr<ijengine::Texture> m_texture;
        unsigned m_id;
        int m_x;
        int m_y;
        int m_priority;
        int m_w;
        int m_h;
        int m_start;
        bool m_done;
    };
}

#endif
