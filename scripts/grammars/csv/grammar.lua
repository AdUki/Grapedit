return {

	[1] = V'csv',
	csv = V'topRow' * V'dataRow'^0 * V'lastDataRow'^-1,
	
	topRow  = (V'headerValue' * V'headerSeparator')^0 * V'headerValue' * V'newline',
	dataRow = (V'dataValue'   * V'dataSeparator')^0   * V'dataValue'   * V'newline',
	lastDataRow = { 'dataRow', (V'dataValue' * V'dataSeparator')^0 * V'dataValue' },

	headerValue = V'value',
	dataValue   = V'value',

	headerSeparator = V'comma',
	dataSeparator   = V'comma',

	value = (1 - V'comma' - V'newline')^1 + P(0),
	comma = P(',') + P(';'),
	newline = P'\n',
}
