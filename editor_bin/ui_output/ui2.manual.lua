-----------------------------------  gen  slide list -------------------------------
local function _gen_slide_list_ani(ex_name, ex_item_name, head, space, count)
  count = count or 200
  local ani = {
    export = ex_name,
    id = alloc_id(ex_name),
    component = {
    }, 
    {
      {

      }
    }
  }

  local cpt = ani.component
   -- add component
  for i=1,count do
    cpt[i] = {name = "item"..(i), id = export2id(ex_item_name)}
  end

  -- add frame1
  local frame1 = ani[1][1]
  for i=0,#cpt-1 do
    frame1[i+1] = {index = i, mat = {1024, 0, 0, 1024, 0, head+space*i}}
  end

  return animation(ani)
end

----------------------------------- guild chat--------------------------------------
animation {
  export = "m_guild_chat",
  id = alloc_id "m_guild_chat",
  component = {
    {name = "window", id = export2id "chat_window"},
    {name = "guild_chat", id = export2id "chat_info"},
    {name = "guild_fight", id = export2id "guild_fight_info"},
    {name = "guild_perk", id = export2id "guild_perk_info"},
    {name = "guild_mvp", id = export2id "guild_msg_info"},
    {name = "chat_world_info", id = export2id "chat_world_info"},
  }, 

  {action = "chat",
    {
      {index=0},
      {index=1},
    }
  },

  {action = "fight",
    {
      {index=0},
      {index=2},
    }
  },

  {action = "perk",
    {
      {index=0},
      {index=3},
    }
  },

  {action = "mvp",
    {
      {index=0},
      {index=4},
    }
  }, 

  {action = "chat_world",
    {
      {index=0},
      {index=5},
    }
  }
}

----------------------------------- guild ------------------------------------------
_gen_slide_list_ani("guild_search_slide_list", "guild_search_item", -3500, 1400)


animation {
  export = "m_guild_add_slide",
  clipbox = {838, 565, -419, -260},
  id = alloc_id "m_guild_add_slide",
  component = {
    {name = "guild_search_list", id = get_id "guild_search_slide_list"},
  },

  {
    {
      {index=0, clip=true}
    }
  }  
}

_gen_slide_list_ani("guild_rank_slide_list", "guild_rank_item", -3500, 1400)
animation {
  export = "m_guild_rank_slide",
  clipbox = {838, 565, -419, -260},
  id = alloc_id "m_guild_rank_slide",
  component = {
    {name = "guild_rank_list", id = get_id "guild_rank_slide_list"},
  },

  {
    {
      {index=0, clip=true}
    }
  }  
}


_gen_slide_list_ani("guild_search_slide_list2", "guild_search_item", -1044, 1400)

animation {
  export = "m_guild_searh_slide",
  id = alloc_id "m_guild_searh_slide",
  component = {
    {name = "guild_search_list", id = get_id "guild_search_slide_list2"},
    {name = "result", font = "CCBackBeat", color = 0xff000000, align = 2, size = 25, width = 350, height = 40},
  },
  {action="search",
    {
      {index=0}
    }
  },
  {action="no_result",
    {
      {index = 1, mat = {1024, 0, 0, 1024, -2618, 599}},
    }
  }
}

_gen_slide_list_ani("guild_join_slide_list", "guild_join_item", -3500, 1400)

animation {
  export = "m_guild_join_slide",
  clipbox = {838, 565, -419, -260},
  id = alloc_id "m_guild_join_slide",
  component = {
    {name = "guild_join_list", id = get_id "guild_join_slide_list"},
  },

  {
    {
      {index=0, clip=true}
    }
  }  
}


animation {
  export = "m_guild_search",
  id = alloc_id "m_guild_search",
  clipbox = {838, 565-153.5, -419, -260+153.5},
  component = {
    {name = "search_bar", id = export2id "guild_search"},
    {name = "guild_search_list", id = get_id "m_guild_searh_slide"}
  },
  {
    {
      {index=0},
      {index=1, clip=true}
    }
  }
}

_gen_slide_list_ani("guild_member_list", "guild_member_item", -544, 1400)

animation {
  export = "m_guild_join_exit",
  id = alloc_id "m_guild_join_exit",
  clipbox = {838, 565-173.5, -419, -260+173.5},
  component = {
    {name = "title", id = export2id "guild_join_exit_back"},
    {name = "list", id = get_id "guild_member_list"},
  },
  {
    {
      {index=0},
      {index=1, clip=true}
    }
  }  
}

_gen_slide_list_ani("guild_league_slide_list", "guild_rank_item", -544, 1400)

animation {
  export = "m_guild_league",
  id = alloc_id "m_guild_league",
  clipbox = {838, 565-173.5, -419, -260+173.5},
  component = {
    {name = "league", id = export2id "guildwar_league"},
    {name = "list", id = get_id "guild_league_slide_list"},
  },
  {
    {
      {index=0, mat = {1024, 0, 0, 1024, 32, -2864}},
      {index=1, clip=true}
    }
  } 
}

animation {
  export = "m_guild_info_window",
  id = alloc_id "m_guild_info_window",
  component = {
    {name = "window", id = export2id "guild_info_window"},
    {name = "guild_add", id = export2id "m_guild_add_slide"},
    {name= "guild_create", id = export2id "guild_create"},
    {name = "guild_search", id = export2id "m_guild_search"},
    {name = "guild_join_exit", id = export2id "m_guild_join_exit"},
    {name = "guild_join_apply", id = export2id "m_guild_join_slide"},
    {name = "guild_rank", id = export2id "m_guild_rank_slide"}, 
    {name = "guild_league", id = export2id "m_guild_league"},
    {name = "member_view", id = export2id "member_view_back"},
  },

  {action="add_guild",
    {
      {index=0},
      {index=1},
    }
  },
  
  {action="create_guild",
    {
      {index=0},
      {index=2},
    }
  },

  {action="search_guild",
    {
      {index=0},
      {index=3}
    }
  },

  {action="join_exit_guild",
    {
      {index=0},
      {index=4}
    }
  }, 

  {action="join_apply_guild",
    {
      {index=0},
      {index=5},
    }
  },
  
  {action="guild_rank",
    {
      {index=0},
      {index=6},
    }
  },
  
  {action="guild_league",
    {
      {index=0},
      {index=7},
    }
  },

  {action= "member_view",
    {
      {index=0},
      {index=8}
    }
  }
}

