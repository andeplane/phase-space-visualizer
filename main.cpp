#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QGuiApplication>
#include <QtQuick/QQuickView>
#include <QSurfaceFormat>
#include <QGLFormat>
#include <QOpenGLContext>
#include <iostream>
#include <QFontDatabase>

#include "controller.h"
using namespace std;

# if defined (Q_OS_IOS)
extern "C" int qtmn (int argc, char * argv [])
#else
int main (int argc, char * argv [])
# endif
{
    qmlRegisterType<CompPhys::Controller>("Controller", 1, 0, "Controller");

    QGuiApplication app(argc, argv);
    QQuickView view;

    QFontDatabase::addApplicationFont(":/fonts/SourceSansPro-Regular.ttf");
    app.setFont(QFont("Source Sans Pro"));

    QSurfaceFormat f;
    f.setMajorVersion(4);
    f.setMinorVersion(1);
    f.setDepthBufferSize(16);
    // f.setStencilBufferSize(16);
    f.setProfile(QSurfaceFormat::CoreProfile);

    view.setFormat(f);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///main.qml"));

    view.show();


    return app.exec();
}
