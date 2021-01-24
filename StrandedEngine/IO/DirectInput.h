// Stranded Engine I/O
// DirectInput 接口声明

#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "IIOSystem.h"


class Keyboard
{
   public:
      Keyboard(LPDIRECTINPUT8 input, HWND hwnd);
	  ~Keyboard();

      bool UpdateDevice();

      int ButtonUp(IOKeyBoardMap key);
      int ButtonDown(IOKeyBoardMap key);

      POINT GetPosition();
      POINT GetZPosition();

      void Shutdown();

      // Keyboard device.
      LPDIRECTINPUTDEVICE8 m_device;
      char m_keys[256];   // buffer
      char m_oldKeys[256];   // buffer
};


class Mouse
{
   public:
      Mouse(LPDIRECTINPUT8 input, HWND hwnd, bool exclusive);
	  ~Mouse();

      bool UpdateDevice();

      int ButtonUp(IOMouseMap key);
      int ButtonDown(IOMouseMap key);

      POINT GetPosition();
      POINT GetZPosition();

      void Shutdown();

      // Mouse device.
      LPDIRECTINPUTDEVICE8 m_device;
      DIMOUSESTATE m_mouseState;
      DIMOUSESTATE m_oldMouseState;
      bool m_button[3];

      // Mouse x, y, and wheel position.
      long m_xMPos;
      long m_yMPos;
      long m_zMPos;

      // Max x (width) and y (height)
      // Restrict means to cap at width/height.
      bool m_restrict;
      int m_width;
      int m_height;
};

class DirectInputSystem : public IIOSystem {
public:
    DirectInputSystem(HWND hwnd, HINSTANCE hInst, bool exclusive);
    virtual ~DirectInputSystem();

    virtual bool Initialize() override;
    virtual bool UpdateDevices() override;


    // 依据旧视角和鼠标的瞬时转动，重设新视角
    virtual void UpdateByMouseRotation(
        float* angleH, 
        float* angleV,
        POINT* g_mousePos, 
        POINT* g_midWindow, 
        Camera* camera,
        Ray* viewRay) override;

    virtual void GetLeftDirection(Vector3* vec, const Camera* camera) override;
    virtual void GetRightDirection(Vector3* vec, const Camera* camera) override;




    // Keyboard 
    virtual int KeyUp(IOKeyBoardMap key) override;
    virtual int KeyDown(IOKeyBoardMap key) override;

    // Mouse 
    virtual int MouseButtonUp(IOMouseMap button) override;
    virtual int MouseButtonDown(IOMouseMap button) override;

    // Get mouse position (x, y) and mouse wheel data (z).
    virtual POINT GetMousePos() override;
    virtual long GetMouseWheelPos() override;

    void Release();

private:
    LPDIRECTINPUT8 m_system;

    Keyboard* m_keyboard;
    Mouse* m_mouse;
};
