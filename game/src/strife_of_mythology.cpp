#include "strife_of_mythology.h"

#include <ijengine/system_event.h>

StrifeOfMythology::StrifeOfMythology(const string& title, int w, int h) :
    m_game(title, w, h),
    m_engine(),
    m_level_factory()
{
    m_translator.add_translation(SystemEvent(0, SystemEvent::QUIT), GameEvent(GAME_EVENT_QUIT));
    event::register_translator(&m_translator);
    level::register_factory(&m_level_factory);
}

StrifeOfMythology::~StrifeOfMythology()
{
    level::unregister_factory();
    event::unregister_translator(&m_translator);
}

int
StrifeOfMythology::run(const string& level_id)
{
    return m_game.run(level_id);
}
