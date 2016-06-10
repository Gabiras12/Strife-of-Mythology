#include <vector>

#include "game.h"

#include <ijengine/engine.h>
#include <ijengine/canvas.h>
#include <ijengine/rectangle.h>

#include <fstream>
#include <cstring>
#include <map>
#include <queue>
#include <cstring>
#include <algorithm>

#include "spawner.h"
#include "luascript.h"
#include "map_level.h"
#include "level_area.h"
#include "tower.h"
#include "player.h"
#include "panel.h"
#include "texture_bar.h"
#include "button.h"
#include "movable_unit.h"

SoMTD::MapLevel::MapLevel(const string& next_level, const string& current_level, const string& audio_file_path) :
    m_next(next_level),
    m_current(current_level),
    m_audio_path(audio_file_path),
    m_done(false),
    m_player(new Player),
    m_start(-1),
    m_texture(nullptr)
{
    m_labyrinth = new Labyrinth(10, 10, std::pair<int, int>(0, 0), std::make_pair(0, 0));
    ijengine::event::register_listener(this);
    load_map_from_file();
    load_tiles();
    load_hud();
    m_labyrinth->update_origin(origin);
    m_labyrinth->update_destiny(destiny);
    m_labyrinth->solve();
    std::reverse(m_labyrinth->solution.begin(), m_labyrinth->solution.end());
    load_spawners();

    m_actions = new LuaScript("lua-src/Action.lua");
}

SoMTD::MapLevel::~MapLevel()
{
    delete m_labyrinth;
    delete m_actions;
    delete m_player;
    ijengine::event::unregister_listener(this);
}

void
SoMTD::MapLevel::load_tiles()
{
    std::string _path;
    unsigned _id;
    int line_count = m_labyrinth->m_grid.size();
    for (int line = 0; line < line_count; ++line) {
        int column_count = m_labyrinth->m_grid[line].size();
        for (auto column = 0; column < column_count; ++column) {
            switch (m_labyrinth->m_grid[line][column]) {
                case 1:
                    _path = "caminho1.png";
                    _id = 1;
                break;

                case 2:
                    _path = "caminho2.png";
                    _id = 2;
                break;

                case 3:
                    _path = "curva3.png";
                    _id = 3;
                break;

                case 4:
                    _path = "curva4.png";
                    _id = 4;
                break;

                case 5:
                    _path = "curva1.png";
                    _id = 5;
                break;

                case 6:
                    _path = "tile_grama.png";
                    _id = 6;
                break;

                case 7:
                    _path = "curva2.png";
                    _id = 7;
                break;

                case 8:
                    _path = "waterfallEndS.png";
                    _id = 8;
                break;

                case 50:
                    origin.first = column;
                    origin.second = line;
                    _path = "caminho2.png";
                    _id = 2;
                break;

                case 60:
                    destiny.first = column;
                    destiny.second = line;
                    _path = "caminho2.png";
                    _id = 2;
                break;

                default:
                    continue;
                break;
            }

            add_child(new SoMTD::LevelArea(_path, _id, column, line, 0));
        }
    }
}

void
SoMTD::MapLevel::load_map_from_file()
{
    if (not m_current.empty()) {
        std::string path("res/");
        path = path.append(m_current);
        path = path.append(".txt");

        m_labyrinth->fetch_file(path);
    }
}

bool
SoMTD::MapLevel::done() const
{
    return m_done;
}

std::string
SoMTD::MapLevel::next() const
{
    return m_next;
}

void
SoMTD::MapLevel::update_self(unsigned now, unsigned)
{
    if (m_start == -1)
        m_start = now;
    if (not (now - m_start > 1000))
        m_done = false;
}

void
SoMTD::MapLevel::draw_self(ijengine::Canvas *canvas, unsigned, unsigned)
{
    canvas->clear();
    canvas->draw(ijengine::resources::get_texture("background.png").get(), 0, 0);
    draw_help_text(canvas);
}

