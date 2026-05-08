# DummyDataGenerator — 수동 테스트 시나리오

DummyDataGenerator PoC의 기능 검증 및 설계 준수 여부를 확인하는 수동 테스트 문서입니다.  
자동화 프레임워크를 사용하지 않으며, 직접 실행·코드 리뷰를 통해 판정합니다.

---

## 공통 사전 조건

- Visual Studio 2022 빌드 완료 상태 (`DummyDataGenerator.vcxproj` 기준)
- 각 TC 시작 전 `data/` 폴더의 JSON 파일을 Seed 원본으로 복구
- 결과 표기: `PASS` / `FAIL` / `SKIP`

### Seed 데이터 — DataMonitor 섹션용 (TC-01~TC-10)

**data/samples.json** (nextId: 4)

| id | name | stock |
|---|---|---|
| 1 | 시료-A형 | 10 |
| 2 | 시료-B형 | 3 |
| 3 | 시료-C형 | 0 |

**data/orders.json** (nextId: 13)

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

### Seed 데이터 — DummyDataGenerator 섹션용 (TC-11~TC-19)

- `data/samples.json` : `{ "nextId": 1, "items": [] }`
- `data/orders.json` : `{ "nextId": 1, "items": [] }`
- `data/production_lines.json` : 파일 없음 (또는 동일 빈 구조)

---

## 1. 프로그램 실행 테스트 (DataMonitor)

### TC-01-A. 정상 실행

**목적**: Seed 데이터가 있을 때 프로그램이 정상 시작되는지 확인한다.

**실행 순서**

1. DummyDataGenerator.exe 실행
2. 최상위 메뉴 출력 확인

**기대 결과**

```
========================================
       DummyDataGenerator
========================================
 1. DataMonitor (기존 시스템)
 2. Dummy 데이터 생성
 0. 종료
========================================
```

**판정**: 메뉴가 출력되고 입력 대기 상태이면 **PASS**

---

### TC-01-B. DataMonitor 메뉴 진입

**목적**: 최상위 메뉴에서 `1`을 선택하면 DataMonitor 서브메뉴로 진입하는지 확인한다.

**실행 순서**

1. 프로그램 실행
2. 최상위 메뉴 → `1` 입력

**기대 결과**: DataMonitor 메인 메뉴 출력 (시료 관리, 주문 관리, 생산 라인 관리, 모니터링, 출하 관리, 종료)

**판정**: DataMonitor 메뉴가 정상 출력되면 **PASS**

---

### TC-01-C. 모니터링 메뉴 진입

**목적**: DataMonitor에서 모니터링 서브메뉴로 정상 진입하는지 확인한다.

**실행 순서**

1. 최상위 메뉴 → `1` → DataMonitor 메인 메뉴 → `4` 입력

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

### TC-01-D. 뒤로가기 처리

**목적**: 서브메뉴에서 `0` 입력 시 상위 메뉴로 복귀하는지 확인한다.

**실행 순서**: 모니터링 서브메뉴 → `0` → DataMonitor 메인 메뉴 → `0` → 최상위 메뉴

**기대 결과**: 각 단계에서 crash 없이 상위 메뉴로 복귀

**판정**: 최상위 메뉴까지 정상 복귀하면 **PASS**

---

## 2. JSON 파일 읽기 테스트 (DataMonitor)

### TC-02-A. orders.json 정상 읽기

**실행 순서**: DataMonitor → 모니터링 → `2. 주문 상태별 집계`

**기대 결과**: 0이 아닌 주문 건수 표시

**판정**: 집계 결과 표시 시 **PASS** / 모든 값 0이면 **FAIL**

---

### TC-02-B. samples.json 정상 읽기

**실행 순서**: DataMonitor → 모니터링 → `3. 시료 재고 현황`

**기대 결과**: 시료-A형, 시료-B형, 시료-C형 목록 표시

**판정**: 3개 시료 출력 시 **PASS**

---

### TC-02-C. 메뉴 재진입 시 최신 파일 반영

**목적**: 외부에서 파일을 수정한 후 재진입 시 변경 사항이 반영되는지 확인한다.

**실행 순서**

1. 모니터링 → 주문 집계 확인 → `0` 뒤로가기
2. `orders.json`에 주문 1건 추가 (외부 편집기)
3. 모니터링 → 주문 집계 재확인

**기대 결과**: 건수가 1 증가

**판정**: 변경 반영 시 **PASS**

---

## 3. 상태별 주문 수 집계 테스트 (DataMonitor)

### TC-03. 주문 상태별 집계 정확성

**실행 순서**: DataMonitor → 모니터링 → `2. 주문 상태별 집계` (Seed 데이터 기준)

**기대 결과**

| 상태 | 기대 건수 |
|---|---|
| RESERVED | 3건 |
| PRODUCING | 2건 |
| CONFIRMED | 2건 |
| RELEASE | 2건 |
| 합계 | 9건 |

**판정**: 모든 항목 일치 시 **PASS**

---

## 4. REJECTED 제외 검증 (DataMonitor)

### TC-04-A. REJECTED 집계 제외 확인

**기대 결과**: 화면에 REJECTED 없음, 합계 9건 (REJECTED 3건 제외)

**판정**: 합계 9 + REJECTED 미표시 시 **PASS**

---

### TC-04-B. REJECTED만 있는 경우

**사전 조건**: orders.json을 REJECTED 2건만 있는 파일로 교체

**기대 결과**: 모든 상태 0건, 합계 0건

**판정**: 합계 0 출력 시 **PASS**

---

## 5. 시료별 재고 현황 테스트 (DataMonitor)

### TC-05-A. 시료 목록 전체 출력

**기대 결과**

```
시료-A형 / A형 반도체 검사 시료 / 재고 10 / 상태 여유
시료-B형 / B형 반도체 검사 시료 / 재고  3 / 상태 부족
시료-C형 / C형 반도체 검사 시료 / 재고  0 / 상태 고갈
```

**판정**: 순서·재고·상태 모두 정확 시 **PASS**

---

### TC-05-B. 시료 0건 처리

**사전 조건**: samples.json → `{ "nextId": 1, "items": [] }`

**기대 결과**: "등록된 시료가 없습니다." 출력, 프로그램 유지

**판정**: 안내 메시지 + 메뉴 복귀 시 **PASS**

---

## 6. 재고 상태 계산 테스트 (DataMonitor)

