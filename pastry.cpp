/*

* File Name : pastry.cpp

* Creation Date : 09-02-2013

* Last Modified : Tue 12 Feb 2013 01:34:30 AM IST

* Created By : ShehbazJaffer <shehbazjaffer007@gmail.com> 

_._._._._._._._._._._._._._._._._._._._._.*/

#include<iostream>
#include<vector>
#include<sstream>
#include<queue>
#include<stack>
#include<cstdio>
#include<cmath>
#include<cstring>
#include<stdint.h>
#include<cstdlib>
#include<map>
#include<algorithm>


#define DEBUG 
#define ll long long
#define sz(a) (int(a.size())

#define vi vector<int> 
#define vvi vector<vi>
#define f(i,l,u) for(i=l;i<u;i++)
#define s(n) scanf("%d", &n)
#define sll(n) scanf("%lld", &n)
#define sf(n) scanf("%lf",&n)
#define p(n) printf("%d",n)
#define pn(n) printf("%d\n",n)
#define pll(n) printf("%lld",n)
#define plln(n) printf("%lld\n",n)
#define pf(n) printf("%f",n)
#define pfl(n) printf("%f\n",n)

#define pv(X) for(IntIter=X.begin();IntIter!=X.end();IntIter++) \
						{ cout << *IntIter << " " ;} ; \
					cout << endl; 

#define pvl(X) for(LLIter=X.begin();LLIter!=X.end();LLIter++) \
						{ cout << *LLIter << " " ;} ; \
					cout << endl; 


