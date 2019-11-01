# Students should not edit this file, since changes here will _only_
# affect how your code runs locally.  It will not change how your code
# executes in the cloud.
 
lab_name = "PMKV"
output_files = [
	'PMKVTest.BasicTest',
	'PMKVTest.SimpleTest',
	'PMKVTest.BinaryKeyTest',
	'PMKVTest.BinaryValueTest',
	'PMKVTest.EmptyKeyTest',
	'PMKVTest.EmptyValueTest',
	'PMKVTest.GetClearExternalValueTest',
	'PMKVTest.GetHeadlessTest',
	'PMKVTest.GetMultipleTest',
	'PMKVTest.GetMultiple2Test',
	'PMKVTest.GetNonexistentTest',
	'PMKVTest.PutTest',
	'PMKVTest.PutKeysOfDifferentSizesTest',
	'PMKVTest.PutValuesOfDifferentSizesTest',
	'PMKVTest.RemoveAllTest',
	'PMKVTest.RemoveAndInsertTest',
	'PMKVTest.RemoveExistingTest',
	'PMKVTest.RemoveHeadlessTest',
	'PMKVTest.RemoveNonexistentTest',
	'PMKVTest.SimpleMultithreadedTest',
	'PMKVLargeTest.LargeAscendingTest',
	'PMKVLargeTest.LargeAscendingAfterRecoveryTest',
	'PMKVLargeTest.LargeDescendingTest',
	'PMKVLargeTest.LargeDescendingAfterRecoveryTest',
        'PMKVRecoveryTest.FillSeqRecoveryTest',
        'PMKVRecoveryTest.OverwriteSeqRecoveryTest',
        'PMKVRecoveryTest.DeleteSeqRecoveryTest',
]
input_files = ['src/pmkv.c']
run_cmd = ['make', 'test_all']
clean_cmd = ['make', 'clean']
repo = "https://github.com/UCSD-Modern-Storage-Systems/MSS-KV-lab"
reference_tag = "314bfbd09ab3a28b446742234851eeef2c29dcba"
time_limit = 600

valid_options={
}

default_options={
}

figures_of_merit=[
]