### TC-06-A. SURPLUS (여유) 확인

| 항목 | 값 |
|---|---|
| 시료-A형 stock | 10 |
| RESERVED+PRODUCING 수량 합계 | 7 |
| 판정 기준 | 10 > 7 → 여유 |

**판정**: `여유` 출력 시 **PASS**

---

### TC-06-B. SHORTAGE (부족) 확인

| 항목 | 값 |
|---|---|
| 시료-B형 stock | 3 |
| RESERVED+PRODUCING 수량 합계 | 5 |
| 판정 기준 | 0 < 3 ≤ 5 → 부족 |

**판정**: `부족` 출력 시 **PASS**

---

### TC-06-C. DEPLETED (고갈) 확인

| 항목 | 값 |
|---|---|
| 시료-C형 stock | 0 |
| 판정 기준 | stock == 0 → 고갈 (demand 무관) |

**판정**: `고갈` 출력 시 **PASS**

---

### TC-06-D. stock == 0, demand == 0

**사전 조건**: 시료-C형 주문 (id 3, 12) 모두 삭제

**기대 결과**: 시료-C형에 `고갈` 유지

**판정**: `고갈` 출력 시 **PASS**

---

## 7. JSON 파일 누락 테스트 (DataMonitor)

### TC-07-A. orders.json 누락

**사전 조건**: `data/orders.json` 삭제

**기대 결과**: crash 없음, 0건 표시, 안내 메시지 출력

**판정**: 안내 메시지 + 0건 + 메뉴 복귀 시 **PASS**

---

### TC-07-B. samples.json 누락

**사전 조건**: `data/samples.json` 삭제

**기대 결과**: crash 없음, 빈 목록 출력

**판정**: 안내 메시지 + 빈 목록 + 메뉴 복귀 시 **PASS**

---

### TC-07-C. 양쪽 파일 모두 누락

**기대 결과**: 주문 0건·시료 0건 Dashboard 출력, 프로그램 유지

**판정**: crash 없이 빈 Dashboard 출력 시 **PASS**

---

## 8. JSON 형식 오류 테스트 (DataMonitor)

### TC-08-A. orders.json 형식 오류

**사전 조건**: `data/orders.json`에 손상된 JSON 저장

**기대 결과**: `[오류]` 메시지 출력, `.bak` 파일 생성, 0건 표시

**판정**: `.bak` 생성 + 0건 + 메뉴 복귀 시 **PASS**

---

### TC-08-B. samples.json 형식 오류

**사전 조건**: `data/samples.json`에 손상된 JSON 저장

**기대 결과**: `[오류]` 메시지, `.bak` 생성, 빈 목록

**판정**: `.bak` 생성 + 빈 목록 + 메뉴 복귀 시 **PASS**

---

### TC-08-C. 빈 파일

**사전 조건**: `data/orders.json`을 0 bytes 파일로 교체

**기대 결과**: 오류 없이 0건 처리

**판정**: 0건 + 메뉴 복귀 시 **PASS**

---

## 9. MVC 역할 분리 검증 (DataMonitor — 코드 리뷰)

### TC-09-A. MonitoringService — 계산 로직 집중

**검증 위치**: `src/services/MonitoringService.cpp`

| 점검 항목 | 기대 상태 | 판정 |
|---|---|---|
| `getOrderSummary()`에 상태별 카운팅 로직 | 존재 | |
| `getSampleStatuses()`에 demand 계산 로직 | 존재 | |
| `std::cout` / `std::cin` 호출 없음 | 없어야 함 | |
| Repository 통해서만 데이터 접근 | 충족 | |

---

### TC-09-B. 의존성 방향 검증

| 검증 항목 | 기대 상태 | 판정 |
|---|---|---|
| View가 Service·Repository include 안 함 | 없어야 함 | |
| Service가 View include 안 함 | 없어야 함 | |
| Repository가 Controller include 안 함 | 없어야 함 | |

**허용 방향**: `Controller → Service → Repository → JsonFileStorage`

---

## 10. Clean Code 점검 (DataMonitor — 코드 리뷰)

### TC-10-A. 네이밍 규칙

| 대상 | 규칙 | 판정 |
|---|---|---|
| 클래스명 | PascalCase | |
| 메서드명 | camelCase | |
| 멤버 변수 | `m_` 접두사 | |

### TC-10-B. 금지 패턴

| 점검 항목 | 기대 상태 | 판정 |
|---|---|---|
| `using namespace std;` | 없어야 함 | |
| 전역 변수 | 없어야 함 | |
| `exit()` / `abort()` | 없어야 함 | |
| `fstream` 직접 사용 — `JsonFileStorage.cpp` 이외 | 없어야 함 | |

---

## 11. Dummy 데이터 생성 테스트

> 사전 조건: 모든 TC에서 DummyDataGenerator Seed(`{ "nextId": 1, "items": [] }`)를 사용한다.

### TC-11-A. 최상위 메뉴 — Dummy 데이터 생성 진입

**목적**: 최상위 메뉴에서 `2`를 선택하면 Dummy 데이터 생성 서브메뉴로 진입하는지 확인한다.

**실행 순서**

1. DummyDataGenerator.exe 실행
2. 최상위 메뉴 → `2` 입력

**기대 결과**

```
========================================
       Dummy 데이터 생성
========================================
 1. Sample Dummy 생성
 2. Order Dummy 생성
 3. ProductionLine Dummy 생성
 4. 전체 Dummy 생성
 0. 돌아가기
========================================
```

**판정**: 서브메뉴 정상 출력 시 **PASS** / 메뉴 없음 또는 crash 시 **FAIL**

---

### TC-11-B. Sample 단독 생성 기본 흐름

**목적**: 메뉴 `1` → 수량 입력 → 저장 완료까지 전체 흐름을 확인한다.

**실행 순서**

1. Dummy 데이터 생성 → `1`
2. 수량 `10` 입력
3. Overwrite 여부 `N` 입력
4. 완료 메시지 확인
5. `data/samples.json` 내용 확인

**기대 결과**

- 콘솔: `[완료] Sample 10건 생성` + 저장 파일 경로 출력
- `data/samples.json` 파일이 생성되거나 갱신된다
- `"items"` 배열에 10개 객체가 존재한다

**판정**: 완료 메시지 + 파일 생성 + 10건 확인 시 **PASS**

---

### TC-11-C. Order 단독 생성 기본 흐름

