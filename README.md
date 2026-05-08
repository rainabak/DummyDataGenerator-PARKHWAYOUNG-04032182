# DummyDataGenerator

C++ 콘솔 기반 테스트용 Dummy 데이터 자동 생성 Tool입니다.  
반도체 검사 공정 시뮬레이션을 위한 Sample, Order, ProductionLine 데이터를 JSON 형식으로 자동 생성합니다.

---

## 프로젝트 개요

| 항목 | 내용 |
|------|------|
| 언어 | C++20 |
| 빌드 환경 | Visual Studio 2022 |
| 실행 환경 | Windows 콘솔 |
| 데이터 저장 | JSON 파일 (외부 라이브러리 없음) |
| 아키텍처 | MVC + Repository + Service 계층 |

### 구현 완료 기능

- MVC 구조 기반 콘솔 UI
- JSON 파일 기반 데이터 영속성 (수동 파싱, 외부 라이브러리 없음)
- Sample / Order CRUD
- DataMonitor Tool (주문 상태 집계, 시료 재고 현황)
- **Dummy 데이터 자동 생성** (이번 PoC)

---

## 실행 방법

### 1. 빌드

Visual Studio 2022에서 `DummyDataGenerator.vcxproj`를 열고 빌드합니다.

```
솔루션 탐색기 → DummyDataGenerator → 빌드 (Ctrl+Shift+B)
```

### 2. 실행

빌드 결과물(`DummyDataGenerator.exe`)을 프로젝트 루트에서 실행합니다.

```
DummyDataGenerator.exe
```

> `data/` 디렉토리는 실행 파일과 같은 위치에 있어야 합니다.  
> 디렉토리가 없으면 자동으로 생성됩니다.

### 3. 메인 메뉴

```
========================================
     DummyDataGenerator
========================================
 1. 시료 관리
 2. 주문 관리
 3. 생산 라인 관리
 4. 모니터링
 5. 출하 관리
 6. Dummy 데이터 생성
 0. 종료
========================================
선택 >
```

---

## 생성 가능한 데이터

### 1. Sample (시료)

반도체 검사 시료 정보입니다.

| 필드 | 타입 | 설명 |
|------|------|------|
| id | 정수 | 순차 자동 증가 |
| name | 문자열 | `"플랜-XX샘"` 형식, 랜덤 생성 |
| description | 문자열 | 미리 정의된 설명 풀에서 선택 |
| stock | 정수 | 0 ~ 50 범위 랜덤 |
| avgProductionTime | 정수 | 2 ~ 12 범위 랜덤 (시간 단위) |
| yield | 실수 | 60.0 ~ 99.9 범위 랜덤 (수율 %) |

### 2. Order (주문)

시료 기반 생산 주문 정보입니다. Sample 데이터가 먼저 존재해야 생성 가능합니다.

| 필드 | 타입 | 설명 |
|------|------|------|
| id | 정수 | 순차 자동 증가 |
| sampleId | 정수 | 기존 Sample ID 범위 내 랜덤 선택 |
| productName | 문자열 | 참조된 Sample의 name |
| customerName | 문자열 | 미리 정의된 고객명 풀에서 선택 |
| quantity | 정수 | 1 ~ 20 범위 랜덤 |
| status | 문자열 | 아래 상태 중 가중치 랜덤 선택 |

**Order 상태 및 가중치**

| 상태 | 의미 | 가중치 |
|------|------|------|
| RESERVED | 예약됨 | 30% |
| PRODUCING | 생산 중 | 25% |
| CONFIRMED | 확정됨 | 20% |
| RELEASE | 출고됨 | 15% |
| REJECTED | 거절됨 | 10% |

### 3. ProductionLine (생산 라인)

Order 기반 생산 라인 가동 정보입니다. Order 데이터가 먼저 존재해야 생성 가능합니다.

| 필드 | 타입 | 설명 |
|------|------|------|
| id | 정수 | 순차 자동 증가 |
| orderId | 정수 | 기존 Order ID 범위 내 랜덤 선택 |
| lineName | 문자열 | `"LINE-A01"` 형식, 랜덤 생성 |
| status | 문자열 | IDLE / RUNNING / PAUSED / DONE 중 랜덤 |
| progress | 정수 | 0 ~ 100 랜덤 (DONE이면 100 고정) |

