#ifndef COLOR_LEVEL_H
#define COLOR_LEVEL_H

#include <ijengine/level.h>
#include <ijengine/texture.h>

#include <string>
#include <memory>

using std::pair;
using std::string;
using std::shared_ptr;

using ijengine::Level;
using ijengine::Canvas;
using ijengine::Texture;

class ColorLevel : public Level {
public:
    ColorLevel(int r, int g, int b, const string& next = "");

    bool done() const;
    string next() const;

protected:
    void update_self(unsigned now, unsigned last);
    void draw_self(Canvas *canvas, unsigned now, unsigned last);

private:
    int m_r, m_g, m_b;
    bool m_done;
    string m_next;
    int m_start;

    shared_ptr<Texture> m_texture;

    pair<int, int> screen_coordinates(int map_x, int map_y, int tw, int th);
};

#endif
