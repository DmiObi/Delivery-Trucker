#pragma once
#include <string>
#include <vector>
#include <list>

using namespace std;

class Driver
{
private:
    string m_name;
    double m_weight;
    bool m_pathValid;
public:
    Driver(string name, double weight, bool path);
    
    string getName() const;
    double getWeight() const;
    bool hasValidPath() const;
    
    bool operator<(Driver& dr) const;
    bool operator>(Driver& dr) const;
    bool operator==(Driver& dr) const;
};

class Vertex
{
private:
    string m_locName;
    vector<Vertex*> m_edges;
    double m_weight;
public:
    Vertex(string locName);
    Vertex(string locName, double weight);
    void addEdge(Vertex *edge);
    void deleteEdge(string &locName);
    string getLocName() const;
    double getWeight() const;
    Vertex* findEdge(Vertex* edge) const;
};

class Graph
{
private:
    class HashMap
    {
    private:
        const int LOAD_FACTOR = 4;
        list<Vertex*>** m_array;
        int m_size;
    public:
        HashMap();
        void fillArray();
        int hashFunc(const Vertex* ver) const;
        int hashFunc(const string &locName) const;
        void incArraySize();
        void insertVertex(Vertex*);
        double avgLengOfChains() const;
        void deleteVertex(string &locName);
        Vertex* findVertex(string &locName) const;
    };
    
    HashMap* m_map;
public:
    Graph();
    void addVertex(Vertex* ver);
    void deleteVertex(string &locName);
    Vertex* findVertex(string &locName) const;
};
