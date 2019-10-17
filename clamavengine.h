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
    void finished();
    void detecting(const QString& file);
    void detected(const QString& file, bool isSafe, const QString& virname);
    void message(const QString& text);

public slots:
    bool open();
    bool close();
    bool detect(const QFileInfo& file);
    bool detect(const QString& file);

    void scan(const QString& file);
    void globalScan();

private:
    struct cl_engine* engine;
    bool isOpen;

    bool detectFile(const QFileInfo& file);
    bool detectDir(const QFileInfo& directory);
};

#endif // CLAMAVENGINE_H
