# OSKernel
This project is a realization of a small, but fully funcitonal, kernel of a multithreaded operating system. Concepts included are:
  Time Sharing,
  Context Switching via calling dispatch, blocking, thread exceeding time limit, explicitly...,
  Concepts such as Threads, Semaphores, Events and
  Signal sending/recieving and processing.

This project is written in both C++ and assembly language(x8086). It's designed for operating on 32bit machine or virtual machine. 
All files necessary for functioning are located in .h and .src(Scheduler is provided in Scheduler folder). Main Program(function) should be provided by user. One example of that main function is situated in folder named Test.
