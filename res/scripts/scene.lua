print("--- scene.lua started ---")
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

    -- 4. Load the default shader
    renderer:LoadShader(
        "DefaultShader",
        "res/shaders/vertexShaders/devShader.vert",
        "res/shaders/fragShaders/devShader.frag"
    )

    -- Add Transform component
    go:AddComponent("Transform")

    return go
end

local cursorLocked = true

-- Lock cursor to screen
Cursor.SetCursorLockMode(CursorLockMode.LOCKED)


local terrainGO = CreateMeshObject("Terrain", "./res/models/mdl_world.obj")
local terrainTF = terrainGO:GetComponent("Transform")

local ballGO = CreateMeshObject("Ball", "./res/models/mdl_ball.obj")
local ballTF = ballGO:GetComponent("Transform")

local isoGO = CreateMeshObject("Ball", "./res/models/mdl_isosphere.obj")
local isoTF = isoGO:GetComponent("Transform")

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

    if (KeyboardInput.IsKeyPressed(Key.KEY_ESCAPE)) then
        cursorLocked = not cursorLocked

        if (cursorLocked) then
            Cursor.SetCursorLockMode(CursorLockMode.LOCKED)
        else
            Cursor.SetCursorLockMode(CursorLockMode.NONE)
        end

    end

end