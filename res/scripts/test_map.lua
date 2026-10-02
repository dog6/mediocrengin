-- Development scene
-- used as sanity check
-- when all else fails, this will put a plane on the screen

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

-- Create gameObject 'test_map_0'
local plGO = CreateMeshObject("test_map_0", "D:/Projects/CPP/smallengine/res/models/dev/test_map_0/mdl_map0.obj")

local plTF = plGO:GetComponent("Transform") -- reference Transform component
plTF:SetPosition(0, -1, 0) -- set transform position

-- Unused, just here
function OnUpdate(deltaTime)
end