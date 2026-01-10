print("--- Lua Example Start ---")
print("Scene exists: ", scene ~= nil)
print("Scene Type: ", type(scene))

-- TODO second cube missing shader for some reason

local cubeObj = scene:CreateGameObject("Cube")
local cubeTwoObj = scene:CreateGameObject("Second cube")

print("Cube 1 created:", cubeObj ~= nil)
print("Cube 2 created:", cubeTwoObj ~= nil)

cubeObj:AddComponent("MeshRenderer")
cubeTwoObj:AddComponent("MeshRenderer")

local cubeMR = cubeObj:GetComponent("MeshRenderer")
local cubeTwoMR = cubeTwoObj:GetComponent("MeshRenderer")

print("Cube 1 MeshRenderer:", cubeMR ~= nil)
print("Cube 2 MeshRenderer:", cubeTwoMR ~= nil)
print("Are they the same object?:", cubeMR == cubeTwoMR)

cubeMR:LoadMesh("res/models/mdl_grass_cube.obj")
print("Cube 1 mesh loaded")
cubeMR:LoadShader("res/shaders/vertexShaders/defaultShader.vert", "res/shaders/fragShaders/defaultShader.frag")
print("Cube 1 shader loaded")

cubeTwoMR:LoadMesh("res/models/mdl_grass_cube.obj")
print("Cube 2 mesh loaded")
cubeTwoMR:LoadShader("res/shaders/vertexShaders/defaultShader.vert", "res/shaders/fragShaders/defaultShader.frag")
print("Cube 2 shader loaded")

local cubeTF = cubeObj:GetComponent("Transform")
cubeTF:SetPosition(-3, 0, -10)

local cubeTwoTF = cubeTwoObj:GetComponent("Transform")
cubeTwoTF:SetPosition(3, 0, -10)

local rotation = 0

function OnUpdate(deltaTime)
    rotation = rotation + (2 * deltaTime)
    cubeTF:SetRotation(rotation, 0, rotation)
    -- cubeTwoTF:SetRotation(rotation, 0, rotation)
end