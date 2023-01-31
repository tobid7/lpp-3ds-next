
timer = Timer.new()
print("Adress: ", timer)

print(Timer.getTime(timer))

rnn = true

print("Firmware: ", System.getFirmware())
System.launch3DSX("")
System.launchPayload("")

-- Main Loop
while rnn do
	if Controls.check(Controls.read(),KEY_A) then
        print("KEY_A Pressed")
        rnn = false
    end

end