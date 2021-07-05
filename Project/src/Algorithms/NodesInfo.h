//
// Created by marhc on 10/05/2021.
//

#ifndef ENTREGA2_NODESINFO_H
#define ENTREGA2_NODESINFO_H

#include <iostream>
#include <cmath>
#include "Graph.h"

typedef enum {RECHARGE, DELIVERY, PICKUP, NONE, GARAGE} interest;

class NodeInfo {
    const double latitude{};
    const double longitude{};
    interest type;
public:
    NodeInfo(double latitude, double longitude) : latitude(latitude), longitude(longitude), type(NONE) {};

    double getLatitude() const {
        return latitude;
    }
    double getLongitude() const {
        return longitude;
    }
    interest getType() const {
        return type;
    }
    void setType(interest type) {
        this->type = type;
    }

    // Euclidian distance
    static double getDistance(double lat1, double long1, double lat2, double long2) {
        return std::sqrt(std::pow(lat1 - lat2, 2) + std::pow(long1 - long2, 2));
    }

    bool operator==(const NodeInfo& nodeInfo) const {
        return this->getLatitude() == nodeInfo.getLatitude() && this->getLongitude() == nodeInfo.getLongitude();
    }
    friend std::ostream& operator<<(std::ostream& os, const NodeInfo& nodeInfo) {
        os << nodeInfo.getLatitude() << "-" << nodeInfo.getLongitude();
        return os;
    }
};



#endif //ENTREGA2_NODESINFO_H
