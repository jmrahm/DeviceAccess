/*
 * RegisterAccessor.h - Non-buffering accessor for device registers
 */

#ifndef MTCA4U_REGISTER_ACCESSOR_H
#define MTCA4U_REGISTER_ACCESSOR_H

#include <typeinfo>

#include "VirtualFunctionTemplate.h"
#include "FixedPointConverter.h"
#include "RegisterInfoMap.h"
#include "BufferingRegisterAccessorImpl.h"
#include "DeviceBackend.h"

namespace mtca4u {

  /// forward declaration (only used as tempate arguments)
  class DeviceBackend;

  /** \brief DEPRECATED! Use BufferingRegisterAccessor instead!
   *  \deprecated
   *  This class is deprecated. Use BufferingRegisterAccessor instead!
   *  @todo Add printed runtime warning after release of version 0.8
   */
  class RegisterAccessor {

    public:

      /** \brief DEPRECATED! Use BufferingRegisterAccessor instead!
       *  \deprecated
       *  This class is deprecated. Use BufferingRegisterAccessor instead!
       *  @todo Add printed runtime warning after release of version 0.8
       */
      RegisterAccessor(boost::shared_ptr<DeviceBackend> deviceBackendPointer, const RegisterPath &registerPathName);

      /** \brief DEPRECATED! Use BufferingRegisterAccessor instead!
       *  \deprecated
       *  This class is deprecated. Use BufferingRegisterAccessor instead!
       *  @todo Add printed runtime warning after release of version 0.8
       */
      ~RegisterAccessor();

      /** \brief DEPRECATED! Use BufferingRegisterAccessor instead!
       *  \deprecated
       *  This class is deprecated. Use BufferingRegisterAccessor instead!
       *  @todo Add printed runtime warning after release of version 0.8
       */
      template <typename ConvertedDataType>
      void read(ConvertedDataType *convertedData, size_t nWords = 1, uint32_t wordOffsetInRegister = 0) const {
        if(nWords == 0) return;
        // obtain accessor
        auto acc = _dev->getBufferingRegisterAccessor<ConvertedDataType>(_registerPathName, nWords, wordOffsetInRegister, false);
        // perform read
        acc->read();
        // swap buffer out (to get the pointer) and copy data to target buffer
        std::vector<ConvertedDataType> temp;
        acc->swap(temp);
        memcpy(convertedData, temp.data(), nWords*sizeof(ConvertedDataType));
        acc->swap(temp);
      }

      /** \brief DEPRECATED! Use BufferingRegisterAccessor instead!
       *  \deprecated
       *  This class is deprecated. Use BufferingRegisterAccessor instead!
       *  @todo Add printed runtime warning after release of version 0.8
       */
      template <typename ConvertedDataType>
      ConvertedDataType read() const {
        ConvertedDataType temp;
        read(&temp);
        return temp;
      }

      /** \brief DEPRECATED! Use BufferingRegisterAccessor instead!
       *  \deprecated
       *  This class is deprecated. Use BufferingRegisterAccessor instead!
       *  @todo Add printed runtime warning after release of version 0.8
       */
      template <typename ConvertedDataType>
      void write(ConvertedDataType const *convertedData, size_t nWords, uint32_t wordOffsetInRegister = 0) {
        if(nWords == 0) return;
        // obtain accessor
        auto acc = _dev->getBufferingRegisterAccessor<ConvertedDataType>(_registerPathName, nWords, wordOffsetInRegister, false);
        // swap buffer out (to get the pointer) and copy data from source buffer
        std::vector<ConvertedDataType> temp;
        acc->swap(temp);
        memcpy(temp.data(), convertedData, nWords*sizeof(ConvertedDataType));
        acc->swap(temp);
        // perform write
        acc->write();
      }

      /** \brief DEPRECATED! Use BufferingRegisterAccessor instead!
       *  \deprecated
       *  This class is deprecated. Use BufferingRegisterAccessor instead!
       *  @todo Add printed runtime warning after release of version 0.8
       */
      template <typename ConvertedDataType>
      void write(ConvertedDataType const &convertedData) {
        write(&convertedData, 1);
      }

      /** \brief DEPRECATED! Use BufferingRegisterAccessor instead!
       *  \deprecated
       *  This class is deprecated. Use BufferingRegisterAccessor instead!
       *  @todo Add printed runtime warning after release of version 0.8
       */
      unsigned int getNumberOfElements() const {
        auto acc = _dev->getBufferingRegisterAccessor<int32_t>(_registerPathName, 1, 0, false);
        return acc->getNumberOfElements();
      }

