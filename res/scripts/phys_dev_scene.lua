-- new example scene

-- worth noting that all new gameObjects come with a Transform attached
-- gameObjects are only rendered if they have a MeshRenderer component attached

print("--- phys_dev_scene.lua started ---")

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

local terrainGO = CreateMeshObject("Terrain", "D:/Projects/CPP/smallengine/res/models/mdl_plane.obj")
local terrainTF = terrainGO:GetComponent("Transform")

-- Create ball with physics
local ballGO = CreateMeshObject("Ball", "D:/Projects/CPP/smallengine/res/models/mdl_ball.obj")
local ballTF = ballGO:GetComponent("Transform")

ballGO:AddComponent("PhysicsBody")
local ballPB = ballGO:GetComponent("PhysicsBody")

-- Set initial ball velocity
-- ballPB:SetLinearVelocity(0,-0.1, 0);

local ballCO = ballGO:AddComponent("SphereCollider")

terrainTF:SetPosition(0,0,0)

ballTF:SetPosition(0, 3, 0)
ballTF:SetScale(0.1,0.1,0.1)



local rot = 0

function OnUpdate(deltaTime)
    
    rot = rot + (.5 * deltaTime)

    -- ballTF:SetRotation(0, rot, 0) -- kinematic rotation example
   ballPB:SetAngularVelocity(0, rot, 0) -- physics based rotation example

end