**사전 조건**: Sample 10건이 이미 존재하는 상태

**실행 순서**

1. Dummy 데이터 생성 → `2`
2. 수량 `20` 입력
3. Overwrite 여부 `N` 입력

**기대 결과**

- `data/orders.json`에 20건 존재
- 각 Order의 `productName`이 기존 Sample 중 하나의 `name`과 일치

**판정**: 20건 확인 + productName 유효 시 **PASS**

---

### TC-11-D. ProductionLine 단독 생성 기본 흐름

**사전 조건**: Sample 10건, Order 20건이 이미 존재하는 상태

**실행 순서**

1. Dummy 데이터 생성 → `3`
2. 수량 `15` 입력
3. Overwrite 여부 `N` 입력

**기대 결과**: `data/production_lines.json`에 15건 존재

**판정**: 15건 확인 + 완료 메시지 시 **PASS**

---

### TC-11-E. 전체 생성 (4번 메뉴) 기본 흐름

**사전 조건**: 모든 데이터 파일 비어있는 상태

**실행 순서**

1. Dummy 데이터 생성 → `4`
2. Sample 수량 `5` 입력
3. Order 수량 `10` 입력
4. ProductionLine 수량 `8` 입력
5. Overwrite 여부 `N` 입력

**기대 결과**

- `data/samples.json`: 5건
- `data/orders.json`: 10건
- `data/production_lines.json`: 8건
- 완료 메시지 3건 순서대로 출력

**판정**: 3개 파일 모두 지정 건수 존재 시 **PASS**

---

### TC-11-F. 돌아가기 처리

**실행 순서**: Dummy 데이터 생성 서브메뉴 → `0`

**기대 결과**: 최상위 메뉴로 복귀, crash 없음

**판정**: 최상위 메뉴 재출력 시 **PASS**

---

## 12. JSON 저장 테스트

### TC-12-A. samples.json 형식 검증

**실행 순서**: Sample `5`건 Overwrite 생성 후 텍스트 편집기로 파일 확인

**기대 결과**

```json
{
  "nextId": 6,
  "items": [
    { "id": 1, "name": "플랜-XX샘", "description": "...", "stock": ..., "avgProductionTime": ..., "yield": ... },
    ...
  ]
}
```

| 점검 항목 | 기대 상태 | 판정 |
|---|---|---|
| 최상위에 `"nextId"` 키 존재 | 존재 | |
| 최상위에 `"items"` 배열 존재 | 존재 | |
| `nextId` 값 = 생성 건수 + 1 | 6 | |
| 배열 항목 수 = 5 | 5 | |
| 각 항목에 6개 필드 모두 존재 | 존재 | |
| 마지막 항목 뒤 trailing comma 없음 | 없어야 함 | |
| UTF-8 BOM 포함 (`EF BB BF`) | 존재 | |

**판정**: 모든 항목 충족 시 **PASS**

---

### TC-12-B. orders.json 형식 검증

**사전 조건**: Sample 5건 생성 후 Order `10`건 Overwrite 생성

**점검 항목**

| 항목 | 기대 상태 | 판정 |
|---|---|---|
| `"nextId"` = 11 | 11 | |
| 배열 항목 수 = 10 | 10 | |
| 각 항목 필드: `id`, `sampleId`, `productName`, `customerName`, `quantity`, `status` | 모두 존재 | |
| `sampleId` 범위: 1 ≤ sampleId ≤ 5 | 충족 | |
| `productName`이 해당 sampleId의 Sample.name과 일치 | 충족 | |

**판정**: 모든 항목 충족 시 **PASS**

---

### TC-12-C. production_lines.json 형식 검증

**사전 조건**: Sample 5건, Order 10건 생성 후 ProductionLine `8`건 Overwrite 생성

**점검 항목**

| 항목 | 기대 상태 | 판정 |
|---|---|---|
| `"nextId"` = 9 | 9 | |
| 배열 항목 수 = 8 | 8 | |
| 각 항목 필드: `id`, `orderId`, `lineName`, `status`, `progress` | 모두 존재 | |
| `orderId` 범위: 1 ≤ orderId ≤ 10 | 충족 | |
| `status`가 IDLE·RUNNING·PAUSED·DONE 중 하나 | 충족 | |

**판정**: 모든 항목 충족 시 **PASS**

---

### TC-12-D. 데이터 없는 경우 파일 형식

**실행 순서**: 수량 `1` → 모든 데이터 파일을 전부 Overwrite로 생성 후 수량 `0`으로 다시 시도 (수량은 1 이상이어야 하므로 이 TC는 직접 파일 편집으로 확인)

**검증 방법**: 코드 리뷰로 `buildJson("")` 결과 확인

**기대 결과**: `"items": []` (trailing comma 없음)

**판정**: 코드에서 빈 배열 처리 확인 시 **PASS**

---

### TC-12-E. nextId 연속성 검증

**실행 순서**

1. Overwrite로 Sample `3`건 생성 → `"nextId"` 확인 (기대: 4)
2. Append로 Sample `2`건 추가 생성 → `"nextId"` 확인 (기대: 6)

**기대 결과**

- 1차 후: nextId = 4, id 1·2·3
- 2차 후: nextId = 6, id 1·2·3·4·5 (중복 없음)

**판정**: nextId 연속성 유지 + 중복 없음 시 **PASS**

---

## 13. Overwrite 테스트

### TC-13-A. Append 모드 — 기존 데이터 보존

**목적**: `N` 입력 시 기존 데이터가 삭제되지 않는지 확인한다.

**실행 순서**

1. Overwrite로 Sample `3`건 생성 (nextId = 4)
2. Append(`N`)로 Sample `2`건 추가 생성

**기대 결과**

- 총 항목 수 = 5
- nextId = 6
- 기존 id 1·2·3 데이터가 그대로 존재
- 새 id 4·5 데이터가 추가됨

**판정**: 기존 데이터 보존 + 새 데이터 추가 확인 시 **PASS** / 기존 데이터 손실 시 **FAIL**

---

### TC-13-B. Overwrite 모드 — 완전 교체

**목적**: `Y` 입력 시 기존 데이터가 모두 교체되는지 확인한다.

**실행 순서**

1. Overwrite로 Sample `3`건 생성 (id 1·2·3, nextId = 4)
2. 다시 Overwrite(`Y`)로 Sample `5`건 생성

**기대 결과**

