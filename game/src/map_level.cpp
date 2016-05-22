#include <vector>

#include "game.h"

#include <ijengine/engine.h>
#include <ijengine/canvas.h>
#include <ijengine/rectangle.h>

#include <fstream>
#include <cstring>

#include <SDL2/SDL.h>

#include "map_level.h"
#include "level_area.h"
#include "tower.h"
#include "player.h"
#include "panel.h"
#include "texture_bar.h"

SoMTD::MapLevel::MapLevel(const string& next_level, const string& current_level, const string& audio_file_path) :
    m_next(next_level),
    m_current(current_level),
    m_audio_path(audio_file_path),
    m_done(false),
    m_player(new Player),
    m_start(-1),
    m_texture(nullptr)
{
    if (current_level == "map002") {
        m_player->m_gold = 999999;
    } else {
        m_player->m_gold = 90000;
    }
    memset(grid, 0, sizeof grid);
    ijengine::event::register_listener(this);

    load_config_from_file();
    load_tiles();
    load_hud();
}

SoMTD::MapLevel::~MapLevel()
{
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
                    add_child(new SoMTD::LevelArea("slopeE.png", 1, j, i, 0));
                break;

                case 2:
                    add_child(new SoMTD::LevelArea("slopeN.png", 2, j, i, 0));
                break;

                case 3:
                    add_child(new SoMTD::LevelArea("slopeW.png", 3, j, i, 0));
                break;

                case 4:
                    add_child(new SoMTD::LevelArea("slopeS.png", 4, j, i, 0));
                break;

                case 5:
                    add_child(new SoMTD::LevelArea("waterfallEndE.png", 5, j, i, 0));
                break;

                case 6:
                    add_child(new SoMTD::LevelArea("tile_grama.png", 6, j, i, 0));
                break;

                case 7:
                    add_child(new SoMTD::LevelArea("waterfallEndW.png", 7, j, i, 0));
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
SoMTD::MapLevel::load_config_from_file()
{
    // fetch data from a file with its level id
    if (not m_current.empty()) {
        std::string path("res/");
        path = path.append(m_current);
        path = path.append(".txt");

        std::ifstream map_data(path);
        if (map_data.is_open()) {
            // 9 = number of rows
            // 12 = number of cols
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
SoMTD::MapLevel::update_self(unsigned, unsigned)
{

}

void
SoMTD::MapLevel::draw_self(ijengine::Canvas *canvas, unsigned, unsigned)
{
    canvas->clear();
    canvas->draw(ijengine::resources::get_texture("background.png").get(), 0, 0);

    int i = m_player->m_x;
    int j = m_player->m_y;

    std::shared_ptr< ijengine::Texture > highlight_area;
    if (m_player->state == 0x00) {
        highlight_area = ijengine::resources::get_texture("press_b.png");
    } else if (m_player->state == 0x01) {
        highlight_area = ijengine::resources::get_texture("click_to_build.png");
    } else if (m_player->state == 0x03) {
        highlight_area = ijengine::resources::get_texture("invalid_location.png");
    } else if (m_player->state == 0x04) {
        highlight_area = ijengine::resources::get_texture("not_enough_gold.png");
    }
    canvas->draw(highlight_area.get(), 1024 - highlight_area->w(), 150);
    ijengine::Rectangle rect { 15, 15, 173, 26 };
    canvas->draw(rect);
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
        if (m_player->state == 0x01) {
            double x_pos = event.get_property<double>("x");
            double y_pos = event.get_property<double>("y");

            int const tile_width = 100;
            int const h_tw = tile_width/2;
            int const tile_height = 81+11;
            int const h_th = tile_height/2;
            int const x0 = 1024/2;
            int const offset = 11;

            myx =  (((x_pos+h_tw-x0)/h_tw)+((y_pos)/(h_th-offset)))/2;
            myy = (((y_pos)/(h_th-offset)) - ((x_pos+h_tw-x0)/h_tw))/2;

            if (myx >= 0 && myy >= 0 && grid[myy][myx] < 8 && myx < 10 && myy < 10) {
                if (m_player->m_gold >= 100) {
                    grid[myy][myx] = 88;
                    SoMTD::Tower *m_tower = new SoMTD::Tower("tower_42.png", 9, myx, myy);
                    add_child(m_tower);
                    m_tower->set_priority(50000+(5*myy+5*myx));
                    m_player->m_gold -= 100;
                    m_player->state = 0x00;
                    m_player->m_hp -= 1;
                } else {
                    printf("You need moar gold! (%d)\n", m_player->m_gold);
                    m_player->state = 0x04;
                }
            } else {
                m_player->state = 0x03;
            }
            return true;
        }
    }

    if (event.id() == SoMTD::BUILD_TOWER) {
        m_player->state = 0x01;
        return true;
    }

    return false;
}

void
SoMTD::MapLevel::load_hud()
{
    std::shared_ptr< ijengine::Texture > hud_texture = ijengine::resources::get_texture("buy_panel.png");

    hud_texture = ijengine::resources::get_texture("hp_panel.png");
    SoMTD::Panel *hp_panel = new SoMTD::Panel("hp_panel.png", 0, 10, 10);
    hp_panel->set_priority(500000);
    add_child(hp_panel);

    SoMTD::TextureBar *hp_bar = new SoMTD::TextureBar("hp_percentage.png", 0, 58, 22, m_player, 12, 12);
    hp_bar->set_priority(500020);
    add_child(hp_bar);

    hud_texture = ijengine::resources::get_texture("upgrade_panel.png");
    SoMTD::Panel *upgrade_panel = new SoMTD::Panel("upgrade_panel.png", 0, 0, 700-hud_texture->h());
    upgrade_panel->set_priority(500000);
    add_child(upgrade_panel);

    hud_texture = ijengine::resources::get_texture("coins_panel.png");
    SoMTD::Panel *coins_panel = new SoMTD::Panel("coins_panel.png", 0, 1024-hud_texture->w()-25, 10);
    coins_panel->set_priority(500000);
    add_child(coins_panel);
}

std::string
SoMTD::MapLevel::audio() const
{
    return m_audio_path;
}
