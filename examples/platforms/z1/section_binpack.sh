#!/bin/sh

BASE=$(dirname $0)

ROM_BUF=$BASE/rom
HIROM_BUF=$BASE/hirom
LD_TEMP=$BASE/msp430f2617_template.ld
LD_OUT=$BASE/msp430f2617_custom.ld

AWK_SCRIPT=$BASE/section_binpack.awk

sed_expr='s/\(\.text[^ ]* \)\([0-9][0-9]*\)/\10x\2/p'

#    *(.text .stub .text.* .gnu.linkonce.t.* .text:*)
function pre_rom () {
    cat >> $ROM_BUF <<-EOF
  .text :
  {
    . = ALIGN(2);
    KEEP(*(.text.startup.main))

    . = ALIGN(2);
    *(.lower.text.* .lower.text)

    . = ALIGN(2);
    *(.text .stub .gnu.linkonce.t.* .text:*)
EOF
}

function post_rom () {
    cat >> $ROM_BUF <<-EOF

    KEEP (*(.text.*personality*))
    /* .gnu.warning sections are handled specially by elf32.em.  */
    *(.gnu.warning)
    *(.interp .hash .dynsym .dynstr .gnu.version*)
    PROVIDE (__etext = .);
    PROVIDE (_etext = .);
    PROVIDE (etext = .);
    . = ALIGN(2);
    KEEP (*(.init))
    KEEP (*(.fini))
    KEEP (*(.tm_clone_table))
  } > ROM
EOF
}

function pre_hirom () {
  cat >> $HIROM_BUF <<-EOF
  .upper.text :
  {
    . = ALIGN(2);
EOF
}

function post_hirom () {
  cat >> $HIROM_BUF <<-EOF
  } > HIROM
}

INCLUDE msp430f2617_symbols.ld
EOF
}


function main () {
    if [ -z $1 ]; then
        echo $1
        echo "No obj file given"
    fi

    echo "" > $ROM_BUF
    echo "" > $HIROM_BUF

    pre_rom
    pre_hirom

    for i in $*; do
        #echo "Processing $i"
        objdump -h $i | sed -n "$sed_expr" | 3>>$ROM_BUF 4>>$HIROM_BUF awk -F ' ' -f $AWK_SCRIPT
    done

    post_rom
    post_hirom

    cat $LD_TEMP > $LD_OUT
    cat $ROM_BUF >> $LD_OUT
    cat $HIROM_BUF >> $LD_OUT
}

main $@
