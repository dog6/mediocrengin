print("=== example.lua started ===")
print("Scene exists: ", scene ~= nil)
print("Scene Type: ", type(scene))

local go = scene:CreateGameObject("Cube")
local meshRenderer = go:AddComponent("MeshRenderer")

local renderer = go:GetComponent("MeshRenderer")

renderer:LoadMesh("res/models/mdl_grass_cube.obj")
renderer:LoadShader("DefaultShader", "res/shaders/vertexShaders/devShader.vert", "res/shaders/fragShaders/devShader.frag")


local transform = go:GetComponent("Transform")
transform:SetPosition(0, 0, -10)

local rotation = 0

function OnUpdate(deltaTime)
    rotation = rotation + (2 * deltaTime)
    transform:SetRotation(rotation, 0, rotation)  -- Fixed: removed 'transform' parameter
end