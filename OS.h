#pragma once
#ifndef BOS_OS.H
#include <queue>
#include <list>
#include <string>
#include <ctime>
#include <windows.h>

#include "Task.h"

/// <summary>
/// ѕакетный вариант ќ—
/// </summary>
class OS
{
private:
	std::queue<Task*>* package = nullptr;		// очередь из задач - пакет
	std::list<Task*>* trash = nullptr;			// корзина дл€ отработанных задач 
	const int operationsMax = 20;				// максимальное кол-во операций в задаче

	// ѕакет
	int countOperations = 0;
	int countTime = 0;

	// типы задач
	
	int countTimeTaskProcess = 0;				// вычислительна€
	int countTimeTaskIO = 0;					// ввод / вывод
	int countTimeTaskBalance = 0;				// сбалансированна€

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
			std::cout << package->front()->getInfo() << " ... ";	// вывод информации о задаче

			int delay = package->front()->getTime();				// делай задержку, как будто выполн€етс€ задача
			Sleep(delay * 10);

			countOperations += package->front()->getOperations();	// считаем общее врем€ и кол-во операций
			countTime += package->front()->getTime();

			if(package->front()->getType() == TaskType::PROCESS)
				countTimeTaskProcess += package->front()->getTime();
			if (package->front()->getType() == TaskType::INT_IO)
				countTimeTaskIO += package->front()->getTime();
			if (package->front()->getType() == TaskType::BALANCE)
				countTimeTaskBalance += package->front()->getTime();

			std::cout << "Done! ";									// говорим, что задача выполнена
			std::cout << std::to_string(countTime) << " y.e.";		// выводим затраченное врем€ на выполнени€ выполненых задач
			std::cout << std::endl;

			trash->push_back(package->front());						// кидаем отработанную задачу в корзину
			package->pop();											// убираем задачу из очереди
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


