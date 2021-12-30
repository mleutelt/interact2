#include "levelhandler.h"
#include "levelfileio.h"

#include <QStandardPaths>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lvlh, "app.levelhandler")

namespace {
  const QString LEVELS_PATH = u":/App/resources/levels"_qs;
  const QString PREVIEW_FILE = u"preview.png"_qs;
  const QString USER_LEVELS_DIRECTORY = u"levels"_qs;
  const QString LEVEL_FILE = u"data.json"_qs;
}

LevelHandler::LevelHandler(QObject *parent)
  : QObject { parent },
    m_availableLevels { new LevelModel(this) },
    m_userLevels { new LevelModel(this) },
    m_currentLevelData { new LevelDataModel(this) },
    m_levelFileIO { new LevelFileIO(this) }
{
  QDir applicationDataDirectory(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
  applicationDataDirectory.mkpath(USER_LEVELS_DIRECTORY);

  updateGameLevelsModel();
  updateUserLevelsModel();
}

LevelModel *LevelHandler::availableLevels() const
{
  return m_availableLevels;
}

LevelModel *LevelHandler::userLevels() const
{
  return m_userLevels;
}

void LevelHandler::loadLevel(const QString &path)
{
  qCDebug(lvlh) << "loading level at path" << path;

  if (QDir(path).exists(LEVEL_FILE)) {
    setLoading(true);

    LevelData data;
    data.name = QDir(path).dirName();
    data.objects = m_levelFileIO->loadLevelFromPath(path);

    m_currentLevelData->setLevelData(data);

    QTimer::singleShot(3000, this, [this] {
      setLoading(false);
    });
  } else {
    qCCritical(lvlh) << "unable to find level data";
    // TODO: emit signal to display UI message
  }
}

bool LevelHandler::loading() const
{
  return m_loading;
}

void LevelHandler::setLoading(bool flag)
{
  if (m_loading == flag) {
    return;
  }

  m_loading = flag;
  emit loadingChanged();
}

void LevelHandler::updateGameLevelsModel()
{
  const QFileInfoList levelList = QDir(LEVELS_PATH).entryInfoList();

  for (const QFileInfo &info : levelList) {
    QDir levelFolder(info.filePath());
    QUrl preview;
    preview.setScheme(u"qrc"_qs);
    preview.setPath(levelFolder.filePath(PREVIEW_FILE).sliced(1));

    m_availableLevels->addLevel({
        info.fileName(),
        levelFolder.exists(PREVIEW_FILE) ? preview : QUrl(),
        levelFolder.path(),
    });
  }

  qCInfo(lvlh) << "game levels available:" << levelList.count();
}

void LevelHandler::updateUserLevelsModel()
{
  qCInfo(lvlh) << "searching for user levels at path:" << userLevelsDirectory().path();

  m_userLevels->clear();

  const QFileInfoList userLevelList = userLevelsDirectory().entryInfoList({}, QDir::Dirs | QDir::NoDotAndDotDot);

  for (const QFileInfo &info : userLevelList) {
    QDir levelFolder(info.filePath());
    QUrl preview;
    preview.setScheme(u"file"_qs);
    preview.setPath(levelFolder.filePath(PREVIEW_FILE));

    m_userLevels->addLevel({
        info.fileName(),
        levelFolder.exists(PREVIEW_FILE) ? preview : QUrl(),
        levelFolder.path(),
    });
  }

  qCInfo(lvlh) << "user levels found:" << userLevelList.count();
}

LevelDataModel *LevelHandler::currentLevelData() const
{
  return m_currentLevelData;
}

QDir LevelHandler::userLevelsDirectory()
{
  return QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).filePath(USER_LEVELS_DIRECTORY);
}

QString LevelHandler::levelPreviewFileName()
{
  return PREVIEW_FILE;
}
