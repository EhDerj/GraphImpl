#include "graphs.h"

BipartiteGraph& operator+(BipartiteGraph& gr1, BipartiteGraph& gr2) {
    BipartiteGraph tmp = gr1;
    for(const auto& i: gr2.getUpper())
        tmp.newUpper(i);
    for(const auto& i: gr2.getLower())
        tmp.newLower(i);
    return tmp;
}

CompleteGraph& operator+(CompleteGraph& gr1, CompleteGraph& gr2) {
    CompleteGraph tmp = gr1;
    for(const auto& i: gr2.getVertices())
        tmp.newVer(i);
    return tmp;
}

WeightedGraph& operator+(WeightedGraph& gr1, WeightedGraph& gr2) {
    WeightedGraph tmp = gr1;
    for(const auto& i: gr2.getEdges())
        if(tmp.getEdges().find(i) != tmp.getEdges().end())
            tmp.setWeight(i.first, i.second,
                tmp.getWeight(i.first, i.second) < gr2.getWeight(i.first, i.second) ?
                tmp.getWeight(i.first, i.second) : gr2.getWeight(i.first, i.second)
            );
        else
            tmp.newEdge(i.first, i.second, gr2.getWeight(i.first, i.second));
    return tmp;
}

template <class Graph>
void operator+(WeightedGraph& gr1, Graph& gr2) {
    throw std::invalid_argument("Weighted graphs can only be summed with other weighted graph. Use '.AsWeight()'");
}

template <class Graph>
void operator+(Graph& gr1, WeightedGraph& gr2) {
    throw std::invalid_argument("Weighted graphs can only be summed with other weighted graph. Use '.AsWeight()'");
}

template <class Graph1, class Graph2>
SimpleGraph& operator+(Graph1& gr1, Graph2& gr2) {
    SimpleGraph tmp(gr1.getEdges(), gr1.getVertices());

    for(const auto& i: gr2.getEdges())
        tmp.newEdge(i.first, i.second);
    for(const auto& i: gr2.getVertices())
        tmp.newVer(i);
    return tmp;
}

BipartiteGraph& operator-(BipartiteGraph& gr1, BipartiteGraph& gr2) {
    BipartiteGraph tmp = gr1;
    for(const auto& i: gr2.getUpper())
        tmp.remUpper(i);
    for(const auto& i: gr2.getLower())
        tmp.remLower(i);
    return tmp;
}

CompleteGraph& operator-(CompleteGraph& gr1, CompleteGraph& gr2) {
    CompleteGraph tmp = gr1;
    for(const auto& i: gr2.getVertices())
        tmp.remVer(i);
    return tmp;
}

template <class Graph>
WeightedGraph& operator-(WeightedGraph& gr1, Graph& gr2) {
    WeightedGraph tmp = gr1;
    for(const auto& i: gr1.getEdges())
        tmp.remEdge(i.first, i.second);
    return tmp;
}

template <class Graph1, class Graph2>
SimpleGraph& operator-(Graph1& gr1, Graph2& gr2) {
    SimpleGraph tmp(gr1.getEdges(), gr1.getVertices());
    for(const auto& i: gr2.getEdges())
        tmp.remEdge(i.first, i.second);
    return tmp;
}