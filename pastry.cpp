/*

* File Name : pastry.cpp

* Creation Date : 09-02-2013

* Last Modified : Sat 16 Feb 2013 12:48:38 PM IST

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
#include<stdlib.h>
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


#define modval 10000
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
#define ROW 8 //int(ceil(log(N) / log(pow(2,b)))) // number of ROW Values
#define COL 16 //int(pow(2,b)) // number of Column Values
#define L 16
#define M 32


typedef class node 
{
/*member data*/
	string nodeId;
	uint8_t ipAddress[4];
	string *leafSet;
	string *neighborhoodSet;
	string **routingTable;
/*member functions*/
	void getNextIp();
	int getNextNodeId(map <int, string> & , map <string, int> & );
public:
	int initNode(map <int, string> &, map <string, int> &);
	void initLeafSet(map <string , int> & );
	void initRoutingTable(map <string , int> &);
	void initNeighborhoodSet(map <int, string> & );
	void forward(string &,char msg []); // returns next Id (if there exists one.. else returns -1);
	int  deliver(char msg[], int key); // why do we need this?
	void updateLeafSet(){};
	void updateNeighborhoodSet(){};
	void updateRoutingTable(){};
	void printNodeId(void); 
	void printNode(void);
}node;

typedef class pastry{
	public:
	vector <node> nodeList;
	map <string, int> mapIdIp;
	map <int, string> mapIpId;
	void pastryInit(void);
	int route(string & key1, string & key2, char []);
}pastry;

void node :: printNodeId(void){
	cout << nodeId << endl ;
}

void node :: forward(string & key, char *msg){
	// returns the next node if there exists a better match for the key
	// else returns -1, if this is the ultimate node.
	int i,j,min_diff;
	string next_node_Id;
	int col;
	//printNode();
	next_node_Id.assign(nodeId);
	if(next_node_Id.compare(key)==0){
		return ;
	}
	min_diff = abs(atoi(leafSet[i].c_str()-atoi(key.c_str())));
	next_node_Id.assign(nodeId);
	for(i=0;i< L; i++){  // check Leaf Set for some nodeId having value closer to key than current nodeId
		if(!leafSet[i].compare( "\0"))
			continue;
		if(abs(atoi(leafSet[i].c_str()-atoi(key.c_str())))  < min_diff ){
			next_node_Id.assign(leafSet[i]);
			min_diff = abs( atoi(leafSet[i].c_str()) - atoi(key.c_str()) );
		}
	}
	if(next_node_Id.compare(nodeId)!=0) { // some leaf nodeId was closer to the key than the current nodeId
		key.assign(next_node_Id);
		return ; // forward message to leaf node having closer node Id.
	}else { // match all column entries having first digit closer to key;
			// [TODO] optimize this loop, you can skip checking all entries of routing table
		for(i=0;i< ROW; i++){
			for(j=0; j< COL; j++){
				if(!routingTable[i][j].compare( "\0")){
					if(abs( atoi(key.c_str()) - atoi(routingTable[i][j].c_str())) < min_diff){
						min_diff = abs(atoi(key.c_str()) - atoi(routingTable[i][j].c_str()));
						next_node_Id = routingTable[i][j];
					}
				}
			}
		}
	}
	printNode();
	getchar();
	next_node_Id.assign(key);
//	if(next_node_Id == nodeId) // [TODO] implement logic of no entry found in routing table
//		return next_node_Id;
	return ;
}

int pastry :: route(string & key1, string & key2, char *msg){
	int next_key = random() % N;	
	char ipadd[100];
	int ip1=0;
	ip1 = mapIdIp[key1];
	while(ip1!=-1){
		string key1 = key2;
		cout<< "Working On Node ";
		nodeList[ip1].printNodeId() ;
		nodeList[ip1].forward(key2, msg);
		if(!key2.compare(key1))
			break;
	}
	return 0;
}

