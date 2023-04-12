/*******************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2011                            *
 *  Developed at the ATP lab, Networked Systems research theme                 *
 *  Author(s): Athanassios Boulis, Yuriy Tselishchev                           *
 *  This file is distributed under the terms in the attached LICENSE file.     *
 *  If you do not find this file, copies can be found by writing to:           *
 *                                                                             *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia                *
 *      Attention:  License Inquiry.                                           *
 *                                                                             *
 *******************************************************************************/

#ifndef _AGGL_H_
#define _AGGL_H_

#include <list>
#include "ODARPacket_m.h"


class AGGL{
 private:
	double* doubleArray;

	std::list<int> listInt;
	std::list<double> listDouble;
	std::list<ODARPacket *> listODAR;

 public:
	
	std::list<int> getListInt();
	std::list<double> getListDouble();
	std::list<ODARPacket *> getListODAR();
	
	void setListInt (std::list<int>) ;
	void setListDouble (std::list<double>) ;
	void setListODAR (std::list<ODARPacket *>) ;


};

#endif				
