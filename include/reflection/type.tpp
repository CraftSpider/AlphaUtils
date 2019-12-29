
#include <typeinfo>
#include "reflection/errors.h"

namespace reflect {

template<typename T>
std::string MetaType<T>::get_name() {
    return "[UNKNOWN TYPE]";
}

template<typename T>
Type::Type(MetaType<T> meta) {
    (void)meta;
    name = MetaType<T>::get_name();
}

template<typename T>
Type* Type::from() {
    std::map<std::string, Type*>& types = __type_map();
    std::string name = MetaType<T>::get_name();
    
    if (name == "[UNKNOWN TYPE]") {
        throw invalid_type("Attempt to get unregistered type '" + std::string(typeid(T).name()) + "'. Make sure to DECLARE_TYPE(typename).");
    }
    
    if (types.count(name) == 0) {
        types[name] = new Type(MetaType<T>());
    }
    
    return types[name];
}

template<typename T>
Type* Type::from_instance(T*) {
    std::map<std::string, Type*>& types = __type_map();
    std::string name = MetaType<T>::get_name();
    
    if (name == "[UNKNOWN TYPE]") {
        throw invalid_type("Attempt to get unregistered type '" + std::string(typeid(T).name()) + "'. Make sure to DECLARE_TYPE(typename).");
    }
    
    if (types.count(name) == 0) {
        types[name] = new Type(MetaType<T>());
    }
    
    return types[name];
}

template<typename... Args>
std::vector<Type*> Type::from_list() {
    return {from<Args>()...};
}

}
