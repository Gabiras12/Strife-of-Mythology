#ifndef SOMTD_ANIMATION_H
#define SOMTD_ANIMATION_H

#include <ijengine/canvas.h>
#include <ijengine/game_object.h>
#include <ijengine/game_events_listener.h>
#include <ijengine/texture.h>
#include <ijengine/rectangle.h>

#include <memory>

namespace SoMTD {
    class Animation {
    public:
        enum StateStyle {
            STATE_PER_COLUMN = 0x00,
            STATE_PER_LINE = 0x01,
            EVERYTHING_PER_LINE = 0x02,
            TOTAL = 0x03
        };

        enum DirectionState {
            DIRECTION_RIGHT = 0x00,
            DIRECTION_LEFT = 0x01
        };

        Animation(int new_grid_x, int new_grid_y, std::string new_file_path, StateStyle new_state_style, int new_frame_per_state, int new_total_states);

        ~Animation();
        void draw(ijengine::Canvas *c, unsigned, unsigned);
        void draw_self_after(ijengine::Canvas *c, unsigned, unsigned);
        void next_frame();
        void update_texture(std::string new_path);
        void update_screen_position(std::pair<int, int> new_pos);

        std::pair<int, int> screen_position() const;
        std::pair<int, int> tile() const;
        std::shared_ptr<ijengine::Texture> texture() const;
        int width() const;
        int height() const;
        int frame_per_state() const;
        void update_direction(SoMTD::Animation::DirectionState newstate);

    private:
        std::pair<int, int> m_screen_position;
        std::pair<int, int> m_tile;
        std::string m_file_path;
        int m_width;
        int m_height;
        int m_frame_per_state;
        int m_actual_state;
        int m_actual_frame;
        int m_total_states;
        StateStyle m_state_style;
        std::shared_ptr<ijengine::Texture> m_texture;
        ijengine::Rectangle *m_frame;
        SoMTD::Animation::DirectionState m_actual_direction;

    };
}

#endif
