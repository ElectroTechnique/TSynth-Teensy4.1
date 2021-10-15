#include "unity.h"
#include <stdlib.h>
// Including the .cpp file is a hack.
// Platformio wants libraries to go in a 'lib' directory for testing but
// doing that would probably break the Arduino IDE.
// https://community.platformio.org/t/pio-test-not-building-project-source/4501
#include "SettingsService.cpp"
#include <iostream>

#define OPTION_1_VALUES {"All", "Some", "None", "\0"}
const char* option1Values[SETTINGSVALUESNO] = OPTION_1_VALUES;
int option1Index = 0;    
const char* option1Value;
int option1Current() {
    std::cout << "Option 1 current index: " << option1Index << std::endl;
    return option1Index;
}                        
void option1Update(int index, const char* value) {
    option1Index = index;
    option1Value = value;
    std::cout << "updating option 1: " << index << "    " << value << std::endl;
}
settings::SettingsOption option1{"MIDI Ch.", OPTION_1_VALUES, option1Update, option1Current};

#define OPTION_2_VALUES {"All", "Some", "None", "\0"}
const char* option2Values[SETTINGSVALUESNO] = OPTION_2_VALUES;
int option2Index = 0;
const char* option2Value;
int option2Current() {   
    return option2Index;
}
void option2Update(int index, const char* value) {
    option2Index = index;
    option2Value = value;
    std::cout << "updating option 2: " << index << "    " << value << std::endl;
}
settings::SettingsOption option2{"MIDI Ch.", OPTION_2_VALUES, option2Update, option2Current};

void resetOptions(void) {    
    option1Index = 0;
    option2Index = 0;
    settings::reset();
}

/*
const char* current_setting();
const char* previous_setting();
const char* next_setting();

const char* previous_setting_value();
const char* next_setting_value();

const char* current_setting_value();
const char* current_setting_previous_value();
const char* current_setting_next_value();

void increment_setting();
void decrement_setting();

void increment_setting_value();
void decrement_setting_value();

void save_current_value();
*/

void test_one_setting(void) {
    resetOptions();
    settings::append(option1);

    // Defaults to first setting.
    TEST_ASSERT_EQUAL(option1.option, settings::current_setting());
    TEST_ASSERT_EQUAL(option1Values[0], settings::current_setting_value());

    // There's only 1 setting, it is still selected
    settings::increment_setting();
    TEST_ASSERT_EQUAL(option1.option, settings::current_setting());
    TEST_ASSERT_EQUAL(option1Values[0], settings::current_setting_value());
}

void test_two_settings(void) {
    resetOptions();
    settings::append(option1);
    settings::append(option2);

    // Defaults to first setting.
    TEST_ASSERT_EQUAL(option1.option, settings::current_setting());
    TEST_ASSERT_EQUAL(option1Values[0], settings::current_setting_value());

    // Increment to second setting
    settings::increment_setting();
    TEST_ASSERT_EQUAL(option2.option, settings::current_setting());
    TEST_ASSERT_EQUAL(option2Values[0], settings::current_setting_value());

    // Increment back to the first setting
    settings::increment_setting();
    TEST_ASSERT_EQUAL(option1.option, settings::current_setting());
    TEST_ASSERT_EQUAL(option1Values[0], settings::current_setting_value());
}

void test_cycle_option_values(void) {
    resetOptions();
    settings::append(option1);

    // There are 3 values, increment twice to make sure we get them all.
    TEST_ASSERT_EQUAL(option1Values[0], settings::current_setting_value());
    settings::increment_setting_value();
    TEST_ASSERT_EQUAL(option1Values[1], settings::current_setting_value());
    settings::increment_setting_value();
    TEST_ASSERT_EQUAL(option1Values[2], settings::current_setting_value());

    // The last value is a null, make sure increment is a no-op at this point.
    settings::increment_setting_value();
    TEST_ASSERT_EQUAL(option1Values[2], settings::current_setting_value());

    // Decrement back to the first value
    TEST_ASSERT_EQUAL(option1Values[2], settings::current_setting_value());
    settings::decrement_setting_value();
    TEST_ASSERT_EQUAL(option1Values[1], settings::current_setting_value());
    settings::decrement_setting_value();
    TEST_ASSERT_EQUAL(option1Values[0], settings::current_setting_value());

    // Decrementing from the first value should be a no-op
    settings::decrement_setting_value();
    TEST_ASSERT_EQUAL(option1Values[0], settings::current_setting_value());
}

void test_save(void) {
    resetOptions();
    settings::append(option2);
    settings::increment_setting_value();
    TEST_ASSERT_EQUAL(option2Index, 0);
    settings::save_current_value();
    TEST_ASSERT_EQUAL(option2Index, 1);
}

void test_initialize(void) {
    resetOptions();
    option1Index = 1;
    option2Index = 2;
    settings::append(option1);
    settings::append(option2);

    // Defaults to first setting, initialized to index 1
    TEST_ASSERT_EQUAL(option1.option, settings::current_setting());
    TEST_ASSERT_EQUAL(option1Values[1], settings::current_setting_value());

    // second setting initialized to index 2
    settings::increment_setting();
    TEST_ASSERT_EQUAL(option2.option, settings::current_setting());
    TEST_ASSERT_EQUAL(option2Values[2], settings::current_setting_value());
}

void settingservice_tests() {
    UNITY_BEGIN();
    RUN_TEST(test_one_setting);
    RUN_TEST(test_two_settings);
    RUN_TEST(test_cycle_option_values);
    RUN_TEST(test_save);
    RUN_TEST(test_initialize);
    UNITY_END();
}

