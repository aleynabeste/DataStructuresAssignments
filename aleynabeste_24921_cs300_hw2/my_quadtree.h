#include <string>
#include <iostream>
#include <vector>
using namespace std;
//implementation is done in the header
vector<string> vec; //those vectors are to keep the track of points visited
vector<string> vecResult;

struct quadtreeNode //this struct has seven fields as specified in the document
{
	quadtreeNode* NW;
	quadtreeNode* NE;
	quadtreeNode* SW;
	quadtreeNode* SE;
	int x_coord;
	int y_coord;
	string name;
	quadtreeNode::quadtreeNode (int x_c, int y_c, string n)
		: NW(NULL), NE(NULL), SW(NULL), SE(NULL),  x_coord(x_c), y_coord(y_c), name(n) {} //constructor

};


class My_QuadTree{
	private:
		quadtreeNode*root;
		int x_boundary; //those are the boundaries of the 2d map
		int y_boundary;


		void insert (int x, int y, string n, quadtreeNode* &p) const //take the pointer as reference 
			//we call this private insert in the public insert by passing the root 
		{
			if (p==nullptr) //if it is null we create a new node
				p=new quadtreeNode(x,y,n);
			else if(x<p->x_coord && y<p->y_coord) //those quadrants are determined geometrically
				return insert(x, y, n , p->SW); //each time we recursively insert the node 
			else if(x<p->x_coord && y > p->y_coord)
				return insert(x,y,n,p->NW);
			else if (x>p->x_coord && y< p->y_coord)
				return insert(x,y,n,p->SE);
			else if (x>p->x_coord && y>p->y_coord)
				return insert(x,y,n,p->NE);
			else if (x>p->x_coord && y==p->y_coord) //left and bottom borders of each quadrant is closed
				return insert (x,y,n, p->NE);
			else if(x==p->x_coord && y < p->y_coord) //those equality cases are constructed according to the rule
				return insert(x,y,n,p->SE);
			else if (x<p->x_coord && y==p->y_coord)
				return insert (x,y,n,p->NW);
			else if (x==p->x_coord && y> p->y_coord)
				return insert(x,y,n,p->NE);
			else 
				return;
		}

		void pretty_print(quadtreeNode*p) 
		{
			if (p !=nullptr) //the pointer should be null to be printed
			{
				cout << p->name << endl;
				pretty_print(p->SE); //the ordering of the quadrant calls are given in the hw document
				pretty_print(p->SW);
				pretty_print(p->NE);
				pretty_print(p->NW);
			}
		}

