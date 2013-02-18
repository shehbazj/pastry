/*

* File Name : pastry.cpp

* Creation Date : 09-02-2013

* Last Modified : Mon 18 Feb 2013 08:40:53 AM IST

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
//#include<curses.h>
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
#define N 1000 // number of nodes
#define ROW 8 //int(ceil(log(N) / log(pow(2,b)))) // number of ROW Values
#define COL 16 //int(pow(2,b)) // number of Column Values
#define L 16
#define M 32


typedef class node 
{
/*member data*/
	uint8_t ipAddress[4];
	string nodeId;
	string *leafSet;
	string *neighborhoodSet;
	string routingTable[ROW][COL];
/*member functions*/
	int getNextNodeId(map <int, string> & , map <string, int> & );
	void getNextIp();
public:
	int initNode(map <int, string> &, map <string, int> &);
	int deliver(char msg[], int key); // why do we need this?
	int nodeListLookup(string &);
	int isKey(string &);
	void initLeafSet(map <string , int> & );
	void initNeighborhoodSet(map <int, string> & );
	void copyLeafSet(node &);
	void copyNeighborhoodSet(node &);
	void printNodeId(void); 
	void printNode(void);
	void setRoutingTableEntry(int, int, string);
	string getNodeId(){return nodeId;}
	string forward(const string &,char msg [], int hitCounter []); // returns next Id (if there exists one.. else returns -1);
	string & getRoutingTableEntry(int, int);
}node;

typedef class pastry{
	public:
	vector <node> nodeList;
	map <string, int> mapIdIp;
	map <int, string> mapIpId;
	void pastryInit(void);
	void initRoutingTable();
	void printNode(string &);
	void addNode();
	node getNodeObject(string &);
	string bestMatch( int , char , string );
	string getNearestNodeId(const string &);
	string & route(string & key1,const string & key2, char [], vector <string> &, int [] );
}pastry;

long long strdiff(const string &s1, string &s2){
	long long x, y;
	stringstream ss,tt;
	ss << hex << s1.c_str();
	ss >> x;
	tt << hex << s2.c_str();
	tt >> y;
	//cout << "x = " << x << endl << "y = " << y << endl;
	return abs(x-y);
}

int shl(const string & key, string &nodeId){ // returns common substring length
	int i;
	for(i=key.length()-1;i>=0;i--)
		if(!strncmp(key.c_str(),nodeId.c_str(),i))
			return i;
}

void  node :: copyNeighborhoodSet(node &n){
	
}

node pastry :: getNodeObject(string &s){
	vector <node> :: iterator ItN;
	node retNode; //= new node();
	for(ItN=nodeList.begin(); ItN!=nodeList.end();ItN++){
		if(ItN->getNodeId().compare(s)){
			retNode =  *ItN;
		}
	}
	return retNode;
}

string pastry::bestMatch( int rowNo, char colChar, string curNode ){
	map<string, int> :: iterator it;	
	string allMask("ffffffff");
	for (it = mapIdIp.begin(); it != mapIdIp.end(); it++) {
		if( !strncmp( curNode.c_str(), (it->first).c_str(), rowNo ) && it->first.at(rowNo) == colChar && curNode.compare(it->first.c_str()) )
			return (it->first);
	}
	return allMask;
}

void pastry::initRoutingTable(){
	
	vector<node> :: iterator it;
	string curid;
	char colChar;
	for( it = nodeList.begin(); it != nodeList.end(); it++){
		curid = it->getNodeId();	
		printf("%d " ,int( distance(nodeList.begin(),it)));
		it->printNodeId();
		for (int i = 0; i < ROW; i++) {
			for (int j = 0; j < COL; j++) {
				if(j>9){
				colChar = j+87;
				}else{
					colChar = j+48;
				}
				string bestMatchString = bestMatch(i,colChar,curid);
				it->setRoutingTableEntry( i, j, bestMatchString);
				//printf("%s\n",it->getRoutingTableEntry(i,j).c_str());
			}
		}
	}
}

void node :: printNodeId(void){
	cout << nodeId << endl ;
}

