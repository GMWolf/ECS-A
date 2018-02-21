#pragma once
#define MAX_COMPONENT_COUNT 32
#define USE_BOOL_VEC false
#define LATE_SUBSCRIPTIONS_RESOLVE true
#define USE_ASPECTS false

#define COMPONENT_STORE HashedArrayTree
#define CHUNK_ALLOCATOR Pool

#if USE_BOOL_VEC
#define vector_bool std::vector<bool>
#else
#define vector_bool std::vector<char>
#endif