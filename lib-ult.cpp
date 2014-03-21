#include <queue>

//Source:
//http://stackoverflow.com/questions/19535644/how-to-use-the-priority-queue-stl-for-objects
struct CompareUThread{
	bool operator()(UThread const & t1, UThread const & t2) {
        // return "true" if "p1" is ordered before "p2", for example:
        return t1.get_priority() > p2.get_priorityI();
    }
};

void system_init(){
	priority_queue<UThread, vector<UThread>, CompareUThread> pq;
}