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
6. Dummy 데이터 생성
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
  └─ Controller → Generator → RandomUtil
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

## 11. Dummy 데이터 생성 검증

### 11-1. 데이터 생성 기본 흐름 검증

#### TC-11-A. Dummy 데이터 생성 메뉴 진입

**목적**: 메인 메뉴에서 `6`을 선택하면 Dummy 데이터 생성 서브메뉴로 진입하는지 확인한다.

**실행 순서**

1. 프로그램 실행
2. 메인 메뉴 → `6` 입력

**기대 결과**

```
[ Dummy 데이터 생성 ] 서브메뉴가 출력된다.
1. Sample 생성
2. Order 생성
3. ProductionLine 생성
4. 전체 생성
0. 돌아가기
```

**판정**: 서브메뉴 정상 출력 시 **PASS** / 메뉴 없음 또는 crash 시 **FAIL**

---

#### TC-11-B. 뒤로가기 처리

**목적**: Dummy 데이터 생성 서브메뉴에서 `0` 입력 시 메인 메뉴로 복귀하는지 확인한다.

**실행 순서**

1. Dummy 데이터 생성 서브메뉴 진입
2. `0` 입력

**기대 결과**: 메인 메뉴로 복귀, 프로그램 종료 없음

**판정**: 메인 메뉴 재출력 시 **PASS** / 프로그램 종료 시 **FAIL**

---

### 11-2. JSON 파일 생성 검증

#### TC-12-A. Sample JSON 파일 생성 확인

**목적**: Sample 생성 후 `data/samples.json`이 올바른 형식으로 저장되는지 확인한다.

**사전 조건**: `data/samples.json`을 삭제한다.

**실행 순서**

1. Dummy 데이터 생성 → `1. Sample 생성`
2. 수량 `10` 입력
3. 덮어쓰기 여부 `N` 입력 (Overwrite)
4. 생성 완료 후 `data/samples.json` 파일을 텍스트 편집기로 확인

**기대 결과**

- `data/samples.json` 파일이 생성된다.
- 파일 최상위에 `"nextId"` 키가 존재한다.
- `"items"` 배열에 10개의 객체가 존재한다.
- 각 객체에 `id`, `name`, `description`, `stock`, `avgProductionTime`, `yield` 필드가 존재한다.
- trailing comma가 없다.

**판정**: 위 조건 모두 충족 시 **PASS** / 파일 미생성 또는 형식 불일치 시 **FAIL**

---

#### TC-12-B. Order JSON 파일 생성 확인

**목적**: Order 생성 후 `data/orders.json`이 올바른 형식으로 저장되는지 확인한다.

**사전 조건**: Sample 데이터 존재 상태. `data/orders.json`을 삭제한다.

**실행 순서**

1. Dummy 데이터 생성 → `2. Order 생성`
2. 수량 `50` 입력
3. 덮어쓰기 여부 `N` 입력 (Overwrite)
4. `data/orders.json` 파일 확인

**기대 결과**

- `data/orders.json` 파일이 생성된다.
- `"items"` 배열에 50개 객체가 존재한다.
- 각 객체에 `id`, `sampleId`, `productName`, `customerName`, `quantity`, `status` 필드가 존재한다.
- `sampleId` 값이 실제 존재하는 Sample ID 범위 내에 있다.

**판정**: 위 조건 모두 충족 시 **PASS**

---

#### TC-12-C. ProductionLine JSON 파일 생성 확인

**목적**: ProductionLine 생성 후 `data/production_lines.json`이 올바른 형식으로 저장되는지 확인한다.

**사전 조건**: Sample, Order 데이터 존재 상태.

**실행 순서**

1. Dummy 데이터 생성 → `3. ProductionLine 생성`
2. 수량 `30` 입력
3. 덮어쓰기 여부 `N` 입력 (Overwrite)
4. `data/production_lines.json` 파일 확인

**기대 결과**

- `data/production_lines.json` 파일이 생성된다.
- `"items"` 배열에 30개 객체가 존재한다.
- 각 객체에 `id`, `orderId`, `lineName`, `status`, `progress` 필드가 존재한다.
- `status`가 `DONE`인 항목의 `progress` 값이 100이다.

**판정**: 위 조건 모두 충족 시 **PASS**

---

### 11-3. 데이터 개수 검증

#### TC-13-A. 생성 수량 정확성 — Sample

**목적**: 입력한 수량과 실제 생성된 항목 수가 일치하는지 확인한다.

