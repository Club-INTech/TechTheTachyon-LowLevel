//
// Created by trotfunky on 24/05/18.
//

#ifndef LL_POINTTOPOINTTRAJECTORY_H
#define LL_POINTTOPOINTTRAJECTORY_H

#include <vector>

class PointToPointTrajectory {

private:

    int counter;
    int size;
    const double* xTable;
    const double* yTable;

public:

    PointToPointTrajectory();
    PointToPointTrajectory(const double*,const double*,int);
    /**
     * Permet de redéfinir tous les points de la trajectoire.
     */
    void set(const double*, const double*, int);
    /**
     * Remet à 0 le nombre de points de la trajectoire pour la "désactiver"
     */
    void clear();
    std::pair<double,double> query();
    explicit operator bool();
};


#endif //LL_POINTTOPOINTTRAJECTORY_H