----------------------------------- pop_action ------------------------------------------
animation {
  export = "m_pop_up_player_actions",
  id = alloc_id "m_pop_up_player_actions",
  component = {
    {id = export2id "action_bg1"}, -- 0
    {id = export2id "action_bg2"},  -- 1
    {id = export2id "action_bg3"},  -- 2
    {id = export2id "action_bg4"},  -- 3
    {name = "player_name", font = "CCBackBeat", color = 0xff42352b, align = 2, size = 16, width = 154, height = 21},  -- 4
    {name = "Button1", id = export2id "yellow_button"}, -- 5
    {name = "Button2", id = export2id "yellow_button"}, -- 6 
    {name = "Button3", id = export2id "yellow_button"}, -- 7
    {name = "Button4", id = export2id "yellow_button"}, -- 8
    {id = export2id "action_bg5"},                      -- 9
    {name = "Button5", id = export2id "yellow_button"}, -- 10
    {id = export2id "action_bg6"},                      -- 11
    {name = "Button6", id = export2id "yellow_button"}, -- 12
  },
  {action="acts1",
    {
      {index=0, mat={1024,0,0, 1024,0,0}},
      {index = 4, mat = {1024, 0, 0, 1024, 384, -856}},
      {index = 5, mat = {1024, 0, 0, 1024, 1568, 59}},
    }
  },
  
  {action="acts2",
    {
      {index=1, mat={1024,0,0, 1024,0,0}},
      {index = 4, mat = {1024, 0, 0, 1024, 368, -1192}},
      {index = 5, mat = {1024, 0, 0, 1024, 1616, -364}},
      {index = 6, mat = {1024, 0, 0, 1024, 1616, 571}},
    }
  },

  {action="acts3",
    {
      {index=2, mat={1024,0,0, 1024,0,0}},
      {index = 4, mat = {1024, 0, 0, 1024, 368, -2648}},
      {index = 5, mat = {1024, 0, 0, 1024, 1632, -1664}},
      {index = 6, mat = {1024, 0, 0, 1024, 1632, -720}},
      {index = 7, mat = {1024, 0, 0, 1024, 1632, 291}},
    }
  },
  
  {action="acts4",
    {
      {index=3, mat={1024,0,0, 1024,0,0}},
      {index = 4, mat = {1024, 0, 0, 1024, 368, -3336}},
      {index = 5, mat = {1024, 0, 0, 1024, 1584, -2336}},
      {index = 6, mat = {1024, 0, 0, 1024, 1584, -1408}},
      {index = 7, mat = {1024, 0, 0, 1024, 1584, -384}},
      {index = 8, mat = {1024, 0, 0, 1024, 1584, 559}},
    }
  },

  {action="acts5",
    {
      {index=9, mat={1024,0,0, 1024,0,0}},
      {index = 4, mat = {1024, 0, 0, 1024, 368, -4184}},
      {index = 5, mat = {1024, 0, 0, 1024, 1616, -3232}},
      {index = 6, mat = {1024, 0, 0, 1024, 1616, -2256}},
      {index = 7, mat = {1024, 0, 0, 1024, 1616, -1292}},
      {index = 8, mat = {1024, 0, 0, 1024, 1616, -332}},
      {index = 10, mat = {1024, 0, 0, 1024, 1616, 603}},
    }
  },
  {action="acts6",
    {
      {index=11, mat={1024,0,0, 1024,0,0}},
      {index = 4, mat = {1024, 0, 0, 1024, 368, -4808}},
      {index = 5, mat = {1024, 0, 0, 1024, 1616, -3968}},
      {index = 6, mat = {1024, 0, 0, 1024, 1616, -3064}},
      {index = 7, mat = {1024, 0, 0, 1024, 1616, -2140}},
      {index = 8, mat = {1024, 0, 0, 1024, 1616, -1204}},
      {index = 10, mat = {1024, 0, 0, 1024, 1616, -300}},
      {index = 12, mat = {1024, 0, 0, 1024, 1616, 619}},
    }
  }
}

----------------------------------- gain label ------------------------------------------
animation {
  export = "gain_label",
  id = alloc_id "gain_label",
  component = {
    { name = "gain", font = "Supercell-Magic", color = 0xffffffff, align = 2, size = 22, width = 303, height = 25},
  },
  {
    {
        {index = 0, mat = {1024,0,0,1024,-2719,-560} }
    }
  }
}

----------------------------------- gift ------------------------------------------
do
  local head, space = -160*16, 135*16
  local ani = {
    export = "m_gift_slide",
    id = alloc_id "m_gift_slide",
    component = {
    
    },
    {
      {
      },
    }   
  }

  local cpt = ani.component
   -- add component
  for i=1,20 do
    cpt[i] = {name = "item"..tostring(i), id = export2id "gift_item"}
  end

  -- add frame1
  local frame1 = ani[1][1]
  for i=0,#cpt-1 do
    frame1[i+1] = {index = i, mat = {1024, 0, 0, 1024, 0, head+space*i}}
  end

  animation(ani)
end

animation {
  export = "m_gift_window",
  id = alloc_id "m_gift_window",
  clipbox = {480, 510, -240, -230},
  component = {
    {name = "window", id = export2id "gift_window"},
    {name = "gift_slide", id = export2id "m_gift_slide"}
  },
  
  {
    {
      {index=0},
      {index=1, clip=true}
    }
  }
}

----------------------------------- notify ------------------------------------------

----------------------- active info --------------------------
animation {
  export = "m_active_button",
  id = alloc_id "m_active_button",
  component = {
    {id = export2id "yellow_background"},
    {id = export2id "bule_background"},
    {name = "title", font = "", color = 0xffffffff, align = 2, size = 22, width = 200, height = 25},
  },
  {action = "blue",
    {
      {index = 1, mat = {1024, 0, 0, 1024, 0, -0}},
      {index = 2, mat = {1024, 0, 0, 1024, -1600, -232}},
    },
  },
  {action = "yellow",
    {
      {index = 0, mat = {1024, 0, 0, 1024, 0, -0}},
      {index = 2, mat = {1024, 0, 0, 1024, -1600, -232}},
    },
  },
}

