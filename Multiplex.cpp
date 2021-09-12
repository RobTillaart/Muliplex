//
//    FILE: Multiplex.cpp
//  AUTHOR: Rob Tillaart
// VERSION: 0.2.1
// PURPOSE: Arduino library to multiplex streams
//    DATE: 2021-01-09
//     URL: https://github.com/RobTillaart/Multiplex
//
//  HISTORY:
//  0.1.0   2021-01-09  initial version
//  0.2.0   2021-08-09  See issues #2 and #3
//  0.2.1   2021-09-    made index(Stream) public
//                      enable() / disable() return true on success
//                      added free() function
//                      minor refactor.


#include "Multiplex.h"


Multiplex::Multiplex()
{
  // malloc ?
  _size = MAX_MULTIPLEX;
  reset();
}


Multiplex::~Multiplex()
{
  // free ?
}


void Multiplex::reset()
{
  for (uint8_t i = 0; i < _size; i++)
  {
    _enabled[i] = false;
  }
  _count = 0;
}


bool Multiplex::add(Print * stream)
{
  if (_count >= _size) return false;
  if (index(stream) != 0xFF) return false;

  _enabled[_count]  = true;
  _stream[_count++] = stream;
  return true;
};


///////////////////////////////////////////
//
// WRITE - the core
//
size_t Multiplex::write(uint8_t c)
{
  size_t n = 0;
  for (uint8_t i = 0; i < _count; i++)
  {
    if (_enabled[i])
    {
      n += _stream[i]->write(c);
    }
  }
  return n;
}


size_t Multiplex::write(const uint8_t *buffer, size_t size) {
  size_t n = 0;
  for (uint8_t i = 0; i < _count; i++)
  {
    if (_enabled[i])
    {
      n += _stream[i]->write(buffer, size);
    }
  }
  return n;
}


uint8_t Multiplex::index(Print *stream) 
{
  for (uint8_t i = 0; i < _count; i++)
  {
    if (stream == _stream[i]) 
    {
      return i;
    }
  }
  return 0xFF;
}


bool Multiplex::enable(uint8_t n)
{
  if (n >= _count) return false;
  _enabled[n] = true;
  return true;
}


bool Multiplex::enableStream(Print *stream)
{
  return enable(index(stream));
}


bool Multiplex::disable(uint8_t n)
{
  if (n >= _count) return false;
  _enabled[n] = false;
  return true;
}


bool Multiplex::disableStream(Print *stream)
{
  return disable(index(stream));
}


bool Multiplex::isEnabled(uint8_t n)
{
  if (n >= _count) return false;
  return _enabled[n];
}


bool Multiplex::isEnabledStream(Print *stream)
{
  return isEnabled(index(stream));
}


// -- END OF FILE --
