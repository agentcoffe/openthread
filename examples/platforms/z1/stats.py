import getopt
import sys
import subprocess
import re


class section():
	s_name = ''
	start = 0
	size = 0

	def __init__(self, s_name, start, size):
		self.s_name = s_name
		self.start = int(start[2:], 16)
		self.size = int(size[2:], 16)
		self.childs = []
	
	def __repr__(self):
		return self.s_name+' @ '+hex(self.start)+' + '+hex(self.size)+'\n'

class subsection():
	f_name = ''
	s_name = ''
	start = 0
	size = 0

	def __init__(self, f_name, s_name, start, size):
		self.f_name = f_name
		self.s_name = s_name
		self.start = int(start[2:], 16)
		self.size = int(size[2:], 16)

	def __repr__(self):
		return self.s_name+'.'+self.f_name+' '+hex(self.start)+' '+hex(self.size)+'\n'

class opts():
	file = ''
	sort = 0
	print = 0
	table = 0


def parse_subsections(f, s):
	# .data.mbedtls_free
	#   0x0000000000001108        0x4 ../../../third_party/mbedtls/libmbedcrypto.a(libmbedcrypto_a-platform.o)
	sect_1 = re.compile('^ \.*([a-zA-Z0-9][a-zA-Z0-9]*)\.([0-9a-zA-Z\_\.]*)$')
	sect_1_2 = re.compile('^ *(0x[0-9a-f]*) *(0x[0-9a-f]*)')

	# .crt_0100init_bss
	#   0x00000000000041f4       0x10 /usr/msp430-elf/lib/large/libcrt.a(crt_bss.o)
	sect_2 = re.compile('^ \.*([a-zA-Z0-9][a-zA-Z0-9\_\.]*)$')
	sect_2_2 = re.compile('^ *(0x[0-9a-f]*) *(0x[0-9a-f]*)')

	# .bss.rxBuffer  0x0000000000002a04       0x84 ../../../examples/platfo
	sect_3 = re.compile('^ \.*([a-zA-Z0-9][a-zA-Z0-9]*)\.([a-zA-Z0-9\_][a-zA-Z0-9\_]*) *(0x[0-9a-f]*) *(0x[0-9a-f]*)')

	# .data          0x0000000000001104        0x3 ../../../src/core/libopenthread-mtd.a(libopenthread_mtd_a-mle.o)
	sect_4 = re.compile('^ \.*([a-zA-Z0-9][a-zA-Z0-9\_]*) *(0x[0-9a-f]*) *(0x[0-9a-f]*)')

	# *fill*         0x0000000000001107        0x1 
	sect_5 = re.compile('^ \*fill\* *(0x[0-9a-f]*) *(0x[0-9a-f]*)')

	sect_end = re.compile('^$')

	for l in f:
		m = sect_1.match(l)
		if m:
			s_name = m.group(1)
			f_name = m.group(2)
			l = f.readline()
			m = sect_1_2.match(l)
			s.childs.append(subsection(f_name, s_name, m.group(1), m.group(2)))
			continue

		m = sect_2.match(l)
		if m:
			f_name = m.group(1)
			l = f.readline()
			m = sect_1_2.match(l)
			s.childs.append(subsection(f_name, s.s_name, m.group(1), m.group(2)))
			continue

		m = sect_3.match(l)
		if m:
			s.childs.append(subsection(m.group(2), m.group(1), m.group(3), m.group(4)))
			continue

		m = sect_4.match(l)
		if m:
			s.childs.append(subsection('@nofunction', m.group(1), m.group(2), m.group(3)))
			continue

		m = sect_5.match(l)
		if m:
			s.childs.append(subsection('@fill', s.s_name, m.group(1), m.group(2)))
			continue

		m = sect_end.match(l)
		if m:
			break

	c_filt = subprocess.Popen(['c++filt'], stdin = subprocess.PIPE, stdout = subprocess.PIPE)

	total_mem = 0
	for c in s.childs:
		#print(hex(c.size))
		c_filt.stdin.write(bytes(c.f_name+'\n', 'utf-8'))
		c_filt.stdin.flush()
		#c_filt.stdout.flush()
		c.f_name = c_filt.stdout.readline().decode('utf-8')[:-1]
		total_mem += c.size

	if total_mem != s.size:
		print('{} size does not match with calculated size'.format(s.s_name))

	return s

def parse(link_map):
	map_list = []

	sect_start = re.compile('^\.([a-zA-Z0-9][a-zA-Z0-9]*) *(0x[0-9a-f]*) *(0x[0-9a-f]*)')

	with open(link_map) as f:
		for l in f:
			m =	sect_start.match(l)
			if m:
				s = section(m.group(1), m.group(2), m.group(3))
				map_list.append(s)
				parse_subsections(f, s)
	return map_list

def panic(msg):
	print('Err: ', msg)
	sys.exit(1)

def usage():
	print('Usage: python stats.py -f file.map [-s section] [-p] [-o] [-t]')
	print('  -p  to print')
	print('  -o  to order by size')
	print('  -t  to print a table of output sections')

def main():
	o = opts()

	o.sect = 'all'

	try:
		opt_list, arg_list = getopt.getopt(sys.argv[1:], "f:s:pot")
	except getopt.GetoptError as err:
		usage()
		panic(str(err))

	for p, v in opt_list:
		if p == '-f':
			o.file = v
		elif p == '-s':
			o.sect = v
		elif p == '-p':
			o.print = 1
		elif p == '-o':
			o.sort = 1
		elif p == '-t':
			o.table = 1
		else:
			usage()
			panic('option not recognised: '+p)
	
	map_list = parse(o.file)

	if o.print:
		for sec in map_list:
			if o.sect == sec.s_name:
				if o.sort:
					sec.childs = sorted(sec.childs, key = lambda x: x.size)

				print('\nSection list:')
				for sub_s in sec.childs:
					print('{:<7} {:<10} {}'.format(hex(sub_s.size), sub_s.s_name, sub_s.f_name))
				print('{} {}'.format(sec.s_name, hex(sec.size)))
	
	if o.table:
		print('\nSection table:')
		if o.sort:
			map_list = sorted(map_list, key = lambda x: x.size)
		for sec in map_list:
			print('{:<7} {:<10}'.format(hex(sec.size), sec.s_name))

if __name__ == '__main__':
	main()
