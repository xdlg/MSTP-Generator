#include "image_provider.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char* argv[])
{
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  ImageProvider* imageProvider = new ImageProvider();

  engine.rootContext()->setContextProperty("imageProvider", imageProvider);
  engine.addImageProvider("imageProvider", imageProvider);
  engine.addImportPath("qrc:/qml");

  const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
  engine.load(url);

  return app.exec();
}