do
  local ani =  {
    export = "m_active_select_list",
    id = alloc_id "m_active_select_list",
    component = {
    },
    {
      {
        {index = 0, mat = {1024, 0, 0, 1024, 0, -0}},
        {index = 1, mat = {1024, 0, 0, 1024, 0, 1119}},
        {index = 2, mat = {1024, 0, 0, 1024, 0, 2239}},
        {index = 3, mat = {1024, 0, 0, 1024, 0, 3359}},
        {index = 4, mat = {1024, 0, 0, 1024, 0, 4479}},
        {index = 5, mat = {1024, 0, 0, 1024, 0, 5599}},
        {index = 6, mat = {1024, 0, 0, 1024, 0, 6719}},
        {index = 7, mat = {1024, 0, 0, 1024, 0, 7839}},
        {index = 8, mat = {1024, 0, 0, 1024, 0, 8959}},
        {index = 9, mat = {1024, 0, 0, 1024, 0, 10079}},
        {index = 10, mat = {1024, 0, 0, 1024, 0, 11199}},
        {index = 11, mat = {1024, 0, 0, 1024, 0, 12319}},
        {index = 12, mat = {1024, 0, 0, 1024, 0, 13439}},
        {index = 13, mat = {1024, 0, 0, 1024, 0, 14559}},
        {index = 14, mat = {1024, 0, 0, 1024, 0, 15679}},
        {index = 15, mat = {1024, 0, 0, 1024, 0, 16799}},
        {index = 16, mat = {1024, 0, 0, 1024, 0, 17919}},
        {index = 17, mat = {1024, 0, 0, 1024, 0, 19039}},
        {index = 18, mat = {1024, 0, 0, 1024, 0, 20159}},
        {index = 19, mat = {1024, 0, 0, 1024, 0, 21279}},
      },
    },
  }

  for i=1,20 do
    ani.component[i] = {name = "act"..(i), id = get_id "m_active_button"}
  end

  animation(ani)
end

animation {
  export = "active_info",
  id = alloc_id "active_info",
  component = {
    {name = "active_select_list", id = export2id "m_active_select_list"},
    {name = "active_login", id = export2id "active_login"},
    {name = "active_eval", id = export2id "active_eval"},
    {name = "active_reward", id = export2id "active_tuangou"},
    {name = "active_invite", id = export2id "active_invite"},
    {name = "active_input", id = export2id "active_input"},
    {name = "active_cost", id = export2id "active_cost"},
    {name = "active_giftreward", id = export2id "active_input"}
  },

  {action = "atp_login",    --- 签到
    {
      {index = 0, mat = {1024, 0, 0, 1024, -4791, -3252}},
      {index = 1, mat = {1024, 0, 0, 1024, 1720, 479}}
    }
  },

  {action = "atp_reward",    --  团购
    {
      {index = 0, mat = {1024, 0, 0, 1024, -4791, -3252}},
      {index = 3, mat = {1024, 0, 0, 1024, 1720, 479}}
    }
  },

  {action = "atp_eval",       --  统计
    {
      {index = 0, mat = {1024, 0, 0, 1024, -4791, -3252}},
      {index = 2, mat = {1024, 0, 0, 1024, 1720, 479}}
    }
  },

  {action = "atp_invite",    -- 邀请
    {
      {index = 0, mat = {1024, 0, 0, 1024, -4791, -3252}},
      {index = 4, mat = {1024, 0, 0, 1024, 1720, 479}}
    }
  },

  {action = "atp_input",    -- 输入邀请
    {
      {index = 0, mat = {1024, 0, 0, 1024, -4791, -3252}},
      {index = 5, mat = {1024, 0, 0, 1024, 1720, 479}}
    }
  },

  {action = "atp_cost",    -- 交钱
    {
      {index = 0, mat = {1024, 0, 0, 1024, -4791, -3252}},
      {index = 6, mat = {1024, 0, 0, 1024, 1720, 479}}
    }
  },
  {action = "atp_giftreward",  -- 礼包兑换
    {
      {index = 0, mat = {1024, 0, 0, 1024, -4791, -3252}},
      {index = 7, mat = {1024, 0, 0, 1024, 1720, 479}}
    }
  }
}

------------------ announcement info --------------------------
animation {
  export = "announcement_info",
  id = alloc_id "announcement_info",
  clipbox = {580,510, -580/2+1720/16, -510/2+479/16},
  component = {
    {name = "ac_select_list", id = export2id "m_active_select_list"},
    {name = "ac_info", id = export2id "ac_info"},
  },

  {
    {
      {index = 0, mat = {1024, 0, 0, 1024, -4791, -3252}},
      {index = 1, mat = {1024, 0, 0, 1024, 1720, 479}},
    }
  }
}

------------------ mail_gift info --------------------------
do
  local head, space = -3014, 135*16
  local ani = {
    export = "m_mail_gift_slide",
    id = alloc_id "m_mail_gift_slide",
    component = {
    
    },
    {
      {
      },
    }   
  }

  local cpt = ani.component
   -- add component
  for i=1,40 do
    cpt[i] = {name = "item"..tostring(i), id = export2id "mail_item"}
  end

  -- add frame1
  local frame1 = ani[1][1]
  for i=0,#cpt-1 do
    frame1[i+1] = {index = i, mat = {1024, 0, 0, 1024, 0, head+space*i}}
  end

  animation(ani)
end

animation {
  export = "mail_gift_info",
  id = alloc_id "mail_gift_info",
  component = {
    {name = "mail_gift_list", id = get_id "m_mail_gift_slide"},
  },
  
  {
    {
      {index=0}
    }
  }
}


animation {
  export = "notify_info",
  id = alloc_id "notify_info",
  clipbox = {780, 530, -390, -250},
  component = {
    {name = "active_info", id = get_id "active_info"},
    {name = "announcement_info", id = get_id "announcement_info"},
    {name = "mail_gift_info", id = get_id "mail_gift_info"}
  },

  {action = "active",
    {
      {index = 0}
    } 
  },
  {action = "notice",
    {
      {index = 1}
    }
  },
  {action = "mail",
    {
      {index = 2, clip = true}
    }
  }
}

animation {
  export = "m_notify_info_window",
  id = alloc_id "m_notify_info_window",
  component = {
    {name = "window", id = export2id "notify_info_window"},
    {name = "notify_info", id = get_id "notify_info"},
  },
  {
    {
      {index=0},
      {index=1}
    }
  }
}

----------------------------------- ranking ------------------------------------------
do
  local head, space = -3500, 1400
  local ani = {
    export = "ranking_slide",
    id = alloc_id "ranking_slide",
    component = {
      { name = "item1", id = export2id "ad_player_rank_item_1st"}, 
      { name = "item2", id = export2id "ad_player_rank_item_2nd"},   
      { name = "item3", id = export2id "ad_player_rank_item_3rd"}, 
    }, 
    {
      {

      }
    }
  }

  local cpt = ani.component
   -- add component
  for i=4,200 do
    cpt[i] = {name = "item"..(i), id = export2id "ad_player_rank_item"}
  end

  -- add frame1
  local frame1 = ani[1][1]
  for i=0,#cpt-1 do
    frame1[i+1] = {index = i, mat = {1024, 0, 0, 1024, 0, head+space*i}}
  end

  animation(ani)
end

animation {
  export = "m_ranking_window",
  id = alloc_id "m_ranking_window",
  clipbox = {838, 565-120, -419, -260},
  component = {
    {name = "window", id = export2id "ranking_base_window"},
    {name = "rank", id = export2id "ranking_slide"},
  },
  {
    {
      {index=0},
      {index=1, clip = true}
    }
  }
}

