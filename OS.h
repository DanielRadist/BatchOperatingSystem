#pragma once
#ifndef BOS_OS_H
#include <vector>
#include <list>
#include <string>
#include <ctime>
#include <windows.h>

#include "Task.h"

/// <summary>
/// Пакетный вариант ОС
/// </summary>
class OS
{
private:
	std::vector<Task*>* package = nullptr;		// очередь из задач - пакет
	const int operationsMax = 20;				// максимальное кол-во операций в задаче

	// Пакет
	int countOperations = 0;					// кол-во выполненых операций
	int countTimeWork = 0;						// время работы
	int countTimeWait = 0;						// простой

	// вероятность генерирования типа задачи в %
	const int probabilityProcess = 50;
	const int probabilityIO = 25;
	const int probabilityBalance = 25;

	int tid = 101;								// task id

	const int delay = 0;

	const int quantTime = 20;					// максимальное время безпрервного выполения задачи

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
		countOperations = 0;					// кол-во выполненых операций
		countTimeWork = 0;						// время работы
		countTimeWait = 0;						// простой

		for (auto task = package->begin(); task != package->end(); task++)
		{
			std::cout << "### Task: " << (*task)->getInfo() << std::endl;	// берем задачу из пакета
			(*task)->setStatus(TaskPerformed);								// меняем статус задачи на выполняется

			std::cout << "###     : ";

			while(1)
			{
				countOperations++;
				auto oper = (*task)->getCurrentOperation();					// берем операцию

				std::cout << (*task)->getIterratorQueue() + 1 << "-P";		// выполняется операция
				countTimeWork += (*oper).getTimeWork();
				Sleep((*oper).getTimeWork() * delay);

				if ((*oper).getType() == IO)
				{
					std::cout << ">W";										// ожидается ввод / вывод
					(*task)->setStatus(TaskWaitIO);							// меняем статус задачи на ожидает ввод/вывод
					countTimeWait += (*oper).getTimeWait();
					Sleep((*oper).getTimeWait() * delay);
				}

				std::cout << ">R ";											// завершилось выполнение операции

				if ((*task)->hasNextOperation())							// останов цикла
					(*task)->nextOperation();
				else
					break;
			}
			(*task)->setStatus(TaskReady);									// меняем статус задачи на выполнена
			std::cout << std::endl;
		}

		std::cout << "TOTAL operations: " << getCountOperationWork() << " | TOTAL time: " << getTimeWork() + getTimeWait() << " y.e." << std::endl;
		std::cout << "TOTAL work: " << getTimeWork() << " | TOTAL wait: " << getTimeWait() << " y.e." << std::endl;
		std::cout << "Wait times: " << countTimeWait << " y.e. " << std::endl;
	}

	void runQuantizationTime()
	{
		countOperations = 0;					// кол-во выполненых операций
		countTimeWork = 0;						// время работы
		countTimeWait = 0;						// простой

		int iterTask = 0;

		bool flag = false;
		int flagIter = -1;

		while (1)
		{
			//

			if (iterTask == -1)												// все задачи выполнили
				break;

			std::cout << "### Task: " << package->at(iterTask)->getInfo() << std::endl;	// берем задачу из пакета
			std::cout << "###     : ";

			int saveCountTimeWork = countTimeWork;
			while (countTimeWork - saveCountTimeWork < quantTime)			// ограничение беспрерывного выполнения задачи (квантование по времени)
			{
				if (package->at(iterTask)->getCurrentOperation()->getStatus() == OperQueue)
				{
					flag = false;
					package->at(iterTask)->getCurrentOperation()->setStatusPerformed();
					package->at(iterTask)->setStatus(TaskPerformed);

					std::cout << " " << package->at(iterTask)->getIterratorQueue() + 1 << "-P";	// выполняется операция
					Sleep(package->at(iterTask)->getTimeWork() * delay);
					countTimeWork += package->at(iterTask)->getCurrentOperation()->getTimeWork();

					if (package->at(iterTask)->getCurrentOperation()->getTimeWait() > 0)
					{
						std::cout << ">W";									// ожидается ввод / вывод
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
			iterTask = nextUncompletedTask(iterTask);						// получаем индекс следующей не выполненой задачи
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

				if ((*task)->hasNextOperation())							// останов цикла
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

	int nextUncompletedTask(int iterratorTask)					// roundrobin (кольцевой список)
	{
		int curr = iterratorTask;

		do
		{
			if (curr + 1 >= package->size())			// дошли до конца списка, перешли в начало
				curr = 0;
			else
				curr++;

			if (package->at(curr)->getStatus() != TaskReady)	// если нашли следующую не выполненую задачу, возвращаем ее индекс
				return curr;
			
		} while (curr != iterratorTask);						// прошли круг -> выходим из цикла

		return -1;
	}

};


#endif // BOS_OS_H


