print("--- Lua Example Start ---")
print("Scene exists: ", scene ~= nil)
print("Scene Type: ", type(scene))

<<<<<<< HEAD
-- ---- Cube 1 ----
local cube1 = scene:CreateGameObject("Cube1")
local mr1 = cube1:AddComponent("MeshRenderer")
local renderer1 = cube1:GetComponent("MeshRenderer")
=======
local go = scene:CreateGameObject("Cube")
go:AddComponent("MeshRenderer")
>>>>>>> 9684c19b644eabdf533a8708f61d4acdddb53a88

renderer1:LoadMesh("res/models/mdl_grass_cube.obj")
renderer1:LoadShader("Default", "res/shaders/vertexShaders/defaultShader.vert",
                     "res/shaders/fragShaders/defaultShader.frag")

local tf1 = cube1:GetComponent("Transform")
tf1:SetPosition(-3, 0, -10)

-- ---- Cube 2 ----
local cube2 = scene:CreateGameObject("Cube2")
local mr2 = cube2:AddComponent("MeshRenderer")
local renderer2 = cube2:GetComponent("MeshRenderer")

renderer2:LoadMesh("res/models/mdl_grass_cube.obj")
renderer2:LoadShader("Default2", "res/shaders/vertexShaders/defaultShader.vert",
                     "res/shaders/fragShaders/defaultShader.frag")

local tf2 = cube2:GetComponent("Transform")
tf2:SetPosition(3, 0, -10)

-- ---- Rotation ----
local rotation = 0

function OnUpdate(deltaTime)
    rotation = rotation + (2 * deltaTime)
    tf1:SetRotation(rotation, 0, rotation)
    tf2:SetRotation(rotation, 0, rotation)
end
