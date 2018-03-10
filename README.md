# CS470Lab5CPUScheduling
PCB is a data structure holding information about processes. In this specific scenario we need to have a process id, a process state (create, waiting, executing, ready, terminating), a priority and a CPU burst time for each process. All other type of information is optional.
Operating Systems Lab (CS 470):
Lab 5: Given a multiprocessor system with 2 processors simulate such a computer by executing the processes assigned to each processor using different scheduling algorithms.
Overview
PCB is a data structure holding information about processes. In this specific scenario we need to have a process id, a process state (create, waiting, executing, ready, terminating), a priority and a CPU burst time for each process. All other type of information is optional.
Instructions
Each processor will have originally a certain number of processes to run. These numbers should be provided through command line arguments. For each process the CPU burst and the process id should be generated randomly. The original CPU burst time cannot be bigger than 5 seconds and should be bigger than 0. The process id should be unique. For the process state, originally each one should be set to “ready”. For the priority the initial value should be in the range [0,127], where a priority of 127 is the lowest priority. If the processes from one processor are finished (all of them were executed), that processor should take over some processes from the other processors to mimic load balancing.
Notes
 Processor 1 will implement a FCFC scheduling mechanism (time quantum 1 second).
 Processor 2 will implement a priority based scheduling (time quantum 0.5 second).
 Implement an aging mechanism which is invoked each 2 seconds.
 Implement a status mimicking mechanism which changes randomly the status of each process (ready and wait) according to the process state diagram which is invoked every 5 secs.
 The 2 processors, the aging, and the status mimicking process should run in a separate thread in parallel, synchronizing between the processes if necessary.
 The load balancing (taking over processes from other processors) takes place only when one processor is completely finished executing its processes.
 A process cannot be “executed” if not in ready state. Once the process passed the “new” state it cannot return into that state. Same with the terminate state, which will be set once the process is completely executed.
 Use wait() to mimic the “execution time”.
 Each step during the simulation should be visible on the screen.
