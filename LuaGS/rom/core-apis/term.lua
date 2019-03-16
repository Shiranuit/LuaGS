-- NATIVES VARIABLES
local nativeSetCursorPos = term.setCursorPos
local nativeGetCursorPos = term.getCursorPos
local nativeSetTextScale = term.setTextScale
local nativeGetSize = term.getSize
local nativeWrite = term.write
local nativeSetTextColor = term.setTextColor
local nativeSetBackgroundColor = term.setBackgroundColor
local nativeGetTextColor = term.getTextColor
local nativeGetBackgroundColor = term.getBackgroundColor

_G.natives.term={write=nativeWrite, pullEvent=nativePullEvent, setCursorPos=nativeSetCursorPos, getCursorPos=nativeGetCursorPos, getSize=nativeGetSize, setTextScale=nativeSetTextScale}

-- VARIABLES
local cursorX = 1
local cursorY = 1
local fg = "0"
local bg = "f"
local hex = "0123456789abcdef"
local screen = {}

local w, h = nativeGetSize()
local wsize = math.ceil(w / (6 * term.getTextScale()))+1

-- SCREEN TABLE INITIALISATION
for i=1, math.ceil(h / (9 * term.getTextScale()))+1 do
	screen[i] = {
	string.rep(" ",  math.ceil(wsize)),
	string.rep(fg,  math.ceil(wsize)),
	string.rep(bg,  math.ceil(wsize)),
	}
end

-- FUNCTION DECLARATION

local function clear()
	local col = colors[colors.black]
	LuaGL.glClearColor(col[1]/256.0, col[2]/256.0, col[3]/256.0, 1.0)
	LuaGL.glClear(LuaGL.GL_COLOR_BUFFER_BIT | LuaGL.GL_DEPTH_BUFFER_BIT)
end

function term.getSize()
	local scale = term.getTextScale()
	local w, h = nativeGetSize()
	
	return math.ceil(w / (6 * scale))-1, math.ceil(h / (9 * scale))-1
end

function term.current()
	return  _G.term
end

