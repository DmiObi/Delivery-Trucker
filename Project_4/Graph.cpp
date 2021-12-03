#include "Graph.h"

// Vertex Functions
Vertex::Vertex(string locName)
{
    m_locName = locName;
    m_weight = 0;
}
Vertex::Vertex(string locName, double weight)
{
    m_locName = locName;
    m_weight = weight;
}

string Vertex::getLocName() const { return m_locName;}
double Vertex::getWeight() const { return m_weight; }

void Vertex::addEdge(Vertex* edge) { m_edges.push_back(edge); }

void Vertex::deleteEdge(string &locName)
{
    for (size_t i = 0; i < m_edges.size(); i++)
    {
        if (m_edges[i] != nullptr) // checks if list is empty
        {
            if (m_edges[i]->getLocName() == locName) //checks if names match
            {
                delete m_edges[i]; // frees memory
                m_edges[i] = nullptr;
            }
        }
    }
}

// Gets an edge and finds weather the vertex has such edge
Vertex* Vertex::findEdge(Vertex* gEdge) const
{
    for (Vertex* edge : m_edges) // loops through vertex's edges
    {
        // if the edges' names match -> return the edge
        if (edge != nullptr && edge->getLocName() == gEdge->getLocName())
            return edge;
    }
    
    return nullptr; // no such edge was found
}

//Graph functions
Graph::Graph() {m_map = new HashMap(); }
void Graph::addVertex(Vertex* ver) { m_map->insertVertex(ver); }
void Graph::deleteVertex(string &locName) { m_map->deleteVertex(locName); }
Vertex* Graph::findVertex(string &locName) const { return m_map->findVertex(locName); } //finds vertex by location name

// Driver functions
Driver::Driver(string name, double weight, bool path)
{
    m_name = name;
    m_weight = weight;
    m_pathValid = path;
}

double Driver::getWeight() const { return m_weight; }
string Driver::getName() const { return m_name; }

bool Driver::hasValidPath() const { return m_pathValid; }

// the operators compare driver objects by their weight
bool Driver::operator<(Driver &dr) const { return getWeight() < dr.getWeight(); }
bool Driver::operator>(Driver &dr) const { return getWeight() > dr.getWeight(); }
bool Driver::operator==(Driver &dr) const { return getWeight() == dr.getWeight(); }

//HashTable functions
Graph::HashMap::HashMap()
{
    m_size = 5;
    m_array = new list<Vertex*>*[m_size];
    fillArray();
}

// Fills array with nullptr
void Graph::HashMap::fillArray()
{
    for (int i = 0; i < m_size ; i++)
        m_array[i] = nullptr;
}

double Graph::HashMap::avgLengOfChains() const
{
    size_t lengSum = 0; // sum of lengths of chains
    int chains = 0; // number of non empty lists
    
    for (int i = 0; i < m_size; i++)
    {
        if (m_array[i] != nullptr) // checks if list is empty
        {
            lengSum += m_array[i]->size();
            chains++;
        }
    }
    
    if (chains == 0)
        return 0;
    else
        return (double)lengSum / (double)chains;
}

//hashes a string based on vertex pointer
int Graph::HashMap::hashFunc(const Vertex* ver) const { return hashFunc(ver->getLocName()); }

// returns the index in the array based on the string
int Graph::HashMap::hashFunc(const string &locName) const
{
    long long hash = 0;
    int prime = 31;
    
    // Sums the askii value of each charater in the string
    for (size_t i = 0; i < locName.length(); i++)
    {
        hash += prime * hash + (long long)locName[i];
    }
    
    return hash % m_size;
}

// Doubles the size of array and inserts vertexes from previous graph
void Graph::HashMap::incArraySize()
{
    list<Vertex*>** prevArray = m_array; // points to prev array
    int prevSize = m_size;
    m_array = new list<Vertex*>*[m_size * 2];
    m_size *= 2;
    fillArray();
    
    // Goes through the previous array and adds vertexes to the new array
    for (int i = 0; i < prevSize; i++)
    {
        if (prevArray[i] != nullptr) // checks if list with vertexes is empty
        {
            for (auto vec = prevArray[i]->cbegin(); vec != prevArray[i]->cend(); vec++)
            {
                if (*vec != nullptr) // checks if vertex is empty
                    insertVertex(*vec);
            }
        }
    }
    
    delete[] prevArray; //frees memory
}

void Graph::HashMap::insertVertex(Vertex* ver)
{
    // checks is size should be increased 
    if (avgLengOfChains() >= LOAD_FACTOR)
        incArraySize();
    
    int ind = hashFunc(ver); // gets index in the array
    
    if (m_array[ind] == nullptr) // if the're no list with vertexes -> create one
        m_array[ind] = new list<Vertex*>;
    
    m_array[ind]->push_back(ver); // adds vertex to the list
}

void Graph::HashMap::deleteVertex(string &locName)
{
    Vertex* ver = findVertex(locName);
    
    if (ver != nullptr) //if vertex with such name is found -> delete it
    {
        int ind = hashFunc(locName); // finds index where list is located
        m_array[ind]->remove(ver); // removed the vertex from the list

    }
}

Vertex* Graph::HashMap::findVertex(string &locName) const
{
    int ind = hashFunc(locName);  // gets index in the array
    list<Vertex*>* verList = m_array[ind]; // points to the previous list
    
    if (verList != nullptr) // checks if such list exist
    {
        // traverses the linked list
        for (auto ver = verList->begin(); ver != verList->end(); ver++)
        {
            // if a vertex with such name is found -> returns the vertex
            if ((*ver)->getLocName() == locName)
                return (*ver);
        }
    }
    
    return nullptr; // no vertex with such location name was found
}

