#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>

using namespace std;

void print(double response, double turnaround, double wait) {
	cout << "Avg. Resp.: " << response << ", Avg. T.A.: " << turnaround << ", Avg. Wait: " << wait << endl;
}

/*Each function will accept an array of 2 arrays, first being the arrival times, second being the burst times */
void FCFS(const vector<int>& arrivalTimes, const vector<int>& burstTimes){
	double avg[3];
	int processCount = arrivalTimes.size();
	double waitTime;
	double currTime;

	//Printing out what kind of scheduling is being used
	cout << "First Come, First Served" << endl;

	//Average Response Time && Wait Time
	waitTime = 0;
	currTime = arrivalTimes[0];
	for (int i = 0; i < processCount; i++){

		if (arrivalTimes[i] <= currTime) {
			waitTime += (currTime - arrivalTimes[i]);
			currTime += burstTimes[i];
		}
		else {
			currTime = arrivalTimes[i];
			currTime += burstTimes[i];
		}
	}
	avg[0] = waitTime / processCount;
	avg[2] = avg[0];

	//Average TurnAround Time
	waitTime = 0;
	currTime = arrivalTimes[0];
	for (int i = 0; i < processCount; i++){

		if (arrivalTimes[i] <= currTime){
			currTime += burstTimes[i];
		}
		else{
			currTime = arrivalTimes[i];
			currTime += burstTimes[i];
		}
		waitTime += (currTime - arrivalTimes[i]);
	}
	avg[1] = waitTime / processCount;

	//Print out the results
	print(avg[0], avg[1], avg[2]);
}

void SJF(const vector<int>& arrivalTimes, const vector<int>& burstTimes){
	double toPrint[3];
	vector<int> startOrder;
	int processCount = arrivalTimes.size();
	double waitTime = 0;
	double currTime = 0;
	int smallBurst = 0;

	//Printing out what kind of scheduling is being used
	cout << "Sortest Job First" << endl;

	//Need to put the first element into the vector startOrder, need to look through all of the arrival times, if the arrival times are the same, put the one with the shortest burst time in first
	for (int k = 0; k < processCount; k++) {
		if (arrivalTimes[k] < arrivalTimes[smallBurst]){
			smallBurst = k;
		}
		else if (arrivalTimes[k] == arrivalTimes[smallBurst]){
			if (burstTimes[k] < burstTimes[smallBurst]){
				smallBurst = k;
			}
		}
	}
	currTime += arrivalTimes[smallBurst] + burstTimes[smallBurst];
	startOrder.push_back(smallBurst);
	//Set smallBurst to a value that determines it has not been filled yet
	smallBurst = -1;
	//We need to make an array of the processes in the order they will be executed
	// 'i 'will be the position that we put the number in start vector
	for (int i = 0; i < processCount - 1 ; i++) {
		// 'k' is going to be the number that we put in the start vector
		for (int k = 0; k < processCount; k++) {
			//This checks to see if 'k' is already in the vector
			auto it = find(startOrder.begin(), startOrder.end(), k);
			if (it != startOrder.end())
				continue;

			//If 'k' has arrived
			if (arrivalTimes[k] < currTime){
				//if Small Burst has not been filled yet, put k as smallBurst
				if (smallBurst == -1){
					smallBurst = k;
					continue;
				}
				// if the burst time of 'k' is less than the current smallBurst replace SmallBurst
				if (burstTimes[k] < burstTimes[smallBurst]){
					smallBurst = k;
				}
			}
		}
		// this will be entered if no process has an arrival time < currTime
		if (smallBurst == -1){
			for (int k = 0; k < processCount; k++) {
				//This checks to see if 'k' is already in the vector
				auto it = find(startOrder.begin(), startOrder.end(), k);
				if (it != startOrder.end())
					continue;
				if (smallBurst == -1){
					smallBurst = k;
					continue;
				}
				if (arrivalTimes[k] < arrivalTimes[smallBurst]){
					smallBurst = k;
				}
				else if (arrivalTimes[k] == arrivalTimes[smallBurst]){
					if (burstTimes[k] < burstTimes[smallBurst]){
						smallBurst = k;
					}
				}
			}
		}
		if (currTime < arrivalTimes[smallBurst]){
			currTime = arrivalTimes[smallBurst];
		}
		currTime += burstTimes[smallBurst];
		startOrder.push_back(smallBurst);
		smallBurst = -1;
	}

	//Average Response Time
	waitTime = 0;
	currTime = arrivalTimes[startOrder[0]];
	for (int i = 0; i < processCount; i++){

		if (arrivalTimes[startOrder[i]] <= currTime) {
			waitTime += (currTime - arrivalTimes[startOrder[i]]);
			currTime += burstTimes[startOrder[i]];
		}
		else {
			currTime = arrivalTimes[startOrder[i]];
			currTime += burstTimes[startOrder[i]];
		}
	}
	toPrint[0] = waitTime / processCount;
	toPrint[2] = toPrint[0];

	//Average TurnAround Time
	waitTime = 0;
	currTime = arrivalTimes[startOrder[0]];
	for (int i = 0; i < processCount; i++){

		if (arrivalTimes[startOrder[i]] <= currTime){
			currTime += burstTimes[startOrder[i]];
		}
		else{
			currTime = arrivalTimes[startOrder[i]];
			currTime += burstTimes[startOrder[i]];
		}
		waitTime += (currTime - arrivalTimes[startOrder[i]]);
	}
	toPrint[1] = waitTime / processCount;

	//Print out the results
	print(toPrint[0], toPrint[1], toPrint[2]);
	return;
}

