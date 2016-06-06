#ifndef SOMTD_TOWER_H
#define SOMTD_TOWER_H

#include <ijengine/game_object.h>
#include <ijengine/game_events_listener.h>
#include <ijengine/texture.h>

#include <memory>
#include "player.h"
#include "animation.h"

namespace SoMTD {
    class Tower : public ijengine::GameObject, public ijengine::GameEventsListener {
    public:
        Tower(std::string texture_name, unsigned id, int x, int y, std::string m_imageselected_path, Player *p);
        ~Tower();

        void draw_self(ijengine::Canvas *canvas, unsigned now, unsigned last);
        void draw_self_after(ijengine::Canvas *, unsigned, unsigned) {}
        bool on_event(const ijengine::GameEvent& event);
        void update_self(unsigned, unsigned);
        int level() const;
        int attack() const;
        double range() const;
        unsigned mytimer;
    protected:
        void level_up();

    private:
        double m_range = 200.0;
        int m_level;
        int m_attack;
        std::shared_ptr<ijengine::Texture> m_texture;
        std::string m_image_path;
        unsigned m_id;
        bool m_selected = false;
        int m_x;
        int m_y;
        int m_w;
        int m_h;
        int m_start;
        bool m_done;
        int m_priority;
        int canvas_x;
        int canvas_y;
        bool m_mouseover = false;
        std::string m_imageselected_path;
        Player *m_player;
        Animation *m_animation;
    };
}

#endif