----------------------------------- momo invite ------------------------------------------
do
  local head, space = 16*(-470/2+24+45), 1400
  local ani = {
    export = "momo_invite_slide",
    id = alloc_id "momo_invite_slide",
    component = {
    }, 
    {
      {

      }
    }
  }

  local cpt = ani.component
   -- add component
  for i=1,200 do
    cpt[i] = {name = "item"..(i), id = export2id "invite_item"}
  end

  -- add frame1
  local frame1 = ani[1][1]
  for i=0,#cpt-1 do
    frame1[i+1] = {index = i, mat = {1024, 0, 0, 1024, 0, head+space*i}}
  end

  animation(ani)
end


animation {
  export = "momo_invite_window",
  id = alloc_id "momo_invite_window",
  clipbox = {380,470,-380/2,-470/2+24},
  component = {
    {name = "window", id = export2id "invite_window"},
    {name = "invite_slide", id = get_id "momo_invite_slide"}
  },
  {
    {
      {index = 0}, 
      {index = 1, clip = true}
      -- {index = 1}
    }
  }
}


_gen_slide_list_ani("gruop_slide", "invite_item", 16*(-470/2+24+45), 1400, 40)
animation {
  export = "m_momo_group_window",
  id = alloc_id "m_momo_group_window",
  clipbox = {380,470,-380/2,-470/2+24},
  component = {
    {name = "window", id = export2id "invite_window"},
    {name = "list", id = get_id "gruop_slide"},
    {name = "result", font = "CCBackBeat", color = 0xff000000, align = 2, size = 20, width = 200, height = 40},
  },
  {action = "show",
    {
      {index = 0},
      {index = 1, clip = true},
    }
  },
  {action = "no_result",
    {
      {index = 0},
      {index = 2, mat = {1024, 0, 0, 1024, -1600, -336}},
    }
  }
}

----------------------------------- momo ranking ------------------------------------------
local function _gen_ad_item(ex_name, com_name)
  return {
    export = ex_name,
    id = alloc_id(ex_name),
    component = {
      {name = "rank_item", id = export2id(com_name)},
      {name = "invite", id = export2id "long_yellow_button"},
    }, 
    
    {action = "rank",
      {0}
    },

    {action = "invite",
      {1}
    }
}
end

animation(_gen_ad_item("momo_item_1st", "momo_rank_item_1st"))
animation(_gen_ad_item("momo_item_2nd", "momo_rank_item_2nd"))
animation(_gen_ad_item("momo_item_3rd", "momo_rank_item_3rd"))
animation(_gen_ad_item("momo_item",     "momo_rank_item"))


do 
  local head, space = -3400, 1400
  local ani = {
    export = "m_momo_ranking_slide",
    id = alloc_id "m_momo_ranking_slide",
    component = {
      { name = "item1", id = export2id "momo_item_1st"}, 
      { name = "item2", id = export2id "momo_item_2nd"},   
      { name = "item3", id = export2id "momo_item_3rd"}, 
    }, 
    {
      {

      }
    }
  }

  local cpt = ani.component
   -- add component
  for i=4,200 do
    cpt[i] = {name = "item"..(i), id = export2id "momo_item"}
  end

  -- add frame1
  local frame1 = ani[1][1]
  for i=0,#cpt-1 do
    frame1[i+1] = {index = i, mat = {1024, 0, 0, 1024, 0, head+space*i}}
  end

  animation(ani)
end

animation {
  export = "m_momo_ranking_window",
  id = alloc_id "m_momo_ranking_window",
  clipbox = {838, 565-120, -419, -260},
  component = {
    {name = "window", id = export2id "ranking_momo_window"},
    {name = "rank", id = export2id "m_momo_ranking_slide"},
    {name = "member_view", id = export2id "member_view_back"},
    {name = "self_item", id = export2id "ranking_momo_self_item"}
  },
  {action="rank",
    {
      {index=0},
      {index=1, clip = true},
      {index = 3}
    }
  },
  {action = "member",
    {
      {index=0},
      {index=2}
    }
  }
}

----------------------------------- achievements ------------------------------------------
do
  local head, space = -2500, 2100
  local ani = {
    export = "m_achievement_slide",
    id = alloc_id "m_achievement_slide",
    component = {
    
    },
    {
      {
      },
    }   
  }

  local cpt = ani.component
   -- add component
  for i=1,200 do
    cpt[i] = {name = "item"..tostring(i), id = export2id "achievement_item"}
  end

  -- add frame1
  local frame1 = ani[1][1]
  for i=0,#cpt-1 do
    frame1[i+1] = {index = i, mat = {1024, 0, 0, 1024, 0, head+space*i}}
  end

  animation(ani)
end

animation {
  export = "m_achievements_window",
  id = alloc_id "m_achievements_window",
  clipbox = {810, 515, -405, -225},
  component = {
    {name = "window", id = export2id "achievements_window"},
    {name  = "achieve_slide", id = get_id "m_achievement_slide"}, 
  },

  {
    {
      {index=0},
      {index=1, clip=true}
    }
  }
}

----------------------------------- building_info ------------------------------------------
local function _gen_troop_list(name, cell, head, space, count)
  local ani = {
    export = name,
    id = alloc_id(name),
    component = {

    },
    {
      {
      },
    },
  }

  local cpt = ani.component
   -- add component
  for i=1,count do
    cpt[i] = {name = "unit_slot_"..(i), id = export2id(cell)}
  end

  -- add frame1
  local frame1 = ani[1][1]
  for i=0,#cpt-1 do
    frame1[i+1] = {index = i, mat = {1024, 0, 0, 1024, head+space*i, -1800}}
  end
  return ani
end

animation(_gen_troop_list("camp_building_info_slide", "troop_info_icon_cell", 1500, 1000, 20))

animation {
  export = "m_building_info_window",
  id = alloc_id "m_building_info_window",
  clipbox = {608, 100, -302, 80},
  component = {
    {name = "window", id = export2id "building_info_window"},
    {name = "troop_slide", id = get_id "camp_building_info_slide"}
  },
  {
    {
      {index = 0},
      {index = 1, mat = {1024, 0, 0, 1024, -5820, 3560}, clip = true}
    }
  }
}

----------------------------------- guild troops info ------------------------------
animation(_gen_troop_list("guild_camp_building_info_slide", "guild_troop_info_icon_cell", 1500, 1100, 60))

