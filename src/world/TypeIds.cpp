#include "kronkworld/TypeId.hpp"
#include <mutex>
#include <string>
#include <unordered_map>

namespace
{
    // Never destroyed: a type can be numbered by a static destructor, after this file's statics are gone
    struct Table {
        std::mutex mutex;
        std::unordered_map<std::string, size_t> ids;

        size_t of(std::string_view name)
        {
            const std::lock_guard<std::mutex> lock(mutex);
            // C++20 has no heterogeneous lookup for unordered_map with a string_view key: one copy per call
            // of the first use of each type only (the callers keep the result in a static)
            return ids.try_emplace(std::string(name), ids.size()).first->second;
        }
    };
}

size_t kw::detail::resourceTypeId(std::string_view name) noexcept
{
    static Table* const table = new Table;
    return table->of(name);
}

size_t kw::detail::componentTypeId(std::string_view name) noexcept
{
    static Table* const table = new Table;
    return table->of(name);
}
