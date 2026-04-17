#include "controller_interface.hpp"
#include <iostream>
#include <thread>
#include <chrono>

ControllerInterface::ControllerInterface()
{
    SDL_Init(SDL_INIT_GAMEPAD);
    int joystickCount = 0;
    SDL_JoystickID* joysticks = SDL_GetJoysticks(&joystickCount);
    if (joysticks && joystickCount > 0)
    {
        m_gamepad = SDL_OpenGamepad(joysticks[0]);
        m_gamepadConnected = true;
        std::cout << "Controller connected: " << SDL_GetGamepadName(m_gamepad) << std::endl;
        SDL_Joystick* joystick = SDL_GetGamepadJoystick(m_gamepad);
        int numAxes = SDL_GetNumJoystickAxes(joystick);
        m_values.resize(numAxes);
        for (int i = 0; i < numAxes; i++)
        {
            m_values[i] = SDL_GetJoystickAxis(joystick, i);
        }
    }
}

ControllerInterface::~ControllerInterface()
{
    SDL_CloseGamepad(m_gamepad);
}

void ControllerInterface::runLoop()
{
    SDL_PumpEvents();
    if (m_gamepadConnected)
    {
        bool signal = false;
        SDL_Joystick* joystick = SDL_GetGamepadJoystick(m_gamepad);
        int numAxes = SDL_GetNumJoystickAxes(joystick);
        for (int i = 0; i < numAxes; i++)
        {
            Sint16 value = SDL_GetJoystickAxis(joystick, i);
            if(SDL_abs(value - m_values[i]) > m_acceptableErrorRange)
            {
                signal = true;
                m_values[i] = value;
            }
        }
        if(signal)
        {
            emit controllerAxisChanged(m_values);
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}