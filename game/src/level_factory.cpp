#include "map_level.h"
#include "level_factory.h"
#include "geometric_level.h"
#include "menu_level.h"

#include <string>

ijengine::Level *
SoMTD::LevelFactory::make_level(const string& level_id)
{
    if (level_id == "mainmenu")
        return new SoMTD::MenuLevel("mainmenu", "map001", " ");
    else if (level_id == "map001")
        return new SoMTD::MapLevel("map002", "map001");
    else if (level_id == "map002")
        return new SoMTD::MapLevel("map003", "map002");
    else if (level_id == "map003")
        return new SoMTD::GeometricLevel("ihdashdu");

    return nullptr;
}

void
SoMTD::LevelFactory::release(ijengine::Level *level)
{
    delete level;
}
