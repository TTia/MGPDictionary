#ifndef PROBINGEXCEPTIONS_HPP
#define PROBINGEXCEPTIONS_HPP

#include <exception>

class FullHashTable: public std::exception{
  virtual const char* what() const throw()
  {
    return "No free slots.";
  }
} FullHashTable;

#endif // PROBINGEXCEPTIONS_HPP
