#pragma once
#include <qstring.h>

class RobotCommandParameter
{
private:
	QString mCommand;
	QString mType;
	QString mName;
	double mDelta;
	QString mReferenceFrame;
	double mTx;
	double mTy;
	double mTz;
	double mRx;
	double mRy;
	double mRz;
	bool mSimulateFirst;
	QString mSystemMessage;
	bool mTransformationMatrix;

public:
	RobotCommandParameter(void);
	~RobotCommandParameter(void);
	QString getCommand(void);
	QString getType(void);
	QString getName(void);
	double getDelta(void);
	QString getReferenceFrame(void);
	QString getSystemMessage(void);
	double getTx(void);
	double getTy(void);
	double getTz(void);
	double getRx(void);
	double getRy(void);
	double getRz(void);
	bool getSimulateFirst();
	bool getTransformationMatrix();
	void setCommand(QString command);
	void setType(QString type);
	void setName(QString name);
	void setDelta(double delta);
	void setReferenceFrame(QString referenceFrame);
	void setSystemMessage(QString systemMessage);
	void setTx(double Tx);
	void setTy(double Ty);
	void setTz(double Tz);
	void setRx(double Rx);
	void setRy(double Ry);
	void setRz(double Rz);
	void setSimulateFirst(bool flag);
	void setTransformationMatrix(bool flag);
};

