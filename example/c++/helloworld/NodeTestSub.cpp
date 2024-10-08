#include "NodeHandler.h"
#include "hello.pb.h"

std::mutex mutex_;

void cb(hello::hello m) {
    mutex_.lock();
    std::cout << "string_field: " << m.string_field() << "\n";
    std::cout << "enum_field: " << m.enum_field() << "\n";
    mutex_.unlock();
}

int main() {
    core::NodeHandler nh;
    core::Rate rate(1000);
    core::Subscriber<hello::hello> &sub = nh.subscribe<hello::hello>("hello", 1000, cb);
    int i = 0;
    while (1)
    {
        core::spinOnce();
        std::cout << rate.sleep() << " << sleep" << "\n";
    }
    return 0;
}
