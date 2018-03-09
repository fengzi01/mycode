#include "test.pb.h"
#include <string>
#include <iostream>
using std::cout;
using std::endl;

int main() {
    std::string type_name = Query::descriptor()->full_name();
    std::cout << type_name << std::endl;
    const google::protobuf::Descriptor *descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);

    cout << "FindMessageTypeByName() = " << descriptor << endl;
    cout << "Query::descriptor = " << Query::descriptor() << endl;

    const google::protobuf::Message *prototype = 
        google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);

    cout << "GetPrototype() = " << prototype << endl;
    cout << "Query::default_instance = " << &Query::default_instance() << endl;

    Query *new_obj = dynamic_cast<Query *>(prototype->New());

    cout << typeid(new_obj).name() << endl;
}
