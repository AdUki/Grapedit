return {

	csv = {
		grid = true,
		object = "vertical",
		background = {
			padding = 10,
		},
	},

	topRow = {
		grid = true,
		object = "horizontal",
		background = {
			color = { 0.9, 0.9, 1.0 },
			padding = 10,
		},
	},

	dataRow = {
		grid = true,
		object = "horizontal",
		background = {
			padding = {
				top = 5,
				bottom = 5,
				left = 10,
				right = 10,
			},
		},
	},

	value = {
		item = true,
		object = "text",
		text = {
			size = 14,
			color = 0,
		},
		content = {
			size = 130,
			color = { 0.0, 1.0, 0.0, 0.2 },
		}
	},

	keywords = {
		item = true,
		object = "text",
		text = {
			size = 18,
			bold = true,
			color = { 1, 0, 0.5 },
		},
	},
}
