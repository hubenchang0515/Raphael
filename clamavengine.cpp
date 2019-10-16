#include <QtDebug>
#include <QDir>
#include <QCoreApplication>
#include "clamavengine.h"

ClamAVEngine::ClamAVEngine(QObject *parent) :
    QObject(parent),
    engine(nullptr)
{

}

ClamAVEngine::~ClamAVEngine()
{
    if(engine != nullptr)
    {
        cl_engine_free(engine);
    }
}

bool ClamAVEngine::open()
{
    int retCode = CL_SUCCESS;

    /* Init */
    if((retCode = cl_init(CL_INIT_DEFAULT)) != CL_SUCCESS)
    {
        qCritical() << cl_strerror(retCode);
        return false;
    }

    /* Create Engine */
    struct cl_engine* engine = cl_engine_new();
    if(engine == nullptr)
    {
        qCritical() << "Cannot create ClamAV engine.";
        return false;
    }

    /* Load Virus Database */
    unsigned int signatures = 0;
    QDir cvdDir(qApp->applicationDirPath() + "/CVD");
    for(auto& file : cvdDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot))
    {
        QString cvdFile = file.absoluteFilePath();
        #ifdef Q_OS_WIN32
            cvdFile.replace('/', "\\");
        #endif
        if((retCode = cl_load(cvdFile.toStdString().c_str(), engine, &signatures, CL_DB_STDOPT)) != CL_SUCCESS)
        {
            qCritical() << cl_strerror(retCode);
            cl_engine_free(engine);
            engine = nullptr;
            return false;
        }
        qInfo() << file.fileName() << " : " << signatures << " signatures loaded.";
    }


    /* Compile */
    if((retCode = cl_engine_compile(engine)) != CL_SUCCESS)
    {
        qCritical() << cl_strerror(retCode);
        cl_engine_free(engine);
        engine = nullptr;
        return false;
    }

    emit opened();
    return true;
}


bool ClamAVEngine::close()
{
    if(engine != nullptr)
    {
        cl_engine_free(engine);
    }

    emit closed();
    return true;
}


bool ClamAVEngine::detect(const QFileInfo& file)
{
    if(file.isFile())
    {
        return detectFile(file);
    }
    else if(file.isDir())
    {
        return detectDir(file);
    }
    else
    {
        qCritical() << "Unsupported file type.";
        return false;
    }
}

bool ClamAVEngine::detect(const QString& file)
{
    return detect(QFileInfo(file));
}

bool ClamAVEngine::detectFile(const QFileInfo& file)
{
    const char* f = file.absoluteFilePath().toStdString().c_str();
    const char* virname = nullptr;
    int retCode = CL_SUCCESS;

    struct cl_scan_options options = {
        CL_SCAN_GENERAL_ALLMATCHES,
        CL_SCAN_PARSE_ARCHIVE,
        CL_SCAN_GENERAL_HEURISTIC_PRECEDENCE,
        CL_SCAN_MAIL_PARTIAL_MESSAGE,
        0,
    };
    if((retCode = cl_scanfile(f, &virname, nullptr, engine,
        &options)) == CL_VIRUS)
    {
        qInfo() << file << " seems like to be " << virname;
        emit detected(file.absoluteFilePath(), false, virname);
        return false;
    }
    else
    {
        qInfo() << file << " is safe.";
        emit detected(file.absoluteFilePath(), true, nullptr);
        return true;
    }
}


bool ClamAVEngine::detectDir(const QFileInfo& directory)
{
    QDir dir(directory.absoluteFilePath());
    bool ret = true;
    for(auto& fileinfo : dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot))
    {
        ret = ret && detect(fileinfo);
    }

    return ret;
}
