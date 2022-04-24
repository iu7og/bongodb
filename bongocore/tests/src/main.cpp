#include <iostream>

#include <backend/VectorCommandsBuffer.h>

int main() {
    bongodb::Backend::VectorCommandsBuffer buffer;

    auto cmd1 = std::make_shared<bongodb::Common::TTruncateStreamCommand>();
    auto cmd2 = std::make_shared<bongodb::Common::TRemoveStreamCommand>("asd");
    auto cmd3 = std::make_shared<bongodb::Common::TPutStreamCommand>("basd", "hello");

    buffer.Push(cmd1);
    buffer.Push(cmd2);
    buffer.Push(cmd3);

    auto p1 = buffer.GetByVersion(1);
    auto p2 = buffer.GetByVersion(2);
    auto p3 = buffer.GetByVersion(3);

    auto p4 = buffer.GetByVersion(111);

    try {
        auto p5 = buffer.GetByVersion(0);
    } catch (const std::invalid_argument&) {
       std::cout << "caught!" << std::endl;
    }

    return 0;
}