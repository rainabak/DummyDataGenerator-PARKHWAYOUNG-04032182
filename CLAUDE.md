# CLAUDE.md — DummyDataGenerator 작업 규칙

이 파일은 Claude가 이 프로젝트에서 작업할 때 반드시 따라야 할 규칙을 정의합니다.

---

## 1. Claude 작업 규칙

### 작업 전 필수 확인

- 작업 시작 전 반드시 관련 파일을 Read한 후 Edit한다. 내용을 읽지 않고 덮어쓰지 않는다.
- 기존 코드의 네이밍 컨벤션, 들여쓰기, 구조를 먼저 파악하고 그 스타일을 따른다.
- 새 파일 작성 전, 동일한 레이어의 기존 파일을 참고하여 패턴을 맞춘다.

### 작업 범위 제한

- 요청된 기능만 구현한다. 요청하지 않은 리팩토링, 파일 정리, 추상화 추가를 하지 않는다.
- 기존에 동작하는 코드(`controllers/`, `views/`, `repositories/`, `services/`, `persistence/`)는 수정하지 않는다. 새 파일을 추가하고 `main.cpp`에서만 연결한다.
- `main.cpp`는 의존성 주입(DI) 전용 파일이다. 비즈니스 로직을 추가하지 않는다.

### 코드 작성 금지 사항

- `using namespace std;` 사용 금지.
- 전역 변수 선언 금지.
- `exit()`, `abort()` 호출 금지.
- `std::cout` / `std::cin`을 Controller / Service / Repository에서 직접 호출 금지.
- `fstream` 직접 사용 금지 — `JsonFileStorage`를 통해서만 파일에 접근한다.
- 외부 라이브러리 추가 금지 (헤더 전용 라이브러리 포함).

---

## 2. JSON 생성 규칙

### 파일 형식 준수

모든 JSON 파일은 아래 구조를 따른다. 이 구조를 변경하지 않는다.

```json
{
  "nextId": <다음_ID_정수>,
  "items": [
    { ... },
    { ... }
  ]
}
```

### 직렬화 규칙

- `JsonUtil::escapeString()`을 반드시 통해 문자열 값을 이스케이프한다. 직접 따옴표를 붙이지 않는다.
- 정수 필드는 따옴표 없이 직렬화한다. (`"quantity": 5` → 맞음, `"quantity": "5"` → 틀림)
- 실수 필드는 소수점 1자리로 고정 출력한다. (`"yield": 85.3`)
- 배열 마지막 항목에 trailing comma를 붙이지 않는다.
- 파일 인코딩은 UTF-8 BOM 포함으로 저장한다. (`JsonFileStorage`의 기존 동작을 그대로 사용한다.)

### nextId 관리

- Append 모드: 기존 `nextId`를 읽어 이어서 증가한다.
- Overwrite 모드: `nextId`를 1부터 재시작한다.
- `persist()` 호출 직전 `nextId`를 최종 확정한다. 중간 값을 파일에 쓰지 않는다.

### 저장 시점

- Generator가 전체 벡터를 메모리에 완성한 후 `persist()`를 1회 호출한다.
- 항목별로 `persist()`를 반복 호출하지 않는다 (I/O 최소화).

---

## 3. Clean Code 규칙

### 네이밍

| 대상 | 규칙 | 예시 |
|------|------|------|
| 클래스 | PascalCase | `SampleGenerator`, `RandomUtil` |
| 메서드 | camelCase | `generate()`, `buildJson()` |
| 멤버 변수 | `m_` 접두사 | `m_config`, `m_engine` |
| 지역 변수 | camelCase | `sampleCount`, `jsonBody` |
| 상수 | `k` 접두사 또는 `static const` | `kMaxSampleCount` |
| 열거형 값 | UPPER_SNAKE_CASE | `RESERVED`, `RUNNING` |

### 헤더 파일

- 모든 `.h` 파일 첫 줄에 `#pragma once`를 작성한다.
- `#ifndef` 방식의 include guard와 혼용하지 않는다.
- `.h` 파일에 구현 코드를 넣지 않는다 (템플릿 제외).

### 함수 설계

