/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "unity.h"

#include "ocs_system/guard_arena.h"
#include "ocs_system/heap_arena.h"

namespace ocs {
namespace system {

namespace {

struct TestObject : private core::NonCopyable<> {
public:
    TestObject(size_t& ctor_call_count, size_t& dtor_call_count)
        : dtor_call_count_(dtor_call_count) {
        ++ctor_call_count;
    }

    ~TestObject() {
        ++dtor_call_count_;
    }

private:
    size_t& dtor_call_count_;
};

} // namespace

TEST_CASE("Heap arena: allocate/deallocate", "[ocs_system], [heap_caps_arena]") {
    HeapArena arena;

    void* ptr = arena.allocate(123);
    TEST_ASSERT_NOT_NULL(ptr);

    arena.deallocate(ptr);
}

TEST_CASE("Heap arena: placemenet new/delete: primitives",
          "[ocs_system], [heap_caps_arena]") {
    HeapArena heap_arena;
    GuardArena guard_arena(heap_arena);

    int* ptr = new (guard_arena) int();
    TEST_ASSERT_NOT_NULL(ptr);
    TEST_ASSERT_EQUAL(1, guard_arena.get_num_allocations());

    int* arr = new (guard_arena) int[123];
    TEST_ASSERT_NOT_NULL(arr);
    TEST_ASSERT_EQUAL(2, guard_arena.get_num_allocations());

    guard_arena.deallocate(ptr);
    TEST_ASSERT_EQUAL(1, guard_arena.get_num_allocations());

    guard_arena.deallocate(arr);
    TEST_ASSERT_EQUAL(0, guard_arena.get_num_allocations());
}

TEST_CASE("Heap arena: placemenet new/delete: user-defined objects",
          "[ocs_system], [heap_caps_arena]") {
    HeapArena heap_arena;
    GuardArena guard_arena(heap_arena);

    size_t ctor_call_count = 0;
    size_t dtor_call_count = 0;

    TestObject* ptr = new (guard_arena) TestObject(ctor_call_count, dtor_call_count);
    TEST_ASSERT_NOT_NULL(ptr);
    TEST_ASSERT_EQUAL(1, ctor_call_count);
    TEST_ASSERT_EQUAL(0, dtor_call_count);
    TEST_ASSERT_EQUAL(1, guard_arena.get_num_allocations());

    guard_arena.destroy(*ptr);

    TEST_ASSERT_EQUAL(1, ctor_call_count);
    TEST_ASSERT_EQUAL(1, dtor_call_count);
    TEST_ASSERT_EQUAL(0, guard_arena.get_num_allocations());
}

TEST_CASE("Heap arena: smart pointers: non-array user-defined objects",
          "[ocs_system], [heap_caps_arena]") {
    HeapArena heap_arena;
    GuardArena guard_arena(heap_arena);

    size_t ctor_call_count = 0;
    size_t dtor_call_count = 0;

    { // Check unique.
        auto ptr =
            make_unique_ptr<TestObject>(guard_arena, ctor_call_count, dtor_call_count);
        TEST_ASSERT_NOT_NULL(ptr);

        TEST_ASSERT_EQUAL(1, ctor_call_count);
        TEST_ASSERT_EQUAL(0, dtor_call_count);
        TEST_ASSERT_EQUAL(1, guard_arena.get_num_allocations());
    }

    { // Check shared.
        auto ptr =
            make_shared_ptr<TestObject>(guard_arena, ctor_call_count, dtor_call_count);
        TEST_ASSERT_NOT_NULL(ptr);

        TEST_ASSERT_EQUAL(2, ctor_call_count);
        TEST_ASSERT_EQUAL(1, dtor_call_count);
        TEST_ASSERT_EQUAL(1, guard_arena.get_num_allocations());
    }

    TEST_ASSERT_EQUAL(2, ctor_call_count);
    TEST_ASSERT_EQUAL(2, dtor_call_count);
    TEST_ASSERT_EQUAL(0, guard_arena.get_num_allocations());
}

TEST_CASE("Heap arena: smart pointers: non-array primitive types",
          "[ocs_system], [heap_caps_arena]") {
    HeapArena heap_arena;
    GuardArena guard_arena(heap_arena);

    TEST_ASSERT_NOT_NULL(make_unique_ptr<int>(guard_arena));
    TEST_ASSERT_NOT_NULL(make_unique_ptr<float>(guard_arena));
    TEST_ASSERT_NOT_NULL(make_unique_ptr<char>(guard_arena));

    TEST_ASSERT_NOT_NULL(make_shared_ptr<int>(guard_arena));
    TEST_ASSERT_NOT_NULL(make_shared_ptr<float>(guard_arena));
    TEST_ASSERT_NOT_NULL(make_shared_ptr<char>(guard_arena));

    TEST_ASSERT_EQUAL(0, guard_arena.get_num_allocations());
}

TEST_CASE("Heap arena: smart pointers: array of primitive types",
          "[ocs_system], [heap_caps_arena]") {
    HeapArena heap_arena;
    GuardArena guard_arena(heap_arena);

    TEST_ASSERT_NOT_NULL(make_unique_ptr<int[]>(guard_arena, 12));
    TEST_ASSERT_NOT_NULL(make_unique_ptr<float[]>(guard_arena, 21));
    TEST_ASSERT_NOT_NULL(make_unique_ptr<char[]>(guard_arena, 42));

    TEST_ASSERT_NOT_NULL(make_shared_ptr<int[]>(guard_arena, 12));
    TEST_ASSERT_NOT_NULL(make_shared_ptr<float[]>(guard_arena, 21));
    TEST_ASSERT_NOT_NULL(make_shared_ptr<char[]>(guard_arena, 42));

    TEST_ASSERT_EQUAL(0, guard_arena.get_num_allocations());
}

} // namespace system
} // namespace ocs
