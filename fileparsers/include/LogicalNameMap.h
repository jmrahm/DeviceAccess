/*
 * LogicalNameMap.h
 *
 *  Created on: Feb 8, 2016
 *      Author: Martin Hierholzer
 */

#ifndef MTCA4U_LOGICAL_NAME_MAP_H
#define MTCA4U_LOGICAL_NAME_MAP_H

#include <string>
#include <map>
#include <unordered_set>
#include <boost/shared_ptr.hpp>

#include "BufferingRegisterAccessor.h"
#include "DeviceBackend.h"

// forward declaration
namespace xmlpp {
  class Node;
}

namespace mtca4u {

  class LogicalNameMap {

    public:
      // forward declaration
      class RegisterInfo;

      /** Sub-class: value of a RegisterInfo field (with proper resolution of dynamic references) */
      template<typename ValueType>
      class Value {

        public:

          // obtain value via implicit type conversion operator
          operator const ValueType&() const {
            if(!hasActualValue) {
              if(!accessor) accessor = backend->getBufferingRegisterAccessor<ValueType>("",registerName);
              accessor->read();
              return (*accessor)[0];
            }
            return value;
          }

          // assignment operator with a value of the type ValueType. This will make the Value having an "actual"
          // value, i.e. no dynamic references are present.
          Value<ValueType>& operator=(const ValueType &rightHandSide) {
            value = rightHandSide;
            hasActualValue = true;
            return *this;
          }

          // allows comparisons with char* if ValueType is std::string
          bool operator==(const char *rightHandSide) {
            return value == rightHandSide;
          }

          // default constructor: no backend will be initialised, so no accessors can be created. It may not be
          // possible to obtain the value like this!
          Value()
          : hasActualValue(true), backend()
          {}

          // assignment operator: allow assigment to Value without given backend, in which case we keep our backend
          // and create the accessors
          Value<ValueType>& operator=(const Value<ValueType> &rightHandSide) {
            if(rightHandSide.hasActualValue) {
              hasActualValue = true;
              value = rightHandSide.value;
            }
            else {
              // we obtain the register accessor later, in case the map file was not yet parsed up to its definition
              hasActualValue = false;
              registerName = rightHandSide.registerName;
              accessor.reset();
            }
            return *this;
          }

        protected:

          // constructor: assume having an actual value by default
          Value(const boost::shared_ptr<DeviceBackend> &_backend)
          : hasActualValue(true), backend(_backend)
          {}

          // flag if the actual value is already known and thus the member variable "value" is valid.
          bool hasActualValue;

          // field to store the actual value
          ValueType value;

          // name of the register to obtain the value from
          std::string registerName;

          // backend to obtain the register accessors from
          boost::shared_ptr<DeviceBackend> backend;

          // register accessor to obtain the value, if not yet known upon construction
          mutable boost::shared_ptr< BufferingRegisterAccessorImpl<ValueType> > accessor;

          friend class LogicalNameMap;
          friend class LogicalNameMap::RegisterInfo;
      };

      /** Potential target types */
      enum TargetType { INVALID, REGISTER, RANGE, CHANNEL, INT_CONSTANT, INT_VARIABLE };

      /** Sub-class: single entry of the logical name mapping */
      class RegisterInfo {
        public:

          /** Type of the target */
          TargetType targetType;

          /** The target device alias */
          Value<std::string> deviceName;

          /** The target register name */
          Value<std::string> registerName;

          /** The first index in the range */
          Value<unsigned int> firstIndex;

          /** The length of the range (i.e. number of indices) */
          Value<unsigned int> length;

          /** The channel of the target 2D register */
          Value<unsigned int> channel;

          /** The constant integer value */
          Value<int> value;

          /** test if deviceName is set (depending on the targetType) */
          bool hasDeviceName() const {
            return targetType != TargetType::INT_CONSTANT && targetType != TargetType::INT_VARIABLE;
          }

          /** test if registerName is set (depending on the targetType) */
          bool hasRegisterName() const {
            return targetType != TargetType::INT_CONSTANT && targetType != TargetType::INT_VARIABLE;
          }

          /** test if firstIndex is set (depending on the targetType) */
          bool hasFirstIndex() const {
            return targetType == TargetType::RANGE;
          }

          /** test if length is set (depending on the targetType) */
          bool hasLength() const {
            return targetType == TargetType::RANGE;
          }

          /** test if channel is set (depending on the targetType) */
          bool hasChannel() const {
            return targetType == TargetType::CHANNEL;
          }

          /** test if value is set (depending on the targetType) */
          bool hasValue() const {
            return targetType == TargetType::INT_CONSTANT || targetType == TargetType::INT_VARIABLE;
          }

        protected:

          // constuctor: initialise Values
          RegisterInfo(const boost::shared_ptr<DeviceBackend> &backend)
          : targetType(TargetType::INVALID),
            deviceName(backend),
            registerName(backend),
            firstIndex(backend),
            length(backend),
            channel(backend),
            value(backend)
          {}

          friend class LogicalNameMap;
      };

      /** Constructor: parse map from XML file */
      LogicalNameMap(const std::string &fileName, const boost::shared_ptr<DeviceBackend> &backend) {
        parseFile(fileName, backend);
      }

      /** Constructor: parse map from XML file. No backend is specified, so internal references cannot be resolved. */
      LogicalNameMap(const std::string &fileName) {
        parseFile(fileName, boost::shared_ptr<DeviceBackend>());
      }

      /** Default constructor: Create empty map. */
      LogicalNameMap() {
      }

      /** Obtain register information for the named register. The register information can be updated, which *will*
       *  have an effect on the logical map itself (and thus any other user of the same register info)! */
      boost::shared_ptr<RegisterInfo> getRegisterInfoShared(const std::string &name);

      /** Obtain register information for the named register (const version) */
      const RegisterInfo& getRegisterInfo(const std::string &name) const;

      /** Obtain list of all target devices referenced in the map */
      std::unordered_set<std::string> getTargetDevices() const;

    protected:

      std::string _fileName;

      /** actual register info map (register name to target information) */
      std::map< std::string, boost::shared_ptr<RegisterInfo> > _map;

      /** parse the given XML file */
      void parseFile(const std::string &fileName, const boost::shared_ptr<DeviceBackend> &backend);

      /** throw a parsing error with more information */
      void parsingError(const std::string &message);

      /** Build a Value object for a given subnode. */
      template<typename ValueType>
      Value<ValueType> getValueFromXmlSubnode(const xmlpp::Node *node, const std::string &subnodeName);

  };

} // namespace mtca4u

#endif /* MTCA4U_LOGICAL_NAME_MAP_H */
