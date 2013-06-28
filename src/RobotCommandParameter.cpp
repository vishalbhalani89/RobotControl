#include "RobotCommandParameter.h"
#include <iostream>

RobotCommandParameter::RobotCommandParameter(void)
{
    
}

QString RobotCommandParameter::getCommand(void)
{
	return mCommand;
}
QString RobotCommandParameter::getType(void)
{
	return mType;
}
QString RobotCommandParameter::getName(void)
{
	return mName;
}
double RobotCommandParameter::getDelta(void)
{
	return mDelta;
}
QString RobotCommandParameter::getReferenceFrame(void)
{
	return mReferenceFrame;
}
QString RobotCommandParameter::getSystemMessage(void)
{
	return mSystemMessage;
}
double RobotCommandParameter::getTx(void)
{
	return mTx;
}
double RobotCommandParameter::getTy(void)
{
	return mTy;
}
double RobotCommandParameter::getTz(void)
{
	return mTz;
}
double RobotCommandParameter::getRx(void)
{
	return mRx;
}
double RobotCommandParameter::getRy(void)
{
	return mRy;
}
double RobotCommandParameter::getRz(void)
{
	return mRz;
}
void RobotCommandParameter::setCommand(QString command)
{
	mCommand=command;
}
void RobotCommandParameter::setType(QString type)
{
	mType=type;
}
void RobotCommandParameter::setName(QString name)
{
	mName=name;
}
void RobotCommandParameter::setDelta(double delta)
{
	mDelta=delta;
}
void RobotCommandParameter::setReferenceFrame(QString referenceFrame)
{
	mReferenceFrame=referenceFrame;
}
void RobotCommandParameter::setSystemMessage(QString systemMessage)
{
	mSystemMessage=systemMessage;
}
void RobotCommandParameter::setTx(double Tx)
{
	mTx=Tx;
}
void RobotCommandParameter::setTy(double Ty)
{
	mTy=Ty;
}
void RobotCommandParameter::setTz(double Tz)
{
	mTz=Tz;
}
void RobotCommandParameter::setRx(double Rx)
{
	mRx=Rx;
}
void RobotCommandParameter::setRy(double Ry)
{
	mRy=Ry;
}
void RobotCommandParameter::setRz(double Rz)
{
	mRz=Rz;
}
bool RobotCommandParameter::getTransformationMatrix()
{
	return mTransformationMatrix;
}
void RobotCommandParameter::setTransformationMatrix(bool flag)
{
	mTransformationMatrix= flag;
}
void RobotCommandParameter::setSimulateFirst(bool flag)
{
	mSimulateFirst=flag;
}
bool RobotCommandParameter::getSimulateFirst()
{
	return mSimulateFirst;
}
RobotCommandParameter::~RobotCommandParameter(void)
{
}