		void search(int xc,int yc, int radius, quadtreeNode*&p)
		{
			if(p != NULL) 
			{
				double dist=distancecalculator(xc,yc,p->x_coord,p->y_coord); //first we calculate the distance
				if(dist>radius && p->x_coord< xc-radius && p->y_coord>yc+radius)//1st area
				{
					vec.push_back( p->name); //each time we visit a node, we keep the record of in in the vector
					search(xc, yc, radius, p->SE);
				}
				else if(dist>radius && p->x_coord>= xc-radius && p->x_coord<= xc+radius && p->y_coord>yc+radius)//2nd area
				{
					vec.push_back( p->name);
					search(xc, yc, radius, p->SE);
					search(xc, yc, radius, p->SW);
				}
				else if(dist>radius && p->x_coord> xc+radius && p->y_coord>yc+radius)//3rd area
				{
					vec.push_back( p->name);
					search(xc, yc, radius, p->SW);
				}
				else if(dist>radius && p->y_coord>= yc-radius && p->y_coord<= yc+radius && p->x_coord<xc-radius)//4th area
				{
					vec.push_back( p->name);
					search(xc, yc, radius, p->SE);
					search(xc, yc, radius, p->NE);
				}
				else if(dist>radius && p->y_coord>= yc-radius && p->y_coord<= yc+radius && p->x_coord>xc+radius)//5th area
				{
					vec.push_back( p->name);
					search(xc, yc, radius, p->SW);
					search(xc, yc, radius, p->NW);
				}
				else if(dist>radius && p->x_coord< xc-radius && p->y_coord<yc-radius)//6th area
				{
					vec.push_back( p->name);
					search(xc, yc, radius, p->NE);
				}
				else if(dist>radius && p->x_coord>= xc-radius && p->x_coord<= xc+radius && p->y_coord<yc-radius)//7th area
				{
					vec.push_back( p->name);
					search(xc, yc, radius, p->NE);
					search(xc, yc, radius, p->NW);
				}
				else if(dist>radius && p->x_coord> xc+radius && p->y_coord<yc-radius)//8th area
				{
					vec.push_back( p->name);
					search(xc, yc, radius, p->NW);
				}
				else if(dist>radius && p->x_coord>= xc-radius && p->x_coord< xc && p->y_coord<=yc+radius && p->y_coord>yc)//9th area
				{
					vec.push_back( p->name);
					search(xc, yc, radius, p->SE);
					search(xc, yc, radius, p->SW);
					search(xc, yc, radius, p->NE);
				}
				else if(dist>radius && p->x_coord<= xc+radius && p->x_coord> xc && p->y_coord<=yc+radius && p->y_coord>yc)//10th area
				{
					vec.push_back( p->name);
					search(xc, yc, radius, p->SE);
					search(xc, yc, radius, p->SW);
					search(xc, yc, radius, p->NW);
				}
				else if(dist>radius && p->x_coord>= xc-radius && p->x_coord< xc && p->y_coord>=yc-radius && p->y_coord<yc)//11th area
				{
					vec.push_back( p->name);
					search(xc, yc, radius, p->SE);
					search(xc, yc, radius, p->NE);
					search(xc, yc, radius, p->NW);
					
				}
				else if(dist>radius &&  p->x_coord<= xc+radius && p->x_coord> xc && p->y_coord>=yc-radius && p->y_coord<yc)//12 area
				{
					vec.push_back( p->name);
					search(xc, yc, radius, p->SW);
					search(xc, yc, radius, p->NE);
					search(xc, yc, radius, p->NW);

				}
				else if(dist<=radius)//13th area: where the point is inside the search circle
				{
					vec.push_back( p->name);
					vecResult.push_back( p->name); //since those points are the answers, we store them also in a result vector
					search(xc, yc, radius, p->SE);
					search(xc, yc, radius, p->SW);//we should check all children of them because they will be in the circle too
					search(xc, yc, radius, p->NE);
					search(xc, yc, radius, p->NW);
	
				}

			}

		}

		void pretty_delete(quadtreeNode*&p) //take the route as reference
		{

			if (p !=nullptr)
			{
				//cout << p->name << "deleted"<< endl;
				pretty_delete(p->SE);//resursively deleted
				pretty_delete(p->SW);
				pretty_delete(p->NE);
				pretty_delete(p->NW);
				delete p;
				p=nullptr; //to prevent dangling
			}
		
		}

	public:
		My_QuadTree(int x, int y)
		{
			root=nullptr;
			x_boundary=x;
			y_boundary=y;
		} //Constructor

		~My_QuadTree()  //destructor, to prevent memory leak
		{
			delete_tree();
		}

		void insert ( int x, int y,string n) //public insert
		{
			if(inSpace(x,y))
				insert(x,y,n,root);
			//else
				//cout << "Point coordinates out of 2D space" << endl;
		}

		void delete_tree()//public delete
		{
			pretty_delete(root);

		}

		bool inSpace(int x, int y)//this function is to check whether given coordinates are inside the 2d map
		{ 
			return (x<= x_boundary && 
			x>=0 && 
			y >= 0&& 
			y<= y_boundary); 
		} 

		void print()//public print
		{
			if(root==nullptr) //if tree is empty no need to call private print method
				cout <<"tree is empty" << endl;
			else
				pretty_print(root);
			cout << "\n" ;
		}

		double distancecalculator(int x1,int y1,int x2, int y2) //this function calculates the distance btw two points
		{
			    // Calculating distance 
				return sqrt(pow(x2 - x1, 2) +  
                pow(y2 - y1, 2) * 1.0); 
		}

		void search(int xc,int yc, int radius) //public search, also we deal with printing the result here
		{
			vec.clear();
			vecResult.clear();
			search(xc,yc,radius, root);
			for (size_t i = 0; i < vecResult.size(); i++ )
			{
				if (i == vecResult.size() -1 )
					cout << vecResult[i] << endl;
				else
					cout << vecResult[i] << ", ";
			}
			if(vecResult.size() ==0) //if no point is found in the circle
			{
				cout << "<None>" << endl;
			}
			for (size_t i = 0; i < vec.size(); i++ )
			{
				if (i == vec.size() -1 )
					cout << vec[i] << endl;
				else
					cout << vec[i] << ", ";
			}
			cout << "\n" ;
		}


};