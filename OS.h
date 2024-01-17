#pragma once
#ifndef BOS_OS_H
#include <vector>
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
	std::vector<Task*>* package = nullptr;		// ������� �� ����� - �����
	const int operationsMax = 20;				// ������������ ���-�� �������� � ������

	// �����
	int countOperations = 0;					// ���-�� ���������� ��������
	int countTimeWork = 0;						// ����� ������
	int countTimeWait = 0;						// �������

	// ����������� ������������� ���� ������ � %
	const int probabilityProcess = 50;
	const int probabilityIO = 25;
	const int probabilityBalance = 25;

	int tid = 101;								// task id

	const int delay = 0;

	const int quantTime = 20;					// ������������ ����� ������������ ��������� ������

public:

	OS() 
	{ 
		package = new std::vector<Task*>(); 
	}

	~OS() 
	{	
		while (!package->empty())
		{
			delete package->front();
			package->erase(package->begin());
		}

		delete package;
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

			int probabilityGeneratorTasksType = rand() % 100;	// [0; 99]
			Sleep(1);
			// task process
			if (probabilityGeneratorTasksType < probabilityProcess)
			{
				if (tasksProcess > 0)
				{
					tasksProcess--;
					package->push_back(new Process(1 + rand() % (30), tid++));
				}
			}
			// task IO
			else if (probabilityGeneratorTasksType < probabilityProcess + probabilityIO)
			{
				if (tasksIO > 0)
				{
					tasksIO--;
					package->push_back(new IntIO(1 + rand() % (4), tid++));
				}
			}
			// task balance
			else
			{
				if (tasksBalance > 0)
				{
					tasksBalance--;
					package->push_back(new Balance(5 + rand() % (10 - 5 + 1), tid++));
				}
			}
		}
	}

	void runClasic()
	{
		countOperations = 0;					// ���-�� ���������� ��������
		countTimeWork = 0;						// ����� ������
		countTimeWait = 0;						// �������

		for (auto task = package->begin(); task != package->end(); task++)
		{
			std::cout << "### Task: " << (*task)->getInfo() << std::endl;	// ����� ������ �� ������
			(*task)->setStatus(TaskPerformed);								// ������ ������ ������ �� �����������

			std::cout << "###     : ";

			while(1)
			{
				countOperations++;
				auto oper = (*task)->getCurrentOperation();					// ����� ��������

				std::cout << (*task)->getIterratorQueue() + 1 << "-P";		// ����������� ��������
				countTimeWork += (*oper).getTimeWork();
				Sleep((*oper).getTimeWork() * delay);

				if ((*oper).getType() == IO)
				{
					std::cout << ">W";										// ��������� ���� / �����
					(*task)->setStatus(TaskWaitIO);							// ������ ������ ������ �� ������� ����/�����
					countTimeWait += (*oper).getTimeWait();
					Sleep((*oper).getTimeWait() * delay);
				}

				std::cout << ">R ";											// ����������� ���������� ��������

				if ((*task)->hasNextOperation())							// ������� �����
					(*task)->nextOperation();
				else
					break;
			}
			(*task)->setStatus(TaskReady);									// ������ ������ ������ �� ���������
			std::cout << std::endl;
		}

		std::cout << "TOTAL operations: " << getCountOperationWork() << " | TOTAL time: " << getTimeWork() + getTimeWait() << " y.e." << std::endl;
		std::cout << "TOTAL work: " << getTimeWork() << " | TOTAL wait: " << getTimeWait() << " y.e." << std::endl;
		std::cout << "Wait times: " << countTimeWait << " y.e. " << std::endl;
	}

	void runQuantizationTime()
	{
		countOperations = 0;					// ���-�� ���������� ��������
		countTimeWork = 0;						// ����� ������
		countTimeWait = 0;						// �������

		int iterTask = 0;

		bool flag = false;
		int flagIter = -1;

		while (1)
		{
			//

			if (iterTask == -1)												// ��� ������ ���������
				break;

			std::cout << "### Task: " << package->at(iterTask)->getInfo() << std::endl;	// ����� ������ �� ������
			std::cout << "###     : ";

			int saveCountTimeWork = countTimeWork;
			while (countTimeWork - saveCountTimeWork < quantTime)			// ����������� ������������� ���������� ������ (����������� �� �������)
			{
				if (package->at(iterTask)->getCurrentOperation()->getStatus() == OperQueue)
				{
					flag = false;
					package->at(iterTask)->getCurrentOperation()->setStatusPerformed();
					package->at(iterTask)->setStatus(TaskPerformed);

					std::cout << " " << package->at(iterTask)->getIterratorQueue() + 1 << "-P";	// ����������� ��������
					Sleep(package->at(iterTask)->getTimeWork() * delay);
					countTimeWork += package->at(iterTask)->getCurrentOperation()->getTimeWork();

					if (package->at(iterTask)->getCurrentOperation()->getTimeWait() > 0)
					{
						std::cout << ">W";									// ��������� ���� / �����
						Sleep(package->at(iterTask)->getTimeWait() * delay);
						package->at(iterTask)->getCurrentOperation()->setStatusWaitIO();
						package->at(iterTask)->setStatus(TaskWaitIO);
						flag = true;
						flagIter = iterTask;
						break;
					}
					else
					{
						package->at(iterTask)->getCurrentOperation()->setStatusReady();
						std::cout << ">R ";								
						flag = false;
					}
				}
				else if (package->at(iterTask)->getCurrentOperation()->getStatus() == OperWaitIO)
				{
					std::cout << " " << package->at(iterTask)->getIterratorQueue() + 1 << "-W";
					if (flag == true && flagIter == iterTask)
					{
						countTimeWait += package->at(iterTask)->getCurrentOperation()->getTimeWait();
						std::cout << "(WAIT)";
						Sleep(package->at(iterTask)->getTimeWait() * delay);
					}
					package->at(iterTask)->getCurrentOperation()->setStatusReady();
					std::cout << ">R ";
					Sleep(package->at(iterTask)->getTimeWork() * delay);
					flag = false;
				}
				else
					std::cout << "ERROR!! \n";
				countOperations++;

				if (package->at(iterTask)->hasNextOperation() == false)
					package->at(iterTask)->setStatus(TaskReady);
				else
					package->at(iterTask)->nextOperation();


				if (package->at(iterTask)->getStatus() == TaskReady)
					break;
				else
					package->at(iterTask)->setStatus(TaskQueue);
			}
			iterTask = nextUncompletedTask(iterTask);						// �������� ������ ��������� �� ���������� ������
			std::cout << std::endl;
		}

		std::cout << "TOTAL operations: " << getCountOperationWork() << " | TOTAL time: " << getTimeWork() + getTimeWait() << " y.e." << std::endl;
		std::cout << "TOTAL work:       " << getTimeWork() << " | TOTAL wait: " << getTimeWait() << " y.e." << std::endl;
		std::cout << "Wait times:       " << countTimeWait << " y.e. " << std::endl;
	}


	void reset()
	{
		for (auto task = package->begin(); task != package->end(); task++)
		{
			(*task)->setStatus(TaskQueue);
			(*task)->startPosIterrator();
			
			while (1)
			{
				(*task)->getCurrentOperation()->setStatusQueue();

				if ((*task)->hasNextOperation())							// ������� �����
					(*task)->nextOperation();
				else
					break;
			}

			(*task)->setStatus(TaskQueue);
			(*task)->startPosIterrator();
		}
	}

	int getTimeWork()
	{
		return countTimeWork;
	}

	int getTimeWait()
	{
		return countTimeWait;
	}

	int getCountOperationWork()
	{
		return countOperations;
	}

	int nextUncompletedTask(int iterratorTask)					// roundrobin (��������� ������)
	{
		int curr = iterratorTask;

		do
		{
			if (curr + 1 >= package->size())			// ����� �� ����� ������, ������� � ������
				curr = 0;
			else
				curr++;

			if (package->at(curr)->getStatus() != TaskReady)	// ���� ����� ��������� �� ���������� ������, ���������� �� ������
				return curr;
			
		} while (curr != iterratorTask);						// ������ ���� -> ������� �� �����

		return -1;
	}

};


#endif // BOS_OS_H