function term.resize()
	local wsize, hsize = term.getSize()
	for i=1, hsize+1 do
		if (screen[i]) then
			screen[i][1] = screen[i][1]:sub(1, wsize+1)
			screen[i][1] = screen[i][1]..string.rep(" ", wsize-#screen[i][1]+1)
			
			screen[i][2] = screen[i][2]:sub(1, wsize)
			screen[i][2] = screen[i][2]..string.rep(fg, wsize-#screen[i][2]+1)
			
			screen[i][3] = screen[i][3]:sub(1, wsize)
			screen[i][3] = screen[i][3]..string.rep(bg, wsize-#screen[i][3]+1)
		else
			screen[i] = {string.rep(" ", wsize+1), string.rep(fg, wsize+1), string.rep(bg, wsize+1)}
		end
	end
	term.redraw()
end

function term.setTextScale(scale)
	if type(scale) == "number" then
		nativeSetTextScale(scale)
		term.resize()
	end
end

function term.setCursorPos(x, y)
	if (type(x) == "number" and type(y) == "number") then
		cursorX = math.floor(x)
		cursorY = math.floor(y)
	end
end

function term.blit(str, strfg, strbg, dontUpdate)
	if str and strfg and strbg then
		str = tostring(str)
		strfg = tostring(strfg)
		strbg = tostring(strbg)
		if #str == #strfg and #strfg == #strbg then
			screen[cursorY][1] = screen[cursorY][1]:sub(1, cursorX-1)..str..screen[cursorY][1]:sub(cursorX+#str, #screen[cursorY][1])
			screen[cursorY][2] = screen[cursorY][2]:sub(1, cursorX-1)..strfg..screen[cursorY][2]:sub(cursorX+#strfg, #screen[cursorY][2])
			screen[cursorY][3] = screen[cursorY][3]:sub(1, cursorX-1)..strbg..screen[cursorY][3]:sub(cursorX+#strbg, #screen[cursorY][3])
			cursorX = cursorX + #str
			if not dontUpdate then
				term.redraw()
			end
		else
			error("Arguments #1 #2 and #3 need to be of the same size")
		end
	else
		error("Expected String, String, String")
	end
end

function _G.term.getCursorPos()
	return cursorX, cursorY
end

function term.scroll(dir)
	if (type(dir) == "number") then
		if (dir > 0) then
			for i=1, dir do
				table.remove(screen, 1)
			end
			local wsize, hsize = term.getSize()
			for i=1, dir do
				screen[#screen + 1] = {string.rep(" ", wsize+1), string.rep(fg, wsize+1), string.rep(bg, wsize+1)}
			end
		elseif (dir < 0) then
			for i=1, -dir do
				screen[#screen] = nil
			end
			local wsize, hsize = term.getSize()
			for i=1, -dir do
				table.insert(screen, 1, {string.rep(" ", wsize+1), string.rep(fg, wsize+1), string.rep(bg, wsize+1)})
			end
		end
	end
end

function term.redraw()
	clear()
	local ff, bb = fg, bg
	local w, h = term.getSize()
	for i=1, h+2 do
		for j=1, w+2 do
			nativeSetCursorPos((j - 1) * 6, (i - 1) * 9)
			term.setTextColor(screen[i][2]:sub(j,j))
			term.setBackgroundColor(screen[i][3]:sub(j,j))
			nativeWrite(screen[i][1]:sub(j,j))
		end
	end
	fg = ff
	bg = bb
	term.refresh()
end

function term.write(txt, dontUpdate)
	txt = tostring(txt)
	if (txt and #txt > 0) then
		local scale = term.getTextScale()
		local w, h = nativeGetSize()
		term.blit(txt, string.rep(fg, #txt), string.rep(bg, #txt), true)
	end
	if (screen and not dontUpdate) then
		term.redraw()
	end
end

function term.clear()
	local w, h = term.getSize()
	for i=1, h+2 do
		screen[i][1] = string.rep(" ",  w+1)
		screen[i][2] = string.rep(fg,  w+1)
		screen[i][3] = string.rep(bg,  w+1)
	end
	term.refresh()
end

function term.clearLine()
	local w, h = term.getSize()
	if (cursorY > 0 and cursorY < h+2) then
		screen[cursorY][1] = screen[cursorY][1]:sub(1, cursorX-1)..string.rep(" ", w-cursorX+1)
	end
end

function term.setTextColor(col)
	if (type(col) == "string" and #col > 0) then
		local val = tonumber(col:sub(1,1), 16)
		if (val and colors[2^val]) then
			fg = col:sub(1, 1)
			nativeSetTextColor(table.unpack(colors[2^val]))
		end
	elseif (type(col) == "number") then
		if (colors[col]) then
			local index = math.log(col) / math.log(2)
			fg = hex:sub(index + 1, index + 1)
			nativeSetTextColor(table.unpack(colors[col]))
		end
	end
end

function term.setBackgroundColor(col)
	if (type(col) == "string" and #col > 0) then
		local val = tonumber(col:sub(1,1), 16)
		if (val and colors[2^val]) then
			bg = col:sub(1,1)
			nativeSetBackgroundColor(table.unpack(colors[2^val]))
		end
	elseif (type(col) == "number") then
		if (colors[col]) then
			local index = math.log(col) / math.log(2)
			bg = hex:sub(index + 1, index + 1)
			nativeSetBackgroundColor(table.unpack(colors[col]))
		end
	end
end

function term.getBackgroundColor()
	return 2^tonumber(bg, 16)
end

function term.getTColor()
	return 2^tonumber(fg, 16)
end

function term.isColor()
	return true
end

function term.setCursorBlink()

end

function term.redirect()
	return _G.term
end

_G.term.isColour = term.isColor
_G.term.setTextColour = term.setTextColor
_G.term.setBackgroundColour = term.setBackgroundColor
_G.term.getTextColour = term.getTextColor
_G.term.getBackgroundColour = term.getBackgroundColor