#include <ostream>
#include "gtest/gtest.h"
/* Add your project include files here */

/* 2 - Test fixture for multiple tests */
class sprintf_test : public testing::Test {
public:
    char output[100];
    const char *expected;
    sprintf_test() {
        memset(output, 0xaa, sizeof(output));
        expected = "";
    };
    ~sprintf_test() override = default;
    void expect(const char *s) {
        expected = s;
    }
    void given(int charsWritten) {
        EXPECT_EQ(strlen(expected), charsWritten);
        EXPECT_STREQ(expected, output);
        EXPECT_EQ(char(0xaa), output[strlen(expected) + 1]);
    }
};

TEST_F(sprintf_test, NoFormatOperations) {
    expect("hey");
    given(sprintf(output, "hey"));
}

TEST_F(sprintf_test, InsertString) {
    expect("Hello World\n");
    given(sprintf(output, "%s\n", "Hello World"));
}