animation {
  export = "m_guild_troop_info_window",
  id = alloc_id "m_guild_troop_info_window",
  clipbox = {616, 140, -306, 60},
  component = {
      {name= "window", id = export2id "guild_troop_info_window"},
      {name = "troop_slide", id = get_id "guild_camp_building_info_slide"},
  },

  {
    {
      {index=0},
      {index = 1, mat = {1024, 0, 0, 1024, -5820, 3560}, clip = true}
    }
  }
}
----------------------------------- town hall cell ------------------------------------
do 
  local head, space = 2000, 1500
  local ani = {
    export = "ht_slide",
    id = alloc_id "ht_slide",
    component = {

    },
    {
      {
      },
    },
  }

  local cpt = ani.component
   -- add component
  for i=1,20 do
    cpt[i] = {name = "unit_slot_"..(i), id = export2id "ht_cell"}
  end

  -- add frame1
  local frame1 = ani[1][1]
  for i=0,#cpt-1 do
    frame1[i+1] = {index = i, mat = {1024, 0, 0, 1024, head+space*i, -1800}}
  end

  animation(ani)
end

animation {
  export = "m_building_upgrade_window",
  id = alloc_id "m_building_upgrade_window",
  clipbox = {608, 150, -302, 30},
  component = {
    {name = "window", id = export2id "upgrade_building_info_window"},
    {name = "ht_slide", id = get_id "ht_slide"}
  },
  {
    {
      {index = 0},
      {index = 1, mat = {1024, 0, 0, 1024, -5820, 3400}, clip = true}
    }
  }
}

----------------------------------- battle_log ----------------------------------------
_gen_slide_list_ani("log_slide", "battle_log_cell", -2700+35*16+496/2, 2600+496, 40)

animation {
  export = "m_battle_log_window",
  id = alloc_id "m_battle_log_window",
  clipbox = {838, 520, -419, -210},
  component = {
    {name = "window", id = export2id "battle_log_window"},
    {name = "logs",   id = get_id "log_slide"}
  },
  
  {
    {
      {index=0},
      {index=1, mat={1024,0,0,1024,0,0}, clip = true}
    }
  }
}
----------------------------------- barrack ----------------------------------------
--do -- {
--  --local left = 130 * 16
--  --local top = 80 * 16
--  local width = 120 * 16
--  local height = 140 * 16
--  local slot_id = export2id "m_barrack_slot"
--  local component = {}
--  local frame = {}
--  for i=1,10 do
--    --local x = left + ((i-1) % 5) * width
--    --local y = top + math.floor((i-1)/5) * height
--    local x = ((i-1) % 5) * width
--    local y = math.floor((i-1)/5) * height
--    component[i] = { name="troop_"..tostring(i), id = slot_id }
--    frame[i] = { index = i-1, trans={x,y} }
--  end
--  animation {
--    id = alloc_id "m_barrack_troop_area",
--    component = component,
--    { frame }
--  }
--end -- }
--
--do -- {
--  --local left = 100 * 16
--  --local top = 50 * 16
--  local width = 110 * 16
--  local slot_id = export2id "m_barrack_active_slot"
--  local component = {}
--  local frame = {}
--  for i=1,5 do
--    --local x = left + (5-i) * width
--    --local y = top
--    local x = (5-i) * width
--    local y = 0
--    component[i] = { name="slot_"..tostring(i), id = slot_id }
--    frame[i] = { index = i-1, trans={x,y} }
--  end
--  animation {
--    id = alloc_id "m_barrack_active_area",
--    component = component,
--    { frame }
--  }
--end -- }

--do -- { m_barrack
--  local s = 0.9
--  animation {
--    export = "m_barrack",
--    id = alloc_id "m_barrack",
--    component = {
--      {name = "window", id = export2id "barrack_window"},
--      --{name = "troop_area", id = get_id "m_barrack_troop_area"},
--      --{name = "slot_area", id = get_id "m_barrack_active_area"},
--      {name = "char_info_window"},
--      {name = "slide", id = export2id "m_window_slide_button"},
--    },
--    {
--      {
--        0,
--        {index=1, mat={1024,0,0,1024,-(240)*16,(30)*16}},
--        {index=2, mat={1024*s,0,0,1024*s,-(340)*16,-(150)*16}},
--        3,
--        4,
--      }
--    }
--  }
--end -- } end of m_barrack
----------------------------------- laboratory ----------------------------------
do -- {
  local width = 103 * 16
  local height = 136 * 16
  local slot_id = export2id "m_barrack_slot"
  local component = {}
  local frame = {}
  for i=1,40 do
    local x = math.floor((i-1) / 2) * width
    local y = ((i-1)%2) * height
    component[i] = { name="troop_"..tostring(i), id = slot_id }
    frame[i] = { index = i-1, trans={x,y} }
  end
  animation {
    id = alloc_id "lab_troop_area",
    component = component,
    { frame }
  }

  animation {
    export = "m_laboratory",
    id = alloc_id "m_laboratory",
    clipbox = {792, 284, -396, 20}, -- (116, 404)
    -- 792/2 = 396
    -- 396 + 116 = 512 = 1024 / 2
    -- 768/2 = 384
    -- 404 - 384 = 20
    component = {
      {name = "window", id = export2id "lab_window"},
      {name = "troop_area", id = get_id "lab_troop_area"},
      {name = "character_upgrade_window"},
      {name = "spell_upgrade_window"},
    },
    {
      {
        0,

        {index=1, mat={1024,0,0,1024,-309*16,90*16}, clip=true},
        --{index=1, mat={1024,0,0,1024,-309*16,90*16}},
        2,
        3
      }
    }
  }
end -- }
----------------------------------- attack menu ---------------------------------
do -- {
  local width = 103 * 16
  local slot_id = export2id "attackmenu_slot"
  local component = {}
  local frame = {}
  for i=1,20 do
    local x = (i-1) * width
    local y = 0
    component[i] = { name="troop_"..tostring(i), id = slot_id }
    frame[i] = { index = i-1, trans={x,y} }
  end
  animation {
    id = alloc_id "attackmenu_slot_area",
    component = component,
    { frame }
  }

  animation {
    export = "m_attackmenu",
    id = alloc_id "m_attackmenu",
    clipbox = {964,150,-482,599-768},
    component = {
      {name = "window", id = export2id "attackmenu_window"},
      {name = "slot_area", id = get_id "attackmenu_slot_area"},
    },
    {
      {
        0,
        {index=1, mat={1024,0,0,1024,-(103*4)*16,-94*16}, clip=true},
      }
    }
  }
end --}
----------------------------------- army ----------------------------------------
animation {
  export = "m_army_window",
  id = alloc_id "m_army_window",
  clipbox = {777.2, 150, -777.2/2, -2.5},
  component = {
    {name = "window", id = export2id "army_window"},
    {name = "slot_area", id = get_id "attackmenu_slot_area"},
    {name = "stage", id = export2id "ta_arrow"},
  },
  {
    {
      0, 
      {index = 1, mat={1024,0,0,1024,-5400,1100}, clip=true},
      {index = 2, mat = {1024, 0, 0, 1024, 5056, 2095}}
    }
  }
}
----------------------------------- shop ----------------------------------------
do
  local function build_frame_from_component(component)
    local left = 0
    local top = 0
    --local hint = 327 * 16
    --local vint = 70 * 4 * 16
    local hint = 294 * 16
    local vint = 260 * 16
    local ncomp = #component
    local frame = {}
    for i = 1, ncomp do
      local x = left + math.floor((i-1) / 2) * hint
      local y = top + ((i-1) % 2) * vint
      frame[i] = { index = i-1, mat = {1024, 0, 0, 1024, x, y} }
    end
    return frame
  end

  do
    animation {
      id = alloc_id "m_shop_item_all",
      component = {
        { name = "cat",           id = export2id "m_shop_item_cat" },
        { name = "build",         id = export2id "m_shop_item_build" },
        { name = "shield",        id = export2id "m_shop_item_shield" },
        { name = "res_pack",      id = export2id "m_shop_item_res_pack" },
        { name = "diamond_pack",  id = export2id "m_shop_item_diamond_pack" },
        { name = "info",          id = export2id "m_shop_item_info" },
        { name = "stage",         id = export2id "m_shop_arrow"},
        { name = "info_btn",      id = export2id "m_shop_info_button"},
      },
      {
        { 0, 6, 7 },
        { 1, 6, 7 },
        { 2, 6, 7 },
        { 3, 6, 7 },
        { 4, 6, 7 },
        { 5, 6, 7 },
      },
    }
  end

  do -- {
    local component = {}
    for i = 1, 30 do
      component[i] = { name = "slot_"..i, id = get_id "m_shop_item_all" }
    end
    animation {
      id = alloc_id "shop_item_area",
      component = component,
      { build_frame_from_component(component) },
    }
  end -- }

  animation {
    export = "m_shop",
    id = alloc_id "m_shop",
    clipbox = {948+8, 536+8+20+200, -512+38-4, -384+90-4-20-100},
    component = {
      {name = "window", id = export2id "shop_window"},
      {name = "item_area", id = get_id "shop_item_area"},
      -- {id = export2id "screen_mask_1"},
    },
    {
      {
        0,
        {index = 1, mat={1024,0,0,1024,-(948*0.5-294*0.5)*16,-(768/2-90-258/2)*16}, clip = true},
        -- {index = 2, mat={10240,0,0,10240,0,0}, clip = true},
      }
    }
  }
