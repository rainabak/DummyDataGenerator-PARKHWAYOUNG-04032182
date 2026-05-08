# DummyDataGenerator — 수동 테스트 시나리오

DummyDataGenerator PoC의 기능 검증 및 설계 준수 여부를 확인하는 수동 테스트 문서입니다.  
자동화 프레임워크를 사용하지 않으며, 직접 실행·코드 리뷰를 통해 판정합니다.

---

## 공통 사전 조건

- Visual Studio 2022 빌드 완료 상태
- `data/samples.json`, `data/orders.json` — Seed 데이터 파일 정상 존재
- 각 TC 시작 전 변경된 데이터 파일을 Seed 원본으로 복구
- 결과 표기: `PASS` / `FAIL` / `SKIP`

### 기준 Seed 데이터 요약

**samples.json**

| id | name | stock |
|---|---|---|
| 1 | 시료-A형 | 10 |
| 2 | 시료-B형 | 3 |
| 3 | 시료-C형 | 0 |

**orders.json**

| id | productName | quantity | status |
|---|---|---|---|
| 1 | 시료-A형 | 3 | RESERVED |
| 2 | 시료-B형 | 2 | RESERVED |
| 3 | 시료-C형 | 2 | RESERVED |
| 4 | 시료-A형 | 4 | PRODUCING |
| 5 | 시료-B형 | 3 | PRODUCING |
| 6 | 시료-A형 | 2 | CONFIRMED |
| 7 | 시료-B형 | 5 | CONFIRMED |
| 8 | 시료-A형 | 1 | RELEASE |
| 9 | 시료-B형 | 2 | RELEASE |
| 10 | 시료-A형 | 5 | REJECTED |
| 11 | 시료-B형 | 3 | REJECTED |
| 12 | 시료-C형 | 1 | REJECTED |

---

## 1. 프로그램 실행 테스트

### TC-01-A. 정상 실행

**목적**: Seed 데이터가 있을 때 프로그램이 정상 시작되는지 확인한다.

**실행 순서**

1. DummyDataGenerator.exe 실행
2. 메인 메뉴 출력 확인

**기대 결과**

```
메인 메뉴가 콘솔에 정상 출력된다.
1. 시료 관리
2. 주문 관리
3. 생산 라인 관리
4. 모니터링
5. 출하 관리
0. 종료
```

**판정**: 메뉴가 출력되고 입력 대기 상태이면 **PASS**

---

### TC-01-B. 모니터링 메뉴 진입

**목적**: 메인 메뉴에서 모니터링 서브메뉴로 정상 진입하는지 확인한다.

**실행 순서**

1. 프로그램 실행
2. 메인 메뉴 → `4` 입력

**기대 결과**

```
[ 모니터링 ] 서브메뉴가 출력된다.
1. 전체 Dashboard
2. 주문 상태별 집계
3. 시료 재고 현황
0. 돌아가기
```

**판정**: 서브메뉴가 정상 출력되면 **PASS**

---

### TC-01-C. 뒤로가기 처리

**목적**: 모니터링 서브메뉴에서 `0` 입력 시 메인 메뉴로 복귀하는지 확인한다.

**실행 순서**

1. 모니터링 서브메뉴 진입
2. `0` 입력

**기대 결과**: 프로그램 종료 없이 메인 메뉴로 복귀

**판정**: 메인 메뉴가 재출력되면 **PASS** / 프로그램 종료 시 **FAIL**

---

## 2. JSON 파일 읽기 테스트

### TC-02-A. orders.json 정상 읽기

**목적**: `orders.json` 파일이 정상적으로 로드되는지 확인한다.

**실행 순서**

1. 프로그램 실행
2. 모니터링 → `2. 주문 상태별 집계` 선택

**기대 결과**: 0이 아닌 주문 건수가 화면에 출력된다.

**판정**: 집계 결과가 표시되면 **PASS** / 모든 값이 0이면 **FAIL**

---

### TC-02-B. samples.json 정상 읽기

**목적**: `samples.json` 파일이 정상적으로 로드되는지 확인한다.

