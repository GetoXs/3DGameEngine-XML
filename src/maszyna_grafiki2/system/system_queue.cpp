#include "system_queue.h"



process_queue::process_queue()
{
	en = false;
	process.clear();
	this->dodano_maszyne=0;
	sigmach = NULL;
	mainsigmach=NULL;
};

void process_queue::addSigMachine(signal_machine *sig)
{
	dodano_maszyne = 1;
	this->sigmach = sig;
}


void process_queue::addSigMachine(main_signal_machine *sig)
{
	dodano_maszyne = 2;
	
	this->mainsigmach=sig;
}

process_queue::~process_queue()
{
	en = false;
	for (unsigned int i=0;i<process.size();i++)
	{
		delete process[i];
	}
	process.clear();
	if(this->dodano_maszyne==1)
		delete this->sigmach;
	if(this->dodano_maszyne==2)
		delete this->mainsigmach;
};


int process_queue::run(unsigned long *tim)
{
	if(mainsigmach!=NULL)
	{
		mainsigmach->runInterpreters(true);	//uruchamia wszystkie sygnaly
	}
	/*
	switch (this->dodano_maszyne)
	{
		// maszyna stanów
	case 0:break;
	case 1:
		this->sigmach->run(true); // tryb wszystkich komunikatów
		break;
	case 2:
		this->mainsigmach->sendMessages(0);
		break;
	}*/

	for (unsigned int i=0;i<process.size();i++)
	{
		int ret = process[i]->run();
		
	};
	return 0;
};


void process_queue::sort()
{
	for (unsigned int i=0;i<process.size()-1;i++)
	{
		
		int min = process[i]->prior;
		int el = i;
		for (unsigned int j=i+1;j<process.size();j++)
		{
			if (process[j]->prior<min)
			{
				el = j;
				min = process[j]->prior;
			}
		};
		if (min!=process[i]->prior)
		{
			basic_func*temp = process[i];
			process[i] = process[el];
			process[el] = temp;
		}
	};
};


void process_queue::add(unsigned int id,unsigned short int prior,int (*func)(unsigned long int*))
{
	basic_func * temp = new basic_func(id,prior,func);
	process.push_back( temp );
	sort();
};


void process_queue::del(unsigned int id)
{
	int i = 0;
	for (std::vector<basic_func*>::iterator p= process.begin();p<process.end();i++,p++)
	{
		if (process[i]->ID == id)
		{
			// TODO::sprawdzić czy ta funkcja działa tak jak powinna!
			delete process[i];
			process.erase(p);
			return;
		}
	}
};