- 함수 하나는 하나의 책임만 수행한다.
- 함수 길이는 30줄 이하를 유지한다.
- 중첩 깊이는 3단계 이하를 유지한다.
- 매직 숫자와 매직 문자열을 함수 안에 직접 쓰지 않는다. 상수로 분리한다.

### 주석

- 주석은 **"왜(Why)"** 가 자명하지 않을 때만 작성한다.
- 코드가 하는 일(What)을 설명하는 주석은 작성하지 않는다. 잘 지은 이름이 그 역할을 한다.
- `// TODO`, `// FIXME`, `// HACK` 을 커밋에 남기지 않는다.
- 멀티라인 `/* ... */` 블록 주석을 사용하지 않는다.

### 레이어 의존성 방향

아래 방향만 허용한다. 역방향 의존성을 만들지 않는다.

```
main.cpp
  └─ Controller → View
  └─ Controller → Service → Repository → JsonFileStorage
  └─ Controller → Generator → RandomUtil
```

---

## 4. 랜덤 데이터 생성 규칙

### RandomUtil 사용 원칙

- 모든 Generator는 `RandomUtil`을 통해서만 난수를 생성한다.
- `std::rand()` / `srand()` 사용 금지. `std::mt19937`만 허용한다.
- `RandomUtil` 인스턴스를 Generator 생성자에서 주입받는다 (의존성 주입).

### 데이터 생성 원칙

- 필드 생성 로직은 `generate()` 메서드 내부에 집중한다. View나 Repository에 분산하지 않는다.
- 상태(status) 가중치 배열은 Generator 내 `static const` 배열로 정의한다.
- 문자열 풀(Pool) 배열은 Generator 내 `static const` 배열로 정의한다.

### 의존성 보호

- `OrderGenerator`는 유효한 `sampleId` 범위를 외부에서 주입받는다. 직접 Repository를 참조하지 않는다.
- `ProductionLineGenerator`는 유효한 `orderId` 범위를 외부에서 주입받는다. 직접 Repository를 참조하지 않는다.
- 범위가 빈 경우(예: Sample 0건) Generator를 호출하지 않는다. Controller에서 사전 검사한다.

### GeneratorConfig

- 생성 수량, Overwrite 여부 등 파라미터는 `GeneratorConfig` 구조체 하나에 묶어 전달한다.
- 개별 파라미터를 여러 인자로 전달하지 않는다.

---

## 5. Commit 규칙

### 커밋 단위

한 커밋은 하나의 논리적 작업 단위를 담는다. 기능과 무관한 정리(cleanup)를 함께 커밋하지 않는다.

| 단계 | 내용 |
|------|------|
| 1 | `ProductionLine` 모델 추가 |
| 2 | `RandomUtil` 추가 |
| 3 | `GeneratorConfig` + `SampleGenerator` 추가 |
| 4 | `OrderGenerator` + `ProductionLineGenerator` 추가 |
| 5 | `IProductionLineRepository` + `ProductionLineRepository` 추가 |
| 6 | `DummyDataView` 추가 |
| 7 | `DummyDataController` 추가 + `main.cpp` 연결 |
| 8 | 통합 검증 완료 후 최종 커밋 |

### 커밋 전 체크리스트

- [ ] 빌드 성공 (경고 0건 목표, 에러 0건 필수)
- [ ] 기존 기능(DataMonitor 등) 동작 이상 없음
- [ ] 새 파일에 `#pragma once` 포함 여부 확인
- [ ] `using namespace std;` 미사용 확인
- [ ] 매직 문자열 / 매직 숫자 잔존 없음 확인

### 커밋 메시지 형식

```
<타입>: <한 줄 요약>

[본문 — 선택사항, 왜(Why)가 자명하지 않을 때만 작성]

Co-Authored-By: Claude Sonnet 4.6 (1M context) <noreply@anthropic.com>
```

**타입 목록**

| 타입 | 사용 시점 |
|------|------|
| `Add` | 새 파일 / 새 기능 추가 |
| `Update` | 기존 기능 수정 또는 개선 |
| `Fix` | 버그 수정 |
| `Refactor` | 동작 변경 없는 코드 정리 |
| `Docs` | 문서 작성 또는 수정 |

**커밋 메시지 예시**

```
Add: SampleGenerator with mt19937 random engine

Add: ProductionLineRepository with JSON persistence
```
