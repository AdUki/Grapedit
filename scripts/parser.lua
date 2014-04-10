-- local lpeg = require 'lpeg'
local lpeg = require 'lpeglj'

lpeg.enableleftrecursion(true)

-- Create shortcuts for lpeg
V = lpeg.V
S = lpeg.S
P = lpeg.P
R = lpeg.R
L = {
	alpha = lpeg.alpha,
	alnum = lpeg.alnum,
	space = lpeg.alnum,
}

--- Function applies style to grammar and creates parser
-- @param grammar Table with LPeg rules defined, must have initial rule at index 1
-- @param style Table with names of key values of rules in grammar. From them function will create coresponding caputres.
-- @return LPeg parser
function createParser(grammar, style)

	local lpegGrammar = {}
	
	for i, v in pairs(grammar) do

		local styleType = nil
		local lpegRule = nil

		if type(grammar[i]) == "table" then
			-- Mozme mat nastavene vlastne meno na styl
			styleType = grammar[i][1] 
			lpegRule = grammar[i][2]
		else
			-- Meno stylu je take iste ako meno lpeg pravidla
			styleType = i
			lpegRule = grammar[i]
		end

		if style[styleType] then

			 -- Pre gridy potrebujeme table capture
			if style[styleType].grid then
				lpegRule = lpeg.Ct(
			        lpeg.Cg(lpeg.Cp(), 'positionBegin') *
					lpeg.Cg(lpeg.Cc(styleType), 'type') * 
					lpeg.Cg(lpeg.Ct(lpegRule), 'value') *
			        lpeg.Cg(lpeg.Cp(), 'positionEnd')
			    )

			-- Pre itemy nam staci obycajna capture
			elseif style[styleType].item then
				lpegRule = lpeg.Ct(
			        lpeg.Cg(lpeg.Cp(), 'positionBegin') *
					lpeg.Cg(lpeg.Cc(styleType), 'type') * 
					lpeg.Cg(lpeg.C(lpegRule), 'value') *
			        lpeg.Cg(lpeg.Cp(), 'positionEnd')
		        )
			else
				error("Style member '" .. styleType .. "' must be either grid or item")
			end
		end

		lpegGrammar[i] = lpegRule
	end

	return lpeg.Ct(lpegGrammar)
end

return {
	["create"] = createParser,
}