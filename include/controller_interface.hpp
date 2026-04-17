#include <SDL3/SDL.h>
#include <QObject>
#include <vector>
class ControllerInterface : public QObject
{
    Q_OBJECT
    public:
        ControllerInterface();
        ~ControllerInterface();
        void runLoop();
    signals:
        void controllerAxisChanged(std::vector<Sint16> values);
    private:
        SDL_Gamepad* m_gamepad = nullptr;
        bool m_gamepadConnected = false;
        Sint16 m_acceptableErrorRange = 200;
        std::vector<Sint16> m_values;
};