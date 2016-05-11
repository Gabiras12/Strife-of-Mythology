#include "color_level.h"
#include "strife_of_mythology_level_factory.h"

Level *
StrifeOfMythologyLevelFactory::make_level(const string& level_id)
{
    if (level_id == "red")
        return new ColorLevel(255, 0, 0, "green");
    else if (level_id == "green")
        return new ColorLevel(0, 255, 0, "blue");
    else if (level_id == "blue")
        return new ColorLevel(0, 0, 255, "red");
    else
        return nullptr;
}

void
StrifeOfMythologyLevelFactory::release(Level *level)
{
    delete level;
}
