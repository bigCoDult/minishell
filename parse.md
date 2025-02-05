# 파싱부에 대한 설명
## 1. 기본 구조
- 명령어 입력시 토큰화 -> 토큰화 된 데이터를 트리 구조로 파싱 -> 트리를 순회하며 명령어 실행

## 2. 구조체
### 전역변수
```c
volatile sig_atomic_t g_signal;  // 시그널 처리용 전역변수
```

### 쉘 상태 구조체
```c
typedef struct s_shell {
    t_token *tokens;        // 토큰 리스트
    t_command *commands;    // 명령어 트리
    t_status status;        // 상태 정보
    t_memory *mem_track;    // 메모리 추적 리스트
    char *input_line;       // 현재 입력 라인
    t_parser parser;        // 파서 컨텍스트
    t_pipe pipe;            // 파이프 관리
    t_heredoc heredoc;       // 히어독 관리
    t_env *env;             // 환경 변수 리스트
} t_shell;
```

### 상태 구조체
```c
typedef struct s_status {
    int exit_code;          // 종료 상태 ($?)
} t_status;
```

### 토큰 구조체
```c
typedef enum {
    TOKEN_WORD,     // 일반 단어
    TOKEN_PIPE,     // 파이프 |
    TOKEN_REDIR,    // 리다이렉션 <, >, >>, <<
    TOKEN_ENV,      // 환경변수 $
    TOKEN_EOF       // 입력 종료
} TokenType;

typedef struct s_token {
    TokenType type;         
    char *value;           
    struct s_token *next;  
    struct s_token *prev;  
} t_token;
```

### 명령어 구조체
```c
typedef struct s_command {
    char *name;                  // 명령어 이름
    char **args;                // 인자 배열
    struct s_redirection *redirs; // 리다이렉션 리스트
    struct s_command *pipe_next; // 파이프 다음 명령어
} t_command;
```

### 리다이렉션 구조체
```c
typedef enum {
    REDIR_IN,     // <
    REDIR_OUT,    // >
    REDIR_APPEND, // >>
    REDIR_HEREDOC // <<
} t_redirection_type;

typedef struct s_redirection {
    t_redirection_type type;    // 리다이렉션 타입
    char *filename;              // 파일명 또는 구분자
    struct s_redirection *next;  // 다음 리다이렉션
} t_redirection;
```

### 메모리 구조체
```c
typedef struct s_memory {
    void *addr;             // 할당된 메모리 주소
    struct s_memory *next;  // 다음 노드
} t_memory;
```

### 환경 변수 구조체
```c
typedef struct s_env {
    char *key;          // 환경변수 키
    char *value;        // 환경변수 값
    struct s_env *next; // 다음 환경변수
} t_env;
```

### 파이프 관리
```c
typedef struct s_pipe {
    int read_fd;        // 읽기용 파일 디스크립터
    int write_fd;       // 쓰기용 파일 디스크립터
} t_pipe;
```

### 히어독 관리
```c
typedef struct s_heredoc {
    char *delimiter;    // 구분자
    int temp_fd;        // 임시 파일 디스크립터
} t_heredoc;
```

### 파서 컨텍스트 관리
```c
typedef struct s_parser {
    char *input;        // 입력 문자열
    size_t pos;         // 현재 파싱 위치
    t_token *curr_token;// 현재 토큰
} t_parser;
```

## 3. 파싱 프로세스
###  토큰화 단계
입력 문자열을 의미 있는 토큰으로 분리
공백, 따옴표, 특수문자 등을 기준으로 토큰 생성
환경변수 확장, 명령어 치환 처리
###  구문 분석 단계
토큰들을 문법 규칙에 따라 트리 구조로 변환
파이프(|), 리다이렉션(<,>), 세미콜론(;) 등의 연산자를 내부 노드로 구성
실제 명령어와 인자들은 리프 노드로 구성
### 트리구조 생성
명령어의 실행 순서를 명확하게 하기 위해 트리 구조 형성

## 4. 특수 처리
###  따옴표 처리
작은따옴표('): 모든 문자를 리터럴로 처리
큰따옴표("): $를 제외한 모든 문자를 리터럴로 처리
닫히지 않은 따옴표는 에러 처리
### 리다이렉션 처리
입력 리다이렉션 (<)
출력 리다이렉션 (>, >>)
heredoc (<<)
파일 디스크립터 관리

## 5. 실행 구조
### 명령어 실행
PATH 검색 또는 경로 기반 실행 파일 찾기
내장 명령어 확인
필요한 경우 서브쉘 생성
execve() 함수를 통한 실행
### 프로세스 관리
fork()를 통한 자식 프로세스 생성
파이프를 통한 프로세스 간 통신
종료 상태 관리 ($?)