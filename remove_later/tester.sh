#!/bin/bash
# tester.sh

# 테스트 입력값, 여기에 테스트 케이스를 추가하면 됨
TEST_INPUTS=(
    "cat < parse.md | grep 'fork'"
    "< parse.md cat | grep \"fork\""
    "< parse.md cat | grep \"fork\""
	"yes hi | grep -m 1 h"

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

    # minishell 실행 결과 캡처
    # 테스트 입력을 임시 파일에 기록하여 전달(여러 줄이 필요한 경우도 안전)
    echo -e "$test_input" > "$TMP_INPUT"
    local mini_output
    mini_output=$(./minishell < "$TMP_INPUT")
    # minishell 출력에서 프롬프트(예: "MINISHELL$>") 제거
    mini_output=$(echo "$mini_output" | sed '/^MINISHELL\$>/d')

    # bash 실행 결과 캡처
    local bash_output
    bash_output=$(echo -e "$test_input" | bash)

    # 양쪽 출력에서 앞뒤 공백/줄바꿈 제거
    mini_output=$(echo "$mini_output" | sed 's/^[[:space:]]*//; s/[[:space:]]*$//')
    bash_output=$(echo "$bash_output" | sed 's/^[[:space:]]*//; s/[[:space:]]*$//')

    # diff 비교 (출력이 동일하면 PASS, 아니면 FAIL)
    if [ "$mini_output" == "$bash_output" ]; then
        diff_result="${GREEN}PASS${NC}"
    else
        diff_result="${RED}FAIL${NC}"
    fi

    # 결과 출력
    echo "--------------"
    echo "입력 : $test_input"
    echo "mini : $mini_output"
    echo "bash : $bash_output"
    echo -e "diff : $diff_result"
}

# 모든 테스트 케이스 실행
for test_input in "${TEST_INPUTS[@]}"; do
    run_test_case "$test_input"
done

# 임시 파일 제거
rm -f "$TMP_INPUT"
