#include"Header.h"
MaxFlow::MaxFlow()
{
	MaxFlowValue = 0;
	cout << "Enter Number Of Nodes : ";
	cin >> NumOfNodes;
	cout << "Enter Number Of Edges : ";
	cin >> NumOfEdges;
	AdjacencyMatrix = new int*[NumOfNodes];
	NewFlow = new int*[NumOfNodes];
	for (int i = 0; i < NumOfNodes; i++)
	{
		AdjacencyMatrix[i] = new int[NumOfNodes];
		NewFlow[i] = new int[NumOfNodes];
	}
	for (int i = 0; i < NumOfNodes; i++)
	for (int j = 0; j < NumOfNodes; j++)
	{
		AdjacencyMatrix[i][j] = 0;
		NewFlow[i][j] = 0;
	}
	parent = new int[NumOfNodes];
	cout << "Enter " << NumOfEdges << " Edges , Ex : From -> to then the capacity :" << endl;
	int u, v, cap;
	for (int i = 0; i < NumOfEdges; i++)
	{
		cin >> u >> v >> cap;
		while (u < 0 || u >= NumOfNodes || v < 0 || v >= NumOfNodes)
		{
			cout << "invalid Edge Enter another valid Edge !! " << endl;
			cin >> u >> v >> cap;
		}
		AdjacencyMatrix[u][v] = cap;
	}
	cout << "Enter your Source Node No. : "; 
	cin >> Source;
	while (Source < 0 || Source >= NumOfNodes)
	{
		cout << "invalid Source Enter another valid Source !!" << endl;
		cin >> Source;
	}
	cout << "Enter your Sink Node No. : "; 
	cin >> Sink;
	while (Sink < 0 || Sink >= NumOfNodes)
	{
		cout << "invalid Sink Enter another valid Sink !! " << endl;
		cin >> Sink;
	}
	cout << endl;
}
bool MaxFlow::GetPath()
{
	queue<int> q;
	vector<bool> vis(NumOfNodes);
	for (int i = 0; i < NumOfNodes; i++)
		parent[i] = -1, vis[i] = 0;
	vis[Source] = 1;
	q.push(Source);
	while (!q.empty())
	{
		int cur = q.front();
		q.pop();
		for (int i = 0; i < NumOfNodes; i++)
		if (AdjacencyMatrix[cur][i]>0 && !vis[i])
		{
			q.push(i);
			parent[i] = cur;
			vis[i] = 1;
		}
	}
	if (vis[Sink] == 0)
		return 0;
	else
		return 1;
}
void MaxFlow::BackTrack(int u, vector<int>& path)
{
	if (parent[u] == -1)
	{
		return;
	}
	BackTrack(parent[u], path);
	path.push_back(u);
}
int MaxFlow::FindFlowAndReduceit(vector<int> path)
{
	int minn = 1e9;
	for (int i = 0; i < path.size() - 1; i++)
	{
		minn = min(AdjacencyMatrix[path[i]][path[i + 1]], minn);
	}

	for (int i = 0; i < path.size() - 1; i++)
	{
		AdjacencyMatrix[path[i]][path[i + 1]] -= minn;
		AdjacencyMatrix[path[i+1]][path[i ]] += minn;
		NewFlow[path[i]][path[i + 1]] += minn;
	}
	return minn;
}  //min cut
void MaxFlow::GetMaxFlow()
{
	vector<int> Path;
	while (GetPath())
	{
		Path.clear();
		Path.push_back(Source);
		BackTrack(Sink, Path);
		int minn = FindFlowAndReduceit(Path);
		Allpathes.push_back({ minn, Path });
		for (int i = 0; i < Path.size(); i++)
		{
			cout << Path[i];
			if (i != Path.size() - 1)
				cout << " -> ";
			else
				cout << "   : This Path With Flow =  " << minn << endl;
		}
		MaxFlowValue += minn;
	}
	if (Allpathes.size() == 0)
	{
		cout << "There is no Pathes from" << Source << " To " << Sink << endl;
	}
	else
		cout << endl << "Max Flow is : " << MaxFlowValue << endl;
}
void MaxFlow::Getlevels(int type)
{    //BFS
	queue<int> q;
	vector<int> lev(NumOfNodes);
	vector<bool> vis(NumOfNodes, 0);
	q.push(Source);
	lev[Source] = 0;
	vis[Source] = 1;
	level[0].push_back(Source);
	while (!q.empty())
	{
		int u = q.front();
		q.pop();
		for (int i = 0; i < NumOfNodes; i++)
		{
			if (u != i &&!vis[i] && AdjacencyMatrix[u][i] > 0)
			{
				vis[i] = 1;
				lev[i] = lev[u] + 1;
				q.push(i);
				level[lev[i]].push_back(i);
			}
		}
	}
	if (type == 1)
	for (int i = 0; i < NumOfNodes; i++)
	if (!vis[i])
		level[1].push_back(i);
}
void MaxFlow::draw()
{
	Getlevels(1);//output
	Text out;
	Font f;
	f.loadFromFile("arial.ttf");
	out.setString("The Original Graph");
	out.setFont(f);
	out.setColor(Color::Yellow);
	out.setPosition(0, 5);
	RenderWindow window(VideoMode(1200, 600), "Max Flow"); // max flow be4 graph
	while (window.isOpen())
	{
		Event event;
		Keyboard key;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed || event.key.code==Keyboard::G)
				window.close();
		}
		window.clear();
		window.draw(out); // text that contain originnal graph
		/********************************************/
		int ymen = 0, tmp;
		map<int, vector<int> >  ::iterator it = level.begin();
		for (it; it != level.end(); it++)
		{
			tmp = 300 - ((it->second.size() / 2) * 100);
			for (int i = 0; i < it->second.size(); i++)
			{
				pos[it->second[i]] = { ymen, tmp }; // node ----> x,y 
				stringstream s1;
				string s;
				s1 << it->second[i];
				s1 >> s;
				Node one(ymen, tmp, s);
				window.draw(one.circ);
				window.draw(one.text);
				tmp += 100;
			}
			ymen += 100;
		}
		/******************************************/
		map<pair<int, int>, bool> test;
		for (int i = 0; i < NumOfNodes; i++)
		for (int j = 0; j < NumOfNodes; j++)
		{
			if (i != j && AdjacencyMatrix[i][j]>0 && !test[{i, j}])
			{
				test[{i, j}] = 1;
				CircleShape t(9, 3); /// draw triangle 
				t.setFillColor(Color::Red);
				int x1 = pos[i].first, y1 = pos[i].second, x2 = pos[j].first, y2 = pos[j].second; //position of arrow from start to end 
				Text text;
				Font myf;
				myf.loadFromFile("arial.ttf");
				text.setFont(myf);
				text.setCharacterSize(10);
				string ss;
				if (y1 == y2 && x2 > x1) // ------>
				{
					stringstream aaa;
					aaa << AdjacencyMatrix[i][j];
					aaa >> ss; //capicty
					text.setString(ss);
					t.setPosition(x2 + 1, y2 + 1);
					t.setRotation(90);
					Vertex line[] =
					{
						Vertex(Vector2f(x1 + 20, y1 + 10)),
						Vertex(Vector2f(x2, y2 + 10))
					};
					text.setPosition(((double)(x1 + 10 + x2) / 2), (double)((y2 + y1 - 10) / 2)); // نقطه المتوسط 
					window.draw(line, 2, Lines);
					window.draw(t);
					window.draw(text);
				}
				else if (y1 == y2 && x1 > x2)// <-----------
				{
					stringstream aaa;
					aaa << AdjacencyMatrix[i][j];
					aaa >> ss;
					text.setString(ss);
					t.setPosition(x2 + 24, y2 + 29);
					t.setRotation(270);
					Vertex line[] =
					{
						Vertex(Vector2f(x1 + 1, y1 + 20)),
						Vertex(Vector2f(x2 + 25, y2 + 20))
					};
					text.setPosition(((double)(x1 + 15 + x2) / 2), (double)((y2 + y1 + 40) / 2));
					window.draw(line, 2, Lines);
					window.draw(t);
					window.draw(text);
				}
				else if (x2 > x1 && y2 < y1)//  up-right
				{
					stringstream aaa;
					aaa << AdjacencyMatrix[i][j];
					aaa >> ss;
					text.setString(ss);
					t.setPosition(x2 - 5, y2 + 8);
					t.setRotation(50);
					Vertex line[] =
					{
						Vertex(Vector2f(x1 + 15, y1)),
						Vertex(Vector2f(x2 - 5, y2 + 20))
					};
					text.setPosition(((double)(x1 + x2-15) / 2), (double)((y2 + y1+5) / 2));
					window.draw(line, 2, Lines);
					window.draw(t);
					window.draw(text);
				}
				else if (x2 < x1 && y2 > y1)//down-left
				{
					stringstream aaa;
					aaa << AdjacencyMatrix[i][j];
					aaa >> ss;
					text.setString(ss);
					t.setPosition(x2 + 28, y2 + 10);
					t.setRotation(230);
					Vertex line[] =
					{
						Vertex(Vector2f(x1 + 5, y1 + 25)),
						Vertex(Vector2f(x2 + 20, y2 + 5))
					};
					text.setPosition(((double)(x1 + 15 + x2) / 2), (double)((y2 + y1 + 40) / 2));
					window.draw(line, 2, Lines);
					window.draw(t);
					window.draw(text);
				}
				else if (x2 > x1 && y2 > y1)//down-right
				{
					stringstream aaa;
					aaa << AdjacencyMatrix[i][j];
					aaa >> ss;
					text.setString(ss);
					t.setPosition(x2 - 11, y2 - 11);
					t.setRotation(5);
					Vertex line[] =
					{
						Vertex(Vector2f(x1 + 20, y1 + 20)),
						Vertex(Vector2f(x2 + 5, y2 + 5))
					};
					text.setPosition(((double)(x1 + 15 + x2) / 2), (double)((y2 + y1 - 15) / 2));
					window.draw(line, 2, Lines);
					window.draw(t);
					window.draw(text);
				}
				else if (x1 > x2 && y1 > y2)//up-left
				{
					stringstream aaa;
					aaa << AdjacencyMatrix[i][j];
					aaa >> ss;
					text.setString(ss);
					t.setPosition(x2 + 8, y2 + 33);
					t.setRotation(-45);
					Vertex line[] =
					{
						Vertex(Vector2f(x1, y1 + 15)),
						Vertex(Vector2f(x2 + 15, y2 + 26))
					};
					text.setPosition(((double)(x1 + 15 + x2) / 2), (double)((y2 + y1 + 52) / 2));
					window.draw(line, 2, Lines);
					window.draw(t);
					window.draw(text);
				}
				else if (x1 == x2 && y1 > y2)//upp
				{
					stringstream aaa;
					aaa << AdjacencyMatrix[i][j];
					aaa >> ss;
					text.setString(ss);
					t.setPosition(x2 + 10, y2 + 23);
					Vertex line[] =
					{
						Vertex(Vector2f(x1 + 18, y1 + 2)),
						Vertex(Vector2f(x2 + 18, y2 + 5))
					};
					text.setPosition(((double)(x1 + 40 + x2) / 2), (double)((y2 + y1 + 30) / 2));
					window.draw(line, 2, Lines);
					window.draw(t);
					window.draw(text);
				}
				else if (x1 == x2 && y1 < y2)//down
				{
					stringstream aaa;
					aaa << AdjacencyMatrix[i][j];
					aaa >> ss;
					text.setString(ss);
					t.setPosition(x2 + 11, y2 - 20);
					t.setRotation(60);
					Vertex line[] =
					{
						Vertex(Vector2f(x1 + 8, y1 + 20)),
						Vertex(Vector2f(x2 + 8, y2 + 2))
					};
					text.setPosition(((double)(x1 + -20 + x2) / 2), (double)((y2 + y1 + 30) / 2));
					window.draw(line, 2, Lines);
					window.draw(t);
					window.draw(text);

				}
			}
		}
		window.display();
	}
	window.clear();
	GetMaxFlow();
	stringstream a;
	string str;
	a << MaxFlowValue;
	a >> str;
	Text flowval;
	if (str == "0")
	{
		flowval.setString("There is No Paths From the Source to the Sink node !!");
	}
	else
	{
		flowval.setString("Max Flow = " + str);
	}
	flowval.setFont(f);
	flowval.setColor(Color::Yellow);
	flowval.setPosition(0, 5);
	for (int i = 0; i < NumOfNodes; i++)
	{
		for (int j = 0; j < NumOfNodes; j++)
		{
			AdjacencyMatrix[i][j] = NewFlow[i][j];
		}
	}
	level.clear();
	Getlevels(0); // draw output
	RenderWindow window1(VideoMode(800, 600), "Max Flow After Solution");
	while (window1.isOpen())
	{
		Event event;
		while (window1.pollEvent(event))
		{
			if (event.type == Event::Closed ||event.key.code==Keyboard::Escape)  
				window1.close();
		}
		window1.clear();
		window1.draw(flowval);
		/********************************************/
		int ymen = 0, tmp;
		map<int, vector<int> >  ::iterator it = level.begin();
		for (it; it != level.end(); it++)
		{
			tmp = 300 - ((it->second.size() / 2) * 100);
			for (int i = 0; i < it->second.size(); i++)
			{
				pos[it->second[i]] = { ymen, tmp };
				stringstream s1;
				string s;
				s1 << it->second[i];
				s1 >> s;
				Node one(ymen, tmp, s);
				window1.draw(one.circ);
				window1.draw(one.text);
				tmp += 100;
			}
			ymen += 100;
		}
		/******************************************/
		map<pair<int, int>, bool> test;
		for (int i = 0; i < NumOfNodes; i++)
		for (int j = 0; j < NumOfNodes; j++)
		{
			if (i != j && AdjacencyMatrix[i][j]>0 && !test[{i, j}])
			{
				test[{i, j}] = 1;
				CircleShape t(9, 3);
				t.setFillColor(Color::Blue);
				int x1 = pos[i].first, y1 = pos[i].second, x2 = pos[j].first, y2 = pos[j].second;
				Text text;
				Font myf;
				myf.loadFromFile("arial.ttf");
				text.setFont(myf);
				text.setCharacterSize(10);
				string ss;
				if (y1 == y2 && x2 > x1) // ------>
				{
					stringstream aaa;
					aaa << AdjacencyMatrix[i][j];
					aaa >> ss;
					text.setString(ss);
					t.setPosition(x2 + 1, y2 + 1);
					t.setRotation(90);
					Vertex line[] =
					{
						Vertex(Vector2f(x1 + 20, y1 + 10)),
						Vertex(Vector2f(x2, y2 + 10))
					};
					text.setPosition(((double)(x1 + 10 + x2) / 2), (double)((y2 + y1 - 10) / 2));
					window1.draw(line, 2, Lines);
					window1.draw(t);
					window1.draw(text);
				}
				else if (y1 == y2 && x1 > x2)// <-----------
				{
					stringstream aaa;
					aaa << AdjacencyMatrix[i][j];
					aaa >> ss;
					text.setString(ss);
					t.setPosition(x2 + 24, y2 + 29);
					t.setRotation(270);
					Vertex line[] =
					{
						Vertex(Vector2f(x1 + 1, y1 + 20)),
						Vertex(Vector2f(x2 + 25, y2 + 20))
					};
					text.setPosition(((double)(x1 + 15 + x2) / 2), (double)((y2 + y1 + 40) / 2));
					window1.draw(line, 2, Lines);
					window1.draw(t);
					window1.draw(text);
				}
				else if (x2 > x1 && y2 < y1)//  up-right
				{
					stringstream aaa;
					aaa << AdjacencyMatrix[i][j];
					aaa >> ss;
					text.setString(ss);
					t.setPosition(x2 - 5, y2 + 8);
					t.setRotation(50);
					Vertex line[] =
					{
						Vertex(Vector2f(x1 + 15, y1)),
						Vertex(Vector2f(x2 - 5, y2 + 20))
					};
					text.setPosition(((double)(x1 + x2 - 15) / 2), (double)((y2 + y1 + 5) / 2));
					window1.draw(line, 2, Lines);
					window1.draw(t);
					window1.draw(text);
				}
				else if (x2 < x1 && y2 > y1)//down-left
				{
					stringstream aaa;
					aaa << AdjacencyMatrix[i][j];
					aaa >> ss;
					text.setString(ss);
					t.setPosition(x2 + 28, y2 + 10);
					t.setRotation(230);
					Vertex line[] =
					{
						Vertex(Vector2f(x1 + 5, y1 + 25)),
						Vertex(Vector2f(x2 + 20, y2 + 5))
					};
					text.setPosition(((double)(x1 + 15 + x2) / 2), (double)((y2 + y1 + 40) / 2));
					window1.draw(line, 2, Lines);
					window1.draw(t);
					window1.draw(text);
				}
				else if (x2 > x1 && y2 > y1)//down-right
				{
					stringstream aaa;
					aaa << AdjacencyMatrix[i][j];
					aaa >> ss;
					text.setString(ss);
					t.setPosition(x2 - 11, y2 - 11);
					t.setRotation(5);
					Vertex line[] =
					{
						Vertex(Vector2f(x1 + 20, y1 + 20)),
						Vertex(Vector2f(x2 + 5, y2 + 5))
					};
					text.setPosition(((double)(x1 + 15 + x2) / 2), (double)((y2 + y1 - 15) / 2));
					window1.draw(line, 2, Lines);
					window1.draw(t);
					window1.draw(text);
				}
				else if (x1 > x2 && y1 > y2)//up-left
				{
					stringstream aaa;
					aaa << AdjacencyMatrix[i][j];
					aaa >> ss;
					text.setString(ss);
					t.setPosition(x2 + 8, y2 + 33);
					t.setRotation(-45);
					Vertex line[] =
					{
						Vertex(Vector2f(x1, y1 + 15)),
						Vertex(Vector2f(x2 + 15, y2 + 26))
					};
					text.setPosition(((double)(x1 + 15 + x2) / 2), (double)((y2 + y1 + 52) / 2));
					window1.draw(line, 2, Lines);
					window1.draw(t);
					window1.draw(text);
				}
				else if (x1 == x2 && y1 > y2)//upp
				{
					stringstream aaa;
					aaa << AdjacencyMatrix[i][j];
					aaa >> ss;
					text.setString(ss);
					t.setPosition(x2 + 10, y2 + 23);
					Vertex line[] =
					{
						Vertex(Vector2f(x1 + 18, y1 + 2)),
						Vertex(Vector2f(x2 + 18, y2 + 5))
					};
					text.setPosition(((double)(x1 + 40 + x2) / 2), (double)((y2 + y1 + 30) / 2));
					window1.draw(line, 2, Lines);
					window1.draw(t);
					window1.draw(text);
				}
				else if (x1 == x2 && y1 < y2)//down
				{
					stringstream aaa;
					aaa << AdjacencyMatrix[i][j];
					aaa >> ss;
					text.setString(ss);
					t.setPosition(x2 + 11, y2 - 20);
					t.setRotation(60);
					Vertex line[] =
					{
						Vertex(Vector2f(x1 + 8, y1 + 20)),
						Vertex(Vector2f(x2 + 8, y2 + 2))
					};
					text.setPosition(((double)(x1 + -20 + x2) / 2), (double)((y2 + y1 + 30) / 2));
					window1.draw(line, 2, Lines);
					window1.draw(t);
					window1.draw(text);

				}
			}
		}
		window1.display();
	}
}
void Start::SetCursorToPosition(int x, int y, int _color)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x, y };
	SetConsoleCursorPosition(out, pos);
	SetConsoleTextAttribute(out, _color);
}
Start::Start()
{
	string temp_Menu[] = { "Get Max Flow", "Credits", "Quit" };
	int UserChoice = 0;
	int ShifTs = 0;
	for (int i = 0; i < 3; i++)
	{
		SetCursorToPosition(73, i + 20 + ShifTs, 10);
		if (UserChoice == i)
			SetCursorToPosition(73, i + 20 + ShifTs, 23);
		cout << temp_Menu[i];
		ShifTs += 2;
	}
	while (true)
	{
		if (GetAsyncKeyState(VK_DOWN))
		{
			if (UserChoice + 1 >= 3)
				continue;
			UserChoice += 1;
			/**************/
			ShifTs = 0;
			for (int i = 0; i < 3; i++)
			{
				SetCursorToPosition(73, i + 20 + ShifTs, 10);
				if (UserChoice == i)
					SetCursorToPosition(73, i + 20 + ShifTs, 23);
				cout << temp_Menu[i];
				ShifTs += 2;
			}
			/*************************/
			Sleep(100);
		}
		if (GetAsyncKeyState(VK_UP))
		{
			if (UserChoice - 1 < 0)
				continue;
			UserChoice -= 1;
			ShifTs = 0;
			for (int i = 0; i < 3; i++)
			{
				SetCursorToPosition(73, i + 20 + ShifTs, 10);
				if (UserChoice == i)
					SetCursorToPosition(73, i + 20 + ShifTs, 23);
				cout << temp_Menu[i];
				ShifTs += 2;
			}
			/*************************/
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_SPACE))
		{
			system("CLS");
			Choice = UserChoice;
			break;
		}
	}
}