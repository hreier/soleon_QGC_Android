#include "VehicleSprayerFactGroup.h"
#include "Vehicle.h"
#include <bitset>


const char* VehicleSprayerFactGroup::_filllevelFactName =  "filllevel";
const char* VehicleSprayerFactGroup::_flowliterFactName =  "flowliter";
const char* VehicleSprayerFactGroup::_flowhaFactName =     "flowha";
const char* VehicleSprayerFactGroup::_distlinesFactName =  "distlines";
const char* VehicleSprayerFactGroup::_speedFactName =      "speed";
const char* VehicleSprayerFactGroup::_statusFactName =     "status";
const char* VehicleSprayerFactGroup::_statusTextFactName = "statusText";

VehicleSprayerFactGroup::VehicleSprayerFactGroup(QObject* parent)
    : FactGroup(1000, ":/json/Vehicle/SprayerFact.json", parent)
    , _filllevelFact (0, _filllevelFactName,  FactMetaData::valueTypeFloat)
    , _flowliterFact (0, _flowliterFactName,  FactMetaData::valueTypeFloat)
    , _flowhaFact    (0, _flowhaFactName,     FactMetaData::valueTypeFloat)
    , _distlinesFact (0, _distlinesFactName,  FactMetaData::valueTypeFloat)
    , _speedFact     (0, _speedFactName,      FactMetaData::valueTypeFloat)
    , _statusFact    (0, _statusFactName,     FactMetaData::valueTypeUint8)
    , _statusTextFact(0, _statusTextFactName, FactMetaData::valueTypeUint8)
{
    _addFact(&_filllevelFact, _filllevelFactName);
    _addFact(&_flowliterFact, _flowliterFactName);
    _addFact(&_flowhaFact,    _flowhaFactName);
    _addFact(&_distlinesFact, _distlinesFactName);
    _addFact(&_speedFact,     _speedFactName);
    _addFact(&_statusFact,    _statusFactName);
    // _addFact(&_statusTextFact,_statusTextFactName);

    // Start out as not available "--.--"
    _filllevelFact.setRawValue(0.0f);
    _flowliterFact.setRawValue(qQNaN());
    _flowhaFact.setRawValue(qQNaN());
    _distlinesFact.setRawValue(qQNaN());
    _speedFact.setRawValue(qQNaN());
    _statusFact.setRawValue(UINT16_MAX);
    _statusTextFact.setRawValue(QString());
}

void VehicleSprayerFactGroup::handleMessage(Vehicle* /* vehicle */, mavlink_message_t& message)
{
    switch (message.msgid) {
    case MAVLINK_MSG_ID_SO_STATUS:
        _handleSoStatus(message);
        break;
    default:
        break;
    }
}

void VehicleSprayerFactGroup::_handleSoStatus(mavlink_message_t& message)
{
    mavlink_so_status_t soStatus;
    mavlink_msg_so_status_decode(&message, &soStatus);

    filllevel()->setRawValue(soStatus.filllevel);
    flowliter()->setRawValue(soStatus.flowliter);
    flowha()->setRawValue(soStatus.flowha);
    distlines()->setRawValue(soStatus.distlines);
    speed()->setRawValue(soStatus.speed);
    status()->setRawValue(soStatus.status);

    _updateSoStatusFlagsText();
}

void VehicleSprayerFactGroup::_updateSoStatusFlagsText()
{
    // Check the status received, and convert it to a List with the state of each flag
    int statusFlags = status()->rawValue().toInt();

    // No need to update the list if we have the same flags
    if ( statusFlags == _prevStatusFlags) {
        return;
    }

    _prevStatusFlags = statusFlags;

    std::bitset<8> bitsetFlags(statusFlags);

    for (size_t i = 0; i < bitsetFlags.size(); i++) {
        bool set = false;
        if (bitsetFlags.test(i)) {
            set = true;
        } else {
            set = false;
        }

        switch(i) {
            case 0:
                _sprayingRight = set;
                break;
            case 1:
                _sprayingLeft = set;
                break;
            case 2:
                _sprayingReady = set;
                break;
            case 3:
                _sprayingPumpError = set;
                break;
            case 4:
                _sprayingNozzlesError = set;
                break;
            default:
                break;
        }
    }
    emit statusChanged();
    
}