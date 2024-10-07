#include "NodeHandler.h"
#include "hello.pb.h"
#include "google/protobuf/text_format.h"


int main() {
    core::NodeHandler nh;
    core::Rate rate(1);
    core::ServiceClient<hello::hellorequest, hello::helloreply> &clt = nh.serviceClient<hello::hellorequest, hello::helloreply>("hello");
    int i = 0;
    while (1)
    {
        hello::hellorequest request;
        hello::helloreply reply;
        request.set_index(i++);
        if (clt.pull_request(request, reply)) {
            std::string receive;
            google::protobuf::TextFormat::PrintToString(reply, &receive);
            std::cout << receive << "\n";
        }
        std::cout << rate.sleep() << " << sleep" << "\n";
    }
    return 0;
}
