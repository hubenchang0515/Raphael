#ifndef CLAMAVENGINE_H
#define CLAMAVENGINE_H

#include <QObject>
#include <QFileInfo>
#include <QQueue>
#include <clamav/clamav.h>

class ClamAVEngine : public QObject
{
    Q_OBJECT
public:
    explicit ClamAVEngine(QObject *parent = nullptr);
    ~ClamAVEngine();

signals:
    void opened();
    void closed();
    void detecting(const QString& file);
    void detected(const QString& file, bool isSafe, const QString& virname);

public slots:
    bool open();
    bool close();
    bool detect(const QFileInfo& file);
    bool detect(const QString& file);

    void start(const QString& file);

private:
    struct cl_engine* engine;
    bool working;

    bool detectFile(const QFileInfo& file);
    bool detectDir(const QFileInfo& directory);
};

#endif // CLAMAVENGINE_H
