--- Compares two AST
-- Deep compare of ASTs, is comparing only value and type
function areTreesSame(t1,t2)
	
	if t1 == nil or t2 == nil then return false end

	local ty1 = type(t1)
	local ty2 = type(t2)
	if ty1 ~= ty2 then return false end
	
	-- non-table types can be directly compared
	if ty1 ~= 'table' and ty2 ~= 'table' then return t1 == t2 end

	for k1,v1 in pairs(t1) do
		if k1 == 'value' or k1 == 'type' then 
			local v2 = t2[k1]
			if v2 == nil or not areTreesSame(v1,v2) then return false end
		end
	end
	
	for k2,v2 in pairs(t2) do
		if k2 == 'value' or k2 == 'type' then 
			local v1 = t1[k2]
			if v1 == nil or not areTreesSame(v1,v2) then return false end
		end
	end

	return true
end

--- Function writting outputs that repeats on multiple lines and parts are offseted to some amount
-- For example offsetPrint(8, 'aaa', 'bbb') prints: "aaa........bbb"
-- You can use function like this offsetPrint('DEBUG: ', 10, 'output is' .. tostring(number), 'for variable' .. tostring(var) )
-- Output: "DEBUG: output is 123002..............for variable testVar"
function offsetPrint(...)
	local parts = {...}
	local lastOffset = 0
	for i, v in ipairs(parts) do

	    if type(v) == 'number' then
	    	lastOffset = v
	    else
	    	local text = tostring(v)
		    io.write(text .. string.rep('.', lastOffset - #text))
		    lastOffset = 0
		end
	end
	print ''
end

return {
	["offsetPrint"] = offsetPrint,
	["areTreesSame"] = areTreesSame,
}