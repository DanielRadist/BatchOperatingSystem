/*
* Реализовать модель системы, функционирующей в пакетном режиме, использующей для организации мультизадачности 
* принцип классического мультипрограммирования. в моделируемом пакете заданий, поступающих на вход системы, 
* предполагается наличие заданий с различными характеристиками - чисто вычислительных, с большим количеством 
* операций ввода/вывода, сбалансированных. Система функционирует определенный период времени. 
* Для данного периода времени определить производительность системы, оборотное время, простои процессора. 
* В процессе моделирования выполнить несколько сеансов, меняя соотношение различных типов задач. 
* Построить графики зависимости выходных параметров модели в зависимости от соотношения типа решаемых задач.
*/


#include <iostream>
#include "Task.h"
#include "OS.h"

int main()
{
    OS* os = new OS();

    std::cout << "###################################################################################" << std::endl;

    int countTaskProcess = 1;
    int countTaskIO = 4;
    int countTaskBalance = 15;
    int totalTasks = countTaskProcess + countTaskIO + countTaskBalance;

    os->generatorPackage(countTaskProcess, countTaskIO, countTaskBalance);
    os->reset();
    os->run();

    std::cout << "     Tasks / 100y.e. : " << (totalTasks * 100.0) / os->getTimeWork() << std::endl;
    std::cout << "Operations / 100y.e. : " << (os->getCountOperationWork() * 100.0) / os->getTimeWork() << std::endl;

    std::cout << "      100y.e. / Task : " << os->getTimeWork() / (totalTasks * 100.0) << std::endl;
    std::cout << "100y.e. / Operations : " << os->getTimeWork() / (os->getCountOperationWork() * 100.0) << std::endl;

    std::cout << "###################################################################################" << std::endl;

   

    delete os;
    //std::cout << "Hello World!\n";
}