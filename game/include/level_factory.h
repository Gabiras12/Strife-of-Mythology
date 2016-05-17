#ifndef SOMTD_LEVEL_FACTORY
#define SOMTD_LEVEL_FACTORY

#include <ijengine/level_factory.h>

namespace SoMTD {
    class LevelFactory : public ijengine::LevelFactory {
    public:
        ijengine::Level *make_level(const string& level_id);
        void release(ijengine::Level *level);
    };
}

#endif

