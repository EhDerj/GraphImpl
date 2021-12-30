#ifndef GRAPHIMPL_GRAPHFACTORY_H
#define GRAPHIMPL_GRAPHFACTORY_H

#include "graphs.h"
#include <memory>

class GraphFactory {

    class TCreator {
    public:
        virtual std::unique_ptr<TGraph> create(charSet upper, charSet lower) {}
        virtual std::unique_ptr<TGraph> create(charSet vers) {}
        virtual std::unique_ptr<TGraph> create(cPairSet edges) {}
        virtual std::unique_ptr<TGraph> create(cPairSet edges, unsigned wgh) {}
    };

    class BipartiteCreator: public TCreator {
        virtual std::unique_ptr<TGraph> create(charSet upper, charSet lower) { return std::make_unique<BipartiteGraph>(upper, lower); }
    };

    class CompleteCreator: public TCreator {
        virtual std::unique_ptr<TGraph> create(charSet vers) { return std::make_unique<CompleteGraph>(vers); }
    };

    class SimpleCreator: public TCreator {
        virtual std::unique_ptr<TGraph> create(cPairSet edges) {}
    };

    class WeightedCreator: public TCreator {
        virtual std::unique_ptr<TGraph> create(cPairSet edges, unsigned wgh) {}
    };

    std::map<std::string, std::unique_ptr<TCreator>> shops;

public:
    GraphFactory() {
        shops["bipartite"] = std::make_unique<BipartiteCreator>();
        shops["complete"]  = std::make_unique<CompleteCreator>();
        shops["simple"]    = std::make_unique<SimpleCreator>();
        shops["weighted"]  = std::make_unique<WeightedCreator>();
    }

    std::unique_ptr<TGraph> create(const std::string& type, charSet& upper, charSet& lower) {
        return shops[type]->create(upper, lower);
    }
    std::unique_ptr<TGraph> create(const std::string& type, charSet& vers) {
        return shops[type]->create(vers);
    }
    std::unique_ptr<TGraph> create(const std::string& type, cPairSet& edges) {
        return shops[type]->create(edges);
    }
    std::unique_ptr<TGraph> create(const std::string& type, cPairSet& edges, unsigned wgh) {
        return shops[type]->create(edges, wgh);
    }
};



#endif