//Class imports
#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <queue>
#include <vector>
#include <unordered_set>
#include <mutex>
#include <stack>
#include <thread>
#include<bits/stdc++.h>

using namespace std;
//global variables
static int runCounter = 0, pcbCounter = 0; //Global variables
static const int TIME_QUANTUM = 30, FILE_OFFSET = 38; //Global constants

process_queue processOne(true);
process_queue processTwo(false);
mutex thread_mutex;
bool p1;
bool p2;

// Function to sort the Process acc. to priority
bool comparison(ProcessesPCB a, ProcessesPCB b)
{
	return (a.priority > b.priority);
}
int ProcessesPCB::getPID(void)
{
	return pid;
}
int ProcessesPCB::getPriority(void)
{
	return priority;
}
int ProcessesPCB::getPlace(void) const
{
	return place;
}
time_t ProcessesPCB::getBurstTime(void)
{
	return burstTime;
} 

ProcessesPCB::ProcessesPCB(int id, int priorit, time_t burst, State state, int spt)
{
	pid = id;
	priority = priorit;
	place = spt;
	burstTime = burst;
	current_state = state;
} 
void ProcessesPCB::newState(void)
{
	if (current_state == State::New || current_state == State::Waiting || current_state == State::Ready)
	{
		if (rand() % 2)
		{
			cout << "For Process ID: " << pid << ", new State is now Waiting" << endl;
			current_state = State::Waiting;
		} 
		else
		{
			cout << "For Process ID: " <<pid<< ", new State is now Ready" << endl;
			current_state = State::Ready;
		} 
	} 
} 
bool ProcessesPCB::execute(time_t burst, int processor)
{
	if (current_state == State::Ready)
	{
		current_state = State::Executing;
		cout << "Processor number " << processor << ": Executing: " <<endl;
		cout << *this << endl;
		if (burstTime > burst)
		{
			burstTime -= burst;
			current_state = State::Ready;
		}
		else
		{
			burstTime = 0;
			current_state = State::Terminated;
		} 
		cout << "Processor number " << processor << ": Finished executing: " << endl;
		cout << *this <<endl;

		return true;
	}
	else
	{
		cout << "Next process is Processor number " << processor << ": Process " << pid << endl;
		return false;
	} 
} 
void ProcessesPCB::switchState(State newState)
{
	current_state = newState;
} 

void ProcessesPCB::age(int aging)
{
	if (priority >= aging)
	{
		std::cout << "Process ID " << pid << ": is aging from " << priority << " to " << priority - aging <<endl;
		priority -= aging;
	} 
} 


State ProcessesPCB::getState(void)
{
	return current_state;
} 

ProcessesPCB::ProcessesPCB(void)
{
	pid = 0;
	priority = 0;
	place = 0;
	burstTime = 0;
	current_state = State::Terminated;
}

ProcessesPCB::ProcessesPCB(const ProcessesPCB & copy)
{
	pid = copy.pid;
	priority = copy.priority;
	burstTime = copy.burstTime;
	current_state = copy.current_state;
	place = copy.place;
} 

ProcessesPCB& ProcessesPCB::operator=(const ProcessesPCB & equal)
{
	pid = equal.pid;
	place = equal.place;
	priority = equal.priority;
	burstTime = equal.burstTime;
	current_state = equal.current_state;

	return *this;
} 

bool ProcessesPCB::operator<(const ProcessesPCB & compare)
{
	return priority > compare.priority;
} 

void findavTime(int processes[], int n, int bt[])
{
	int wt[n], tat[n], total_wt = 0, total_tat = 0;
	//Display processes along with all details
	cout << "Processes  " << " Burst time  "
		<< " Waiting time  " << " Turn around time\n";

	// Calculate total waiting time and total turn 
	// around time
	for (int i = 0; i<n; i++)
	{
		total_wt = total_wt + wt[i];
		total_tat = total_tat + tat[i];
		cout << "   " << i + 1 << "\t\t" << bt[i] << "\t    "
			<< wt[i] << "\t\t  " << tat[i] << endl;
	}

	cout << "Average waiting time = "
		<< (float)total_wt / (float)n;
	cout << "\nAverage turn around time = "
		<< (float)total_tat / (float)n;
}


