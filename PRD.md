# PRD — DummyDataGenerator PoC

Product Requirements Document  
버전: 1.0 | 작성일: 2026-05-08

---

## 배경 및 목적

기존 DataMonitor Tool은 수동으로 입력한 소량 데이터(Sample 3건, Order 12건)로만 검증되었습니다.  
이번 PoC는 대량 데이터(최대 10,000건) 환경에서 MVC 구조, JSON 영속성, 모니터링 기능의 안정성을 자동으로 검증하기 위한 Dummy 데이터 생성 기능을 추가합니다.

---

## 1. 기능 요구사항

### FR-01. Dummy 데이터 생성 메뉴 진입

- 메인 메뉴에 `6. Dummy 데이터 생성` 항목이 추가된다.
- 해당 항목 선택 시 Dummy 데이터 생성 서브메뉴로 진입한다.

### FR-02. 생성 대상 선택

- 사용자는 생성 대상을 선택할 수 있다.
  - `1. Sample 생성`
  - `2. Order 생성`
  - `3. ProductionLine 생성`
  - `4. 전체 생성 (Sample → Order → ProductionLine 순서)`

### FR-03. 생성 수량 입력

- 생성 대상 선택 후 생성 수량을 입력받는다.
- 허용 범위를 벗어나면 재입력을 요청한다.
- 수량 범위는 대상별로 다르다.

| 대상 | 최소 | 최대 |
|------|------|------|
| Sample | 1 | 1,000 |
| Order | 1 | 10,000 |
| ProductionLine | 1 | 10,000 |

### FR-04. 파일 덮어쓰기 여부 확인

- 생성 수량 입력 후 기존 데이터 처리 방식을 묻는다.
  - `Y (추가)`: 기존 데이터를 유지하고 새 데이터를 추가한다.
  - `N (덮어쓰기)`: 기존 데이터를 모두 삭제하고 새 데이터로 교체한다.
- 덮어쓰기 선택 시 추가 확인 메시지를 출력한다.

### FR-05. 데이터 생성 및 저장

- 선택된 Generator가 메모리에 전체 데이터를 생성한 후 파일에 1회 저장한다.
- 저장 완료 후 생성 건수와 파일 경로를 출력한다.

### FR-06. 의존성 보호

- Order 생성 시 Sample 데이터가 없으면 생성을 중단하고 안내 메시지를 출력한다.
- ProductionLine 생성 시 Order 데이터가 없으면 생성을 중단하고 안내 메시지를 출력한다.

### FR-07. 수량 입력 오류 처리

- 0 이하 입력 시: `"1 이상의 수를 입력하세요."` 출력 후 재입력.
- 최대치 초과 입력 시: `"최대 N건까지 가능합니다."` 출력 후 재입력.
- 숫자가 아닌 입력 시: `"숫자를 입력하세요."` 출력 후 재입력.

---

## 2. 생성 대상 데이터 정의

### 2-1. Sample

| 필드 | 타입 | 생성 방식 |
|------|------|------|
| id | int | 기존 nextId에서 순차 증가 |
| name | string | `"플랜-"` + 랜덤 알파벳 2자 + `"샘"` |
| description | string | 미리 정의된 설명 풀에서 랜덤 선택 |
| stock | int | 0 ~ 50 균등 분포 |
| avgProductionTime | int | 2 ~ 12 균등 분포 |
| yield | double | 60.0 ~ 99.9 균등 분포 (소수점 1자리) |

### 2-2. Order

| 필드 | 타입 | 생성 방식 |
|------|------|------|
| id | int | 기존 nextId에서 순차 증가 |
| sampleId | int | 유효한 Sample ID 범위 내 랜덤 선택 |
| productName | string | 참조된 Sample의 name 그대로 사용 |
| customerName | string | 미리 정의된 고객명 풀에서 랜덤 선택 |
| quantity | int | 1 ~ 20 균등 분포 |
| status | string | 가중치 기반 랜덤 선택 (아래 정책 참조) |

### 2-3. ProductionLine

