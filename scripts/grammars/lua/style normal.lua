local NORMAL_TEXT = {
	item = true,
	object = "text",
	text = {
		size = 13,
		color = 0,
		bold = false,
	}
}

local function COLOR_TEXT(color)
	return {
		item = true,
		object = "text",
		text = {
			size = 13,
			color = color,
			bold = false,
		}
	}
end

local function COLOR_BOLD_TEXT(color)
	return {
		item = true,
		object = "text",
		text = {
			size = 13,
			color = color,
			bold = false,
		}
	}
end

local NORMAL_BLOCK = {
	grid = true,
	object = "vertical",
		background = {
		padding = 10,
	},
}

local OFFSET_BLOCK = {
	grid = true,
	object = "vertical",
	background = {
		padding = {
			left = 10,
		},
	},
}

local LINE = {
	grid = true,
	object = "horizontal",
}

return {

	block = OFFSET_BLOCK,
	stat = OFFSET_BLOCK,

	par_list = LINE,
	dot_comma = LINE,
	assing_stat = LINE,
	functioncall_stat = LINE,
	label_stat = LINE,
	break_stat = LINE,
	goto_stat = LINE,
	return_stat = LINE,
	exp = LINE,

	keyword = COLOR_BOLD_TEXT({ 0, 0, 1 }),
	operator = COLOR_BOLD_TEXT({ 1, 0, 1 }),
	comment = COLOR_TEXT(0.5),

	Name = NORMAL_TEXT,
	Number = COLOR_BOLD_TEXT({ 0, 0.8, 0 }),
	String = COLOR_TEXT({ 1, 0, 0 }),

	cycle = COLOR_BOLD_TEXT({ 0, 0, 1 }),
}