**실행 순서**

1. 프로그램 실행
2. 모니터링 → `3. 시료 재고 현황` 선택

**기대 결과**: 시료-A형, 시료-B형, 시료-C형이 목록에 표시된다.

**판정**: 3개 시료가 모두 출력되면 **PASS** / 목록이 비어있으면 **FAIL**

---

### TC-02-C. 메뉴 재진입 시 최신 파일 반영

**목적**: 메뉴를 나갔다 다시 들어올 때 변경된 JSON 파일이 반영되는지 확인한다.

**실행 순서**

1. 프로그램 실행
2. 모니터링 → `2. 주문 상태별 집계` 선택 → 건수 확인 → `0` 뒤로가기
3. `orders.json`에서 임의 주문 1건 추가 (외부 편집기 사용)
4. 모니터링 → `2. 주문 상태별 집계` 재선택

**기대 결과**: 편집된 내용이 반영되어 건수가 1 증가한다.

**판정**: 변경이 반영되면 **PASS** / 기존 값 그대로이면 **FAIL**

> `findAll()`이 매 호출마다 파일을 재읽기하는 설계를 검증한다.

---

## 3. 상태별 주문 수 집계 테스트

### TC-03. 주문 상태별 집계 정확성

**목적**: Seed 데이터 기준으로 상태별 주문 건수가 정확히 집계되는지 확인한다.

**실행 순서**

1. 프로그램 실행 (Seed 데이터 기준)
2. 모니터링 → `2. 주문 상태별 집계` 선택

**기대 결과**

| 상태 | 기대 건수 |
|---|---|
| RESERVED | 3건 |
| PRODUCING | 2건 |
| CONFIRMED | 2건 |
| RELEASE | 2건 |
| 합계 | 9건 |

**판정**: 모든 항목이 기대값과 일치하면 **PASS** / 하나라도 불일치하면 **FAIL**

---

## 4. REJECTED 제외 검증

### TC-04-A. REJECTED 집계 제외 확인

**목적**: REJECTED 상태 주문이 집계 합계에 포함되지 않는지 확인한다.

**사전 조건**: Seed 데이터 사용 (REJECTED: id 10, 11, 12 총 3건)

**실행 순서**

1. 모니터링 → `2. 주문 상태별 집계` 선택

**기대 결과**

- 화면에 `REJECTED` 항목이 표시되지 않는다.
- 합계: `9건` (전체 12건에서 REJECTED 3건 제외)

**판정**: 합계가 9이고 REJECTED 항목이 없으면 **PASS** / 합계가 12이거나 REJECTED가 표시되면 **FAIL**

---

### TC-04-B. REJECTED만 있는 경우

**목적**: 모든 주문이 REJECTED일 때 합계가 0으로 표시되는지 확인한다.

**사전 조건**: `orders.json`을 아래로 교체한다.

```json
{
  "nextId": 3,
  "items": [
    { "id": 1, "productName": "시료-A형", "quantity": 2, "status": "REJECTED" },
    { "id": 2, "productName": "시료-B형", "quantity": 3, "status": "REJECTED" }
  ]
}
```

**실행 순서**: 모니터링 → `2. 주문 상태별 집계` 선택

**기대 결과**: RESERVED / PRODUCING / CONFIRMED / RELEASE 모두 0건, 합계 0건

**판정**: 합계 0 출력 시 **PASS** / 1 이상이면 **FAIL**

---

## 5. 시료별 재고 현황 테스트

### TC-05-A. 시료 목록 전체 출력 확인

**목적**: Seed 데이터의 3개 시료가 순서대로 표시되는지 확인한다.

**실행 순서**

1. 프로그램 실행
2. 모니터링 → `3. 시료 재고 현황` 선택

**기대 결과**

```
- 시료-A형 / A형 반도체 검사 시료 / 재고 10 / 상태 여유
- 시료-B형 / B형 반도체 검사 시료 / 재고  3 / 상태 부족
- 시료-C형 / C형 반도체 검사 시료 / 재고  0 / 상태 고갈
```