**실행 순서**

1. Overwrite 모드로 Sample `100`건 생성
2. `data/samples.json` 열기
3. `"items"` 배열의 항목 수를 직접 센다

**기대 결과**: `"items"` 배열 내 객체 수 = 100

**판정**: 정확히 100개이면 **PASS** / 다르면 **FAIL**

---

#### TC-13-B. 생성 수량 정확성 — Order

**목적**: Order 1,000건 생성 시 정확히 1,000개가 저장되는지 확인한다.

**실행 순서**

1. Sample 존재 상태에서 Overwrite 모드로 Order `1000`건 생성
2. `data/orders.json`의 `"nextId"` 값 확인
3. `"items"` 배열 항목 수 확인

**기대 결과**

- `"items"` 배열 항목 수 = 1,000
- `"nextId"` = 1,001

**판정**: 수량과 nextId 모두 정확하면 **PASS**

---

#### TC-13-C. nextId 연속성 검증 (Append 모드)

**목적**: Append 모드 반복 실행 시 `nextId`가 누락 없이 연속으로 증가하는지 확인한다.

**실행 순서**

1. Overwrite 모드로 Sample `10`건 생성 (nextId = 11)
2. Append 모드로 Sample `5`건 추가 생성
3. `data/samples.json` 확인

**기대 결과**

- 총 항목 수 = 15
- `"nextId"` = 16
- 11번째 항목의 `id` = 11, 15번째 항목의 `id` = 15 (중복 없음)

**판정**: 항목 수, nextId, id 연속성 모두 충족 시 **PASS**

---

#### TC-13-D. 최솟값 / 최댓값 경계 입력

**목적**: 수량 입력 경계값(1, 최대치)에서 정상 동작하는지 확인한다.

**실행 순서 (최솟값)**

1. Overwrite 모드로 Sample `1`건 생성
2. `"items"` 항목 수 확인

**기대 결과**: `"items"` = 1개

**실행 순서 (최댓값)**

1. Overwrite 모드로 Sample `1000`건 생성
2. `"items"` 항목 수 확인

**기대 결과**: `"items"` = 1,000개, crash 없음

**판정**: 양쪽 모두 정상이면 **PASS** / crash 또는 수량 불일치 시 **FAIL**

---

### 11-4. 랜덤 상태값 검증

#### TC-14-A. Order Status 분포 확인

**목적**: Order 1,000건 생성 시 Status 분포가 가중치 정책(±10%p)에 부합하는지 확인한다.

**실행 순서**

1. Overwrite 모드로 Order `1000`건 생성
2. `data/orders.json`에서 각 status 값의 개수를 수동으로 집계한다.

**기대 결과 (허용 범위)**

| 상태 | 목표 비율 | 허용 범위 |
|------|------|------|
| RESERVED | 30% | 200 ~ 400건 |
| PRODUCING | 25% | 150 ~ 350건 |
| CONFIRMED | 20% | 100 ~ 300건 |
| RELEASE | 15% | 50 ~ 250건 |
| REJECTED | 10% | 0 ~ 200건 |

**판정**: 모든 상태가 허용 범위 내이면 **PASS** / 한 상태라도 범위 초과 시 **FAIL**

---

#### TC-14-B. ProductionLine DONE 상태 progress 고정값 확인

**목적**: `status`가 `DONE`인 ProductionLine의 `progress`가 100으로 고정되는지 확인한다.

**실행 순서**

1. Overwrite 모드로 ProductionLine `200`건 생성
2. `data/production_lines.json`에서 `"status": "DONE"` 항목을 찾는다.
3. 해당 항목의 `progress` 값을 확인한다.

**기대 결과**: `"status": "DONE"`인 모든 항목의 `"progress"` = 100

**판정**: DONE인 항목 전부 progress=100이면 **PASS** / 하나라도 100이 아니면 **FAIL**

---

#### TC-14-C. Sample yield 범위 확인

**목적**: Sample의 `yield` 값이 정의된 범위(60.0 ~ 99.9) 내에 있는지 확인한다.

**실행 순서**

1. Overwrite 모드로 Sample `100`건 생성
2. `data/samples.json`에서 `yield` 값 전체를 검토한다.

**기대 결과**: 모든 `yield` 값이 60.0 이상 99.9 이하

**판정**: 범위 이탈 없으면 **PASS** / 범위 이탈 항목 존재 시 **FAIL**

---

#### TC-14-D. 동일 시드에서 다른 실행 결과 확인

