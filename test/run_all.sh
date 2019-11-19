#!/bin/bash

BASIC_TEST="PMKVTest.BasicTest
	PMKVTest.SimpleTest
	PMKVTest.BinaryKeyTest
	PMKVTest.BinaryValueTest
	PMKVTest.EmptyKeyTest
	PMKVTest.EmptyValueTest
	PMKVTest.GetClearExternalValueTest
	PMKVTest.GetHeadlessTest
	PMKVTest.GetMultipleTest
	PMKVTest.GetMultiple2Test
	PMKVTest.GetNonexistentTest
	PMKVTest.PutTest
	PMKVTest.PutKeysOfDifferentSizesTest
	PMKVTest.PutValuesOfDifferentSizesTest
	PMKVTest.RemoveAllTest
	PMKVTest.RemoveAndInsertTest
	PMKVTest.RemoveExistingTest
	PMKVTest.RemoveHeadlessTest
	PMKVTest.RemoveNonexistentTest
	PMKVTest.SimpleMultithreadedTest
	PMKVLargeTest.LargeAscendingTest
	PMKVLargeTest.LargeAscendingAfterRecoveryTest
	PMKVLargeTest.LargeDescendingTest
	PMKVLargeTest.LargeDescendingAfterRecoveryTest"

RECOVERY_TEST="PMKVRecoveryTest.FillSeqRecoveryTest
        PMKVRecoveryTest.OverwriteSeqRecoveryTest
        PMKVRecoveryTest.DeleteSeqRecoveryTest"

# basic_test
for TEST in $BASIC_TEST; do
	echo "$TEST"
	PMEM_IS_PMEM_FORCE=1 ./bin/basic_test --gtest_filter=$TEST --gtest_output=xml:xml/$TEST.xml
done

# recovery_test
for TEST in $RECOVERY_TEST; do
	echo "$TEST"
	PMEM_IS_PMEM_FORCE=1 ./bin/recovery_test --gtest_filter=$TEST --gtest_output=xml:xml/$TEST.xml
done
