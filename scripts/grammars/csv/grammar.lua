return {

	[1] = V'csv',
	csv = V'topRow' * (V'dataRow' * V'newline')^0 * V'dataRow'^0,
	
	topRow = V'row' * V'newline'^-1,
	dataRow = V'row',

	row = (V'value' * V'comma')^0 * V'value' * V'comma'^-1,
	value = (1 - V'comma' - V'newline')^1,

	comma = { 'keywords', P(',') },
	newline = { 'keywords', P'\n' },
}
