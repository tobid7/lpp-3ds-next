-- Init resources
if System.currentDirectory() == "/" then
	System.currentDirectory("")
end
ttf = Font.load("romfs:/sudoku/main.ttf")
Font.setPixelSizes(ttf,18)
white = Color.new(255,255,255)
red = Color.new(255,0,0)
black = Color.new(0,0,0)
yellow = Color.new(255,255,128)
green = Color.new(128,255,128)
cyan = Color.new(128,255,255)
update_screen = true
top_blend = true
won = false

function OneshotPrint(my_func)
	my_func()
	Screen.flip()
	Screen.refresh()
	my_func()
end

function GetMaxCoord(l)
	if l <= 3 then
		return 3
	elseif l <= 6 then
		return 6
	else
		return 9
	end
end

function CheckLine(n,l,m)
	for i=1,9 do
		if m[l][i] == n then
			return false
		end
	end
	return true
end

function CheckColumn(n,l,m)
	for i=1,9 do
		if m[i][l] == n then
			return false
		end
	end
	return true
end

function CheckSquare(n,l,c,m)
	my_nums = {}
	for i=(l-2),l do
		for j=(c-2),c do
			table.insert(my_nums,m[i][j])
		end
	end
	for i=1,9 do
		if my_nums[i] == n then
			return false
		end
	end
	return true
end

function AvailableNumbers(l,c,m)
	square_l = GetMaxCoord(l)
	square_c = GetMaxCoord(c)
	res = {0,{}}
	for i=1,9 do
		if CheckLine(i,l,m) then
			if CheckColumn(i,c,m) then
				if CheckSquare(i,square_l,square_c,m) then
					table.insert(res[2],i)
					res[1] = res[1] + 1
				end
			end
		end
	end
	return res
end

function PartialGenerateSudoku(m,z)
	for i=1,9 do
		m[z][i] = 0
		m[z+1][i] = 0
	end
	for l=z,9 do
		for c=1,9 do
			available = AvailableNumbers(l,c,matrix)
			if available[1] == 0 then
				return PartialGenerateSudoku(matrix,l-1)
			else
				matrix[l][c] = available[2][math.random(1,available[1])]
			end
		end
	end
	return matrix
end

function DefaultNumbers(d,m,s)
	if d == "Easy" then
		num = math.random(32,36)
	elseif d == "Normal" then
		num = math.random(20,25)
	elseif d == "Hard" then
		num = math.random(15,20)
	end
	z = 0
	while z < num do
		i = math.random(1,9)
		j = math.random(1,9)
		if m[i][j][1] ~= s[i][j] then
			m[i][j][1] = s[i][j]
			m[i][j][2] = true
			z = z + 1
		end
	end
	return num
end

function AddNumber(n,l,r,m)
	if not m[l][r][2] then
		if m[l][r][i] == 0 then
			inserted = inserted + 1
		end
		m[l][r][1] = n
	end
	if inserted >= 81 then
		won = true
		for i=1,9 do
			if not won then
				break
			end
			for j=1,9 do
				if m[i][j][1] ~= my_solution[i][j] then
					won = false
					break
				end
			end
		end
	end
end

function RemoveNumber(l,r,m)
	if not m[l][r][2] then
		if m[l][r][i] ~= 0 then
			inserted = inserted - 1
		end
		m[l][r][1] = 0
	end
end

function GenerateSudoku()
	matrix = {}
	for i=1,9 do
		matrix[i] = {0,0,0,0,0,0,0,0,0}
	end
	h,m,s = System.getTime()
	math.randomseed(h*3600+m*60+s)
	for l=1,9 do
		for c=1,9 do
			available = AvailableNumbers(l,c,matrix)
			if available[1] == 0 then
				return PartialGenerateSudoku(matrix,l-1)
			else
				matrix[l][c] = available[2][math.random(1,available[1])]
			end
		end
	end
	return matrix
end

