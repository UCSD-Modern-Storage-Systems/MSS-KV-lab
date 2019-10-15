#include "gtest/gtest.h"
#include "libpmemkv.hpp"
extern "C" {
#include "pmkv.h"
}

const size_t SIZE = 1024ull * 1024ull * 512ull;
const size_t LARGE_SIZE = 1024ull * 1024ull * 1024ull * 2ull;

using namespace pmem::kv;

// wrapper class to use most of pmemkv testcases
class PMKVWrapper {
public:
	PMKVWrapper(std::string path, size_t size, bool create)
	{
		_kv = pmkv_open(path.c_str(), size, create ? 1 : 0);
		if (_kv == NULL)
			throw std::runtime_error("Failed to open kv file");
	}

	~PMKVWrapper()
	{
		pmkv_close(_kv);
	}

	status get(string_view key, std::string *value) {
		void *v = pmkv_get(_kv, key.data());
		if (!v)
			return status::NOT_FOUND;
		*value = std::string((char*)v);
		return status::OK;
	}

	status put(string_view key, string_view value) {
		pmkv_put(_kv, key.data(), value.data());
		return status::OK;
	}

	status remove(string_view key) {
		pmkv_del(_kv, key.data());
		return status::OK;
	}

	status count_all(std::size_t &cnt) {
		pmkv_count_all(_kv, &cnt);
		return status::OK;
	}

	status exists(string_view key) {
		if (pmkv_exists(_kv, key.data()))
			return status::OK;
		return status::NOT_FOUND;
	}

private:
	pmkv* _kv;
};

template <size_t POOL_SIZE>
class PMKVBaseTest : public ::testing::Test {
public:
	std::string PATH = "/mnt/ramdisk/test";
	PMKVWrapper *kv;
	PMKVBaseTest()
	{
		std::remove(PATH.c_str());
		Start(true);
	}

	~PMKVBaseTest()
	{
		delete kv;
	}

	void Restart()
	{
		delete kv;
		Start(false);
	}

protected:
	void Start(bool create)
	{
		kv = new PMKVWrapper(PATH, POOL_SIZE, create);
	}
};

using PMKVTest = PMKVBaseTest<SIZE>;
using PMKVLargeTest = PMKVBaseTest<LARGE_SIZE>;

TEST_F(PMKVTest, BasicTest) {
	EXPECT_NE(kv, nullptr);
	kv->put("key1", "value1");
	std::string value;
	ASSERT_TRUE(kv->get("key1", &value) == status::OK && value == "value1");
}

TEST_F(PMKVTest, SimpleTest)
{
	EXPECT_NE(kv, nullptr);
	std::size_t cnt = std::numeric_limits<std::size_t>::max();
	ASSERT_TRUE(kv->count_all(cnt) == status::OK);
	ASSERT_TRUE(cnt == 0);
	ASSERT_TRUE(status::NOT_FOUND == kv->exists("key1"));
	std::string value;
	ASSERT_TRUE(kv->get("key1", &value) == status::NOT_FOUND);
	ASSERT_TRUE(kv->put("key1", "value1") == status::OK) << errormsg();
	cnt = std::numeric_limits<std::size_t>::max();
	ASSERT_TRUE(kv->count_all(cnt) == status::OK);
	ASSERT_TRUE(cnt == 1);
	ASSERT_TRUE(status::OK == kv->exists("key1"));
	ASSERT_TRUE(kv->get("key1", &value) == status::OK && value == "value1");
	//value = "";
	//kv->get("key1", [&](string_view v) { value.append(v.data(), v.size()); });
	//ASSERT_TRUE(value == "value1");
}

TEST_F(PMKVTest, BinaryKeyTest)
{
	std::size_t cnt = std::numeric_limits<std::size_t>::max();
	ASSERT_TRUE(kv->count_all(cnt) == status::OK);
	ASSERT_TRUE(cnt == 0);
	ASSERT_TRUE(status::NOT_FOUND == kv->exists("a"));
	ASSERT_TRUE(kv->put("a", "should_not_change") == status::OK) << errormsg();
	cnt = std::numeric_limits<std::size_t>::max();
	ASSERT_TRUE(kv->count_all(cnt) == status::OK);
	ASSERT_TRUE(cnt == 1);
	ASSERT_TRUE(status::OK == kv->exists("a"));
	std::string key1 = std::string("a\0b", 3);
	ASSERT_TRUE(status::NOT_FOUND == kv->exists(key1));
	ASSERT_TRUE(kv->put(key1, "stuff") == status::OK) << errormsg();
	cnt = std::numeric_limits<std::size_t>::max();
	ASSERT_TRUE(kv->count_all(cnt) == status::OK);
	ASSERT_TRUE(cnt == 2);
	ASSERT_TRUE(status::OK == kv->exists("a"));
	ASSERT_TRUE(status::OK == kv->exists(key1));
	std::string value;
	ASSERT_TRUE(kv->get(key1, &value) == status::OK);
	ASSERT_EQ(value, "stuff");
	std::string value2;
	ASSERT_TRUE(kv->get("a", &value2) == status::OK);
	ASSERT_EQ(value2, "should_not_change");
	ASSERT_TRUE(kv->remove(key1) == status::OK);
	cnt = std::numeric_limits<std::size_t>::max();
	ASSERT_TRUE(kv->count_all(cnt) == status::OK);
	ASSERT_TRUE(cnt == 1);
	ASSERT_TRUE(status::OK == kv->exists("a"));
	ASSERT_TRUE(status::NOT_FOUND == kv->exists(key1));
	std::string value3;
	ASSERT_TRUE(kv->get(key1, &value3) == status::NOT_FOUND);
	ASSERT_TRUE(kv->get("a", &value3) == status::OK && value3 == "should_not_change");
}


int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
