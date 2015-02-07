# include <iostream>
# include <vector>
# include <queue> 
# include <cmath>
# include <algorithm>
# include <string>
# include <map>


using namespace std;


/* Input to the 3x3 puzzle as a 2D array */
int input[3][3] = { {1,6,4} ,{8,7,0} ,{3,2,5} };

/* The numbers in their correct positions to be used in the calculation of Manhattan distance */
int numbers[3][3] = { {0,1,2} ,{3,4,5},{6,7,8} } ;



typedef struct Node
{
	int tileset[3][3];
	float f;
	float g;
	float h;
	bool visited;
	inline bool operator==(const Node& x)
	{
		if( x.tileset[0][0] == this->tileset[0][0] && x.tileset[0][1] ==this->tileset[0][1] && x.tileset[0][2] ==this->tileset[0][2] \
			&& x.tileset[1][0]==this->tileset[1][0] && x.tileset[1][1] ==this->tileset[1][1] && x.tileset[1][2] ==this->tileset[1][2] \
			&& x.tileset[2][0] ==this->tileset[2][0]	&& x.tileset[2][1]==this->tileset[2][1] && x.tileset[2][2]==this->tileset[2][2] )
			return true;
		else
			return false;
	}

}Node;

struct LessThanF
{
  bool operator()(const Node& lhs, const Node& rhs) const
  {
    return lhs.f > rhs.f;
  }
};

typedef priority_queue< Node ,vector< Node >, LessThanF > queuetype;

int coordinates[] = {0,1};

vector< Node > visited_nodes;

void swap(Node* input,int i1,int j1,int i2, int j2) 
{
	int temp = input->tileset[i2][j2];
	input->tileset[i2][j2] = input->tileset[i1][j1];
	input->tileset[i1][j1] =temp;
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

bool isGoal( Node x )
{
	bool result = false;

	if( x.tileset[0][0] == 0 && x.tileset[0][1] ==1 && x.tileset[0][2] ==2 \
		&& x.tileset[1][0]==3 && x.tileset[1][1] ==4 && x.tileset[1][2] ==5 \
		&& x.tileset[2][0] ==6	&& x.tileset[2][1]==7 && x.tileset[2][2]==8 )
	{
		result = true;
	}


	return result;
}

void expand( Node& current , queuetype* p)
{
	queuetype pq;
	Node outputRight;
	Node outputLeft;
	Node outputUp;
	Node outputDown;
	string value;
	for( int i=0;i<3;++i)
	{
		for(int j=0;j<3;++j)
		{
			outputRight.tileset[i][j] = current.tileset[i][j];
			outputLeft.tileset[i][j] = current.tileset[i][j];
			outputUp.tileset[i][j] = current.tileset[i][j];
			outputDown.tileset[i][j] = current.tileset[i][j];
		}
	}


	int x=0,y=0;
	for (int i=0;i<3;++i)
	{
		for (int j=0;j<3;++j)
		{
			if( 0==current.tileset[i][j])
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
		{
			for(int j=0;j<3;++j)
			{
				cout<<outputRight.tileset[i][j]<<'\t';
			}
			cout<<endl;
		}
		value="right";
		outputRight.g = current.g + 1;
		outputRight.h = calculateHValue(outputRight.tileset);
		outputRight.f = outputRight.g + outputRight.h;
		cout<<outputRight.f<<endl;
		cout<<value<<endl;
		p->push(outputRight);
	}

	/* left (i,j-1)*/
	if ( !( (y-1) < 0 ) )
	{
		swap(&outputLeft,x,y,x,y-1);
		for( int i=0;i<3;++i)
		{
			for(int j=0;j<3;++j)
			{
				cout<<outputLeft.tileset[i][j]<<'\t';
			}
			cout<<endl;
		}
		value="left";
		outputLeft.g = current.g + 1;
		outputLeft.h = calculateHValue(outputLeft.tileset);
		outputLeft.f = outputLeft.g + outputLeft.h;
		cout<<value<<endl;
		p->push(outputLeft);
	}


	/* up (i-1,j) */
	if ( !( (x-1) < 0) ) 
	{
		swap(&outputUp,x,y,x-1,y);
		for( int i=0;i<3;++i)
		{
			for(int j=0;j<3;++j)
			{
				cout<<outputUp.tileset[i][j]<<'\t';
			}
			cout<<endl;
		}
		value="up";
		outputUp.g = current.g + 1;
		outputUp.h = calculateHValue(outputUp.tileset);
		outputUp.f = outputUp.g + outputUp.h;
		cout<<value<<endl;
		p->push(outputUp);
	}

	/* down (i+1,j) */ 
	if( !( (x+1) > 2) )
	{
		swap(&outputDown,x,y,x+1,y);
		for( int i=0;i<3;++i)
		{
			for(int j=0;j<3;++j)
			{
				cout<<outputDown.tileset[i][j]<<'\t';
			}
			cout<<endl;
			}
		value="down";
		outputDown.g = current.g + 1;
		outputDown.h = calculateHValue(outputDown.tileset);
		outputDown.f = outputDown.g + outputDown.h;
		cout<<value<<endl;
		p->push(outputDown);
	}
	cout<<"========================"<<value<<"============================="<<endl;
	
	//p = &pq;

}




/* Implementation of the A* algorithm based on the Manhattan distance heuristic */
int aStarAlgorithm ( int problem[][3] , int goal[][3])
{
	Node startNode;
	Node goalNode;
	Node currentNode;
	
	queuetype successors;
	queuetype fringe;

	for(int i=0;i<3;++i)
	{
		for(int j=0;j<3;++j)
		{
			startNode.tileset[i][j] = problem[i][j];
			goalNode.tileset[i][j] = goal[i][j];
			currentNode.tileset[i][j] = problem[i][j];
		}
	}
	
	startNode.g = 0;
	startNode.h = calculateHValue(startNode.tileset);
	startNode.f = startNode.g + startNode.h;


	currentNode.g = 0;
	currentNode.h = calculateHValue(startNode.tileset);
	currentNode.f = currentNode.g + currentNode.h;

		
	fringe.push(currentNode);


	while(!fringe.empty())
	{	
		currentNode = fringe.top();	
	

		if( currentNode == goalNode )
		{

			cout<<"Solution Reached"<<endl;

			for( int i=0;i<3;++i)
				for ( int j=0;j<3;++j)
					cout<<currentNode.tileset[i][j]<<' ';
			return fringe.size();
		}
		
		expand(currentNode,&successors);


		while( !successors.empty())
		{

			vector< Node>::iterator it;

			it = find(visited_nodes.begin(),visited_nodes.end(),successors.top());

			if( it!= visited_nodes.end() )
			{
				if( successors.top().f < it->f )
				{
					fringe.push( successors.top());
					visited_nodes.push_back(successors.top());
					successors.pop();
				}
				else
				{
					successors.pop();
					continue;
				}
			}
			else
			{
				
				fringe.push(successors.top());
				visited_nodes.push_back(successors.top());
				successors.pop();

			}

		}
		
		fringe.pop();

		cout<<"F"<<fringe.size()<<endl;
	}

}


void main ( )
{
	cout<<"Algorithm output:"<<aStarAlgorithm(input,numbers)<<endl;

}
