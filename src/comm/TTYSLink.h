#pragma once

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QString>
#include <QMetaType>
#include <QLoggingCategory>
#include "QGCConfig.h"
#include "LinkConfiguration.h"
#include "LinkInterface.h"
#ifdef __android__
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdint.h>
#include <poll.h>
#include <string.h>
#endif
#include <QSocketNotifier>
Q_DECLARE_LOGGING_CATEGORY(TtysLinkLog)

class LinkManager;

/// SerialLink configuration
class TtysConfiguration : public LinkConfiguration
{
    Q_OBJECT

public:

    TtysConfiguration(const QString& name);
    TtysConfiguration(TtysConfiguration* copy);
    Q_PROPERTY(QString  devFile         READ devFile            WRITE setDevFile            NOTIFY devFileChanged)

    const QString devFile          () { return _devFile; }
    void setDevFile        (const QString& devFile);

    LinkType    type            () { return LinkConfiguration::TypeTtys; }
    void        copyFrom        (LinkConfiguration* source);
    void        loadSettings    (QSettings& settings, const QString& root);
    void        saveSettings    (QSettings& settings, const QString& root);
    QString     settingsURL     () { return "TtysSettings.qml"; }
    QString     settingsTitle   () { return tr("TTYS Link Settings"); }

signals:
      void devFileChanged        ();
private:
      QString  _devFile;
};

class TTYSLink: public LinkInterface
{
    Q_OBJECT

public:
    TTYSLink(SharedLinkConfigurationPtr& config, bool isPX4Flow = false);
    virtual ~TTYSLink();

    // LinkInterface overrides
    bool isConnected(void) const override;
    void disconnect (void) override;
    // QThread overrides
    void run(void) override;
private slots:
    void _writeBytes(const QByteArray data) override;
private slots:
    void _readBytes     (void);
private:

    // LinkInterface overrides
    bool _connect(void) override;
    bool _hardwareConnect   (void);
    void _emitLinkError     (const QString& errorMsg);

    int                      fd = -1;
    fd_set rd;
    fd_set wd;
    QSocketNotifier         *m_notifier=nullptr;
    bool                    _running= false;
    quint64                 _bytesRead          = 0;
    int                     _timeout;
    QMutex                  _dataMutex;                     ///< Mutex for reading data from _port
    QMutex                  _writeMutex;                    ///< Mutex for accessing the _transmitBuffer.
    volatile bool           _stopp              = false;
    QMutex                  _stoppMutex;                    ///< Mutex for accessing _stopp
    QByteArray              _transmitBuffer;                ///< An internal buffer for receiving data from member functions and actually transmitting them via the serial port.
    TtysConfiguration*      _ttysConfig       = nullptr;
};
