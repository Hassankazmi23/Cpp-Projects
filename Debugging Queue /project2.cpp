// On some compilers this works as expected, but ...
// on some  .... try it and see what happens.
/*
	CSCI381 Summer 2014
	Project 2
	@Daisy Ramos

	The program compiled perfectly however the result of the expression does not depend on the order of evaluation 
    of their subexpressions for instance in
	cout << q.del() << q.del() << q.del();
    The << operator works on q.del() as a left argument thus 
	the output of the queue is "cba", at which it is guaranteed that all side effects of the previous 
    evaluations will have been performed, and no side effects from the following evaluations have yet 
    been performed. 
	*/

#include <iostream>
using namespace std;


class queue
{
private:
	char* s;
	int F, R, size;
public:
	queue(int i=4)
	{
		s= new char[i];
		size=i;
		F=R=-1;
	}

	~queue()
	{
		delete []s;
	}

	bool empty()
	{
		return F==R;
	}

/*	bool full()//this is for a non circular queue
	{
		if (F==-1 && R==size-1)
			return true;
		for (int i=0; i<=F; i++)
			s[i]=s[F+i+1];
		R=R-F-1;
		F=-1;
		return false;
	}*/

	bool full()
	{
		if ((R+1)%5==F)
			return true;
		return false;
	}


/*	void add (char c)//non circular add
	{
		if (full())
		{
			cout << "queue is full";
			exit(1);
		}
		R++;
		s[R]=c;
	}*/

	void add (char c)
	{
		if (full())
		{
			cout << "queue is full";
			exit(1);
		}
		R=(R+1)%size;
		s[R]=c;
	}

/*	char del()// non circular
	{
		if (empty())
		{
			cout << "queue empty";
			exit(1);
		}
		F++;
		return s[F];
	}*/

	char del()
	{
		if (empty())
		{
			cout << "queue empty";
			exit(1);
		}
		F=(F+1)%size;//only difference
		return s[F];
	}


};

int main()
{
	queue q;
	q.add('a');
	q.add('b');
	q.add('c');
	cout << q.del() << q.del() << q.del();
	cout<<endl;
	system("PAUSE");
	return 0;
}
