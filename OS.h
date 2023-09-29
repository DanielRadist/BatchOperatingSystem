#pragma once
#ifndef BOS_OS.H
#include <queue>
#include <list>
#include <string>
#include <ctime>
#include <windows.h>

#include "Task.h"

/// <summary>
/// �������� ������� ��
/// </summary>
class OS
{
private:
	std::queue<Task*>* package = nullptr;		// ������� �� ����� - �����
	std::list<Task*>* trash = nullptr;			// ������� ��� ������������ ����� 
	const int operationsMax = 20;				// ������������ ���-�� �������� � ������

	// �����
	int countOperations = 0;
	int countTime = 0;

	// ���� �����
	
	int countTimeTaskProcess = 0;				// ��������������
	int countTimeTaskIO = 0;					// ���� / �����
	int countTimeTaskBalance = 0;				// ����������������

public:

	OS() 
	{ 
		package = new std::queue<Task*>(); 
		trash = new std::list<Task*>(); 
	}

	~OS() 
	{	
		while (!package->empty())
		{
			delete package->front();
			package->pop();
		}

		while (!trash->empty())
		{
			delete trash->front();
			trash->pop_front();
		}

		delete package;
		delete trash;
	}

	void generatorPackage(int tasksProcess, int tasksIO, int tasksBalance)
	{
		srand(time(NULL));

		std::cout << "Tasks: "
			<< "  process: " << tasksProcess 
			<< "  IO: " << tasksIO 
			<< "  balance: " << tasksBalance 
			<< "  TOTAL: " << tasksProcess + tasksIO + tasksBalance << std::endl;

		while (tasksProcess > 0 || tasksIO > 0 || tasksBalance > 0)
		{
			int genTasksType = rand() % 4;		// [0; 5]

			switch (genTasksType)
			{
			case 0:								// tasksBalance
				if (tasksBalance > 0)
				{
					tasksBalance--;
					package->push(new Balance(5 + rand() % (10 - 5 + 1)));
				}
				break;
			case 1:								// tasksIO
				if (tasksIO > 0)
				{
					tasksIO--;
					package->push(new IntIO(1 + rand() % (3)));
				}
				break;
			default:							// tasksProcess
				if (tasksProcess > 0)
				{
					tasksProcess--;
					package->push(new Process(1 + rand() % (30)));
				}
				break;
			}
		}
	}

	void run()
	{
		while (!package->empty())
		{
			std::cout << package->front()->getInfo() << " ... ";	// ����� ���������� � ������

			int delay = package->front()->getTime();				// ����� ��������, ��� ����� ����������� ������
			Sleep(delay * 10);

			countOperations += package->front()->getOperations();	// ������� ����� ����� � ���-�� ��������
			countTime += package->front()->getTime();

			if(package->front()->getType() == TaskType::PROCESS)
				countTimeTaskProcess += package->front()->getTime();
			if (package->front()->getType() == TaskType::INT_IO)
				countTimeTaskIO += package->front()->getTime();
			if (package->front()->getType() == TaskType::BALANCE)
				countTimeTaskBalance += package->front()->getTime();

			std::cout << "Done! ";									// �������, ��� ������ ���������
			std::cout << std::to_string(countTime) << " y.e.";		// ������� ����������� ����� �� ���������� ���������� �����
			std::cout << std::endl;

			trash->push_back(package->front());						// ������ ������������ ������ � �������
			package->pop();											// ������� ������ �� �������
		}

		std::cout << "TOTAL operations: " << getCountOperationWork() << " | TOTAL time: " << getTimeWork() << " y.e." << std::endl;
		std::cout << "Task times: "
			<< "  process: " << countTimeTaskProcess << " y.e. "
			<< "  IO: " << countTimeTaskIO << " y.e. "
			<< "  Balance: " << countTimeTaskBalance << " y.e. "
			<< std::endl;
	}

	void reset()
	{
		countOperations = 0;
		countTime = 0;
		countTimeTaskProcess = 0;
		countTimeTaskIO = 0;					
		countTimeTaskBalance = 0;
	}

	int getTimeWork()
	{
		return countTime;
	}

	int getCountOperationWork()
	{
		return countOperations;
	}

	int getTimeTaskProcess()
	{
		return countTimeTaskProcess;
	}

	int getTimeTaskIO()
	{
		return countTimeTaskIO;
	}

	int getTimeTaskBalance()
	{
		return countTimeTaskBalance;
	}

};


#endif // BOS_OS.H


