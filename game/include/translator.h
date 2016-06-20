#ifndef SOMTD_TRANSLATOR_H
#define SOMTD_TRANSLATOR_H

#include <ijengine/game.h>
#include <ijengine/engine.h>
#include <ijengine/mouse_event.h>
#include <ijengine/system_event.h>
#include <ijengine/keyboard_event.h>
#include <ijengine/events_translator.h>

namespace SoMTD {
    class Translator : public ijengine::EventsTranslator {
    public:
        const unsigned MOVEMENT = ijengine::GameEvent::assign_id();
        const unsigned MOTION = ijengine::GameEvent::assign_id();
        const unsigned CLICK = ijengine::GameEvent::assign_id();
        const unsigned BUILD_TOWER = ijengine::GameEvent::assign_id();
        const unsigned UPGRADE_TOWER = ijengine::GameEvent::assign_id();
        const unsigned SPAWN_UNIT = ijengine::GameEvent::assign_id();
        bool translate(ijengine::GameEvent& to, const ijengine::MouseEvent& from);
        bool translate(ijengine::GameEvent& to, const ijengine::SystemEvent& from);
        bool translate(ijengine::GameEvent& to, const ijengine::JoystickEvent& from);
        virtual bool translate(ijengine::GameEvent& to, const ijengine::KeyboardEvent& from);
    };
}

#endif