end -- end of 'shop'
----------------------------------- language ----------------------------------------
animation {
  export = "flag_language",
  id  = alloc_id "flag_language",
  component = {
    {id = export2id "flag_cn"},
    {id = export2id "flag_tw"}
  },
  {
    action = "CN",
    {0}
  },
  {
    action = "P91",
    {0}
  },
  {
    action = "TW",
    {1}
  },
  {
    action = "HK",
    {1}
  }
}
----------------------------------- language setting ----------------------------------------
--animation {
--  export = "m_flag_item",
--  id = alloc_id "m_flag_item",
--  component = {
--    {name = "flag", id = get_id "flag_language"},
--    {name = "title", font = "", color = 0xffffffff, align = 2, size = 18, width = 100, height = 25},
--  },
--  {
--    {
--      {index = 0, mat = {1024, 0, 0, 1024, -735, -0}},
--      {index = 1, mat = {1024, 0, 0, 1024, 448, -200}},
--    },
--  },
--}
--
--do
--  local head, space = -190*16+600, 1000
--   ani = {
--    export = "m_flag_slide",
--    id = alloc_id "m_flag_slide",
--    component = {
--
--    },
--    {
--      {
--
--      }
--    }
--  }
--
--  local component = ani.component
--  for i=1,10 do
--    component[i] = {name="item"..(i), id = get_id "m_flag_item"}
--  end
--
--  local frame1 = ani[1][1]
--  for i=0,#component-1 do
--    frame1[i+1] = {index=i, mat= {1024,0,0,1024,0,head+space*i}}
--  end
--
--  animation(ani)
--end
--
--animation {
--  export = "m_setting_language",
--  id = alloc_id "m_setting_language",
--  clipbox = {380,420, -190, -(430-50)/2},
--  component = {
--    {name = "window", id = export2id "setting_language"},
--    {name = "flag_slide", id = get_id "m_flag_slide"},
--  },
--  {
--    {
--      0,
--      {index = 1, mat= {1024,0,0,1024,0,0}, clip=true}
--    }
--  }
--}





----------------------------------------------------------------------------------------------
----------------------------------- coc 转换 资源  --------------------------------------------
----------------------------------------------------------------------------------------------

----------------------------------- test menu ----------------------------------------
--do -- {
--  local left = -320 * 16
--  local top = -180 * 16
--  local width = 200 * 16
--  local height = 80 * 16
--  local slot_id = export2id "blue_button"
--  local component = {}
--  local frame = {}
--  for i=1,24 do
--    local x = left + math.floor((i-1) / 6) * width
--    local y = top + ((i-1)%6) * height
--    component[i] = { name="button_"..tostring(i), id = slot_id }
--    frame[i] = { index = i-1, trans={x,y} }
--  end
--  animation {
--    id = alloc_id "m_testmenu_buttons",
--    component = component,
--    { frame }
--  }
--end -- }


--animation {
--  export = "m_testmenu",
--  id = alloc_id "m_testmenu",
--  component = {
--    {name = "window", id = export2id "testmenu_window"},
--    --{name = "buttons", id = get_id "m_testmenu_buttons"},
--  },
--  {
--    {
--      { index = 0, mat = {1024,0,0,1024,0,0} },
--      -- { index = 1, mat = {1024,0,0,1024,0,0} },
--    }
--  }
--}

----------------------------------- building text print ----------------------------------------
animation {
    export = "m_building_text_print",
    id = alloc_id "m_building_text_print",
    component = {
        { name = "text", font = "CCBackBeat", color = 4294901721, align = 2, size = 18, width = 798, height = 35, data = "00 00 00 00 02 17 fe ff fe ff 1e 03 23 00 01 ff", }, 
    },
    {
        { { index = 0, mat = { 1024,0,0,1024,-6352,-1000 }, } },
    },
}

