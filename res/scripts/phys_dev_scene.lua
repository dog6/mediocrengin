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


-- Create terrain
local terrainGO = CreateMeshObject(
    "Terrain",
    "D:/Projects/CPP/smallengine/res/models/dev/plane/mdl_plane.obj"
)

-- Get terrain transform
local terrainTF = terrainGO:GetComponent("Transform")
terrainTF:SetPosition(0, 0, 0)

-- Create terrain collider
terrainGO:AddComponent("Collider")
local terrainCO = terrainGO:GetComponent("Collider")
terrainCO:SetBox(1.1,.05,1.1)
terrainCO:SetGizmoVisible(true)


-- Create terrain
local terrainGOA = CreateMeshObject(
    "Terrain",
    "D:/Projects/CPP/smallengine/res/models/dev/plane/mdl_plane.obj"
)

-- Get terrain transform
local terrainTFA = terrainGO:GetComponent("Transform")
terrainTFA:SetPosition(0, 3, 0)

-- Create terrain collider
terrainGOA:AddComponent("Collider")
local terrainCOA = terrainGOA:GetComponent("Collider")
terrainCOA:SetBox(1.1,.05,1.1)
terrainCOA:SetGizmoVisible(true)

-- Create ball with physics
local ballGO = CreateMeshObject(
    "Ball",
    "D:/Projects/CPP/smallengine/res/models/dev/sphere/mdl_sphere.obj"
)

local ballTF = ballGO:GetComponent("Transform")

-- Add physics
ballGO:AddComponent("PhysicsBody")
local ballPB = ballGO:GetComponent("PhysicsBody")
ballPB:SetLinearVelocity(0, -2, 0)

-- Add collider
ballGO:AddComponent("Collider")

local ballCO = ballGO:GetComponent("Collider")
ballCO:SetBox(1,1,1)
ballCO:SetGizmoVisible(true)

-- Set initial ball position/scale
ballTF:SetPosition(0, 1.5, 0)
ballTF:SetScale(0.1, 0.1, 0.1)


-- ballPB:SetAngularVelocity(0, 0.1, 0) -- physics based rotation example

function OnUpdate(deltaTime)

    if ballCO:HasCollision() then 
        ballCO:SetGizmoColor(255,0,0)
        -- local linv = ballPB:GetLinearVelocity()
        -- ballPB:SetLinearVelocity(-linv[0], -linv[1], -linv[2])

        local vx, vy, vz = ballPB:GetLinearVelocity()
        ballPB:SetLinearVelocity(-vx, -vy, -vz)

    else 
        ballCO:SetGizmoColor(0,0,255)
    end
    -- ballTF:SetRotation(0, rot, 0) -- kinematic rotation example
    -- if ()

end