import os, sys
import xml.etree.ElementTree as ET

spec = [
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

test_result_home = '../'


def parse_test_result(xml_file):
    ret = {}
    try:
        tree = ET.parse(xml_file)
    except:
        print '%s not found. skip.' % xml_file
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
    for test_name, result in ret.iteritems():
        with open(test_result_home + test_name, 'w') as f:
            f.write(result[1])

def generate_failed_result(xml_dir, spec):
    for test_name in spec:
        if not os.path.exists(xml_dir + '/' + test_name + '.xml'):
            result = (False, 'Error: output file couldn\'t be produced')
            with open(test_result_home + test_name, 'w') as f:
                f.write(result[1])


if __name__ == '__main__':
    if len(sys.argv) != 2:
        print 'python %s xml_dir' % sys.argv[0]
        sys.exit(1)

    xml_dir = sys.argv[1]

    # parse the input xml file
    for xml_file in os.listdir(xml_dir):
        parse_test_result(xml_dir + '/' + xml_file)

    # generate output files for failed test cases
    generate_failed_result(xml_dir, spec)
