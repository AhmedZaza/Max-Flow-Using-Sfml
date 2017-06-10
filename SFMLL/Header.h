#include <SFML/Graphics.hpp>
#include <Windows.h>
#include<string>
#include<sstream>
#include<vector>
#include<map>
#include<iostream>
#include<queue>
using namespace std;
using namespace sf;
class Node
{
public:
	Text text;
	Font myfont;
	CircleShape circ;
	Node(int x, int y, string s)
	{
		myfont.loadFromFile("arial.ttf");
		text.setFont(myfont);
		circ.setRadius(13);
		circ.setPosition(x, y);
		text.setString(s);
		text.setCharacterSize(20);
		text.setColor(Color::Red);
		text.setPosition(x + 6, y );
	}
};
class MaxFlow
{
private:
	int **AdjacencyMatrix;//Matrix that conrain every edge with the capacity before the maxflow
	int **NewFlow; // the new array that contain every edge with the capacity after the maxflow
	int NumOfNodes, NumOfEdges, Source, Sink, MaxFlowValue;
	vector<pair<int, vector<int> > >  Allpathes;//to store all pathes during the flow all[0]
	map<int, pair<int, int> > pos;// to give every node (x,y) position
	map<int, vector<int> >level; //to save in every level a number of nodes
	int *parent;// temporary array to save every node and it's parent when run backtrack
	void BackTrack(int, vector<int>&);// to store the path
	bool GetPath();// return true if there is a path and save it
	int FindFlowAndReduceit(vector<int>);// find the minimum cut in the given path
	void Getlevels(int);// to check levels and passing 1 if i check before the flow else i check after the flow
	void GetMaxFlow();// calculate the maxflow
public:
	MaxFlow();//constractor to read the input
	void draw();// to draw all graphs
};
class Start
{
public:
	Start();
    void SetCursorToPosition(int, int, int);
	int Choice;
};