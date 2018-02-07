BEGIN {
	last_section = "";
}

{
	tmp = strtonum($4);
	name = $2;
	section = $1;
	printf("%s, %s\n", $2, name);

	if (section == "*fill*") {
		section = last_section;
	}

	if (section in sum) {
		sum[section] += tmp;
	} else {
		sum[section] = tmp;
	}

	last_section = section;

	printf(" %d\t%s\t\t%s\n", tmp, $4, name) >> "stats.sections.tmp";
}

END {
	for (x in sum) {
		printf(" 0x%x(%d)\t\t%s\n", sum[x], sum[x], x);
	}
}
