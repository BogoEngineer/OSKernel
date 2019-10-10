#include "System.h"
#include "Lists.h"
#include "PCB.h"
#include "Thread.h"
#include "Timer.h"
#include "test.h"
#include <dos.h>


#include <iostream.h>

extern int userMain (int argc, char* argv[]);
int ret;

int main(int argc, char* argv[]){
    cout<<"Pokrenuo se main"<<endl;
    System::initialize();
    ret = userMain(argc, argv);
    System::terminate();
    cout<<"Zavrsio se main: "<<System::running_threads<<endl;
    return ret;
}
