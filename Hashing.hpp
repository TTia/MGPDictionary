#ifndef HASHING_HPP
#define HASHING_HPP

/*!
 * \brief The Hashing class
 * Data l'impronta hash della chiave e la dimensione della tabella, viene calcolata
 * la normalizzazione.
 */
class Hashing{
public:
  unsigned long hash(const unsigned long, const unsigned long) = 0;
  unsigned long operator()(const unsigned long, const unsigned long) = 0;
};

class DivisionMethodHashing: public Hashing{
public:
  inline unsigned long hash(unsigned long, unsigned long) const{

  }
  inline unsigned long operator()(unsigned long, unsigned long) = 0;
}

#endif // HASHING_HPP
