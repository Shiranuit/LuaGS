
function main()
	function clear(r, g, b, a)
		LuaGL.glClearColor(r, g, b, a)
		LuaGL.glClear(LuaGL.GL_DEPTH_BUFFER_BIT | LuaGL.GL_COLOR_BUFFER_BIT)
	end

	function rotate(x, y, z)
		LuaGL.glRotatef(x, 1.0, 0.0, 0.0)
		LuaGL.glRotatef(y, 0.0, 1.0, 0.0)
		LuaGL.glRotatef(z, 0.0, 0.0, 1.0)
	end

	start = os.time()
	time = os.time()
	val = 0.5
	while true do
		if os.time() - time < 1 then
			clear(0.0, 0.0, 0.0, 1.0)
			LuaGL.glPushMatrix()
			LuaGL.glBegin(LuaGL.GL_TRIANGLES)
			LuaGL.glColor3f(1.0, 0.0, 0.0)
			LuaGL.glVertex3f(0.0, val, 0.0)
			LuaGL.glColor3f(0.0, 1.0, 0.0)
			LuaGL.glVertex3f(-val, 0.0, 0.0)
			LuaGL.glColor3f(0.0, 0.0, 1.0)
			LuaGL.glVertex3f(0.0, 0.0, val)
			LuaGL.glEnd()
			rotate(0.1, 0.0, 0.0)
			LuaGL.glPopMatrix()
			term.setTextColor(255,0,0)
			term.write(os.time()-start)
			term.refresh()
			time = os.time()
			val = val * 1.001
		end
	end
end


main()
--term.write("Salut")
--term.refresh()