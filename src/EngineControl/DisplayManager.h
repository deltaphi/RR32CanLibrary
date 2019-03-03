#ifndef __ENGINECONTROL_DISPLAYMANAGER_H__
#define __ENGINECONTROL_DISPLAYMANAGER_H__

#include <Arduino.h>
#include "config.h"

#if (DISPLAY_ATTACHED == STD_ON)

#include "SSD1306Wire.h"

namespace EngineControl {

#define STRING_CHAR_LENGTH (16)
#define STRING_DATATYPE_LENGTH ((STRING_CHAR_LENGTH) + 1)
#define DISPLAY_LINES (2)

class DisplayManager {
 public:
  using LineBuffer = char[STRING_DATATYPE_LENGTH];

  void begin();

  /**
   * \brief Return a buffer to be written to and set the flag that a change
   * needs to be printed in the next loop.
   *
   * \param lineNumber The index of the line (0-based) to be rewritten.
   * Out-of-bounds values will wrap.
   */
  LineBuffer& getWritableBuffer(uint8_t lineNumber) {
    updateRequired = true;
    return buffer[lineNumber % DISPLAY_LINES];
  }

  void loop();

  void enableCursor() {
    if (!cursorEnabled) {
      cursorEnabled = true;
      updateRequired = true;
    }
  }

  void disableCursor() {
    if (cursorEnabled) {
      cursorEnabled = false;
      updateRequired = true;
    }
  }

  void setCursorLine(uint8_t line) {
    if (this->cursorLine != line) {
      this->cursorLine = line;
      updateRequired = true;
    }
  }
  
  uint8_t getCursorLine() const { return cursorLine; }

 private:
  bool cursorEnabled;
  uint8_t cursorLine;
  static const uint8_t voffset[];
  static const uint8_t baselineOffset[];

  using TextBuffer = LineBuffer[DISPLAY_LINES];

  SSD1306Wire display = {DISPLAY_TWI_ADDRESS, TWI_SDA_PIN, TWI_SCL_PIN};
  TextBuffer buffer;
  bool updateRequired = false;
};

} /* namespace EngineControl */

#endif
#endif
