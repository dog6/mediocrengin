# Lua Scripting Documentation

The AVGNG is controlled through Lua scripts. Everything from loading meshes and shaders to listening for player input and updating transforms can be accomplished through `lua bindings`.

This engine uses the sol2 library as a bridge between the engine and Lua.

> [!IMPORTANT]
> **Please keep in mind**: As the engine is early in development, scripting keywords *may* be subject to change in the future.

---

## API Reference

### Scene

#### Methods

| Method | Description |
|--------|-------------|
| `scene:CreateGameObject(string objectName)` | Creates a new game object with the specified name |

---

<details>
  <summary>ng::Core</summary>
<br/>
<details>
  <summary>GameObject</summary>

#### Properties

| Property | Type | Description |
|----------|------|-------------|
| `name` | `const char*` | The name of the game object |

#### Methods

| Method | Description |
|--------|-------------|
| `GameObject:AddComponent(string componentName)` | Adds a component to the game object |
| `GameObject:GetComponent(string componentName)` | Retrieves a component from the game object |

---
</details>

<details>
  <summary>Transform</summary>

#### Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `Transform:SetPosition(float x, float y, float z)` | `void` | Sets the position of the transform |
| `Transform:GetPosition()` | `(float x, float y, float z)` | Gets the current position |
| `Transform:SetRotation(float x, float y, float z)` | `void` | Sets the rotation of the transform |
| `Transform:GetRotation()` | `(float x, float y, float z)` | Gets the current rotation |
| `Transform:SetScale(float x, float y, float z)` | `void` | Sets the scale of the transform |
| `Transform:GetScale()` | `(float x, float y, float z)` | Gets the current scale |

---
</details>

<details>
  <summary>Camera</summary>  

#### Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `camera:SetPosition(float x, float y, float z)` | `void` | Sets the camera position |
| `camera:GetPosition()` | `(float x, float y, float z)` | Gets the current camera position |
| `camera:SetTarget(float x, float y, float z)` | `void` | Sets the camera target |
| `camera:GetTarget()` | `(float x, float y, float z)` | Gets the current camera target |
| `camera:SetUpwardDirection(float x, float y, float z)` | `void` | Sets the camera's upward direction |
| `camera:GetUpwardDirection()` | `(float x, float y, float z)` | Gets the current upward direction |

---
</details>

<details>
  <summary>Keyboard Input</summary>

#### Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `KeyboardInput.IsKeyDown(Key key)` | `bool` | Checks if a key is currently held down |
| `KeyboardInput.IsKeyPressed(Key key)` | `bool` | Checks if a key was just pressed |
| `KeyboardInput.IsKeyReleased(Key key)` | `bool` | Checks if a key was just released |

#### Key Constants

**Regular Keys**
```
KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M,
KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z
```

**Number Keys (Top Row)**
```
KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9
```

**Function Keys**
```
KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12
```

**Control Keys**
```
KEY_ESCAPE, KEY_ENTER, KEY_TAB, KEY_BACKSPACE, KEY_INSERT, KEY_DELETE,
KEY_RIGHT, KEY_LEFT, KEY_DOWN, KEY_UP, KEY_PAGE_UP, KEY_PAGE_DOWN, KEY_HOME, KEY_END,
KEY_CAPS_LOCK, KEY_SCROLL_LOCK, KEY_NUM_LOCK, KEY_PRINT_SCREEN, KEY_PAUSE
```

**Modifier Keys**
```
KEY_LEFT_SHIFT, KEY_LEFT_CONTROL, KEY_LEFT_ALT, KEY_LEFT_SUPER,
KEY_RIGHT_SHIFT, KEY_RIGHT_CONTROL, KEY_RIGHT_ALT, KEY_RIGHT_SUPER, KEY_MENU
```

**Special Keys**
```
KEY_SPACE, KEY_APOSTROPHE, KEY_COMMA, KEY_MINUS, KEY_PERIOD, KEY_SLASH,
KEY_SEMICOLON, KEY_EQUAL, KEY_LEFT_BRACKET, KEY_BACKSLASH, KEY_RIGHT_BRACKET, KEY_GRAVE_ACCENT
```

**Keypad Keys**
```
KEY_KP_0, KEY_KP_1, KEY_KP_2, KEY_KP_3, KEY_KP_4, KEY_KP_5, KEY_KP_6, KEY_KP_7, KEY_KP_8, KEY_KP_9,
KEY_KP_DECIMAL, KEY_KP_DIVIDE, KEY_KP_MULTIPLY, KEY_KP_SUBTRACT, KEY_KP_ADD, KEY_KP_ENTER, KEY_KP_EQUAL
```

---
</details>

<details>
  <summary>MouseInput</summary>
### MouseInput

#### Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `MouseInput.GetMousePosition()` | `(float x, float y)` | Gets the current mouse position |
| `MouseInput.GetMouseDelta()` | `(float dx, float dy)` | Gets the mouse movement delta |
| `MouseInput.IsMouseButtonPressed(MouseButton button)` | `bool` | Checks if a mouse button is pressed |

#### MouseButton Constants
```
LEFT_BUTTON, RIGHT_BUTTON, MIDDLE_BUTTON,
BUTTON_4, BUTTON_5, BUTTON_6, BUTTON_7, BUTTON_8
```

---
</details>

<details>
  <summary>Cursor</summary>
### Cursor

#### Methods

| Method | Description |
|--------|-------------|
| `Cursor:SetCursorLockMode(CursorLockMode lockMode)` | Sets the cursor lock mode |

#### CursorLockMode Constants
```
NONE     - Cursor is free to move
LOCKED   - Cursor is locked to the window center
CONFINED - Cursor is confined to the window bounds
```
</details>

</details>

<details>
  <summary>ng::Graphics</summary>
<br/>
<details>
<summary>MeshRenderer</summary>

#### Methods

| Method | Description |
|--------|-------------|
| `MeshRenderer:LoadMesh(string mesh_filepath)` | Loads a mesh from the specified file path |
| `MeshRenderer:LoadShader(string shaderName, string vertex_filepath, string fragment_filepath)` | Loads a shader with the given name and file paths |

---
</details>
</details>



<details>
  <summary>Generic Lua Engine Bindings</summary>

### Generic Methods

#### Methods

| Method | Description |
|--------|-------------|
| `print("")` | Overridden lua print() method to support engine logging |
| `clear()` | Clears in-engine developer console

---
</details>