----------------------------------- enter game hint ----------------------------------------
animation {
    export = "m_enter_game_hint",
    id = alloc_id "m_enter_game_hint",
    component = {
        { name = "text", font = "Supercell-Magic", color = 4294967295, align = 2, size = 22, width = 558, height = 129, data = "00 00 01 01 02 16 fe ff fe ff 2e 02 81 00 01 ff", }, 
    },
    {
        {
          { index = 0, mat = { 1024,0,0,1024,-4448,0 }, },
        },
    },
}

----------------------------------- hud hint ----------------------------------------
animation {
    export = "m_hud_hint",
    id = alloc_id "m_hud_hint",
    component = {
        { name = "text", font = "Supercell-Magic", color = 4294967295, align = 2, size = 22, width = 558, height = 129, data = "00 00 01 01 02 16 fe ff fe ff 2e 02 81 00 01 ff", }, 
        { name = "banner", font = "Supercell-Magic", color = 0xffff0000, align = 2, size = 22, width = 558, height = 129, data = "00 00 01 01 02 16 fe ff fe ff 2e 02 81 00 01 ff", }, 
    },
    {
        {
          { index = 0, mat = { 1024,0,0,1024,-4448,-3888+640 }, },
          { index = 1, mat = { 1024,0,0,1024,-4448,-3888+640+16*40 }, },
        },
    },
}
----------------------------------- replay ----------------------------------------
animation {
  export = "m_replay_hud",
  id = alloc_id "m_replay_hud",
  component = {
    { name = "button",  id = export2id "blue_button" },
    { name = "TID_REPLAY_SPEED", font = "CCBackBeat", color = 0xfffff7c4, align = 2, size = 18, width = 323, height = 26, data = '00 00 00 00 02 12 fe ff fe ff 43 01 1a 00 01 ff' },
  },
  {
    { { index = 0, mat = { 1024,0,0,1024,-1600,-992 }, }, { index = 1, mat = { 1024,0,0,1024,-4168,-2032 }, } },
  },
}

----------------------------------- health bar ----------------------------------------
animation {
  export = "health_bar_clan",
  id = alloc_id "health_bar_clan",
  component = {
    { id = export2id "e_677" },
    { id = export2id "e_678" },
    { name = "badge", font = "CCBackBeat", color = 0xff595546, align = 0, size = 9, width = 18, height = 18, data = '00 00 00 00 00 09 fe ff fe ff 12 00 12 00 00 ff' },
  },
   {
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 2,0,0,1024,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 12,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 22,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 32,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 43,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 53,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 63,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 73,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 83,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 94,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 104,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 114,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 124,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 135,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 145,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 155,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 165,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 175,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 186,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 196,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 206,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 216,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 227,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 237,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 247,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 257,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 267,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 278,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 288,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 298,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 308,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 319,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 329,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 339,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 349,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 359,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 370,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 380,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 390,0,0,1023,-371.2,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 400,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 411,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 421,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 431,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 441,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 451,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 462,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 472,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 482,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 492,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 503,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 513,0,0,1023,-371.2,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 523,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 533,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 544,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 554,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 564,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 574,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 584,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 595,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 605,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 615,0,0,1023,-371.2,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 625,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 636,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 646,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 656,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 666,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 676,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 687,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 697,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 707,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 717,0,0,1023,-371.2,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 728,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 738,0,0,1023,-371.2,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 748,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 758,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 768,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 779,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 789,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 799,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 809,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 820,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 830,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 840,0,0,1023,-371.2,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 850,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 860,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 871,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 881,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 891,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 901,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 912,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 922,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 932,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 942,0,0,1023,-371.2,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 952,0,0,1023,-371.2,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 963,0,0,1023,-371.2,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 973,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 983,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 993,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 1004,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-398.4,-317.6 }, }, { index = 1, mat = { 1014,0,0,1023,-372,-296 }, color = 4278255615, add = 0, }, { index = 2, mat = { 1024,0,0,1024,-688,-369.6 }, } },
    },
}


animation {
  export = "hero_health_bar_regenerate",
  id = alloc_id "hero_health_bar_regenerate",
  component = {
    { name = "xp",  id = export2id "e_684" },
    { id = export2id "e_673" },
    { name = "progress_bar",  id = export2id "e_685" },
    { name = "time", font = "Supercell-Magic", color = 0xffffffff, align = 2, size = 13, width = 114, height = 16, data = '00 00 01 01 02 0d fe ff fe ff 72 00 10 00 01 ff' },
  },
  {
   { 
        { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 3,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 9,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 14,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 19,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 25,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 30,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 35,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 40,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 46,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 51,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 56,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 62,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 67,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 72,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 77,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 83,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 88,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 93,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 99,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 104,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 109,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 114,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 120,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 125,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 130,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 136,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 141,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 146,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 151,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 157,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 162,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 167,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 173,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 178,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 183,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 188,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 194,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 199,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 204,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 210,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 215,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 220,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 225,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 231,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 236,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 241,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 247,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 252,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 257,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 262,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 268,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 273,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 278,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 284,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 289,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 294,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 299,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 305,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 310,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 315,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 321,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 326,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 331,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 336,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 342,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 347,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 352,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 358,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 363,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 368,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 373,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 379,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 384,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 389,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 395,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 400,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 405,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 410,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 416,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 421,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 426,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 432,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 437,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 442,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 447,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 453,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 458,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 463,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 469,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 474,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 479,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 484,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 490,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 495,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 500,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 506,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 511,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 516,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 521,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 527,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 532,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 537,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 543,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 548,0,0,1024,-692.8,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 553,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 558,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 564,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 569,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 574,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 580,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 585,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 590,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 595,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 601,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 606,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 611,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 617,0,0,1024,-692.8,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 622,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 627,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 632,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 638,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 643,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 648,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 654,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 659,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 664,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 669,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 675,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 680,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 685,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 691,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 696,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 701,0,0,1024,-692.8,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 706,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 712,0,0,1024,-692.8,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 717,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 722,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 728,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 733,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 738,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 743,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 749,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 754,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 759,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 765,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 770,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 775,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 780,0,0,1024,-692.8,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 786,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 791,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 796,0,0,1024,-692.8,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 802,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 807,0,0,1024,-692.8,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 812,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 817,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 823,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 828,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 833,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 839,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 844,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 849,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 854,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 860,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 865,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 870,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 876,0,0,1024,-692.8,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 881,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 886,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 891,0,0,1024,-692.8,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 897,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 902,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 907,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 913,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 918,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 923,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 928,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 934,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 939,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 944,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 950,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 955,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 960,0,0,1024,-692.8,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 965,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 971,0,0,1024,-692.8,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 976,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 981,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 987,0,0,1024,-692.8,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 992,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 997,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 1002,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 1008,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 1013,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 1018,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-1030.4,-188 }, }, 1, { index = 2, mat = { 1024,0,0,1024,-692,-256 }, }, { index = 3, mat = { 1024,0,0,1024,-792,-404.8 }, } },
    },
}


