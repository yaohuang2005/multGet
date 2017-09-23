
#ifndef MULTIGET_MANAGER_H
#define MULTIGET_MANAGER_H
#include "Writer.h"
#include <thread>
#include <vector>

class Manager {
public:
    Manager(Writer &writer_, std::vector<std::thread> &threads_);
    virtual ~Manager();

    Writer &getWriter_();
    std::vector<std::thread> &getThreads_();

private:
    Writer &writer_;
    std::vector<std::thread> &threads_;
};


#endif //MULTIGET_MANAGER_H
