#include "levelhandler.h"

#include <QDir>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lvlh, "app.levelhandler")

namespace {
    const QString LEVELS_PATH = u":/App/resources/levels"_qs;
    const QString PREVIEW_FILE = u"preview.png"_qs;
}

LevelHandler::LevelHandler(QObject *parent)
    : QObject{parent}
    , m_availableLevels{new LevelModel(this)}
    , m_userLevels{new LevelModel(this)}
{
    const QFileInfoList levelList = QDir(LEVELS_PATH).entryInfoList();

    for (const QFileInfo &info : levelList) {
        QDir levelFolder(info.filePath());
        QString previewPath = levelFolder.filePath(PREVIEW_FILE).prepend(u"qrc"_qs);

        m_availableLevels->addLevel({
            info.fileName(),
            levelFolder.exists(PREVIEW_FILE) ? previewPath : QUrl(),
            levelFolder.path()
        });
    }

    qCInfo(lvlh) << "game levels available:" << levelList.count();

    // TODO: search for user defined levels
    // User defined levels should be saved in the applications data folder, which can
    // be checked on every application start.
    qCInfo(lvlh) << "user levels available:" << 0;
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
}
