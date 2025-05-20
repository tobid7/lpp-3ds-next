function MenuPrint()
	i = 0
	while i < 9 do
		Screen.debugPrint(0, i*16, "Test: "..i, Color.new(255, 255, 255), TOP_SCREEN)
		i = i + 1
	end
end

-- Main Loop
while System.mainLoop() do

	-- Updating screens
	Screen.waitVblankStart()
	Screen.refresh()
	
	MenuPrint()
	
	-- Flipping screen
	Screen.flip()

	if Controls.check(Controls.read(), KEY_START) then
		Graphics.term()
		System.exit()
	end
end