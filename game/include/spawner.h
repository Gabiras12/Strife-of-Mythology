#ifndef SOMTD_SPAWNER_H
#define SOMTD_SPAWNER_H

#include <ijengine/engine.h>
#include <ijengine/game_object.h>
#include <ijengine/game_events_listener.h>

namespace SoMTD {
    template <class T>
    class Spawner : public ijengine::GameObject, public ijengine::GameEventsListener {
    public:
        Spawner(T *prototype) {
            m_prototype = prototype;
            ijengine::event::register_listener(this);
        };

        ~Spawner() {
            ijengine::event::unregister_listener(this);
            for (auto it : units) {
                delete it;
            }
        };

        T *spawn_unit() {
            T* new_unit = m_prototype->clone();
            units.push_back(new_unit);
            new_unit->spawn();
        };

        void update_self(unsigned a1, unsigned a2) {
            for (auto it=units.begin(); it != units.end(); ++it) {
                if ((*it)->active() == false) {
                    it = units.erase(it);
                } else {
                    (*it)->update_self(a1, a2);
                }
            }
        }

        void draw_self(ijengine::Canvas *c, unsigned a1, unsigned a2) {
            for (auto it : units) {
                it->draw_self(c, a1, a2);
            }
        }

        bool on_event(const ijengine::GameEvent& event) {
            if (event.id() == SoMTD::SPAWN_UNIT) {
                spawn_unit();
            }
        }

    private:
        T *m_prototype;
        std::list<T*> units;
    };
}

#endif

