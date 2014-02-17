dofile 'init.lua'

local parser = require 'parser'
local compare = require 'compare'
local utils = require 'utils'

--- Load grammar and style
-- Must be called to initialize lua state
-- @param name
function loadGrammarAndStyle(name)
	local grammar = require(string.format("grammars.%s.grammar", name))
	currentActiveStyle = require(string.format("grammars.%s.style", name))
	currentActiveParser = parser.create(grammar, currentActiveStyle)
end

--- Parse new text
-- You can call it to initialize AST also to parse new text
-- @param newText
-- TODO:
-- @returns Last parsed character
function parseText(newText)

    print ('\nParsing text...')
    print "==============================================="
	local newTree = compare.parse(currentActiveParser, newText, currentActiveAST,
		
		-- Add element 
		function(element, parent, index)
		    utils.offsetPrint(
		    	50, 'ADD: ' .. element.type .. ' "' .. tostring(element.value) .. '" ', 
		    	18, ' at index ' .. tostring(index) .. ' ',
		    	' to parent ' .. tostring(parent.type) .. ' "' .. tostring(parent.value) .. '"')

			if type(element.value) == 'table' then
		        element.instance = QT_addGrid(parent.instance, element.index)
		    else
		        element.instance = QT_addItem(parent.instance, element.index , element.value)
		    end
		end,

		-- Update element
		function(newElement, oldElement)
			utils.offsetPrint(
				50, 'UPDATE: ' .. tostring(oldElement.type) .. ' "' .. tostring(oldElement.value) .. '" ',
				' to "' .. tostring(newElement.value) .. '"')

		    QT_updateItem(newElement.instance, newElement.value)
		end,

		-- Remove element
		function(element, parent, index)
		    utils.offsetPrint(
		    	50, 'REMOVE: ' .. element.type .. ' "' .. tostring(element.value) .. '" ',
		    	18, ' at index ' .. tostring(index) .. ' ',
		    	' from parent ' .. tostring(parent and parent.type) .. ' "' .. tostring(parent and parent.value) .. '"')

	        QT_removeElement(element.instance)
		end
		)

	-- pretty.dump(newTree)
	currentActiveAST = newTree
end

--- Get element style
-- @param type Element type defined in style file
-- @returns string(required), type(grid, item)
-- @returns string(required), object to be created
-- @returns string(optional), CSS style for Qt object
function getStyle(type)
	local style = currentActiveStyle[type]
	return style.type, style.object, style.style
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