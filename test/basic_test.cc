#include "gtest/gtest.h"
extern "C" {
#include "pmkv.h"
}

namespace {
	class PMKVTest : public ::testing::Test {
	protected:
		PMKVTest() {
			kv = pmkv_open("/mnt/ramdisk/test", 16*1024*1024, 1);
		}
		~PMKVTest() override {
			pmkv_close(kv);
		}

	protected:
		pmkv* kv;
	};

	TEST_F(PMKVTest, TestBasic) {
		EXPECT_NE(kv, nullptr);
		pmkv_put(kv, "key1", "value1");
		EXPECT_STREQ((char*)pmkv_get(kv, "key1"), "value1");
	}

}	// namespace

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
