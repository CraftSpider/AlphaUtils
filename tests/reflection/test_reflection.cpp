
#include "test_reflection.h"
#include "at_tests"
#include "at_reflect"


using namespace reflect;


class TestClass {
public:
    
    int a = 5;
    TestClass* b = nullptr;
    
    TestClass() = default;
    
    TestClass(int a, TestClass* b) {
        this->a = a;
        this->b = b;
    }
    
    ~TestClass() = default;
    
};


DECLARE_TYPE(TestClass)
DECLARE_TYPE(TestClass*)

DECLARE_CONSTRUCTOR(TestClass, int, TestClass*)
DECLARE_MEMBER_DATA(TestClass, a)
DECLARE_MEMBER_DATA(TestClass, b)


void test_construction() {
    Type* t = Type::from<TestClass>();
    
    std::vector<Constructor*> constructors = t->get_constructors();
    
    Constructor* c1 = t->get_constructor({});
    ASSERT(c1 != nullptr);
    Constructor* c2 = t->get_constructor({Type::from<int>(), Type::from<TestClass*>()});
    ASSERT(c2 != nullptr);
    
    TestClass tc1 = c1->construct({}).get_value_ref<TestClass>();
    TestClass tc2 = c2->construct(
        {Variant::from_instance(1), Variant::from_instance(&tc1)}
    ).get_value_ref<TestClass>();
    
    ASSERT(tc1.a == 5, "TestClass a wasn't default");
    ASSERT(tc1.b == nullptr, "TestClass b wasn't default");
    
    ASSERT(tc2.a == 1, "TestClass a wasn't 1");
    ASSERT(tc2.b == &tc1, "TestClass b wasn't tc1");
}


void test_member_data() {
    Type* t = Type::from<TestClass>();
    
    Constructor* c = t->get_constructor({Type::from<int>(), Type::from<TestClass*>()});
    
    TestClass tc = c->construct(
        {Variant::from_instance(-10), Variant::from_instance((TestClass*)nullptr)}
    ).get_value_ref<TestClass>();
    
    MemberProperty* mp = t->get_property("a");
    MemberProperty* mp2 = t->get_property("b");
    
    int a = mp->get(Variant::from_ref(tc)).get_value<int>();
    ASSERT(a == tc.a, "Get a didn't work");
    
    TestClass* b = mp2->get(Variant::from_ref(tc)).get_value<TestClass*>();
    ASSERT(b == tc.b, "Get b didn't work");
    
    mp->set(Variant::from_ref(tc), Variant::from_instance(10));
    ASSERT(tc.a == 10, "Set a didn't work");
    mp2->set(Variant::from_ref(tc), Variant::from_instance(&tc));
    ASSERT(tc.b == &tc, "Set b didn't work");
    
    // TODO: assert throws tests for type safety
}


void test_destruction() {
    Type* t = Type::from<TestClass>();
    
    TestClass* tc = new TestClass();
    
    t->get_destructor()->destruct(Variant::from_ptr(tc));
}


void test_fully_variant() {
    Type* int_type = Type::from_name("int");
    Type* tc_type = Type::from_name("TestClass");
    
    Variant int_instance = int_type->get_constructor({})->construct({});
    Variant tc_instance = tc_type->get_constructor({})->construct({});
    
    tc_type->get_property("a")->set(tc_instance, int_instance);
    
    Variant a_value = tc_type->get_property("a")->get(tc_instance);
    
    ASSERT(a_value.get_value_ref<int>() == 0); // This is the only place we actually use non-reflection a type!
    
    int_type->get_destructor()->destruct(int_instance);
    tc_type->get_destructor()->destruct(tc_instance);
}


void run_reflection_tests() {
    TEST(test_construction)
    TEST(test_member_data)
    TEST(test_destruction)
    
    TEST(test_fully_variant)
}
