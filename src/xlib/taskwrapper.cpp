#include "xlib/taskwrapper.hpp"

/*
 * The TaskWrapper class abstracts away the complicated syntax for declaring and
 * managing tasks, simplifying the implementation of asynchronous functions. 
 * Other classes need only inherit from TaskWrapper and immediately have a
 * fully-functional task manager that can be overwritten as necessary using
 * virtual functions
 */

namespace xlib {
    //Destructor. Ensures task is removed upon deallocation
    TaskWrapper::~TaskWrapper() {
        task->remove();
        task.reset(nullptr);
    }

    //Calls function trampoline() and allocates task
    void TaskWrapper::startTask(const char* iname) {
        task = std::move(std::make_unique<pros::Task>(trampoline, this, iname));
    }

    //Suspends running task
    void TaskWrapper::pauseTask() {
        task->suspend();
    }

    //Resumes suspended task
    void TaskWrapper::resumeTask() {
        task->resume();
    }

    //Deallocates task
    void TaskWrapper::stopTask() {
        task->remove();
        task = nullptr;
    }

    //Trivial getter of task's name
    const char* TaskWrapper::getName() {
        return task->get_name();
    }

    //Calls virtual function loop()
    void TaskWrapper::trampoline(void* iparam) {
        if(iparam) {
            TaskWrapper* that = static_cast<TaskWrapper*>(iparam);
            that->loop();
        }
    }
}