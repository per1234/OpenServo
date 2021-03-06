
/**
 * OpenServo Arduino/Wire Communication Library
 *
 *   This library communicates with an OpenServo on the i2c bus using the
 * OpenServo i2c API (http://www.openservo.com/APIGuide).  You must create
 * an instance of Wire to pass to the library.  The library will grab and
 * release the i2c bus during its communication cycles, but will not lock
 * up the bus outside of the library, so you can use the Wire instance to
 * communicate with other i2c devices on the bus, or send special commands
 * to an OpenServo, should you find this library lacking in some way.
 *
 * Copyright 2012 Sterling Lewis Peet <sterling.peet@gatech.edu>
 *
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial-ShareAlike 3.0 Unported License. To view a copy of
 * this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or
 * send a letter to:
 *
 * Creative Commons
 * 444 Castro Street, Suite 900
 * Mountain View, California, 94041, USA.
 *
 * Other Licensing is available upon request.
 *
 */

#include "OpenServo.h"

OpenServo::OpenServo(int address)
{
  //Wire = wire;
  _address = (uint8_t)address;
}

/* ================ Public methods ================ */

void OpenServo::setPosition(int pos)
{
  _OpenServoWrite16(OPENSERVO_SEEK, pos);
}


unsigned int OpenServo::getPosition()
{
  return _OpenServoRead16(OPENSERVO_POSITION);
}


void OpenServo::enable()
{
  _OpenServoCommand8(OPENSERVO_PWM_ENABLE);
}

void OpenServo::disable()
{
  _OpenServoCommand8(OPENSERVO_PWM_DISABLE);
}

/*
OpenServo::reverseSeekEnable()
{
}

OpenServo::reverseSeekDisable()
{
}*/

/* ================ Private methods ================ */

/*OpenServo::_setReverseSeek()
{
  
}*/

void OpenServo::_OpenServoWrite16(uint8_t reg, int data)
{
  // write a 16 bit register
  _OpenServoTransactionInit(reg);
 
  OS_WRITE(data >> 8);    //  high uint8_t 
  OS_WRITE(data & 0xff);  //  low uint8_t

  Wire.endTransmission();
}

unsigned int OpenServo::_OpenServoRead16(uint8_t reg)
{
  // read a 16 bit register 
  _OpenServoTransactionInit(reg);
 
  unsigned int data;
  
  Wire.requestFrom(_address, 2);
  if (Wire.available())
    data = OS_READ() << 8; // high uint8_t 
  if (Wire.available())   
    data |= OS_READ();     //  low uint8_t
 
  Wire.endTransmission();
 
  return data;
}

void OpenServo::_OpenServoCommand8(uint8_t reg)
{
  // Send an 8 bit servo command
  _OpenServoTransactionInit(reg);
  Wire.endTransmission();
}

void OpenServo::_OpenServoTransactionInit(uint8_t reg)
{
  // init an i2c transmission
  Wire.beginTransmission(_address);
  OS_WRITE(reg);
}