ostream& operator<<(ostream & status, ProcessesPCB& process)
{
	status << "current informations are: " << endl;
	status << "\nProcess ID is: " << process.pid;
	status << "             priority number is: " << process.priority;
	status << "\nCurrent State is: ";

	switch (process.current_state)
	{
	case State::New:
		status << "New";
		break;
	case State::Waiting:
		status << "Waiting";
		break;
	case State::Executing:
		status << "Executing";
		break;
	case State::Ready:
		status << "Ready";
		break;
	case State::Terminated:
		status << "Terminated";
		break;
	} 
	status << "      Remaining burst time is: " << process.burstTime << " ms.\n";
	return status;
} 


bool operator<(const ProcessesPCB & processOne, const ProcessesPCB & processTwo)
{
	return processOne.priority > processTwo.priority;
} 

  // Function to find the waiting time for all 
  // processes
void findWaitingTime(int processes[], int n,
	int bt[], int wt[])
{
	// waiting time for first process is 0
	wt[0] = 0;

	// calculating waiting time
	for (int i = 1; i < n; i++)
		wt[i] = bt[i - 1] + wt[i - 1];
}

// Function to calculate turn around time
void findTurnAroundTime(int processes[], int n,
	int bt[], int wt[], int tat[])
{
	// calculating turnaround time by adding
	// bt[i] + wt[i]
	for (int i = 0; i < n; i++)
		tat[i] = bt[i] + wt[i];
}

//Function to calculate average time
void findavgTime(int processes[], int n, int bt[])
{
	int wt[n], tat[n], total_wt = 0, total_tat = 0;

	//Function to find waiting time of all processes
	findWaitingTime(processes, n, bt, wt);

	//Function to find turn around time for all processes
	findTurnAroundTime(processes, n, bt, wt, tat);

	//Display processes along with all details
	cout << "Processes  " << " Burst time  "
		<< " Waiting time  " << " Turn around time\n";

	// Calculate total waiting time and total turn 
	// around time
	for (int i = 0; i<n; i++)
	{
		total_wt = total_wt + wt[i];
		total_tat = total_tat + tat[i];
		cout << "   " << i + 1 << "\t\t" << bt[i] << "\t    "
			<< wt[i] << "\t\t  " << tat[i] << endl;
	}

	cout << "Average waiting time = "
		<< (float)total_wt / (float)n;
	cout << "\nAverage turn around time = "
		<< (float)total_tat / (float)n;
}
void* ProcessOne(void* args) {

	stack<ProcessesPCB> nextProcess;
	const time_t quantum = (time_t)args;
	const int asd = 1;
	bool b = false;
	if (processOne.empty())
	{
		while (!thread_mutex.try_lock());
		p1 = true;
		thread_mutex.unlock();
	}

	if (0)
		p2 = true;
	while (!p1 || !p2)
	{
		ProcessesPCB newPCB;
		do
		{
			if (!processOne.empty())
			{
				while (!thread_mutex.try_lock());
				newPCB = processOne.top();
				processOne.pop();
				b = newPCB.execute(quantum, asd);
				if (!b)
				{
					if (!processOne.empty())
					{
						nextProcess.push(newPCB);
					}
					else if (newPCB.getState() != State::Terminated)
					{
						processOne.push(newPCB);
					}
				}
				else if (newPCB.getState() != State::Terminated)
				{
					processOne.push(newPCB);
				}
				thread_mutex.unlock();
				this_thread::sleep_for(chrono::milliseconds(500));
			}
			else
			{
				break;
			}
		} while (!b);
		while (!nextProcess.empty())
		{
			ProcessesPCB temp = nextProcess.top();
			nextProcess.pop();
			while (!thread_mutex.try_lock());
			processOne.push(temp);
			thread_mutex.unlock();
		} 

		while (!thread_mutex.try_lock());
		cout << "Processor number " << asd << ": number of Processes left: " << processOne.size() << endl;

		if (processOne.empty())
		{
			p1 = true;
		} 
		thread_mutex.unlock();
		while (p1 && !p2);
	} 
}

