#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>
#include <cmath>
#include <algorithm>

class Graph {
public:
    Graph(int dimension = 0);
    int getMatrixAdjElement(int i, int j);
    int getMatrixShimElement(int i, int j);
    int getMatrixReachElement(int i, int j);
    int getDimension();
    bool getIsCreated() const;
    void setIsCreated(bool value);
    void fillMatrixAdj(int dimension);
    void fillMatrixShim(int length, int mode);
    void fillMatrixReach();
    void writeDotFile(const char* fname);
private:
    bool isCreated;
    QVector<QVector<int>>* matrixAdj;
    QVector<QVector<int>>* matrixShim;
    QVector<QVector<int>>* matrixReach;
    QVector<QVector<int>> multShimbell(QVector<QVector<int>> firstMatrix, QVector<QVector<int>> secondMatrix, int mode);
    void sumReach(QVector<QVector<int>>* matrix);
    void mMult(QVector<QVector<int>>* matrixF, QVector<QVector<int>>* matrixS);
};

int poissonRNG();

#endif // GRAPH_H
