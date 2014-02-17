local textItemStyle = {
	item = true,
	object = "text"
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

	roundLeft = textItemStyle,
	roundRight = textItemStyle,
	squareLeft = textItemStyle,
	squareRight = textItemStyle,
	word = textItemStyle,
	number = textItemStyle,
	other = textItemStyle,
}
