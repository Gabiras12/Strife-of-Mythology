#include "movable_unit.h"

#include "game.h"

#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/texture.h>

SoMTD::MovableUnit::MovableUnit(std::pair<int, int> s_pos, std::pair<int, int> e_pos, std::string t_path) :
    m_enemy(false),
    end_position(e_pos),
    start_position(s_pos),
    m_texture(ijengine::resources::get_texture(t_path)),
    m_active(false)
{
    ijengine::event::register_listener(this);
    desired_place.first = 0;
    desired_place.second = 0;
}

SoMTD::MovableUnit::~MovableUnit()
{
    ijengine::event::unregister_listener(this);
}

void
SoMTD::MovableUnit::update_self(unsigned start, unsigned end)
{
    if (m_moving) {
        printf("m_moving...\n");
        if (m_x == desired_place.first && m_y == desired_place.second) {
            if (instructions_queue.empty()) {
                printf("empty queue..\n");
                m_moving = false;
            } else {
                printf("poping new instruction..\n");
                move(instructions_queue.front().first, instructions_queue.front().second);
                instructions_queue.pop();
            }
        }

        if (m_x > desired_place.first)
            m_x -= 1;
        else if (m_x < desired_place.first)
            m_x += 1;

        if (m_y > desired_place.second)
            m_y -= 1;
        else if (m_y < desired_place.second)
            m_y += 1;

    }

    if (not m_active) {
        if ((start % 1000) == 0) {
            printf("spawn!\n");
            spawn();
            m_moving = true;
        }
    }
}

bool
SoMTD::MovableUnit::on_event(const ijengine::GameEvent& event)
{
    return false;
}

void
SoMTD::MovableUnit::draw_self(ijengine::Canvas *c, unsigned, unsigned)
{
    if (m_active) {
        std::pair<int, int> actual_grid_position = SoMTD::tools::isometric_to_grid(m_x, m_y, 100, 81, 1024/2, 11);
        std::pair<int, int> p = SoMTD::tools::isometric_adjust(m_x, m_y, 1024/2, 11, 100, 81, actual_grid_position.first, actual_grid_position.second);
        c->draw(m_texture.get(), p.first, p.second);
    }
}

void
SoMTD::MovableUnit::draw_self_after(ijengine::Canvas *c, unsigned, unsigned)
{

}

bool
SoMTD::MovableUnit::enemy() const
{
    return m_enemy;
}

void
SoMTD::MovableUnit::spawn()
{
    m_active = true;
}

bool
SoMTD::MovableUnit::active() const
{
    return m_active;
}

void
SoMTD::MovableUnit::move(int x, int y)
{
    printf("move to %d, %d\n", x, y);
    m_moving = true;
    std::pair<int, int> destiny = std::make_pair(x, y);
    const int tile_width = 100;
    const int tile_height = 81;
    std::pair<int, int> dest_canvas = SoMTD::tools::grid_to_isometric_canvas(destiny.first, destiny.second, tile_width, tile_height);
    desired_place = dest_canvas;
}

void
SoMTD::MovableUnit::add_instruction(int instruction, int paramA, int paramB)
{
    switch (instruction) {
        case 0x00:
            printf("adding instruction\n");
            instructions_queue.push(std::make_pair(paramA, paramB));
            break;

        default:
            break;
    }
}
