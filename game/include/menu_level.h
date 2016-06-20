#ifndef MENU_LEVEL_H
#define MENU_LEVEL_H

#include <ijengine/level.h>
#include <ijengine/texture.h>
#include <ijengine/game_events_listener.h>

#include <string>

#include <memory>

namespace SoMTD {
    class MenuLevel : public ijengine::Level, public ijengine::GameEventsListener {
    public:
        MenuLevel(std::string current_map_name, std::string next_map_name, std::string p_audio);
        ~MenuLevel();

    protected:
        void draw_self(ijengine::Canvas *c, unsigned, unsigned);
        void draw_self_after(ijengine::Canvas *c, unsigned, unsigned);
        void update_self(unsigned now, unsigned last);
        bool on_event(const ijengine::GameEvent&);
        std::string audio() const;
        std::string next() const;
        bool done() const;

    private:
        std::string m_next;
        bool m_done;
        std::string m_audio="";
        std::shared_ptr<ijengine::Texture> m_texture;

    };
}

#endif
