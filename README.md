# minishell

**프로젝트 개요**
minishell은 C 언어와 시스템 콜을 사용하여 기본적인 셸 기능을 재현한 프로젝트입니다.

**프로젝트 의의**
- 프로세스 생성과 제어: fork, execve 등을 사용하여 명령 실행 방식을 이해합니다.
- 파이프와 리디렉션: 파이프(`|`), 입력(`<`), 출력(`>`), 이어쓰기(`>>`), here-doc(`<<`)을 구현하여 I/O 흐름을 학습합니다.
- 내장 명령어 구현: echo, cd, pwd, export, unset, env, exit 등의 기본 셸 명령어를 직접 구현합니다.
- 환경 변수 확장: `$VAR` 형식의 변수 치환 처리 로직을 학습합니다.
- 시그널 처리: SIGINT(Ctrl+C), SIGQUIT(Ctrl+\) 신호를 처리하여 셸의 안정성을 높입니다.

## 주요 기능
- 명령어 실행: 절대/상대 경로 및 PATH 환경 변수를 통한 커맨드 실행
- 내장 명령어: echo, cd, pwd, export, unset, env, exit
- 파이프: `|` 연산자를 통한 다중 명령어 파이프라인 지원
- 리디렉션: `<`, `>`, `>>`, `<<` 구현
- 환경 변수: `$VAR` 치환 기능
- 시그널: SIGINT, SIGQUIT 핸들링

## 프로젝트 구조
```
minishell/
├── inc/         # 헤더 파일
├── src/         # 소스 파일(파싱, 실행, 내장 등)
├── main.c       # 프로그램 진입점
└── Makefile     # 빌드 스크립트
```

## 라이센스
이 프로젝트는 **Beerware License (Revision 42)**를 따릅니다.

```
THE BEER-WARE LICENSE (Revision 42)
As long as you retain this notice you can do anything with this stuff.
If we meet some day, and you think this stuff is worth it, you can buy me a beer in return.
```
