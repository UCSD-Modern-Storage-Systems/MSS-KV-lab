#include "gtest/gtest.h"

namespace {
	class PMKVTest : public ::testing::Test {
	protected:
		PMKVTest() {}
		~PMKVTest() override {}
	};

	TEST_F(PMKVTest, TestBasic) {
		
	}

}	// namespace

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
