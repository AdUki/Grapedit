return {

	[1] = V'chunk'^0,
	
	roundBrackets = V'roundLeft' * V'chunk'^0 * V'roundRight',
	roundLeft = P'(',
	roundRight = P')',

	squareBrackets = V'squareLeft' * V'chunk'^0 * V'squareRight',
	squareLeft = P'[',
	squareRight = P']',
	
	word = (R'az' + R'AZ')^1,
	number = R'09'^1 * P'.'^-1 * R'09'^0 ,
	other = (P(1) - S'()[]')^1,

	chunk = V'squareBrackets' + V'roundBrackets' + V'token',
	token = V'word' + V'number' + V'other',
}
