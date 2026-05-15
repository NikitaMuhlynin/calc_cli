#include "calc_cli/cache_service/memory_cache.h"


namespace calc_cli {

MemoryCache::MemoryCache(
    std::unordered_map<std::string, ApplicationContext> records
    ) : cache_(std::move(records)) {}

void MemoryCache::load(
    std::unordered_map<std::string, ApplicationContext>& cache
) {
    cache_ = std::move(cache);
}

void MemoryCache::save(
    const std::string& key,
    const ApplicationContext& context) {
    cache_[key] = context;
}

std::optional<ApplicationContext> MemoryCache::find(
    const std::string& key
    ) const {
    const auto iter = cache_.find(key);

    if (iter == cache_.end())
        return std::nullopt;
    
    return iter->second;
}

std::size_t MemoryCache::size() const {
    return cache_.size();
}

}