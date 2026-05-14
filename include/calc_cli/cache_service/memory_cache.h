#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include <cstddef>

#include "calc_cli/application/context.h"

namespace calc_cli {

class MemoryCache {
public:
    MemoryCache() = default;

    explicit MemoryCache(
        const std::unordered_map<std::string, ApplicationContext>& records
    );

    ~MemoryCache() = default;

    MemoryCache(const MemoryCache& other) = delete;
    MemoryCache& operator=(const MemoryCache& other) = delete;

    MemoryCache(MemoryCache&& other) = default;
    MemoryCache& operator=(MemoryCache&& other) = default;

    void load(std::unordered_map<std::string, ApplicationContext>& cache);
    void save(const std::string& key, const ApplicationContext& context);
    std::optional<ApplicationContext> find(const std::string& key) const;

    std::size_t size() const;

private:
    std::unordered_map<std::string, ApplicationContext> cache_;
};
    
}