      /** \brief DEPRECATED! Use BufferingRegisterAccessor instead!
       *  \deprecated
       *  This class is deprecated. Use BufferingRegisterAccessor instead!
       *  @todo Add printed runtime warning after release of version 0.8
       */
      RegisterInfoMap::RegisterInfo getRegisterInfo() const {
        RegisterInfoMap::RegisterInfo info;
        _dev->getRegisterMap()->getRegisterInfo(_registerPathName, info);
        return info;
      }

      /** \brief DEPRECATED! Use BufferingRegisterAccessor instead!
       *  \deprecated
       *  This class is deprecated. Use BufferingRegisterAccessor instead!
       *  @todo Add printed runtime warning after release of version 0.8
       */
      FixedPointConverter getFixedPointConverter() const {
        auto acc = _dev->getBufferingRegisterAccessor<int32_t>(_registerPathName, 1, 0, false);
        return acc->getFixedPointConverter();
      }

      /** \brief DEPRECATED! Use BufferingRegisterAccessor instead!
       *  \deprecated
       *  This class is deprecated. Use BufferingRegisterAccessor instead!
       *  @todo Add printed runtime warning after release of version 0.8
       */
      void readRaw(int32_t *data, size_t dataSize = 0, uint32_t addRegOffset = 0) const {
        // obtain bytes to copy
        if(dataSize == 0) dataSize = getNumberOfElements() * sizeof(int32_t);
        // check word alignment
        if(dataSize % 4 != 0 || addRegOffset % 4 != 0) {
          throw DeviceException("RegisterAccessor::writeRaw with incorrect word alignment (size and offset must be "
              "dividable by 4)",DeviceException::EX_WRONG_PARAMETER);
        }
        // obtain accessor
        auto acc = _dev->getBufferingRegisterAccessor<int32_t>(_registerPathName, dataSize/sizeof(int32_t),
            addRegOffset/sizeof(int32_t), true);
        // perform read
        acc->read();
        // swap buffer out (to get the pointer) and copy to target buffer
        std::vector<int32_t> temp;
        acc->swap(temp);
        memcpy(data, temp.data(), dataSize);
        acc->swap(temp);
      }

      /** \brief DEPRECATED! Use BufferingRegisterAccessor instead!
       *  \deprecated
       *  This class is deprecated. Use BufferingRegisterAccessor instead!
       *  @todo Add printed runtime warning after release of version 0.8
       */
      void writeRaw(int32_t const *data, size_t dataSize = 0, uint32_t addRegOffset = 0) {
        // obtain bytes to copy
        if(dataSize == 0) dataSize = getNumberOfElements() * sizeof(int32_t);
        // check word alignment
        if(dataSize % 4 != 0 || addRegOffset % 4 != 0) {
          throw DeviceException("RegisterAccessor::writeRaw with incorrect word alignment (size and offset must be "
              "dividable by 4)",DeviceException::EX_WRONG_PARAMETER);
        }
        // obtain accessor
        auto acc = _dev->getBufferingRegisterAccessor<int32_t>(_registerPathName, dataSize/sizeof(int32_t),
            addRegOffset/sizeof(int32_t), true);
        // swap buffer out (to get the pointer) and copy data from source buffer
        std::vector<int32_t> temp;
        acc->swap(temp);
        memcpy(temp.data(), data, dataSize);
        acc->swap(temp);
        // perform write
        acc->write();
      }

      /** \brief DEPRECATED! Use BufferingRegisterAccessor instead!
       *  \deprecated
       *  This class is deprecated. Use BufferingRegisterAccessor instead!
       *  @todo Add printed runtime warning after release of version 0.8
       */
      void readDMA(int32_t *data, size_t dataSize = 0, uint32_t addRegOffset = 0) const;

      /** \brief DEPRECATED! Use BufferingRegisterAccessor instead!
       *  \deprecated
       *  This class is deprecated. Use BufferingRegisterAccessor instead!
       *  @todo Add printed runtime warning after release of version 0.8
       */
      void writeDMA(int32_t const *data, size_t dataSize = 0, uint32_t addRegOffset = 0);

    protected:

      /** Path name of the register to access */
      RegisterPath _registerPathName;

      /** Pointer to the device backend used for reading and writing the data */
      boost::shared_ptr<DeviceBackend> _dev;

  };

} // namespace mtca4u

#endif /* MTCA4U_REGISTER_ACCESSOR_H */
