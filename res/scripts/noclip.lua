-- Handles no-clip camera movement only

local speed = 10.0
local fastMultiplier = 3.0

function OnUpdate(dt)
    local moveSpeed = speed * dt

    -- Hold shift to move faster
    if KeyboardInput.IsKeyDown(Key.KEY_LEFT_SHIFT) then
        moveSpeed = moveSpeed * fastMultiplier
    end

    -- Get current camera position and target
    local px, py, pz = camera:GetPosition()
    local tx, ty, tz = camera:GetTarget()

    -- Calculate forward vector (normalized)
    local fx = tx - px
    local fy = ty - py
    local fz = tz - pz
    local flen = math.sqrt(fx*fx + fy*fy + fz*fz)
    fx, fy, fz = fx/flen, fy/flen, fz/flen

    -- Calculate HORIZONTAL forward vector (flatten Y for WASD movement)
    local hfx = fx
    local hfz = fz
    local hflen = math.sqrt(hfx*hfx + hfz*hfz)
    hfx, hfz = hfx/hflen, hfz/hflen

    -- Calculate right vector (cross product of forward and world up)
    local upx, upy, upz = 0, 1, 0
    local rx = fy*upz - fz*upy
    local ry = fz*upx - fx*upz
    local rz = fx*upy - fy*upx
    local rlen = math.sqrt(rx*rx + ry*ry + rz*rz)
    rx, ry, rz = rx/rlen, ry/rlen, rz/rlen

    -- Apply movement
    local dx, dy, dz = 0, 0, 0

    -- Forward / Backward (use horizontal forward vector)
    if KeyboardInput.IsKeyDown(Key.KEY_W) then
        dx = dx + hfx * moveSpeed
        dz = dz + hfz * moveSpeed
    end
    if KeyboardInput.IsKeyDown(Key.KEY_S) then
        dx = dx - hfx * moveSpeed
        dz = dz - hfz * moveSpeed
    end

    -- Left / Right
    if KeyboardInput.IsKeyDown(Key.KEY_A) then
        dx = dx - rx * moveSpeed
        dy = dy - ry * moveSpeed
        dz = dz - rz * moveSpeed
    end
    if KeyboardInput.IsKeyDown(Key.KEY_D) then
        dx = dx + rx * moveSpeed
        dy = dy + ry * moveSpeed
        dz = dz + rz * moveSpeed
    end

    -- Up / Down (world space vertical)
    if KeyboardInput.IsKeyDown(Key.KEY_SPACE) then
        dy = dy + moveSpeed
    end
    if KeyboardInput.IsKeyDown(Key.KEY_LEFT_CONTROL) then
        dy = dy - moveSpeed
    end

    -- Update camera position and target
    camera:SetPosition(px + dx, py + dy, pz + dz)
    camera:SetTarget(tx + dx, ty + dy, tz + dz)
end