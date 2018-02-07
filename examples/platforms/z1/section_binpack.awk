BEGIN { ROM_SIZE = 52926;
        HIROM_SIZE = 40958;
        rom_size_c = 0;
        hirom_size_c = 0;
        fill_c = 0;
    }
{ 
    tmp = strtonum($4);
    if ($1 == "*fill*") {
        fill_c += tmp;
    } else if ($2 == "@none@") {
        if ((rom_size_c + tmp) < ROM_SIZE) {
            rom_size_c += tmp;
        } else if ((hirom_size_c + tmp) < HIROM_SIZE) {
            hirom_size_c += tmp;
        } else {
            printf("OUT OF MEMORY_\n");
            printf("> %x    hi *(%s)\n", tmp, $2);
        }
    } else {
        if ((rom_size_c + tmp) < ROM_SIZE) {
            printf("    *(%s.%s)\n", $1, $2) | "cat 1>&3";
            rom_size_c += tmp;
        } else if ((hirom_size_c + tmp) < HIROM_SIZE) {
            printf("    *(%s.%s)\n", $1, $2) | "cat 1>&4";;
            hirom_size_c += tmp;
        } else {
            printf("OUT OF MEMORY\n");
            printf("> %x    hi *(%s)\n", tmp, $2);
        }
    }
}


END { printf("ROM: %x, HIROM: %x\n", rom_size_c, hirom_size_c); }
