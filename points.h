#pragma once
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <vector>

struct PointsData {
    std::vector<QVector2D> positions;
};

class Points
{
public:
    Points();
    ~Points();
    void update(const PointsData &data);
    void render();
private:
    GLuint m_vboIds[1];
    unsigned int m_numberOfPoints;
    QOpenGLBuffer *m_vbo;
    QOpenGLVertexArrayObject *m_vao;
    QOpenGLFunctions *m_funcs;
    QOpenGLShaderProgram *m_program;

    void createShaderProgram();
    void generateVBOs();
    void ensureInitialized();
};