string node :: forward(const string & key, char *msg, int *hitCounter){
	// returns the next node if there exists a better match for the key
	int i,j;
	long long min_diff;
	string nextNodeId;
	int col,row;
	printf("msg received = %s\n",msg);
	if(nodeId.compare(key)==0){
		hitCounter[0]++;
		return key; // reached destination, return.
	}
	nextNodeId.assign(nodeId);
	min_diff = strdiff(key,nextNodeId);
	string min_leaf("ffffffff"), max_leaf("ffffffff");
	for(i=0;i<L;i++){ // select Min and Max leaf Elements
		if(!leafSet[i].compare("ffffffff"))
			continue;
		if(!min_leaf.compare("ffffffff"))
			min_leaf.assign(leafSet[i]);
		max_leaf.assign(leafSet[i]);
	}
	if(key.compare(min_leaf) >= 0 && key.compare(max_leaf) <= 0 && max_leaf.compare("ffffffff") && min_leaf.compare("ffffffff") )	
	{  // leaf set [HIT]	
		for(i=0;i< L; i++){  // check Leaf Set for some nodeId having value closer to key than current nodeId
			if(!leafSet[i].compare( "ffffffff"))
				continue;
			if(strdiff(key,leafSet[i])  < min_diff ){
				nextNodeId.assign(leafSet[i]);
				min_diff = strdiff(key,leafSet[i]);
			}
		}
		hitCounter[0]++;
		return nextNodeId;
	}
	row = shl(key,nodeId);
	col = (key[row] > '9') ? (key[row]-87): (key[row]-48);
	if(routingTable[row][col].compare("ffffffff")){ // routingTable [HIT]
		hitCounter[1]++;
		return routingTable[row][col];
	}
	// [END CASE] return minimum node entry encountered among  (Leaf Set) U (Routing Table) U (NeighborHood Set)
	
	// Scanning LeafSet
	
	for(i=0;i< L; i++){ 
		if(!leafSet[i].compare( "ffffffff"))
			continue;
		if(strdiff(key,leafSet[i])  < min_diff && shl(key,leafSet[i])>=row ){  // min diff has been taken wrt current NodeId at start of function
			nextNodeId.assign(leafSet[i]);
			min_diff = strdiff(key,leafSet[i]);
		}
	}

	// Scanning Routing Table 
	
	for(i=0;i<ROW ; i++){  
		for(j=0; j< COL; j++){
			if(routingTable[i][j].compare( "ffffffff")){
				if(strdiff(key,routingTable[i][j]) < min_diff && shl(key,leafSet[i])>=row){
					min_diff = strdiff(key,routingTable[i][j]);
					nextNodeId = routingTable[i][j];
				}
			}
		}
	}
	
	// Scanning Neighborhood Set
	
	for(i=0;i< M; i++){  // check Neighborhood Set for some nodeId having value closer to key than current nodeId
		if(!neighborhoodSet[i].compare( "ffffffff"))
			continue;
		if(strdiff(key,neighborhoodSet[i])  < min_diff && shl(key,leafSet[i])>=row){
			nextNodeId.assign(neighborhoodSet[i]);
			min_diff = strdiff(key,neighborhoodSet[i]);
		}
	}
//	printNode();
	hitCounter[2]++;
	getchar();
	return nextNodeId;
}

string  pastry :: getNearestNodeId(const string &s){
	map <string , int > :: iterator it;
	string keyNext, keyPrev;
	long long min = 999999999;
	for(it=mapIdIp.begin();it!=mapIdIp.end(); it++){
		printf("%s\n",it->first.c_str());
		keyPrev.assign(it->first);
		if(strdiff(s,keyPrev) < min){
			keyNext.assign(keyPrev);
			min = strdiff(s,keyPrev);
		}  // find key1 or first key.
	}
	//s.assign(keyNext);	
return keyNext;
}

