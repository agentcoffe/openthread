# look for the pattern
#
# |.lowtext        0x00000000000041f4       0x9e
#
# and put the name of the section into the hold buffer
/^\.[a-zA-Z0-9][a-zA-Z0-9]*/ {
	!b end
}

s/^\.\([a-zA-Z0-9][a-zA-Z0-9]*\)/\1/h
b continue

:loop
/^ \.[a-zA-Z0-9][a-zA-Z0-9]*\.[0-9a-zA-Z\_]*$/ {
	s/\./ /g
	N
	s/\n//
	p
	b continue
}

/^ \.[a-zA-Z0-9][a-zA-Z0-9_]*$/ {
	s/\./ /g
	N
	s/\n//
	p
	b continue
}

/^ \.[a-zA-Z0-9][a-zA-Z0-9]*\.[0-9a-zA-Z\_]* *0x[0-9a-f]* *0x[0-9a-f]*/ {
	s/\./ /g
	p
	b continue
}

/^ \.[a-zA-Z0-9][a-zA-Z0-9]* *0x[0-9a-f]* *0x[0-9a-f]*/ {
	s/\.\([a-zA-Z.0-9][a-zA-Z.0-9]*\)/ \1 nosection/
	p
	b continue
}

/^ \*fill\*/ {
	s/\*fill\*/ *fill* @none@/
	p
	b continue
}

:continue
n
/^\.[^\.]/b test
b loop

:test
/^\.[a-zA-Z0-9][a-zA-Z0-9]*/b continue

:end
