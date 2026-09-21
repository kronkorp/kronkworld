/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** Encapsulation of kronkworld
*/
#ifndef KRONKWORLD_SCH_ENCAPS_H
    #define KRONKWORLD_SCH_ENCAPS_H
    extern "C" {
        #include "kronkflow/macros/types.h"
        #include "kronkflow/scheduler.h"
        #include "kronkflow/task.h"
    }
    #include <cstddef>
    #include <new>

namespace kw
{

    class Scheduler
    {
        public:
            Scheduler(size_t size)
            {
                sch = kfScheduler_create(size);
                if (sch == NULL) {
                    throw std::bad_alloc();
                }
            }

            Scheduler(Scheduler& other) = delete;
            Scheduler(Scheduler&& other) = delete;

            ~Scheduler()
            {
                kfScheduler_destroy(sch);
            }

            size_t tick(void *ctx)
            {
                return kfScheduler_tick(sch, ctx);
            }

            size_t tick(void) const
            {
                return kfScheduler_currentTick(sch);
            }

            kfTaskID pushTask(
                const kfTaskOpt& opt,
                size_t delay,
                size_t interval
            )
            {
                return kfScheduler_addTask(sch, opt, delay, interval);
            }

            // true if the task was found (its clearer has run or will run)
            bool remove(kfTaskID id)
            {
                return kfScheduler_removeTask(sch, id) != 0;
            }

        private:
            kfScheduler* sch;
    };

}

#endif /* KRONKWORLD_SCH_ENCAPS_H */
