#HEADER DEFINITION

-- // Other Functions \\ --
function TestFunction(Data : string)
	print("Received Message from C++! It reads: " .. Data)
end

-- // Engine Hooks \\ --
function OnUpdate(DeltaTime : number)
	print(DeltaTime)
end

function OnLoad()
	print("Test.lua LOADED!")
end

function OnUnload()
	print("Test.lua UNLOADED!")
end