void
SoMTD::MapLevel::draw_help_text(ijengine::Canvas *canvas)
{
    std::shared_ptr< ijengine::Texture > help_text;
    if (m_player->state == SoMTD::Player::PlayerState::IDLE) {
        help_text = ijengine::resources::get_texture("press_b.png");
    } else if (m_player->state == SoMTD::Player::PlayerState::HOLDING_BUILD) {
        help_text = ijengine::resources::get_texture("click_to_build.png");
    } else if (m_player->state == SoMTD::Player::PlayerState::INVALID_BUILD) {
        help_text = ijengine::resources::get_texture("invalid_location.png");
    } else if (m_player->state == SoMTD::Player::PlayerState::NOT_ENOUGH_GOLD) {
        help_text = ijengine::resources::get_texture("not_enough_gold.png");
    } else {
        help_text = ijengine::resources::get_texture("click_to_build.png");
    }
    const int window_width = 1024;
    const int y_position = 160;
    canvas->draw(help_text.get(), window_width - help_text->w(), y_position);
}

bool
SoMTD::MapLevel::on_event(const ijengine::GameEvent& event)
{
    if (event.id() == SoMTD::CLICK) {
        double x_pos = event.get_property<double>("x");
        double y_pos = event.get_property<double>("y");

        auto tile_position = SoMTD::tools::isometric_to_grid((int)x_pos, (int)y_pos, 100, 81, 1024/2, 11);

        if (m_player->state == 0x01 || m_player->state == 0x05 || m_player->state == 0x06 || m_player->state == 0x07) {
            if (tile_position.first >= 0 && tile_position.second >= 0 && tile_position.first < 10 && tile_position.second < 10) {
                if (m_player->gold() >= 100) {
                    if (m_labyrinth->m_grid[tile_position.second][tile_position.first] == 6) {
                        m_labyrinth->m_grid[tile_position.second][tile_position.first] = 88;
                        SoMTD::Tower *m_tower = nullptr;
                        if (m_player->state == SoMTD::Player::PlayerState::HOLDING_BUILD) {
                            std::string tower_name("tower_");
                            tower_name.append(std::to_string(m_player->desired_tower));
                            tower_name.append(".png");
                            m_tower = new SoMTD::Tower(tower_name, 9, tile_position.first, tile_position.second, "selected_"+tower_name, m_player);
                            m_tower->set_priority(50000+(5*tile_position.second+5*tile_position.first));
                            add_child(m_tower);
                            m_player->discount_gold(100);
                            m_player->m_hp -= 1;
                        }
                    }
                } else {
                    printf("You need moar gold! (%d)\n", m_player->gold());
                    m_player->state = SoMTD::Player::PlayerState::NOT_ENOUGH_GOLD;
                }
            }
            m_player->state= SoMTD::Player::PlayerState::IDLE;
            return true;
        }
    }

    if (event.id() == 777) {
        m_done = true;
        return true;
    }

    if (event.id() == SoMTD::BUILD_TOWER) {
        m_player->state = SoMTD::Player::PlayerState::HOLDING_BUILD;
        return true;
    }

    return false;
}

void
SoMTD::MapLevel::load_panels()
{
    LuaScript panel_list("lua-src/Panel.lua");

    std::string panel_file_path;
    pair<int, int> panel_screen_position; // x = first, y = second
    unsigned panel_id;
    std::shared_ptr< ijengine::Texture > panel_texture;
    int panel_priority = 0;

    std::vector< std::string > panel_names {
        "hp_panel", "left_upgrade_panel", "right_upgrade_panel",
        "coins_panel", "poseidon_panel", "zeus_panel", "hades_panel"
    };

    for (std::string it : panel_names) {
        panel_file_path = panel_list.get<std::string>((it + ".file_path").c_str());
        panel_screen_position.first = panel_list.get<int>((it + ".screen_position.x").c_str());
        panel_screen_position.second = panel_list.get<int>((it + ".screen_position.y").c_str());
        panel_id = panel_list.get<unsigned>((it + ".id").c_str());
        panel_priority = panel_list.get<int>((it + ".priority").c_str());
        SoMTD::Panel *p = new SoMTD::Panel(panel_file_path, panel_id, panel_screen_position.first, panel_screen_position.second, m_player, panel_priority);
        add_child(p);
    }
}

