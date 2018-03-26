#include "sa2a.h"
#include <thread>
using std::thread;
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <string>
using std::stoi;
using std::string;
#include <queue>
using std::queue;
#include <chrono>
using std::chrono::seconds;
#include <mutex>
using std::mutex;



bool inputFinished = false;
queue<int> inputNums;
mutex inputLock;
mutex outputLock;
queue<int> outputNums;
int numThreadsDone = 0;

void threadfunc() {
	while (true) {
		if (inputFinished) {
			inputLock.lock();
			if (inputNums.empty() || inputNums.front() == 0) {
				++numThreadsDone;
				inputLock.unlock();
				break;
			}

			int inputNum = inputNums.front();
			inputNums.pop();
			inputLock.unlock();

			int outputNum = sa2a(inputNum);
			
			outputLock.lock();
			outputNums.push(inputNum);
			outputNums.push(outputNum);
			outputLock.unlock();
		}
		else
			std::this_thread::sleep_for(seconds(1));
	}
}

int main() {
	
	vector<thread> workers(6);
	for (auto i = 0; i < 6; ++i) {
		workers[i] = thread(threadfunc);
	}
	
	string inputString;

	
	do {
		cout << "Enter a positive integer (or 0 to end input): ";
		std::getline(cin, inputString);
		inputNums.push(stoi(inputString));
	} while (inputNums.back() > 0);
	inputFinished = true;
	

	bool MaybeDone = false;

	while (true) {
		if (!outputNums.empty()) {
			outputLock.lock();
			cout << "sa2a(" << outputNums.front();
			outputNums.pop();
			cout << ") = " << outputNums.front() << endl;
			outputNums.pop();
			outputLock.unlock();
		}
		else if (numThreadsDone == 6)
			break;
	}

	
	//Ensure all threads are done before exiting.
	for (int index = 0; index < 6; index++) {
		workers[index].join();
	}

	// Wait for user
	cout << "Press ENTER to quit ";
	while (cin.get() != '\n');

	return 0;

}