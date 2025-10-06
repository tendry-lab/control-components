#include "unity.h"

#include "ocs_control/reset_fsm.h"
#include "ocs_core/noncopyable.h"

namespace ocs {
namespace control {

namespace {

struct TestEventHandler : public scheduler::IEventHandler, private core::NonCopyable<> {
    unsigned call_count { 0 };
    status::StatusCode code { status::StatusCode::OK };

    status::StatusCode handle_event() override {
        ++call_count;

        return code;
    }
};

} // namespace

TEST_CASE("Reset FSM: button is released within interval", "[reset_fsm], [ocs_control]") {
    TestEventHandler handler;

    const core::Time want_release_interval = core::Duration::second;
    const core::Time got_release_interval = want_release_interval / 2;

    ResetFsm fsm(handler, want_release_interval);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.handle_pressed(got_release_interval));
    taskYIELD();

    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());
    TEST_ASSERT_EQUAL(1, handler.call_count);
}

TEST_CASE("Reset FSM: button is released within interval: handler failed",
          "[reset_fsm], [ocs_control]") {
    TestEventHandler handler;
    handler.code = status::StatusCode::Timeout;

    const core::Time want_release_interval = core::Duration::second;
    const core::Time got_release_interval = want_release_interval / 2;

    ResetFsm fsm(handler, want_release_interval);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.handle_pressed(got_release_interval));
    taskYIELD();

    TEST_ASSERT_EQUAL(handler.code, fsm.run());
    TEST_ASSERT_EQUAL(1, handler.call_count);
}

TEST_CASE("Reset FSM: button isn't released within interval",
          "[reset_fsm], [ocs_control]") {
    TestEventHandler handler;

    const core::Time want_release_interval = core::Duration::second;
    const core::Time got_release_interval = want_release_interval * 2;

    ResetFsm fsm(handler, want_release_interval);

    TEST_ASSERT_EQUAL(status::StatusCode::Error,
                      fsm.handle_pressed(got_release_interval));
    taskYIELD();

    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());
    TEST_ASSERT_EQUAL(0, handler.call_count);
}

} // namespace control
} // namespace ocs
