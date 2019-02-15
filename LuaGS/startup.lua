local nativePrint = print
local nativeWrite = term.write
local nativeRead = read
local nativePullEvent = os.pullEvent
local nativeStringChar = string.char
local nativeSetCursorPos = term.setCursorPos
local nativeGetCursorPos = term.getCursorPos
local nativeSetTextScale = term.setTextScale
local nativeGetSize = term.getSize
_G.natives = {term={getSize = nativeGetSize, setTextScale=nativeSetTextScale, setCursorPos=nativeSetCursorPos, getCursorPos=nativeGetCursorPos, write=nativeWrite}, os={pullEvent=nativePullEvent},read=nativeRead, print=nativePrint}
local cursorX = 1
local cursorY = 1

local screen = {}
local w, h = nativeGetSize()
local wsize = w / (6 * term.getTextScale())
for i=1, h / (9 * term.getTextScale()) do
	screen[i] = string.rep(" ",  math.floor(wsize))
end

function _G.out(str)
	str = tostring(str)
	if (str and #str > 0) then
		nativePrint(str)
	end
end

function _G.term.getSize()
	local scale = term.getTextScale()
	return math.floor(w / (6 * scale)), math.floor(h / (9 * scale))
end

function _G.term.resize()
	wsize = w / (6 * term.getTextScale())
	for i=1, h / (9 * term.getTextScale()) do
		screen[i] = screen[i]:sub(1, wsize)
		screen[i] = screen[i]..string.rep(" ", wsize-#screen[i])
	end	
end

function _G.term.setTextScale(scale)
	if type(scale) == "number" then
		nativeSetTextScale(scale)
		term.resize()
	end
end

function _G.term.setCursorPos(x, y)
	if (type(x) == "number" and type(y) == "number") then
		cursorX = math.floor(x)
		cursorY = math.floor(y)
	end
end


function _G.term.getCursorPos()
	return cursorX, cursorY
end


Keys = {
	["ENTER"] = 40,
	["BACKSPACE"] = 42,
	["SUPPR"] = 76,
	["UP"] = 82,
	["RIGHT"] = 79,
	["DOWN"] = 81,
	["LEFT"] = 80,
	["NUM0"] = 98,
	["NUM1"] = 89,
	["NUM2"] = 90,
	["NUM3"] = 91,
	["NUM4"] = 92,
	["NUM5"] = 93,
	["NUM6"] = 94,
	["NUM7"] = 95,
	["NUM8"] = 96,
	["NUM9"] = 97,
	["PLUS"] = 87,
	["MINUS"] = 86,
	["MUL"] = 85,
	["DIV"] = 84,
	["NUMENTER"] = 88,
	["NUMDOT"] = 89,
	["VERNUM"] = 83,
	["INSERT"] = 73,
	["HOME"] = 74,
	["PAGEUP"] = 75,
	["END"] = 77,
	["PAGEDOWN"] = 78,
	["F1"] = 58,
	["F2"] = 59,
	["F3"] = 60,
	["F4"] = 61,
	["F5"] = 62,
	["F6"] = 63,
	["F7"] = 64,
	["F8"] = 65,
	["F9"] = 66,
	["F10"] = 67,
	["F11"] = 68,
	["F12"] = 69,
	["ESCAPE"] = 41,
	["LCTRL"] = 224,
	["SHIFT"] = 225,
	["ALT"] = 226,
	["WIN"] = 227,
	["VERMAJ"] = 57,
	["SPACE"] = 44,
	["ALTGR"] = 230,
	["RSHIFT"] = 229,
	["FILE"] = 101,
	["RCTRL"] = 228,
	["PRINTSCR"] = 70,
	["BREAK"] = 71,
	["PAUSE"] = 72,
	["TAB"] = 43,	
}

local function clear()
	LuaGL.glClearColor(0.0, 0.0, 0.0, 1.0)
	LuaGL.glClear(LuaGL.GL_COLOR_BUFFER_BIT)
end

function term.scroll(dir)
	if (type(dir) == "number") then
		if (dir < 0) then
			for i=1, -dir do
				table.remove(screen, 1)
			end
			local wsize = math.floor(w / (6 * term.getTextScale()))
			for i=1, -dir do
				screen[#screen + 1] = string.rep(" ", wsize)
			end
		elseif (dir > 0) then
			for i=1, -dir do
				screen[#screen] = nil
			end
			local wsize = math.floor(w / (6 * term.getTextScale()))
			for i=1, -dir do
				table.insert(screen, 1, string.rep(" ", wsize))
			end
		end
	end
end

function _G.write(txt)
	txt = tostring(txt)
	if (txt and #txt > 0) then
		local scale = term.getTextScale()
		local w, h = nativeGetSize()
		for i=1, #txt do
			if cursorX > 0 and cursorX < w / (scale * 6)  and cursorY > 0 and cursorY < h / (scale * 9) then
				screen[cursorY] = screen[cursorY]:sub(1, cursorX-1)..txt:sub(i,i)..screen[cursorY]:sub(cursorX+1, #screen[cursorY])
				cursorX = cursorX + 1
				if (cursorX > w / (scale * 6)) then
					cursorX = 1
					cursorY = cursorY + 1
				end
			end
		end
	end
	clear()
	for i=1, #screen do
		nativeSetCursorPos(0, (i - 1) * 9)
		nativeWrite(screen[i])
	end
	term.refresh()
end

_G.term.write = write

function _G.print(txt)
	txt = tostring(txt)
	if txt and #txt > 0 then
		for line in txt:gmatch("[^\n]+") do
			term.write(line)
			local x, y = term.getCursorPos()
			term.setCursorPos(1, y + 1)
		end
	else
		local x, y = term.getCursorPos()
		term.setCursorPos(1, y + 1)
	end
	term.refresh()
end

function _G.os.pullEvent(filter)
	if (filter) then
		local event = nativePullEvent()
		if (filter == "skip_event") then
			return nil
		end
		while (not event or (event and event[1] ~= filter)) do
			event = nativePullEvent()
		end
		return table.unpack(event)
	else
		local event = nativePullEvent()
		while (not event) do
			event = nativePullEvent()
		end
		return table.unpack(event)
	end
end

function _G.string.char(...)
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

function _G.term.clear()
	for i=1, h / (9 * term.getTextScale()) do
		screen[i] = string.rep(" ",  math.floor(wsize))
	end
	term.refresh()
end

function _G.term.clearLine()
	local w, h = nativeGetSize()
	if (cursorY > 0 and cursorY < h / (9 * term.getTextScale())) then
		screen[cursorY] = string.rep(" ", math.floor(wsize))
	end
end

function _G.read(str)
	local enter = false
	local txt = ""
	local pos = 0
	local startX, startY = term.getCursorPos()
	if (str and #tostring(str) > 0) then
		term.write(tostring(str))
	end
	
	function update()
		term.setCursorPos(startX, startY)
		term.clearLine()
		if (str and #tostring(str) > 0) then
			term.write(tostring(str))
		end
		term.write(txt)
	end
	
	while not enter do
		local event, key, char = os.pullEvent("key")
		if key == Keys.ENTER or key == Keys.NUMENTER then
			local x, y = term.getCursorPos()
			if (y + 1 > ({term.getSize()})[2]) then
				term.scroll(-1)
				term.setCursorPos(1, y)
			else
				term.setCursorPos(1, y + 1)
			end
			return txt
		elseif key == Keys.BACKSPACE then
			if #txt > 0 then
				txt = txt:sub(0, pos - 1)..txt:sub(pos+1, #txt)
				pos = pos - 1
				update()
			end
		elseif char > -1 and char < 256 then
			txt = txt .. string.char(char)
			pos = pos + 1
			update()
		end
	end
end

while true do
	local input = read("> ")
	if #input > 0 then
		local code = ""
		local h = io.open(input) or io.open(input..".lua")
		if (h) then
			for line in h:lines() do
				code = code..line.."\n"
			end
			io.close(h)
			local func = load(code, input, "t", _G)
			local success, output = pcall(func)
			if (not success) then
				print("Error: "..tostring(msg))
			end
		else
			print("File not found '"..input.."'")
		end
	end
end