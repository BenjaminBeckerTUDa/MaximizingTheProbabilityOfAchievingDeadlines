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

#ifndef _CFP_H_
#define _CFP_H_

#include <map>
#include <list>
#include <set>


class CFP{
 private:
	double* doubleArray;

	std::map<int,int> mapIntInt;
	std::map<int,double> mapIntDouble;
	
	std::set<int> setInt;
	std::set<double> setDouble;

	std::list<int> listInt;
	std::list<double> listDouble;
 public:
	double* getDoubleArray();

	std::map<int,int> getMapIntInt();
	std::map<int,double> getMapIntDouble();

	std::list<int> getListInt();
	std::list<double> getListDouble();

	std::set<int> getSetInt();
	std::set<double> getSetDouble();

	void setDoubleArray (double*) ;

	void setMapIntInt (std::map<int,int>) ;
	void setMapIntDouble (std::map<int,double>) ;
	
	void setListInt (std::list<int>) ;
	void setListDouble (std::list<double>) ;

	void setSetInt (std::set<int>) ;
	void setSetDouble (std::set<double>) ;


};

#endif				
