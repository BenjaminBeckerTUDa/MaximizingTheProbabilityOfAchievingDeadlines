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


// simple container for all kinds of data structures...
// packets in castalia do not like std objects, but accept this kind of classes containing the objects
#include "AGGL.h"

std::list<int> AGGL::getListInt(){
	return listInt;
}
std::list<double> AGGL::getListDouble(){
	return listDouble;
}
std::list<ODARPacket *> AGGL::getListODAR(){
	return listODAR;
}

void AGGL::setListInt (std::list<int> listInt1) {
	listInt = listInt1;
}
void AGGL::setListDouble (std::list<double> listDouble1) {
	listDouble = listDouble1;
}
void AGGL::setListODAR (std::list<ODARPacket *> listODAR1) {
	listODAR = listODAR1;
}