void SRTF(const vector<int>& arrivalTimes, const vector<int>& burstTimes){

	vector<int>at = arrivalTimes;
	vector<int>bt = burstTimes;
	vector<int>rt = burstTimes;
	int processCount = at.size();
	int avg[3] = { 0, 0, 0 };
	int currTime = 0;
	int cIndex = 0;
	int nIndex = 0;
	int counter = 0;
	int changeTime = 0;
	int smallRemain = 0;
	bool check = true;
	vector<int>::iterator current;
	vector<int>::iterator next;

	current = min_element(at.begin(), at.end());
	currTime = *current;
	while(check){
		cIndex = distance(at.begin(), current);
		if (*current != -1){
			avg[0] += abs(currTime - arrivalTimes[cIndex]);
		}
		*current = -1;
		smallRemain = -1;
		counter = 0;

		//Find the next one that is going to be processed
		next = upper_bound(at.begin(), at.end(), 0);
		if (next == at.end()){
			if (upper_bound(rt.begin(), rt.end(), 0) == rt.end()){
				check = false;
				break;
			}
			else{
				for (int i = 0; i < processCount; i++) {
					if (smallRemain == -1 && rt[i] > 0)
						smallRemain = i;
					else if (rt[i] > 0 && rt[i] < rt[nIndex])
						smallRemain = i;
				}
				if (smallRemain != -1){
					next = at.begin() + smallRemain;
				}
			}
		}
		else{
			if (currTime + rt[cIndex] <= *next){
				for (int i = 0; i < processCount; i++){
					if (at[i] == -1 && smallRemain == -1 && rt[i] > 0)
						smallRemain = i;
					else if (at[i] == -1 && rt[i] > 0 && rt[i] < smallRemain)
						smallRemain = i;
				}
				if (smallRemain != -1){
					next = at.begin() + smallRemain;
				}
			}
		}
		//we've found the next one we're going to process Finding the Index for it
		nIndex = distance(at.begin(), next);


		//We now need to find out how far the curr will process
		if (*next == -1 || *next >= currTime + rt[cIndex]){
			changeTime = rt[cIndex];
			currTime += changeTime;
		}
		else if (*next < currTime + rt[cIndex] && *next != -1){
			changeTime = abs(currTime - at[nIndex]);
			currTime += changeTime;
		}
		else{
			changeTime = rt[cIndex];
			currTime = at[nIndex];
		}

		

		if (changeTime - rt[cIndex] == 0)
			avg[1] += abs(arrivalTimes[cIndex] - currTime);

		for (int i = 0; i < processCount; i++){
			if (at[i] == -1 && rt[i] > 0 && i != cIndex)
				counter++;
		}
		avg[2] += changeTime * counter;

		rt[cIndex] -= changeTime;
		cout << cIndex << "  " << *next << "  " << avg[0] << "  " << avg[1] << "  " << avg[2] << endl;
		current = next;
		
	}
	avg[0] /= processCount;
	avg[1] /= processCount;
	avg[2] /= processCount;
	//Print out the results
	print(avg[0], avg[1], avg[2]);
}

void RR(const vector<int>& arrivalTimes, const vector<int>& burstTimes){

	return;
}

int main(int argc, char *argv[]) {

	cout << "\nAssignment 5 by Zachary Price\n\n";
	//cout << "This program will work correctly only with a list of programs sorted by their Arrival Time\n" << endl;

	int arrival;
	vector<int> arrivalTimes;

	int burst;
	vector<int> burstTimes;

	while (cin >> arrival >> burst)
	{
		arrivalTimes.push_back(arrival);
		burstTimes.push_back(burst);
	}

	//FCFS(arrivalTimes, burstTimes);
	//SJF(arrivalTimes, burstTimes);
	SRTF(arrivalTimes, burstTimes);
	//RR(arrivalTimes, burstTimes);

	system("PAUSE");
	return 0;
}

