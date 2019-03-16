_G.fs = _G.fs or {}

local nativeGetPath = fs.getPath

_G.natives.fs = {getPath = nativeGetPath}

function fs.getName( name, noExt)
	local fname = name:match("/?([^/]+)$")
	if (noExt) then
		return fname:match("[^.]+")
	end
	return fname
end

function fs.open( name, mode )
	local h = io.open(name, mode)
	if (h) then
		local file = {}
		function file.close()
			h:close()
		end
		
		function file.read()
			return h:read(1)
		end
		
		function file.readAll()
			return h:read("*a")
		end
		
		function file.readLine()
			return h:read("*l")
		end
		return file
	end
end

function fs.combine(path, dir)
	
	local function cleanup(path)
		local chunks = {}
		for chunk in path:gmatch("[^/\\]+") do
			chunks[#chunks + 1] = tostring(chunk)
		end
		if #chunks > 0 then
			return table.concat(chunks, "\\")
		else
			return ""
		end
	end

	if path and #tostring(path) > 0 and dir then
		path = tostring(path)
		dir = tostring(dir)
		return fs.getPath(cleanup(path.."\\"..dir))
	elseif path and dir then
		return fs.getPath(cleanup(".\\"..dir))
	end
	error("Expected String, String")
end

