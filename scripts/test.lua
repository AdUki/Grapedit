dofile 'init.lua'

local utils = require 'utils'
local compare = require 'compare'
local parserx = require 'parser'

local grammar = require 'grammars.test.grammar'
local style = require 'grammars.test.style'

-- compile parser
local parser = parserx.create(grammar, style)

-- test it
local function parseText(text)

    print ('\nParsing text: ' .. tostring(text))
    print "==============================================="

	local newItems = {}
	local oldItems = {}

	local newTree = compare.parse(parser, text, tree,
		
		-- Add element 
		function(element, parent, index, oldParent)		    
		    utils.offsetPrint(
		    	50, 'ADD: ' .. element.type .. ' "' .. tostring(element.value) .. '" ', 
		    	18, ' at index ' .. tostring(index) .. ' ',
		    	' to parent ' .. tostring(parent.type) .. ' "' .. tostring(parent.value) .. '"')

			if tree ~= nil then
				if oldParent ~= nil and type(oldParent.value) == 'table' then
					newItems[#newItems + 1] = {oldParent.value, index, element}
				end
			end
		end,

		-- Update element
		function(newElement, oldElement)
			utils.offsetPrint(
				50, 'UPDATE: ' .. tostring(oldElement.type) .. ' "' .. tostring(oldElement.value) .. '" ',
				' to "' .. tostring(newElement.value) .. '"')

			if tree ~= nil then
				oldElement.value = newElement.value
			end
		end,

		-- Remove element
		function(element, parent, index)
		    utils.offsetPrint(
		    	50, 'REMOVE: ' .. element.type .. ' "' .. tostring(element.value) .. '" ',
		    	18, ' at index ' .. tostring(index) .. ' ',
		    	' from parent ' .. tostring(parent and parent.type) .. ' "' .. tostring(parent and parent.value) .. '"')
		    
		    if tree ~= nil then
				if parent ~= nil then
					oldItems[#oldItems + 1] = {parent.value, index}
				end
			end
		end
		)

	-- remove elements
	tablex.map(function(v) 
			table.remove(unpack(v))
			-- print "==============================================="
			-- print "ITEM REMOVED"
			-- pretty.dump(tree)
		end, oldItems)

	-- add elements
	table.sort(newItems, function(a,b) return a[2] < b[2] end) -- must be sorted, indexes can be mixed
	tablex.map(function(v) 
			if v[1][v[2]] == nil then
				v[1][v[2]] = v[3]
			else 
				table.insert(unpack(v))
			end
			-- print "==============================================="
			-- print "ITEM ADDED"
			-- pretty.dump(tree) 
		end, newItems)

	-- compare new tree with old tree
	if tree and newTree and utils.areTreesSame(newTree, tree) == false then
		print ""
		print "==============================================="
		print "OLD TREE"
		pretty.dump(tree)

		print ""
		print "==============================================="
		print "NEW TREE"
		pretty.dump(newTree)

		print ""
		print "==============================================="
		error 'Compare algorithm is invalid, AST are not same!'
	end
	pretty.dump(newTree)
	tree = newTree
end

parseText('')
parseText('(a)')
parseText('()')
parseText('(b)')
parseText('(b)a')
parseText('a(b)a')
parseText('(b)')
parseText('[a]')
parseText('(a)(((b))(b))')
parseText 's[bbb(cc[c]c)eee((nnnnnn)ddd(jjjj))]'
parseText 's[bbb(cc[c]c)eee((nnnnn)ddd(jjjj))]'
parseText 's[bbb(cc[c]c)eee((nnnnn)ddd)]'
parseText '(a)(100)(cc)(-)'
parseText '(a)(100())(c)c(-)'
parseText '(a)[(100())(c)c](-)'
parseText '(a)(100(c)c)'
parseText 's[bbb(cc[c]c)eee((nnnnn)ddd(jjjj))]'
parseText('[a]')
parseText '(a)(100)(cc)(-)'
parseText '(a)(100(c)c)'
parseText 's[bbb(cc[c]c)eee((nnnnn)ddd(jjjj))]'
parseText ''
