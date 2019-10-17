#include <QtDebug>
#include <QDir>
#include <QCoreApplication>
#include <QThread>
#include "clamavengine.h"

ClamAVEngine::ClamAVEngine(QObject *parent) :
    QObject(parent),
    engine(nullptr),
    isOpen(false)
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
    if(isOpen)
    {
        return true;
    }
    else if(engine != nullptr)
    {
        cl_engine_free(engine);
    }
    int retCode = CL_SUCCESS;

    /* Init */
    if((retCode = cl_init(CL_INIT_DEFAULT)) != CL_SUCCESS)
    {
        emit message(cl_strerror(retCode));
        return false;
    }

    /* Create Engine */
    engine = cl_engine_new();
    if(engine == nullptr)
    {
        emit message("Cannot create ClamAV engine.");
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
            emit message(cl_strerror(retCode));
            cl_engine_free(engine);
            engine = nullptr;
            return false;
        }
        emit message(QString::asprintf("CVD %s : loaded %d signatures.",
                                       file.fileName().toStdString().c_str(),
                                       signatures));
    }


    /* Compile */
    if((retCode = cl_engine_compile(engine)) != CL_SUCCESS)
    {
        emit message(cl_strerror(retCode));
        cl_engine_free(engine);
        engine = nullptr;
        return false;
    }

    /* set option */
    memset(&options, 0, sizeof(struct cl_scan_options));
    options.parse |= UINT32_MAX;
    options.general |= CL_SCAN_GENERAL_HEURISTICS;

    emit opened();
    isOpen = true;
    qDebug() << "opened";
    return true;
}


bool ClamAVEngine::close()
{
    if(engine != nullptr)
    {
        cl_engine_free(engine);
    }
    isOpen = false;
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
    else if(file.isSymLink())
    {
        emit message(file.fileName() + " is SymLink.");
        return false;
    }
    else
    {
        emit message(file.fileName() + " is Unknown.");
        return false;
    }
}

bool ClamAVEngine::detect(const QString& file)
{
    return detect(QFileInfo(file));
}

void ClamAVEngine::scan(const QString& path)
{
    if(open())
    {
        detect(path);
        emit finished();
    }
}

void ClamAVEngine::globalScan()
{
    if(open())
    {
        #ifdef Q_OS_WIN32
        for(auto& drive : QDir::drives())
        {
            detect(drive.absoluteFilePath());
        }
        #else
            scan("/");
        #endif
        emit finished();
    }
}


bool ClamAVEngine::detectFile(const QFileInfo& file)
{
    QString filepath = file.absoluteFilePath();
    #ifdef Q_OS_WIN32
        filepath.replace('/', "\\");
    #endif

    const char* virname = nullptr;
    int retCode = CL_SUCCESS;


    emit detecting(file.absoluteFilePath());
    if((retCode = cl_scanfile(filepath.toStdString().c_str(), &virname, nullptr, engine,
        &options)) == CL_VIRUS)
    {
        emit detected(file.absoluteFilePath(), false, virname);
        return false;
    }
    else if(retCode == CL_CLEAN)
    {
        emit detected(file.absoluteFilePath(), true, nullptr);
        return true;
    }
    else
    {
        emit message(cl_strerror(retCode));
        return false;
    }
}


bool ClamAVEngine::detectDir(const QFileInfo& directory)
{
    QDir dir(directory.absoluteFilePath());
    bool ret = true;
    for(auto& fileinfo : dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot))
    {
        ret = detect(fileinfo) && ret;
    }

    return ret;
}
