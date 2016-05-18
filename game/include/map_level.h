#ifndef SOMTD_MAP_LEVEL_H
#define SOMTD_MAP_LEVEL_H

#include <ijengine/level.h>
#include <ijengine/texture.h>

#include <string>
#include <memory>
#include <fstream>

namespace SoMTD {
    class MapLevel : public ijengine::Level {
    public:
        MapLevel(const string& actual_map = "", const string& next_map = "");
        bool done() const;
        string next() const;
        int grid[80][80];
        void load_config_from_file();
        void load_tiles();

    protected:
        void update_self(unsigned now, unsigned last);
        void draw_self(ijengine::Canvas *canvas, unsigned now, unsigned last);

    private:
        bool m_done;
        std::string m_next;
        int m_start;
        std::string m_current;

        std::shared_ptr< ijengine::Texture > m_texture;
        std::pair<int, int> screen_coordinates(int map_x, int map_y, int tw, int th);
    };
}

#endif

