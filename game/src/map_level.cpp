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
#include <sstream>
#include <iostream>
#include <cmath>

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
    fetch_waves_from_file();
    m_current_wave = 0;

    m_actions = new LuaScript("lua-src/Action.lua");
    m_actual_state = MapLevel::State::IDLE;
    m_towers = new std::list<SoMTD::Tower*>();
    m_towers->clear();
}

SoMTD::MapLevel::~MapLevel()
{
    delete m_labyrinth;
    delete m_actions;
    delete m_player;
    delete m_towers;
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
            string expression;
            expression = "tile_";
            std::ostringstream convert;
            convert << m_labyrinth->m_grid[line][column];
            if (m_labyrinth->m_grid[line][column] == 0x0 || m_labyrinth->m_grid[line][column] == 0xA) {
                printf("origin: %d %d\n", column, line);
                origin.first = column;
                origin.second = line;
            } else if (m_labyrinth->m_grid[line][column] == 0x9 || m_labyrinth->m_grid[line][column] == 0x13) {
                printf("destination : %d %d\n", column, line);
                destiny.first = column;
                destiny.second = line;
            }
            expression.append(convert.str());
            expression.append(".png");
            _id = m_labyrinth->m_grid[line][column];
            std::cout << "path: " << expression.c_str() << std::endl;
            add_child(new SoMTD::LevelArea(expression, _id, column, line, 0));
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

SoMTD::MapLevel::State
SoMTD::MapLevel::actual_state() const
{
    return m_actual_state;
}

void
SoMTD::MapLevel::update_self(unsigned now, unsigned last)
{
    switch (actual_state()) {
        case IDLE:
            handle_idle_state(now, last);
        break;

        case RESTING:
            handle_resting_state(now, last);
        break;

        case PLAYING:
            handle_playing_state(now, last);
        break;

        default:
        break;
    }

    if (m_start == -1)
        m_start = now;
    if (not (now - m_start > 1000))
        m_done = false;
}

void
SoMTD::MapLevel::draw_self(ijengine::Canvas *canvas, unsigned a1, unsigned a2)
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
        if (m_player->state == SoMTD::Player::PlayerState::HOLDING_BUILD) {
            double x_pos = event.get_property<double>("x");
            double y_pos = event.get_property<double>("y");

            auto tile_position = SoMTD::tools::isometric_to_grid((int)x_pos, (int)y_pos, 100, 81, 1024/2, 11);

            if (tile_position.first >= 0 && tile_position.second >= 0 && tile_position.first < 10 && tile_position.second < 10) {
                if (m_player->gold() >= m_player->m_desired_tower_price) {
                    if (m_labyrinth->m_grid[tile_position.second][tile_position.first] == 0x7 ||
                            m_labyrinth->m_grid[tile_position.second][tile_position.first] == 0x11) {
                        m_labyrinth->m_grid[tile_position.second][tile_position.first] = 88;
                        build_tower(m_player->desired_tower(), tile_position.first, tile_position.second);
                        m_player->discount_gold(m_player->m_desired_tower_price);
                        m_player->discount_hp(1);
                    }
                } else {
                    printf("You need moar gold! (%d)\n", m_player->gold());
                }
            }
            m_player->state = SoMTD::Player::PlayerState::IDLE;
            return true;
        }
    }

    if (event.id() == 777) {
        m_done = true;
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
        "coins_panel", "poseidon_panel", "zeus_panel", "hades_panel",
        "towerbox_panel"
        // "buy_tower_panel"
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
    std::string button_tower_file_path;
    
    std::vector< std::string > button_names {
        "zeus_button", "hades_button", "poseidon_button",
        "tower_zeus_1_button", "tower_zeus_2_button", "tower_zeus_3_button", "tower_zeus_4_button",
        "tower_poseidon_1_button", "tower_poseidon_2_button", "tower_poseidon_3_button", "tower_poseidon_4_button",
        "tower_hades_1_button", "tower_hades_2_button", "tower_hades_3_button", "tower_hades_4_button",
        "tier_2_button", "tier_3_button", "tier_4_button"

    };

    for (std::string it : button_names) {
        button_file_path = button_list.get<std::string>((it + ".file_path").c_str());
        button_screen_position.first = button_list.get<int>((it + ".screen_position.x").c_str());
        button_screen_position.second = button_list.get<int>((it + ".screen_position.y").c_str());
        button_id = button_list.get<int>((it + ".id").c_str());
        button_priority = button_list.get<int>((it + ".priority").c_str());
        button_mouseover_path = button_list.get<std::string>((it + ".mouseover_file_path").c_str());
        button_tower_file_path = button_list.get<std::string>((it + ".tower_file_path").c_str());
        std::vector<int> *infos = new std::vector<int>();
        switch (button_id) {
            case 0x2000:
            case 0x2001:
            case 0x2002:
            case 0x2003:
                infos->push_back(button_list.get<int>((it + ".tower_price").c_str()));
                infos->push_back(button_list.get<int>((it + ".requirements").c_str()));
                break;

            case 0x3000:
            case 0x3001:
            case 0x3002:
                infos->push_back(button_list.get<int>((it + ".upgrade_price").c_str()));
                infos->push_back(button_list.get<int>((it + ".requirements").c_str()));
                break;

            default:
                break;
        }

        SoMTD::Button *b = new SoMTD::Button(button_file_path, button_id, button_screen_position.first, button_screen_position.second, button_mouseover_path, button_tower_file_path, m_player, button_priority, infos);
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
SoMTD::MapLevel::draw_self_after(ijengine::Canvas *c, unsigned a1, unsigned a2)
{
    current_wave()->draw_self(c, a1, a2);
    current_wave()->draw_self_after(c, a1, a2);

    if (m_player->state == SoMTD::Player::PlayerState::HOLDING_BUILD) {
        std::string tower_name = "tower_";
        tower_name.append( std::to_string(m_player->desired_tower()) );
        tower_name.append("_holding.png");
        auto mytext = ijengine::resources::get_texture(tower_name);
        auto pos = ijengine::event::mouse_position();
        int xpos = pos.first;
        int ypos = pos.second;

        c->draw(mytext.get(), xpos - mytext->w()/2, ypos - mytext->h()/2);
    }

    auto font = ijengine::resources::get_font("Forelle.ttf", 40);
    c->set_font(font);
    std::ostringstream convert;

    convert << m_current_wave + 1;
    std::string mytext = "Wave ";
    mytext.append(convert.str());
    c->draw(mytext, 1024/2, 0);
    draw_selected_panel(c, a1, a2);

    if(m_actual_state == RESTING || m_actual_state == IDLE){
        c->draw(set_time_to_start_wave(a1), 700/2, 0);
    }
}

void
SoMTD::MapLevel::draw_selected_panel(ijengine::Canvas *c, unsigned now, unsigned last)
{
    if (player()->selected_object) {
        auto font = ijengine::resources::get_font("Forelle.ttf", 30);
        c->set_font(font);
        std::ostringstream convert;
        std::string expression;

        SoMTD::Tower* t = dynamic_cast<SoMTD::Tower*>(player()->selected_object);
        expression = "Damage: ";
        convert << t->damage();
        expression.append(convert.str());
        c->draw(expression, 950, 600);
        expression = "Range: ";
        convert.str("");
        convert.clear();
        convert << t->range();
        expression.append(convert.str());
        c->draw(expression, 950, 500);
        expression = "Level: ";
        convert.str("");
        convert.clear();
        convert << t->level();
        expression.append(convert.str());
        c->draw(expression, 950, 550);
    } else{
    }
}

void
SoMTD::MapLevel::load_spawners()
{
    LuaScript units_list("lua-src/Unit.lua");

    std::vector< std::string > unit_names {
        "cyclop", "medusa", "bat", "centauro"
    };

    std::string unit_path;
    int unit_statestyle;
    int unit_total_states, unit_frame_per_state;
    SoMTD::MovableUnit *myunit;
    SoMTD::Spawner<MovableUnit> *spawner;
    int unit_hp;
    int unit_gold_reward;
    int unit_time_per_tile;
    int unit_hp_discount_unit_win;
    std::string unit_slowedpath;
    std::string unit_bleedpath;

    for (std::string it : unit_names) {
        unit_path = units_list.get<std::string>((it + ".file_path").c_str());
        unit_statestyle = units_list.get<int>((it + ".state_style").c_str());
        unit_total_states = units_list.get<int>((it + ".total_states").c_str());
        unit_frame_per_state = units_list.get<int>((it + ".frame_per_state").c_str());
        unit_gold_reward = units_list.get<int>((it + ".gold_reward").c_str());
        unit_time_per_tile = units_list.get<int>((it + ".time_per_tile").c_str());
        unit_hp = units_list.get<int>((it + ".hp").c_str());
        unit_hp_discount_unit_win = units_list.get<int>((it + ".hp_discount_unit_win").c_str());
        unit_slowedpath = units_list.get<std::string>((it + ".slowed_path").c_str());
        unit_bleedpath = units_list.get<std::string>((it + ".bleeding_path").c_str());

        myunit = new SoMTD::MovableUnit(origin, destiny, unit_path, m_labyrinth->solution, m_player, (Animation::StateStyle)unit_statestyle, unit_frame_per_state, unit_total_states, unit_hp, unit_gold_reward, unit_time_per_tile, unit_hp_discount_unit_win, unit_slowedpath, unit_bleedpath);
        spawner = new SoMTD::Spawner<MovableUnit>(myunit);
        spawners.push_back(spawner);
    }
}

void
SoMTD::MapLevel::fetch_waves_from_file()
{
    int aux;
    int wave_length;
    int total_waves;
    std::string waves_path = "res/" + m_current + "_waves.txt";
    std::ifstream map_data(waves_path);
    if (map_data.is_open()) {
        map_data >> total_waves;
        for (int j=0; j < total_waves; ++j) {
            Wave *w = new SoMTD::Wave(j);
            map_data >> wave_length;
            for (int i=0; i < wave_length; ++i) {
                map_data >> aux;
                SoMTD::MovableUnit *myunit = spawners[aux]->spawn_unit();
                w->add_unit(myunit);
            }
            m_waves.push_back(w);
        }
        map_data.close();
    }
}

void
SoMTD::MapLevel::start_wave(unsigned now)
{
    current_wave()->start(now);
}

void
SoMTD::MapLevel::update_current_wave(unsigned now, unsigned last)
{
    current_wave()->update_self(now, last);
}

SoMTD::Wave*
SoMTD::MapLevel::current_wave()
{
    return m_waves[m_current_wave];
}

void
SoMTD::MapLevel::build_tower(unsigned tower_id, int x, int y)
{
    LuaScript towers_list("lua-src/Tower.lua");
    std::string affix = "tower_";

    ostringstream convert;
    convert << tower_id;
    affix.append(convert.str());

    std::string tower_path = towers_list.get<std::string>((affix+".file_path").c_str());
    std::string selected_tower_path = towers_list.get<std::string>((affix+".selected_file_path").c_str());
    int tower_state_style = towers_list.get<int>((affix+".state_style").c_str());
    int total_states = towers_list.get<int>((affix+".total_states").c_str());
    int frame_per_state = towers_list.get<int>((affix+".frame_per_state").c_str());
    float tower_attack_speed = towers_list.get<float>((affix + ".attack_speed").c_str());
    int tower_damage = towers_list.get<int>((affix + ".damage").c_str());
    int towerid = towers_list.get<int>((affix + ".id").c_str());

    SoMTD::Tower *m_tower = new SoMTD::Tower(tower_path, towerid, x, y, selected_tower_path, m_player, (Animation::StateStyle)tower_state_style, frame_per_state, total_states, tower_attack_speed, tower_damage);
    m_tower->set_priority(50000+(5*x*y));
    add_child(m_tower);
    m_towers->push_back(m_tower);
}

void
SoMTD::MapLevel::handle_idle_state(unsigned now, unsigned last)
{
    if (not m_state_started_at)
        m_state_started_at = now;
    if (now > 5000+m_state_started_at) {
        transition_to(IDLE, PLAYING, now, last);
    }
}

void
SoMTD::MapLevel::handle_resting_state(unsigned now, unsigned last)
{
    if (now > m_state_started_at + 5000) {
        transition_to(RESTING, PLAYING, now, last);
    }
}

void
SoMTD::MapLevel::handle_playing_state(unsigned now, unsigned last)
{
    if (not current_wave()->started()) {
        start_wave(now);
    } else if (current_wave()->done()) {
        transition_to(PLAYING, RESTING, now, last);
    } else {
        update_current_wave(now, last);
        update_units_events(now, last);
        check_towers_collisions(now, last);
    }
}

void
SoMTD::MapLevel::update_units_events(unsigned now, unsigned last)
{
    if (not player()->units_events()->empty()) {
        for (auto event=player()->units_events()->begin(); event != player()->units_events()->end(); ++event) {
            handle_unit_event((*event), now, last);
        }

        while (not player()->units_events()->empty()) {
            player()->units_events()->pop_back();
        }
    }
}

void
SoMTD::MapLevel::handle_unit_event(int event_id, unsigned now, unsigned last)
{
    double dx, dy, distance;
    int x_event, y_event, slow_range, time_penalization, slow_coeff, slow_damage;
    switch (event_id){
        case 0x000:
            x_event = player()->event_args()->front();
            player()->event_args()->pop_front();
            y_event = player()->event_args()->front();
            player()->event_args()->pop_front();
            slow_range = player()->event_args()->front();
            player()->event_args()->pop_front();
            slow_damage = player()->event_args()->front();
            player()->event_args()->pop_front();
            slow_coeff = player()->event_args()->front();
            player()->event_args()->pop_front();
            time_penalization = player()->event_args()->front();
            player()->event_args()->pop_front();

            for (auto unit=current_wave()->units()->begin(); unit != current_wave()->units()->end(); ++unit) {
                if ((*unit)->active()) {
                    dx = x_event - (*unit)->animation()->screen_position().first;
                    dy = y_event - (*unit)->animation()->screen_position().second;
                    distance = sqrt(dx*dx + dy*dy);
                    if (distance < (slow_range +(*unit)->animation()->width()/2)) {
                        (*unit)->suffer(slow_damage);
                        (*unit)->suffer_slow(slow_coeff, time_penalization, now, last);
                    }
                }
            }
            break;

        default:
            break;

    }
}

void
SoMTD::MapLevel::check_towers_collisions(unsigned now, unsigned last)
{
    for (auto unit=current_wave()->units()->begin(); unit != current_wave()->units()->end(); ++unit) {
        if ((*unit)->active()) {
            for (auto tower=m_towers->begin(); tower != m_towers->end(); ++tower) {
                if ((*tower)->actual_state() == SoMTD::Tower::State::IDLE) {
                    double dx = (*tower)->animation()->screen_position().first - (*unit)->animation()->screen_position().first;
                    double dy = (*tower)->animation()->screen_position().second - (*unit)->animation()->screen_position().second;
                    double distance = sqrt(dx*dx + dy*dy);
                    if (distance < ((*tower)->range()+(*unit)->animation()->width()/2)) {
                        (*tower)->attack(*unit, now, last);
                    }
                }
            }
        }
    }
}

void
SoMTD::MapLevel::transition_to(MapLevel::State from, MapLevel::State to, unsigned now, unsigned last)
{
    m_actual_state = to;
    m_state_started_at = now;
    if (from == RESTING && to == PLAYING) {
        if (m_current_wave >= m_waves.size()-1) {
            m_done = true;
        } else {
            m_current_wave++;
        }
    }
}

std::string
SoMTD::MapLevel::set_time_to_start_wave(unsigned now){

  std::ostringstream convert;

  convert << 5 - (now - m_state_started_at)/1000;

  return convert.str();
}

SoMTD::Player*
SoMTD::MapLevel::player() const
{
    return m_player;
}