**목적**: 매 실행마다 다른 랜덤 데이터가 생성되는지 확인한다.

**실행 순서**

1. Overwrite 모드로 Sample `10`건 생성 → `samples.json` 내용 복사 보관
2. 동일 수량으로 재실행 (Overwrite 모드)
3. 두 결과를 비교한다.

**기대 결과**: 두 결과가 완전히 동일하지 않음 (최소 1개 이상 필드 값 다름)

**판정**: 다른 결과이면 **PASS** / 완전히 동일하면 **FAIL**

---

### 11-5. 파일 Overwrite 검증

#### TC-15-A. Append 모드 — 기존 데이터 보존 확인

**목적**: Append 모드에서 기존 데이터가 손실되지 않는지 확인한다.

**실행 순서**

1. Seed 데이터 상태 (Sample 3건, `nextId`: 4)
2. Append 모드로 Sample `5`건 추가 생성
3. `data/samples.json` 확인

**기대 결과**

- 기존 Sample (id: 1, 2, 3) 데이터가 그대로 존재한다.
- 새 Sample (id: 4, 5, 6, 7, 8) 5건이 추가된다.
- 총 항목 수 = 8
- `"nextId"` = 9

**판정**: 기존 데이터 보존 + 새 데이터 추가 확인 시 **PASS** / 기존 데이터 손실 시 **FAIL**

---

#### TC-15-B. Overwrite 모드 — 기존 데이터 완전 교체 확인

**목적**: Overwrite 모드에서 기존 데이터가 완전히 교체되는지 확인한다.

**실행 순서**

1. Seed 데이터 상태 (Sample 3건, `nextId`: 4)
2. Overwrite 모드 선택 → 확인 메시지에 `Y` 입력
3. Sample `10`건 생성
4. `data/samples.json` 확인

**기대 결과**

- 기존 Sample (id: 1, 2, 3) 데이터가 존재하지 않는다.
- `"items"` 배열에 새 데이터 10건만 존재한다.
- `"nextId"` = 11
- 새 데이터의 id가 1부터 시작한다.

**판정**: 위 조건 모두 충족 시 **PASS** / 기존 데이터 잔존 시 **FAIL**

---

#### TC-15-C. Overwrite 확인 메시지 `N` 입력 시 취소 확인

**목적**: Overwrite 모드에서 사용자가 `N`을 입력하면 생성이 취소되는지 확인한다.

**실행 순서**

1. Seed 데이터 상태 (Sample 3건)
2. Overwrite 모드 선택 → 확인 메시지에 `N` 입력

**기대 결과**

- 생성이 중단된다.
- `data/samples.json`의 내용이 변경되지 않는다. (여전히 3건)

**판정**: 파일 변경 없으면 **PASS** / 파일 변경되거나 crash 시 **FAIL**

---

### 11-6. 의존성 보호 검증

#### TC-16-A. Sample 없을 때 Order 생성 시도

**목적**: Sample 데이터가 없는 상태에서 Order 생성을 시도하면 안내 메시지가 출력되고 중단되는지 확인한다.

**사전 조건**: `data/samples.json`을 `{ "nextId": 1, "items": [] }` 로 교체한다.

**실행 순서**

1. Dummy 데이터 생성 → `2. Order 생성`

**기대 결과**

- `"먼저 Sample 데이터를 생성하세요."` 안내 메시지 출력
- Order 생성이 중단된다.
- `data/orders.json` 파일이 변경되지 않는다.

**판정**: 안내 메시지 출력 + 생성 중단 시 **PASS** / 비정상 Order 생성 또는 crash 시 **FAIL**

---

#### TC-16-B. Order 없을 때 ProductionLine 생성 시도

**목적**: Order 데이터가 없는 상태에서 ProductionLine 생성을 시도하면 안내 메시지가 출력되고 중단되는지 확인한다.

**사전 조건**: `data/orders.json`을 `{ "nextId": 1, "items": [] }` 로 교체한다.

**실행 순서**

1. Dummy 데이터 생성 → `3. ProductionLine 생성`

**기대 결과**

- `"먼저 Order 데이터를 생성하세요."` 안내 메시지 출력
- ProductionLine 생성이 중단된다.

**판정**: 안내 메시지 출력 + 생성 중단 시 **PASS** / 비정상 생성 또는 crash 시 **FAIL**

---

#### TC-16-C. 전체 생성 — 의존성 순서 준수

**목적**: `4. 전체 생성` 선택 시 Sample → Order → ProductionLine 순서로 생성되는지 확인한다.