---

## JSON 파일 구조

모든 데이터 파일은 `data/` 디렉토리에 저장됩니다.

### 공통 구조

```json
{
  "nextId": <다음_ID>,
  "items": [
    { 필드1: 값1, 필드2: 값2 },
    ...
  ]
}
```

### data/samples.json

```json
{
  "nextId": 4,
  "items": [
    {
      "id": 1,
      "name": "플랜-A샘",
      "description": "A타입 반도체 검사 플랜",
      "stock": 10,
      "avgProductionTime": 4,
      "yield": 95.5
    }
  ]
}
```

### data/orders.json

```json
{
  "nextId": 13,
  "items": [
    {
      "id": 1,
      "sampleId": 1,
      "productName": "플랜-A샘",
      "customerName": "삼성전자",
      "quantity": 3,
      "status": "RESERVED"
    }
  ]
}
```

### data/production_lines.json

```json
{
  "nextId": 6,
  "items": [
    {
      "id": 1,
      "orderId": 3,
      "lineName": "LINE-B04",
      "status": "RUNNING",
      "progress": 67
    }
  ]
}
```

---

## 출력 예시

### Dummy 데이터 생성 메뉴

```
========================================
     Dummy 데이터 생성
========================================
 1. Sample 생성
 2. Order 생성
 3. ProductionLine 생성
 4. 전체 생성 (Sample → Order → ProductionLine)
 0. 돌아가기
========================================
선택 >
```

### 생성 옵션 입력

```
----------------------------------------
 [Sample 생성]
----------------------------------------
 생성 수량 입력 (1 ~ 1000): 100
 기존 데이터 유지 여부 (Y: 추가 / N: 덮어쓰기): Y
```

### 생성 결과 출력

```
----------------------------------------
 생성 완료
----------------------------------------
 Sample       :  100건
 저장 파일   : data/samples.json
----------------------------------------
 임의 키를 누르면 계속합니다...
```

### 전체 생성 결과 출력

```
----------------------------------------
 전체 생성 완료
----------------------------------------
 Sample       :  100건  → data/samples.json
 Order        : 1000건  → data/orders.json
 ProductionLine: 1000건 → data/production_lines.json
----------------------------------------
 임의 키를 누르면 계속합니다...
```

---

## 폴더 구조

```
DummyDataGenerator/
├── data/
│   ├── samples.json
│   ├── orders.json
│   └── production_lines.json
├── src/
│   ├── controllers/
│   ├── generators/          ← Dummy 데이터 생성 전담 레이어
│   ├── models/
│   ├── persistence/
│   ├── repositories/
│   ├── services/
│   ├── utils/
│   ├── views/
│   └── main.cpp
├── tests/
│   └── README.md
├── PRD.md
├── CLAUDE.md
└── README.md
```

## 테스트 화면
<img width="942" height="853" alt="스크린샷 2026-05-08 152555" src="https://github.com/user-attachments/assets/ab708a63-5edc-4d4a-95f1-6c563d537508" />
<img width="942" height="853" alt="스크린샷 2026-05-08 152606" src="https://github.com/user-attachments/assets/28668958-51b9-48d2-be03-9e758f0f68d0" />
<img width="942" height="853" alt="스크린샷 2026-05-08 152610" src="https://github.com/user-attachments/assets/5ad85e25-54ac-48c6-b979-ccf8e3816c81" />
<img width="942" height="853" alt="스크린샷 2026-05-08 152630" src="https://github.com/user-attachments/assets/151ffad0-bbe0-4101-8cab-ac56b510086b" />
<img width="942" height="853" alt="스크린샷 2026-05-08 152639" src="https://github.com/user-attachments/assets/cbef950d-ba24-4225-b782-c0b333ee91de" />
<img width="942" height="853" alt="스크린샷 2026-05-08 152656" src="https://github.com/user-attachments/assets/c8ae6517-1dc6-442d-a3a3-08aeb09f9919" />