- 총 항목 수 = 5
- nextId = 6
- 기존 id 1·2·3이 존재하지 않음
- 새 id 1·2·3·4·5만 존재

**판정**: 기존 데이터 교체 + 새 데이터만 존재 시 **PASS** / 기존 데이터 잔존 시 **FAIL**

---

### TC-13-C. Overwrite 입력 취소 처리

**목적**: Overwrite 여부 입력에서 `N`을 입력하면 기존 데이터가 변경되지 않는지 확인한다.

**실행 순서**

1. Overwrite로 Sample `3`건 생성
2. 다시 생성 시도 → 수량 `10` 입력 → Overwrite 여부 `N` 입력

**기대 결과**: 기존 3건 + 새 10건 = 13건 (Append 모드로 처리)

**판정**: 총 13건 확인 시 **PASS**

---

### TC-13-D. Overwrite 경고 메시지 확인

**목적**: `Y` 입력 시 경고 메시지가 출력되는지 확인한다.

**실행 순서**: 수량 입력 → Overwrite 여부 `Y` 입력

**기대 결과**: `[주의] 기존 데이터가 모두 삭제됩니다.` 출력

**판정**: 경고 메시지 출력 후 생성 진행 시 **PASS**

---

### TC-13-E. Append 반복 — nextId 단조 증가

**목적**: Append 모드를 여러 번 반복해도 nextId가 항상 단조 증가하는지 확인한다.

**실행 순서**

1. Overwrite로 Sample `2`건 생성 (nextId = 3)
2. Append로 `2`건 추가 (nextId = 5)
3. Append로 `2`건 추가 (nextId = 7)

**기대 결과**

- 총 6건, nextId = 7
- id 1·2·3·4·5·6 (중복·갭 없음)

**판정**: 모든 id 연속 + nextId 정확 시 **PASS**

---

## 14. 랜덤 상태값 생성 테스트

### TC-14-A. Order status 가중치 분포 확인

**목적**: 1,000건 생성 시 status 분포가 가중치 정책 범위 내에 있는지 확인한다.

**실행 순서**

1. Sample `10`건 Overwrite 생성
2. Order `1000`건 Overwrite 생성
3. `data/orders.json`에서 각 status 건수를 수동 집계

**기대 결과 (허용 범위 ±10%p)**

| 상태 | 목표 비율 | 허용 범위 |
|---|---|---|
| RESERVED | 30% | 200 ~ 400건 |
| PRODUCING | 25% | 150 ~ 350건 |
| CONFIRMED | 20% | 100 ~ 300건 |
| RELEASE | 15% | 50 ~ 250건 |
| REJECTED | 10% | 0 ~ 200건 |

**판정**: 모든 상태가 허용 범위 내 시 **PASS**

---

### TC-14-B. Sample yield 범위 확인

**목적**: 생성된 Sample의 `yield` 값이 60.0~99.9 범위에 있는지 확인한다.

**실행 순서**: Sample `100`건 Overwrite 생성 → `yield` 값 전체 검토

**기대 결과**

- 모든 `yield` 값 ≥ 60.0
- 모든 `yield` 값 ≤ 99.9
- 소수점 1자리 (예: 72.5, 88.3)

**판정**: 범위 이탈 없음 시 **PASS**

---

### TC-14-C. Sample stock 범위 확인

**실행 순서**: Sample `100`건 Overwrite 생성 → `stock` 값 전체 검토

**기대 결과**: 모든 `stock` 값 0 ≤ stock ≤ 50

**판정**: 범위 이탈 없음 시 **PASS**

---

### TC-14-D. Sample avgProductionTime 범위 확인

**실행 순서**: Sample `100`건 Overwrite 생성 → `avgProductionTime` 값 전체 검토

**기대 결과**: 모든 값 2 ≤ avgProductionTime ≤ 12

**판정**: 범위 이탈 없음 시 **PASS**

---

### TC-14-E. ProductionLine DONE → progress 고정 확인

**목적**: `status`가 `DONE`인 항목의 `progress`가 100으로 고정되는지 확인한다.

**실행 순서**

1. Sample·Order 각 10건 생성
2. ProductionLine `200`건 Overwrite 생성
3. `data/production_lines.json`에서 `"status": "DONE"` 항목 모두 확인

**기대 결과**: `"status": "DONE"`인 모든 항목의 `"progress"` = 100

**판정**: DONE 항목 전부 progress=100 시 **PASS** / 하나라도 100이 아니면 **FAIL**

---

### TC-14-F. ProductionLine 비-DONE → progress 범위 확인

**실행 순서**: TC-14-E와 동일 설정

**기대 결과**: `"status"` ≠ `"DONE"`인 항목의 `"progress"` 값이 0~99 범위

**판정**: DONE이 아닌 항목의 progress가 모두 0~99 내 시 **PASS**

---

### TC-14-G. 매 실행마다 다른 결과 확인

**목적**: 동일 수량으로 재실행 시 다른 데이터가 생성되는지 확인한다.

**실행 순서**

1. Sample `10`건 Overwrite 생성 → 결과 복사 보관
2. Sample `10`건 Overwrite 재생성 → 두 결과 비교

**기대 결과**: 두 결과가 완전히 동일하지 않음 (최소 1개 이상 필드 값 다름)

**판정**: 다른 결과 시 **PASS** / 완전히 동일 시 **FAIL**

---

### TC-14-H. Order quantity 범위 확인

**실행 순서**: Order `100`건 생성 후 `quantity` 값 검토

**기대 결과**: 모든 값 1 ≤ quantity ≤ 20

**판정**: 범위 이탈 없음 시 **PASS**

---

## 15. 대량 데이터 생성 테스트

### TC-15-A. Sample 최대치(1,000건) 생성

**목적**: 허용 최대치에서 crash 없이 생성·저장되는지 확인한다.

**실행 순서**: Sample `1000`건 Overwrite 생성

**기대 결과**

- 생성 완료 메시지 출력
- `data/samples.json`의 `"items"` 배열 = 1,000개
- `"nextId"` = 1001
- 파일 생성 성공

**판정**: crash 없음 + 1,000건 확인 시 **PASS**

---

### TC-15-B. Order 최대치(10,000건) 생성

**사전 조건**: Sample 100건 존재

**실행 순서**: Order `10000`건 Overwrite 생성

**기대 결과**

- `data/orders.json`의 `"items"` = 10,000개
- `"nextId"` = 10001
- 생성~저장 시간 10초 이내 (참고 기준)

