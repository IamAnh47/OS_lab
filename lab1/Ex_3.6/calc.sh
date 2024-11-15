#!/bin/bash

# Khởi tạo biến ANS và mảng HISTORY để lưu trữ lịch sử
ANS=0
HISTORY=()

# Hàm tính toán
calculate() {
    local op1=$1
    local operator=$2
    local op2=$3

    # Nếu toán hạng đầu tiên là ANS, sử dụng giá trị của biến ANS
    if [ "$op1" == "ANS" ]; then
        op1=$ANS
    fi

    # Nếu toán hạng thứ hai là ANS, sử dụng giá trị của biến ANS
    if [ "$op2" == "ANS" ]; then
        op2=$ANS
    fi

    case "$operator" in
        +) ANS=$(echo "$op1 + $op2" | bc);;
        -) ANS=$(echo "$op1 - $op2" | bc);;
        x) ANS=$(echo "$op1 * $op2" | bc);;
        /) 
            if [ "$op2" -eq 0 ]; then
                echo "MATH ERROR"
                return 1
            else
                ANS=$(echo "scale=2; ($op1 / $op2) + 0.005" | bc)
            fi
            ;;
        %) 
            if [ "$op2" -eq 0 ]; then
                echo "MATH ERROR"
                return 1
            else
                ANS=$(echo "scale=2; $op1 / $op2" | bc)
            fi
            ;;
        *) 
            echo "SYNTAX ERROR"
            return 1
            ;;
    esac

    # Lưu vào lịch sử
    HISTORY+=("$op1 $operator $op2 = $ANS")
    if [ "${#HISTORY[@]}" -gt 5 ]; then
        HISTORY=("${HISTORY[@]:1}")
    fi

    # Hiển thị kết quả
    echo "$ANS"
}

# Hiển thị lịch sử
show_history() {
    if [ ${#HISTORY[@]} -eq 0 ]; then
        echo "No history available."
    else
        for entry in "${HISTORY[@]}"; do
            echo "$entry"
        done
    fi
}

# Vòng lặp để nhận, xử lý lệnh
while true; do
    echo -n ">> "
    read input

    # Kiểm tra các lệnh đặc biệt
    if [ "$input" == "EXIT" ]; then
        break
    elif [ "$input" == "HIST" ]; then
        show_history
    else
        set -- $input  

        # Kiểm tra nếu có đúng 3 tham số thì thực hiện phép tính
        if [ $# -eq 3 ]; then
            calculate $1 $2 $3
        else
            echo "SYNTAX ERROR"
        fi
    fi
done