#!/usr/bin/env bash

function FillTable() {
    n=0
    m=0

    a=()
    while read -r LinefromFile || [[ -n "${LinefromFile}" ]]; do # второе условие возвращает true если строка не пустая
        m=0
        for k in ${LinefromFile//,/ }; do
            a+=($k)
            m=$(($m + 1))
        done

        n=$(($n + 1))

    done <"$1"
}

function Gauss() {
    p=`echo "5 * $r" | bc`
    for ((i = 0; i < $n; i++)); do
        for ((j = $i + 1; j < $n; j++)); do
            val1=${a[$i * $m + $i]}
            val2=${a[$j * $m + $i]}
            if [ $val1 -lt $val2 ]; then
                for ((k = 0; k <= $n; k++)); do
                    # swap ik jk
                    #echo ${a[$i * $m + $k]} ${a[$j * $m + $k]}
                    temp_ik=${a[$i * $m + $k]}
                    temp_jk=${a[$j * $m + $k]}
                    a[$i * $m + $k]=$(echo "$temp_ik + $temp_jk" | bc)

                    temp_ik=${a[$i * $m + $k]}
                    temp_jk=${a[$j * $m + $k]}
                    a[$j * $m + $k]=$(echo "$temp_ik - $temp_jk" | bc)

                    temp_ik=${a[$i * $m + $k]}
                    temp_jk=${a[$j * $m + $k]}
                    a[$i * $m + $k]=$(echo "$temp_ik - $temp_jk" | bc)
                    #echo ${a[$i * $m + $k]} ${a[$j * $m + $k]}
                    #echo =========
                done
            fi

        done
    done

    for ((i = 0; i < $n - 1; i++)); do
        for ((j = $i + 1; j < $n; j++)); do
            temp_ji=${a[$j * $m + $i]}
            temp_ii=${a[$i * $m + $i]}

            f=$(echo "scale=20; ${temp_ji}/${temp_ii}" | bc)
            for ((k = 0; k <= $n; k++)); do
                temp_jk=${a[$j * $m + $k]}
                temp_ik=${a[$i * $m + $k]}
                a[$j * $m + $k]=$(echo "scale=20; $temp_jk - $f * $temp_ik" | bc)
            done

        done
    done

    res=()
    for ((i = 0; i < $n; i++)); do
        res+=(0.0)
    done

    for ((i = $n - 1; i >= 0; i--)); do
        res[$i]=${a[$i * $m + $n]}
        # echo $i _1=${res[$i]}
        for ((j = $i + 1; j < $n; j++)); do
            if [[ $i -ne $j ]]; then
                temp_ij=${a[$i * $m + $j]}
                res[$i]=$(echo "scale=20; ${res[$i]} - ${temp_ij}*${res[$j]}" | bc)
                # echo $i _2=${res[$i]}
            fi
        done
        temp_ii=${a[$i * $m + $i]}
        res[i]=$(echo "scale=20; ${res[$i]}/${temp_ii}" | bc)
        # echo $i _3=${res[$i]}
    done

    for ((i = 0; i < $n; i++)); do
        echo ${res[$i]}
    done
}

FillTable $1
Gauss