**판정**: crash 없음 + 10,000건 확인 시 **PASS**

---

### TC-15-C. ProductionLine 최대치(10,000건) 생성

**사전 조건**: Sample 100건, Order 10,000건 존재

**실행 순서**: ProductionLine `10000`건 Overwrite 생성

**기대 결과**: `data/production_lines.json`의 `"items"` = 10,000개

**판정**: crash 없음 + 10,000건 확인 시 **PASS**

---

### TC-15-D. 전체 대량 생성

**목적**: 세 데이터 유형을 대량으로 순서대로 생성했을 때 전체 흐름이 안정적인지 확인한다.

**실행 순서**: 전체 생성 → Sample `500` / Order `5000` / ProductionLine `5000` / Overwrite `Y`

**기대 결과**

- 세 파일 모두 정상 생성
- 생성 완료 메시지 3건 모두 출력
- DataMonitor에서 5,000건 Order 집계 정상 동작 (TC-17과 연계)

**판정**: 세 파일 모두 지정 건수 확인 시 **PASS**

---

### TC-15-E. 대량 Append 반복 — 파일 무결성

**목적**: Append를 여러 번 반복해도 파일이 깨지지 않는지 확인한다.

**실행 순서**

1. Overwrite로 Sample `100`건 생성
2. Append로 `100`건 추가 × 3회 반복
3. 최종 파일 확인

**기대 결과**

- 총 400건, nextId = 401
- JSON 구조 이상 없음 (편집기 또는 파서로 확인)

**판정**: 총 400건 + 정상 JSON 구조 시 **PASS**

---

## 16. 잘못된 입력 테스트

### TC-16-A. 수량 0 입력

**실행 순서**: Sample 생성 → 수량 `0` 입력

**기대 결과**: `[오류] 1 이상의 수를 입력하세요:` 출력 후 재입력 대기

**판정**: 재입력 요청 시 **PASS** / 생성 진행 또는 crash 시 **FAIL**

---

### TC-16-B. 수량 음수 입력

**실행 순서**: Sample 생성 → 수량 `-5` 입력

**기대 결과**: 재입력 요청

**판정**: 재입력 요청 시 **PASS**

---

### TC-16-C. 수량 최대치 초과 입력

**실행 순서**: Sample 생성 → 수량 `1001` 입력

**기대 결과**: `[오류] 최대 1000건까지 가능합니다:` 출력 후 재입력 대기

**판정**: 재입력 요청 시 **PASS**

---

### TC-16-D. 수량 문자 입력

**실행 순서**: Sample 생성 → 수량 `abc` 입력

**기대 결과**: 숫자 재입력 요청

**판정**: 재입력 요청 시 **PASS** / crash 또는 무한루프 시 **FAIL**

---

### TC-16-E. 수량 실수 입력

**실행 순서**: Sample 생성 → 수량 `3.5` 입력

**기대 결과**: `3`으로 처리되거나 재입력 요청 (정수 파싱 동작)

**판정**: crash 없음 + 생성 결과 정상 시 **PASS**

---

### TC-16-F. Overwrite 여부 잘못된 입력

**실행 순서**: Sample 생성 → 수량 입력 → Overwrite 여부 `X` 입력

**기대 결과**: `[오류] Y 또는 N을 입력하세요:` 출력 후 재입력 대기

**판정**: 재입력 요청 시 **PASS**

---

### TC-16-G. 메뉴 범위 외 숫자 입력

**실행 순서**: Dummy 데이터 생성 서브메뉴 → `9` 입력

**기대 결과**: `[오류] 0~4 사이의 숫자를 입력하세요:` 출력 후 재입력 대기

**판정**: 재입력 요청 시 **PASS**

---

### TC-16-H. Sample 없을 때 Order 생성 시도

**사전 조건**: `data/samples.json` → `{ "nextId": 1, "items": [] }`

**실행 순서**: Dummy 데이터 생성 → `2. Order Dummy 생성`

**기대 결과**: `[오류] Order 생성 실패 — 먼저 Sample 데이터를 생성하세요.` 출력 후 중단

**판정**: 오류 메시지 출력 + 파일 미변경 시 **PASS**

---

### TC-16-I. Order 없을 때 ProductionLine 생성 시도

**사전 조건**: Sample 10건 존재, orders.json 비어 있음

**실행 순서**: Dummy 데이터 생성 → `3. ProductionLine Dummy 생성`

**기대 결과**: `[오류] ProductionLine 생성 실패 — 먼저 Order 데이터를 생성하세요.` 출력 후 중단

**판정**: 오류 메시지 출력 + 파일 미변경 시 **PASS**

---

## 17. DataMonitor 연동 테스트

> 이 섹션은 DummyDataGenerator로 생성한 데이터를 DataMonitor가 정상적으로 읽고 처리하는지 검증한다.

### TC-17-A. DummyDataGenerator 생성 데이터 — DataMonitor 시료 목록 표시

**실행 순서**

1. DummyDataGenerator → Sample `5`건 Overwrite 생성
2. 최상위 메뉴 → DataMonitor → 모니터링 → `3. 시료 재고 현황`

**기대 결과**

- 시료 5건이 목록에 표시된다
- crash 없음

**판정**: 5건 표시 시 **PASS** / 0건 또는 crash 시 **FAIL**

---

### TC-17-B. DummyDataGenerator 생성 데이터 — 주문 상태별 집계

**실행 순서**

1. DummyDataGenerator → Sample `5`건, Order `100`건 Overwrite 생성
2. DataMonitor → 모니터링 → `2. 주문 상태별 집계`

**기대 결과**

- RESERVED + PRODUCING + CONFIRMED + RELEASE + 합계가 모두 0이 아님
- REJECTED는 집계에서 제외됨
- 합계 = 전체 100건 중 REJECTED 건수를 뺀 값

**판정**: 0이 아닌 정상 집계 표시 시 **PASS**

---

### TC-17-C. productName ↔ sample.name 매칭 검증 (demand 계산)

**목적**: DummyDataGenerator가 설정한 `order.productName`이 `sample.name`과 일치하여 DataMonitor의 수요 계산이 올바르게 동작하는지 확인한다.

**실행 순서**

1. DummyDataGenerator → Sample `3`건, Order `30`건 Overwrite 생성
2. DataMonitor → 모니터링 → `3. 시료 재고 현황`

**기대 결과**

