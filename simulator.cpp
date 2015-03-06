#include "simulator.h"
#include "random.h"
#include <cmath>
#include <iostream>
#include <functional>
#include <QDebug>
using std::cout; using std::endl; using std::function;

Simulator::Simulator(QObject *parent) :
    QObject(parent)
{
    dt = 0.001;
    scale = 0.1;
    createParticles(500000);
}


Simulator::~Simulator()
{

}

void Simulator::createParticles(unsigned int numberOfParticles) {
    float sqrtOfNumParticles = sqrt(numberOfParticles);
    for(unsigned int i=0; i<sqrtOfNumParticles; i++) {
        float x = 2*(float(i) - sqrtOfNumParticles*0.5) / sqrtOfNumParticles;

        for(unsigned int j=0; j<sqrtOfNumParticles; j++) {
            // This is really the momentum, the mass is 1
            float v = (float(j) - sqrtOfNumParticles*0.5) / sqrtOfNumParticles;
            // v = 0;
            float E = 1.0;//0.5*v*v + x*x + 3*exp(-x*x) + x;
            // float E = 0.5*v*v + x*x + 3*exp(-x*x) + x;
            if(E < 3.1) {
                velocities.push_back(v);
                particles.push_back(QVector2D(x, 0));
            }
        }
    }
}

void Simulator::step()
{
    std::function<float(float x)> force = [&](float x) {
        // return -x*(x<1) - ( 1 - 2*(x-1)*(x-1)*(1 - (x-1)*(x-1)))*(x>=1);
        // return -x*(x<1.0) - (1 + (x - 1)*(x - 1))*(x>=1.0);
        // return -2*x + 3*2*x*exp(-x*x) - 1;
        // return -(10*x*x*x*x*x + 2*x*x*x*x +3*x*x*x - 2.5*3*x*x + x);
        // return -4*x*x*x + 2*x;
        // return -x + Random::nextGaussian(0, 0.1);
        return -x;
    };

    const unsigned int numberOfParticles = particles.size();
    for(unsigned int i=0; i<numberOfParticles; i++) {
        // This is velocity, mass is 1
        velocities[i] += force(particles[i][0])*dt;
        // velocities[i] *= 0.999;
//        if(Random::nextDouble() < 0.001) {
//            velocities[i] *= (Random::nextDouble() > 0.5) ? 1.1 : 1.0/1.1;
//        }

//        if(Random::nextDouble() < 0.0001) {
//            velocities[i] = 0.01;
//        }

        // This is position
        particles[i][0] += velocities[i]*dt;
        particles[i][1] = velocities[i];
    }
    emit stepCompleted();
}
