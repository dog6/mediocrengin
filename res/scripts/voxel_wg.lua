print("=== voxel_wg.lua started ===")

-- World settings
local WORLD_SIZE = 16        -- 12x12 grid (144 columns)
local MAX_HEIGHT = 6         -- Maximum terrain height
local BLOCK_SIZE = 2         -- Space between blocks
local WORLD_Z = 30          -- Push back so we can see it all

-- Seed for world generation
local WORLD_SEED = 123456789

print("World Seed: " .. WORLD_SEED)
math.randomseed(WORLD_SEED)

-- Generate random octave offsets for variety
local offset1 = math.random() * 100
local offset2 = math.random() * 100
local offset3 = math.random() * 100

function getTerrainHeight(x, z)
    -- Multi-layered noise for more interesting terrain
    -- Large features (hills and valleys)
    local h1 = math.sin((x + offset1) * 0.25) * math.cos((z + offset1) * 0.25)
    
    -- Medium features (rolling terrain)
    local h2 = math.sin((x + offset2) * 0.5) * math.sin((z + offset2) * 0.5) * 0.6
    
    -- Small features (detail)
    local h3 = math.sin((x + offset3) * 1.2) * math.cos((z + offset3) * 1.2) * 0.3
    
    -- Combine all layers
    local height = (h1 + h2 + h3 + 2.0) / 4.0  -- Normalize to 0-1 range
    height = math.floor(height * MAX_HEIGHT) + 1
    
    return math.max(1, math.min(MAX_HEIGHT, height))
end

print("Generating " .. WORLD_SIZE .. "x" .. WORLD_SIZE .. " terrain...")
print("Max height: " .. MAX_HEIGHT .. " blocks")

local blocks = {}
local blockCount = 0

-- Center the world
local startX = -(WORLD_SIZE - 1) * BLOCK_SIZE / 2
local startZ = -(WORLD_SIZE - 1) * BLOCK_SIZE / 2

-- Generate terrain
for x = 0, WORLD_SIZE - 1 do
    for z = 0, WORLD_SIZE - 1 do
        local height = getTerrainHeight(x, z)
        
        -- Build column of blocks up to height
        for y = 0, height - 1 do
            blockCount = blockCount + 1
            
            local blockName = "Block_" .. blockCount
            local go = scene:CreateGameObject(blockName)
            
            go:AddComponent("MeshRenderer")
            local renderer = go:GetComponent("MeshRenderer")
            
            renderer:LoadMesh("res/models/mdl_grass_cube.obj")
            renderer:LoadShader(
                "VoxelShader_" .. blockCount,
                "res/shaders/vertexShaders/devShader.vert",
                "res/shaders/fragShaders/devShader.frag"
            )
            
            -- Position block
            local transform = go:GetComponent("Transform")
            local posX = startX + (x * BLOCK_SIZE)
            local posY = y * BLOCK_SIZE -5 
            local posZ = WORLD_Z + (z * BLOCK_SIZE)
            
            transform:SetPosition(posX, posY, posZ)
            
            -- Store block info
            blocks[blockCount] = {
                transform = transform,
                x = x,
                y = y,
                z = z,
                worldX = posX,
                worldY = posY,
                worldZ = posZ,
                isTop = (y == height - 1)  -- Track if this is the top block
            }
        end
    end
    
    -- Progress
    if x % 3 == 0 then
        local progress = math.floor((x / WORLD_SIZE) * 100)
        print("Progress: " .. progress .. "% (" .. blockCount .. " blocks)")
    end
end

print("Terrain generation complete!")
print("Total blocks: " .. blockCount)
print("World center: (0, 0, " .. WORLD_Z .. ")")

print("=== World Ready! ===")
print("Seed: " .. WORLD_SEED .. " - Change WORLD_SEED for different terrain!")
print("Tip: Position camera at (0, 10, 0) looking forward to see terrain")