void
SoMTD::MapLevel::load_buttons()
{
    LuaScript button_list("lua-src/Button.lua");

    std::string button_file_path;
    pair<int, int> button_screen_position; // x = first, y = second
    int button_id;
    int button_priority = 0;
    std::string button_mouseover_path;

    std::vector< std::string > button_names {
        "zeus_button", "hades_button", "poseidon_button"
    };

    for (std::string it : button_names) {
        button_file_path = button_list.get<std::string>((it + ".file_path").c_str());
        button_screen_position.first = button_list.get<int>((it + ".screen_position.x").c_str());
        button_screen_position.second = button_list.get<int>((it + ".screen_position.y").c_str());
        button_id = button_list.get<int>((it + ".id").c_str());
        button_priority = button_list.get<int>((it + ".priority").c_str());
        button_mouseover_path = button_list.get<std::string>((it + ".mouseover_file_path").c_str());
        SoMTD::Button *b = new SoMTD::Button(button_file_path, button_id, button_screen_position.first, button_screen_position.second, button_mouseover_path, m_player, button_priority);
        add_child(b);
    }
}


void
SoMTD::MapLevel::load_hud()
{
    load_panels();
    load_buttons();

    std::shared_ptr< ijengine::Texture > hud_texture;

    SoMTD::TextureBar *hp_bar = new SoMTD::TextureBar("hp_percentage.png", 0, 58, 22, m_player, 12, 12);
    hp_bar->set_priority(500020);
    add_child(hp_bar);

}

std::string
SoMTD::MapLevel::audio() const
{
    return m_audio_path;
}

void
SoMTD::MapLevel::draw_self_after(ijengine::Canvas *c, unsigned, unsigned)
{
    if (m_player->state == SoMTD::Player::PlayerState::HOLDING_BUILD) {
        std::string tower_name = "tower_";
        tower_name.append( std::to_string(m_player->desired_tower) );
        tower_name.append("_holding.png");
        auto mytext = ijengine::resources::get_texture(tower_name);
        auto pos = ijengine::event::mouse_position();
        int xpos = pos.first;
        int ypos = pos.second;

        c->draw(mytext.get(), xpos - mytext->w()/2, ypos - mytext->h()/2);
    }
}

void
SoMTD::MapLevel::load_spawners()
{
    SoMTD::MovableUnit *cyclop = new SoMTD::MovableUnit(origin, destiny, "cyclop.png", m_labyrinth->solution, m_player);
    SoMTD::Spawner<MovableUnit> *spawner = new SoMTD::Spawner<MovableUnit>(cyclop);
    SoMTD::MovableUnit *zeus = new SoMTD::MovableUnit(origin, destiny, "zeus_panel.png", m_labyrinth->solution, m_player);
    SoMTD::Spawner<MovableUnit> *spawner2 = new SoMTD::Spawner<MovableUnit>(zeus);
    SoMTD::MovableUnit *hades = new SoMTD::MovableUnit(origin, destiny, "hades_panel.png", m_labyrinth->solution, m_player);
    SoMTD::Spawner<MovableUnit> *spawner3 = new SoMTD::Spawner<MovableUnit>(hades);
    SoMTD::MovableUnit *poseidon = new SoMTD::MovableUnit(origin, destiny, "poseidon_panel.png", m_labyrinth->solution, m_player);
    SoMTD::Spawner<MovableUnit> *spawner4 = new SoMTD::Spawner<MovableUnit>(poseidon);
    add_child(spawner);
    add_child(spawner2);
    add_child(spawner3);
    add_child(spawner4);
}

