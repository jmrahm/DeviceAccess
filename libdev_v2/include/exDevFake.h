#ifndef MTCA4U_EXDEVFAKE_H
#define	MTCA4U_EXDEVFAKE_H

#include "exBase.h"
#include <string>

namespace mtca4u{

class exDevFake : public exBase {
public:
    enum {
                EX_CANNOT_CREATE_DEV_FILE, 
                EX_DEVICE_FILE_WRITE_DATA_ERROR, 
                EX_DEVICE_FILE_READ_DATA_ERROR, 
                EX_DEVICE_FILE_WRITE_DMA_ERROR, 
                EX_DEVICE_FILE_READ_DMA_ERROR,
                EX_DEVICE_OPENED, 
                EX_DEVICE_CLOSED
    };
public:
    exDevFake(const std::string &_exMessage, unsigned int _exID);
private:

};

}//namespace mtca4u

#endif	/* MTCA4U_EXDEVFAKE_H */