- 3개 Sample 중 RESERVED·PRODUCING 주문이 있는 Sample의 demand > 0
- demand > 0인 Sample이 최소 1건 이상 존재
- demand = 0인 Sample만 있으면 모든 Stock이 우연히 0인 경우만 해당

**검증 보조 (코드 리뷰)**

`data/orders.json`에서 임의 Order의 `productName`을 선택 →  
`data/samples.json`에서 동일 `name`을 가진 Sample 존재 여부 확인

**판정**: 최소 1건 이상 demand > 0으로 표시 + productName-name 매칭 확인 시 **PASS** / 모든 demand = 0 시 **FAIL**

---

### TC-17-D. 재고 상태 계산 정확성

**목적**: DummyDataGenerator 데이터로 SURPLUS·SHORTAGE·DEPLETED가 올바르게 계산되는지 확인한다.

**실행 순서**

1. DummyDataGenerator → Sample `10`건, Order `200`건 Overwrite 생성
2. DataMonitor → 모니터링 → `3. 시료 재고 현황`
3. stock=0인 Sample → `고갈` 표시 여부 확인
4. stock > demand인 Sample → `여유` 표시 여부 확인

**판정**: 각 상태가 정의에 맞게 표시 시 **PASS**

---

### TC-17-E. 대량 데이터 DataMonitor 처리

**목적**: 대량 데이터에서도 DataMonitor가 정상 동작하는지 확인한다.

**실행 순서**

1. DummyDataGenerator → Sample `100`건, Order `10,000`건 Overwrite 생성
2. DataMonitor → 모니터링 → `2. 주문 상태별 집계`

**기대 결과**

- 화면에 집계 결과가 표시된다
- crash 없음
- 합계가 ~9,000건 내외 (REJECTED ~10% 제외)

**판정**: crash 없음 + 집계 결과 표시 시 **PASS**

---

### TC-17-F. JSON 필드 호환성 — 추가 필드 무시 검증

**목적**: DummyDataGenerator가 추가한 필드(`avgProductionTime`, `yield`, `sampleId`, `customerName`)를 DataMonitor가 무시하고 정상 로드하는지 확인한다.

**검증 방법**: 코드 리뷰

| 검증 항목 | 기대 동작 | 판정 |
|---|---|---|
| `SampleRepository::fromJson`이 `id`, `name`, `description`, `stock`만 읽음 | 확인 | |
| `OrderRepository::fromJson`이 `id`, `sampleId`, `productName`, `customerName`, `quantity`, `status` 모두 읽음 | 확인 | |
| 추가 필드로 인한 파싱 오류 없음 | 없어야 함 | |

**판정**: 추가 필드 무시 + 정상 로드 확인 시 **PASS**

---

## 18. JSON 파일 구조 검증

### TC-18-A. 최상위 구조 검증

**목적**: 세 JSON 파일이 DataMonitor가 기대하는 최상위 구조를 갖추고 있는지 확인한다.

**검증 대상**: `data/samples.json`, `data/orders.json`, `data/production_lines.json`

| 점검 항목 | 기대 상태 | 판정 |
|---|---|---|
| 최상위가 `{` 로 시작하고 `}` 로 끝남 | 충족 | |
| `"nextId"` 키가 최상위에 존재 | 존재 | |
| `"items"` 키가 최상위에 존재 | 존재 | |
| `"items"` 값이 배열(`[...]`) | 충족 | |

**판정**: 세 파일 모두 구조 충족 시 **PASS**

---

### TC-18-B. Sample 필드 완전성

**실행 후 검증 대상**: `data/samples.json`의 임의 항목 3개

| 필드 | 타입 | 허용 범위 | 판정 |
|---|---|---|---|
| `id` | 정수 | 1 이상 | |
| `name` | 문자열 | `"플랜-XX샘"` 형식 | |
| `description` | 문자열 | 비어있지 않음 | |
| `stock` | 정수 | 0~50 | |
| `avgProductionTime` | 정수 | 2~12 | |
| `yield` | 실수 | 60.0~99.9 (소수점 1자리) | |

**판정**: 모든 필드 존재 + 범위 충족 시 **PASS**

---

### TC-18-C. Order 필드 완전성

**검증 대상**: `data/orders.json`의 임의 항목 3개

| 필드 | 타입 | 허용 범위 | 판정 |
|---|---|---|---|
| `id` | 정수 | 1 이상 | |
| `sampleId` | 정수 | 1 ≤ sampleId ≤ maxSampleId | |
| `productName` | 문자열 | 해당 sampleId의 Sample.name과 동일 | |
| `customerName` | 문자열 | 비어있지 않음 | |
| `quantity` | 정수 | 1~20 | |
| `status` | 문자열 | RESERVED·PRODUCING·CONFIRMED·RELEASE·REJECTED 중 하나 | |

**판정**: 모든 필드 존재 + 범위 충족 시 **PASS**

---

### TC-18-D. ProductionLine 필드 완전성

**검증 대상**: `data/production_lines.json`의 임의 항목 3개

| 필드 | 타입 | 허용 범위 | 판정 |
|---|---|---|---|
| `id` | 정수 | 1 이상 | |
| `orderId` | 정수 | 1 ≤ orderId ≤ maxOrderId | |
| `lineName` | 문자열 | `"LINE-X00"` 형식 | |
| `status` | 문자열 | IDLE·RUNNING·PAUSED·DONE 중 하나 | |
| `progress` | 정수 | 0~100 (DONE이면 정확히 100) | |

**판정**: 모든 필드 존재 + 범위 충족 시 **PASS**

---

### TC-18-E. sampleId 유효성 — 교차 검증

**목적**: `orders.json`의 모든 `sampleId`가 `samples.json`에 존재하는 id를 가리키는지 확인한다.

**실행 순서**

1. DummyDataGenerator → Sample `5`건, Order `50`건 Overwrite 생성
2. `orders.json`의 `sampleId` 집합 추출
3. `samples.json`의 `id` 집합 추출
4. 두 집합의 교집합 확인

**기대 결과**: `orders.json`의 모든 sampleId가 `samples.json`의 id 집합에 속함 (범위: 1~5)

**판정**: 집합 포함 관계 충족 시 **PASS** / 범위 초과 sampleId 존재 시 **FAIL**

---

### TC-18-F. orderId 유효성 — 교차 검증

**목적**: `production_lines.json`의 모든 `orderId`가 `orders.json`에 존재하는 id를 가리키는지 확인한다.

