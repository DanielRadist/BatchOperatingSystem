#pragma once
#ifndef BOS_TASK_H

#include "Operation.h"

#include <string>
#include <map>
#include <vector>

enum TaskType;
enum TaskStatus;
class Task;
class Process;
class IntIO;
class Balance;

enum TaskType
{
	PROCESS,
	INT_IO,
	BALANCE
};

enum TaskStatus
{
	TaskQueue,				// ������� � �������
	TaskPerformed,			// �����������
	TaskWaitIO,				// ��������� ���� / ����� (��� �������� IO)
	TaskReady				// ���������
};

inline std::string TaskTypeToString(TaskType type)
{
	static std::map<TaskType, std::string> list = 
	{
		{ TaskType::PROCESS, "[PROCESS]" },
		 { TaskType::INT_IO, "[INT_IO ]" },
		{ TaskType::BALANCE, "[BALANCE]" }
	};

	return list.at(type);
}

/// <summary>
/// ����������� ����� - ������. ����� ��������.
/// </summary>
class Task
{
protected:

	TaskType type;							// ��� ������
	TaskStatus status;						// ��������� ������
	std::vector<Operation*> queue;			// ������� ��������
	int iterratorQueue;						// ��������� �� ������� ��-� � �������

	int tid;								// task id

	const int timeOperationProcess = 5;	// ����� �� ���� �������������� ��������
	const int timeOperationIO = 50;		// ����� �� ���� �������� �����/������

private:

	std::string getOperationsString()
	{
		std::string tmp = "";
		std::string valueStr = std::to_string(queue.size());

		while (tmp.size() + valueStr.size() < 5)
			tmp.push_back(' ');

		return tmp + valueStr;
	}

public:

	Task(TaskType type, int tid)
	{
		this->type = type;
		this->iterratorQueue = 0;
		this->tid = tid;

		this->status = TaskStatus::TaskQueue;
	}

	~Task()
	{
		while (!queue.empty())
		{
			delete queue.front();
			queue.erase(queue.begin());
		}
	}

	Operation* getCurrentOperation()
	{
		return queue.at(iterratorQueue);
	}

	bool hasNextOperation()
	{
		if (iterratorQueue + 1 < queue.size())
			return true;
		else
		{
			this->status = TaskReady;
			return false;
		}
	}

	void nextOperation()
	{
		iterratorQueue++;
	}

	void startPosIterrator()
	{
		iterratorQueue = 0;
	}

	int getIterratorQueue()
	{
		return iterratorQueue;
	}

	TaskType getType()
	{
		return type;
	}

	TaskStatus getStatus()
	{
		return status;
	}

	void setStatus(TaskStatus status)
	{
		this->status = status;
	}

	int getTimeWork()
	{
		int sum = 0;
		for (int i = 0; i < queue.size(); i++)
			sum += queue.at(i)->getTimeWork();
		return sum;
	}

	int getTimeWait()
	{
		int sum = 0;
		for (int i = 0; i < queue.size(); i++)
			sum += queue.at(i)->getTimeWait();
		return sum;
	}

	std::string getTimeWorkString()
	{
		std::string tmp = "";
		std::string valueStr = std::to_string(getTimeWork());

		while (tmp.size() + valueStr.size() < 5)
			tmp.push_back(' ');

		return tmp + valueStr;
	}

	std::string getTimeWaitString()
	{
		std::string tmp = std::to_string(getTimeWait());
		while (tmp.size() < 5)
			tmp.push_back(' ');
		return tmp;
	}

	
	virtual std::string getInfo()
	{
		return TaskTypeToString(type) + "[" + std::to_string(tid) + "] operations:" 
			+ getOperationsString() + "  time work / time wait :" + getTimeWorkString() + " / " + getTimeWaitString();
	}
};

/// <summary>
/// �������������� ������
/// </summary>
class Process : public Task
{
public:

	Process(int operations, int tid) : Task(TaskType::PROCESS, tid) 
	{
		for (int i = 0; i < operations; i++)
			queue.push_back(new Operation(OperationType::Math, timeOperationProcess, 0));
	}
};

/// <summary>
/// ������ �� ����/�����
/// </summary>
class IntIO : public Task
{
public:
	IntIO(int operations, int tid) : Task(TaskType::INT_IO, tid)
	{
		srand(time(NULL));
		for (int i = 0; i < operations; i++)
		{
			if ((rand() % 100) < 20)					// ����������� ��������� �������� �� ���������� � ������ IO
				queue.push_back(new Operation(OperationType::Math, timeOperationProcess, 0));
			else
				queue.push_back(new Operation(OperationType::IO, timeOperationProcess, timeOperationIO));
		}
	}
};

/// <summary>
/// ���������������� ������ (������� �� ����� �������������� � ����� / ������ ��������)
/// </summary>
class Balance : public Task
{
public:
	Balance(int operations, int tid) : Task(TaskType::BALANCE, tid)
	{
		srand(time(NULL));
		for (int i = 0; i < operations; i++)
		{
			if ((rand() % 100) < 50)					// ����������� ��������� �������� �� ���������� � ������ Balance
				queue.push_back(new Operation(OperationType::Math, timeOperationProcess, 0));
			else
				queue.push_back(new Operation(OperationType::IO, timeOperationProcess, timeOperationIO));
		}
	}
};

#endif // BOS_TASK_H
