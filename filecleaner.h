#ifndef FILECLEANER_H
#define FILECLEANER_H

#include <QObject>

class FileCleaner : public QObject
{
    Q_OBJECT
public:
    explicit FileCleaner(QObject *parent = nullptr);

signals:
    void removed(const QString& file);

public slots:
    void remove(const QString& file);
};

#endif // FILECLEANER_H
