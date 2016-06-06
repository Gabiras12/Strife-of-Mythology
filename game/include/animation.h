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
            TOTAL = 0x02
        };

        Animation(int new_grid_x, int new_grid_y, std::string new_file_path, StateStyle new_state_style, int new_frame_per_state, int new_total_states, int new_total_frames);

        ~Animation();

        virtual void draw(ijengine::Canvas *c, unsigned, unsigned) {
            ijengine::Rectangle rect(0, 0, m_width, m_height);
            // printf("width: %d\n", m_width);
            if (m_state_style ==  StateStyle::STATE_PER_COLUMN)
                rect.set_position(m_width*m_actual_state, m_height*m_actual_frame);
            else
                rect.set_position(m_width*m_actual_frame, m_height*m_actual_state);

            c->draw(m_texture.get(), rect, m_screen_position.first, m_screen_position.second);
        }

        void next_frame() {
            if (m_actual_frame == m_total_frames-1) {
                m_actual_frame = 0;
            } else {
                m_actual_frame ++;
            }
        }

    public:
        std::pair<int, int> m_screen_position;
        std::pair<int, int> m_tile;
        std::string m_file_path;
        int m_width;
        int m_height;
        int m_frame_per_state;
        int m_actual_state;
        int m_actual_frame;
        int m_total_states;
        int m_total_frames;
        StateStyle m_state_style;
        std::shared_ptr<ijengine::Texture> m_texture;
        ijengine::Rectangle *m_frame;
    };
}

#endif