**판정**: 3개 시료가 순서대로 올바른 재고·상태로 표시되면 **PASS**

---

### TC-05-B. 시료 0건 처리

**목적**: 시료 데이터가 없을 때 빈 목록 안내 메시지가 출력되는지 확인한다.

**사전 조건**: `samples.json`을 아래로 교체한다.

```json
{ "nextId": 1, "items": [] }
```

**실행 순서**: 모니터링 → `3. 시료 재고 현황` 선택

**기대 결과**: "등록된 시료가 없습니다." 메시지 출력, 프로그램 종료 없음

**판정**: 안내 메시지 출력 후 메뉴 복귀 시 **PASS** / crash 또는 메시지 없음 시 **FAIL**

---

## 6. 재고 상태 계산 테스트

### TC-06-A. SURPLUS (여유) 상태 확인

**목적**: `stock > demand(RESERVED+PRODUCING)`일 때 `여유`로 표시되는지 확인한다.

**검증 대상**: 시료-A형

| 항목 | 값 |
|---|---|
| stock | 10 |
| RESERVED 수량 | 3 |
| PRODUCING 수량 | 4 |
| demand 합계 | 7 |
| stock > demand | 10 > 7 → **여유** |

**실행 순서**: 모니터링 → `3. 시료 재고 현황` 선택

**기대 결과**: `시료-A형` 항목에 `상태 여유` 표시

**판정**: `여유` 출력 시 **PASS** / 다른 값 출력 시 **FAIL**

---

### TC-06-B. SHORTAGE (부족) 상태 확인

**목적**: `0 < stock ≤ demand`일 때 `부족`으로 표시되는지 확인한다.

**검증 대상**: 시료-B형

| 항목 | 값 |
|---|---|
| stock | 3 |
| RESERVED 수량 | 2 |
| PRODUCING 수량 | 3 |
| demand 합계 | 5 |
| 0 < stock ≤ demand | 0 < 3 ≤ 5 → **부족** |

**기대 결과**: `시료-B형` 항목에 `상태 부족` 표시

**판정**: `부족` 출력 시 **PASS** / 다른 값 출력 시 **FAIL**

---

### TC-06-C. DEPLETED (고갈) 상태 확인

**목적**: `stock == 0`일 때 demand와 무관하게 `고갈`로 표시되는지 확인한다.

**검증 대상**: 시료-C형

| 항목 | 값 |
|---|---|
| stock | 0 |
| RESERVED 수량 | 2 |
| demand 합계 | 2 |
| stock == 0 | → **고갈** (demand 무관) |

**기대 결과**: `시료-C형` 항목에 `상태 고갈` 표시

**판정**: `고갈` 출력 시 **PASS** / 다른 값 출력 시 **FAIL**

---

### TC-06-D. stock == 0, demand == 0 (주문 없는 고갈)

**목적**: stock이 0이고 해당 시료의 주문이 전혀 없을 때도 `고갈`로 표시되는지 확인한다.

**사전 조건**: `orders.json`에서 시료-C형 주문(id 3, 12)을 모두 삭제한다.

**기대 결과**: 시료-C형에 `상태 고갈` 유지 (demand=0이어도 stock=0이므로 고갈)

**판정**: `고갈` 출력 시 **PASS** / `여유` 출력 시 **FAIL**

---

## 7. JSON 파일 누락 테스트

### TC-07-A. orders.json 누락

**목적**: `orders.json`이 없을 때 프로그램이 안전하게 동작하는지 확인한다.

**사전 조건**: `data/orders.json`을 삭제 또는 이름 변경, `samples.json`은 정상 유지

**실행 순서**

1. 프로그램 실행
2. 모니터링 → `2. 주문 상태별 집계` 선택

**기대 결과**

- 프로그램이 종료되지 않는다.
- 콘솔에 `[안내] 주문 데이터 파일이 없습니다. 빈 목록으로 표시합니다.` 출력
- 모든 상태 건수 0, 합계 0

