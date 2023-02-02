timer = Timer.new()

s = "0x"
s = s .. string.format("%x",timer)

print("hexAddr: ", s)

text = "TimerPos: " .. s

-- Main Loop
while System.mainLoop() do
    -- Updating screens
	Screen.waitVblankStart()
	Screen.refresh()
	
	-- Writing something on screen
	Screen.debugPrint(0,0,"Welcome to the Screen Module!",Color.new(255,255,255),TOP_SCREEN)
	Screen.debugPrint(0,30,text,Color.new(255,255,255),TOP_SCREEN)
	
	-- Flipping screen
	Screen.flip()

	if Controls.check(Controls.read(),KEY_A) then
        print("KEY_A Pressed")
    end

	if Controls.check(Controls.read(),KEY_A) then
        break
    end

end
System.exit()
print("loop break")