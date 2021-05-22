#include "unity.h"
#include <stdlib.h>
// Including the .cpp file is a hack.
// Platformio wants libraries to go in a 'lib' directory for testing but
// doing that would probably break the Arduino IDE.
// https://community.platformio.org/t/pio-test-not-building-project-source/4501
#include "MonoNoteHistory.cpp"

#ifdef UNIT_TEST

MonoNoteHistory* s;

void setUp(void) {
    s = new MonoNoteHistory();
    s->push(1,2);
    s->push(3,4);
    s->push(5,6);
}

void tearDown(void) {
    delete s;
}

void test_function_push(void) {
    TEST_ASSERT_EQUAL(3, s->size());
}

void test_function_get_last(void) {
    auto elem = s->getLast();
    TEST_ASSERT_EQUAL(5, elem.note);
    TEST_ASSERT_EQUAL(6, elem.velocity);
}

void test_function_erase_end(void) {
    s->erase(5);
    TEST_ASSERT_EQUAL(2, s->size());
    // Pop should return the next one.
    auto elem = s->getLast();
    TEST_ASSERT_EQUAL(3, elem.note);
    TEST_ASSERT_EQUAL(4, elem.velocity);
}

void test_function_erase_middle(void) {
    s->erase(3);

    // Verify that the last element is still there.
    auto elem = s->getLast();
    TEST_ASSERT_EQUAL(5, elem.note);
    TEST_ASSERT_EQUAL(6, elem.velocity);

    // The next get s hould get the first element.
    s->erase(elem.note);
    elem = s->getLast();
    TEST_ASSERT_EQUAL(1, elem.note);
    TEST_ASSERT_EQUAL(2, elem.velocity);
}

void test_function_get_first(void) {
    auto elem = s->getFirst();
    TEST_ASSERT_EQUAL(1, elem.note);
    TEST_ASSERT_EQUAL(2, elem.velocity);
}

void test_function_get_highest_1(void) {
    auto elem = s->getHighest();
    TEST_ASSERT_EQUAL(5, elem.note);
    TEST_ASSERT_EQUAL(6, elem.velocity);
}

void test_function_get_highest_2(void) {
    MonoNoteHistory s2;
    s2.push(5, 6);
    s2.push(3, 4);
    s2.push(1, 2);

    auto elem = s2.getHighest();
    TEST_ASSERT_EQUAL(5, elem.note);
    TEST_ASSERT_EQUAL(6, elem.velocity);
}

void test_function_get_lowest_1(void) {
    auto elem = s->getLowest();
    TEST_ASSERT_EQUAL(1, elem.note);
    TEST_ASSERT_EQUAL(2, elem.velocity);
}

void test_function_get_lowest_2(void) {
    MonoNoteHistory s2;
    s2.push(5, 6);
    s2.push(3, 4);
    s2.push(1, 2);

    auto elem = s2.getLowest();
    TEST_ASSERT_EQUAL(1, elem.note);
    TEST_ASSERT_EQUAL(2, elem.velocity);
}


int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_function_push);
    RUN_TEST(test_function_erase_end);
    RUN_TEST(test_function_erase_middle);
    RUN_TEST(test_function_get_last);
    RUN_TEST(test_function_get_first);
    RUN_TEST(test_function_get_highest_1);
    RUN_TEST(test_function_get_highest_2);
    RUN_TEST(test_function_get_lowest_1);
    RUN_TEST(test_function_get_lowest_2);
    UNITY_END();

    return 0;
}

#endif
