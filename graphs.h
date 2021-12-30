#ifndef GRAPHIMPL_GRAPHS_H
#define GRAPHIMPL_GRAPHS_H

#include <cstdio>
#include <set>
#include <string>
#include <algorithm>
#include <map>
#include <iostream>

class WeightedGraph;

using charPair = std::pair<char, char>;
using cPairSet = std::set<charPair>;
using charSet = std::set<char>;

class TGraph {
    charSet vertices;
    cPairSet edges;

public:
    TGraph() {
        vertices = charSet();
        edges = cPairSet();
    }
    virtual ~TGraph();

    charSet& getVertices() { return vertices; }
    cPairSet& getEdges() { return edges; }
    virtual WeightedGraph& AsWeighted(unsigned default_weight);
    virtual const std::string ToString() { return "BASED"; }

protected:
    void addVer(char ver) { if('A' <= ver && ver <= 'Z') vertices.insert(ver); }

    void delVer(char ver) {
        cPairSet tmp;
        for(auto i: edges)
            if(i.first == ver || i.second == ver) tmp.insert(i);
        for(const auto& i: tmp)
            edges.erase(i);
        vertices.erase(ver);

    }

    void addEdge(char ver1, char ver2) {
        if('A' > ver1 || ver1 > 'Z' || 'A' > ver2 || ver2 > 'Z') return;
        vertices.insert(ver1);
        vertices.insert(ver2);
        if(ver1 <= ver2)
            edges.insert(std::pair(ver1, ver2));
        else
            edges.insert(std::pair(ver2, ver1));
    }

    void delEdge(char ver1, char ver2) {
        if(ver1 <= ver2)
            edges.erase(std::pair(ver1, ver2));
        else
            edges.erase(std::pair(ver2, ver1));
    }

public:
    virtual void newVer(char ver) {};
    virtual void remVer(char ver) {};
    virtual void newEdge(char a, char b) {};
    virtual void newEdge(char ver1, char ver2, unsigned wgh) {};
    virtual void remEdge(char a, char b) {};
    virtual void newUpper(char ver) {};
    virtual void newLower(char ver) {};
    virtual void remUpper(char ver) {};
    virtual void remLower(char ver) {};
    virtual unsigned getWeight(char a, char b) {};
    virtual void setWeight(char a, char b, unsigned w) {};
};

inline TGraph::~TGraph() {};


class SimpleGraph: public TGraph {
public:
    virtual void newVer(char ver) { addVer(ver); }
    virtual void remVer(char ver) { delVer(ver); }
    virtual void newEdge(char ver1, char ver2) { addEdge(ver1, ver2); }
    virtual void remEdge(char ver1, char ver2) { delEdge(ver1, ver2); }

    SimpleGraph(const cPairSet& edges, const charSet& extraVer = {}) {
        for(const auto& i: edges)
            addEdge(i.first, i.second);
        for(const auto& i: extraVer)
            addVer(i);
    }

    virtual const std::string ToString() {
        std::string tmp = "Simple    { ";
        for(const auto& i: getEdges()) {
            tmp += i.first;
            tmp += i.second;
            tmp += ", ";
        }
        tmp[tmp.length()-2] = ' ';
        tmp[tmp.length()-1] = '}';
        return tmp;
    }
};

class BipartiteGraph: public TGraph {
    charSet upper;
    charSet lower;

public:
    virtual const charSet& getUpper() { return upper; }
    virtual const charSet& getLower() { return lower; }

    BipartiteGraph(const charSet& vers1, const charSet& vers2) {
        for(const auto& i: vers1)
            newUpper(i);
        for(const auto& i: vers2)
            newLower(i);
    }

    virtual void newUpper(char ver) {
        if('A' > ver || ver > 'Z') return;
        upper.insert(ver);
        addVer(ver);
        for(const auto& i: lower)
            addEdge(ver, i);
    }

    virtual void newLower(char ver) {
        if('A' > ver || ver > 'Z') return;
        lower.insert(ver);
        addVer(ver);
        for(const auto& i: upper)
            addEdge(ver, i);
    }

    virtual void remUpper(char ver) { upper.erase(ver); delVer(ver); }
    virtual void remLower(char ver) { lower.erase(ver); delVer(ver); }

    virtual const std::string ToString() {
        std::string tmp = "Bipartite { ";
        for(const auto& i: getVertices()) {
            tmp += i;
            tmp += ", ";
        }
        tmp[tmp.length()-2] = ' ';
        tmp[tmp.length()-1] = '}';
        return tmp;
    }
};

class CompleteGraph: public TGraph {
public:
    virtual void newVer(char ver) {
        addVer(ver);
        for(const auto& i: getVertices())
            if(ver != i)
                addEdge(ver, i);
    }

    virtual void remVer(char ver) { delVer(ver); }

    CompleteGraph(const charSet& vers) {
        for(const auto& i: vers)
            newVer(i);
    }

    virtual const std::string ToString() {
        std::string tmp = "Complete  { ";
        for(const auto& i: getVertices()) {
            tmp += i;
            tmp += ", ";
        }
        tmp[tmp.length()-2] = ' ';
        tmp[tmp.length()-1] = '}';
        return tmp;
    }
};

class WeightedGraph: public TGraph {
    unsigned defaultWeight;
    std::map<charPair, unsigned> wMap;

public:
    virtual unsigned getDefWeight() { return defaultWeight; }
    virtual void setDefWeight(unsigned w) { defaultWeight = w; }

    virtual void newVer(char ver) { addVer(ver); }
    virtual void remVer(char ver) { delVer(ver); }
    virtual void newEdge(char ver1, char ver2, unsigned wgh) {
        addEdge(ver1, ver2);
        if(ver1 <= ver2)
            wMap.emplace(charPair(ver1, ver2), wgh);
        else
            wMap.emplace(charPair(ver2, ver1), wgh);
    }
    virtual void newEdge(char ver1, char ver2) { newEdge(ver1, ver2, defaultWeight); }
    virtual void remEdge(char ver1, char ver2) {
        delEdge(ver1, ver2);
        if(ver1 <= ver2)
            wMap.erase(charPair(ver1, ver2));
        else
            wMap.erase(charPair(ver2, ver1));
    }

    WeightedGraph(const cPairSet& edges, unsigned defWeight, const charSet& extraVer = {}): defaultWeight(defWeight) {
        for(const auto& i: edges)
            addEdge(i.first, i.second);
        for(const auto& i: getEdges())
            wMap.emplace(i, defWeight);
        for(const auto& i: extraVer)
            addVer(i);
    }

    virtual unsigned getWeight(char a, char b) {
        return a<b ? wMap[charPair(a, b)] : wMap[charPair(b, a)];
    }

    virtual void setWeight(char a, char b, unsigned w) {
        a<=b ? wMap[charPair(a, b)] : wMap[charPair(b, a)] = w;
    }

    virtual const std::string ToString() {
        std::string tmp = "Weighted  { ";
        for(const auto& i: getEdges()) {
            tmp += i.first;
            tmp += i.second;
            tmp += ": ";
            tmp += std::to_string(wMap[i]);
            tmp += ", ";
        }
        tmp[tmp.length()-2] = ' ';
        tmp[tmp.length()-1] = '}';
        return tmp;
    }
};

inline WeightedGraph& TGraph::AsWeighted(unsigned default_weight) {
    WeightedGraph tmp(getEdges(), default_weight, getVertices());
    return tmp;
}

#endif