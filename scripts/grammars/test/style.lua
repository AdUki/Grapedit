local roundBracketsStyle = {
	item = true,
	object = "text",
	size = 25,
}

local squareBracketsStyle = {
	item = true,
	object = "text",
	size = 25,
}

return {
	roundBrackets = {
		grid = true,
		object = "vertical"
	},
	squareBrackets = {
		grid = true,
		object = "horizontal"
	},

	roundLeft = roundBracketsStyle,
	roundRight = roundBracketsStyle,

	squareLeft = squareBracketsStyle,
	squareRight = squareBracketsStyle,

	word = {
		item = true,
		object = "text",
		size = 17,
	},

	number = {
		item = true,
		object = "text",
		size = 20,
	},

	other = {
		item = true,
		object = "text",
		size = 14,
	},
}
