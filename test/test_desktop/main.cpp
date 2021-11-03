#ifdef UNIT_TEST

#include "../test_desktop_files/test_notestack.cpp"
#include "../test_desktop_files/test_settings_service.cpp"

int main(int argc, char **argv) {
    notestack_tests();
    settingservice_tests();

    return 0;
}

#endif

