#ifndef __SERIALIZATION_SYSTEM_
#define __SERIALIZATION_SYSTEM_

#include <fstream>

#include <GameEngineFramework/configuration.h>


class ENGINE_API Serialization {
    
public:
    
    Serialization();
    
    /// Serialize data out to a file.
    bool Serialize(std::string filename, void* buffer, unsigned int size);
    
    /// Deserialize data in from a file.
    bool Deserialize(std::string filename, void* buffer, unsigned int size);
    
    
private:
    
};


#endif
