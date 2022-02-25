#include<iostream>
#include<vector>
#include<string>
#include<fstream>

using namespace std;

class node {
	string name;
	node* next;
	node* prev;
	vector<string> proccesses;
public:
	void setName(string);
	void setNext(node*);
	void setPrev(node*);
	void setProccesses(vector<string>);
	node* getPrev();
	node* getNext();
	vector<string> getProccesses();
	string getName();
	node(vector<string>,string);
	node();
};

string node::getName() {
	return this->name;
}

void node::setName(string n) {
	this->name = n;
}

class Queue
{
public:
	Queue();
	void enqueue(vector<string>, string);
	node* dequeue();
	int getsize();
private:
	node* rear;
	node* front;
	int size;
};

Queue::Queue() : size(0), rear(new node), front(new node) {
	rear->setNext(front);
	front->setPrev(rear);
}

node::node() : next(nullptr), prev(nullptr), name("") {}

node::node(vector<string> proccesses, string n) : next(nullptr), prev(nullptr), name(n) {
	this->proccesses = proccesses;
}

void node::setNext(node* ptr) {
	this->next = ptr;
	if (ptr != nullptr)
		ptr->prev = this;
}

void node::setPrev(node* ptr) {
	this->prev = ptr;
	ptr->next = this;
}

void node::setProccesses(vector<string> vec) {
	this->proccesses = vec;
}

void Queue::enqueue(vector<string> proccesses, string name) {
	if (this->size == 0) {
		front->setProccesses(proccesses);
		front->setName(name);
	}
	else if (this->size == 1) {
		rear->setProccesses(proccesses);
		rear->setName(name);
	}
	else {
		node* mptr = new node(proccesses,name);
		rear->setPrev(mptr);
		mptr->setNext(rear);
		rear = mptr;
	}
	this->size += 1;
}

vector<string> node::getProccesses() {
	return this->proccesses;
}

node* node::getNext() {
	return this->next;
}

node* node::getPrev() {
	return this->prev;
}

node* Queue::dequeue() {
	node* returner = nullptr;
	if (size == 2) {
		returner = new node(front->getProccesses(),front->getName());
		vector<string> empt;
		empt.resize(0);
		front->setProccesses(rear->getProccesses());
		front->setName(rear->getName());
		rear->setProccesses(empt);
		rear->setName("");
		this->size -= 1;
	}
	else if (size == 1) {
		returner = new node(front->getProccesses(), front->getName());
		vector<string> empt;
		empt.resize(0);
		front->setProccesses(empt);
		front->setName("");
		this->size -= 1;
	}
	else if (size != 0) {
		returner = new node(front->getProccesses(), front->getName());
		node* newfront = front->getPrev();
		newfront->setNext(nullptr);
		delete front;
		front = newfront;
		this->size -= 1;
	}
	return returner;
}

int Queue::getsize() {
	return this->size;
}

class MLFQ {
	int S;
	int curr;
	Queue* arr;
public:
	MLFQ();
};

MLFQ::MLFQ() {
	string folder;
	cout << "Please enter the process folder name: ";
	cin >> folder;
	cout << "When all processes are completed, you can find execution sequence in\n"
		 << "\"" << folder << "/output.txt\".";
	folder = folder + "/";
	if (folder == "/")
		folder = "";
	ofstream writer;
	writer.open(folder+"output.txt");
	ifstream reader1;
	reader1.open(folder + "configuration.txt");
	string sqn, spn, ss;
	int qn, pn, s;
	getline(reader1, sqn);
	getline(reader1, spn);
	getline(reader1, ss);
	qn = stoi(sqn);
	pn = stoi(spn);
	s = stoi(ss);
	reader1.clear();
	reader1.close();
	this->arr = new Queue[qn];
	this->S = 0;
	this->curr = 0;
	for (int i = 1; i <= pn; i++) {
		ifstream reader;
		string fname = folder + "p" + to_string(i) + ".txt";
		reader.open(fname.c_str());
		vector<string> proc;
		string line;
		while (getline(reader,line)) {
			proc.push_back(line);
		}
		string name = "PC" + to_string(i);
		this->arr[qn - 1].enqueue(proc,name);
		reader.clear();
		reader.close();
	}
	while (pn != 0) {
		node* selected = nullptr;
		int qprio = 0;
		for (int j = qn-1; j > 0;j--) {
			if (selected == nullptr && arr[j].getsize() != 0) {
				qprio = j + 1;
				selected = this->arr[j].dequeue();
			}
		}
		string pcname = selected->getName();
		vector<string> pproccesses = selected->getProccesses();
		string ptype = pproccesses[0];
		pproccesses.erase(pproccesses.begin());
		selected->setProccesses(pproccesses);
		if (pproccesses[0] == "-") {
			pn -= 1;
			writer << "E" << ", " << pcname << ", " << "QX" << "\n";
		}
		else if (ptype == "1") {
			int to;
			if (qprio - 1 < 1) {
				qprio = 2;
			}
			writer << ptype << ", " << pcname << ", Q" << to_string(qprio - 1) << "\n";
			this->arr[qprio - 2].enqueue(selected->getProccesses(),selected->getName());
		}
		else if (ptype == "0") {
			writer << ptype << ", " << pcname << ", Q" << to_string(qprio) << "\n";
			this->arr[qprio - 1].enqueue(selected->getProccesses(), selected->getName());
		}
		this->S += 1;
		if (this->S == s) {
			this->S = 0;
			for (int j = qn - 2; j >= 0; j--) {
				Queue& curr = this->arr[j];
				int size = curr.getsize();
				while (curr.getsize() != 0) {
					node* deleted = curr.dequeue();
					writer << "B, " << deleted->getName() << ", Q" << to_string(qn) << "\n";
					this->arr[qn - 1].enqueue(deleted->getProccesses(),deleted->getName());
				}
			}
		}
	}
}

int main() {
	MLFQ kafandaayivar;
}