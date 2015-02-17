#ifndef SIMULATOR_H
#define SIMULATOR_H
#include <QObject>
#include <QVector2D>
#include <vector>

class Simulator : public QObject
{
    Q_OBJECT
public:
    float        dt;
    float        scale;

    Simulator(QObject* parent = 0);
    ~Simulator();
    std::vector<QVector2D> particles;
    std::vector<float> velocities;
    void createParticles(unsigned int numberOfParticles);
public slots:
    void step();

signals:
    void stepCompleted();
};

#endif // SIMULATOR_H
