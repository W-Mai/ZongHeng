//
// Created by W-Mai on 2022/10/30.
//

#ifndef EXAMPLE_TEST_UTILS_H
#define EXAMPLE_TEST_UTILS_H

#define __ASSERT_HEADER(x, y) \
    if ((x) != (y)) { printf("%s, %s, %d, "
#define __ASSERT_TAIL(x, y)                            \
    __func__, __ASSERT_FILE_NAME, __LINE__, (x), (y)); \
    return -1;                                         \
    }

#define ASSERT_I(x, y)    \
    __ASSERT_HEADER(x, y) \
    "L: %d, R: %d",       \
        __ASSERT_TAIL(x, y)

#define ASSERT_F(x, y)    \
    __ASSERT_HEADER(x, y) \
    "L: %f, R: %f",       \
        __ASSERT_TAIL(x, y)

#define ASSERT_S(x, y)    \
    __ASSERT_HEADER(x, y) \
    "L: %s, R: %s",       \
        __ASSERT_TAIL((x).c_str(), (y).c_str())

#endif // EXAMPLE_TEST_UTILS_H
