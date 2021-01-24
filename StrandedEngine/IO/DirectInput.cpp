// Stranded Engine I/O
// DirectInput 接口声明

#include "DirectInput.h"


Keyboard::Keyboard(LPDIRECTINPUT8 input, HWND hwnd)
{
	m_device = nullptr;

   // Initialize the keyboard.
   if(input->CreateDevice(GUID_SysKeyboard, &m_device, NULL) == DI_OK)
      {
         if(m_device->SetDataFormat(&c_dfDIKeyboard) == DI_OK)
            {
               if(m_device->SetCooperativeLevel(hwnd,
                  DISCL_BACKGROUND |DISCL_NONEXCLUSIVE) == DI_OK)
                  {
                     m_device->Acquire();
                  }
            }
   }

   // Clear keys will clear out the array of keys we have.
   memset(m_keys, 0, sizeof(m_keys));
}

Keyboard::~Keyboard()
{
	Shutdown();
}

int Keyboard::ButtonUp(IOKeyBoardMap key)
{
   // If the key in the variable is not pressed then return false.
   return (!(m_keys[key] & 0x80) && m_keys[key] != m_oldKeys[key]);
}


int Keyboard::ButtonDown(IOKeyBoardMap key)
{
   // If the key in the variable is not pressed then return false.
   return m_keys[key] & 0x80;
}


POINT Keyboard::GetPosition()
{
   // Doesn't have position.
   POINT p = { 0, 0 };
   return p;
}


POINT Keyboard::GetZPosition()
{
   // Doesn't have position.
   POINT p = { 0, 0 };
   return p;
}


bool Keyboard::UpdateDevice()
{
   if(m_device)
      {
         // Save old state for input comparing.
         memcpy(m_oldKeys, m_keys, sizeof(m_keys));

         // If error getting device state, re-aquire.
         if(FAILED(m_device->GetDeviceState(sizeof(m_keys), (LPVOID)m_keys)))
            {
               if(FAILED(m_device->Acquire())) return false;
               if(FAILED(m_device->GetDeviceState(sizeof(m_keys), (LPVOID)m_keys)))
                  return false;
            }
       }

   return true;
}


void Keyboard::Shutdown()
{
   if(m_device)
      {
         m_device->Unacquire();
         m_device->Release();
         m_device = nullptr;
      }
}


Mouse::Mouse(LPDIRECTINPUT8 input, HWND hwnd, bool exclusive)
{
	m_device = nullptr;

   // Initialize the Mouse.
   DWORD flags;

   if(input->CreateDevice(GUID_SysMouse, &m_device, NULL) == DI_OK)
      {
         if(m_device->SetDataFormat(&c_dfDIMouse) == DI_OK)
            {
               if(exclusive) flags = DISCL_BACKGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY;
               else flags = DISCL_BACKGROUND | DISCL_NONEXCLUSIVE;

               if(m_device->SetCooperativeLevel(hwnd, flags) == DI_OK)
                  {
                     m_device->Acquire();
                  }
            }
      }
}

Mouse::~Mouse()
{
	Shutdown();
}


int Mouse::ButtonUp(IOMouseMap button)
{
   // If the button is not clicked we return false.
   return (!(m_mouseState.rgbButtons[button] & 0x80) &&
           m_mouseState.rgbButtons[button] != m_oldMouseState.rgbButtons[button]);

}


int Mouse::ButtonDown(IOMouseMap button)
{
   // If the button is clicked we return true.
   return m_mouseState.rgbButtons[button] & 0x80;
}

bool Mouse::UpdateDevice()
{
   // Get the device state.
   if(m_device)
      {
         // Save old state for input comparing.
         memcpy(&m_oldMouseState, &m_mouseState, sizeof(m_mouseState));

         // If error getting device state, re-aquire.
         if(FAILED(m_device->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState)))
            {
               if(FAILED(m_device->Acquire())) return false;
               if(FAILED(m_device->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState)))
                  return false;
            }

         m_xMPos += m_mouseState.lX;
         m_yMPos += m_mouseState.lY;
         m_zMPos = m_mouseState.lZ;
      }

   return true;
}

POINT Mouse::GetPosition()
{
   POINT pos;

   pos.x = m_xMPos;
   pos.y = m_yMPos;
   return pos;
}


POINT Mouse::GetZPosition()
{
   POINT p = { m_zMPos, m_zMPos };
   return p;
}


void Mouse::Shutdown()
{
   if(m_device)
      {
         m_device->Unacquire();
         m_device->Release();
         m_device = nullptr;
      }
}


DirectInputSystem::DirectInputSystem(HWND hwnd, HINSTANCE hInst, bool exclusive)
{
	m_keyboard = nullptr;
	m_mouse = nullptr;

   // Create input system.
   if(DirectInput8Create(hInst, DIRECTINPUT_VERSION,
      IID_IDirectInput8, (void **)&m_system, NULL) == DI_OK)
   {
      m_keyboard = new Keyboard(m_system, hwnd);
      m_mouse = new Mouse(m_system, hwnd, exclusive);
   }
}


