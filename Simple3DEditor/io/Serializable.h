#ifndef __IO_SERIALIZABLE__
#define __IO_SERIALIZABLE__

#include <define.h>

#include <io/IOStream.h>

#include <lib/json/nlohmann/json_fwd.hpp>

namespace simple3deditor {

class ISerializable {
public:
    virtual void Serialize(nlohmann::json& o) = 0;
};

class IDeserializable {
public:
    virtual void Deserialize(nlohmann::json& o) = 0;
};

// ��̳���ֻ���нӿڣ��������ݲ����в�ȷ����
class IMemorable : public ISerializable, public IDeserializable {
public:
    using ISerializable::Serialize;
    using IDeserializable::Deserialize;
};

}

#endif