-- Global functions adding updating and removing items
local addElement
local updateElement
local removeElement

--- Parse AST with new text. This function can utilize old tree to delete and update old elements and create new ones.
--
-- @param parser 
-- @param text New text to be parsed
-- @param tree Old tree, you can be nil if we dont have it 
-- @param addFnc Callback when item is created
-- @param updateFnc Callback when item is updated
-- @param removeFnc Callback when item is deleted
--
-- @return New reparsed tree (table) if succeeded, nil if not.
-- @return Number of parsed characters from beginning.
function parseAST(parser, text, tree, addFnc, updateFnc, removeFnc)

    -- initialization of variables
    addElement = addFnc
    updateElement = updateFnc
    removeElement = removeFnc

    local oldTree = tree or {}
    local newTree = parser:match(text)

    local parsedCharacters = 0
    if newTree ~= nil and #newTree > 0 then
        parsedCharacters = newTree[#newTree].positionEnd
    end

    io.write('Parsed ' .. tostring(parsedCharacters) .. ' characters, ')

    if #text + 1 ~= parsedCharacters then

        -- if grammar failed to parse whole tree, return error
        print 'reparsing done with error!\n'
        return nil, parsedCharacters

    else

        -- when successful compare trees and set new tree
        local rootOld = { type = 'root' , value = oldTree }
        local rootNew = { type = 'root' , value = newTree }
        compareTrees(rootOld, rootNew, 1, nil)

        print("reparsing file done!\n")
        return newTree, parsedCharacters
    end
end

--- Compares two nodes from ASTrees
-- @param node1
-- @param node2
-- @return True if same, false otherwise
function compareNodes(node1, node2)

    -- NOTE: print only for debug
    -- print("Comparing [" .. tostring(node1 and node1.table and node1.type) .. "," .. tostring(node1 and node1.value) 
    --     .. "] with [" .. tostring(node2 and node2.table and node2.type) .. "," .. tostring(node2 and node2.value) .. "]")
    
    if node1 == nil or node2 == nil then
        return false
    end

    return node1.type == node2.type
end

--- Compares two tables from ASTrees
-- Compares table values (ipairs) from front. If there is mismatch, function tryies to compare nodes from back. When table is occured, function CompareTrees is called.
-- @param oldTable Node of old tree
-- @param newTable Node of new tree
-- @param oldParent From which we will delete old nodes
-- @param newParent To which we will put new nodes
function compareTables(oldTable, newTable, oldParent, newParent)

    assert(oldTable ~= nil or newTable ~= nil, "Trying to compare nil with nil!")

    -- We will set empty table instead of nil because when we index empty table we will get nil
    oldTable = oldTable or {}
    newTable = newTable or {}

    local frontIndex = 1

    -- Start to compare from front until first error
    while compareNodes(oldTable[frontIndex], newTable[frontIndex]) do  
        compareTrees(
            oldTable[frontIndex], newTable[frontIndex], 
            frontIndex, frontIndex, 
            oldParent, newParent)

        frontIndex = frontIndex + 1
    end
    frontIndex = frontIndex - 1

    -- Set back indexes
    local sizeOfOld = #oldTable
    local sizeOfNew = #newTable
    local backOldIndex = 0
    local backNewIndex = 0

    -- Start compare from back until all nodes are compared
    while sizeOfOld ~= 0 or sizeOfNew ~= 0 do
        
        compareTrees(
            oldTable[sizeOfOld - backOldIndex + 1], 
            newTable[sizeOfNew - backNewIndex + 1], 
            sizeOfOld - backOldIndex + 1,
            sizeOfNew - backNewIndex + 1, 
            oldParent, newParent)

        backNewIndex = backNewIndex + 1
        backOldIndex = backOldIndex + 1

        if sizeOfOld - backOldIndex < frontIndex then
            oldTable = {}
            sizeOfOld = 0
        end

        if sizeOfNew - backNewIndex < frontIndex then
            newTable = {}
            sizeOfNew = 0
        end
    end
end

--- Functions compare trees by their root nodes
-- This function compare nodes, if they are different, node is deleted, if values are different node is updated. If node is table, function compareTables is called. If nodes are same (also when node is updated), values from old node is copied to new one.
-- Also this function adds additional information to nodes, and that are parent and parent index
-- Functions for creating, updating and deleting elements are createElement, updateElement and deleteElement
-- @param old Node of old tree
-- @param new Node of new tree
-- @param oldIndex Index of old node in oldParent
-- @param newIndex Index of new node in newParent
-- @param oldParent From which we will delete old nodes
-- @param newParent To which we will put new nodes
function compareTrees(old, new, oldIndex, newIndex, oldParent, newParent)

    if old == nil and new == nil then
        -- print ("Rejecting", old, new)
        return
    end
    -- print "Accepting"

    if new then
        new.index = newIndex
        new.parent = newParent
    end

    -- Compare nodes
    local nodesAreSame = compareNodes(old, new)
    if nodesAreSame then

        -- Copy values from old
        new.instance = old.instance

        -- Compare values and update nodes if necessary
        if  type(old.value) == 'string' and 
            type(new.value) == 'string' and 
            old.value ~= new.value then

            print (' >>> updating element with instance ' .. tostring(old.instance))

            updateElement(new, old)
        end

    -- We add nodes BEFORE processing children
    elseif new ~= nil then
        addElement(new, newParent, newIndex, oldParent)
    end

    -- Check for tables
    local tableNodeFromNew = nil
    local tableNodeFromOld = nil
    
    if old and type(old.value) == 'table' then
        -- Delete old nodes
        tableNodeFromOld = old.value
    end

    if new and type(new.value) == 'table' then
        -- Add new nodes
        tableNodeFromNew = new.value
    end

    -- Process childrens of tables
    if nodesAreSame then
        if tableNodeFromNew ~= nil or tableNodeFromOld ~= nil then
            compareTables(tableNodeFromOld, tableNodeFromNew, old, new)
        end
    else
        if tableNodeFromOld ~= nil then
            compareTables(tableNodeFromOld, nil, old, nil)
        end
        if tableNodeFromNew ~= nil then
            compareTables(nil, tableNodeFromNew, nil, new)
        end
    end

    -- We delete old nodes AFTER processing children
    if not nodesAreSame and old ~= nil then
        removeElement(old, oldParent, oldIndex)
    end
end

-- Return functions for module
return {
    parse = parseAST,
}