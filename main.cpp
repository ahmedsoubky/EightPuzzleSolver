# include <iostream>
# include <vector>
# include <queue> 
# include <cmath>
 

using namespace std;


/* Input to the 3x3 puzzle as a 2D array */
int input[3][3] = { {7,2,0} ,{5,4,6} ,{8,3,1} };

/* The numbers in their correct positions to be used in the calculation of Manhattan distance */
int numbers[3][3] = { {0,1,2} ,{3,4,5},{6,7,8} } ;


class Node
{
public:
	int node[3][3];
	int g;
	int h;
	int f;
	Node* left;
	Node* right;
	Node* up;
	Node* down;

};

class mycomparison
{
  bool reverse;
public:
  mycomparison(const bool& revparam=false)
    {reverse=revparam;}
  bool operator() (const Node& lhs, const Node&rhs) const
  {
	  if (reverse) return (lhs.f>rhs.f);
    else return (lhs.f<rhs.f);
  }
};


vector< Node > tree(4);

typedef priority_queue< Node ,vector< Node >,mycomparison > queuetype;


int coordinates[] = {0,1};


void swap(Node* input,int i1,int j1,int i2, int j2) 
{
	int temp = input->node[i2][j2];
	input->node[i2][j2] = input->node[i1][j1];
	input->node[i1][j1] =temp;
}


/* Returns the correct (goal) coordinates of a given number x in the puzzle */
int* findCoordinates(int x)
{
	for( int i=0;i<3;++i)
	{
		for(int j=0;j<3;++j)
		{
			if( x == numbers[i][j])
			{
				coordinates[0] = i;
				coordinates[1] = j;
				return coordinates;
			}
		}
	}
}							

/* Calculates the Manhattan distance which is used to calculate the heuristic function */
int calcluateManhattanDistance ( int problem[][3], int i, int j )
{
	if ( problem[i][j] == 0)
	{
		return 0;
	}
	else
	{
		int  x = abs(findCoordinates(problem[i][j])[0] - i);
		int  y = abs(findCoordinates(problem[i][j])[1] - j);
		return  (x+y) ;
	}
}

/* Calculates the h-value for a given array */
int calculateHValue ( int problem[][3] ) 
{
	int result=0;
	for(int i=0;i<3;++i)
		for(int j=0;j<3;++j)
			result+=calcluateManhattanDistance(problem,i,j);
	return result;
}

/* Expands the current node into the possible successors*/
vector< Node > expand ( int input[][3] )
{
	Node outputLeft;
	Node outputRight;
	Node outputUp;
	Node outputDown;
	Node output;

	for( int i=0;i<3;++i)
	{
		for(int j=0;j<3;++j)
		{
			output.node[i][j] = input[i][j];
			outputRight.node[i][j] = input[i][j];
			outputLeft.node[i][j] = input[i][j];
			outputUp.node[i][j] = input[i][j];
			outputDown.node[i][j] = input[i][j];
		}
	}

	outputRight.h = 0;
	outputLeft.h = 0;
	outputUp.h = 0;
	outputDown.h = 0;

	outputRight.g = 0;
	outputLeft.g = 0;
	outputUp.g = 0;
	outputDown.g = 0;

	outputRight.f = 0;
	outputLeft.f = 0;
	outputUp.f = 0;
	outputDown.f = 0;

	int x,y;
	for (int i=0;i<3;++i)
	{
		for (int j=0;j<3;++j)
		{
			if( 0==input[i][j])
			{
				x = i;
				y = j;
				break;
			}
		}
	}

	/* right (i,j+1)*/
	if( !( (y+1) > 2) )
	{
		swap(&outputRight,x,y,x,y+1);
		for( int i=0;i<3;++i)
			for(int j=0;j<3;++j)
				cout<<outputRight.node[i][j]<<'\t';
		cout<<"right"<<endl;
		outputRight.g += 1;
		outputRight.h  = calculateHValue(outputRight.node);
		outputRight.f = outputRight.g + outputRight.h;
		tree.push_back(outputRight);
	}
		
	/* left (i,j-1)*/
	if ( !( (y-1) < 0 ) )
	{
		swap(&outputLeft,x,y,x,y-1);
		for( int i=0;i<3;++i)
			for(int j=0;j<3;++j)
				cout<<outputLeft.node[i][j]<<'\t';
		cout<<"left"<<endl;
		outputLeft.g +=1;
		outputLeft.h = calculateHValue(outputLeft.node);
		outputLeft.f = output.g + output.h;
		tree.push_back(outputLeft);
	}

	/* up (i-1,j) */
	if ( !( (x-1) < 0) ) 
	{
		swap(&outputUp,x,y,x-1,y);
		for( int i=0;i<3;++i)
			for(int j=0;j<3;++j)
				cout<<outputUp.node[i][j]<<'\t';
		cout<<"up"<<endl;
		outputUp.g += 1;
		outputUp.h = calculateHValue(outputUp.node);
		outputUp.f = outputUp.g + outputUp.h;
		tree.push_back(outputUp);
	}

	/* down (i+1,j) */ 
	if( !( (x+1) > 2) )
	{
		swap(&outputDown,x,y,x+1,y);
		for( int i=0;i<3;++i)
			for(int j=0;j<3;++j)
				cout<<outputDown.node[i][j]<<'\t';
		cout<<"down"<<endl;
		outputDown.g += 1;
		outputDown.h = calculateHValue(outputDown.node);
		outputDown.f = outputDown.g + outputDown.h;
		tree.push_back(outputDown);
	}
	
	for (vector< Node >::iterator it=tree.begin(); it!=tree.end(); ++it)
	{
		for( int i=0;i<3;++i)
			for(int j=0;j<3;++j)
				cout<<it->node[i][j]<<'\t';
		cout<<endl;
	}
	
	return tree;

}


/* Implementation of the A* algorithm based on the Manhattan distance heuristic */
int aStarAlgorithm ( int problem[][3] , int goal[][3])
{	
	//while(1)
	//{
		
		queuetype queue;
		
		expand(problem);
		
		for( vector < Node >::iterator i=tree.begin();i!=tree.end();++i)
		{
			queue.push((*i));
		}
		
		int i=1;
		
		while(!queue.empty())
		{
			cout<<i++<<'\t';
			Node tmp = queue.top();
			for(int i=0;i<3;++i)
			{
				for(int j=0;j<3;++j)
				{
					cout<<tmp.node[i][j]<<'\t';
				}
			}
			queue.pop();
			cout<<endl;
		}
	return 0;
}


void main ()
{

	cout<<"H(input):"<<calculateHValue(input)<<endl;

	aStarAlgorithm(input,numbers);
	
	for ( int i=0;i<9;++i)
		cout<<"Coordinates of "<<i<<'\t'<<'('<<findCoordinates(i)[0]<<','<<findCoordinates(i)[1]<<')'<<endl;


	for ( int i=0;i<3;++i)
	{
		for( int j=0;j<3;++j)
		{
			cout<<"Distance :"<<calcluateManhattanDistance(input,i,j)<<endl;
		}
	}

}

