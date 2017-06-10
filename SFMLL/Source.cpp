#include"MaxFlow.cpp"
int main()
{
	Start StartMenu;
	if (StartMenu.Choice == 0)
	{
	  MaxFlow one;
	  one.draw();
	}
	else if (StartMenu.Choice == 1)
	{
		cout << "Ahmed Mohamed Zaher" << endl;
		system("pause");
	}
	else
	{
		return 0;
	}
}