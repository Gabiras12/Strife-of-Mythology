#include "menu_level.h"
#include "game.h"

#include <ijengine/engine.h>
#include <ijengine/canvas.h>
#include "luascript.h"
#include "button.h"
#include "player.h"

SoMTD::MenuLevel::MenuLevel(std::string mapname, std::string _new_map_path, std::string p_audio) :
    m_next(_new_map_path),
    m_audio(p_audio),
    m_done(false)
{
    m_level_name = mapname;
    m_player = new SoMTD::Player();
    if (mapname == "mainmenu")
        m_texture = ijengine::resources::get_texture("Menu.png");
    else if (mapname == "menucredits")
        m_texture = ijengine::resources::get_texture("telacreditos.png");

    ijengine::event::register_listener(this);
    load_buttons();
}

SoMTD::MenuLevel::~MenuLevel()
{
    delete m_player;
    ijengine::event::unregister_listener(this);
}

void
SoMTD::MenuLevel::draw_self(ijengine::Canvas *c, unsigned, unsigned)
{
    c->draw(m_texture.get(), 0, 0);
}

void
SoMTD::MenuLevel::draw_self_after(ijengine::Canvas* canvas, unsigned, unsigned)
{
    if (m_level_name == "menucredits") {
        auto font = ijengine::resources::get_font("Forelle.ttf", 40);
        canvas->set_font(font);
        canvas->draw(std::string("Desenvolvimento: Dylan, Jonnatas, Marcelo e Victor\n"), m_x+70, m_y+10);
        canvas->draw(std::string("Arte: Gabriel Andrade e Vitor Bichara\n"), m_x+70, m_y+80);
        canvas->draw(std::string("Música: Raul Marques\n"), m_x+70, m_y+140);
    }
}


bool
SoMTD::MenuLevel::on_event(const ijengine::GameEvent&)
{
    return false;
}

std::string
SoMTD::MenuLevel::audio() const
{
    return m_audio;
}

std::string
SoMTD::MenuLevel::next() const
{
    return m_next;
}

bool
SoMTD::MenuLevel::done() const
{
    return m_done;
}

void
SoMTD::MenuLevel::update_self(unsigned, unsigned)
{
}

void
SoMTD::MenuLevel::load_buttons()
{
    LuaScript button_list("lua-src/Button.lua");

    std::string button_file_path;
    pair<int, int> button_screen_position; // x = first, y = second
    int button_id;
    int button_priority = 0;
    std::string button_mouseover_path;

    std::vector< std::string > button_names {
        "play_button", "exit_button", "credits_button"
    };

    for (std::string it : button_names) {
        button_file_path = button_list.get<std::string>((it + ".file_path").c_str());
        button_screen_position.first = button_list.get<int>((it + ".screen_position.x").c_str());
        button_screen_position.second = button_list.get<int>((it + ".screen_position.y").c_str());
        button_id = button_list.get<int>((it + ".id").c_str());
        button_priority = button_list.get<int>((it + ".priority").c_str());
        button_mouseover_path = button_list.get<std::string>((it + ".mouseover_file_path").c_str());
        SoMTD::Button *b = new SoMTD::Button(button_file_path, button_id, button_screen_position.first, button_screen_position.second, button_mouseover_path, m_player, button_priority, new std::vector<int>());
        b->set_menu_level(this);
        add_child(b);
    }
}

SoMTD::Player*
SoMTD::MenuLevel::player() const
{
    return m_player;
}

void
SoMTD::MenuLevel::finish()
{
    m_done = true;
}

void
SoMTD::MenuLevel::exit_game()
{
  m_next = "";
  m_done = true;
}

void
SoMTD::MenuLevel::update_next_level(std::string next_map)
{
    m_next = next_map;
}
