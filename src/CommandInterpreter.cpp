#include"CommandInterpreter.h"

CommandInterpreter::CommandInterpreter(CRobotControl *robotControl)
{
    this->control=robotControl;
}

void CommandInterpreter::run()
{
    qDebug()<<"thread started";
    while(true)
    {
        QMap<QString,QString> command = this->control->popCommand();
        if(!command.contains("nodata"))
        {
            sleep(5);
            if(command.contains("kinectmotion"))
            {
                command.remove("kinectmotion");
            }
            foreach (QString key, command.keys())
            {
                qDebug() << key<<" "<<command.value(key);
            }
            control->generateInitialPointCloud();
        }
    }
}
