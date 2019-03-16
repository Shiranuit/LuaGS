local nativePullEvent = os.pullEvent
local nativePathExec = os.pathExec
_G.natives.os = {pullEvent=nativePullEvent, pathExec=nativePathExec}

function os.pullEvent(filter)
	if (type(filter) == "string") then
		local event
		while (not event or (event and event[1] ~= filter)) do
			event = coroutine.yield()
		end
		return table.unpack(event)
	elseif (type(filter) == "boolean" and filter == true) then
		local event
		while (not event) do
			event = coroutine.yield()
		end
		return table.unpack(event)
	else
		local event = coroutine.yield()
		if (event) then
			return table.unpack(event)
		end
	end
end

_G.loadfile = function( _sFile, _tEnv )
    local file = fs.open( _sFile, "r")
    if file then
        local func, err = load( file.readAll(), fs.getName( _sFile ), "t", _tEnv )
        file.close()
        return func, err
    end
    return nil, "File not found"
end

local tAPIsLoading = {}
function os.loadAPI( _sPath )
    local sName = fs.getName(_sPath, true )
    if tAPIsLoading[sName] == true then
        printError( "API "..sName.." is already being loaded" )
        return false
    end
    tAPIsLoading[sName] = true

    local tEnv = {}
    setmetatable( tEnv, { __index = _G } )
    local fnAPI, err = loadfile( _sPath, tEnv )
    if fnAPI then
        local ok, err = pcall( fnAPI )
        if not ok then
            printError( err )
            tAPIsLoading[sName] = nil
            return false
        end
    else
        printError( err )
        tAPIsLoading[sName] = nil
        return false
    end
    
    local tAPI = {}
    for k,v in pairs( tEnv ) do
        if k ~= "_ENV" then
            tAPI[k] =  v
        end
    end

    _G[sName] = tAPI    
    tAPIsLoading[sName] = nil
    return true
end

function os.unloadAPI( _sName )
    if _sName ~= "_G" and type(_G[_sName]) == "table" then
        _G[_sName] = nil
    end
end

function os.run( _tEnv, _sPath, ... )
    local tArgs = { ... }
    local tEnv = _tEnv
    setmetatable( tEnv, { __index = _G } )
    local fnFile, err = loadfile( _sPath, tEnv )
    if fnFile then
        local ok, err = pcall( function()
            fnFile( table.unpack( tArgs ) )
        end )
        if not ok then
            if err and err ~= "" then
                printError( err )
            end
            return false
        end
        return true
    end
    if err and err ~= "" then
        printError( err )
    end
    return false
end

function os.version()
	return "LuaGS 1.0"
end


os.pullEventRaw = os.pullEvent