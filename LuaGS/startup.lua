local nativePrint = print
local nativeRead = read
local nativeStringChar = string.char

_G.natives = {read=nativeRead, print=nativePrint, write=nativeWrite, string={char=nativeStringChar}}

dofile("./rom/core-apis/keys.lua")
dofile("./rom/core-apis/colors.lua")
dofile("./rom/core-apis/os.lua")
dofile("./rom/core-apis/term.lua")
dofile("./rom/core-apis/fs.lua")

os.loadAPI("./rom/apis/textutils.lua")
os.loadAPI("./rom/apis/paintutils.lua")
os.loadAPI("./rom/apis/parallel.lua")
os.loadAPI("./rom/apis/vector.lua")
os.loadAPI("./rom/apis/window.lua")

function out(str)
	str = tostring(str)
	if (str and #str > 0) then
		nativePrint(str)
	end
end

function write( sText )
    local w,h = term.getSize()        
    local x,y = term.getCursorPos()
    
    local nLinesPrinted = 0
    local function newLine()
        if y + 1 <= h then
            term.setCursorPos(1, y + 1)
        else
            term.setCursorPos(1, h)
            term.scroll(1)
        end
        x, y = term.getCursorPos()
        nLinesPrinted = nLinesPrinted + 1
    end
    
    
    while string.len(sText) > 0 do
        local whitespace = string.match( sText, "^[ \t]+" )
        if whitespace then
            
            term.write( whitespace, true )
            x,y = term.getCursorPos()
            sText = string.sub( sText, string.len(whitespace) + 1 )
        end
        
        local newline = string.match( sText, "^\n" )
        if newline then
            
            newLine()
            sText = string.sub( sText, 2 )
        end
        
        local text = string.match( sText, "^[^ \t\n]+" )
        if text then
            sText = string.sub( sText, string.len(text) + 1 )
            if string.len(text) > w then
                
                while string.len( text ) > 0 do
                    if x > w then
                        newLine()
                    end
                    term.write( text, true )
                    text = string.sub( text, (w-x) + 2 )
                    x,y = term.getCursorPos()
                end
            else
                
                if x + string.len(text) - 1 > w then
                    newLine()
                end
                term.write( text, true )
                x,y = term.getCursorPos()
            end
        end
    end
    term.redraw()
    return nLinesPrinted
end

function print( ... )
    local nLinesPrinted = 0
    for n,v in ipairs( { ... } ) do
        nLinesPrinted = nLinesPrinted + write( tostring( v ) )
    end
    nLinesPrinted = nLinesPrinted + write( "\n" )
    return nLinesPrinted
end

function printError( ... )
    if term.isColour() then
        term.setTextColour( colors.red )
    end
    print( ... )
    if term.isColour() then
        term.setTextColour( colors.white )
    end
end

function string.char(...)
	local data = {...}
	for k, v in pairs(data) do
		if type(v) == "number" and v > -1 and v < 256 then
			data[k]=nativeStringChar(v)
		else
			data[k]=nativeStringChar(0)
		end
	end
	return table.unpack(data)
end

function read( _sReplaceChar, _tHistory, _fnComplete )
    term.setCursorBlink( true )

    local sLine = ""
    local nHistoryPos
    local nPos = 0
    if _sReplaceChar then
        _sReplaceChar = string.sub( _sReplaceChar, 1, 1 )
    end

    local tCompletions
    local nCompletion
    local function recomplete()
        if _fnComplete and nPos == string.len(sLine) then
            tCompletions = _fnComplete( sLine )
            if tCompletions and #tCompletions > 0 then
                nCompletion = 1
            else
                nCompletion = nil
            end
        else
            tCompletions = nil
            nCompletion = nil
        end
    end

    local function uncomplete()
        tCompletions = nil
        nCompletion = nil
    end

    local w = term.getSize()
    local sx = term.getCursorPos()

    local function redraw( _bClear )
        local nScroll = 0
        if sx + nPos >= w then
            nScroll = (sx + nPos) - w
        end

        local cx,cy = term.getCursorPos()
        term.setCursorPos( sx, cy )
        local sReplace = (_bClear and " ") or _sReplaceChar
        if sReplace then
            term.write( string.rep( sReplace, math.max( string.len(sLine) - nScroll, 0 ) ) )
        else
            term.write( string.sub( sLine, nScroll + 1 ) )
        end

        if nCompletion then
            local sCompletion = tCompletions[ nCompletion ]
            local oldText, oldBg
            if not _bClear then
                oldText = term.getTextColor()
                oldBg = term.getBackgroundColor()
                term.setTextColor( colors.white )
                term.setBackgroundColor( colors.gray )
            end
            if sReplace then
                term.write( string.rep( sReplace, string.len( sCompletion ) ) )
            else
                term.write( sCompletion )
            end
            if not _bClear then
                term.setTextColor( oldText )
                term.setBackgroundColor( oldBg )
            end
        end

        term.setCursorPos( sx + nPos - nScroll, cy )
    end
    
    local function clear()
        redraw( true )
    end

    recomplete()
    redraw()

    local function acceptCompletion()
        if nCompletion then
            
            clear()

            
            local sCompletion = tCompletions[ nCompletion ]
            local sFirstLetter = string.sub( sCompletion, 1, 1 )
            local sCommonPrefix = sCompletion
            for n=1,#tCompletions do
                local sResult = tCompletions[n]
                if n ~= nCompletion and string.find( sResult, sFirstLetter, 1, true ) == 1 then
                    while #sCommonPrefix > 1 do
                        if string.find( sResult, sCommonPrefix, 1, true ) == 1 then
                            break
                        else
                            sCommonPrefix = string.sub( sCommonPrefix, 1, #sCommonPrefix - 1 )
                        end
                    end
                end
            end

            
            sLine = sLine .. sCommonPrefix
            nPos = string.len( sLine )
        end

        recomplete()
        redraw()
    end
    while true do
        local sEvent, param = os.pullEvent()
        if sEvent == "char" then
            
            clear()
            sLine = string.sub( sLine, 1, nPos ) .. param .. string.sub( sLine, nPos + 1 )
            nPos = nPos + 1
            recomplete()
            redraw()

        elseif sEvent == "paste" then
            
            clear()
            sLine = string.sub( sLine, 1, nPos ) .. param .. string.sub( sLine, nPos + 1 )
            nPos = nPos + string.len( param )
            recomplete()
            redraw()

        elseif sEvent == "key" then
            if param == keys.enter or param == keys.numenter then
                
                if nCompletion then
                    clear()
                    uncomplete()
                    redraw()
                end
                break
                
            elseif param == keys.left then
                
                if nPos > 0 then
                    clear()
                    nPos = nPos - 1
                    recomplete()
                    redraw()
                end
                
            elseif param == keys.right then
                
                if nPos < string.len(sLine) then
                    
                    clear()
                    nPos = nPos + 1
                    recomplete()
                    redraw()
                else
                    
                    acceptCompletion()
                end

            elseif param == keys.up or param == keys.down then
                
                if nCompletion then
                    
                    clear()
                    if param == keys.up then
                        nCompletion = nCompletion - 1
                        if nCompletion < 1 then
                            nCompletion = #tCompletions
                        end
                    elseif param == keys.down then
                        nCompletion = nCompletion + 1
                        if nCompletion > #tCompletions then
                            nCompletion = 1
                        end
                    end
                    redraw()

                elseif _tHistory then
                    
                    clear()
                    if param == keys.up then
                        
                        if nHistoryPos == nil then
                            if #_tHistory > 0 then
                                nHistoryPos = #_tHistory
                            end
                        elseif nHistoryPos > 1 then
                            nHistoryPos = nHistoryPos - 1
                        end
                    else
                        
                        if nHistoryPos == #_tHistory then
                            nHistoryPos = nil
                        elseif nHistoryPos ~= nil then
                            nHistoryPos = nHistoryPos + 1
                        end                        
                    end
                    if nHistoryPos then
                        sLine = _tHistory[nHistoryPos]
                        nPos = string.len( sLine ) 
                    else
                        sLine = ""
                        nPos = 0
                    end
                    uncomplete()
                    redraw()

                end

            elseif param == keys.backspace then
                
                if nPos > 0 then
                    clear()
                    sLine = string.sub( sLine, 1, nPos - 1 ) .. string.sub( sLine, nPos + 1 )
                    nPos = nPos - 1
                    recomplete()
                    redraw()
                end

            elseif param == keys.home then
                
                if nPos > 0 then
                    clear()
                    nPos = 0
                    recomplete()
                    redraw()
                end

            elseif param == keys.delete then
                
                if nPos < string.len(sLine) then
                    clear()
                    sLine = string.sub( sLine, 1, nPos ) .. string.sub( sLine, nPos + 2 )                
                    recomplete()
                    redraw()
                end

            elseif param == keys["end"] then
                
                if nPos < string.len(sLine ) then
                    clear()
                    nPos = string.len(sLine)
                    recomplete()
                    redraw()
                end

            elseif param == keys.tab then
                
                acceptCompletion()

            end

        elseif sEvent == "term_resize" then
            
            w = term.getSize()
            redraw()

        end
    end

    local cx, cy = term.getCursorPos()
    term.setCursorBlink( false )
    term.setCursorPos( w + 1, cy )
    print()
    
    return sLine
end

os.run({}, fs.combine("", "/rom/programs/shell"))