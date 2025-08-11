#pragma once

#include "../../../utility/cache.hpp"
#include "../../../../vendor/kernel/communications.hpp"
#include "../../../utility/offsets.hpp" 

class actor_cache_class
{
public:
	void actor_cache();
};

inline actor_cache_class* actor_cache = new actor_cache_class();