DirectInputSystem::~DirectInputSystem()
{
   // Delete each object...
    if (m_keyboard)
    {
        m_keyboard->Shutdown();
        delete m_keyboard;
        m_keyboard = NULL;
    }

    if (m_mouse)
    {
        m_mouse->Shutdown();
        delete m_mouse;
        m_mouse = NULL;
    }

    if (m_system)
    {
        m_system->Release();
        m_system = NULL;
    }
}


bool DirectInputSystem::Initialize()
{
   // Everything took place in the constructor.
   // Force keyboard and mouse before considering the
   // system initialized.
   return (m_keyboard && m_mouse);
}


bool DirectInputSystem::UpdateDevices()
{
   // Get the device state.
   if(m_mouse) m_mouse->UpdateDevice();
   if(m_keyboard) m_keyboard->UpdateDevice();

   return true;
}

void DirectInputSystem::UpdateByMouseRotation(float* angleH, float* angleV, POINT* g_mousePos, POINT* g_midWindow, Camera* camera, Ray* viewRay)
{
    // 累计沿水平和垂直轴旋转的角度（->全局）
        /*static float angleH = 0.5f * D3DX_PI;
        static float angleV = 0.0f;*/

        // 获取鼠标当前位置
    GetCursorPos(g_mousePos);

    // 计算鼠标相对于屏幕中心的位移
    int xDelta = g_mousePos->x - g_midWindow->x;
    int yDelta = g_midWindow->y - g_mousePos->y;

    if (!(xDelta == 0 && yDelta == 0))
    {
        float Tx, Ty, Tz, Txz;

        // 计算摄影机的y轴方向分量
        *angleV += 0.001f * yDelta;

        if (*angleV > 0.49f * D3DX_PI) *angleV = 0.49f * D3DX_PI;
        if (*angleV < -0.49f * D3DX_PI) *angleV = -0.49f * D3DX_PI;

        Ty = sinf(*angleV);    // 沿y方向的分量
        Txz = cosf(*angleV);   // 沿x和z方向分量的合向量

        // 计算摄影机x和z轴方向分量
        *angleH -= 0.001f * xDelta;

        if (*angleH > 2.0f * D3DX_PI) *angleH = 0.0f;
        if (*angleH < 0.0f) *angleH = 2.0f * D3DX_PI;

        Tx = Txz * cosf(*angleH);  // 沿x轴方向分量
        Tz = Txz * sinf(*angleH);  // 沿z轴方向分量

        SetCursorPos(g_midWindow->x, g_midWindow->y);

        // 设置摄影机的lookat参数
        camera->m_view = camera->m_pos + Vector3(Tx, Ty, Tz);

        /* 计算当前朝向（准星指向） */
        viewRay->m_origin = camera->m_pos;
        viewRay->m_direction = camera->m_view - camera->m_pos;
        STEVector3Normalize(&viewRay->m_direction);
    }
}

void DirectInputSystem::GetLeftDirection(Vector3* vec, const Camera* camera)
{
    D3DXVECTOR3 Direction =
        D3DXVECTOR3(camera->m_view.x, 0.0f, camera->m_view.z) -
        D3DXVECTOR3(camera->m_pos.x, 0.0f, camera->m_pos.z);

    D3DXMATRIX RotateLeft;
    D3DXMatrixRotationY(&RotateLeft, -0.5f * D3DX_PI);

    D3DXVec3Normalize(&Direction, &Direction);
    D3DXVec3TransformCoord(&Direction, &Direction, &RotateLeft);
    
    vec->x = Direction.x;
    vec->y = Direction.y;
    vec->z = Direction.z;
}

void DirectInputSystem::GetRightDirection(Vector3* vec, const Camera* camera)
{
    D3DXVECTOR3 Direction =
        D3DXVECTOR3(camera->m_view.x, 0.0f, camera->m_view.z) -
        D3DXVECTOR3(camera->m_pos.x, 0.0f, camera->m_pos.z);

    D3DXMATRIX RotateLeft;
    D3DXMatrixRotationY(&RotateLeft, 0.5f * D3DX_PI);

    D3DXVec3Normalize(&Direction, &Direction);
    D3DXVec3TransformCoord(&Direction, &Direction, &RotateLeft);

    vec->x = Direction.x;
    vec->y = Direction.y;
    vec->z = Direction.z;
}

int DirectInputSystem::KeyUp(IOKeyBoardMap key)
{
   if(!m_keyboard) return 0;
   return m_keyboard->ButtonUp(key);
}


int DirectInputSystem::KeyDown(IOKeyBoardMap key)
{
   if(!m_keyboard) return 0;
   return m_keyboard->ButtonDown(key);
}


int DirectInputSystem::MouseButtonUp(IOMouseMap button)
{
   if(!m_mouse) return 0;
   return m_mouse->ButtonUp(button);
}


int DirectInputSystem::MouseButtonDown(IOMouseMap button)
{
   if(!m_mouse) return 0;
   return m_mouse->ButtonDown(button);
}


POINT DirectInputSystem::GetMousePos()
{
   POINT null = {0, 0};
   if(!m_mouse) return null;

   return m_mouse->GetPosition();
}


long DirectInputSystem::GetMouseWheelPos()
{
   if(!m_mouse) return 0;

   POINT wheel =  m_mouse->GetZPosition();
   return wheel.y;
}


void DirectInputSystem::Release() {
   delete this;
}