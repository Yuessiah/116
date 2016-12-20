#include<cstdio>
#include<cstring>
#include<iostream>
#include<set>
#include<queue>
#define FIFO 0
#define LRU 1
#define RANDOM 2
#define HIT 3
#define MISS 4
const int capacity = 100;
using namespace std;

int policy, maxVP, maxPF;
int Disk[capacity], Frame[capacity];
queue<int> q;
int VPN, PFN, EVPN;
int Dest, Src;

void initialization()
{
	memset(Disk, -1, sizeof(Disk));
	memset(Frame, -1, sizeof(Frame));
	EVPN = Dest = Src = -1;
}

void input_trace_detail()
{
	char s[100];

	//policy
	scanf("%s%s", s, s);
	if(!strcmp(s, "FIFO")) policy = FIFO;
	if(!strcmp(s, "LRU")) policy = LRU;
	if(!strcmp(s, "RANDOM")) policy = RANDOM;

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
	int rep = q.front(); q.pop();
	return rep;
}
void record_for_FIFO()
{
	if(policy != FIFO) return;
	q.push(PFN);
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

		//record_for_LRU();
	}
	printf("Page Fault Rate: %.3f", failure/(success+failure));

	return 0;
}
