local roundBracketsStyle = {
	item = true,
	object = "text",
	size = 18,
}

local squareBracketsStyle = {
	item = true,
	object = "text",
	size = 30,
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
		size = 18,
	},

	number = {
		item = true,
		object = "text",
		size = 21,
	},

	other = {
		item = true,
		object = "text",
		size = 20,
	},
}