void* ProcessTwo(void* args)
{
	const time_t quantum = (time_t)args;
	const int asd = 2;
	bool b = false;
	stack<ProcessesPCB> onHold;
	if (0)
		p1 = true;
	if (processTwo.empty())
	{
		p2 = true;
	} 
	while (!p1 || !p2)
	{
		ProcessesPCB temp;
		do
		{
			if (!processTwo.empty())
			{
				while (!thread_mutex.try_lock());

				temp = processTwo.top();
				processTwo.pop();
				b = temp.execute(quantum, asd);
				if (!b)
				{
					if (!processTwo.empty())
					{
						onHold.push(temp);
					} 
					else if (temp.getState() != State::Terminated)
					{
						processTwo.push(temp);
					} 
				} 
				else if (temp.getState() != State::Terminated)
				{
					processTwo.push(temp);
				} 
				thread_mutex.unlock();
				this_thread::sleep_for(chrono::milliseconds(500));
			} 
			else
			{
				break;
			} 
		} while (!b);

		while (!onHold.empty())
		{
			ProcessesPCB temp = onHold.top();
			onHold.pop();
			while (!thread_mutex.try_lock());
			processTwo.push(temp);
			thread_mutex.unlock();
		} 
		while (!thread_mutex.try_lock());
		cout << "Processor number " << asd << ": number of Processes left: " << processTwo.size() << endl;
		if (processTwo.empty())
		{
			p2 = true;
		} 
		thread_mutex.unlock();
		while (!p1 && p2);
	} 
} 


void* Aging(void* args)
{
	while (!p1 || !p2)
	{
		this_thread::sleep_for(chrono::milliseconds(2000));
		while (!thread_mutex.try_lock());
		stack<ProcessesPCB> s;
		while (!processOne.empty())
		{
			ProcessesPCB temp = processOne.top();
			processOne.pop();
			temp.age();
			s.push(temp);
		} 
		while (!s.empty())
		{
			ProcessesPCB temp = s.top();
			s.pop();
			processOne.push(temp);
		} 
		while (!processTwo.empty())
		{
			ProcessesPCB temp = processTwo.top();
			processTwo.pop();
			temp.age();
			s.push(temp);
		} 
		while (!s.empty())
		{
			ProcessesPCB temp = s.top();
			s.pop();
			processTwo.push(temp);
		} 
		thread_mutex.unlock();
	} 
} 


void* stateInfo(void* args)
{
	while (!p1 || !p2)
	{
		this_thread::sleep_for(chrono::milliseconds(4000));
		while (!thread_mutex.try_lock());
		stack<ProcessesPCB> stack;
		while (!processOne.empty())
		{
			ProcessesPCB temp = processOne.top();
			processOne.pop();
			temp.newState();
			stack.push(temp);
		} 
		while (!stack.empty())
		{
			ProcessesPCB temp = stack.top();
			stack.pop();
			processOne.push(temp);
		} 
		while (!processTwo.empty())
		{
			ProcessesPCB temp = processTwo.top();
			processTwo.pop();
			temp.newState();
			stack.push(temp);
		} 
		while (!stack.empty())
		{
			ProcessesPCB temp = stack.top();
			stack.pop();
			processTwo.push(temp);
		} 
		thread_mutex.unlock();
	} 
} 

//The main method
int main(int argc, char ** argv) {
	time_t quantum1 = 1000;
	time_t quantum2 = 500;
	int i = 0, prStatus = 0; //Initialize variables for looping through PCB processes    
	srand((unsigned)time(NULL));
	//time should write like this .
	//rewind(readP); //Return to the beginning of the file
	int processOneArg;
	int processTwoArg;
	if (argc == 3)
	{
		processOneArg = atoi(argv[1]);
		processTwoArg = atoi(argv[2]);
		if (processOneArg <= 0 || processTwoArg <= 0)
		{
			cout << "error, the number of process is less or equal than 0. Please enter numbers bigger than 0";
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		cout << "error, the number of argument is not 3";
		exit(EXIT_FAILURE);
	}
	unordered_set<int> taken;
	p1= false;
	p2 = false;
	for (int j = 0; j < 2; j++)
	{
		int processID = 0,
		priority = 0;
		time_t burstTime = 0;
		State currentState = State::Ready;
		int number = (j == 0 ? processOneArg : processTwoArg);
		for (int k = 0; k < number; k++)
		{
			do
			{
				processID = rand() + 1;
			} while (taken.count(processID) != 0);
			taken.insert(processID);
			priority = rand() % 128;
			burstTime = static_cast<time_t>((rand() % 10) + 1);
			burstTime *= 500;
			currentState = State::Ready;
			ProcessesPCB pcb = ProcessesPCB(processID, priority, burstTime, currentState, k);
			j == 0 ? processOne.push(pcb) : processTwo.push(pcb);
		} 
	} 
	thread threads[4];
	threads[0] = thread(ProcessOne, (void*)quantum1);
	threads[1] = thread(ProcessTwo, (void*)quantum2);
	threads[2] = thread(Aging, (void*)NULL);
	threads[3] = thread(stateInfo, (void*)NULL);

	for (int i = 0; i < 4; i++)
	{
		threads[i].join();
	} 
	system("pause");
	return 0; 
} 