**판정**: 안내 메시지 출력 + 0건 + 메뉴 복귀 시 **PASS** / crash 시 **FAIL**

---

### TC-07-B. samples.json 누락

**목적**: `samples.json`이 없을 때 프로그램이 안전하게 동작하는지 확인한다.

**사전 조건**: `data/samples.json`을 삭제 또는 이름 변경, `orders.json`은 정상 유지

**실행 순서**

1. 프로그램 실행
2. 모니터링 → `3. 시료 재고 현황` 선택

**기대 결과**

- 프로그램이 종료되지 않는다.
- 콘솔에 `[안내] 시료 데이터 파일이 없습니다. 빈 목록으로 표시합니다.` 출력
- "등록된 시료가 없습니다." 화면 출력

**판정**: 안내 메시지 출력 + 빈 목록 + 메뉴 복귀 시 **PASS** / crash 시 **FAIL**

---

### TC-07-C. 양쪽 파일 모두 누락

**목적**: 두 파일 모두 없을 때 Dashboard가 안전하게 출력되는지 확인한다.

**사전 조건**: `data/orders.json`, `data/samples.json` 모두 삭제

**실행 순서**: 모니터링 → `1. 전체 Dashboard` 선택

**기대 결과**: 주문 0건, 시료 0건 Dashboard 출력, 프로그램 유지

**판정**: crash 없이 빈 Dashboard 출력 시 **PASS**

---

## 8. JSON 형식 오류 테스트

### TC-08-A. orders.json 형식 오류

**목적**: `orders.json`이 유효하지 않은 JSON일 때 `.bak` 백업 후 안전하게 처리되는지 확인한다.

**사전 조건**: `data/orders.json`에 아래 내용을 저장한다.

```
{ broken json content
  "nextId": ???
  "items": [
```

**실행 순서**

1. 프로그램 실행
2. 모니터링 → `2. 주문 상태별 집계` 선택

**기대 결과**

- 콘솔에 `[오류] JSON 형식 오류 — 파일을 백업하고 초기화합니다:` 메시지 출력
- `data/orders.json.bak` 파일이 생성된다.
- 주문 건수 모두 0, 프로그램 종료 없음

**판정**: `.bak` 생성 + 0건 표시 + 메뉴 복귀 시 **PASS** / crash 또는 `.bak` 미생성 시 **FAIL**

---

### TC-08-B. samples.json 형식 오류

**목적**: `samples.json`이 유효하지 않은 JSON일 때 안전하게 처리되는지 확인한다.

**사전 조건**: `data/samples.json`에 손상된 내용을 저장한다. (TC-08-A와 동일 방식)

**실행 순서**: 모니터링 → `3. 시료 재고 현황` 선택

**기대 결과**

- `[오류]` 메시지 출력
- `data/samples.json.bak` 생성
- "등록된 시료가 없습니다." 출력, 프로그램 유지

**판정**: `.bak` 생성 + 안내 메시지 + 메뉴 복귀 시 **PASS** / crash 또는 `.bak` 미생성 시 **FAIL**

---

### TC-08-C. 빈 파일

**목적**: JSON 파일이 존재하지만 내용이 비어있을 때 안전하게 처리되는지 확인한다.

**사전 조건**: `data/orders.json`을 빈 파일(0 bytes)로 교체한다.

**기대 결과**: 오류 없이 0건으로 처리, 프로그램 유지

**판정**: 0건 표시 + 메뉴 복귀 시 **PASS** / crash 시 **FAIL**

---

## 9. MVC 역할 분리 검증

> 런타임 실행 테스트가 아닌 **코드 리뷰**로 검증한다.

### TC-09-A. MonitoringService — 계산 로직 집중

**검증 위치**: `src/services/MonitoringService.cpp`

| 점검 항목 | 기대 상태 | 판정 |
|---|---|---|
| `getOrderSummary()`에 상태별 카운팅 로직 존재 | 존재 | |
| `getSampleStatuses()`에 demand 계산 로직 존재 | 존재 | |
| `calcStockStatus()` — SURPLUS / SHORTAGE / DEPLETED 분기 | 존재 | |
| `std::cout` 또는 `std::cin` 호출 없음 | 없어야 함 | |
| Repository를 통해서만 데이터 접근 (직접 파일 접근 없음) | 없어야 함 | |

