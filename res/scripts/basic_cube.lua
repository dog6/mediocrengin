-- Development scene
-- used as sanity check
-- when all else fails, this will put a plane on the screen

print("--- development.lua started ---")

local scene = game:GetActiveScene() -- get active scene

-- Helper function
function CreateMeshObject(name, modelPath, diffusePath)
    local go = scene:CreateGameObject(name)
    go:AddComponent("MeshRenderer")
    local renderer = go:GetComponent("MeshRenderer")
    renderer:LoadMesh(modelPath)

    -- Load the diffuse texture only if a path is given
    if diffusePath ~= nil then
        renderer:SetTexture("diffuse", diffusePath)
    end

    return go
end


function AddAndGetComponent(go, name)
    go:AddComponent(name)
    return go:GetComponent(name)
end

-- Create gameObject 'plane_lower'
local plGO = CreateMeshObject("plane_lower", "D:/Projects/CPP/smallengine/res/models/dev/plane/mdl_plane.obj", "D:/Projects/CPP/smallengine/res/images/dev_texture.png")

local plTF = plGO:GetComponent("Transform") -- reference Transform component
plTF:SetPosition(0, -1, 0) -- set transform position

-- Unused, just here
function OnUpdate(deltaTime)
end