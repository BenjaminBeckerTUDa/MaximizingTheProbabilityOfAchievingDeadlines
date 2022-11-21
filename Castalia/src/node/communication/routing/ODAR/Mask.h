#ifndef _MASK_H_
#define _MASK_H_

class Mask{
 private:
    int shift;
    int slots;
    double * mask;

 public:
	double* getMask();
	int getShift();
    int getSlots();

	void setShift (int) ;
    void setSlots (int) ;
    void setMask (double * ) ;
};

#endif				
