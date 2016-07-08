#include "map_level.h"
#include "level_factory.h"
#include "geometric_level.h"
#include "menu_level.h"

#include <string>

ijengine::Level *
SoMTD::LevelFactory::make_level(const string& level_id)
{
    if (level_id == "mainmenu")
        return new SoMTD::MenuLevel("mainmenu", "map001", "res/sound_efects/music/somtd-menu.ogg");
    else if (level_id == "map001")
        return new SoMTD::MapLevel("map002", "map001", "res/sound_efects/music/gameplay.ogg");
    else if (level_id == "map002")
        return new SoMTD::MapLevel("map003", "map002", "res/sound_efects/music/gameplay_map2.ogg");
    else if (level_id == "map003")
        return new SoMTD::MapLevel("mainmenu", "map003", "res/sound_efects/music/gameplay_map3.ogg");
    else if (level_id == "menuoptions")
        return new SoMTD::MenuLevel("menuoptions", "mainmenu", "res/sound_efects/music/somtd-menu.ogg");
    else if (level_id == "menucredits")
        return new SoMTD::MenuLevel("menucredits", "map001", "res/sound_efects/music/credits.ogg");

    return nullptr;
}

void
SoMTD::LevelFactory::release(ijengine::Level *level)
{
    delete level;
}
