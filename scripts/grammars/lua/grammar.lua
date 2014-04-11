local whitespace = S' \n\t'
local EOF = P(-1)
local number = R'09'^1 * P( P'.' * R'09'^1 )^-1

-- TODO: vytiahnut komentare do samostatnych blokov

local function token(pattern)
	return pattern * whitespace^0 * V'comment'^-1
end

local function tokenw(pattern)
	return pattern * whitespace^1 * V'comment'^-1
end

-- Adds boundary flag (\b in regexp) at the end of pattern
local function tokenb(pattern)
	return (pattern * #((1 - (R'az' + R'AZ' + P'_') + EOF))) * whitespace^0 * V'comment'^-1
end


return {

	--[[========================================================]]

	keyword_break = 	{ "keyword" , tokenb'break' },
	keyword_do = 		{ "keyword" , tokenb'do' },
	keyword_else = 		{ "keyword" , tokenb'else' },
	keyword_elseif = 	{ "keyword" , tokenb'elseif' },
	keyword_end = 		{ "keyword" , tokenb'end' },
	keyword_false = 	{ "keyword" , tokenb'false' },
	keyword_for = 		{ "keyword" , tokenb'for' },
	keyword_function = 	{ "keyword" , tokenb'function' },
	keyword_goto = 		{ "keyword" , tokenb'goto' },
	keyword_if = 		{ "keyword" , tokenb'if' },
	keyword_in = 		{ "keyword" , tokenb'in' },
	keyword_local = 	{ "keyword" , tokenb'local' },
	keyword_nil = 		{ "keyword" , tokenb'nil' },
	keyword_repeat = 	{ "keyword" , tokenb'repeat' },
	keyword_return = 	{ "keyword" , tokenb'return' },
	keyword_then = 		{ "keyword" , tokenb'then' },
	keyword_true = 		{ "keyword" , tokenb'true' },
	keyword_until = 	{ "keyword" , tokenb'until' },
	keyword_while = 	{ "keyword" , tokenb'while' },

	--[[========================================================]]

	['#'] = 	{ "operator" , token'#' },
	['%'] = 	{ "operator" , token'%' },
	['('] = 	{ "operator" , token'(' },
	[')'] = 	{ "operator" , token')' },
	['*'] = 	{ "operator" , token'*' },
	['+'] = 	{ "operator" , token'+' },
	[','] = 	{ "operator" , token',' },
	['-'] = 	{ "operator" , token'-' - P'--' },
	['.'] = 	{ "operator" , token'.' },
	['..'] = 	{ "operator" , token'..' },
	['...'] = 	{ "operator" , token'...' },
	['/'] = 	{ "operator" , token'/' },
	[':'] = 	{ "operator" , token':' },
	['::'] = 	{ "operator" , token'::' },
	[';'] = 	{ "operator" , token';' },
	['<'] = 	{ "operator" , token'<' },
	['<='] = 	{ "operator" , token'<=' },
	['='] = 	{ "operator" , token'=' },
	['=='] = 	{ "operator" , token'==' },
	['>'] = 	{ "operator" , token'>' },
	['>='] = 	{ "operator" , token'>=' },
	['['] = 	{ "operator" , token'[' },
	[']'] = 	{ "operator" , token']' },
	['^'] = 	{ "operator" , token'^' },
	['and'] = 	{ "operator" , tokenb'and' },
	['or'] = 	{ "operator" , tokenb'or' },
	['not'] = 	{ "operator" , tokenb'not' },
	['{'] = 	{ "operator" , token'{' },
	['}'] = 	{ "operator" , token'}' },
	['~='] = 	{ "operator" , token'~=' },

	--[[========================================================]]

	block_comment = P'--[[' * (1 - P"]]")^0 * P"]]"^-1 * whitespace^0,
	normal_comment = P'--' * (1 - P'\n')^0 * P'\n'^-1 * whitespace^0 ,
	comment = (V'block_comment' + V'normal_comment')^1,

	--[[========================================================]]

	-- chunk ::= block
	[1] =  V'block',
	
	-- block ::= {stat} [return_stat]
	block = token'' 
	      * V'stat'^0 
	      * V'return_stat'^-1
		  ,

	-- stat ::=
	stat = V'function_stat'
		 + V'if_stat'
		 + V'while_stat'
		 + V'repeat_stat'
		 + V'for_stat'
		 + V'do_stat'
		 + V'functioncall_stat'
		 + V'break_stat'
		 + V'goto_stat'
		 + V'assing_stat'
		 + V'label_stat'
		 + V';'
		 ,

	-- var_list ‘=’ exp_list
	-- local name_list [‘=’ exp_list] 
	assing_stat = V'var_list' * V'=' * V'exp_list'
				+ V'keyword_local' * V'name_list' * P( V'=' * V'exp_list' )^-1,

	-- goto Name | 
	goto_stat =  V'keyword_goto' * V'Name',
		
	-- do block end | 
	do_stat = V'keyword_do' * V'block' * V'keyword_end',

	-- while exp do block end | 
	while_stat = V'keyword_while' * V'exp' * V'keyword_do' * V'block' * V'keyword_end',

	-- repeat block until exp | 
	repeat_stat = V'keyword_repeat' * V'block' * V'keyword_until' * V'exp',

	-- if exp then block {elseif exp then block} [else block] end | 
	if_stat = V'keyword_if' * V'exp' 
			* V'keyword_then' * V'block' 
	        * P( V'keyword_elseif' * V'exp' * V'keyword_then' * V'block' )^0 
	        * ( V'keyword_else' * V'block' )^-1
	        * V'keyword_end',

	-- for Name ‘=’ exp ‘,’ exp [‘,’ exp] do block end | 
	-- for name_list in exp_list do block end | 
	for_stat = V'keyword_for' 
			   * V'Name' * V'=' * V'exp' * V',' * V'exp' * ( V',' * V'exp' )^-1 
			   * V'keyword_do' * V'block' * V'keyword_end'
			 + V'keyword_for' 
			   * V'name_list' * V'keyword_in' * V'exp_list' 
			   * V'keyword_do' * V'block' * V'keyword_end',

	-- label_stat ‘::’ Name ‘::’
	label_stat = V'::' * V'Name' * V'::',

	-- break
	break_stat = V'keyword_break',

	

	-- function funcname funcbody | 
	-- local function Name funcbody | 
	function_stat = V'keyword_function' * V'funcname' * V'funcbody'
				  + V'keyword_local' * V'keyword_function' * V'Name' * V'funcbody'
				  ,

	-- funcname ::= Name {‘.’ Name} [‘:’ Name]
	funcname = V'Name' * P( V'.' * V'Name' )^0 * P( V':' * V'Name' )^-1,

	-- functiondef ::= function funcbody
	functiondef = V'keyword_function' * V'funcbody',

	-- funcbody ::= ‘(’ [par_list] ‘)’ block end
	funcbody = V'(' * V'par_list'^-1 * V')' * V'block' * V'keyword_end',

	-- return_stat ::= return [exp_list] [‘;’]
	return_stat = V'keyword_return' * V'exp_list'^-1 * V';'^-1,



	-- var_list ::= var {‘,’ var}
	var_list = V'var' * P( V',' * V'var' )^0,

	-- var ::=  Name | prefixexp ‘[’ exp ‘]’ | prefixexp ‘.’ Name 
	var = V'prefixexp' * V'[' * V'exp' * V']'
		+ V'prefixexp' * V'.' * V'Name'
		+ V'Name'
		,

	-- name_list ::= Name {‘,’ Name}
	name_list = V'Name' * P( V',' * V'Name' )^0,

	-- exp_list ::= exp {‘,’ exp}
	exp_list = V'exp' * P( V',' * V'exp' )^0,

	-- speed hack, rule: V'exp' * V'binary_op' * V'exp'
	-- can result in O(!n) performance while chaining binary operators: res = a+a+a+a+a+a+a+a+a+a+a+a+a
	exp_op_list = V'exp' * ( V'binary_op' * V'exp_op_list' )^0,

	-- exp ::=  nil | false | true | Number | String | ‘...’ | functiondef | 
	-- 	 prefixexp | tableconstructor | exp binary_op exp | unary_op exp 
	exp = V'comment'
		+ V'exp' * V'binary_op' * V'exp'
		+ V'unary_op' * V'exp'
		+ V'prefixexp' 
		+ V'tableconstructor' 
		+ V'functiondef' 
		+ V'keyword_nil' 
		+ V'keyword_false' 
		+ V'keyword_true' 
		+ V'Number' 
		+ V'String' 
		+ V'...'
		,

	-- prefixexp ::= var | functioncall_stat | ‘(’ exp ‘)’
	prefixexp = V'functioncall_stat'
			  + V'var' 
			  + V'(' * V'exp' * V')'
		      ,

	-- functioncall_stat ::=  prefixexp args | prefixexp ‘:’ Name args 
	functioncall_stat = V'prefixexp' * V'args' 
				      + V'prefixexp' * V':' * V'Name' * V'args',

	-- args ::=  ‘(’ [exp_list] ‘)’ | tableconstructor | String 
	args = V'(' * V'exp_list'^-1 * V')'
		 + V'tableconstructor'
		 + V'String',

	-- par_list ::= name_list [‘,’ ‘...’] | ‘...’
	par_list = V'name_list' * P( V',' * V'...' )^-1 + V'...',

	-- tableconstructor ::= ‘{’ [field_list] ‘}’
	tableconstructor = V'{' * V'field_list'^-1 * V'}',

	-- fieldlist ::= field {field_sep field} [field_sep]
	field_list = V'field' * P( V'field_sep' * V'field' )^0 * V'field_sep'^-1,

	-- field ::= ‘[’ exp ‘]’ ‘=’ exp | Name ‘=’ exp | exp
	field = V'[' * V'exp' * V']' * V'=' * V'exp'
		  + V'Name' * V'=' * V'exp'
		  + V'exp',

	-- field_sep ::= ‘,’ | ‘;’
	field_sep = V',' + V';',

	-- binary_op ::= ‘+’ | ‘-’ | ‘*’ | ‘/’ | ‘^’ | ‘%’ | ‘..’ | 
	-- 	 ‘<’ | ‘<=’ | ‘>’ | ‘>=’ | ‘==’ | ‘~=’ | 
	-- 	 and | or
	binary_op = V'<=' 
		      + V'>=' 
		      + V'==' 
		      + V'~=' 
		      + V'and' 
		      + V'or'
		      + V'+' 
	          + V'-'
	          + V'*' 
	          + V'/' 
	          + V'^' 
	          + V'%' 
	          + V'..' 
	          + V'<'
		      + V'>'
		      ,

	-- unary_op ::= ‘-’ | not | ‘#’
	unary_op =
			 ( V'not'
			 + V'-' 
			 + V'#'
			 ) * V'comment'^-1,

	-- Names in Lua can be any string of letters, digits, and underscores, not beginning with a digit BUT they cannot be any keyword!
	-- TODO: zahrnut vsetky znaky z locale
	Name = token( (R'az' + R'AZ' + P'_') * (R'az' + R'AZ' + R'09' + P'_')^0 )
	     - ((
	       P'break' + P'do' + P'elseif' + P'else' + P'end' + 
	       P'false' + P'for' + P'function' + P'goto' + P'if' + 
	       P'in' + P'local' + P'nil' + P'repeat' + P'return' + 
	       P'then' + P'true' + P'until' + P'while'
	       ) * (1 - (R'az' + R'AZ' + P'_')) )
	     ,

	String = token(P'"' * (P('\\"') + P(1 - S'"\n') )^0 * P'"')
		   + token(P"'" * (P("\\'") + P(1 - S"'\n") )^0 * P"'")
		   + token(P'[[' * (1 - P"]]")^0 * P"]]")
		   ,

	Number = token(P'0x' * P( R'09' + R'af' + R'AF' )^1) -- hexa
		   + token( number * (P'e' * number)^-1 )        -- decimal / floating / e-notation
		   ,
}
