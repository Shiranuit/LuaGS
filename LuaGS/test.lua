function clear(r, g, b, a)
	LuaGL.glClearColor(r, g, b, a)
	LuaGL.glClear(LuaGL.GL_DEPTH_BUFFER_BIT | LuaGL.GL_COLOR_BUFFER_BIT)
end

function rotate(x, y, z)
	LuaGL.glRotatef(x, 1.0, 0.0, 0.0)
	LuaGL.glRotatef(y, 0.0, 1.0, 0.0)
	LuaGL.glRotatef(z, 0.0, 0.0, 1.0)
end

--[[LuaGL.glEnable(LuaGL.GL_CULL_FACE)
LuaGL.glCullFace(LuaGL.GL_BACK)
LuaGL.glFrontFace(LuaGL.GL_CCW)
]]

start = os.timeNano()
time = os.timeNano()
val = 0.5
rot = 0.5
while true do
	os.pullEvent("skip_event")
	if os.timeNano() - time > 1000000000/60 then
		clear(0.0, 0.0, 0.0, 1.0)
		
		LuaGL.glPushMatrix()
		rotate(rot, rot, rot)
		LuaGL.glBegin(LuaGL.GL_TRIANGLES)
		LuaGL.glColor3f(1.0, 0.0, 0.0)
		LuaGL.glVertex3f(0.0, val, 0.0)
		LuaGL.glColor3f(0.0, 1.0, 0.0)
		LuaGL.glVertex3f(-val, 0.0, 0.0)
		LuaGL.glColor3f(0.0, 0.0, 1.0)
		LuaGL.glVertex3f(0.0, 0.0, val)
		LuaGL.glEnd()
		LuaGL.glPopMatrix()
		
		natives.term.setCursorPos(0, 0)
		natives.term.write((os.timeNano()-start) / 1000000000)
		term.refresh()
		rot = rot + 0.3
		time = os.timeNano()
	end
end