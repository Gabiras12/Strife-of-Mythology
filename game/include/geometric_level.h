#ifndef SOMTD_GEOMETRIC_LEVEL_H
#define SOMTD_GEOMETRIC_LEVEL_H

#include <ijengine/engine.h>
#include <ijengine/canvas.h>
#include <ijengine/level.h>
#include <ijengine/game_events_listener.h>

namespace SoMTD {
    class GeometricLevel : public ijengine::Level, public ijengine::GameEventsListener {
    public:
        GeometricLevel(std::string pathz);
        ~GeometricLevel();
    protected:
        void draw_self(ijengine::Canvas *c, unsigned, unsigned);
        void update_self(unsigned now, unsigned last);
        bool done() const;
        std::string audio() const;
        std::string next() const;
        bool on_event(const ijengine::GameEvent&);

    private:
        std::string m_audio="";
        bool m_done;
        std::string m_next;
    };
}

#endif

