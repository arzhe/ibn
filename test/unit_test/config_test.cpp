#include <gtest/gtest.h>

#include "config/config.h"

namespace {

std::string example = "../../ibn_policy.json";

TEST(TestConfig, TestString) {
    ASSERT_EQ("../../ibn_policy.json", example);
}

TEST(TestConfig, TestNewConfigFromFile) {
    auto cfg = ibn::Config::NewConfigFromFile(example);
    EXPECT_NE(cfg, nullptr);
}

/* TEST(TestConfig, TestGetPath) { */
/*     auto cfg = ibn::Config::NewConfigFromFile(example); */
/*     auto path = cfg->GetPath(); */
/*     ASSERT_EQ(path, example); */
/* } */

}
