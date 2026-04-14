#include <SDL3/SDL.h>
#include <QObject>

class ControllerInterface
{
    Q_OBJECT
    public:
        ControllerInterface();
        ~ControllerInterface();
        void runLoop();
    signals:
        void controllerAxisChanged();
};