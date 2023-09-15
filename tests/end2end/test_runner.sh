#!/bin/bash

#color atributes
red="\033[1;31m"
green="\033[1;32m"
blue="\033[1;36m"
purple="\033[1;35m"
white="\033[1;37m"
usual="\033[0m"
#directories paths and exe file for test generating
resource_dir="../tests/end2end/resources/"
tests_dir="../tests/end2end/resources/tests/"
answs_dir="../tests/end2end/resources/answers/"
exe_test_file="./tests/end2end"
exe_main_file="./triangles"
answ_generator="python3 ../tests/end2end/generator.py"

##------------------------------------------------------------------------------------------##
function run_tests {
    clear_old_data

    generate_tests

    touch compare_file.txt
    if [ -d "${resource_dir}" ]
    then
        echo -e "${white}end2end testing:${usual}"
        for ((i = 1; i <= ${tests_number}; ++i))
        do      
                tmp_check=${tests_dir}/test${i}.txt
                ${exe_main_file} < ${tmp_check} > compare_file.txt

                echo -n -e "${purple}Test ${i}: ${usual}\n"
                ans_file="${answs_dir}answ_${i}.txt"
                if diff -w -n ${ans_file} compare_file.txt &>/dev/null
                then
                    echo -e "${green}passed${usual}" 
                else
                    echo -e "${red}failed${usual}"
                fi
                echo -en "${blue}numbers of intersecting triangles:${usual}"
                printf "\n"
                cat ${ans_file}
                printf "\n"
        done
    else
        echo -e "${blue}can't generate tests. ${usual}${resource_dir} is empty"
    fi

    rm compare_file.txt
}
##------------------------------------------------------------------------------------------##

##------------------------------------------------------------------------------------------##
function generate_tests {
    def_file_name="test"
    for ((i = 1; i <= ${tests_number}; ++i))
    do
        tmp_test_file="${def_file_name}${i}.txt"
        ${exe_test_file} ${tmp_test_file} ${triangles_number} ${min_bound} ${max_bound}
        ${answ_generator} ${tests_dir}${tmp_test_file} ${i}
    done

}
##------------------------------------------------------------------------------------------##

##------------------------------------------------------------------------------------------##
function clear_old_data {
    rm ${tests_dir}test*
    if !([ `ls ${answs_dir} | wc -l` -eq 0 ])
    then
        rm ${answs_dir}answ*
    fi
}
##------------------------------------------------------------------------------------------##

#main part
error="false"
tests_number=0
triangles_number=0
min_bound=0
max_bound=0

if [ $# -lt 2 ]
then
    error="true"
    echo -e "${red}invalid number of arguments: expected 2 or 3 or 4, got $#"
elif [ $# -eq 2 ]
then
    tests_number=$1
    triangles_number=$2
elif [ $# -eq 3 ]
then
    tests_number=$1
    triangles_number=$2
    min_bound=$3
elif [ $# -eq 4 ] 
then
    tests_number=$1
    triangles_number=$2
    min_bound=$3
    max_bound=$4
else
    error="false"
    echo -e "${red}invalid number of arguments: expected 2 or 3 or 4, got $#"
fi

if [ $error = "false" ]
then
    run_tests 
fi
