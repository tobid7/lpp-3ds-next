local api = {}

-- Note: dofile(path) is the same as loadfile(path)()
loadfile(minetest.get_modpath("mymod") .. "/lppn/ScreenTest.lua")(api)

timer = Timer.new()

s = "0x"
s = s .. string.format("%x",timer)

print("hexAddr: ", s)

-- Main Loop
while System.mainLoop() do
    -- Updating screens
	Screen.waitVblankStart()
	Screen.refresh()
	
	-- Writing something on screen
	Screen.debugPrint(0,0,"Welcome to the Screen Module!",Color.new(255,255,255),TOP_SCREEN)
	
	-- Flipping screen
	Screen.flip()

	if Controls.check(Controls.read(),KEY_A) then
        print("KEY_A Pressed")
    end

end