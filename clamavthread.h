#ifndef CLAMAVTHREAD_H
#define CLAMAVTHREAD_H

#include <QObject>
#include <QThread>
#include <clamavengine.h>

class ClamAVThread : public QObject
{
    Q_OBJECT
public:
    explicit ClamAVThread(QObject *parent = nullptr);

signals:
    void detect(const QString& path);

public slots:

private:
    ClamAVEngine* engine;
    QThread* thread;
};

#endif // CLAMAVTHREAD_H
