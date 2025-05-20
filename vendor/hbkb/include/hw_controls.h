/*
 * hw_helper.h
 *
 *  Created on: Sep 10, 2015
 *      Author: jbr373
 */

#ifndef HW_CONTROLS_H_
#define HW_CONTROLS_H_

#include <3ds.h>

class HW_Controls {
 public:
  HW_Controls();
  virtual ~HW_Controls();

  u8 CheckPressedKey(s16 &T_X, s16 &T_Y, u8 &KeyboardState);

 private:
  // Controls
  u8 CheckPressedKeySpecial(s16 &T_X, s16 &T_Y);
  u8 CheckPressedKeyABC(s16 &T_X, s16 &T_Y);
};

#endif /* HW_CONTROLS_H_ */