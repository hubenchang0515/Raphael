#ifndef CLAMAVTHREAD_H
#define CLAMAVTHREAD_H

#include <QObject>
#include <QThread>
#include <QTime>
#include <clamavengine.h>

class ClamAVThread : public QObject
{
    Q_OBJECT
public:
    explicit ClamAVThread(QObject *parent = nullptr);

signals:
    void detect(const QString& path);
    void abandon();
    void opened();
    void closed();
    void detecting(const QString& file);
    void detected(const QString& file, bool isSafe, const QString& virname);

public slots:
    void start(const QString& path);
    void cancel();

    void proxyDetecting(const QString& file);

private:
    ClamAVEngine* engine;
    QThread* thread;
    QTime time;
};

#endif // CLAMAVTHREAD_H
