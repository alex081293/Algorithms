#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <iomanip>
#include <string>
#include <iostream>
#include <cmath>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/pending/indirect_cmp.hpp>

using namespace std;
using namespace boost;

// Enums
#define BLUE 100
#define RED 200
#define GOAL 300
#define NORTH 8
#define NORTHWEST 7
#define NORTHEAST 9
#define EAST 6
#define SOUTHEAST 3
#define SOUTH 2
#define SOUTHWEST 1
#define WEST 4

// Declares dfs_time_vistor class
template < typename TimeMap > class dfs_time_visitor:public default_dfs_visitor {
  typedef typename property_traits < TimeMap >::value_type T;
public:
  dfs_time_visitor(TimeMap dmap, TimeMap fmap, T & t)
:  m_dtimemap(dmap), m_ftimemap(fmap), m_time(t) {
  }
  template < typename Vertex, typename Graph >
    void discover_vertex(Vertex u, const Graph & GRAPH) const
  {
    put(m_dtimemap, u, m_time++);
  }
  template < typename Vertex, typename Graph >
    void finish_vertex(Vertex u, const Graph & GRAPH) const
  {
    put(m_ftimemap, u, m_time++);
  }
  TimeMap m_dtimemap;
  TimeMap m_ftimemap;
  T & m_time;
};

// Arrow struct
struct arrow{
    int direction;
    int color;
    int position;    
};

