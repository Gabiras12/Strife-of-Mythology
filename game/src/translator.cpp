#include "translator.h"

#include <ijengine/game.h>
#include <ijengine/engine.h>
#include <ijengine/mouse_event.h>
#include <ijengine/keyboard_event.h>
#include <ijengine/system_event.h>

#include "game.h"

bool
SoMTD::Translator::translate(ijengine::GameEvent& to, const ijengine::MouseEvent& from)
{
    to.set_timestamp(from.timestamp());
    to.set_property<int>("x", from.x());
    to.set_property<int>("y", from.y());

    if (from.state() == ijengine::MouseEvent::MOTION)
        to.set_id(SoMTD::Translator::MOTION);
    else
        to.set_id(SoMTD::Translator::CLICK);

    return true;
}

bool
SoMTD::Translator::translate(ijengine::GameEvent& to, const ijengine::SystemEvent& from)
{
    if (from.action() == ijengine::SystemEvent::QUIT)
    {
        to.set_timestamp(from.timestamp());
        to.set_id(ijengine::game_event::QUIT);

        return true;
    }

    return false;
}

bool
SoMTD::Translator::translate(ijengine::GameEvent& to, const ijengine::KeyboardEvent& from)
{
    to.set_timestamp(from.timestamp());

    bool done = true;
    int id = 0;

    switch (from.key()) {
        case ijengine::KeyboardEvent::ESCAPE:
            id = ijengine::game_event::QUIT;
            break;

        case ijengine::KeyboardEvent::UP:
            id = SoMTD::Translator::MOVEMENT;
            to.set_property<string>("direction", "up");
            break;

        case ijengine::KeyboardEvent::DOWN:
            id = SoMTD::Translator::MOVEMENT;
            to.set_property<string>("direction", "down");
            break;

        case ijengine::KeyboardEvent::B:
            id = SoMTD::Translator::BUILD_TOWER;
            break;

        case ijengine::KeyboardEvent::M:
            id = 777;
            break;

        case ijengine::KeyboardEvent::U:
            id = SoMTD::UPGRADE_TOWER;
            break;

        case ijengine::KeyboardEvent::S:
            id = SoMTD::SPAWN_UNIT;
            break;

        default:
            done = false;
    }

    to.set_id(id);

    return done;
}

bool
SoMTD::Translator::translate(ijengine::GameEvent&, const ijengine::JoystickEvent&)
{
    return false;
}