**사전 조건**: 기존 데이터 파일을 모두 삭제한다.

**실행 순서**

1. Dummy 데이터 생성 → `4. 전체 생성`
2. 수량 입력 (예: Sample 10, Order 50, ProductionLine 50)
3. Overwrite 모드 선택

**기대 결과**

- `data/samples.json` → `data/orders.json` → `data/production_lines.json` 순서로 파일이 생성된다.
- Order의 `sampleId`가 생성된 Sample ID 범위 (1 ~ 10) 내에 있다.
- ProductionLine의 `orderId`가 생성된 Order ID 범위 (1 ~ 50) 내에 있다.

**판정**: 순서 준수 + 참조 ID 유효 시 **PASS** / 순서 위반 또는 참조 ID 범위 초과 시 **FAIL**

---

### 11-7. 수량 입력 오류 처리 검증

#### TC-17-A. 0 이하 수량 입력

**목적**: 0 또는 음수 입력 시 재입력을 요청하는지 확인한다.

**실행 순서**

1. Dummy 데이터 생성 → `1. Sample 생성`
2. 수량 `0` 입력

**기대 결과**: `"1 이상의 수를 입력하세요."` 출력 후 재입력 대기

**판정**: 재입력 요청 시 **PASS** / 생성 진행 또는 crash 시 **FAIL**

---

#### TC-17-B. 최대치 초과 수량 입력

**목적**: 최대치를 초과한 수량 입력 시 재입력을 요청하는지 확인한다.

**실행 순서**

1. Sample 생성 → 수량 `1001` 입력

**기대 결과**: `"최대 1000건까지 가능합니다."` 출력 후 재입력 대기

**판정**: 재입력 요청 시 **PASS** / 생성 진행 또는 crash 시 **FAIL**

---

#### TC-17-C. 숫자가 아닌 입력

**목적**: 문자열 입력 시 재입력을 요청하는지 확인한다.

**실행 순서**

1. Sample 생성 → 수량 `abc` 입력

**기대 결과**: `"숫자를 입력하세요."` 출력 후 재입력 대기

**판정**: 재입력 요청 시 **PASS** / crash 또는 비정상 값 처리 시 **FAIL**

---

## 테스트 결과 기록표

### DataMonitor 기능 테스트

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

### Dummy 데이터 생성 테스트

| TC | 분류 | 테스트 항목 | 결과 | 비고 |
|---|---|---|---|---|
| TC-11-A | 메뉴 | Dummy 데이터 생성 메뉴 진입 | | |
| TC-11-B | 메뉴 | 뒤로가기 처리 | | |
| TC-12-A | JSON 생성 | Sample JSON 파일 생성 확인 | | |
| TC-12-B | JSON 생성 | Order JSON 파일 생성 확인 | | |
| TC-12-C | JSON 생성 | ProductionLine JSON 파일 생성 확인 | | |
| TC-13-A | 개수 검증 | Sample 생성 수량 정확성 | | |
| TC-13-B | 개수 검증 | Order 1,000건 생성 수량 정확성 | | |
| TC-13-C | 개수 검증 | nextId 연속성 (Append 모드) | | |
| TC-13-D | 개수 검증 | 최솟값 / 최댓값 경계 입력 | | |
| TC-14-A | 랜덤 검증 | Order Status 분포 확인 | | |
| TC-14-B | 랜덤 검증 | DONE status progress=100 고정 | | |
| TC-14-C | 랜덤 검증 | Sample yield 범위 확인 | | |
| TC-14-D | 랜덤 검증 | 매 실행마다 다른 결과 확인 | | |
| TC-15-A | Overwrite | Append 모드 기존 데이터 보존 | | |
| TC-15-B | Overwrite | Overwrite 모드 완전 교체 | | |
| TC-15-C | Overwrite | Overwrite 확인 N 입력 시 취소 | | |
| TC-16-A | 의존성 | Sample 없이 Order 생성 시도 | | |
| TC-16-B | 의존성 | Order 없이 ProductionLine 생성 시도 | | |
| TC-16-C | 의존성 | 전체 생성 의존성 순서 준수 | | |
| TC-17-A | 입력 오류 | 0 이하 수량 입력 | | |
| TC-17-B | 입력 오류 | 최대치 초과 수량 입력 | | |
| TC-17-C | 입력 오류 | 숫자가 아닌 입력 | | |

결과: `PASS` / `FAIL` / `SKIP`