int main() {
    typedef adjacency_list<vecS, vecS, bidirectionalS> graphMaze; 
    typedef graph_traits < graphMaze >::vertices_size_type size_type;
    typedef graph_traits < graphMaze >::vertex_descriptor Vertices;
    int numOfSets;
    int completedSets = 0;
    int lineCount = 0;
    int nextSpace = 2;
    int i, k, j;
    int positionCount = 0;
    static int yCount = 0;
    int completeFlag = 0;
    int gridLength; 
    vector<arrow> maze;
    graphMaze GRAPH;
    string line;
    int flag = -1;
    ifstream myfile("input.txt");
    ofstream writeFile("Moran.txt");
    //Checks to make sure the file is open
    if(myfile.is_open()) {
        // Reads the next line until the file is empty
        while(getline(myfile, line)) {
            lineCount++;
            // If it is the first line, then we know it's the number of sets
            if(lineCount == 1) numOfSets = atoi(line.c_str());
            // if the line count is the next space, then we know to do no reading
            else if(lineCount == nextSpace) {
                flag = 1;
            }
            // If the last line was a space, the next line is the grid length
            else if(flag == 1) {
                flag = 0;
                gridLength = atoi(line.c_str());
                graphMaze newGraph(gridLength*gridLength);
                GRAPH = newGraph;
                nextSpace = lineCount + gridLength+1;
            }
            // Reads in the rows
           else if(lineCount < nextSpace) {
                int flag = 0;
                arrow newArrow;
                stringstream s(line);
                vector <string> temp;
                // While the string is not empty
                while(!s.eof()) {
                    int value;
                    string tmp;
                    s >> tmp;
                    // Splits the string up by '-'
                    string delimiter = "-";
                    size_t pos = 0;
                    string token;

                    while((pos = tmp.find(delimiter)) != string::npos) {

                        token = tmp.substr(0, pos);
                        // Depending on the first token, gives arrow a direction
                        if(token == "N") newArrow.direction = NORTH;
                        else if(token == "E") newArrow.direction = EAST;
                        else if(token == "S") newArrow.direction = SOUTH;
                        else if(token == "W") newArrow.direction = WEST;
                        else if(token == "NE") newArrow.direction = NORTHEAST;
                        else if(token == "NW") newArrow.direction = NORTHWEST;
                        else if(token == "SE") newArrow.direction = SOUTHEAST;
                        else if(token == "SW")  newArrow.direction = SOUTHWEST;
                        else flag = 1;

                        tmp.erase(0, pos+delimiter.length());
                    }
                    // arrow then gets it color
                    if(tmp == "B") newArrow.color =  BLUE;
                    else if(tmp == "R") newArrow.color = RED;
                    else flag = 1;
                    // and it's position
                    newArrow.position = positionCount;
                    // and then is inserted into the maze
                    if(flag == 0) maze.push_back(newArrow);
                    positionCount++;
                }
            }
            // When we have finished reading in all of the rows of the maze
            if(lineCount == nextSpace-1 && flag != -1) {
                // Insert the last arrow
                arrow lastArrow;
                lastArrow.color = GOAL;
                lastArrow.direction = 10;
                lastArrow.position = gridLength*gridLength-1;
                maze.push_back(lastArrow);
                // go through each arrow in the maze
                for(i=0; i<gridLength*gridLength; i++) {
                    // Based on it's direction, find all possible connections and add an edge to the graph
                    switch(maze[i].direction) {
                        // East
                        case EAST:
                            for(k = i + 1; ; k++) {
                                if((bool)(k % gridLength == 0)) break;
                                if(maze[i].color != maze[k].color) add_edge(i, k, GRAPH); 
                            }
                            break;
                        // West
                        case WEST:
                            for(k = i - 1; ; k--) {
                                if(maze[i].color != maze[k].color) add_edge(i, k, GRAPH);  
                                if((bool)(k % gridLength == 0)) break;
                            }
                            break;
                        // North
                        case NORTH:
                            for(k = i - gridLength; ; k-=gridLength) {
                                if((bool)(k < 0) == 1) break;
                                if(maze[i].color != maze[k].color) add_edge(i, k, GRAPH);                                 
                            }
                            break;
                        // South
                        case SOUTH:  
                            for(k = i + gridLength; ; k+=gridLength) {
                                if((bool)(k > gridLength*gridLength)) break;
                                if(maze[i].color != maze[k].color) add_edge(i, k, GRAPH);                                 
                            } 
                            break;
                        // North East
                        case NORTHEAST: 
                            for(k = i - gridLength + 1; ; k = k - gridLength + 1) {
                                if((bool)((k < 0) || (k % gridLength == 0))) break;
                                if(maze[i].color != maze[k].color) add_edge(i, k, GRAPH);                                 
                            } 
                            break;
                        // South East
                        case SOUTHEAST:   
                            for(k = i + gridLength + 1; ; k = k + gridLength + 1) {
                                if((bool)((k > gridLength*gridLength) || (k % gridLength == 0))) break;   
                                if(maze[i].color != maze[k].color) add_edge(i, k, GRAPH);                               
                            } //
                            break;
                        // South West
                       case SOUTHWEST:  
                            for(k = i + gridLength - 1; ; k = k + gridLength - 1) {
                                if(maze[i].color != maze[k].color) add_edge(i, k, GRAPH);                               
                                if((bool)((k > gridLength*gridLength) || (k % gridLength == 0))) break;
                            } 
                            break;
                        // North West
                        case NORTHWEST:   
                            for(k = i - gridLength - 1; ; k = k - gridLength - 1) {
                                if((bool)(k < 0)) break; 
                                if(maze[i].color != maze[k].color) add_edge(i, k, GRAPH); 
                                if((k % gridLength == 0)) break;                              
                            } 
                            break;
                        default:
                            break;                       
                    } 
                }
                
                // create a vector of Vertices to hold the predecessor array
                vector<Vertices> pred(gridLength*gridLength*1000);  

                // run depth_first_search on the graph and records the predecssor
                depth_first_search(GRAPH,visitor(make_dfs_visitor(record_predecessors(&pred[0], on_tree_edge()))));
                
                Vertices pathV = gridLength*gridLength-1;       
                vector<Vertices> path;
                path.push_back(pathV);
                
                while(pathV != 0) {
                    pathV = pred[pathV];
                    path.push_back(pathV);
                }

                string printOut;
                int start = 0;
                // reverse the path to get into correct order 
                reverse(path.begin(), path.end()); 
                
                // for each position in the path
                for(i=0; i<gridLength*gridLength-1; i++) {
                    int count = 0;
                    // if we reach the goal or run out of index, break
                    if(maze[path[i]].position >= (gridLength*gridLength-1) || 
                        maze[path[i]].position < 0 || maze[path[i]].color == GOAL) break;
                    // Grabs the equivalent cord position of the current item and next item
                    // this is done for ease of logic during the count
                    int xCord = (maze[path[i]].position / gridLength);
                    int yCord = (maze[path[i]].position % gridLength);
                    int xCord2 = (maze[path[i+1]].position / gridLength);
                    int yCord2 = (maze[path[i+1]].position % gridLength);
                    // Makes sure we start at the first position                   
                    if(start != 1) {
                        if (xCord == 0 && yCord == 0) start = 1;
                    } 

                    if(start == 1) {   
                        switch(maze[path[i]].direction) {
                            case NORTH: 
                                count = xCord - xCord2;
                                printOut = "N-";
                                break;
                            case SOUTH:
                                count = xCord2 - xCord;
                                printOut = "S-";
                                break;
                            case WEST: 
                                count = yCord2 - yCord;
                                printOut = "W-";
                                break;
                            case EAST: 
                                count = yCord - yCord2;
                                printOut = "E-" ;
                                break;                        
                            case NORTHEAST: 
                                count = xCord - xCord2;
                                printOut = "NE-"; 
                                break;
                            case SOUTHEAST: 
                                count = xCord2 - xCord;
                                printOut = "SE-";
                                break;
                            case SOUTHWEST: 
                                count = xCord2 - xCord;
                                printOut = "SW-"; 
                                break;
                            case NORTHWEST: 
                                count = xCord - xCord2;
                                printOut = "NW-";
                                break;
                            default:
                                break;                            
                        }
                        writeFile << printOut << abs(count) << " "; 
                    }
                }
                
                if(++completedSets != numOfSets) writeFile << "\n\n";
                maze.clear();
                path.clear();
                positionCount = 0;
                completeFlag = 0;
                yCount = 0; 
            
            }

        }
    }
    myfile.close();
    writeFile.close();
    return 0;
}