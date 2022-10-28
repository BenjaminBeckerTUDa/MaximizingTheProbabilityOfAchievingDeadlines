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
#include "CFP.h"

double* CFP::getDoubleArray(){
	return doubleArray;
}

std::map<int,int> CFP::getMapIntInt(){
	 return mapIntInt;
}

std::map<int,double> CFP::getMapIntDouble(){
	return mapIntDouble;
}

std::list<int> CFP::getListInt(){
	return listInt;
}
std::list<double> CFP::getListDouble(){
	return listDouble;
}

std::set<int> CFP::getSetInt(){
	return setInt;
}

std::set<double> CFP::getSetDouble(){
	return setDouble;
}



void CFP::setDoubleArray (double* doubleArray1) {
	doubleArray = doubleArray1;
}
void CFP::setMapIntInt (std::map<int,int> mapIntInt1) {
	mapIntInt = mapIntInt1;
}
void CFP::setMapIntDouble (std::map<int,double> mapIntDouble1) {
	mapIntDouble = mapIntDouble1;
}
void CFP::setListInt (std::list<int> listInt1) {
	listInt = listInt1;
}
void CFP::setListDouble (std::list<double> listDouble1) {
	listDouble = listDouble1;
}

void CFP::setSetInt (std::set<int> setInt1)
{
	setInt = setInt1;
}
void CFP::setSetDouble (std::set<double> setDouble1)
{
	setDouble = setDouble;
}
