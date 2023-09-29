#pragma once
#ifndef BOS_TASK.H
#include <string>
#include <map>

enum TaskType;
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
/// Абстрактный класс - Задача. Набор операций.
/// </summary>
class Task
{
protected:

	TaskType type;			// Тип задачи
	int operations;			// Кол-во операций

	const int timeOperationProcess = 5;	// время на одну вычислительную операцию
	const int timeOperationIO = 20;		// время на одну операцию ввода/вывода

private:

	std::string getOperationsString()
	{
		std::string tmp = "";
		std::string valueStr = std::to_string(getOperations());

		while (tmp.size() + valueStr.size() < 5)
			tmp.push_back(' ');

		return tmp + valueStr;
	}

	std::string getTimeString()
	{
		std::string tmp = std::to_string(getTime());
		while (tmp.size() < 5)
			tmp.push_back(' ');
		return tmp;
	}

public:

	Task(TaskType type, int operations)
	{
		this->type = type;
		this->operations = operations;
	}

	TaskType getType()
	{
		return type;
	}

	virtual int getOperations()
	{
		return operations;
	}

	virtual int getTime() = 0;

	virtual std::string getInfo()
	{
		return "### Task: " + TaskTypeToString(type) + " " + "  operations / time : " + getOperationsString() + " / " + getTimeString();
	}
};

/// <summary>
/// Вычислительная задача
/// </summary>
class Process : public Task
{
public:

	Process(int operations) : Task(TaskType::PROCESS, operations) { }

	int getTime()
	{
		return timeOperationProcess * operations;
	}
};

/// <summary>
/// Задача на ввод/вывод
/// </summary>
class IntIO : public Task
{
public:

	IntIO(int operations) : Task(TaskType::INT_IO, operations) { }

	int getTime()
	{
		return timeOperationIO * operations;
	}
};

/// <summary>
/// Сбалансированная задача (состоит из долей вычислительных и ввода / вывода операций)
/// </summary>
class Balance : public Task
{
private:

	int operationsProcess;
	int operationsIO;

public:

	Balance(int operationsProcess, int operationsIO) : Task(TaskType::BALANCE, -1)
	{
		this->operationsProcess = operationsProcess;
		this->operationsIO = operationsIO;
	}

	Balance(int operations) : Task(TaskType::BALANCE, -1)
	{
		this->operationsIO = operations / 2;
		this->operationsProcess = operations - operationsIO;
	}

	virtual int getOperations()
	{
		return operationsProcess + operationsIO;
	}

	int getTime()
	{
		return timeOperationProcess * operationsProcess + timeOperationIO * operationsIO;
	}
};

#endif // BOS_TASK.H
