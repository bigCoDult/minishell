#!/bin/bash
# tester.sh

# 테스트 입력값, 여기에 테스트 케이스를 추가하면 됨
TEST_INPUTS=(
    "yes hi | grep -m 3 h"
    "cat < parse.md | grep \"fork\""
    "< parse.md cat | grep \"fork\""
    "cat << delimiter | grep \"fork\"\naaa\nbbb fork\nfork ccc\nddd fork ddd\neee\ndelimiter"
    "<< delimiter cat | grep \"fork\"\naaa\nbbb fork\nfork ccc\nddd fork ddd\neee\ndelimiter"
)

# ANSI 색상 정의
GREEN="\033[32m"
RED="\033[31m"
NC="\033[0m"   # 색상 리셋

# 모든 테스트 실행 시 사용할 임시 파일 (필요시)
TMP_INPUT="./tmp_test_input.tmp"
mkdir -p "$(dirname "$TMP_INPUT")"

# 각 테스트 케이스 실행 함수
run_test_case() {
    local test_input="$1"

    # 테스트 입력을 임시 파일에 기록하여 전달 (여러 줄인 경우도 안전)
    echo -e "$test_input" > "$TMP_INPUT"

    # minishell 실행 결과 캡처
    local mini_output
    mini_output=$(./minishell < "$TMP_INPUT")
    # minishell 출력에서 프롬프트(예: "MINISHELL$>")와 히어독 입력 부분(> 로 시작하는 줄) 제거
    mini_output=$(echo "$mini_output" | sed '/^MINISHELL\$>/d' | sed '/^> /d')

    # bash 실행 결과 캡처 (파일 redirection 방식)
    local bash_output
    bash_output=$(bash < "$TMP_INPUT")

    # 양쪽 출력에서 앞뒤 공백/줄바꿈 제거
    mini_output=$(echo "$mini_output" | sed 's/^[[:space:]]*//; s/[[:space:]]*$//')
    bash_output=$(echo "$bash_output" | sed 's/^[[:space:]]*//; s/[[:space:]]*$//')

    # diff 비교 (출력이 동일하면 PASS, 아니면 FAIL)
    if [ "$mini_output" == "$bash_output" ]; then
        diff_result="${GREEN}PASS${NC}"
    else
        diff_result="${RED}FAIL${NC}"
    fi

    # 결과 출력 (echo -e로 개행 적용)
    echo "--------------"
    echo -e "[입력]\n$test_input\n"
    echo -e "[minishell]\n$mini_output\n"
    echo -e "[bash]\n$bash_output\n"
    echo -e "diff : $diff_result"
}

# 모든 테스트 케이스 실행
for test_input in "${TEST_INPUTS[@]}"; do
    run_test_case "$test_input"
done

# 임시 파일 제거
rm -f "$TMP_INPUT"
