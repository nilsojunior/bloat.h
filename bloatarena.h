#ifndef BLOATARENA_H
#define BLOATARENA_H

#if defined(BLOATARENA_IMPLEMENTATION) && defined(BLOATARENA_STATIC_INLINE)
    #error "Please specify only one between `BLOATARENA_IMPLEMENTATION` and `BLOATARENA_STATIC_INLINE`"
#endif

#if defined(BLOATARENA_STATIC_INLINE)
    #define BLOATADEF static inline
#elif defined(BLOATARENA_IMPLEMENTATION)
    #define BLOATADEF
#else
    #define BLOATADEF extern
#endif

#include <stdint.h>
#include <stddef.h>

#define align_pow2(x, y) (((x) + (y) - 1) & (~((y) - 1)))

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#define KB(x) (((uint64_t)(x)) << 10)
#define MB(x) (((uint64_t)(x)) << 20)
#define GB(x) (((uint64_t)(x)) << 30)

typedef struct {
    size_t pos;
    size_t commit_size;
    size_t size;
    size_t capacity;
} Arena;

typedef struct {
    Arena *arena;
    size_t pos;
} ArenaTemp;

BLOATADEF const char* arena_failure_reason(void);

BLOATADEF Arena* init_arena(size_t capacity, size_t commit_size);
BLOATADEF void   end_arena(Arena *arena);
BLOATADEF void*  arena_push(Arena *arena, size_t size, size_t align, bool zero);
BLOATADEF void   arena_pop(Arena *arena, size_t size);
BLOATADEF void   arena_pop_to(Arena *arena, size_t pos);
BLOATADEF void   arena_reset(Arena *arena);

BLOATADEF ArenaTemp init_arena_temp(Arena *arena);
BLOATADEF void      end_arena_temp(ArenaTemp temp);

#endif // BLOATARENA_H

#if defined(BLOATARENA_STATIC_INLINE) || defined(BLOATARENA_IMPLEMENTATION)

static const char *arena__failure_reason = NULL;
static char arena__failure_buffer[1024];

BLOATADEF const char* arena_failure_reason(void)
{
    return arena__failure_reason;
}

static void arena__err(const char *s)
{
    arena__failure_reason = s;
}

static void arena__errno(const char *s)
{
    snprintf(arena__failure_buffer, sizeof(arena__failure_buffer), "%s: %s", s, strerror(errno));
    arena__failure_reason = arena__failure_buffer;
}

#define arena_err(s, result) do { arena__err((s)); return (result); } while(0)
#define arena_errno(s, result) do { arena__errno((s)); return (result); } while(0)

#include <sys/mman.h>
#include <unistd.h>

static void* os_reserve(size_t size)
{
    void *map = mmap(NULL, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (map == MAP_FAILED) arena_errno("mmap failed", NULL);
    return map;
}

static bool os_commit(void *ptr, size_t size)
{
    if (mprotect(ptr, size, PROT_READ | PROT_WRITE) != 0) arena_errno("mprotect failed", false);
    return true;
}

static bool os_decommit(void *ptr, size_t size)
{
    if (mprotect(ptr, size, PROT_NONE) != 0) arena_errno("mprotect failed", false);
    if (madvise(ptr, size, MADV_DONTNEED) != 0) arena_errno("madvise failed", false);
    return true;
}

static bool os_release(void *ptr, size_t size)
{
    if (munmap(ptr, size) == 0) arena_errno("munmap failed", false);
    return true;
}

static size_t os_pagesize(void)
{
    return (size_t)getpagesize();
}

BLOATADEF Arena* init_arena(size_t capacity, size_t commit_size)
{
    size_t pagesize = os_pagesize();

    capacity    = align_pow2(capacity, pagesize);
    commit_size = align_pow2(commit_size, pagesize);

    Arena *arena = os_reserve(capacity);
    if (arena == NULL) return NULL;

    if (!os_commit(arena, commit_size)) return NULL;

    arena->capacity    = capacity;
    arena->size        = commit_size;
    arena->commit_size = commit_size;
    arena->pos         = sizeof(Arena);

    return arena;
}

BLOATADEF void end_arena(Arena *arena)
{
    os_release(arena, arena->capacity);
}

BLOATADEF void* arena_push(Arena *arena, size_t size, size_t align, bool zero)
{
    size_t aligned_pos = align_pow2(arena->pos, align);
    size_t new_pos     = aligned_pos + size;

    if (new_pos > arena->capacity) arena_err("Arena capacity exceeded", NULL);

    if (new_pos > arena->size) {
        // size_t new_size = new_pos;
        // new_size += arena->commit_size - 1;
        // new_size -= new_size % arena->commit_size;

        size_t new_size = align_pow2(new_pos + align, arena->commit_size);

        new_size = MIN(new_size, arena->capacity);

        uint8_t *commit_ptr  = (uint8_t*)arena + arena->size;
        size_t   commit_size = new_size - arena->size;

        if (!os_commit(commit_ptr, commit_size)) return NULL;

        arena->size = new_size;
    }

    arena->pos = new_pos;

    uint8_t *out = (uint8_t*)arena + aligned_pos;

    if (zero) memset(out, 0, size);

    return out;
}

BLOATADEF void arena_pop(Arena *arena, size_t size)
{
    arena->pos -= MIN(size, arena->pos - sizeof(Arena));
}

BLOATADEF void arena_pop_to(Arena *arena, size_t pos)
{
    size_t size = pos < arena->pos ? arena->pos - pos : 0;
    arena_pop(arena, size);
}

BLOATADEF void arena_reset(Arena *arena)
{
    arena_pop_to(arena, sizeof(Arena));
}

BLOATADEF ArenaTemp init_arena_temp(Arena *arena)
{
    return (ArenaTemp) {
        .arena = arena,
        .pos   = arena->pos,
    };
}

BLOATADEF void end_arena_temp(ArenaTemp temp)
{
    arena_pop_to(temp.arena, temp.pos);
}

#undef arena_err
#undef arena_errno

#endif // BLOATARENA_STATIC_INLINE || BLOATARENA_IMPLEMENTATION
