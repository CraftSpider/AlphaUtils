
#include "utils/sfinae.h"

namespace reflect {

template<typename T, typename Ret, typename... Args, size_t... I>
Variant __member_invoke_impl(Ret(T::*ptr)(Args...), Variant instance, std::vector<Variant>& args, util::TemplateRange<I...>) {
    if constexpr (std::is_same<void, Ret>::value) {
        T& temp = instance.get_value_ref<T>();
        
        (temp.*ptr)(args[I].get_value_ref<Args>()...);
        return Variant::from_type(Type::from<void>());
    } else {
        return Variant::from_instance<Ret>((instance.get_value_ref<T>().*ptr)(args[I].get_value_ref<Args>()...));
    }
}

template<typename T, typename Ret, typename... Args>
Variant MemberFunctionMeta<T, Ret, Args...>::invoke(Ret(T::**ptr)(Args...), Variant& instance, std::vector<Variant>& args) {
    return __member_invoke_impl<T, Ret, Args...>(*ptr, instance, args, util::VariadicRange<Args...>());
}

template<typename T, typename Ret, typename... Args>
MemberFunctionFuncRef MemberFunctionMeta<T, Ret, Args...>::get_invoke_func() {
    return (MemberFunctionFuncRef)&invoke;
}

template<typename T, typename Ret, typename... Args>
MemberFunction::MemberFunction(Ret(T::*ptr)(Args...), std::string name) {
    type = Type::from<T>();
    return_type = Type::from<Ret>();
    arg_types = Type::from_list<Args...>();
    this->name = std::move(name);
    
    using PtrType = Ret(T::*)(Args...);
    this->ptr = new PtrType(ptr);
    invoke_ptr = MemberFunctionMeta<T, Ret, Args...>::get_invoke_func();
    num_args = sizeof...(Args);
}

template<typename T, typename Ret, typename... Args>
MemberFunction& MemberFunction::from(Ret (T::*ptr)(Args...), std::string name) {
    static std::map<std::string, MemberFunction*> member_functions = std::map<std::string, MemberFunction*>();
    
    if (member_functions.count(name) == 0) {
        member_functions[name] = new MemberFunction(ptr, name);
    }
    
    return *member_functions[name];
}

}
