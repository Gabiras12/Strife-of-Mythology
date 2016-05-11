#ifndef STRIFE_OF_MYTHOLOGY_LEVEL_FACTORY_H
#define STRIFE_OF_MYTHOLOGY_LEVEL_FACTORY_H

using namespace ijengine;

class StrifeOfMythologyLevelFactory : public LevelFactory {
public:
    Level *make_level(const string& level_id);
    void release(Level *level);
};

#endif

