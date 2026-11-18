#include "controller_interface.hpp"
#include <iostream>
#include <thread>
#include <chrono>

ControllerInterface::ControllerInterface()
{
    // Initialize SDL and check for connected gamepads
    SDL_Init(SDL_INIT_GAMEPAD);
    int joystickCount = 0;
    SDL_JoystickID *joysticks = SDL_GetJoysticks(&joystickCount);
    if (joysticks && joystickCount > 0)
    {
        m_gamepad = SDL_OpenGamepad(joysticks[0]);
        m_gamepadConnected = true;
        std::cout << "Controller connected: " << SDL_GetGamepadName(m_gamepad) << std::endl;
        m_name = SDL_GetGamepadName(m_gamepad);
        SDL_Joystick *joystick = SDL_GetGamepadJoystick(m_gamepad);
        int numAxes = SDL_GetNumJoystickAxes(joystick);
        m_values.resize(numAxes);
        for (int i = 0; i < numAxes; i++)
        {
            m_values[i] = SDL_GetJoystickAxis(joystick, i);
        }
    }
    m_controller = this;
}

ControllerInterface::~ControllerInterface()
{
    // Clean up SDL resources
    if (m_gamepad)
        SDL_CloseGamepad(m_gamepad);
}

void ControllerInterface::runLoop()
{
    // Main loop to poll controller input
    SDL_PumpEvents();
    if (m_gamepadConnected)
    {
        bool signal = false;
        SDL_Joystick *joystick = SDL_GetGamepadJoystick(m_gamepad);
        int numAxes = SDL_GetNumJoystickAxes(joystick);
        for (int i = 0; i < numAxes; i++)
        {
            Sint16 value = SDL_GetJoystickAxis(joystick, i);
            if (SDL_abs(value - m_values[i]) > m_acceptableErrorRange)
            {
                signal = true;
                m_values[i] = value;
            }
        }
        if (signal)
        {
            emit controllerAxisChanged(m_values);
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}
std::string ControllerInterface::getControllerName()
{
    // Return the name of the connected controller
    return m_name;
}

ControllerInterface *ControllerInterface::getController()
{
    // Return the singleton instance of the controller interface
    return m_controller;
}