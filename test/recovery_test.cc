#include "gtest/gtest.h"
#include <thread>
#include <vector>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "libpmemkv.hpp"
extern "C" {
#include "pmkv.h"
}

const size_t SIZE = 1024ull * 1024ull * 1024ull * 2ull;

using namespace pmem::kv;

class PMKVWrapper {
public:
	PMKVWrapper(std::string path, size_t size, bool create)
	{
		_kv = pmkv_open(path.c_str(), size, create ? 1 : 0);
	}

	~PMKVWrapper()
	{
		pmkv_close(_kv);
	}

	status get(string_view key, std::string *value) {
		char val[MAX_VAL_LEN];
		size_t val_size;
		int s = pmkv_get(_kv, key.data(), key.size(), val, &val_size);
		if (s)
			return status::NOT_FOUND;
		value->assign(val, val_size);
		return status::OK;
	}

	status put(string_view key, string_view value) {
		int s = pmkv_put(_kv, key.data(), key.size(), value.data(), value.size());
		if (s)
			throw std::runtime_error("Failed to put with an undefined error");
		return status::OK;
	}

	status remove(string_view key) {
		int s = pmkv_delete(_kv, key.data(), key.size());
		if (s)
			return status::NOT_FOUND;
		return status::OK;
	}

	status count_all(std::size_t &cnt) {
		int s = pmkv_count_all(_kv, &cnt);
		if (s)
			throw std::runtime_error("Failed to count all with an undefined error");
		return status::OK;
	}

	status exists(string_view key) {
		if (!pmkv_exists(_kv, key.data(), key.size()))
			return status::NOT_FOUND;
		return status::OK;
	}

private:
	pmkv* _kv;
};

template <size_t POOL_SIZE, size_t NUM_OP, size_t ITERATION>
class PMKVBaseTest : public ::testing::Test {
public:
	std::string PATH = "/mnt/ramdisk/test";
	PMKVWrapper *kv;
	size_t iteration;
	PMKVBaseTest()
	{
		iteration = ITERATION;
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

	void Cleanup()
	{
		std::remove(PATH.c_str());
		Start(true);
	}

	void FillSeq() {
		for (int i = 1; i < NUM_OP; i++) {
			std::string istr = std::to_string(i);
			ASSERT_TRUE(kv->put(istr, (istr + "!")) == status::OK);
		}
	}

	void DeleteSeq() {
		for (int i = 1; i < NUM_OP; i++) {
			std::string istr = std::to_string(i);
			ASSERT_TRUE(kv->remove(istr) == status::OK);
		}
	}

	void SanityCheck() {
		size_t i, cnt = 0;
		std::size_t cnt_all = std::numeric_limits<std::size_t>::max();

		ASSERT_TRUE(kv->count_all(cnt_all) == status::OK);
		for (i = 1; i < NUM_OP; i++) {
			std::string istr = std::to_string(i);
			std::string value;
			if (kv->get(istr, &value) == status::OK && value == (istr + "!"))
				cnt++;
		}
		ASSERT_TRUE(cnt_all == cnt);
	}

protected:
	void Start(bool create)
	{
		kv = new PMKVWrapper(PATH, POOL_SIZE, create);
	}
};

void sigsegv_handler(int sig)
{
	printf("Error occurred during recovery!\n");
}

using PMKVRecoveryTest = PMKVBaseTest<SIZE, 1000000, 10>;

TEST_F(PMKVRecoveryTest, FillSeqRecoveryTest) {
	pid_t pid;
	for (size_t i = 1; i <= iteration; i++) {
		// print status
		printf("\r%d/%d testing...", i, iteration);
		fflush(stdout);

		// clean up
		Cleanup();

		pid = fork();
		if (pid == 0) {
			// child : exec benchmark long enough time
			FillSeq();

		} else {
			// register SIGSEGV handler
			ASSERT_TRUE(signal(SIGSEGV, sigsegv_handler) != SIG_ERR);
			// parent : sleep and kill
			sleep(3);
			kill(pid, SIGSEGV);
			// try recovery
			sleep(1);
			Restart();
			SanityCheck();
			// deregister SIGSEGV handler
			ASSERT_TRUE(signal(SIGSEGV, SIG_DFL) != SIG_ERR);
		}
	}
	printf("passed!\n");
}

TEST_F(PMKVRecoveryTest, OverwriteSeqRecoveryTest) {
	pid_t pid;
	for (size_t i = 1; i <= iteration; i++) {
		// print status
		printf("\r%d/%d testing...", i, iteration);
		fflush(stdout);

		// clean up
		Cleanup();
		// fill
		FillSeq();

		pid = fork();
		if (pid == 0) {
			// child : exec benchmark long enough time
			FillSeq();

		} else {
			// register SIGSEGV handler
			ASSERT_TRUE(signal(SIGSEGV, sigsegv_handler) != SIG_ERR);
			// parent : sleep and kill
			sleep(3);
			kill(pid, SIGSEGV);
			// try recovery
			sleep(1);
			Restart();
			SanityCheck();
			// deregister SIGSEGV handler
			ASSERT_TRUE(signal(SIGSEGV, SIG_DFL) != SIG_ERR);
		}
	}
	printf("passed!\n");
}

TEST_F(PMKVRecoveryTest, DeleteSeqRecoveryTest) {
	pid_t pid;
	for (size_t i = 1; i <= iteration; i++) {
		// print status
		printf("\r%d/%d testing...", i, iteration);
		fflush(stdout);

		// clean up
		Cleanup();
		// fill
		FillSeq();

		pid = fork();
		if (pid == 0) {
			// child : exec benchmark long enough time
			DeleteSeq();

		} else {
			// register SIGSEGV handler
			ASSERT_TRUE(signal(SIGSEGV, sigsegv_handler) != SIG_ERR);
			// parent : sleep and kill
			sleep(5);
			kill(pid, SIGSEGV);
			// try recovery
			sleep(1);
			Restart();
			SanityCheck();
			// deregister SIGSEGV handler
			ASSERT_TRUE(signal(SIGSEGV, SIG_DFL) != SIG_ERR);
		}
	}
	printf("passed!\n");
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