**판정**: 모든 항목 충족 시 **PASS**

---

### TC-09-B. MonitoringView — 표시 전용

**검증 위치**: `src/views/MonitoringView.cpp`

| 점검 항목 | 기대 상태 | 판정 |
|---|---|---|
| `showOrderSummary()` — `sum`, `count` 등 산술 연산 없음 | 없어야 함 | |
| `showSampleList()` — demand 계산 없음 | 없어야 함 | |
| `toStatusLabel()` — enum → 문자열 변환만 수행 (분기 없는 switch) | 존재 | |
| `JsonFileStorage`, `Repository` include 없음 | 없어야 함 | |
| `MonitoringService` include 없음 | 없어야 함 | |

**판정**: 모든 항목 충족 시 **PASS**

---

### TC-09-C. MonitoringController — 흐름 조율 전용

**검증 위치**: `src/controllers/MonitoringController.cpp`

| 점검 항목 | 기대 상태 | 판정 |
|---|---|---|
| `handleDashboard()` — Service 호출 후 View 전달, 그 외 로직 없음 | 충족 | |
| `handleOrderSummary()` — 같은 패턴 | 충족 | |
| `handleSampleList()` — 같은 패턴 | 충족 | |
| `std::cout` 직접 호출 없음 (View 통해서만 출력) | 없어야 함 | |
| `if/else` 집계 분기 없음 | 없어야 함 | |

**판정**: 모든 항목 충족 시 **PASS**

---

### TC-09-D. Repository — 조회 전용 사용 확인

**검증 위치**: `src/services/MonitoringService.cpp`

| 점검 항목 | 기대 상태 | 판정 |
|---|---|---|
| `m_orderRepo.findAll()` 만 사용 | 충족 | |
| `m_sampleRepo.findAll()` 만 사용 | 충족 | |
| `add()`, `update()`, `remove()`, `persist()` 미호출 | 없어야 함 | |

**판정**: 모든 항목 충족 시 **PASS**

---

### TC-09-E. 의존성 방향 검증

**목적**: 레이어 간 역방향 의존성이 없는지 확인한다.

| 검증 항목 | 기대 상태 | 판정 |
|---|---|---|
| View가 Service를 include하지 않음 | 없어야 함 | |
| View가 Repository를 include하지 않음 | 없어야 함 | |
| Service가 View를 include하지 않음 | 없어야 함 | |
| Repository가 Controller를 include하지 않음 | 없어야 함 | |

**의존성 방향 (정방향만 허용)**

```
main.cpp
  └─ Controller → Service → Repository → JsonFileStorage
  └─ Controller → View
```

**판정**: 모든 역방향 의존성 부재 시 **PASS**

---

## 10. Clean Code 점검 항목

> 코드 리뷰로 확인한다. 파일 경로를 명시한다.

### TC-10-A. 네이밍 규칙

| 점검 대상 | 규칙 | 파일 | 판정 |
|---|---|---|---|
| 클래스명 | PascalCase | 모든 .h | |
| 메서드명 | camelCase | 모든 .cpp | |
| 멤버 변수 | `m_` 접두사 | 모든 .cpp | |
| 상수 | UPPER_SNAKE_CASE 또는 파일 스코프 static const | MonitoringService.cpp, MonitoringView.cpp | |

---

### TC-10-B. 헤더 / 가드

| 점검 항목 | 기대 상태 | 판정 |
|---|---|---|
| 모든 `.h` 파일에 `#pragma once` 존재 | 존재 | |
| `#pragma once`와 `#ifndef` 혼용 없음 | 혼용 없음 | |

---

### TC-10-C. 주석 규칙

| 점검 항목 | 기대 상태 | 판정 |
|---|---|---|
| `// TODO`, `// FIXME` 잔존 없음 | 없어야 함 | |
| 멀티라인 `/* ... */` 블록 주석 없음 | 없어야 함 | |
| 존재하는 주석이 "왜(Why)"를 설명 | 충족 | |

