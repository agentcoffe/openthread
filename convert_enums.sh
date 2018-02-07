#!/bin/bash

#for file in $(grep -rnl $1 -e 'enum')
#do
    
#done

cname="[a-zA-Z0-9_-]"

# for comparsion
sed -n "/enum/,/}/p" test.hpp   
# scope the enum again
sed -nr "/enum/,/\}/ {
    # scope the brackets
    /\{/,/\}/ {
        # search for enums with a defined value
        s/($cname$cname*) *= *(.*),/#define \1 \2/p
        T count
        b exit
        # search for enums with an implicit counter
        :count
        /$cname$cname*/ {
                x
                :b
                s/9(_*)$/_\1/
                t b
                s/^(_*)$/0\1/
                s/$/:0123456789/
                s/([^_])(_*):.*\1(.).*/\3\2/
                s/_/0/g
                x
                G
                s/($cname$cname*).*\n(.*)/#define \1 \2/p
         }
        :exit
    }
}" test.hpp

sed -nr ':a
    /A/ {
        x
        :b
        s/9(_*)$/_\1/
        tb
        s/^(_*)$/0\1/
        s/$/:0123456789/
        s/([^_])(_*):.*\1(.).*/\3\2/
        s/_/0/g
        x
        G
        s/A(.*)\n(.*)/\2\1/
        ta
   }' a.txt
