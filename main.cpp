#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
  QGuiApplication::setOrganizationName(u"Wurstcrew"_qs);
  QGuiApplication::setApplicationName(u"interact2"_qs);
  QGuiApplication::setApplicationDisplayName(u"Interact 2"_qs);
  QGuiApplication::setApplicationVersion(APPLICATION_VERSION);
  QGuiApplication app(argc, argv);

  // TODO: move this into the application class
  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "Interact2_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      app.installTranslator(&translator);
      break;
    }
  }

  QQmlApplicationEngine engine;
  const QUrl url(u"qrc:/App/qml/main.qml"_qs);
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
