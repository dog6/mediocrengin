-- Handles no-clip camera movement and mouse look

local speed = 10.0
local fastMultiplier = 3.0
local mouseSensitivity = 0.1

local cursorLocked = true

-- Lock cursor to screen
Cursor.SetCursorLockMode(CursorLockMode.LOCKED)

function ListenForCursorLockToggle()
 if (KeyboardInput.IsKeyPressed(Key.KEY_ESCAPE)) then
        cursorLocked = not cursorLocked
        if (cursorLocked) then
            Cursor.SetCursorLockMode(CursorLockMode.LOCKED)
            print("Cursor locked")
        else
            Cursor.SetCursorLockMode(CursorLockMode.NONE)
            print("Cursor unlocked")
        end

    end
end

function HandleNoclip(dt)
    local moveSpeed = speed * dt

    -- Hold shift to move faster
    if KeyboardInput.IsKeyDown(Key.KEY_LEFT_SHIFT) then
        moveSpeed = moveSpeed * fastMultiplier
    end

    -- Get current camera position and target
    local px, py, pz = camera:GetPosition()
    local tx, ty, tz = camera:GetTarget()

    -- IMPORTANT: Must call GetMousePosition first to update internal state
    local mdx, mdy = MouseInput.GetMouseDelta()
    
    -- Calculate current forward vector
    local fx = tx - px
    local fy = ty - py
    local fz = tz - pz
    local flen = math.sqrt(fx*fx + fy*fy + fz*fz)
    fx, fy, fz = fx/flen, fy/flen, fz/flen

    -- Calculate right vector (cross product of forward and world up)
    local upx, upy, upz = 0, 1, 0
    local rx = fy*upz - fz*upy
    local ry = fz*upx - fx*upz
    local rz = fx*upy - fy*upx
    local rlen = math.sqrt(rx*rx + ry*ry + rz*rz)
    rx, ry, rz = rx/rlen, ry/rlen, rz/rlen

    -- Apply mouse rotation
    -- Horizontal rotation (yaw) around world up axis - FIXED: removed negative sign
    local yawAngle = mdx * mouseSensitivity * dt
    local cosYaw = math.cos(yawAngle)
    local sinYaw = math.sin(yawAngle)
    local nfx = fx * cosYaw - fz * sinYaw
    local nfz = fx * sinYaw + fz * cosYaw
    fx, fz = nfx, nfz

    -- Vertical rotation (pitch) around right axis
    -- Using the actual right vector for proper pitch rotation
    local pitchAngle = mdy * mouseSensitivity * dt
    
    -- Rotate forward vector around the right vector
    local dot = fx*rx + fy*ry + fz*rz
    local cosPitch = math.cos(pitchAngle)
    local sinPitch = math.sin(pitchAngle)
    
    fx = fx * cosPitch + (rx * dot * (1 - cosPitch) - (ry*fz - rz*fy) * sinPitch)
    fy = fy * cosPitch + (ry * dot * (1 - cosPitch) - (rz*fx - rx*fz) * sinPitch)
    fz = fz * cosPitch + (rz * dot * (1 - cosPitch) - (rx*fy - ry*fx) * sinPitch)

    -- Re-normalize forward vector
    flen = math.sqrt(fx*fx + fy*fy + fz*fz)
    fx, fy, fz = fx/flen, fy/flen, fz/flen

    -- Calculate HORIZONTAL forward vector (flatten Y for WASD movement)
    local hfx = fx
    local hfz = fz
    local hflen = math.sqrt(hfx*hfx + hfz*hfz)
    if hflen > 0.0001 then
        hfx, hfz = hfx/hflen, hfz/hflen
    end

    -- Recalculate right vector for movement
    rx = fy*upz - fz*upy
    ry = fz*upx - fx*upz
    rz = fx*upy - fy*upx
    rlen = math.sqrt(rx*rx + ry*ry + rz*rz)
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


    -- Update camera position
    camera:SetPosition(px + dx, py + dy, pz + dz)
    
    -- Update camera target based on rotated forward vector
    camera:SetTarget(px + dx + fx, py + dy + fy, pz + dz + fz)

end

function OnUpdate(dt)
    ListenForCursorLockToggle()

    if (cursorLocked) then
        HandleNoclip(dt)
    end
    
end