**실행 순서**

1. Sample `5`건, Order `20`건, ProductionLine `30`건 Overwrite 생성
2. `production_lines.json`의 `orderId` 집합 추출
3. `orders.json`의 `id` 집합 추출
4. 포함 관계 확인

**기대 결과**: 모든 orderId가 1~20 범위 내

**판정**: 범위 이탈 없음 시 **PASS**

---

### TC-18-G. nextId 정확성 검증

**목적**: `nextId` 값이 실제 마지막 id + 1인지 확인한다.

**검증 공식**: `nextId == max(items[*].id) + 1`

| 검증 대상 파일 | 기대 nextId | 판정 |
|---|---|---|
| `samples.json` (5건 생성) | 6 | |
| `orders.json` (20건 생성) | 21 | |
| `production_lines.json` (10건 생성) | 11 | |

**판정**: 모든 파일의 nextId 정확 시 **PASS**

---

### TC-18-H. JSON 특수문자 이스케이프 검증

**목적**: 한글 및 특수문자가 포함된 문자열이 정상적으로 이스케이프되는지 확인한다.

**검증 방법**: 코드 리뷰 (`JsonUtil::escapeString` 사용 여부)

| 점검 항목 | 기대 상태 | 판정 |
|---|---|---|
| `serializeSample`이 `JsonUtil::escapeString` 사용 | 사용 | |
| `serializeOrder`가 `JsonUtil::escapeString` 사용 | 사용 | |
| `serializeProductionLine`이 `JsonUtil::escapeString` 사용 | 사용 | |
| 직접 따옴표(`"`) 문자열 조합 없음 | 없어야 함 | |

**판정**: 모든 항목 충족 시 **PASS**

---

## 19. Clean Code 점검 항목 (DummyDataGenerator — 코드 리뷰)

### TC-19-A. 역할 분리 검증

**목적**: 각 레이어가 자신의 책임만 수행하는지 확인한다.

**허용 의존 방향**

```
main.cpp
  └─ DummyDataController → ConsoleView       (UI 입출력)
  └─ DummyDataController → GeneratorService  (생성 흐름 조율)
      └─ GeneratorService → *Generator       (랜덤 데이터 생성)
      └─ GeneratorService → JsonFileWriter   (파일 저장)
          └─ JsonFileWriter → JsonFileStorage / JsonUtil
```

| 점검 항목 | 기대 상태 | 판정 |
|---|---|---|
| `ConsoleView`에 생성 로직 없음 | 없어야 함 | |
| `ConsoleView`에 파일 I/O 없음 | 없어야 함 | |
| `DummyDataController`에 랜덤 생성 로직 없음 | 없어야 함 | |
| `DummyDataController`에 JSON 직렬화 없음 | 없어야 함 | |
| `*Generator`에 파일 I/O 없음 | 없어야 함 | |
| `*Generator`에 `std::cout` 없음 | 없어야 함 | |
| `JsonFileWriter`에 랜덤 생성 없음 | 없어야 함 | |
| `GeneratorService`에 직접 `fstream` 없음 | 없어야 함 | |

**판정**: 모든 항목 충족 시 **PASS**

---

### TC-19-B. 네이밍 규칙

| 대상 | 규칙 | 파일 예시 | 판정 |
|---|---|---|---|
| 클래스명 | PascalCase | `SampleGenerator`, `JsonFileWriter` | |
| 메서드명 | camelCase | `generate()`, `writeSamples()` | |
| 멤버 변수 | `m_` 접두사 | `m_random`, `m_writer` | |
| 상수 (anonymous ns) | `k` 접두사 또는 UPPER_SNAKE | `kStockMax`, `kOrderStatuses` | |
| 구조체 필드 | camelCase | `avgProductionTime`, `lineName` | |

**판정**: 모든 파일에서 규칙 일관 적용 시 **PASS**

---

### TC-19-C. 헤더 가드

| 점검 항목 | 기대 상태 | 판정 |
|---|---|---|
| `src/generators/*.h`의 모든 파일에 `#pragma once` | 존재 | |
| `src/persistence/JsonFileWriter.h`에 `#pragma once` | 존재 | |
| `src/services/GeneratorService.h`에 `#pragma once` | 존재 | |
| `src/views/ConsoleView.h`에 `#pragma once` | 존재 | |
| `src/controllers/DummyDataController.h`에 `#pragma once` | 존재 | |
| `#ifndef` 방식과 혼용 없음 | 혼용 없어야 함 | |

**판정**: 모든 항목 충족 시 **PASS**

---

### TC-19-D. 금지 패턴

**검증 위치**: `src/generators/`, `src/services/GeneratorService.cpp`, `src/persistence/JsonFileWriter.cpp`, `src/views/ConsoleView.cpp`, `src/controllers/DummyDataController.cpp`

| 점검 항목 | 기대 상태 | 판정 |
|---|---|---|
| `using namespace std;` 없음 | 없어야 함 | |
| 전역 변수 없음 | 없어야 함 | |
| `exit()` / `abort()` 없음 | 없어야 함 | |
| `std::rand()` / `srand()` 없음 (mt19937만 허용) | 없어야 함 | |
| `Generator` 클래스에서 직접 `fstream` 사용 없음 | 없어야 함 | |

**판정**: 모든 항목 충족 시 **PASS**

---

### TC-19-E. 함수 규모

| 점검 항목 | 기준 | 파일 | 판정 |
|---|---|---|---|
| 함수 길이 30줄 이하 | 초과 없어야 함 | 모든 .cpp | |
| 중첩 깊이 3단계 이하 | 초과 없어야 함 | 모든 .cpp | |
| 함수 하나가 하나의 책임 | 충족 | 모든 .cpp | |

**판정**: 모든 함수 기준 충족 시 **PASS**

---

### TC-19-F. 매직 상수 관리

| 점검 위치 | 점검 항목 | 기대 상태 | 판정 |
|---|---|---|---|
| `SampleGenerator.cpp` | `kStockMin`, `kStockMax` 등 상수 정의 | 정의됨 | |
| `OrderGenerator.cpp` | `kOrderStatuses[]` 배열에 가중치 정의 | 정의됨 | |
| `OrderGenerator.cpp` | `kQuantityMin`, `kQuantityMax` 상수 정의 | 정의됨 | |
| `ProductionLineGenerator.cpp` | `kProgressDone`, `kProgressMax` 상수 정의 | 정의됨 | |
| `DummyDataController.h` | `kMaxSample`, `kMaxOrder`, `kMaxLine` 상수 정의 | 정의됨 | |
| 모든 Generator | 상수 대신 리터럴 숫자 직접 사용 없음 | 없어야 함 | |

