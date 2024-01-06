#pragma once

#include "FactGroup.h"
#include "QGCMAVLink.h"

class VehicleSprayerFactGroup : public FactGroup
{
    Q_OBJECT

public:
    VehicleSprayerFactGroup(QObject* parent = nullptr);

    Q_PROPERTY(Fact* filllevel     READ filllevel     CONSTANT)
    Q_PROPERTY(Fact* flowliter     READ flowliter     CONSTANT)
    Q_PROPERTY(Fact* flowha        READ flowha        CONSTANT)
    Q_PROPERTY(Fact* distlines     READ distlines     CONSTANT)
    Q_PROPERTY(Fact* speed         READ speed         CONSTANT)
    Q_PROPERTY(Fact* status        READ status        CONSTANT)
    Q_PROPERTY(Fact* statusText    READ statusText    CONSTANT)
    Q_PROPERTY(bool  sprayingRight        READ sprayingRight        NOTIFY statusChanged)
    Q_PROPERTY(bool  sprayingLeft         READ sprayingLeft         NOTIFY statusChanged)
    Q_PROPERTY(bool  sprayingReady        READ sprayingReady        NOTIFY statusChanged)
    Q_PROPERTY(bool  sprayingPumpError    READ sprayingPumpError    NOTIFY statusChanged)
    Q_PROPERTY(bool  sprayingNozzlesError READ sprayingNozzlesError NOTIFY statusChanged)

    Fact* filllevel              () { return &_filllevelFact; }
    Fact* flowliter              () { return &_flowliterFact; }
    Fact* flowha                 () { return &_flowhaFact; }
    Fact* distlines              () { return &_distlinesFact; }
    Fact* speed                  () { return &_speedFact; }
    Fact* status                 () { return &_statusFact; }
    Fact* statusText             () { return &_statusTextFact; }
    bool  sprayingRight          () { return _sprayingRight; }
    bool  sprayingLeft           () { return _sprayingLeft; }
    bool  sprayingReady          () { return _sprayingReady; }
    bool  sprayingPumpError      () { return _sprayingPumpError; }
    bool  sprayingNozzlesError   () { return _sprayingNozzlesError; }


    // Overrides from FactGroup
    virtual void handleMessage(Vehicle* vehicle, mavlink_message_t& message) override;

    static const char* _filllevelFactName;
    static const char* _flowliterFactName;
    static const char* _flowhaFactName;
    static const char* _distlinesFactName;
    static const char* _speedFactName;
    static const char* _statusFactName;
    static const char* _statusTextFactName;

signals:
    void statusChanged();

protected:
    void _handleSoStatus(mavlink_message_t& message);
    void _updateSoStatusFlagsText();

    Fact _filllevelFact;
    Fact _flowliterFact;
    Fact _flowhaFact;
    Fact _distlinesFact;
    Fact _speedFact;
    Fact _statusFact;
    Fact _statusTextFact;
    bool _sprayingRight = false;
    bool _sprayingLeft = false;
    bool _sprayingReady = false;
    bool _sprayingPumpError = false;
    bool _sprayingNozzlesError = false;
    uint8_t _prevStatusFlags = 0;
};
