// DMITRII OBIDEIKO
// DXO200006

#include <iostream>
#include <fstream>
#include <string>
#include "Graph.h"

using namespace std;

Vertex* parseGraphLine(string line)
{
    size_t start = 0;
    size_t end = line.find(" ");
    
    Vertex* ver = new Vertex(line.substr(start, end - start)); // finds location's name and creates a new vertex
    ver->addEdge(ver); // adds itself to its edges

    // while there are still words in the line
    // finds loc name and weight and then adds it to the list of edges the belong to ver
    while (end != string::npos)
    {
        start = end + 1;
        end = line.find(",", start);
        string locName = line.substr(start, end - start);
        
        start = end + 1;
        end = line.find(" ", start);
        double weight = stod(line.substr(start, end - start));

        ver->addEdge(new Vertex(locName, weight));
    }
    
    return ver;
}

void loadGraphFile(fstream &file, Graph &graph)
{
    // While end of file is not reached
    // Adds vertex to the graph
    while (!file.eof())
    {
        string line;
        getline(file, line);
        if (line.size() != 0) // if line is not empty
            graph.addVertex(parseGraphLine(line));
    }
}

// finds the total weight for traveling the entire path of vertexes
// returns true of the path valid; false otherwise
bool findWeight(vector<Vertex*> path, double &totWeight)
{
    Vertex* curVer = path.front();
        
    for (size_t i = 1; i < path.size(); i++)
    {
        Vertex* ed = curVer->findEdge(path[i]);
        if (ed != nullptr) // if such edge is found, the total weight increases
        {
            totWeight += ed->getWeight();
            curVer = path[i];
        }
        else // no such edge was found -> path does not exist
        {
            totWeight = 0;
            return false;
        }
    }
    
    return true;
}

void parseRouteLine(string line, Graph &graph, vector<Driver> &drList)
{
    size_t start = 0;
    size_t end = line.find(" ");
    string driverName = line.substr(start, end - start);
    vector<Vertex*> path;
    
    // loops until all locations' names are found
    while (end != string::npos)
    {
        start = end + 1;
        end = line.find(" ", start);
        
        string locName = line.substr(start, end - start);
        
        Vertex* ver = graph.findVertex(locName);
        if (ver != nullptr) // if a vertex with such loc name exists -> add it to the path
        {
            path.push_back(ver);
        }
        else // vertex with such name was not found -> path is not valid
        {
            drList.push_back(Driver(driverName, 0, false));
            return;
        }
    }
        
    double totWeight = 0;
    bool validPath = findWeight(path, totWeight); // finds the total weight of the path and checks if path is valid
    drList.push_back(Driver(driverName, totWeight, validPath)); // adds the driver to the driver list
}

void loadRoutsFile(fstream &file, Graph &graph, vector<Driver> &drList)
{
    // While end of file is not reached
    while (!file.eof())
    {
        string line;
        getline(file, line);
        if (line.size() != 0) // if line is not empty
            parseRouteLine(line, graph, drList);
    }
}

void sort(vector<Driver> &list)
{
    size_t maxInd = list.size() - 1; // marks the last index that is not sorted
    
    // sorts in ascending order by weight using bubble sort
    for (size_t i = 0; i < list.size(); i++) // how many times we loop through the list
    {
        for (size_t j = 0; j < maxInd; j++) // loops through elements that are not sorted
        {
            if (list[j] > list[j + 1]) // if next element is > then swap
            {
                Driver copy = list[j];
                list[j] = list[j + 1];
                list[j + 1] = copy;
            }
            else if (list[j] == list[j + 1]) //if weights are equal -> sort alphabetically
            {
                if (list[j].getName() > list[j+1].getName()) // if next element is > then swap
                {
                    Driver copy = list[j];
                    list[j] = list[j + 1];
                    list[j + 1] = copy;
                }
            }
        }
        maxInd--; // with each iteration, the element with the greatest weight gets sorted
    }
}

// Prints drivers in ascending order
void printDrivers(vector<Driver> &list)
{
    for (Driver dr : list)
    {
        cout << dr.getName() << " " << dr.getWeight() << " ";
        
        if (dr.hasValidPath())
            cout << "valid" << endl;
        else
            cout << "invalid" << endl;
    }
}

int main()
{
    Graph graph;
    vector<Driver> driverList;
  
    string graphFileName;
    string routsFileName;
    cin >> graphFileName;
    cin >> routsFileName;
    
    // Opens files with such file names
    fstream graphFile(graphFileName);
    fstream routesFile(routsFileName);
    
    if (graphFile.is_open()) // checks if file was found
    {
        loadGraphFile(graphFile, graph);
        graphFile.close();
    }
    else
        cout << "Graph file is not open" << endl;
    
    if (routesFile.is_open()) // checks if file was found
    {
        loadRoutsFile(routesFile, graph, driverList);
        routesFile.close();
    }
    else
        cout << "Routs file is not open" << endl;

    sort(driverList);
    printDrivers(driverList);
    
    return 0;
}
