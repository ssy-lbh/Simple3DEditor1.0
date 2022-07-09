#ifndef __IO_SERIALIZABLE__
#define __IO_SERIALIZABLE__

#include <define.h>

#include <io/IOStream.h>

#include <lib/json/nlohmann/json.hpp>

namespace simple3deditor {

using nlohmann::json;

class ISerializable {
public:
    virtual void Serialize(json& o) = 0;
};

class IDeserializable {
public:
    virtual void Deserialize(json& o) = 0;
};

// 多继承中只含有接口，期望数据不会有不确定性
class IMemorable : public ISerializable, public IDeserializable {
public:
    using ISerializable::Serialize;
    using IDeserializable::Deserialize;
};

}

#endif