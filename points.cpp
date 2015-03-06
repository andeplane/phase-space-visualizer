#include "points.h"
#include <QOpenGLFunctions>
#include <QColor>
#include <QRgb>
#include <iostream>

using std::vector;

Points::Points() :
    m_numberOfPoints(0),
    m_funcs(0),
    m_program(0),
    m_vao(0),
    m_vbo(0)
{
    generateVBOs();
}

Points::~Points()
{
    if(m_funcs) delete m_funcs;
    if(m_program) delete m_program;
}

void Points::generateVBOs()
{
    ensureInitialized();
    m_funcs->glGenBuffers(1, m_vboIds);
}

void Points::ensureInitialized()
{
    if(!m_funcs) m_funcs = new QOpenGLFunctions(QOpenGLContext::currentContext());
    if(!m_vao) {
        m_vao = new QOpenGLVertexArrayObject();
        m_vao->create();
        m_vao->bind();

        m_vbo->create();
        // m_vbo->setUsagePattern(QOpenGLBuffer::StreamDraw);
    }

}

void Points::update(const PointsData &data)
{
    ensureInitialized();
    m_numberOfPoints = data.positions.size();
    vector<QVector2D> pos;
    pos.push_back(QVector2D(0,0));
    pos.push_back(QVector2D(0.1,0.1));
    pos.push_back(QVector2D(-0.1,-0.1));
    // Transfer vertex data to VBO 0
    // m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    // m_funcs->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(QVector2D), &pos[0], GL_STATIC_DRAW);
    m_vbo->bind();
    m_vbo->allocate(&pos[0], pos.size()*sizeof(QVector2D));
}

void Points::createShaderProgram() {
//    std::cout << "Version " << glGetString(GL_VERSION) << std::endl;
//    std::cout << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    if (!m_program) {
        m_program = new QOpenGLShaderProgram();

        m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                           "#version 330\n"
                                           "in vec2 a_position;\n"
                                           "out vec4 pos;"
                                           "void main() {\n"
                                           "    gl_Position = vec4(a_position, -1.0, 0.0);\n"
                                           "    pos = vec4(a_position, -1.0, 0.0);\n"
                                           "}");

//        m_program->addShaderFromSourceCode(QOpenGLShader::Geometry,
//                                           "#version 330\n"
//                                           "layout( points ) in;\n"
//                                           "layout( points, max_vertices = 4) out;"
//                                           "void main() {\n"
//                                           "vec4 pos = gl_in[0].gl_Position;\n"
//                                           "gl_Position = pos;\n"
//                                           "EmitVertex();\n"
//                                           "gl_Position = pos + vec4(0.2, 0.0, 0.0, 0.0);\n"
//                                           "EmitVertex();\n"
//                                           "EndPrimitive();\n"
//                                           "}\n"
//                                           );

        m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                           "#version 330\n"
                                           "in vec4 pos;"
                                           "layout(location = 0, index = 0) out vec4 fragColor;\n"
                                           "void main() {\n"
                                           "    fragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
                                           "}");

        m_program->link();
    }
}

void Points::render()
{
    if(m_numberOfPoints == 0) return;
    ensureInitialized();
    createShaderProgram();

    m_program->bind();

    // Tell OpenGL which VBOs to use
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);

    // Tell OpenGL programmable pipeline how to locate vertex position data
    // int vertexLocation = m_program->attributeLocation("a_position");
    m_program->enableAttributeArray("a_position");
    // m_program->enableAttributeArray(vertexLocation);
    // m_program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 2, 0);
    m_program->setAttributeBuffer("a_position", GL_FLOAT, 0, 2, 0);
    // m_funcs->glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_TRUE, sizeof(QVector2D), 0);
    m_vao->bind();
    m_funcs->glDrawArrays(GL_POINTS, 0, 3);

    // glPointSize(3.0);
    // Draw cube geometry using indices from VBO 1
    // m_funcs->glDrawArrays(GL_POINTS, 0, 3);
    // glDrawArrays(GL_POINTS, 0, m_numberOfPoints);

    // m_program->disableAttributeArray(vertexLocation);
    m_program->disableAttributeArray("a_position");

    m_program->release();
}
