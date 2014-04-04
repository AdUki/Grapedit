local roundBracketsStyle = {
	item = true,
	object = "text",
	text = {
		size = 18,
		color = { 0, 0.5, 0 },
	},
}

local squareBracketsStyle = {
	item = true,
	object = "text",
	text = {
		size = 20,
		color = { 0, 0.5, 0 },
		bold = true,
	},
}

return {
	roundBrackets = {
		grid = true,
		object = "vertical",
		background = {
			color = { 0, 0, 1, 0.05 },
		},
	},
	squareBrackets = {
		grid = true,
		object = "horizontal",
		background = {
			color = { 1, 0, 0, 0.05 },
		},
	},

	roundLeft = roundBracketsStyle,
	roundRight = roundBracketsStyle,

	squareLeft = squareBracketsStyle,
	squareRight = squareBracketsStyle,

	word = {
		item = true,
		object = "text",
		text = {
			size = 18,
			color = { 0.1, 0.1, 0.1 },
			bold = true,
		},
		content = {
			color = { 1, 1, 0, 0.5 },
		},
	},

	number = {
		item = true,
		object = "text",
		text = {
			size = 21,
			color = { 0, 0, 1 },
		},
	},

	other = {
		item = true,
		object = "text",
		text = {
			size = 20,
			color = { 1, 0, 1 }
		},
	},
}
