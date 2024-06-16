// This is the code for Operating System Scheduling Algorithms

/* This code contains 
 *
 * First Come First Serve
 * Shortest Job First
 * Round Robin
 *
*/

// The code starts from here
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstddef>
#include <cstdio>

struct process
{
    size_t id;
    unsigned int arrival;
    unsigned int burst;
    unsigned int remaining;
    unsigned int start_time;
    unsigned int end_time;
    unsigned int waiting_time;

    bool started;
    bool completed;

    // default constructor
    process ()
    {
	this->id = 0;
	this->arrival = 0;
	this->burst = 0;
	this->remaining = 0;
	this->start_time = 0;
	this->end_time = 0;
	this->waiting_time = 0;
	this->started = false;
	this->completed = false;
    }

    // construct by id
    process (size_t const id)
    {
	this->id = id;
	this->arrival = 0;
	this->burst = 0;
	this->remaining = 0;
	this->start_time = 0;
	this->end_time = 0;
	this->waiting_time = 0;
	this->started = false;
	this->completed = false;
    }

    bool const operator == (process const compare)
    {
	return (this->id == compare.id);
    }

};

struct gantt_node
{
    unsigned int process_id;
    unsigned int execution_start_time;
    unsigned int execution_end_time;

    gantt_node ()
    {
	this->process_id = 0;
	this->execution_start_time = 0;
	this->execution_end_time = 0;
    }

    gantt_node (size_t const process_id)
    {
	this->process_id = process_id;
	this->execution_start_time = 0;
	this->execution_end_time = 0;
    }
};

std::string three_spaces ()
{
    return "   ";
}

class process_vector
{
    private:
	std::vector<process> processes;
	std::vector<gantt_node> gantt_chart;

	unsigned int calculate_average_waiting_time ()
	{
	    unsigned int sum = 0;

	    for (auto i : processes)
	    {
		sum += i.waiting_time;
	    }

	    return sum/(processes.size());
	}

    public:
	process_vector ()
	{
	    processes = {};
	    gantt_chart = {};
	}

	void ask_and_set_ids ()
	{
	    size_t process_count;

	    std::cout << "Enter the number of processes: ";
	    std::cin >> process_count;

	    for (size_t i = 0; i < process_count; i++)
	    {
		processes.push_back(i);
	    }
	}

	void ask_and_set_arrival ()
	{
	    size_t process_count = processes.size();

	    for (size_t i = 0; i < process_count; i++)
	    {
		std::cout << "Enter the arrival time of process[" << i << "]: ";
		std::cin >> processes[i].arrival;
	    }
	}

	void ask_and_set_burst_and_remaining ()
	{
	    size_t process_count = processes.size();

	    for (size_t i = 0; i < process_count; i++)
	    {
		std::cout << "Enter the burst time of process[" << i << "]: ";
		std::cin >> processes[i].burst;
		processes[i].remaining = processes[i].burst;
	    }
	}

	void sort_by_arrival ()
	{
	    if (processes.empty() == true)
	    {
		return;
	    }

	    bool is_sorted = false;
	    std::size_t process_count = processes.size();

	    for (std::size_t i = 0; (i < process_count - 1) && (is_sorted == false); i++)
	    {
		is_sorted = true;
		for (std::size_t j = 0; j < process_count - 1; j++)
		{
		    if (processes[j].arrival > processes[j+1].arrival)
		    {
			is_sorted = false;
			std::swap(processes[j], processes[j+1]);
		    }
		}
	    }
	}

	void first_come_first_serve ()
	{
	    sort_by_arrival();

	    unsigned int current_time = processes[0].arrival;
	    size_t process_count = processes.size();

	    for (size_t i = 0; i < process_count; i++)
	    {
		processes[i].start_time = current_time;
		gantt_chart.push_back(processes[i].id);
		gantt_chart[i].execution_start_time = processes[i].start_time;
		current_time += processes[i].burst;
		processes[i].end_time = current_time;
		gantt_chart[i].execution_end_time = processes[i].end_time;
		processes[i].completed = true;
		processes[i].waiting_time = processes[i].start_time - processes[i].arrival;
	    }
	}

	void print ()
	{

	    std::cout << std::setw (15);
	    std::cout << std::setfill (' ');
	    std::cout << "\n\tID" << three_spaces() << "BurstTime" << three_spaces() 
		<< "ArrivalTime" << three_spaces() << "StartTime" << three_spaces()
		<< "EndTime" << three_spaces() << "WaitingTime\n";

	    for (auto i : processes)
	    {
		std::cout << '\t' << std::setw(2) << i.id << three_spaces() << std::setw(9)
		    << i.burst << three_spaces() << std::setw(11) << i.arrival << three_spaces()
		    << std::setw(9) << i.start_time << three_spaces() << std::setw(7) 
		    << i.end_time << three_spaces() << std::setw(11) << i.waiting_time << '\n';
	    }

	    std::cout << "\nGantt Chart:\n\n";
	    for (size_t i = 0; i < gantt_chart.size(); i++)
	    {
		if (i == (gantt_chart.size() - 1))
		{
		    if (gantt_chart[i].execution_start_time > gantt_chart[i].execution_end_time)
		    {
			std::cout << std::setw(3) << "WaitingTime" << std::setw(3) << "|";
			continue;
		    }

		    std::cout << std::setw(3) << '|' << std::setw(3) << "P" << std::setw(2) 
			<< gantt_chart[i].process_id << std::setw(3) << "|";
		    continue;
		}

		if (i > 0)
		{
		    if (gantt_chart[i].execution_start_time > gantt_chart[i].execution_end_time)
		    {
			std::cout << std::setw(3) << "|" << std::setw(3) << "WaitingTime";
			continue;
		    }
		}

		std::cout << std::setw(3) << "|" << std::setw(3) << "P" << std::setw(2)
		    << gantt_chart[i].process_id;
	    }
	    std::cout << '\n';
	    for (size_t i = 0; i < gantt_chart.size(); i++)
	    {
		if (i == (gantt_chart.size() - 1))
		{
		    std::cout << std::setw(8) << gantt_chart[i].execution_start_time
			<< std::setw(8) << gantt_chart[i].execution_end_time;
		    continue;
		}

		if (i == 0)
		{
		    std::cout << std::setw(3) << gantt_chart[i].execution_start_time;
		    continue;
		}

		std::cout << std::setw(8) << gantt_chart[i].execution_end_time;
		continue;
	    }

	    std::cout << "\n\nAverage Waiting Time: " << calculate_average_waiting_time() << '\n';
	}

