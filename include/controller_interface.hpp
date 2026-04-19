#include <SDL3/SDL.h>
#include <QObject>
#include <vector>
#include <string>
class ControllerInterface : public QObject
{
    Q_OBJECT
    public:
        ControllerInterface();
        ~ControllerInterface();
        void runLoop();
        std::string getControllerName();
    signals:
        void controllerAxisChanged(std::vector<Sint16> values);
    private:
        SDL_Gamepad* m_gamepad = nullptr;
        bool m_gamepadConnected = false;
        Sint16 m_acceptableErrorRange = 200;
        std::vector<Sint16> m_values;
        std::string m_name;
};