local genlist = require "genlist"


local str = genlist.gen("active_select_list", 20, "act", "blue_button2_complex.json", 0, 70)

genlist.writefile("../sg_ui/active_select_list_complex.json", str)