//#define modval 10000
#ifdef DEBUG
#define DPRINTF(fmt, ...) \
    do { printf(fmt, ## __VA_ARGS__); } while (0)
    #else
    #define DPRINTF(fmt, ...) \
        do { } while (0)
        #endif

using namespace std;
vector <int> :: iterator IntIter;

#define b 4 // base value
#define N 10000 // number of nodes
#define ROW int(ceil(log(N) / log(pow(2,b)))) // number of ROW Values
#define COL int(pow(2,b)) // number of Column Values
#define L 16
#define M 32

bool *availableNodeId;
//map <int , int> mapIpId;
int *mapIpId;

typedef class node{
/*member data*/
	int nodeId;
	uint8_t ipAddress[4];
	int *leafSet;
	int *neighborhoodSet;
	int **routingTable;
/*member functions*/
	void getNextIp();
	void getNextNodeId();
	void initLeafSet();
	void initRoutingTable();
public:
	void initNode();
	void initNeighborhoodSet();
	void updateLeafSet(){};
	void updateNeighborhoodSet(){};
	void updateRoutingTable(){};
	void printNode(); 
	
//	int route(int node1, int node2, char msg[100]);
}node;
/*
int node :: route(int nodeId1,int nodeId2,char msg[100]){
		
	return 0;
}
*/
void node :: printNode(){
	int i,j;
	printf("\t\t\tNodeid = %x\n", nodeId);
	printf("\t\t\tIPAdd = %d.%d.%d.%d\n",ipAddress[3],ipAddress[2],ipAddress[1],ipAddress[0] );
	
	printf("\t\t\tLeaf Set\n");
	f(i,0,L/2)
		printf("%x\t",leafSet[i]);
	printf("\n");
	f(i,L/2,L)
		printf("%x\t",leafSet[i]);
	printf("\n");
	printf("\t\t\tNeighborhood Set\n");
	f(i,0,M/2)
		printf("%x\t",neighborhoodSet[i]);
	printf("\n");
	f(i,M/2,M)
		printf("%x\t",neighborhoodSet[i]);
	printf("\n");
	printf("\t\t\tRouting Table\n");
	f(i,0,ROW){
		f(j,0,COL){
			printf("%x\t",routingTable[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void node :: getNextIp(){
	static int ip = 0;
	ip++;
	ipAddress[0] = ip % 256;
	ipAddress[1] = (ip / 256) % 256;
	ipAddress[2] = (ip / (256 * 256) ) % 256;
	ipAddress[3] = (ip / (256 * 256 * 256)) % 256;
	return;
}

void node :: getNextNodeId(){
int ip = ipAddress[0] + (ipAddress[1] * 256) + (ipAddress[2] * 256 * 256) + (ipAddress[3] * 256 * 256 * 256);
	int nodeId = random() % N;
	if(availableNodeId[nodeId]==false){
		while(availableNodeId[nodeId]==false){
			nodeId = (nodeId +1 ) % N;
		}
	}
	availableNodeId[nodeId]=false;
	mapIpId[ip] = nodeId;	
	this->nodeId = nodeId;
}

void node :: initLeafSet(){
	int i;
	leafSet = new int [L];
	for(i=L/2;i>0;i--){
		leafSet[i-1] = (nodeId - (L/2 - i + 1) + N) % N ;
		leafSet[L/2+i-1] = (nodeId + i) % N;
	}
}

void node :: initNeighborhoodSet(){
	int i;
	int ip = ipAddress[0] + (ipAddress[1] * 256) + (ipAddress[2] * 256 * 256) + (ipAddress[3] * 256 * 256 * 256);
	neighborhoodSet = new int [M];
	for(i=M/2;i>0;i--){
		neighborhoodSet[i-1] = mapIpId[(ip-(M/2-i+1)+N)%N];
		neighborhoodSet[M/2+i-1] = mapIpId[(ip+i)%N];
	}
}

void node :: initRoutingTable(){
	int x,i,j;
	int mask,allOnes=0xffff0fff,sum;
	routingTable = new int *[ROW];
	f(i,0,ROW){
		routingTable[i] = new int [COL];
	}
	f(i,0,ROW)
		f(j,0,COL){
			mask = allOnes >> (i*4);
			mask = mask & 0x0000ffff;
			sum = (nodeId & mask ) + (j * pow((pow(2,b)),(ROW-i-1))) ;
			routingTable[i][j] = sum;
		}
}

void node :: initNode(){
	int i;
	getNextIp();
	getNextNodeId();
	initLeafSet();
	//initNeighborhoodSet();
	initRoutingTable();
}

int menu(){
	int choice;
	printf("\n1. Send Message\n");
	printf("\n2. Add Node\n");
	printf("\n3. Delete Node\n");
	printf("\n4. Print Node\n");
	printf("\n5. Exit\n");
	scanf("%d",&choice);
	return choice;
}

int 
main(int argc, char *argv[])
{
	int choice,i,j,nodeId1,nodeId2;
	char msg[100];
	node *nodeObject = new node [N];
	availableNodeId = new bool[N];
	fill_n(availableNodeId,N,true);
	mapIpId = new int [N];
	f(i,0,N)			// Initialize all Nodes
		nodeObject[i].initNode();	
	f(i,0,N)
		nodeObject[i].initNeighborhoodSet();
	nodeObject[1001].printNode();	
	while(choice!=5){
	choice = menu();
	switch(choice){
		case 1:
				printf("Enter NodeId to which msg has to be sent\n");
				scanf("%d",&nodeId1);
				printf("Enter NodeId to which msg has to be sent\n");
				scanf("%d",&nodeId2);
				printf("Enter msg\n");
				scanf("%s",msg);
				#ifndef DPRINTF
				#define DPRINTF
				nodeId2 = node :: route(nodeId2,nodeId1,msg);
				#endif
				break;
		case 2:
				break;
		case 3: 
				break;

		case 4: printf("Enter the nodeId:\n");
				scanf("%d",&nodeId1);
				//nodeId2 = node :: route(nodeId1);
				printf("Node %d found (closest to %d)\n",nodeId2,nodeId1);
				nodeObject[nodeId2].printNode();
				printf("\n");
				break;

		case 5: exit(0);
	}
	}
	return 0;	
}