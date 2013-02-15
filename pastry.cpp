/*

* File Name : pastry.cpp

* Creation Date : 09-02-2013

* Last Modified : Thu 14 Feb 2013 02:33:36 AM IST

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
#include<assert.h>
#include<curses.h>
#include"md5.h"

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


typedef class node 
{
/*member data*/
	int nodeId;
	uint8_t ipAddress[4];
	int *leafSet;
	int *neighborhoodSet;
	int **routingTable;
/*member functions*/
	void getNextIp();
	int getNextNodeId(vector <bool> , vector <int> & );
	void initLeafSet();
	void initRoutingTable();
public:
	int initNode(vector <bool> &, vector <int> &);
	void initNeighborhoodSet(vector <int> & );
	int  forward( int key,char msg []); // returns next Id (if there exists one.. else returns -1);
	int  deliver(char msg[], int key); // why do we need this?
	void updateLeafSet(){};
	void updateNeighborhoodSet(){};
	void updateRoutingTable(){};
	void printNode(); 
}node;

typedef class pastry{
	public:
	vector <node> nodeList;
	vector <bool> availableNodeId;
	vector <int> mapIpId;
	vector <int> nodeIdList;
	void pastryInit(void);
	int route(int key, char []);
}pastry;

int pastry :: route( int key, char *msg){
	int next_key = random() % N;	
	int dum;
	while(next_key != -1){
		printf("Working On Node %d\n",next_key);
		next_key = nodeList[nodeIdList[next_key]].forward(key, msg);
		//getc(stdin);
	}
}

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
	static int ip = -1;
	ip++;
	ipAddress[0] = ip % 256;
	ipAddress[1] = (ip / 256) % 256;
	ipAddress[2] = (ip / (256 * 256) ) % 256;
	ipAddress[3] = (ip / (256 * 256 * 256)) % 256;
	return;
}

int node :: getNextNodeId(vector <bool> availableNodeId, vector <int> & mapIpId){
int ip = ipAddress[0] + (ipAddress[1] * 256) + (ipAddress[2] * 256 * 256) + (ipAddress[3] * 256 * 256 * 256);
	//int nodeId = random() % N;
	string S;
	
	if(availableNodeId[nodeId]==false){
		while(availableNodeId[nodeId]==false){
			nodeId = (nodeId +1 ) % N;
		}
	}
	availableNodeId[nodeId]=false;
	mapIpId[ip] = nodeId;	
	this->nodeId = nodeId;
	return nodeId;
}

int node :: forward(int key, char *msg){
	// returns the next node if there exists a better match for the key
	// else returns -1, if this is the ultimate node.
	int i,j,min_diff,next_node_Id;
	int col;
	//printNode();
	if(nodeId == key){
		return -1;
	}
	min_diff = abs(nodeId - key);
	next_node_Id = nodeId;
	for(i=0;i< L; i++){  // check Leaf Set for some nodeId having value closer to key than current nodeId
		if(leafSet[i]==-1)
			continue;
		if(abs(leafSet[i] - key) < min_diff ){
			next_node_Id = leafSet[i];
			min_diff = abs(leafSet[i]-key);
		}
	}
	if(next_node_Id != nodeId) { // some leaf nodeId was closer to the key than the current nodeId
		return next_node_Id; // forward message to leaf node having closer node Id.
	}else { // match all column entries having first digit closer to key;
			// [TODO] optimize this loop, you can skip checking all entries of routing table
		for(i=0;i< ROW; i++){
			for(j=0; j< COL; j++){
				if(routingTable[i][j] != -1){
					if(abs(key - routingTable[i][j]) < min_diff){
						min_diff = abs(key - routingTable[i][j]);
						next_node_Id = routingTable[i][j];
					}
				}
			}
		}
	}
	if(next_node_Id == nodeId) // no entry found in routing table that is closer than present node
		return -1;
	return next_node_Id;
}


void node :: initLeafSet(){
	int i;
	leafSet = new int [L];
	for(i=L/2;i>0;i--){
		leafSet[i-1] = (nodeId - (L/2 - i + 1) + N) % N ;
		leafSet[L/2+i-1] = (nodeId + i) % N;
	}
}

void node :: initNeighborhoodSet(vector <int>  & mapIpId){
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
	assert(routingTable!=NULL);
	f(i,0,ROW){
		routingTable[i] = new int [COL];
		assert(routingTable[i]!=NULL);
	}
	f(i,0,ROW)
		f(j,0,COL){
			mask = allOnes >> (i*4);
			mask = mask & 0x0000ffff;
			sum = (nodeId & mask ) + (j * pow((pow(2,b)),(ROW-i-1))) ;
			routingTable[i][j] = sum;
		}
}

int node :: initNode( vector <bool> & availableNodeId , vector <int> & mapIpId){
	int i;
	getNextIp();
	i = getNextNodeId(availableNodeId, mapIpId);
	initLeafSet();
	//initNeighborhoodSet();
	initRoutingTable();
	return i;
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

void pastry :: pastryInit(void){
	int i,nodeId;
	availableNodeId.resize(N); //= new bool[N];
	vector <bool> :: iterator itb;
	for(itb=availableNodeId.begin();itb!=availableNodeId.end();itb++)
		*itb=true;
	mapIpId.resize(N); //= new int [N];
	nodeList.resize(N);// = new node[N];
	nodeIdList.resize(N); // = new int[N];
	f(i,0,N){			//Initialize all Nodes
		nodeId = nodeList[i].initNode(availableNodeId, mapIpId);	
		nodeIdList[nodeId] = i;
	}
	f(i,0,N)
		nodeList[i].initNeighborhoodSet(mapIpId);
//		nodeList[1001].printNode();	
}

int 
main(int argc, char *argv[])
{
	int choice,i,j,nodeId1,nodeId2,key;
	char msg[100];
	pastry *pastryObj = new pastry();
	pastryObj->pastryInit();
	while(choice!=5){
	choice = menu();
	switch(choice){
		case 1:
				printf("Enter Key\n");
				scanf("%d",&key);
				printf("Enter msg\n");
				scanf("%s",msg);
				pastryObj->route(key,msg);
				break;
		case 2:
				break;
		case 3: 
				break;

		case 4: printf("Enter the nodeId:\n");
				scanf("%d",&nodeId1);
				//nodeId2 = node :: route(nodeId1);
				//printf("Node %d found (closest to %d)\n",nodeId2,nodeId1);
				//printNode();
				printf("\n");
				break;

		case 5: exit(0);
	}
	}
	return 0;	
}