---

### TC-10-D. 금지 패턴

| 점검 항목 | 기대 상태 | 판정 |
|---|---|---|
| `using namespace std;` 없음 | 없어야 함 | |
| 전역 변수 없음 | 없어야 함 | |
| `exit()`, `abort()` 강제 종료 없음 | 없어야 함 | |
| 파일 직접 접근 (`fstream`) — `JsonFileStorage.cpp` 이외 없음 | 없어야 함 | |

---

### TC-10-E. 함수 규모

| 점검 항목 | 기준 | 판정 |
|---|---|---|
| 함수 길이 30줄 이하 | 초과 없어야 함 | |
| 중첩 깊이 3단계 이하 | 초과 없어야 함 | |
| 함수 하나가 하나의 책임만 수행 | 충족 | |

---

### TC-10-F. 매직 스트링 / 매직 넘버

| 점검 위치 | 점검 항목 | 기대 상태 | 판정 |
|---|---|---|---|
| `MonitoringService.cpp` | 상태 문자열 — `static const std::string` 상수 사용 | 충족 | |
| `MonitoringView.cpp` | 구분선 — `SEP`, `SEP_S` 상수 사용 | 충족 | |
| `MonitoringService.cpp` | `"RESERVED"` 등 리터럴 직접 비교 없음 | 없어야 함 | |

---

## 테스트 결과 기록표

| TC | 분류 | 테스트 항목 | 결과 | 비고 |
|---|---|---|---|---|
| TC-01-A | 실행 | 정상 실행 | | |
| TC-01-B | 실행 | 모니터링 메뉴 진입 | | |
| TC-01-C | 실행 | 뒤로가기 처리 | | |
| TC-02-A | JSON 읽기 | orders.json 정상 읽기 | | |
| TC-02-B | JSON 읽기 | samples.json 정상 읽기 | | |
| TC-02-C | JSON 읽기 | 재진입 시 최신 파일 반영 | | |
| TC-03 | 집계 | 상태별 주문 수 정확성 | | |
| TC-04-A | REJECTED | 집계 제외 확인 | | |
| TC-04-B | REJECTED | REJECTED만 있는 경우 | | |
| TC-05-A | 재고 | 시료 목록 전체 출력 | | |
| TC-05-B | 재고 | 시료 0건 처리 | | |
| TC-06-A | 상태 계산 | SURPLUS (여유) | | |
| TC-06-B | 상태 계산 | SHORTAGE (부족) | | |
| TC-06-C | 상태 계산 | DEPLETED (고갈) | | |
| TC-06-D | 상태 계산 | stock=0, demand=0 고갈 | | |
| TC-07-A | 누락 | orders.json 누락 | | |
| TC-07-B | 누락 | samples.json 누락 | | |
| TC-07-C | 누락 | 양쪽 파일 누락 | | |
| TC-08-A | 형식 오류 | orders.json 형식 오류 | | |
| TC-08-B | 형식 오류 | samples.json 형식 오류 | | |
| TC-08-C | 형식 오류 | 빈 파일 | | |
| TC-09-A | MVC | MonitoringService 계산 집중 | | |
| TC-09-B | MVC | MonitoringView 표시 전용 | | |
| TC-09-C | MVC | MonitoringController 흐름 전용 | | |
| TC-09-D | MVC | Repository 조회 전용 사용 | | |
| TC-09-E | MVC | 의존성 방향 검증 | | |
| TC-10-A | Clean Code | 네이밍 규칙 | | |
| TC-10-B | Clean Code | 헤더 가드 | | |
| TC-10-C | Clean Code | 주석 규칙 | | |
| TC-10-D | Clean Code | 금지 패턴 | | |
| TC-10-E | Clean Code | 함수 규모 | | |
| TC-10-F | Clean Code | 매직 스트링 / 매직 넘버 | | |

결과: `PASS` / `FAIL` / `SKIP`