	void increase_waiting_time (unsigned int except, unsigned int current_time, 
		unsigned int increase_by)
	{
	    for (size_t i = 0; i < processes.size(); i++)
	    {
		if ((processes[i].id == except) || (processes[i].arrival > current_time) || (processes[i].completed == true))
		{
		    continue;
		}

		processes[i].waiting_time += increase_by;
	    }
	}

	bool is_shortest_job (process & job)
	{
	    for (auto i : processes)
	    {
		if (i.id == job.id)
		{
		    continue;
		}

		if (i.completed == true)
		{
		    continue;
		}

		if (i.burst < job.burst)
		{
		    return false;
		}
	    }

	    return true;
	}

	void shortest_job_first ()
	{
	    unsigned int current_time = 0;
	    
	    for (unsigned int completed_processes = 0; completed_processes < processes.size(); )
	    {
		unsigned int current_process_time = 0;

		for (size_t i = 0; i < processes.size(); i++)
		{
		    if (processes[i].completed == true)
		    {
			continue;
		    }

		    if (processes[i].arrival >= current_time)
		    {
			continue;
		    }

		    if (is_shortest_job(processes[i]) == false)
		    {
			continue;
		    }

		    processes[i].start_time = current_time;
		    gantt_chart.push_back(processes[i].id);
		    gantt_chart[i].execution_start_time = processes[i].start_time;
		    current_time += processes[i].burst;
		    processes[i].end_time = current_time;
		    gantt_chart[i].execution_end_time = processes[i].end_time;
		    processes[i].completed = true;
		    processes[i].waiting_time = processes[i].start_time - processes[i].arrival;

		    completed_processes ++;
		    current_process_time = processes[i].burst;
		}

		if (current_process_time == 0)
		{
		    current_time ++;
		}
	    }
	}

	void round_robin ()
	{
	    unsigned int quantum = 0;
	    unsigned int current_time = 0;
	    unsigned int current_process_time = 0;
	    
	    std::cout << "Enter the quantum time: ";
	    std::cin >> quantum;
	    std::cout << '\n';

	    for (unsigned int completed_processes = 0; completed_processes < processes.size(); )
	    {
		current_process_time = 0;

		for (size_t i = 0; i < processes.size(); i++)
		{
		    if (processes[i].completed == true)
		    {
			continue;
		    }

		    if (processes[i].started == false)
		    {
			if (processes[i].arrival > current_time)
			{
			    continue;
			}

			processes[i].started = true;
			processes[i].start_time = current_time;
			processes[i].waiting_time = current_time - processes[i].arrival;
		    }

		    if (processes[i].remaining <= quantum)
		    {
			increase_waiting_time(processes[i].id, current_time, processes[i].remaining);
			current_process_time = processes[i].remaining;
			gantt_chart.push_back(processes[i].id);
			gantt_chart.back().execution_start_time = current_time;
			current_time += processes[i].remaining;
			gantt_chart.back().execution_end_time = current_time;
			processes[i].completed = true;
			processes[i].remaining = 0;
			processes[i].end_time = current_time;
			
			completed_processes ++;

			continue;
		    }

		    increase_waiting_time(processes[i].id, current_time, quantum);
		    gantt_chart.push_back(processes[i].id);
		    gantt_chart.back().execution_start_time = current_time;
		    current_process_time = quantum;
		    current_time += quantum;
		    gantt_chart.back().execution_end_time = current_time;
		    processes[i].remaining -= quantum;
		}

		if (current_process_time == 0)
		{
		    current_time ++;
		}
	    }

	}

};


int main(int argc, char *argv[])
{
	process_vector p;
	p.ask_and_set_ids();
	std::cout << '\n';
	p.ask_and_set_burst_and_remaining();
	std::cout << '\n';
	p.ask_and_set_arrival();
	std::cout << '\n';

	unsigned int algorithm = 0;
	std::cout << "Choose the algorithm to use: \n";
	std::cout << "\t1. First Come First Serve\n";
	std::cout << "\t2. Shortest Job First\n";
	std::cout << "\t3. Round Robin\n";
	std::cout << "\n\tChoice: ";
	std::cin >> algorithm;
	std::cout << '\n';

	switch (algorithm)
	{
	    case 1:
		p.first_come_first_serve();
		p.print();
		return 0;
	
	    case 2:
		p.shortest_job_first();
		p.print();
		return 0;

	    case 3:
		p.round_robin();
		p.print();
		return 0;
	}

	return 0;
}