function TopScreen()
	Screen.fillRect(0,399,0,239,white,TOP_SCREEN)
	Font.print(ttf,75,5,"3DSudoku v.1.2 by Rinnegatamante",red,TOP_SCREEN)
	Font.print(ttf,5,20,"Generated in "..(Timer.getTime(time_spent)/1000).." secs",black,TOP_SCREEN)
	Font.print(ttf,5,35,"Difficulty set to "..difficulty,black,TOP_SCREEN)
	Font.print(ttf,5,70,"Use touch to play",black,TOP_SCREEN)
	Font.print(ttf,5,85,"Press X to clear selected box",black,TOP_SCREEN)
	Font.print(ttf,5,100,"Press R to clear current sudoku",black,TOP_SCREEN)
	Font.print(ttf,5,115,"Press L to generate a new sudoku",black,TOP_SCREEN)
	Font.print(ttf,5,130,"Press START to return main menu",black,TOP_SCREEN)
end

function BottomScreen()
	Screen.fillRect(0,319,0,239,white,BOTTOM_SCREEN)
	for i=0,8 do
		Screen.fillEmptyRect(8+i*33,41+i*33,200,225,black,BOTTOM_SCREEN)
	end
	Font.setPixelSizes(ttf,24)
	if won then
		Font.print(ttf,60,150,"Sudoku completed succesfully!",red,TOP_SCREEN)
	end
	Font.print(ttf,20,200,"1      2      3      4      5      6      7      8      9",black,BOTTOM_SCREEN)
	for z1=1,3 do
		for z2=1,3 do
			if (z1+z2) % 2 == 0 then
				color = yellow
			else
				color = green
			end
			Screen.fillRect(3+z1*60,3+z1*60+60,3+(z2-1)*60+5,3+z2*60+5,color,BOTTOM_SCREEN)
			Screen.fillEmptyRect(3+z1*60,3+z1*60+20,3+(z2-1)*60+5,3+z2*60-35,black,BOTTOM_SCREEN)
			Screen.fillEmptyRect(3+z1*60,3+z1*60+20,3+(z2-1)*60+25,3+z2*60-15,black,BOTTOM_SCREEN)
			Screen.fillEmptyRect(3+z1*60,3+z1*60+20,3+(z2-1)*60+45,3+z2*60+5,black,BOTTOM_SCREEN)
			Screen.fillEmptyRect(3+z1*60+20,3+z1*60+40,3+(z2-1)*60+5,3+z2*60-35,black,BOTTOM_SCREEN)
			Screen.fillEmptyRect(3+z1*60+20,3+z1*60+40,3+(z2-1)*60+25,3+z2*60-15,black,BOTTOM_SCREEN)
			Screen.fillEmptyRect(3+z1*60+20,3+z1*60+40,3+(z2-1)*60+45,3+z2*60+5,black,BOTTOM_SCREEN)
			Screen.fillEmptyRect(3+z1*60+40,3+z1*60+60,3+(z2-1)*60+5,3+z2*60-35,black,BOTTOM_SCREEN)
			Screen.fillEmptyRect(3+z1*60+40,3+z1*60+60,3+(z2-1)*60+25,3+z2*60-15,black,BOTTOM_SCREEN)
			Screen.fillEmptyRect(3+z1*60+40,3+z1*60+60,3+(z2-1)*60+45,3+z2*60+5,black,BOTTOM_SCREEN)
		end
	end
	Screen.fillRect(44+cursor_l*20,62+cursor_l*20,9+(cursor_r-1)*20,27+(cursor_r-1)*20,cyan,BOTTOM_SCREEN)
	Font.setPixelSizes(ttf,18)
	for i=1,9 do
		for j=1,9 do
			if my_matrix[i][j][1] ~= 0 then
				if my_matrix[i][j][2] then
					color = red
				else
					color = black
				end
				Font.print(ttf,70+(i-1)*20,10+(j-1)*20,my_matrix[i][j][1],color,BOTTOM_SCREEN)
			end
		end
	end  
end

