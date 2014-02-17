local lpeg = require 'lpeglj'

-- Create shortcuts for lpeg
V = lpeg.V
S = lpeg.S
P = lpeg.P
R = lpeg.R

--- Function applies style to grammar and creates parser
-- @param grammar Table with LPeg rules defined, must have initial rule at index 1
-- @param style Table with names of key values of rules in grammar. From them function will create coresponding caputres.
-- @return LPeg parser
function createParser(grammar, style)
	
	for i, v in pairs(grammar) do	
		if style[i] then

			 -- Pre gridy potrebujeme table capture
			if style[i].grid then
				grammar[i] = lpeg.Ct(
			        lpeg.Cg(lpeg.Cp(), 'positionBegin') *
					lpeg.Cg(lpeg.Cc(i), 'type') * 
					lpeg.Cg(lpeg.Ct(v), 'value') *
			        lpeg.Cg(lpeg.Cp(), 'positionEnd')
			    )

			-- Pre itemy nam staci obycajna capture
			elseif style[i].item then
				grammar[i] = lpeg.Ct(
			        lpeg.Cg(lpeg.Cp(), 'positionBegin') *
					lpeg.Cg(lpeg.Cc(i), 'type') * 
					lpeg.Cg(lpeg.C(v), 'value') *
			        lpeg.Cg(lpeg.Cp(), 'positionEnd')
		        )
			else
				error("Style member '" .. i .. "' must be either grid or item")
			end
		end
	end

	return lpeg.Ct(grammar)
end

return {
	["create"] = createParser,
}