-- Add lib file and other libraries to path
package.path = './libs/?.lua;' .. package.path
package.path = './libs/?/init.lua;' .. package.path

require 'pl'

local lpeg = require 'lpeglj'
V = lpeg.V
S = lpeg.S
P = lpeg.P
R = lpeg.R

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


--- Function applies style to grammar and creates parser
function createParser(grammar, style)
	
	for i, v in pairs(grammar) do	
		if style[i] then

			 -- Pre gridy potrebujeme table capture
			if style[i].grid then
				grammar[i] = lpeg.Ct(
			        lpeg.Cg(lpeg.Cp(), 'startIndex') *
					lpeg.Cg(lpeg.Cc(i), 'type') * 
					lpeg.Cg(lpeg.Ct(v), 'value') *
			        lpeg.Cg(lpeg.Cp(), 'endIndex')
			    )

			-- Pre itemy nam staci obycajna capture
			elseif style[i].item then
				grammar[i] = lpeg.Ct(
			        lpeg.Cg(lpeg.Cp(), 'startIndex') *
					lpeg.Cg(lpeg.Cc(i), 'type') * 
					lpeg.Cg(lpeg.C(v), 'value') *
			        lpeg.Cg(lpeg.Cp(), 'endIndex')
		        )
			else
				error("Style member '" .. i .. "' must be either grid or item")
			end
		end
	end

	return lpeg.Ct(grammar)
end

--[[

--- Adds element from NEW TREE
-- @param element Node from new tree
-- @param parent Parent node from new tree
-- @param index Index of node in parent from new tree
function addElement(element, parent, index)

    -- Lua is indexing from 1, C++ from 0
    index = index - 1

    -- TODO formatovanie objektu
    if element.type == 'node' then
        element.instance = QT_addGrid(parent.instance, index )
    elseif element.type == 'leaf' then
        element.instance = QT_addItem(parent.instance, index , element.value)
    end

    local firstStr = 'ADD: {' .. tostring(element.instance) .. ' ' 
    			     .. element.type .. ' "' .. tostring(element.value) .. '"} '
    local secondStr = 'to parent {' .. tostring(parent.instance) .. ' ' .. 
    				  tostring(parent and parent.type) .. '} at index ' .. tostring(index + 1)
    print(firstStr .. string.rep('.', 60 - #firstStr) .. ' ' .. secondStr)
end

--- Removes element from OLD TREE
-- @param element Node from new tree
function removeElement(element)
    print('REMOVE: ' .. element.type .. ' "' .. tostring(element.value) .. '"')
    QT_removeElement(element.instance)
end

--- Updates element from OLD TREE to NEW TREE
-- @param newElement Node from new tree
-- @param oldElement Node from old tree
function updateElement(newElement, oldElement)
    print('UPDATE: ' .. tostring(newElement.instance) .. ' ' .. tostring(oldElement.type) .. ' "' .. tostring(oldElement.value) .. '" to "' .. tostring(newElement.value) .. '"')
    QT_updateItem(newElement.instance, newElement.value)
end

--]]