string & pastry :: route(string & key1,const string & key2, char *msg, vector <string> & hopNodes, int *hitCounter){
	string keyNext,keyPrev,nearestKey;
	map <string , int > :: iterator it;
	if(mapIdIp.find(key1)==mapIdIp.end()){	
		cout << key1 << " not found.. searching for new key " << endl;
		nearestKey.assign( getNearestNodeId(key1));
	}
	cout << "Taking " << nearestKey << " as default Source key..." << endl;
	keyNext.assign(nearestKey);
	do{
		cout << "Next Key " << keyNext << endl;
		hopNodes.push_back(keyNext);
		keyPrev.assign(keyNext);
		keyNext = nodeList[mapIdIp[keyPrev]].forward(key2, msg, hitCounter);
	}while(keyPrev.compare(keyNext));
	key1.assign(keyNext);
	return key1;
}

int node :: isKey(string &key){
	if(!nodeId.compare(key))
		return 1;
	return 0;
}
void pastry :: printNode(string & key){
	vector <node> :: iterator it;
	for(it = nodeList.begin(); it != nodeList.end(); it++)
		if(it->isKey(key)){
			it->printNode();
		}
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
	if(mapIdIp.find(nodeIdStr)!=mapIdIp.end())
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
		if(low == mapIdIp.begin()) { low = mapIdIp.end();} 
			low--;
	//	printf("low : mapIdIp <%s , %d>\n",low->first.c_str(), low->second);
		leafSet[L/2-i-1].assign( low ->first);
	//	printf("\t\t\thigh : mapIdIp <%s , %d>\n",high->first.c_str(), high->second);
		high++;
		if( high == mapIdIp.end() ) high = mapIdIp.begin();
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
		if(low == mapIpId.begin()) { low = mapIpId.end(); }
			low--;
//		printf("low : mapIdIp <%d , %s>\n",low->first, low->second.c_str());
		neighborhoodSet[M/2 -i -1].assign(low->second);
		
		high++;
		if( high == mapIpId.end() ) high = mapIpId.begin();
		neighborhoodSet[M/2+i].assign(high->second);
//		printf("\t\t\thigh : mapIpId <%d , %s>\n", high->first, high->second.c_str());
	}
}

void node :: setRoutingTableEntry( int row, int column, string entry){
	routingTable[row][column] = entry;
}

string & node :: getRoutingTableEntry( int row, int column){
	return routingTable[row][column];
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
	nodeList.resize(N);
	f(i,0,N){			//Initialize all Nodes
		nodeIp = nodeList[i].initNode(mapIpId,mapIdIp );	
	}
	f(i,0,N){
		nodeList[i].initLeafSet(mapIdIp);
		nodeList[i].initNeighborhoodSet(mapIpId);
	}
	initRoutingTable();
}

void pastry :: addNode(){
	int ip;
	char msg[] = "ping";
	int hitCounter[3];
	vector <string> hopNodes;
	string neighborNodeId;
	node newNode;
	ip = newNode.initNode(mapIpId, mapIdIp);
	neighborNodeId = nodeList.back().getNodeId();
	const string destinationNodeId = newNode.getNodeId();
	route(neighborNodeId,destinationNodeId,msg,hopNodes, hitCounter);
	node neighborNode = getNodeObject(neighborNodeId);
	newNode.copyNeighborhoodSet(neighborNode);	
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
	int choice,i,j,key;
	string nodeId1, nodeId2;
	char msg[100];
	vector <string> hopNodes;
	vector <string> :: iterator itS;
	int hitCounter[3];
	string key11, key12;
	pastry *pastryObj = new pastry();
	pastryObj->pastryInit();
	while(choice!=5){
	choice = menu();
	switch(choice){
		case 1:
				printf("Enter Source Key\n");
				cin >> key11;
				printf("Enter Destination Key\n");
				cin >> key12;
				printf("Enter String\n");
				cin  >> msg;
				pastryObj->route(key11,key12,msg,hopNodes, hitCounter);
				for(itS= hopNodes.begin(); itS!=hopNodes.end(); itS++)
					cout << "Node : "<< *itS << "\n " ;
				cout << "\n";
				break;
		case 2:
				printf("Creating A new Node...\n");
				pastryObj->addNode();
				break;
		case 3: 
				break;

		case 4: printf("Enter the nodeId:\n");
				cin >> nodeId1;
				pastryObj->printNode(nodeId1);	
				break;
	
		case 5: exit(0);
	}
	}
	return 0;	
}
