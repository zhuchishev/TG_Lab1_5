#include "graph.h"
#include <random>
#include <fstream>

int poissonRNG() {
    int result;
    rand();

    int paramU = 3;
    double p = exp(-paramU);
    int x = 0;
    double randN = (rand() % (30000 - 1) + 1) / 30000.;

    randN = randN - p;
    while (randN >= 0) {
        x += 1;
        p = (p * paramU) / x;
        randN = randN - p;
    }
    result = x;

    return result;
}

Graph::Graph(int dimension) {
    if (dimension < 2) isCreated = false;
    else {
        isCreated = true;
        matrixAdj = new QVector<QVector<int>>;
        matrixShim = new QVector<QVector<int>>;
        matrixReach = new QVector<QVector<int>>;
        for (int i = 0; i < dimension; i++) {
            matrixAdj->push_back(QVector<int>(dimension, 0));
            matrixShim->push_back(QVector<int>(dimension, 0));
            matrixReach->push_back(QVector<int>(dimension, 0));
        }
    }
}

int Graph::getMatrixAdjElement(int i, int j) {
    return (*matrixAdj)[i][j];
}

int Graph::getMatrixShimElement(int i, int j) {
    return (*matrixShim)[i][j];
}

int Graph::getMatrixReachElement(int i, int j) {
    return (*matrixReach)[i][j];
}

bool Graph::getIsCreated() const
{
    return isCreated;
}

void Graph::setIsCreated(bool value)
{
    isCreated = value;
}

void Graph::fillMatrixAdj(int dimension) {
    QVector<int> distribution;
    for (int i = 0; i < dimension - 2; i++) {
        distribution.append(poissonRNG());
    }
    (*matrixAdj)[dimension - 2][dimension - 1] = rand()%26 + 5;
    if (dimension > 2) {
        std::sort(distribution.begin(), distribution.end(), std::greater<int>());
        QVector<int> shuffledVector;
        for (int i = 0; i < dimension; i++) {
            shuffledVector.append(i);
        }
        for (int nRow = 0; nRow < dimension - 2; nRow++) {
            std::random_shuffle(shuffledVector.begin(), shuffledVector.end());
            if (distribution[nRow] == 0) distribution[nRow] = 1;
            if (distribution[nRow] > dimension - 1 - nRow) distribution[nRow] = dimension - 1 - nRow;
            int nFound = 0;
            for (int j = 0; ((j < dimension) && (nFound < distribution[nRow])); j++) {
                if (shuffledVector[j] > nRow) {
                    (*matrixAdj)[nRow][shuffledVector[j]] = rand()%26 + 5;
                    nFound++;
                }
            }
        }
    }
}

QVector<QVector<int>> Graph::multShimbell(QVector<QVector<int>> firstMatrix, QVector<QVector<int>> secondMatrix, int mode) {
    QVector<QVector<int>> resultMatrix;
    for (int i = 0; i < this->getDimension(); i++) {
        resultMatrix.push_back(QVector<int>(this->getDimension(), 0));
    }
    QVector<int> resultBuffer;
    bool zeroResult;
    for (int i = 0; i < firstMatrix.size(); i++) {
        for (int j = 0; j < firstMatrix.size(); j++) {
            resultBuffer.clear();
            zeroResult = true;
            for (int k = 0; k < firstMatrix.size(); k++) {
                if ((firstMatrix[i][k] != 0) && (secondMatrix[k][j] != 0)) {
                    resultBuffer.append(firstMatrix[i][k] + secondMatrix[k][j]);
                    zeroResult = false;
                }
            }
            if (!zeroResult) {
                if (mode == 0) {
                    resultMatrix[i][j] = *std::min_element(resultBuffer.begin(), resultBuffer.end());
                }
                else {
                    resultMatrix[i][j] = *std::max_element(resultBuffer.begin(), resultBuffer.end());
                }
            }
        }
    }
    return resultMatrix;
}

void Graph::sumReach(QVector<QVector<int>> *matrix)
{
    for (int i = 0; i < this->getDimension(); i++) {
        for (int j = 0; j < this->getDimension(); j++) {
            (*matrixReach)[i][j] += (*matrix)[i][j];
        }
    }
}

void Graph::mMult(QVector<QVector<int>> *matrixF, QVector<QVector<int>>* matrixS)
{
    QVector<QVector<int>>* matrixBuf = new QVector<QVector<int>>;
    for (int i = 0; i < this->getDimension(); i++) {
        matrixBuf->push_back(QVector<int>(this->getDimension(), 0));
    }
    for (int i = 0; i < this->getDimension(); i++) {
        for (int j = 0; j < this->getDimension(); j++) {
            for (int k = 0; k < this->getDimension(); k++) {
                (*matrixBuf)[i][j] += (*matrixF)[i][k] * (*matrixS)[k][j];
            }
        }
    }
    for (int i = 0; i < matrixF->size(); i++) {
        for (int j = 0; j < matrixF->size(); j++) {
            (*matrixF)[i][j] = (*matrixBuf)[i][j];
        }
    }
}

void Graph::fillMatrixShim(int length, int mode) {
    for (int i = 0; i < this->getDimension(); i++) {
        for (int j = 0; j < this->getDimension(); j++) {
            (*matrixShim)[i][j] = (*matrixAdj)[i][j];
        }
    }
    for (int i = 0; i < length - 1; i++) {
        *matrixShim = multShimbell(*matrixShim, *matrixAdj, mode);
    }
}

void Graph::fillMatrixReach() {
    for (int i = 0; i < this->getDimension(); i++) {
        (*matrixReach)[i][i] = 1;
    }
    QVector<QVector<int>>* matrixBuf = new QVector<QVector<int>>;
    QVector<QVector<int>>* matrixCoef = new QVector<QVector<int>>;
    for (int i = 0; i < this->getDimension(); i++) {
        matrixBuf->push_back(QVector<int>(this->getDimension(), 0));
        matrixCoef->push_back(QVector<int>(this->getDimension(), 0));
    }
    for (int i = 0; i < this->getDimension(); i++) {
        for (int j = 0; j < this->getDimension(); j++) {
            if ((*matrixAdj)[i][j] != 0) {
                (*matrixBuf)[i][j] = 1;
                (*matrixCoef)[i][j] = 1;
            }
        }
    }
    this->sumReach(matrixBuf);
    for (int i = 0; i < this->getDimension() - 2; i++) {
        this->mMult(matrixBuf, matrixCoef);
        this->sumReach(matrixBuf);
    }
}

int Graph::getDimension() {
    return matrixAdj->size();
}

void Graph::writeDotFile(const char *fname) {
    std::ofstream out(fname);
    out << "digraph {\n";
    for (int i = 0; i < matrixAdj->size(); i++) {
       out << i + 1 << ";\n";
    }
    for (int i = 0; i < matrixAdj->size(); i++) {
        for (int j = i; j < matrixAdj->size(); j++) {
            if ((*matrixAdj)[i][j] != 0) {
                out << i + 1 << " -> " << j + 1 << "[label=\"" << (*matrixAdj)[i][j] << "\"];\n";
            }
        }
    }
    out << "}\n";
}