void node :: printNode(){
	int i,j;
	//printf("\t\t\tNodeid = %s\n", nodeId.c_str());
	cout << "\t\t\tNodeId = " << nodeId << endl;
	printf("\t\t\tIPAdd = %d.%d.%d.%d\n",ipAddress[3],ipAddress[2],ipAddress[1],ipAddress[0] );
	printf("\t\t\tLeaf Set\n");
	f(i,0,L/2)
		printf("%s\t",leafSet[i].c_str());
	printf("\n");
	f(i,L/2,L)
		printf("%s\t",leafSet[i].c_str());
	printf("\n");
	printf("\t\t\tNeighborhood Set\n");
	f(i,0,M/2)
		printf("%s\t",neighborhoodSet[i].c_str());
	printf("\n");
	f(i,M/2,M)
		printf("%s\t",neighborhoodSet[i].c_str());
	printf("\n");
	printf("\t\t\tRouting Table\n");
	f(i,0,ROW){
		f(j,0,COL){
			printf("%s\t",routingTable[i][j].c_str());
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

int node :: getNextNodeId(map <int, string> & mapIpId, map <string, int> & mapIdIp){
	int ip = ipAddress[0] + (ipAddress[1] * 256) + (ipAddress[2] * 256 * 256) + (ipAddress[3] * 256 * 256 * 256);
	char  n[10];
	sprintf(n,"%d",ip);
	string ipstr(n);
	string nodeIdStr(md5(ipstr).substr(0,8));
	if(mapIdIp[nodeIdStr]!=0)
		return -1;
	mapIdIp[nodeIdStr] = ip;
	mapIpId[ip] = nodeIdStr;
	nodeId.assign(nodeIdStr);
	return ip;
}

void node :: initLeafSet(map <string , int> & mapIdIp){
	int i;
	leafSet = new string [L];
	map <string , int> :: iterator mit,low,high;
	mit = mapIdIp.find(nodeId);
	//printf("mit -> set to :: for IP = %d, id = %s\n",mapIdIp[nodeId],mit->first.c_str());
	low = mit;
	high = mit;
	for(i=0;i<L/2;i++){
		if(low!= mapIdIp.begin()){
			low--;
		}else {
		 	low = mapIdIp.end();
			low--;
		}
	//	printf("low : mapIdIp <%s , %d>\n",low->first.c_str(), low->second);
		leafSet[L/2-i-1].assign( low ->first);
		if(high!= mapIdIp.end()){
			high++;
		}else{
			high = mapIdIp.begin();
			high++;
		}
	//	printf("\t\t\thigh : mapIdIp <%s , %d>\n",high->first.c_str(), high->second);
		leafSet[L/2+i].assign(high->first);
	}
	//printf("Leaf Set Initialized for %s\n",nodeId.c_str());
}

void node :: initNeighborhoodSet(map <int, string >  & mapIpId){
	int i;
	int ip = ipAddress[0] + (ipAddress[1] * 256) + (ipAddress[2] * 256 * 256) + (ipAddress[3] * 256 * 256 * 256);
	map <int, string> :: iterator mit,low, high;
	neighborhoodSet = new string [M];
	mit = mapIpId.find(ip);
	//printf("Initializing NeighborHood Set for %s \n",nodeId.c_str());
	//printf("mit -> set to :: for ID = %s, ip = %d\n",mapIpId[ip].c_str(),mit->first);
	low = mit;
	high = mit;
	for(i=0;i<M/2;i++){
		if(low!=mapIpId.begin() || low->first != 0)
			low--;
		else{
			low = mapIpId.end();
			low--;
		}
//		printf("low : mapIdIp <%d , %s>\n",low->first, low->second.c_str());
		neighborhoodSet[M/2 -i -1].assign(low->second);
		high++;
		if( high == mapIpId.end() ) high = mapIpId.begin();
		neighborhoodSet[M/2+i].assign(high->second);
//		printf("\t\t\thigh : mapIpId <%d , %s>\n", high->first, high->second.c_str());
	}
}

void node :: initRoutingTable(map <string, int> & mapIdIp ){
	int x,i,j;
	char buf[2];
	string routingEntry;
	routingTable = new string *[ROW];
	assert(routingTable!=NULL);
	f(i,0,ROW){
		routingTable[i] = new string [COL];
		assert(routingTable[i]!=NULL);
	}
	routingEntry.assign(nodeId);
	//printf("Initializing Routing Table for %s\n",nodeId.c_str());
	f(i,0,ROW){
		f(j,0,COL){
			sprintf(buf,"%x",j);
			routingEntry.replace(i,1,buf);
			if(mapIdIp[routingEntry]!=0){
				routingTable[i][j].assign(routingEntry);
			}else{
			routingTable[i][j].assign("\0");	
			}
			//printf("NodeId = %s :: routingTable[%d][%d] =  %s\n",nodeId.c_str(),i,j,routingTable[i][j].c_str());
		}
	}
//	printf("Routing Table Initialized for %s\n",nodeId.c_str());
}

int node :: initNode( map <int, string>  & mapIpId , map <string, int> & mapIdIp  ){
	int i=-1;
	while(i==-1){
		getNextIp();
		i = getNextNodeId(mapIpId, mapIdIp);
	//	printf("id = %d ip = %s\n",i, mapIpId[i].c_str());
	}
	return i;
}

void pastry :: pastryInit(void){
	int i,nodeIp;
	nodeList.resize(N+1000);
	f(i,0,N){			//Initialize all Nodes
		nodeIp = nodeList[i].initNode(mapIpId,mapIdIp );	
	//	printf("PASTRY INIT :id = %s ip = %d\n",mapIpId[nodeIp].c_str(), nodeIp);
	}
	f(i,0,N){
		nodeList[i].initLeafSet(mapIdIp);
		nodeList[i].initRoutingTable(mapIdIp);
		nodeList[i].initNeighborhoodSet(mapIpId);
//		nodeList[1001].printNode();	
	}
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
	int choice,i,j,nodeId1,nodeId2,key;
	char msg[100];
	string key1, key2;
	pastry *pastryObj = new pastry();
	pastryObj->pastryInit();
	while(choice!=5){
	choice = menu();
	switch(choice){
		case 1:
				printf("Enter Source Key\n");
				cin >> key1;
				printf("Enter Destination Key\n");
				cin >> key2;
				printf("Enter String\n");
				cin  >> msg;
				pastryObj->route(key1,key2,msg);
				break;
		case 2:
				break;
		case 3: 
				break;

		case 4: printf("Enter the nodeId:\n");
				scanf("%d",&nodeId1);
				break;
	
		case 5: exit(0);
	}
	}
	return 0;	
}
