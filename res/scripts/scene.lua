print("--- scene.lua started ---")

local scene = game:GetActiveScene()

print("Scene exists: ", scene ~= nil)
print("Scene Type: ", type(scene))

-- Create terrain
function CreateMeshObject(name, modelPath)
    -- 1. Create the GameObject
    local go = scene:CreateGameObject(name)

    -- 2. Add a MeshRenderer component
    go:AddComponent("MeshRenderer")
    local renderer = go:GetComponent("MeshRenderer")

    -- 3. Load the mesh
    renderer:LoadMesh(modelPath)
    return go
end

local towerGO = CreateMeshObject("Skyscraper", "./res/models/mdl_skyscraper.obj")
local towerTF = towerGO:GetComponent("Transform")

local terrainGO = CreateMeshObject("Terrain", "./res/models/mdl_world.obj")
local terrainTF = terrainGO:GetComponent("Transform")

local ballGO = CreateMeshObject("Ball", "./res/models/mdl_ball.obj")
local ballTF = ballGO:GetComponent("Transform")

local isoGO = CreateMeshObject("Ball", "./res/models/mdl_isosphere.obj")
local isoTF = isoGO:GetComponent("Transform")

towerTF:SetPosition(0,0,0)
towerTF:SetScale(.1,.1,.1)

terrainTF:SetPosition(0,-10,-10)

ballTF:SetPosition(-.5, -7, 0)
ballTF:SetScale(0.1,0.1,0.1)
isoTF:SetPosition(.5, -7, 0)
isoTF:SetScale(0.1,0.1,0.1)

local rot = 0

function OnUpdate(deltaTime)
    rot = rot + (.5 * deltaTime)

    ballTF:SetRotation(0, rot, 0)
    isoTF:SetRotation(0, rot, 0)
   
end