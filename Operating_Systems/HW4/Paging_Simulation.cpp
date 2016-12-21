#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<ctime>
#include<queue>
#define FIFO 0
#define LRU 1
#define RANDOM 2
#define SECOND_CHANCE 3
#define HIT 1
#define MISS 0
const int capacity = 100;
const int INF = 0x7fffffff;
using namespace std;

int policy, maxVP, maxPF;
int Disk[capacity], Frame[capacity];

int PF_used_time[capacity], timestamp, chance_PFN[capacity];
queue<int> Q;
set<int> S;

int VPN, PFN, EVPN;
int Dest, Src;

void initialization()
{
	memset(Disk, -1, sizeof(Disk));
	memset(Frame, -1, sizeof(Frame));
	EVPN = Dest = Src = -1;
	timestamp = 0;
	srand(time(NULL));
}

void input_trace_detail()
{
	char s[100];

	//policy
	scanf("%s%s", s, s);
	if(!strcmp(s, "FIFO")) policy = FIFO;
	if(!strcmp(s, "LRU")) policy = LRU;
	if(!strcmp(s, "Random")) policy = RANDOM;
	if(!strcmp(s, "Second-chance")) policy = SECOND_CHANCE;

	//Number of Virtual Page
	scanf("%s%s%s%s%d", s, s, s, s, &maxVP);

	//Number of Physical Frame
	scanf("%s%s%s%s%d", s, s, s, s, &maxPF);

	//-----Trace-----
	scanf("%s", s);
}

int find_replacement()
{
	for(int i = 0; i < maxPF; i++) if(Frame[i] == -1) return i;

	//Physical Frame is full, Policy start!
	int rep; //replacement

	//First In First Out
	if(policy == FIFO) {
		rep = Q.front(); Q.pop();
		return rep;
	}

	//Least Recently Used
	if(policy == LRU) {
		int min = INF;
		for(int i = 0; i < maxPF; i++) if(min > PF_used_time[i]) {
			min = PF_used_time[i];
			rep = i;
		}
		PF_used_time[rep] = INF;
		return rep;
	}

	//Random
	if(policy == RANDOM) {
		rep = rand() % maxPF;
		return rep;
	}

	//Second-chance
	if(policy == SECOND_CHANCE) {
		rep = Q.front(); Q.pop();
		while(chance_PFN[rep]) {
			chance_PFN[rep]--;
			Q.push(rep);
			rep = Q.front(); Q.pop();
		}
		return rep;
	}
}
void record_for_FIFO()
{
	if(policy != FIFO) return;
	Q.push(PFN);
}
void record_for_LRU()
{
	if(policy != LRU) return;
	PF_used_time[PFN] = timestamp++;
}
void record_for_SECOND_CHANCE()
{
	if(policy != SECOND_CHANCE) return;
	chance_PFN[PFN] = 1;
	Q.push(PFN);
}

int search()
{
	for(int i = 0; i < maxPF; i++) if(Frame[i] == VPN) {
		PFN = i;
		return HIT;
	}

	//Miss
	int rep = find_replacement();

	if(Frame[rep] != -1) {
		int i;
		for(i = 0; i < maxVP; i++) if(Disk[i] == -1) {
			Dest = i;
			EVPN = Disk[i] = Frame[rep];
			break;
		}
		for(i = 0; i < maxVP; i++) if(Disk[i] == VPN) {
			Src = i;
			Disk[i] = -1;
			break;
		}
		if(i == maxVP) Src = -1;
	}
	Frame[rep] = VPN;
	PFN = rep;
	record_for_FIFO();
	record_for_SECOND_CHANCE();
	return MISS;
}

int main()
{
	initialization();
	input_trace_detail();

	double failure = 0, success = 0;
	char ref[100];
	while(~scanf("%s%d", ref, &VPN)) {
		int status = search();
		if(status == HIT) {
			printf("Hit, %d=>%d\n", VPN, PFN);
			success++;
		}
		if(status == MISS) {
			printf("Miss, %d, %d>>%d, %d<<%d\n", PFN, EVPN, Dest, VPN, Src);
			failure++;
		}

		record_for_LRU();
	}
	printf("Page Fault Rate: %.3f", failure/(success+failure));

	return 0;
}
