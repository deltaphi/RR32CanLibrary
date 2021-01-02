#ifndef __RR32CAN__ENGINE_H__
#define __RR32CAN__ENGINE_H__

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstring>
#endif

#include "RR32Can/Constants.h"
#include "RR32Can/Types.h"

namespace RR32Can {

/*
 * \brief Locomotive with a name only.
 */
class LocomotiveShortInfo {
 public:
  enum class AvailabilityStatus { EMPTY = 0, NAME_KNOWN, FULL_DETAILS };

  LocomotiveShortInfo() : availability_(AvailabilityStatus::EMPTY) { eraseName(); }

  virtual ~LocomotiveShortInfo() = default;

  virtual void reset() {
    availability_ = AvailabilityStatus::EMPTY;
    eraseName();
  }

  void setName(const char* name) {
    reset();
    availability_ = AvailabilityStatus::NAME_KNOWN;
    strncpy(this->name_, name, kEngineNameLength);
  }

  /**
   * \brief Sets the name only when it is different from the current name.
   *
   * \return True if the engine was changed, false otherwise.
   */
  bool setNameConditional(const char* name) {
    if (availability_ == AvailabilityStatus::EMPTY || strncmp(name, this->name_, kEngineNameLength) != 0) {
      setName(name);
      return true;
    } else {
      return false;
    }
  }

  const char* getName() const { return name_; }

  AvailabilityStatus getAvailability() const { return availability_; }
  bool isNameKnown() const { return availability_ != AvailabilityStatus::EMPTY; }

  bool isNameOnlyKnown() const { return availability_ == AvailabilityStatus::NAME_KNOWN; }

  bool isFree() const { return availability_ == AvailabilityStatus::EMPTY; }

  virtual void print() const;

 protected:
  AvailabilityStatus availability_;
  char name_[kEngineNameLength + 1];

  void eraseName() { memset(this->name_, '\0', kEngineNameLength + 1); }

  friend class LocoConsumer;
};

/**
 * Non-String data portion of a Locomotive.
 */
class LocomotiveData {
 public:
  using Uid_t = RR32Can::Uid_t;
  // Velocities have a range of 0..1000 (..1023).
  using Velocity_t = RR32Can::Velocity_t;
  using Address_t = RR32Can::MachineLocomotiveAddress;
  using FunctionBits_t = RR32Can::FunctionBits_t;

  void reset() {
    // Remove all data of this class
    uid_ = 0;
    velocity_ = 0;
    direction_ = RR32Can::EngineDirection::UNKNOWN;
    address_ = MachineLocomotiveAddress(0);
    functionBits_ = 0;
  }

  void setUid(Uid_t uid) { this->uid_ = uid; }
  Uid_t getUid() const { return uid_; }

  /// Get the UID with only the two low bytes set.
  Uid_t getUidMasked() const { return uid_ & 0xFFFF; }

  void setVelocity(Velocity_t velocity) { this->velocity_ = velocity; }
  Velocity_t getVelocity() const { return velocity_; }

  void setDirection(RR32Can::EngineDirection direction) { this->direction_ = direction; }
  RR32Can::EngineDirection getDirection() const { return direction_; }
  void changeDirection() {
    if (direction_ == EngineDirection::FORWARD) {
      direction_ = EngineDirection::REVERSE;
    } else if (direction_ == EngineDirection::REVERSE) {
      direction_ = EngineDirection::FORWARD;
    }
  }

  void setAddress(Address_t address) { this->address_ = address; }
  Address_t getAddress() const { return address_; }

  FunctionBits_t getFunctionBits() const { return functionBits_; }

  void setFunction(uint8_t function, bool onOff) {
    FunctionBits_t mask = 1;
    mask <<= function;
    if (onOff != 0) {
      functionBits_ |= mask;
    } else {
      functionBits_ &= ~mask;
    }
  }

  bool getFunction(uint8_t function) const {
    FunctionBits_t mask = 1;
    mask <<= function;
    if ((functionBits_ & mask) != 0) {
      return true;
    } else {
      return false;
    }
  }

  void print() const;

 private:
  Uid_t uid_;
  Velocity_t velocity_;
  RR32Can::EngineDirection direction_;
  Address_t address_;
  FunctionBits_t functionBits_;
};

/**
 * \brief Locomotive with full data.
 */
class Locomotive : public LocomotiveShortInfo, public LocomotiveData {
 public:
  using LocomotiveShortInfo::LocomotiveShortInfo;

  static constexpr const uint8_t kProtocolNameMaxLength = 8;

  using ProtocolName_t = char[kProtocolNameMaxLength];

  Locomotive() { reset(); }

  void reset() {
    // Remove all data of this class
    LocomotiveShortInfo::reset();
    LocomotiveData::reset();
    memset(protocol_, 0, kProtocolNameMaxLength);
  }

  bool isFullDetailsKnown() const { return availability_ == AvailabilityStatus::FULL_DETAILS; }

  void print() const override;

  void setProtocolString(const char* protocolString) { strncpy(protocol_, protocolString, kProtocolNameMaxLength); }
  const char* getProtocolString() const { return protocol_; }

 protected:
  ProtocolName_t protocol_;
};

}  // namespace RR32Can

#endif  // __RR32CAN__ENGINE_H__
