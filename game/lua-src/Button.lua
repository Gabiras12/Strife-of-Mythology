zeus_button = {
  file_path = "button/hero_button.png",
  mouseover_file_path = "button/hero_button_mouseover.png",
  screen_position = {
    x = 20,
    y = 600
  },
  priority = 50000,
  id = 0x0,
  width = 66,
  height = 65,
  description = ""
}
hades_button = {
  file_path = "button/hero_button.png",
  mouseover_file_path = "button/hero_button_mouseover.png",
  screen_position = {
    x = 20,
    y = 600
  },
  priority = 50000,
  id = 0x1,
  width = 66,
  height = 65,
  description = ""
}
hades_button.screen_position.x = hades_button.screen_position.x + hades_button.width
poseidon_button = {
  file_path = "button/hero_button.png",
  mouseover_file_path = "button/hero_button_mouseover.png",
  screen_position = {
    x = 20,
    y = 600
  },
  priority = 50000,
  id = 0x2,
  width = 66,
  height = 65,
  description = ""
}
poseidon_button.screen_position.x = poseidon_button.screen_position.x + poseidon_button.width*2

play_button = {
  file_path = "button/play_black.png",
  mouseover_file_path = "button/play_white.png",
  screen_position = {
    x = 943,
    y = 380
  },
  priority = 50000,
  id = 1000,
  width = 81,
  height = 35,
  description = ""
}

credits_button = {
  file_path = "button/credits_black.png",
  mouseover_file_path = "button/credits_white.png",
  screen_position = {
    x = 910,
    y = 450
  },
  priority = 50000,
  id = 1001,
  width = 66,
  height = 65,
  description = ""
}

exit_button = {
  file_path = "button/exit_black.png",
  mouseover_file_path = "button/exit_white.png",
  screen_position = {
    x = 945,
    y = 535
  },
  priority = 50000,
  id = 1002,
  width = 66,
  height = 65,
  description = ""
}

-- towers buttons below
-- info[0] = tower price, info[1] = tower prerequisites
tower_1_button = {
  file_path = "button/buy_tower.png",
  mouseover_file_path = "button/buy_tower_mouseover.png",
  screen_position = {
    x = 315,
    y = 550
  },
  priority = 500230,
  id = 0x2000,
  width = 105,
  height = 134,
  tower_price = 150,
  requirements = 0x0001,
  description = "Descrição 1"

}

tower_2_button = {
  file_path = "button/buy_tower.png",
  mouseover_file_path = "button/buy_tower_mouseover.png",
  screen_position = {
    x = 445,
    y = 550
  },
  priority = 500230,
  id = 0x2001,
  width = 105,
  height = 134,
  tower_price = 200,
  requirements = 0x0002,
  description = "Descrição 2"
}

tower_3_button = {
  file_path = "button/buy_tower.png",
  mouseover_file_path = "button/buy_tower_mouseover.png",
  screen_position = {
    x = 574,
    y = 550
  },
  priority = 500230,
  id = 0x2002,
  width = 105,
  height = 134,
  tower_price = 300,
  requirements = 0x0004,
  description = "Descrição 3"
}

tower_4_button = {
  file_path = "button/buy_tower.png",
  mouseover_file_path = "button/buy_tower_mouseover.png",
  screen_position = {
    x = 702,
    y = 550
  },
  priority = 500230,
  id = 0x2003,
  width = 105,
  height = 134,
  tower_price = 500,
  requirements = 0x0008,
  description = "Descrição 4"
}

-- tier buttons below
tier_2_button = {
  file_path = "button/tier2.png",
  mouseover_file_path = "button/tier2_mouseover.png",
  screen_position = {
    x = 20,
    y = 500
  },
  priority = 500230,
  id = 0x3000,
  width = 66,
  height = 134,
  requirements = 0x0001,
  upgrade_price = 1000,
  description = ""
}

tier_3_button = {
  file_path = "button/tier3.png",
  mouseover_file_path = "button/tier3_mouseover.png",
  screen_position = {
    x = 86,
    y = 500
  },
  priority = 500230,
  id = 0x3001,
  width = 66,
  height = 134,
  requirements = 0x0002,
  upgrade_price = 3000,
  description = ""
}

tier_4_button = {
  file_path = "button/tier4.png",
  mouseover_file_path = "button/tier4_mouseover.png",
  screen_position = {
    x = 152,
    y = 500
  },
  priority = 500230,
  id = 0x3002,
  width = 66,
  height = 134,
  requirements = 0x0004,
  upgrade_price = 10000,
  description = "HELLLO!!"
}
