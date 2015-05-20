/* 
 * File:   donuts.hpp
 * Author: apple
 *
 * Created on October 7, 2014, 9:36 AM
 */

#ifndef DONUTS_HPP
#define	DONUTS_HPP
#include "globals.hpp"


struct donut_ring {
    int flavor [NUMFLAVORS] [NUMSLOTS];
    int outptr [NUMFLAVORS];
};

#endif	/* DONUTS_HPP */

