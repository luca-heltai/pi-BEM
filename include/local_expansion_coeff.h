/* 
 * File:   LocalExpansionCoeff.hpp
 * Author: matteo
 *
 * Created on April 3, 2015, 6:05 PM
 */

#ifndef LOCALEXPANSIONCOEFF_H
#define	LOCALEXPANSIONCOEFF_H

class LocalExpansionCoeff {
public:
    LocalExpansionCoeff();
    LocalExpansionCoeff(const unsigned int & p);
    LocalExpansionCoeff(const LocalExpansionCoeff& orig);
    double const get(const unsigned int & n, const unsigned int & m, const unsigned int & nn, const unsigned int & mm);
    void set(const unsigned int & n, const unsigned int & m, const unsigned int & nn, const unsigned int & mm, const double &value);
    unsigned int const getNumberOfElements();
    unsigned int const getNNOffset(const unsigned int & nn);
    unsigned int const getMOffset(const unsigned int & m);
    unsigned int const getNOffset(const unsigned int & n);
    virtual ~LocalExpansionCoeff();
    
    //Debugging and test of indexes
//    static unsigned int const loopDebugger(const unsigned int & p);
//    void fillCoeffWithIndex();
//    void printCoeff();
    
private:
    unsigned int _p;
    double * _coeff;

};

#endif	/* LOCALEXPANSIONCOEFF_HPP */

