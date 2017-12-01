//============================================================================
// Name        : 12_when_threads_go_wrong.cpp
// Author      : 
// Version     :
// Copyright   : Steal this code!
// Description : see numbers for potential problems
//============================================================================

#include <iostream>
#include <thread>
#include <chrono>
#include <time.h>
#include <mutex>
#include <atomic>
using namespace std;

mutex m1;					
mutex m2;		

int iGlobal = 0;

void inc1() {
	for(int i=0;i<1000;i++){
		//lock_guard<mutex> m(m1);	//4. Always prefer scoped mutexes over raw mutexes
		
		//m1.lock();
		//m1.lock();	//2. try to lock twice and you are deadlocked

//		m2.lock();		//3. locking out of order (see dec1) m2 then m1, possible deadlock
//		m1.lock();		//program hangs (in eclipse ->debug run->hit a breakpoint->then resume
						//               if it hangs, hit the pause button to see where threads are)

		iGlobal++;
		std::this_thread::sleep_for(std::chrono::microseconds(1));
//		m2.unlock();	//3. unlock in order aquired
//		m1.unlock();

		//m1.unlock();	//forget to unlock and you are deadlocked
	}
}

void dec1() {
	for(int i=0;i<1000;i++){
		//lock_guard<mutex> m(m1);

//		m1.lock();		//3. locking out of order (see inc1) m1 then m2
//		m2.lock();
		iGlobal--;

		std::this_thread::sleep_for(std::chrono::microseconds(1));

//		m1.unlock();	//3. unlock in order aquired
//		m2.unlock();
	}
}

int main() {
	thread t1(inc1);
	thread t2(inc1);
	thread t3(dec1);
	thread t4(dec1);
	
	t1.join();
	t2.join();
	t3.join();
	t4.join();
//	t4.join();		//1.  add an extra join and you crash
	
	cout << "iGlobal=" <<iGlobal<< endl;
}
