import sys

benchmarks = [
	('fillseq', '100'),
	('fillseq', '1024'),
	('fillrandom', '100'),
	('fillrandom', '1024'),
	('overwrite', '100'),
	('overwrite', '1024'),
	('readseq', '100'),
	('readseq', '1024'),
	('readrandom', '100'),
	('readrandom', '1024'),
	('deleteseq', '100'),
	('deleteseq', '1024'),
	('deleterandom', '100'),
	('deleterandom', '1024'),
]

if __name__ == "__main__":
	if len(sys.argv) != 2:
		print "python %s output_file" % sys.argv[0]
		sys.exit(1)

	outfile_name = sys.argv[1]

	# fillrandom   :      15.140 micros/op 66048 ops/sec;   65.5 MB/s
	# fillrandom   :      15.140 micros/op 66051 ops/sec;   65.5 MB/s
	# fillrandom   :      15.100 micros/op 66225 ops/sec;   65.7 MB/s
	# fillrandom   :      15.047 micros/op 66460 ops/sec;   65.9 MB/s

	files = []
	tputs = []
	for name, val_size in benchmarks:
		file_name = '%s_%s.txt' % (name, val_size)
		files.append(file_name)
		tput = 0
		try:
			with open(file_name) as f:
				lines = f.readlines()[-4:]
				for line in lines:
					words = line.strip().split()
					bench_name = words[0]
					assert bench_name == name
					tput += int(words[4])
		except:
			tput = 0
		tputs.append(tput)
	files.insert(0, 'AverageThroughput')
	print tputs
	mean = int(sum(tputs) / len(tputs))
	print "avg: %d ops/sec" % mean
	tputs.insert(0, mean)
	tputs = map(lambda x: str(x), tputs)
	with open(outfile_name, 'w') as f:
		f.write("%s\n" % ",".join(files))
		f.write("%s\n" % ",".join(tputs))
