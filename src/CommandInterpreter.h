#include "CRobotControl.h"

class CommandInterpreter : public QThread
{
private:
    CRobotControl *control;
public:
    CommandInterpreter(CRobotControl *control);
    void run();
};
