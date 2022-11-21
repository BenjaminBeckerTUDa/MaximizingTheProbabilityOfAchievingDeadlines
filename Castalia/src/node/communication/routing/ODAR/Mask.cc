#include "Mask.h"

double* Mask::getMask(){
	return mask;
}
int Mask::getShift(){
	return shift;
}
int Mask::getSlots(){
	return slots;
}
void Mask::setShift(int s){
	shift = s;
}
void Mask::setSlots(int s){
	slots = s;
}
void Mask::setMask(double * m ){
	mask = m;
}

