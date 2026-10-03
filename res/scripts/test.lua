print("==== test.lua loaded ====")

local scene = game:GetActiveScene()

local go = scene:CreateGameObject("Cube")
go:AddComponent("MeshRenderer")

local mr = go:GetComponent("MeshRenderer")
mr:LoadMesh("./res/models/mdl_grass_cube.obj")

local tf = go:GetComponent("Transform")
tf:SetPosition(8, -5, -8)

local rot = 0
function OnUpdate(dt)
    rot = rot + (0.2*dt)
    tf:SetRotation(rot, 0, rot)
end
