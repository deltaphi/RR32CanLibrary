#ifndef __RR32CAN__CALLBACK__ENGINECBK_H__
#define __RR32CAN__CALLBACK__ENGINECBK_H__

namespace RR32Can {
namespace callback {

/*
 * \brief Class EngineCbk
 */
class EngineCbk {
 public:
  /**
   * \brief Set the velocity of the loco with the given UID.
   *
   * Should have no effect, if the engine is not known.
   */
  virtual void setLocoVelocity(Locomotive::Uid_t engineUid, RR32Can::Velocity_t velocity) = 0;

  /**
   * \brief Unconditionally set the velocity of Engine(s). Used for the SYSTEM_HALT command.
   */
  virtual void setLocoVelocity(RR32Can::Velocity_t velocity) = 0;

  /**
   * \brief Set the function to on or off.
   *
   * \param uid UID of the Locomotive.
   * \param functionIdx Number of the function.
   * \param functionOn true == function is on, false == function is off.
   */
  virtual void setLocoFunction(const Locomotive::Uid_t uid, uint8_t functionIdx, bool functionOn) = 0;

  virtual void setLocoDirection(const Uid_t uid, const EngineDirection direction) = 0;
  virtual void changeLocoDirection(const Uid_t uid) = 0;
};

}  // namespace callback
}  // namespace RR32Can

#endif  // __RR32CAN__CALLBACK__ENGINECBK_H__
