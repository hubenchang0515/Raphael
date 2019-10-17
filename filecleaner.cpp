#include "filecleaner.h"
#include <QFile>
#include <QDebug>

FileCleaner::FileCleaner(QObject *parent) : QObject(parent)
{

}

void FileCleaner::remove(const QString& file)
{
    qDebug() << "remove" << file;
    QFile::remove(file);
    emit removed(file);
}
