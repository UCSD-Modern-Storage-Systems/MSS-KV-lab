import os, sys
import xml.etree.ElementTree as ET

basic_test_spec = [
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
]

recovery_test_spec = [
        'PMKVRecoveryTest.FillSeqRecoveryTest',
        'PMKVRecoveryTest.OverwriteSeqRecoveryTest',
        'PMKVRecoveryTest.DeleteSeqRecoveryTest',
]

test_result_home = '../'


def parse_test_result(xml, test_spec):
    ret = {}
    try:
        tree = ET.parse(xml)
    except:
        for test_name in test_spec:
            ret[test_name] = (False, 'Error: output file couldn\'t be produced')
    else:
        root = tree.getroot()
        testsuites = root
        for testsuite in testsuites:
            for testcase in testsuite:
                test_name = '%s.%s' % (testsuite.attrib['name'], testcase.attrib['name'])
                failure = testcase.find('failure')
                passed = True if failure is None else False
                ret[test_name] = (passed, '' if passed else failure.attrib['message'])

    # write the result out to the file
    for test_name in test_spec:
        result = ret.get(test_name)
        if not result:
            result = (False, 'Error: output file couldn\'t be produced')
        with open(test_result_home + test_name, 'w') as f:
            f.write(result[1])


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print 'python %s xmlfile1 xmlfile2' % sys.argv[0]
        sys.exit(1)

    xmlfile1 = sys.argv[1]
    xmlfile2 = sys.argv[2]

    # parse the input xml file
    parse_test_result(xmlfile1, basic_test_spec)
    parse_test_result(xmlfile2, recovery_test_spec)
