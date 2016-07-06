hp_panel = {
  file_path = "hp_panel.png",
  screen_position = {
    y = 10,
    x = 10
  },
  id = 0xFFF0,
  priority = 500000,
  width = 230,
  height = 48
}
left_upgrade_panel = {
  file_path = "upgrade_panel.png",
  width = 238,
  height = 228,
  screen_position = {
    y = 700,
    x = 0
  },
  priority = 5000,
  id = 0xFFF1
}
left_upgrade_panel.screen_position.y = left_upgrade_panel.screen_position.y - left_upgrade_panel.height

right_upgrade_panel = {
  file_path = "upgrade_panel.png",
  width = 238,
  height = 228,
  screen_position = {
    y = 700,
    x = 1124
  },
  priority = 50000,
  id = 0xFFF2
}
right_upgrade_panel.screen_position.y = right_upgrade_panel.screen_position.y - right_upgrade_panel.height
right_upgrade_panel.screen_position.x = right_upgrade_panel.screen_position.x - right_upgrade_panel.width

coins_panel = {
  file_path = "coins_panel.png",
  width = 199,
  height = 69,
  screen_position = {
    y = 10,
    x = 1124
  },
  priority = 500000,
  id = 0xFFF3
}
coins_panel.screen_position.x = coins_panel.screen_position.x - coins_panel.width - 25
zeus_panel = {
  file_path = "zeus_panel.png",
  screen_position = {
    x = 30,
    y = 615
  },
  priority = 500220,
  id = 0x0,
  width = 38,
  height = 40
}
poseidon_panel = {
  file_path = "poseidon_panel.png",
  screen_position = {
    x = 102,
    y = 605
  },
  priority = 500220,
  id = 0x1,
  width = 28,
  height = 50
}
hades_panel = {
  file_path = "hades_panel.png",
  screen_position = {
    x = 170,
    y = 610
  },
  priority = 500220,
  id = 0x2,
  width = 32,
  height = 46
}
towerbox_panel = {
  file_path = "towerbox.png",
  screen_position = {
    x = 265,
    y = 605
  },
  priority = 500220,
  id = 0xFFF4,
  width = 766,
  height = 136
}

coins_tier_panel_1 = {
  file_path = "button/buy_tier.png",
  screen_position = {
    x = 11,
    y = 565
  },
  priority = 500220,
  id = 0x20,
}

coins_tier_panel_2 = {
  file_path = "button/buy_tier.png",
  screen_position = {
    x = 82,
    y = 565
  },
  priority = 500220,
  id = 0xFFF7,
}

coins_tier_panel_3 = {
  file_path = "button/buy_tier.png",
  screen_position = {
    x = 154,
    y = 565
  },
  priority = 500220,
  id = 0xFFF8,
}
