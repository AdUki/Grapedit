return {

	csv = {
		grid = true,
		object = "vertical",
		background = {
			padding = 1,
		},
	},

	topRow = {
		grid = true,
		object = "horizontal",
		background = {
			color = 0.2,
			padding = {
				top = 1,
				bottom = 1,
				left = 10,
				right = 1,
			},
		},
	},

	dataRow = {
		grid = true,
		object = "horizontal",
		background = {
			padding = {
				bottom = 1,
				left = 10,
				right = 1,
			},
			color = 0.5,
		},
	},

	headerValue = {
		item = true,
		object = "text",
		text = {
			size = 14,
			color = { 1, 0.8, 0.5 },
			bold = true,
		},
		content = {
			size = 130,
			color = 0.2,
		}
	},

	dataValue = {
		item = true,
		object = "text",
		text = {
			size = 14,
			color = 0,
		},
		content = {
			size = 130,
			color = 1,
		}
	},

	headerSeparator = {
		item = true,
		object = "text",
		text = {
			size = 14,
			bold = true,
			color = { 1, 0.9, 0.6 },
		},
		background = {
			padding = {
				right = 1,
			},
		},
		content = {
			color = 0.2,
		}
	},

	dataSeparator = {
		item = true,
		object = "text",
		text = {
			size = 14,
			bold = true,
			color = { 1, 0, 0.5 },
		},
		background = {
			padding = {
				right = 1,
			},
		},
		content = {
			color = 1,
		}
	},

	newline = {
		item = true,
		object = "image",
		image = "return.png",
	}
}