| 필드 | 타입 | 생성 방식 |
|------|------|------|
| id | int | 기존 nextId에서 순차 증가 |
| orderId | int | 유효한 Order ID 범위 내 랜덤 선택 |
| lineName | string | `"LINE-"` + A~Z 랜덤 1자 + 숫자 2자리 |
| status | string | IDLE / RUNNING / PAUSED / DONE 균등 랜덤 |
| progress | int | 0 ~ 100 균등 분포 (DONE이면 100 고정) |

---

## 3. 랜덤 생성 정책

### 3-1. 난수 엔진

- `std::mt19937` (Mersenne Twister) 사용.
- 시드: `std::random_device` 기반 자동 결정.
- 분포: `std::uniform_int_distribution`, `std::uniform_real_distribution`.
- 외부 랜덤 라이브러리 사용 금지.

### 3-2. Order Status 가중치 정책

단순 균등 분포를 사용하면 DataMonitor 검증 시 모든 상태가 비슷하게 나와 모니터링 기능 검증이 어렵습니다. 실제 운영 분포를 모사한 가중치를 적용합니다.

| 상태 | 가중치 | 비율 |
|------|------|------|
| RESERVED | 30 | 30% |
| PRODUCING | 25 | 25% |
| CONFIRMED | 20 | 20% |
| RELEASE | 15 | 15% |
| REJECTED | 10 | 10% |

### 3-3. 문자열 풀 (Pool) 방식

아래 데이터는 코드 내 정적 배열로 정의하고 인덱스를 랜덤으로 선택합니다.

**고객명 풀 (예시)**
- 삼성전자, SK하이닉스, 인텔, TSMC, AMD, 마이크론, 퀄컴, 엔비디아, 도시바, 웨스턴디지털

**설명 풀 (예시)**
- `"A타입 반도체 검사 플랜"`, `"B타입 고속 처리 플랜"`, `"C타입 저전력 플랜"`,  
  `"D타입 고밀도 집적 플랜"`, `"E타입 신뢰성 검사 플랜"`

### 3-4. yield 소수점 처리

`yield` 값은 소수점 첫째 자리까지 생성합니다. (예: 85.3, 92.7)

---

## 4. 완료 조건 (Definition of Done)

아래 항목이 모두 충족되면 이번 PoC를 완료로 간주합니다.

| # | 항목 |
|---|------|
| 1 | Sample 100건 생성 후 `data/samples.json`에 정상 저장 |
| 2 | Order 1,000건 생성 후 `data/orders.json`에 정상 저장 |
| 3 | ProductionLine 1,000건 생성 후 `data/production_lines.json`에 정상 저장 |
| 4 | 생성된 JSON의 `nextId` 값이 실제 items 수와 일치 |
| 5 | Order의 `sampleId`가 유효한 Sample ID를 참조함 |
| 6 | ProductionLine의 `orderId`가 유효한 Order ID를 참조함 |
| 7 | Order Status 분포가 가중치 정책을 크게 벗어나지 않음 (±10%p 허용) |
| 8 | Append 모드: 기존 데이터 보존 + 새 데이터 추가 확인 |
| 9 | Overwrite 모드: 기존 데이터 완전 교체 확인 |
| 10 | Sample 없이 Order 생성 시도 → 안내 메시지 출력 후 중단 |
| 11 | DataMonitor에서 대량 데이터를 정상적으로 집계 확인 |
| 12 | 기존 MVC 구조, 레이어 의존성 방향 유지 |

---

## 5. 제외 범위 (Out of Scope)

| 항목 | 제외 이유 |
|------|------|
| DB 연동 (SQLite 포함) | 제약사항 — JSON 파일만 사용 |
| 자동화 테스트 프레임워크 (gtest 등) | 외부 라이브러리 최소화 방침 |
| 롤백 트랜잭션 | PoC 수준 — 저장 실패 시 에러 메시지로 충분 |
| 데이터 백업 파일 생성 | Overwrite 시 백업 없음, 사용자 확인으로 대체 |
| GUI / 웹 인터페이스 | 콘솔 기반 제약사항 |
| 날짜/시간 필드 | 이번 PoC 데이터 모델에 미포함 |
| 분산 생성 / 멀티스레드 | PoC 단계 범위 초과 |
| 외부 파일에서 생성 규칙 로드 | 정적 코드 내 풀 정의로 충분 |
