print "Initializing core"

local parser = require 'parser'
local compare = require 'compare'
local utils = require 'utils'

--- Load grammar and style
-- Must be called to initialize lua state
-- @param name
function loadGrammarAndStyle(name)
	print("Loading grammar and style: " .. name)

	local grammar = require(string.format("grammars.%s.grammar", name))
	currentActiveStyle = require(string.format("grammars.%s.style", name))
	currentActiveParser = parser.create(grammar, currentActiveStyle)

	local sep = "/"
	currentGramarBasePath = "scripts"..sep.."grammars"..sep..name..sep

	print("Grammar and style '" .. currentGramarBasePath .. "' successfully loaded.")
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
		    print('ADD: ' .. element.type .. ' "' .. tostring(element.value) .. '" at index ' .. tostring(index) .. ' to parent[' .. tostring(parent.instance) .. '] ' .. tostring(parent.type) .. ' "' .. tostring(parent.value) .. '"')

			-- Pozor Lua indexuje od 1, C++ od 0
			if type(element.value) == 'table' then
		        element.instance = send_addGrid(element.type, parent.instance, element.index - 1)
		    else
		        element.instance = send_addItem(element.type, element.value, parent.instance, element.index - 1)
		    end
		end,

		-- Update element
		function(newElement, oldElement)
			print('UPDATE: ' .. tostring(oldElement.type) .. ' "' .. tostring(oldElement.value) .. '" to[' .. tostring(newElement.instance) .. ', ' .. tostring(newElement.value) .. ']')
			
		    send_updateItem(newElement.instance, newElement.value)
		end,

		-- Remove element
		function(element, parent, index)
		    print('REMOVE: ' .. element.type .. ' "' .. tostring(element.value) .. '" at index ' .. tostring(index) .. ' from parent ' .. tostring(parent and parent.type) .. ' "' .. tostring(parent and parent.value) .. '"')

	        send_removeItem(element.instance)
		end
		)

	-- pretty.dump(newTree)

	-- If successfully parsed we will set new tree and commit updates
	if newTree ~= nil then
		currentActiveAST = newTree
		send_commit()
	end
end

--- Get path to current selected grammar. Useful for loading images for display.
-- @returns string(required) Base path to current grammar
function getCurrentGrammarPath()
	assert(currentGramarBasePath)
	return currentGramarBasePath
end

--- Get element style
-- @param type Element type defined in style file
-- @returns string(required), type(grid, item)
-- @returns string(required), object to be created
-- @returns string(optional), CSS style for Qt object
function getElementStyle(type)
	local style = currentActiveStyle[type]
	return style.type, style.object, style.style
end

