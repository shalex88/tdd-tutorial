#include <ostream>
#include "gtest/gtest.h"
/* Add your project include files here */

/* 1 - Simple test */
TEST(sprintf, NoFormatOperations) {
  char output[5] {};

  EXPECT_EQ(3, sprintf(output, "hey"));
  EXPECT_STREQ("hey", output);

  /* Test for memory corruption */
  memset(output, 0xaa, sizeof(output));
  EXPECT_EQ(char(0xaa), output[4]);
}

TEST(sprintf, InsertString) {
    char output[20] {};

    EXPECT_EQ(12, sprintf(output, "Hello %s\n", "World"));
    EXPECT_STREQ("Hello World\n", output);

    /* Test for memory corruption */
    memset(output, 0xaa, sizeof(output));
    EXPECT_EQ(char(0xaa), output[13]);
}