**판정**: 모든 항목 충족 시 **PASS**

---

### TC-19-G. 주석 규칙

| 점검 항목 | 기대 상태 | 판정 |
|---|---|---|
| `// TODO`, `// FIXME` 잔존 없음 | 없어야 함 | |
| 멀티라인 `/* ... */` 블록 주석 없음 | 없어야 함 | |
| 존재하는 주석이 "왜(Why)"를 설명 | 충족 | |
| 코드가 하는 일(What)을 중복 설명하는 주석 없음 | 없어야 함 | |

**판정**: 모든 항목 충족 시 **PASS**

---

## 테스트 결과 기록표

### DataMonitor 기능 테스트 (TC-01~TC-10)

| TC | 분류 | 테스트 항목 | 결과 | 비고 |
|---|---|---|---|---|
| TC-01-A | 실행 | 정상 실행 | | |
| TC-01-B | 실행 | DataMonitor 메뉴 진입 | | |
| TC-01-C | 실행 | 모니터링 메뉴 진입 | | |
| TC-01-D | 실행 | 뒤로가기 처리 | | |
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
| TC-09-B | MVC | 의존성 방향 검증 | | |
| TC-10-A | Clean Code | 네이밍 규칙 | | |
| TC-10-B | Clean Code | 금지 패턴 | | |

### DummyDataGenerator 테스트 (TC-11~TC-19)

| TC | 분류 | 테스트 항목 | 결과 | 비고 |
|---|---|---|---|---|
| TC-11-A | 생성 | 최상위 메뉴 진입 | | |
| TC-11-B | 생성 | Sample 단독 생성 기본 흐름 | | |
| TC-11-C | 생성 | Order 단독 생성 기본 흐름 | | |
| TC-11-D | 생성 | ProductionLine 단독 생성 기본 흐름 | | |
| TC-11-E | 생성 | 전체 생성 기본 흐름 | | |
| TC-11-F | 생성 | 돌아가기 처리 | | |
| TC-12-A | JSON 저장 | samples.json 형식 검증 | | |
| TC-12-B | JSON 저장 | orders.json 형식 검증 | | |
| TC-12-C | JSON 저장 | production_lines.json 형식 검증 | | |
| TC-12-D | JSON 저장 | 빈 배열 형식 | | |
| TC-12-E | JSON 저장 | nextId 연속성 | | |
| TC-13-A | Overwrite | Append 모드 기존 데이터 보존 | | |
| TC-13-B | Overwrite | Overwrite 모드 완전 교체 | | |
| TC-13-C | Overwrite | 취소 처리 (N 입력) | | |
| TC-13-D | Overwrite | 경고 메시지 확인 | | |
| TC-13-E | Overwrite | Append 반복 nextId 단조 증가 | | |
| TC-14-A | 랜덤 | Order status 가중치 분포 | | |
| TC-14-B | 랜덤 | Sample yield 범위 | | |
| TC-14-C | 랜덤 | Sample stock 범위 | | |
| TC-14-D | 랜덤 | Sample avgProductionTime 범위 | | |
| TC-14-E | 랜덤 | DONE → progress=100 고정 | | |
| TC-14-F | 랜덤 | 비-DONE progress 범위 | | |
| TC-14-G | 랜덤 | 매 실행마다 다른 결과 | | |
| TC-14-H | 랜덤 | Order quantity 범위 | | |
| TC-15-A | 대량 | Sample 1,000건 생성 | | |
| TC-15-B | 대량 | Order 10,000건 생성 | | |
| TC-15-C | 대량 | ProductionLine 10,000건 생성 | | |
| TC-15-D | 대량 | 전체 대량 생성 | | |
| TC-15-E | 대량 | Append 반복 파일 무결성 | | |
| TC-16-A | 입력 오류 | 수량 0 입력 | | |
| TC-16-B | 입력 오류 | 수량 음수 입력 | | |
| TC-16-C | 입력 오류 | 수량 최대치 초과 | | |
| TC-16-D | 입력 오류 | 수량 문자 입력 | | |
| TC-16-E | 입력 오류 | 수량 실수 입력 | | |
| TC-16-F | 입력 오류 | Overwrite 잘못된 입력 | | |
| TC-16-G | 입력 오류 | 메뉴 범위 외 숫자 | | |
| TC-16-H | 입력 오류 | Sample 없을 때 Order 생성 | | |
| TC-16-I | 입력 오류 | Order 없을 때 ProductionLine 생성 | | |
| TC-17-A | DataMonitor 연동 | 시료 목록 표시 | | |
| TC-17-B | DataMonitor 연동 | 주문 상태별 집계 | | |
| TC-17-C | DataMonitor 연동 | productName-sample.name 매칭 | | |
| TC-17-D | DataMonitor 연동 | 재고 상태 계산 정확성 | | |
| TC-17-E | DataMonitor 연동 | 대량 데이터 처리 | | |
| TC-17-F | DataMonitor 연동 | JSON 필드 호환성 | | |
| TC-18-A | JSON 구조 | 최상위 구조 검증 | | |
| TC-18-B | JSON 구조 | Sample 필드 완전성 | | |
| TC-18-C | JSON 구조 | Order 필드 완전성 | | |
| TC-18-D | JSON 구조 | ProductionLine 필드 완전성 | | |
| TC-18-E | JSON 구조 | sampleId 교차 검증 | | |
| TC-18-F | JSON 구조 | orderId 교차 검증 | | |
| TC-18-G | JSON 구조 | nextId 정확성 | | |
| TC-18-H | JSON 구조 | 특수문자 이스케이프 | | |
| TC-19-A | Clean Code | 역할 분리 검증 | | |
| TC-19-B | Clean Code | 네이밍 규칙 | | |
| TC-19-C | Clean Code | 헤더 가드 | | |
| TC-19-D | Clean Code | 금지 패턴 | | |
| TC-19-E | Clean Code | 함수 규모 | | |
| TC-19-F | Clean Code | 매직 상수 관리 | | |
| TC-19-G | Clean Code | 주석 규칙 | | |

결과: `PASS` / `FAIL` / `SKIP`
