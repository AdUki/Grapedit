dofile 'init.lua'

local utils = require 'utils'
local compare = require 'compare'
local parserx = require 'parser'

utils.offsetPrint = function() end

local function benchmarkGrammar(text)

    print "\n==============================================="
    print ('Parsing ' .. tostring(#text) .. ' characters')

	local benchmarkStart = os.clock()
	print ('Benchmark started at ' .. tostring(benchmarkStart))

	local additions = 0
	local updates = 0
	local deletions = 0
	tree = compare.parse(parser, text, tree, 
		function() additions = additions + 1 end, 
		function() updates = updates + 1 end, 
		function() deletions = deletions + 1 end)

	local benchmarkEnded = os.clock()
	print ('Benchmark ended at ' .. tostring(benchmarkEnded))
	print ('Total additions ' .. tostring(additions))
	print ('Total updates ' .. tostring(updates))
	print ('Total deletions ' .. tostring(deletions))
	print ('Total time ' .. tostring(benchmarkEnded - benchmarkStart))
end

-- test it
local function parseText(text)

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

	if (newTree == nil) then
		print ('Parsed text: [[' .. tostring(text) .. ']]')
		print "==============================================="
	end

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
	-- pretty.dump(newTree)
	tree = newTree
end

--[[ =========================================================================== --[]
								     Test grammar
--[] =========================================================================== --]]

local grammar = require 'grammars.test.grammar'
local style = require 'grammars.test.style'

-- compile parser
parser = parserx.create(grammar, style)
assert(parser ~= nil)

parseText ''
parseText '(a)'
parseText '()'
parseText '(b)'
parseText '(b)a'
parseText 'a(b)a'
parseText '(b)'
parseText '[a]'
parseText '(a)(((b))(b))'
parseText 's[bbb(cc[c]c)eee((nnnnnn)ddd(jjjj))]'
parseText 's[bbb(cc[c]c)eee((nnnnn)ddd(jjjj))]'
parseText 's[bbb(cc[c]c)eee((nnnnn)ddd)]'
parseText '(a)(100)(cc)(-)'
parseText '(a)(100())(c)c(-)'
parseText '(a)[(100())(c)c](-)'
parseText '(a)(100(c)c)'
parseText 's[bbb(cc[c]c)eee((nnnnn)ddd(jjjj))]'
parseText '[a]'
parseText '(a)(100)(cc)(-)'
parseText '(a)(100(c)c)'
parseText 's[bbb(cc[c]c)eee((nnnnn)ddd(jjjj))]'
parseText ''
parseText 'a 1 h 8'

--[[ =========================================================================== --[]
								     Lua grammar
--[] =========================================================================== --]]

local grammar = require 'grammars.lua.grammar'
local style = require 'grammars.lua.style'

-- compile parser
parser = parserx.create(grammar, style)

parseText [[
local ffi = require"ffi"

local band, bor, bnot, rshift, lshift = bit.band, bit.bor, bit.bnot, bit.rshift, bit.lshift

local TChar = 0
local TSet = 1
local TAny = 2 -- standard PEG elements

]]

parseText [[
local patternelement = ffi.typeof('PATTERN_ELEMENT')
local pattern = ffi.typeof('PATTERN')
local settype = ffi.typeof('int32_t[8]')
local fullset = settype(-1, -1, -1, -1, -1, -1, -1, -1)

-- {======================================================
-- Analysis and some optimizations
-- =======================================================

local codegen

]]


parseText [[
-- A few basic operations on Charsets

local function cs_complement(cs)
    for i = 0, 8 - 1 do
        cs[i] = bnot(cs[i])
    end
end


local function cs_equal(cs1, cs2)
    for i = 0, 8 - 1 do
        if cs1[i] ~= cs2[i] then
            return
        end
    end
    return true
end

]]

parseText [[

-- computes whether sets st1 and st2 are disjoint

local function cs_disjoint(st1, st2)
    for i = 0, 8 - 1 do
        if band(st1[i], st2[i]) ~= 0 then
            return
        end
    end
    return true
end

]]

parseText [[

-- Convert a 'char' pattern (TSet, TChar, TAny) to a charset

local function tocharset(tree, index, valuetable)
    local val = settype()
    if tree.p[index].tag == TSet then
        ffi.copy(val, valuetable[tree.p[index].val], ffi.sizeof(val))
        return val
    elseif tree.p[index].tag == TChar then
        local b = tree.p[index].val
        -- only one char
        -- add that one
        val[rshift(b, 5)] = lshift(1, band(b, 31))
        return val
    elseif tree.p[index].tag == TAny then
        ffi.fill(val, ffi.sizeof(val), 0xff)
        return val
    end
end

]]

parseText [[

-- checks whether a pattern has captures

local function hascaptures(tree, index)
    if tree.p[index].tag == TCapture or tree.p[index].tag == TRunTime then
        return true
    else
        local ns = numsiblings[tree.p[index].tag + 1]
        if ns == 0 then
            return
        elseif ns == 1 then
            return hascaptures(tree, index + 1)
        elseif ns == 2 then
            if hascaptures(tree, index + 1) then
                return true
            else
                return hascaptures(tree, index + tree.p[index].ps)
            end
        else
            assert(false)
        end
    end
end

]]

parseText [[

local function checkaux(tree, pred, index, lrcall)
    lrcall = lrcall or {}
    local tag = tree.p[index].tag
    if tag == TChar or tag == TSet or tag == TAny or
            tag == TFalse or tag == TOpenCall then
        return -- not nullable
    elseif tag == TRep or tag == TTrue then
        return true -- no fail
    elseif tag == TNot or tag == TBehind then
        -- can match empty, but may fail
        if pred == PEnofail then
            return
        else
            return true -- PEnullable
        end
    elseif tag == TAnd then
        -- can match empty; fail iff body does
        if pred == PEnullable then
            return true
        else
            return checkaux(tree, pred, index + 1, lrcall)
        end
    elseif tag == TRunTime then -- can fail; match empty iff body does
        if pred == PEnofail then
            return
        else
            return checkaux(tree, pred, index + 1, lrcall)
        end
    elseif tag == TSeq then
        if not checkaux(tree, pred, index + 1, lrcall) then
            return
        else
            return checkaux(tree, pred, index + tree.p[index].ps, lrcall)
        end
    elseif tag == TChoice then
        if checkaux(tree, pred, index + tree.p[index].ps, lrcall) then
            return true
        else
            return checkaux(tree, pred, index + 1, lrcall)
        end
    elseif tag == TCapture or tag == TGrammar or tag == TRule then
        return checkaux(tree, pred, index + 1, lrcall)
    elseif tag == TCall then
        if bit.band(tree.p[index].cap, 0xffff) ~= 0 then --left recursive rule
            local lr = index + tree.p[index].ps
            if lrcall[lr] then
                return
            end
            lrcall[lr] = true
        end
        return checkaux(tree, pred, index + tree.p[index].ps, lrcall)
    else
        assert(false)
    end
end

]]

parseText [[

local function fixedlenx(tree, count, len, index)
    local tag = tree.p[index].tag
    if tag == TChar or tag == TSet or tag == TAny then
        return len + 1;
    elseif tag == TFalse or tag == TTrue or tag == TNot or tag == TAnd or tag == TBehind then
        return len;
    elseif tag == TRep or tag == TRunTime or tag == TOpenCall then
        return -1;
    elseif tag == TCapture or tag == TRule or tag == TGrammar then
        return fixedlenx(tree, count, len, index + 1)
    elseif tag == TCall then
        if count >= MAXRULES then
            return -1; -- may be a loop
        else
            return fixedlenx(tree, count + 1, len, index + tree.p[index].ps)
        end
    elseif tag == TSeq then
        len = fixedlenx(tree, count, len, index + 1)
        if (len < 0) then
            return -1;
        else
            return fixedlenx(tree, count, len, index + tree.p[index].ps)
        end
    elseif tag == TChoice then
        local n1 = fixedlenx(tree, count, len, index + 1)
        if n1 < 0 then return -1 end
        local n2 = fixedlenx(tree, count, len, index + tree.p[index].ps)
        if n1 == n2 then
            return n1
        else
            return -1
        end
    else
        assert(false)
    end
end

]]

parseText [[

local function getfirst(tree, follow, index, valuetable, lrcall)
    lrcall = lrcall or {}
    local tag = tree.p[index].tag
    if tag == TChar or tag == TSet or tag == TAny then
        local firstset = tocharset(tree, index, valuetable)
        return 0, firstset
    elseif tag == TTrue then
        local firstset = settype()
        ffi.copy(firstset, follow, ffi.sizeof(firstset))
        return 1, firstset
    elseif tag == TFalse then
        local firstset = settype()
        return 0, firstset
    elseif tag == TChoice then
        local e1, firstset = getfirst(tree, follow, index + 1, valuetable, lrcall)
        local e2, csaux = getfirst(tree, follow, index + tree.p[index].ps, valuetable, lrcall)
        for i = 0, 8 - 1 do
            firstset[i] = bor(firstset[i], csaux[i])
        end
        return bor(e1, e2), firstset
    elseif tag == TSeq then
        if not checkaux(tree, PEnullable, index + 1) then
            return getfirst(tree, fullset, index + 1, valuetable, lrcall)
        else -- FIRST(p1 p2, fl) = FIRST(p1, FIRST(p2, fl))
            local e2, csaux = getfirst(tree, follow, index + tree.p[index].ps, valuetable, lrcall)
            local e1, firstset = getfirst(tree, csaux, index + 1, valuetable, lrcall)
            if e1 == 0 then -- 'e1' ensures that first can be used
                return 0, firstset
            elseif band(bor(e1, e2), 2) == 2 then -- one of the children has a matchtime?
                return 2, firstset -- pattern has a matchtime capture
            else
                return e2, firstset -- else depends on 'e2'
            end
        end
    elseif tag == TRep then
        local _, firstset = getfirst(tree, follow, index + 1, valuetable, lrcall)
        for i = 0, 8 - 1 do
            firstset[i] = bor(firstset[i], follow[i])
        end
        return 1, firstset -- accept the empty string
    elseif tag == TCapture or tag == TGrammar or tag == TRule then
        return getfirst(tree, follow, index + 1, valuetable, lrcall)
    elseif tag == TRunTime then -- function invalidates any follow info.
        local e, firstset = getfirst(tree, fullset, index + 1, valuetable, lrcall)
        if e ~= 0 then
            return 2, firstset -- function is not "protected"?
        else
            return 0, firstset -- pattern inside capture ensures first can be used
        end
    elseif tag == TCall then
        if bit.band(tree.p[index].cap, 0xffff) ~= 0 then -- left recursive rule
            local lr = index + tree.p[index].ps
            if lrcall[lr] then
                return 0, settype()
            else
                lrcall[lr] = true
            end
        end
        return getfirst(tree, follow, index + tree.p[index].ps, valuetable, lrcall)
    elseif tag == TAnd then
        local e, firstset = getfirst(tree, follow, index + 1, valuetable, lrcall)
        for i = 0, 8 - 1 do
            firstset[i] = band(firstset[i], follow[i])
        end
        return e, firstset
    elseif tag == TNot then
        local firstset = tocharset(tree, index + 1, valuetable)
        if firstset then
            cs_complement(firstset)
            return 1, firstset
        end
        local e, firstset = getfirst(tree, follow, index + 1, valuetable, lrcall)
        ffi.copy(firstset, follow, ffi.sizeof(firstset))
        return bor(e, 1), firstset -- always can accept the empty string
    elseif tag == TBehind then -- instruction gives no new information
        -- call 'getfirst' to check for math-time captures
        local e, firstset = getfirst(tree, follow, index + 1, valuetable, lrcall)
        ffi.copy(firstset, follow, ffi.sizeof(firstset))
        return bor(e, 1), firstset -- always can accept the empty string
    else
        assert(false)
    end
end

]]

parseText [[

local function headfail(tree, index, lrcall)
    lrcall = lrcall or {}
    local tag = tree.p[index].tag
    if tag == TChar or tag == TSet or tag == TAny or tag == TFalse then
        return true
    elseif tag == TTrue or tag == TRep or tag == TRunTime or tag == TNot or tag == TBehind then
        return
    elseif tag == TCapture or tag == TGrammar or tag == TRule or tag == TAnd then
        return headfail(tree, index + 1, lrcall)
    elseif tag == TCall then
        if bit.band(tree.p[index].cap, 0xffff) ~= 0 then -- left recursive rule
            local lr = index + tree.p[index].ps
            if lrcall[lr] then
                return true
            else
                lrcall[lr] = true
            end
        end
        return headfail(tree, index + tree.p[index].ps, lrcall)
    elseif tag == TSeq then
        if not checkaux(tree, PEnofail, index + tree.p[index].ps) then
            return
        else
            return headfail(tree, index + 1, lrcall)
        end
    elseif tag == TChoice then
        if not headfail(tree, index + 1, lrcall) then
            return
        else
            return headfail(tree, index + tree.p[index].ps, lrcall)
        end
    else
        assert(false)
    end
end

]]

parseText [[

local function needfollow(tree, index)
    local tag = tree.p[index].tag
    if tag == TChar or tag == TSet or tag == TAny or tag == TFalse or tag == TTrue or tag == TAnd or tag == TNot or
            tag == TRunTime or tag == TGrammar or tag == TCall or tag == TBehind then
        return
    elseif tag == TChoice or tag == TRep then
        return true
    elseif tag == TCapture then
        return needfollow(tree, index + 1)
    elseif tag == TSeq then
        return needfollow(tree, index + tree.p[index].ps)
    else
        assert(false)
    end
end

-- ======================================================

]]

parseText [[

local function addinstruction(code, op, val)
    local size = code.size
    if size >= code.allocsize then
        code:doublesize()
    end
    code.p[size].code = op
    code.p[size].val = val
    code.size = size + 1
    return size
end

]]

parseText [[

local function setoffset(code, instruction, offset)
    code.p[instruction].offset = offset;
end

]]

parseText [[

local function addinstcap(code, op, cap, key, aux)
    local i = addinstruction(code, op, bor(cap, lshift(aux, 4)))
    setoffset(code, i, key)
    return i
end
]]

parseText [[

local function jumptothere(code, instruction, target)
    if instruction >= 0 then
        setoffset(code, instruction, target - instruction)
    end
end

]]

parseText [[
local function jumptohere(code, instruction)
    jumptothere(code, instruction, code.size)
end
]]

parseText [[
local function codechar(code, c, tt)
    assert(tt ~= -1)
    if tt >= 0 and code.p[tt].code == ITestChar and
            code.p[tt].val == c then
        addinstruction(code, IAny, 0)
    else
        addinstruction(code, IChar, c)
    end
end
]]

parseText [[
local function coderealcharset(code, cs, valuetable)
    local ind = #valuetable + 1
    valuetable[ind] = cs
    return addinstruction(code, ISet, ind)
end

]]

parseText [[

local function codecharset(code, cs, tt, valuetable)
    local op, c = charsettype(cs)
    if op == IChar then
        codechar(code, c, tt)
    elseif op == ISet then
        assert(tt ~= -1)
        if tt >= 0 and code.p[tt].code == ITestSet and
                cs_equal(cs, valuetable[code.p[tt].val]) then
            addinstruction(code, IAny, 0)
        else
            coderealcharset(code, cs, valuetable)
        end
    else
        addinstruction(code, op, c)
    end
end
]]

parseText [[
local function codetestset(code, cs, e, valuetable)
    if e ~= 0 then
        return NOINST -- no test
    else
        local pos = code.size
        codecharset(code, cs, NOINST, valuetable)
        local inst = code.p[pos]
        local code = inst.code
        if code == IFail then
            inst.code = IJmp -- always jump
        elseif code == IAny then
            inst.code = ITestAny
        elseif code == IChar then
            inst.code = ITestChar
        elseif code == ISet then
            inst.code = ITestSet
        else
            assert(false)
        end
        return pos
    end
end
]]

parseText [[
local function finallabel(code, i)
    return finaltarget(code, i + code.p[i].offset)
end

]]

parseText [[
local function codebehind(code, tree, index, valuetable)
    if tree.p[index].val > 0 then
        addinstruction(code, IBehind, tree.p[index].val)
    end
    codegen(code, tree, fullset, false, NOINST, index + 1, valuetable) --  NOINST
end

]]

parseText [[
local function codechoice(code, tree, fl, opt, p1, p2, valuetable)
    local emptyp2 = tree.p[p2].tag == TTrue
    local e1, st1 = getfirst(tree, fullset, p1, valuetable)
    local _, st2 = getfirst(tree, fl, p2, valuetable)
    if headfail(tree, p1) or (e1 == 0 and cs_disjoint(st1, st2)) then
        -- <p1 / p2> == test (fail(p1)) -> L1 ; p1 ; jmp L2; L1: p2; L2:
        local test = codetestset(code, st1, 0, valuetable)
        local jmp = NOINST;
        codegen(code, tree, fl, false, test, p1, valuetable)
        if not emptyp2 then
            jmp = addinstruction(code, IJmp, 0)
        end
        jumptohere(code, test)
        codegen(code, tree, fl, opt, NOINST, p2, valuetable)
        jumptohere(code, jmp)
    elseif opt and emptyp2 then
        -- p1? == IPartialCommit; p1
        jumptohere(code, addinstruction(code, IPartialCommit, 0))
        codegen(code, tree, fullset, true, NOINST, p1, valuetable)
    else
        -- <p1 / p2> ==
        --  test(fail(p1)) -> L1; choice L1; <p1>; commit L2; L1: <p2>; L2:
        local test = codetestset(code, st1, e1, valuetable)
        local pchoice = addinstruction(code, IChoice, 0)
        codegen(code, tree, fullset, emptyp2, test, p1, valuetable)
        local pcommit = addinstruction(code, ICommit, 0)
        jumptohere(code, pchoice)
        jumptohere(code, test)
        codegen(code, tree, fl, opt, NOINST, p2, valuetable)
        jumptohere(code, pcommit)
    end
end
]]

parseText [[
local function codeand(code, tree, tt, index, valuetable)
    local n = fixedlenx(tree, 0, 0, index)
    if n >= 0 and n <= MAXBEHIND and not hascaptures(tree, index) then
        codegen(code, tree, fullset, false, tt, index, valuetable)
        if n > 0 then
            addinstruction(code, IBehind, n)
        end
    else -- default: Choice L1; p1; BackCommit L2; L1: Fail; L2:
        local pchoice = addinstruction(code, IChoice, 0)
        codegen(code, tree, fullset, false, tt, index, valuetable)
        local pcommit = addinstruction(code, IBackCommit, 0)
        jumptohere(code, pchoice)
        addinstruction(code, IFail, 0)
        jumptohere(code, pcommit)
    end
end
]]

parseText [[
local function codecapture(code, tree, fl, tt, index, valuetable)
    local len = fixedlenx(tree, 0, 0, index + 1)
    if len >= 0 and len <= MAXOFF and not hascaptures(tree, index + 1) then
        codegen(code, tree, fl, false, tt, index + 1, valuetable)
        addinstcap(code, IFullCapture, tree.p[index].cap, tree.p[index].val, len)
    else
        addinstcap(code, IOpenCapture, tree.p[index].cap, tree.p[index].val, 0)
        codegen(code, tree, fl, false, tt, index + 1, valuetable)
        addinstcap(code, ICloseCapture, Cclose, 0, 0)
    end
end
]]

parseText [[

local function coderuntime(code, tree, tt, index, valuetable)
    addinstcap(code, IOpenCapture, Cgroup, tree.p[index].val, 0)
    codegen(code, tree, fullset, false, tt, index + 1, valuetable)
    addinstcap(code, ICloseRunTime, Cclose, 0, 0)
end
]]

parseText [[
local function coderep(code, tree, opt, fl, index, valuetable)
    local st = tocharset(tree, index, valuetable)
    if st then
        local op = coderealcharset(code, st, valuetable)
        code.p[op].code = ISpan;
    else
        local e1, st = getfirst(tree, fullset, index, valuetable)
        if headfail(tree, index) or (e1 == 0 and cs_disjoint(st, fl)) then
            -- L1: test (fail(p1)) -> L2; <p>; jmp L1; L2:
            local test = codetestset(code, st, 0, valuetable)
            codegen(code, tree, fullset, opt, test, index, valuetable)
            local jmp = addinstruction(code, IJmp, 0)
            jumptohere(code, test)
            jumptothere(code, jmp, test)
        else
            -- test(fail(p1)) -> L2; choice L2; L1: <p>; partialcommit L1; L2:
            -- or (if 'opt'): partialcommit L1; L1: <p>; partialcommit L1;
            local test = codetestset(code, st, e1, valuetable)
            local pchoice = NOINST;
            if opt then
                jumptohere(code, addinstruction(code, IPartialCommit, 0))
            else
                pchoice = addinstruction(code, IChoice, 0)
            end
            local l2 = code.size
            codegen(code, tree, fullset, false, NOINST, index, valuetable)
            local commit = addinstruction(code, IPartialCommit, 0)
            jumptothere(code, commit, l2)
            jumptohere(code, pchoice)
            jumptohere(code, test)
        end
    end
end
]]

parseText [[

local function codenot(code, tree, index, valuetable)
    local e, st = getfirst(tree, fullset, index, valuetable)
    local test = codetestset(code, st, e, valuetable)
    if headfail(tree, index) then -- test (fail(p1)) -> L1; fail; L1:
        addinstruction(code, IFail, 0)
    else
        -- test(fail(p))-> L1; choice L1; <p>; failtwice; L1:
        local pchoice = addinstruction(code, IChoice, 0)
        codegen(code, tree, fullset, false, NOINST, index, valuetable)
        addinstruction(code, IFailTwice, 0)
        jumptohere(code, pchoice)
    end
    jumptohere(code, test)
end

]]

parseText [[

local function correctcalls(code, positions, from, to)
    for i = from, to - 1 do
        if code.p[i].code == IOpenCall then
            local n = code.p[i].offset; -- rule number
            local rule = positions[n]; -- rule position
            assert(rule == from or code.p[rule - 1].code == IRet)
            if bit.band(code.p[i].val, 0xffff) == 0 and code.p[finaltarget(code, i + 1)].code == IRet then -- call; ret ?
                code.p[i].code = IJmp; -- tail call
            else
                code.p[i].code = ICall;
            end
            jumptothere(code, i, rule) -- call jumps to respective rule
        end
    end
end
]]

parseText [[
local function codegrammar(code, tree, index, valuetable)
    local positions = {}
    local rulenumber = 1;
    --    tree.p[rule].tag
    local rule = index + 1
    assert(tree.p[rule].tag == TRule)
    local LR = 0
    if band(RuleLR, tree.p[rule].cap) ~= 0 then LR = 1 end
    local firstcall = addinstruction(code, ICall, LR) -- call initial rule
    code.p[firstcall].aux = tree.p[rule].val
    local jumptoend = addinstruction(code, IJmp, 0) -- jump to the end
    jumptohere(code, firstcall) -- here starts the initial rule
    while tree.p[rule].tag == TRule do
        positions[rulenumber] = code.size -- save rule position
        rulenumber = rulenumber + 1
        codegen(code, tree, fullset, false, NOINST, rule + 1, valuetable) -- code rule
        addinstruction(code, IRet, 0)
        rule = rule + tree.p[rule].ps
    end
    assert(tree.p[rule].tag == TTrue)
    jumptohere(code, jumptoend)
    correctcalls(code, positions, firstcall + 2, code.size)
end
]]

parseText [[
local function codecall(code, tree, index, val)
    local c = addinstruction(code, IOpenCall, tree.p[index].cap) -- to be corrected later
    code.p[c].aux = val
    assert(tree.p[index + tree.p[index].ps].tag == TRule)
    setoffset(code, c, band(tree.p[index + tree.p[index].ps].cap, 0x7fff)) -- offset = rule number
end

]]

parseText [[
local function codeseq(code, tree, fl, opt, tt, p1, p2, valuetable)
    if needfollow(tree, p1) then
        local _, fll = getfirst(tree, fl, p2, valuetable) -- p1 follow is p2 first
        codegen(code, tree, fll, false, tt, p1, valuetable)
    else -- use 'fullset' as follow
        codegen(code, tree, fullset, false, tt, p1, valuetable)
    end
    if (fixedlenx(tree, 0, 0, p1) ~= 0) then -- can p1 consume anything?
        tt = NOINST; -- invalidate test
    end
    codegen(code, tree, fl, opt, tt, p2, valuetable)
end

]]

parseText [[
function codegen(code, tree, fl, opt, tt, index, valuetable)
    local tag = tree.p[index].tag
    if tag == TChar then
        codechar(code, tree.p[index].val, tt)
    elseif tag == TAny then
        addinstruction(code, IAny, 0)
    elseif tag == TSet then
        codecharset(code, valuetable[tree.p[index].val], tt, valuetable)
    elseif tag == TTrue then
    elseif tag == TFalse then
        addinstruction(code, IFail, 0)
    elseif tag == TSeq then
        codeseq(code, tree, fl, opt, tt, index + 1, index + tree.p[index].ps, valuetable)
    elseif tag == TChoice then
        codechoice(code, tree, fl, opt, index + 1, index + tree.p[index].ps, valuetable)
    elseif tag == TRep then
        coderep(code, tree, opt, fl, index + 1, valuetable)
    elseif tag == TBehind then
        codebehind(code, tree, index, valuetable)
    elseif tag == TNot then
        codenot(code, tree, index + 1, valuetable)
    elseif tag == TAnd then
        codeand(code, tree, tt, index + 1, valuetable)
    elseif tag == TCapture then
        codecapture(code, tree, fl, tt, index, valuetable)
    elseif tag == TRunTime then
        coderuntime(code, tree, tt, index, valuetable)
    elseif tag == TGrammar then
        codegrammar(code, tree, index, valuetable)
    elseif tag == TCall then
        codecall(code, tree, index, tree.p[index].val)
    else
        assert(false)
    end
end
]]

parseText [[
local function peephole(code)
    local i = 0
    while i < code.size do
        local tag = code.p[i].code
        if tag == IChoice or tag == ICall or tag == ICommit or tag == IPartialCommit or
                tag == IBackCommit or tag == ITestChar or tag == ITestSet or tag == ITestAny then
            -- instructions with labels
            jumptothere(code, i, finallabel(code, i)) -- optimize label

        elseif tag == IJmp then
            local ft = finaltarget(code, i)
            local tag = code.p[ft].code -- jumping to what?
            if tag == IRet or tag == IFail or tag == IFailTwice or tag == IEnd then -- instructions with unconditional implicit jumps
                ffi.copy(code.p + i, code.p + ft, ffi.sizeof(patternelement)) -- jump becomes that instruction
            elseif tag == ICommit or tag == IPartialCommit or tag == IBackCommit then -- inst. with unconditional explicit jumps
                local fft = finallabel(code, ft)
                ffi.copy(code.p + i, code.p + ft, ffi.sizeof(patternelement)) -- jump becomes that instruction...
                jumptothere(code, i, fft) -- but must correct its offset
                i = i - 1 -- reoptimize its label
            else
                jumptothere(code, i, ft) -- optimize label
            end
        end
        i = i + 1
    end
end
]]

parseText [[
local function compile(tree, index, valuetable)
    local code = pattern()
    codegen(code, tree, fullset, false, NOINST, index, valuetable)
    addinstruction(code, IEnd, 0)
    peephole(code)
    ffi.C.free(tree.code)
    tree.code = code
end

]]

parseText [[
local function pat_new(ct, size)
    size = size or 0
    local allocsize = size
    if allocsize < 10 then
        allocsize = 10
    end
    local pat = ffi.cast('PATTERN*', ffi.C.malloc(ffi.sizeof(pattern)))
    assert(pat ~= nil)
    pat.allocsize = allocsize
    pat.size = size
    pat.p = ffi.C.malloc(ffi.sizeof(patternelement) * allocsize)
    assert(pat.p ~= nil)
    ffi.fill(pat.p, ffi.sizeof(patternelement) * allocsize)
    return pat
end
]]

parseText [[

local function doublesize(ct)
    ct.p = ffi.C.realloc(ct.p, ffi.sizeof(patternelement) * ct.allocsize * 2)
    assert(ct.p ~= nil)
    ffi.fill(ct.p + ct.allocsize, ffi.sizeof(patternelement) * ct.allocsize)
    ct.allocsize = ct.allocsize * 2
end
]]

parseText [[
local pattreg = {
    doublesize = doublesize,
}

]]

parseText [[
local metareg = {
    ["__new"] = pat_new,
    ["__index"] = pattreg
}

ffi.metatype(pattern, metareg)

return {
    checkaux = checkaux,
    tocharset = tocharset,
    fixedlenx = fixedlenx,
    hascaptures = hascaptures,
    compile = compile,
}
]]

parseText'(a)()'
parseText'(a).a()'
parseText'a((a)())'
parseText'a(a == true)'
parseText'a((a) == true)'
parseText'a((a).a == true)'
parseText' fnc((reply or emptytable).queued == true) '

parseText'for k, v in pairs(defaults) do end'
parseText'for i = 1, 1, 1 do end'
parseText'if not hascaptures(tree) then end'
parseText'a=(a)or(c)and order'
parseText[[
a = c
order = 6
]]

parseText [[
a = a + a / a + a + a * a * a * a * a * a + a + a + a + a / a * a * a * a * a * a / a + a + a + a / a + a + a / a + a + a * a * a * a * a * a + a + a + a / a + a + a + a + a + a / a + a + a + a + a
]]

parseText [[

local exp = m.P{ "Exp",
  Exp = S * ( m.V"Grammar"
            + m.Cf(m.V"Seq" * ("/" * S * m.V"Seq")^0, mt.__add) );
  Seq = m.Cf(m.Cc(m.P"") * m.V"Prefix"^0 , mt.__mul)
        * (#seq_follow + patt_error);
  Prefix = "&" * S * m.V"Prefix" / mt.__len
         + "!" * S * m.V"Prefix" / mt.__unm
         + m.V"Suffix";
  Suffix = m.Cf(m.V"Primary" * S *
          ( ( m.P"+" * m.Cc(1, mt.__pow)
            + m.P"*" * m.Cc(0, mt.__pow)
            + m.P"?" * m.Cc(-1, mt.__pow)
            + "^" * ( m.Cg(num * m.Cc(mult))
                    + m.Cg(m.C(m.S"+-" * m.R"09"^1) * m.Cc(mt.__pow))
                    )
            + "->" * S * ( m.Cg((String + num) * m.Cc(mt.__div))
                         + m.P"{}" * m.Cc(nil, m.Ct)
                         + m.Cg(Def / getdef * m.Cc(mt.__div))
                         )
            + "=>" * S * m.Cg(Def / getdef * m.Cc(m.Cmt))
            ) * S
          )^0, function (a,b,f) return f(a,b) end );
  Primary = "(" * m.V"Exp" * ")"
            + String / mm.P
            + Class
            + defined
            + "{:" * (name * ":" + m.Cc(nil)) * m.V"Exp" * ":}" /
                     function (n, p) return mm.Cg(p, n) end
            + "=" * name / function (n) return mm.Cmt(mm.Cb(n), equalcap) end
            + m.P"{}" / mm.Cp
            + "{~" * m.V"Exp" * "~}" / mm.Cs
            + "{|" * m.V"Exp" * "|}" / mm.Ct
            + "{" * m.V"Exp" * "}" / mm.C
            + m.P"." * m.Cc(any)
            + (name * m.Cb("G") * (S * ":" * S * num)^-1 * -arrow + "<" * name * m.Cb("G") * (S * ":" * S * num)^-1 * ">") / NT;
  Definition = name * arrow * m.V"Exp";
  Grammar = m.Cg(m.Cc(true), "G") *
            m.Cf(m.V"Definition" / firstdef * m.Cg(m.V"Definition")^0,
              adddef) / mm.P
}
]]

parseText' for i=1,1,1 do end '