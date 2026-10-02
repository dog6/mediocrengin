-- Development scene

print("--- development.lua started ---")

local scene = game:GetActiveScene() -- get active scene

-- Helper function
function CreateMeshObject(name, modelPath)
    local go = scene:CreateGameObject(name) -- create new gameObject
    go:AddComponent("MeshRenderer") -- add a MeshRenderer component
    local renderer = go:GetComponent("MeshRenderer") -- reference MeshRenderer
    renderer:LoadMesh(modelPath) -- load mesh into MeshRenderer component
    return go -- return new gameObject
end

function AddAndGetComponent(go, name)
    go:AddComponent(name)
    return go:GetComponent(name)
end

function CreatePlane(x,y,z)
    -- Create gameObject 'plane_lower'
    local plGO = CreateMeshObject("plane_lower", "D:/Projects/CPP/smallengine/res/models/dev/plane/mdl_plane.obj")

    local plTF = plGO:GetComponent("Transform") -- reference Transform component
    plTF:SetPosition(x, y, z) -- set transform position

    local plCOL = AddAndGetComponent(plGO, "Collider") -- add and reference Collider component
    plCOL:SetBox(1,0.2,1) -- set collider shape to Box
    plCOL:SetPositionOffset(0,-0.2,0) -- set collider offset
    plCOL:SetGizmoVisible(true) -- set collider gizmo visible

    local plane = {
        gameobject = plGO,
        transform = plTF,
        collider = plCOL
    }

    return plane

end

function CreateBall(x,y,z)
    -- Create ball with physics
    local bGO = CreateMeshObject("Ball","D:/Projects/CPP/smallengine/res/models/dev/sphere/mdl_sphere.obj") -- Create and load gameObject with Mesh
    local bTF = bGO:GetComponent("Transform") -- reference Transform component

    bTF:SetPosition(x, y, z) -- set ball start position
    bTF:SetScale(0.1, 0.1, 0.1) -- set ball scale

    -- Add collider
    local bCOL = AddAndGetComponent(bGO, "Collider") -- add and reference Collider component
    bCOL:SetBox(1,1,1) -- set collider shape to Box
    bCOL:SetGizmoVisible(true) -- set collider gizmo visible

    -- Add physics
    -- bGO:AddComponent("PhysicsBody")
    local bPB = AddAndGetComponent(bGO, "PhysicsBody") -- add and reference PhysicsBody component
    bPB:SetGravity(0, -0.981, 0) -- set gravity force

    local ball = {
        gameobject = bGO,
        transform = bTF,
        collider = bCOL,
        physics = bPB
    }
    return ball
end

local player = scene:CreateGameObject("Player")
local playerTF = player:GetComponent("Transform")
playerTF:SetPosition(0, 1, 0)

local playerCol = AddAndGetComponent(player, "Collider")
playerCol:SetBox(0.5, 1.8, 0.5) -- same width and depth
local playerPB = AddAndGetComponent(player, "PhysicsBody")
playerPB:SetGravity(0, -9.81, 0)

local camGO = scene:CreateGameObject("PlayerCamera")
AddAndGetComponent(camGO, "CameraComponent")
local camTF = camGO:GetComponent("Transform")
camTF:SetParent(playerTF)
camTF:SetPosition(0, 0.7, 0) -- eye height, local to the player

local planeBottom = CreatePlane(0,-2,0)
planeBottom.transform:SetScale(5,1,5)

local ballA = CreateBall(-0.5,1,0)
local ballB = CreateBall(0.5, 1, 0)

ballA.physics:SetRestitution(0.45)
ballB.physics:SetRestitution(0.9)

function OnUpdate(deltaTime)
end