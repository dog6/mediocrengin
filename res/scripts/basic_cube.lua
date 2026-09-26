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

-- Create gameObject 'plane_lower'
local plGO = CreateMeshObject("plane_lower", "D:/Projects/CPP/smallengine/res/models/dev/plane/mdl_plane.obj")

local plTF = plGO:GetComponent("Transform") -- reference Transform component
plTF:SetPosition(0, -1, 0) -- set transform position

-- local plCOL = AddAndGetComponent(plGO, "Collider") -- add and reference Collider component
-- plCOL:SetBox(1,1,1) -- set collider shape to Box
-- plCOL:SetGizmoVisible(true) -- set collider gizmo visible


function OnUpdate(deltaTime)
end