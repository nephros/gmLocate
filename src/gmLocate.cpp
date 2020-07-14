#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include <QtQuick>
#include <QScopedPointer>
#include "filesmodel.h"
int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/gmLocate.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //   - SailfishApp::pathToMainQml() to get a QUrl to the main QML file
    //
    // To display the view, call "show()" (will show fullscreen on device).

   //// return SailfishApp::main(argc, argv);
    // Set up qml engine.
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> v(SailfishApp::createView());

    // If you wish to publish your app on the Jolla harbour, follow
    // https://harbour.jolla.com/faq#5.3.0 about naming own QML modules.
    qmlRegisterType<FilesModel>("com.gmLocate", 1, 0, "FilesModel");

    // Start the application.
    v->setSource(SailfishApp::pathTo("qml/gmLocate.qml"));
    v->show();
    return app->exec();
}
