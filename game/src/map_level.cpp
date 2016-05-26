#include <vector>

#include "game.h"

#include <ijengine/engine.h>
#include <ijengine/canvas.h>
#include <ijengine/rectangle.h>

#include <fstream>
#include <cstring>
#include <map>


#include "luascript.h"
#include "map_level.h"
#include "level_area.h"
#include "tower.h"
#include "player.h"
#include "panel.h"
#include "texture_bar.h"
#include "button.h"

SoMTD::MapLevel::MapLevel(const string& next_level, const string& current_level, const string& audio_file_path) :
    m_next(next_level),
    m_current(current_level),
    m_audio_path(audio_file_path),
    m_done(false),
    m_player(new Player),
    m_start(-1),
    m_texture(nullptr)
{
    // reset grid positions
    memset(grid, 0, sizeof grid);
    ijengine::event::register_listener(this);

    load_map_from_file();
    load_tiles();
    load_hud();
    m_actions = new LuaScript("lua-src/Action.lua");
}

SoMTD::MapLevel::~MapLevel()
{
    delete m_actions;
    ijengine::event::unregister_listener(this);
    delete m_player;
}

void
SoMTD::MapLevel::load_tiles()
{
    for (int i=0; i < 10; ++i) {
        for (int j=0; j < 10; ++j) {
            switch (grid[i][j]) {
                case 1:
                    add_child(new SoMTD::LevelArea("caminho1.png", 1, j, i, 0));
                break;

                case 2:
                    add_child(new SoMTD::LevelArea("caminho2.png", 2, j, i, 0));
                break;

                case 3:
                    add_child(new SoMTD::LevelArea("curva3.png", 3, j, i, 0));
                break;

                case 4:
                    add_child(new SoMTD::LevelArea("curva4.png", 4, j, i, 0));
                break;

                case 5:
                    add_child(new SoMTD::LevelArea("curva1.png", 5, j, i, 0));
                break;

                case 6:
                    add_child(new SoMTD::LevelArea("tile_grama.png", 6, j, i, 0));
                break;

                case 7:
                    add_child(new SoMTD::LevelArea("curva2.png", 7, j, i, 0));
                break;

                case 8:
                    add_child(new SoMTD::LevelArea("waterfallEndS.png", 8, j, i, 0));
                break;

                default:
                break;
            }
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

        std::ifstream map_data(path);
        if (map_data.is_open()) {
            // 10 = number of rows
            // 10 = number of cols
            for (int i=0; i < 10; ++i) {
                for (int j=0; j < 10; ++j) {
                    map_data >> grid[i][j];
                }
            }
            map_data.close();
        }
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

std::pair<int, int>
SoMTD::MapLevel::screen_coordinates(int map_x, int map_y, int tw, int th)
{
    int xs = (map_x - map_y) * (tw / 2);
    int ys = (map_x + map_y) * (th / 2);

    return std::pair<int, int>(xs, ys);
}

bool
SoMTD::MapLevel::on_event(const ijengine::GameEvent& event)
{
    int myx = m_player->m_x;
    int myy = m_player->m_y;

    if (event.id() == SoMTD::CLICK) {
        if (m_player->state == 0x01 || m_player->state == 0x05 || m_player->state == 0x06 || m_player->state == 0x07) {
            double x_pos = event.get_property<double>("x");
            double y_pos = event.get_property<double>("y");

            int const tile_width = 100;
            int const h_tw = tile_width/2;
            int const tile_height = 81;
            int const h_th = tile_height/2;
            int const x0 = 1024/2;
            int const offset = 11;

            myx =  (((x_pos+h_th-x0)/h_tw)+((y_pos)/(h_th-offset)))/2;
            myy = (((y_pos)/(h_th-offset)) - ((x_pos+h_tw-x0)/h_tw))/2;

            if (myx >= 0 && myy >= 0 && grid[myy][myx] < 8 && myx < 10 && myy < 10) {
                if (m_player->m_gold >= 100) {
                    if (grid[myy][myx] == 6) {
                        grid[myy][myx] = 88;
                        SoMTD::Tower *m_tower = nullptr;
                        if (m_player->state == SoMTD::Player::PlayerState::HOLDING_BUILD) {
                            std::string tower_name("tower_");
                            tower_name.append(std::to_string(m_player->desired_tower));
                            tower_name.append(".png");
                            m_tower = new SoMTD::Tower(tower_name, 9, myx, myy, "selected_"+tower_name);
                            m_tower->set_priority(50000+(5*myy+5*myx));
                            add_child(m_tower);
                            m_player->m_gold -= 100;
                            m_player->state = SoMTD::Player::PlayerState::IDLE;
                            m_player->m_hp -= 1;
                        }
                    }
                } else {
                    printf("You need moar gold! (%d)\n", m_player->m_gold);
                    m_player->state = SoMTD::Player::PlayerState::NOT_ENOUGH_GOLD;
                }
            }
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
        printf("button id: %d\n", button_id);
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
        std::shared_ptr<ijengine::Texture> mytext = nullptr;
        std::string tower_name = "tower_";
        tower_name.append( std::to_string(m_player->desired_tower) );
        tower_name.append("_holding.png");
        mytext = ijengine::resources::get_texture(tower_name);
        c->draw(mytext.get(), m_player->m_x-mytext->w()/2, m_player->m_y-mytext->h()/2);
    }

}
