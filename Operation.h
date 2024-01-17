#pragma once
#ifndef BOS_OPERATION_H

class Operation;
enum OperationStatus;
enum OperationType;

enum OperationStatus
{
	OperQueue,				// ������� � �������
	OperPerformed,			// �����������
	OperWaitIO,				// ��������� ���� / ����� (��� �������� IO)
	OperReady				// ��������
};

enum OperationType
{
	Math,				// �������������� ��������
	IO					// �������� ����� / ������
};

class Operation
{
protected:
	int timeWork;		// ����� ����������
	int timeWait;		// ����� �������� (��� IO ��������)
	OperationStatus status;
	OperationType type;

public:

	Operation(OperationType type, int timeWork, int timeWait)
	{
		this->type = type;
		this->timeWork = timeWork;
		this->timeWait = timeWait;
		this->status = OperationStatus::OperQueue;
	}

	OperationStatus getStatus()
	{
		return status;
	}

	OperationType getType()
	{
		return type;
	}

	void setStatusQueue()
	{
		status = OperationStatus::OperQueue;
	}

	void setStatusPerformed()
	{
		status = OperationStatus::OperPerformed;
	}

	void setStatusWaitIO()
	{
		status = OperationStatus::OperWaitIO;
	}

	void setStatusReady()
	{
		status = OperationStatus::OperReady;
	}

	int getTimeWork()
	{
		return timeWork;
	}

	int getTimeWait()
	{
		return timeWait;
	}

	void setTimeWork(int time)
	{
		timeWork = time;
	}

	void setTimeWait(int time)
	{
		timeWait = time;
	}

	~Operation()
	{

	}

};

#endif // BOS_OPERATION_H