function CreateSudoku()
	-- Start sudoku generation
	Screen.refresh()
	Screen.fillRect(0,399,0,239,white,TOP_SCREEN)
	Screen.fillRect(0,319,0,239,white,BOTTOM_SCREEN)
	Font.print(ttf,5,5,"Generating Sudoku...",black,TOP_SCREEN)
	Screen.flip()
	Screen.waitVblankStart()
	time_spent = Timer.new()
	my_solution = GenerateSudoku()
	Timer.pause(time_spent)

	-- Set default cursor position
	cursor_l = 1
	cursor_r = 1
	
	my_matrix = {}
	for i=1,9 do
		my_matrix[i] = {{0,false},{0,false},{0,false},{0,false},{0,false},{0,false},{0,false},{0,false},{0,false}}
	end
	inserted = DefaultNumbers(difficulty,my_matrix,my_solution)
	update_screen = true
	top_blend = true
	won = false
end

-- Set default config
difficulty = "Easy"
mode = "Menu"
index = 1
oldpad = Controls.read()
menu = {"Start new sudoku","Difficulty: "..difficulty,"Exit game"}
difficulties = {"Easy","Normal","Hard"}
diff_idx = 1

while System.mainLoop() do
	Screen.refresh()
	pad = Controls.read()
	if mode == "Menu" then
		Screen.fillRect(0,399,0,239,white,TOP_SCREEN)
		Screen.fillRect(0,319,0,239,white,BOTTOM_SCREEN)
		Font.print(ttf,5,5,"3DSudoku v.1.2 by Rinnegatamante",red,TOP_SCREEN)
		for i,voice in pairs(menu) do
			if i == index then
				color = red
			else
				color = black
			end
			Font.print(ttf,10,20*i,voice,color,TOP_SCREEN)
		end
		if Controls.check(pad,KEY_DUP) and not Controls.check(oldpad,KEY_DUP) then
			index = index - 1
		elseif Controls.check(pad,KEY_DDOWN) and not Controls.check(oldpad,KEY_DDOWN) then
			index = index + 1
		elseif Controls.check(pad,KEY_A) and not Controls.check(oldpad,KEY_A) then
			if index == 1 then
				mode = "Game"
				CreateSudoku()
			elseif index == 2 then
				diff_idx = diff_idx + 1
				if diff_idx > #difficulties then
					diff_idx = 1
				end
				difficulty = difficulties[diff_idx]
				menu[2] = "Difficulty: "..difficulty
			elseif index == 3 then
				Font.unload(ttf)
				System.exit()
			end
		end
		if index > #menu then
			index = 1
		elseif index < 1 then
			index  =  #menu
		end
	elseif mode == "Game" then
		if top_blend then
			OneshotPrint(TopScreen)
			top_blend = false
		end
		if update_screen then
			OneshotPrint(BottomScreen)
			update_screen = false
		end
		if Controls.check(pad, KEY_START) then
			mode = "Menu"
			index = 1
		elseif Controls.check(pad,KEY_X) and not Controls.check(oldpad,KEY_X) then
			RemoveNumber(cursor_l,cursor_r,my_matrix)
			update_screen = true
		elseif Controls.check(Controls.read(),KEY_TOUCH) and not won then
			x,y = Controls.readTouch()
			if y < 188 and x < 243 and x > 63 and y > 8 then
				cursor_l = math.ceil((x - 63) / 20)
				cursor_r = math.ceil((y - 8) / 20)
				update_screen = true
			end
			for i=0,8 do
			Screen.fillEmptyRect(8+i*33,41+i*33,200,225,black,BOTTOM_SCREEN)
		end
			if y > 200 and y < 225 and x > 8 and x < 305 then
				AddNumber(math.ceil((x-8) / 33),cursor_l,cursor_r,my_matrix)
				update_screen = true
			end
		elseif Controls.check(pad,KEY_R) then
			for i=1,9 do
				for j=1,9 do
					if my_matrix[i][j][1] ~= 0 then
						if not my_matrix[i][j][2] then
							inserted = inserted - 1
							my_matrix[i][j][1] = 0
						end
					end
				end
			end
			update_screen = true
		elseif Controls.check(Controls.read(),KEY_L) then
			CreateSudoku()
		end
	end
	oldpad = pad
	Screen.flip()
	Screen.waitVblankStart()
end