#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QLocale>
#include <QTranslator>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
  QGuiApplication::setOrganizationName(u"Wurstcrew"_qs);
  QGuiApplication::setApplicationName(u"interact2"_qs);
  QGuiApplication::setApplicationDisplayName(u"Interact 2"_qs);
  QGuiApplication::setApplicationVersion(APPLICATION_VERSION);
  QGuiApplication app(argc, argv);

  QSurfaceFormat format;
  format.setSamples(8);
  QSurfaceFormat::setDefaultFormat(format);

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
  engine.addImportPath(u"qrc:/"_qs);
  engine.addImportPath(u"qml"_qs);

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
