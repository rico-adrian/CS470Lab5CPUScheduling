#include <iostream>
#include <string>
#include <stdexcept>
#include <time.h>
#include <iostream>
#include <algorithm>
#include <vector>
class process_queue;
class ProcessesPCB;

enum State
{
	New, Waiting, Ready, Executing, Terminated
};
class ProcessesPCB
{
public:
	int getPID(void);
	int	getPriority(void);
	int getPlace(void) const;
	time_t getBurstTime(void);
	State getState(void);
	bool execute(time_t, int);
	void switchState(State);
	void age(int ageBy = 1);
	void newState(void);
	ProcessesPCB(void);
	ProcessesPCB(int, int, time_t, State, int spot = 0);
	ProcessesPCB(const ProcessesPCB&);
	ProcessesPCB& operator=(const ProcessesPCB& rhs);
	bool operator<(const ProcessesPCB& rhs);
	friend std::ostream& operator<<(std::ostream&, ProcessesPCB&);
	friend bool operator<(const ProcessesPCB& lhs, const ProcessesPCB& rhs);
	int pid;  // Process ID
	int priority; // Priority of this process
	int place;
	time_t burstTime;
	State current_state;
	struct Compare
	{
		bool operator()(const ProcessesPCB& process1, const ProcessesPCB& process2) const
		{
			bool b = process1.getPlace() > process2.getPlace();
			return b;
		} 
	};
};
class process
{
public:
	process(const std::string &name, const int burst_time = 0, const int wait_time = 0);
	std::vector<ProcessesPCB> Processes;
	std::string name() const;
	int burst_time() const;
	int wait_time() const;
	const process* next() const;
	process* next();

	void decrement_burst_time(const int time);
	
private:
	int burst_time_;
	int wait_time_;
	std::string name_;
	process *next_;

	friend  std::ostream& operator<< (std::ostream &os, const process &proc);
	friend class process_queue;
};

process::process(const std::string &name, const int burst_time, const int wait_time)
	: name_(name), burst_time_(burst_time), wait_time_(wait_time), next_(NULL)
{
}

std::string process::name() const
{
	return name_;
}


int process::burst_time() const
{
	return burst_time_;
}


int process::wait_time() const
{
	return wait_time_;
}

const process* process::next() const
{
	return next_;
}

process* process::next()
{
	return next_;
}

void process::decrement_burst_time(const int time)
{
	burst_time_ -= time;
}

std::ostream & operator<< (std::ostream &os, const process &proc)
{
	os << "{Name = " << proc.name()
		<< ", Wait time = " << proc.wait_time()
		<< ", Burst time = " << proc.burst_time()
		<< ", Next = ";

	if (proc.next() != NULL) {
		os << proc.next()->name();
	}

	else {
		os << "None";
	}

	os << "}";

	return os;
}

class process_queue
{

	struct Compare
	{
		bool operator()(const ProcessesPCB& lhs, const ProcessesPCB& rhs) const
		{
			bool b = lhs.getPlace() > rhs.getPlace();
			return b;
		} 
	};
public:
	process_queue();
	~process_queue();
	std::vector<ProcessesPCB> v;
	bool priority;
	void add_node(const process &proc);
	void increment_all_wait_times(const int wait);

	friend std::ostream & operator<< (std::ostream &os, const process_queue &pq);
     process_queue(bool order)
	{
		priority = order;
	} 
	 void pop(void)
	 {
		 if (!v.empty())
		 {
			 v.pop_back();
		 } 
	 } 
	 void push(ProcessesPCB& pcb)
	 {
		 v.insert(v.begin(), pcb);
		 if (priority)
		 {
			 std::sort(v.begin(), v.end(), std::less<ProcessesPCB>());
		 } 
		 else
		 {
			 std::sort(v.begin(), v.end(), Compare());
		 } 
		
	 } 
	 ProcessesPCB& top(void) const
	 {
		 if (!v.empty())
		 {
			 ProcessesPCB temp = v.at(v.size() - 1);
		 } 
	 } 
	 int size(void) const
	 {
		 return v.size();
	 }

	 bool empty(void)
	 {
		 return v.empty();
	 } 
private:
	size_t size_;
	process *head_;
	process *tail_; // for O(1) insertions
};

process_queue::process_queue() : head_(NULL), tail_(NULL), size_(0)
{
}


void process_queue::add_node(const process &proc)
{
	process *temp = new process(proc);

	if (tail_ != NULL) {
		tail_->next_ = temp;

		if (head_ == tail_) {
			head_->next_ = temp;
		}

		tail_ = tail_->next_;
	}

	else {
		tail_ = temp;
		head_ = tail_;
	}

	++size_;
}

void process_queue::increment_all_wait_times(const int wait)
{
	process *temp = head_;

	while (temp != NULL) {
		temp->wait_time_ += wait;
		temp = temp->next_;
	}
}




std::ostream & operator<< (std::ostream &os, const process_queue &pq)
{
	process *temp = pq.head_;

	while (temp != NULL) {
		os << *temp << "\n";
		temp = temp->next();
	}

	return os;
}

process_queue::~process_queue()
{
	while (size_ > 0) {
		this->pop();
	}
}



