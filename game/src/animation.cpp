#include "animation.h"
#include "game.h"

SoMTD::Animation::Animation(int new_grid_x, int new_grid_y, std::string new_file_path, StateStyle new_state_style, int new_frame_per_state, int new_total_states)
{
    m_tile = std::make_pair(new_grid_x, new_grid_y);
    m_screen_position = SoMTD::tools::grid_to_isometric(m_tile.first, m_tile.second, 100, 81, 1024/2, 11);
    m_file_path = new_file_path;
    m_texture = ijengine::resources::get_texture(new_file_path);
    m_frame_per_state = new_frame_per_state;
    m_state_style = new_state_style;
    m_frame_per_state = new_frame_per_state;
    m_total_states = new_total_states;
    m_actual_state = 0;
    m_actual_frame = 0;

    if (new_state_style == Animation::StateStyle::STATE_PER_COLUMN) {
        m_width = m_texture->w()/m_total_states;
        m_height = m_texture->h()/m_frame_per_state;
    } else if (new_state_style == Animation::StateStyle::STATE_PER_LINE) {
        m_width = m_texture->w()/m_frame_per_state;
        m_height = m_texture->h()/m_total_states;
    } else if (new_state_style == Animation::StateStyle::EVERYTHING_PER_LINE) {
        m_width = m_texture->w()/(m_total_states);
        m_width /= m_frame_per_state;
        m_height = m_texture->h();
    }
}

void
SoMTD::Animation::update_screen_position(std::pair<int, int> new_pos)
{
    m_screen_position = new_pos;
}

SoMTD::Animation::~Animation()
{
}

void
SoMTD::Animation::next_frame()
{
    if (m_actual_frame == m_frame_per_state-1) {
        m_actual_frame = 0;
    } else {
        m_actual_frame ++;
    }
}

void
SoMTD::Animation::draw(ijengine::Canvas *c, unsigned, unsigned)
{
    ijengine::Rectangle rect(0, 0, m_width, m_height);
    if (m_state_style ==  StateStyle::STATE_PER_COLUMN)
        rect.set_position(m_width*m_actual_state, m_height*m_actual_frame);
    else if (m_state_style == StateStyle::STATE_PER_LINE)
        rect.set_position(m_width*m_actual_frame, m_height*m_actual_state);
    else if (m_state_style == 0x02)
        rect.set_position((m_actual_frame*m_width)+(m_width*m_frame_per_state*m_actual_state), 0);

    m_texture = ijengine::resources::get_texture(m_file_path);
    c->draw(m_texture.get(), rect, m_screen_position.first+(100-m_width)/2, m_screen_position.second+81/2-m_height);
}

void
SoMTD::Animation::draw_self_after(ijengine::Canvas*, unsigned, unsigned)
{

}

std::pair<int, int>
SoMTD::Animation::screen_position() const
{
    return m_screen_position;
}

std::pair<int, int>
SoMTD::Animation::tile() const
{
    return m_tile;
}

void
SoMTD::Animation::update_texture(std::string new_path)
{
    m_file_path = new_path;
    m_texture = ijengine::resources::get_texture(m_file_path);
}

std::shared_ptr<ijengine::Texture>
SoMTD::Animation::texture() const
{
    return m_texture;
}

int
SoMTD::Animation::width() const
{
    return m_width;
}

int
SoMTD::Animation::height() const
{
    return m_height;
}

int
SoMTD::Animation::frame_per_state() const
{
    return m_frame_per_state;
}

void
SoMTD::Animation::update_direction(SoMTD::Animation::DirectionState ds)
{
    if (m_total_states >= (int)ds)
        m_actual_state = (int)ds;
}