animation {
  export = "hero_health_bar_enemy",
  id = alloc_id "hero_health_bar_enemy",
  component = {
    { name = "xp",  id = export2id "e_684" },
    { id = export2id "e_675" },
    { name = "progress_bar",  id = export2id "e_687" },
  },
  {
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 3,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 7,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 11,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 15,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 18,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 22,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 26,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 29,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 33,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 37,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 40,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 44,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 48,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 51,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 55,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 59,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 63,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 66,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 70,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 74,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 77,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 81,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 85,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 88,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 92,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 96,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 99,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 103,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 107,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 111,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 114,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 118,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 122,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 125,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 129,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 133,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 136,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 140,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 144,0,0,716,-292,-384 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 147,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 151,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 155,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 159,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 162,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 166,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 170,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 173,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 177,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 181,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 184,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 188,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 192,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 195,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 199,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 203,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 207,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 210,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 214,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 218,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 221,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 225,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 229,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 232,0,0,716,-292.8,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 236,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 240,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 244,0,0,716,-292.8,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 247,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 251,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 255,0,0,716,-292.8,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 258,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 262,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 266,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 269,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 273,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 277,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 280,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 284,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 288,0,0,716,-292,-384.8 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 292,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 295,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 299,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 303,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 306,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 310,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 314,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 317,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 321,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 325,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 328,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 332,0,0,716,-292.8,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 336,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 340,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 343,0,0,716,-292.8,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 347,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 351,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 354,0,0,716,-292.8,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 358,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 362,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 365,0,0,716,-292.8,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 369,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 373,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 376,0,0,716,-292.8,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 380,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 384,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 388,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 391,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 395,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 399,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 402,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 406,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 410,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 413,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 417,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 421,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 425,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 428,0,0,716,-292,-385.6 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 432,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 436,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 439,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 443,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 447,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 450,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 454,0,0,716,-292.8,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 458,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 461,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 465,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 469,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 473,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 476,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 480,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 484,0,0,716,-292.8,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 487,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 491,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 495,0,0,716,-292.8,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 498,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 502,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 506,0,0,716,-292.8,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 509,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 513,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 517,0,0,716,-292.8,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 521,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 524,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 528,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 532,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 535,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 539,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 543,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 546,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 550,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 554,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 557,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 561,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 565,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 569,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 572,0,0,716,-292,-386.4 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 576,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 580,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 583,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 587,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 591,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 594,0,0,716,-292.8,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 598,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 602,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 605,0,0,716,-292.8,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 609,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 613,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 617,0,0,716,-292.8,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 620,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 624,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 628,0,0,716,-292.8,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 631,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 635,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 639,0,0,716,-292.8,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 642,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 646,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 650,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 654,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 657,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 661,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 665,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 668,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 672,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 676,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 679,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 683,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 687,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 690,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 694,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 698,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 702,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 705,0,0,716,-292.8,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 709,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 713,0,0,716,-292,-387.2 }, } },
        { { index = 0, mat = { 1024,0,0,1024,-606.4,-436 }, }, 1, { index = 2, mat = { 716,0,0,716,-292.8,-388 }, } },
     
  },
}


----------------------------------- building progress bar ----------------------------------------

animation {
  export = "building_progress_bar",
  id = alloc_id "building_progress_bar",
  component = {
    { id = export2id "e_627" },
    { name = "progress_bar",  id = export2id "e_636" },
    { name = "time", font = "Supercell-Magic", color = 0xfffafff4, align = 2, size = 14, width = 178, height = 24, data = '00 00 00 00 02 0e fe ff fe ff b2 00 18 00 01 ff' },
  },
  {
    { 0, { index = 1, mat = { 3,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 9,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 14,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 19,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 25,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 30,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 35,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 40,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 46,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 51,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 56,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 62,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 67,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 72,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 77,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 83,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 88,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 93,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 99,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 104,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 109,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 114,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 120,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 125,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 130,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 136,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 141,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 146,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 151,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 157,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 162,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 167,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 173,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 178,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 183,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 188,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 194,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 199,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 204,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 210,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 215,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 220,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 225,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 231,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 236,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 241,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 247,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 252,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 257,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 262,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 268,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 273,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 278,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 284,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 289,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 294,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 299,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 305,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 310,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 315,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 321,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 326,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 331,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 336,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 342,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 347,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 352,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 358,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 363,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 368,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 373,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 379,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 384,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 389,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 395,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 400,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 405,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 410,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 416,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 421,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 426,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 432,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 437,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 442,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 447,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 453,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 458,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 463,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 469,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 474,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 479,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 484,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 490,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 495,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 500,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 506,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 511,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 516,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 521,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 527,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 532,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 537,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 543,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 548,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 553,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 558,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 564,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 569,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 574,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 580,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 585,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 590,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 595,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 601,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 606,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 611,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 617,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 622,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 627,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 632,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 638,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 643,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 648,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 654,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 659,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 664,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 669,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 675,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 680,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 685,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 691,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 696,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 701,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 706,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 712,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 717,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 722,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 728,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 733,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 738,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 743,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 749,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 754,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 759,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 765,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 770,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 775,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 780,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 786,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 791,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 796,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 802,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 807,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 812,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 817,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 823,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 828,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 833,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 839,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 844,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 849,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 854,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 860,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 865,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 870,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 876,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 881,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 886,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 891,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 897,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 902,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 907,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 913,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 918,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 923,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 928,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 934,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 939,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 944,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 950,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 955,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 960,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 965,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 971,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 976,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 981,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 987,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 992,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 997,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 1002,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 1008,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 1013,0,0,1024,-799.2,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 1018,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
    { 0, { index = 1, mat = { 1024,0,0,1024,-800,-416 }, }, { index = 2, mat = { 1024,0,0,1024,-1408,-608 }, } },
  },
}

----------------------------------- building name and level ----------------------------------------
animation {
    export = "building_name_and_level",
    id = alloc_id "building_name_and_level",
    component = {
        { name = "building_name", font = "Supercell-Magic", color = 4294705089, align = 2, size = 15, width = 303, height = 18, data = "00 00 00 00 02 0f fe ff fe ff 2f 01 12 00 01 ff", }, 
        { name = "building_level", font = "CCBackBeat", color = 4291493723, align = 2, size = 19, width = 304, height = 22, data = "00 00 00 00 02 13 fe ff fe ff 30 01 16 00 01 ff", }, 
    },
    {
        { { index = 0, mat = { 1024,0,0,1024,-2415.2,-448 }, }, { index = 1, mat = { 1024,0,0,1024,-2419.2,-160 }, } },
    },
}

