# Sokoban Team Project

프로그래밍기초및실습 팀 프로젝트

## 팀원

| 이름  | 브랜치 |
| --- | --- |
| Kim | kim |
| Jwa | jwa |
| Yoo | yoo |

---

# 프로젝트 소개

C 언어 기반 소코반(Sokoban) 게임 구현 프로젝트입니다.

프로젝트 목표:

* 파일 입출력 학습
* 배열 기반 맵 처리
* 함수 분리 및 구조 설계
* GitHub 협업 경험
* 게임 로직 구현

---

# 개발 환경

* Language : C
* Compiler : gcc
* OS : Ubuntu

컴파일 예시:

```bash
gcc main.c -o sokoban
./sokoban
```

---

# GitHub 협업 규칙

## 브랜치 구조

```text
main
 ├── kim
 ├── jwa
 └── yoo
```

---

## 작업 순서

### 1. 최신 코드 받기

```bash
git checkout main
git pull origin main
```

---

### 2. 자신의 브랜치로 이동

```bash
git checkout kim
```

또는

```bash
git checkout jwa
```

또는

```bash
git checkout yoo
```

---

### 3. 작업 후 commit

```bash
git add .
git commit -m "MOVE-001 플레이어 이동 구현"
```

---

### 4. GitHub 업로드

```bash
git push origin kim
```

---

### 5. Pull Request 생성

GitHub에서:

```text
본인 브랜치 → main
```

Pull Request 생성 후 merge.

---

# 프로젝트 구조

```text
sokoban/
├── main.c
├── map
├── ranking
├── soko
├── README.md
└── docs/
```

---

# 주요 기능

## 필수 기능

* map 파일 읽기
* 맵 오류 검사
* 플레이어 이동
* 박스 밀기
* 레벨 진행
* 이동 횟수 출력
* save / load
* undo
* ranking
* record / play

---

# 파일 설명

## map

레벨 맵 저장 파일.

사용 문자:

```text
@ : 플레이어
# : 벽
$ : 박스
O : 보관장소
. : 빈칸
s : 레벨 시작
e : 마지막 레벨 끝
```

---

## soko

게임 저장 파일.

저장 내용:

* 플레이어 이름
* 현재 레벨
* 이동 횟수
* 현재 맵 상태

---

## ranking

레벨별 랭킹 저장 파일.

저장 형식 예시:

```text
1 kim 120
1 jwa 140
2 yoo 95
```

---

# 실행 방법

```bash
gcc main.c -o sokoban
./sokoban
```

입력 재지정 테스트:

```bash
./sokoban < testfile
```

---

# 사용 명령어

| 명령 | 기능          |
| -- | ----------- |
| h  | 왼쪽 이동       |
| j  | 아래 이동       |
| k  | 위 이동        |
| l  | 오른쪽 이동      |
| u  | undo        |
| a  | 현재 레벨 다시 시작 |
| n  | 새 게임        |
| s  | 저장          |
| f  | 불러오기        |
| r  | 녹화 시작       |
| e  | 녹화 종료       |
| p  | 녹화 재생       |
| d  | 도움말         |
| t  | 랭킹 출력       |
| x  | 종료          |

---

# 구현 예정 구조

```text
main
 ├── map loader
 ├── movement system
 ├── save/load system
 ├── undo system
 ├── ranking system
 └── recording system
```

---

# 주의 사항

* 전역변수 사용 금지
* gcc 경고 없이 컴파일
* Ubuntu 환경에서 실행 가능해야 함
* map / ranking / soko 파일은 텍스트 파일 사용
* 팀 회의 최소 5회 이상 진행

---

# 팀 규칙

* main 브랜치 직접 수정 금지
* 작업 전 반드시 git pull
* commit 메시지에 기능 ID 작성
* 작업 완료 후 Pull Request 생성
