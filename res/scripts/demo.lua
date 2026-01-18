print("==== test.lua loaded ====")

local scene = game:GetActiveScene()

local go = scene:CreateGameObject("Cube")
go:AddComponent("MeshRenderer")

local mr = go:GetComponent("MeshRenderer")
mr:LoadMesh("./res/models/mdl_bridge.obj")

local tf = go:GetComponent("Transform")
tf:SetPosition(0,-5,0)
