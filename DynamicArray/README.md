# DynamicArray\<T\>

STL `std::vector` 호환 동적 배열 구현체. `bool` 특수화는 비트 패킹으로 메모리를 1/8로 절감.

- Capacity 성장 전략: `ceil(n × 1.5)`
- Deep copy 의미론 (copy constructor + copy assignment)
- `bool` 특수화: `uint8_t` 배열 + `BoolReference` proxy 패턴

---

## API

| 메서드 | 설명 |
|---|---|
| `push_back(val)` | 끝에 원소 추가, 필요 시 재할당 |
| `insert(ptr, val)` | 임의 위치 삽입, 이후 원소 shift |
| `erase(ptr)` | 임의 위치 제거, 이후 원소 shift |
| `resize(n)` | 논리 크기 변경, 필요 시 reserve |
| `reserve(n)` | capacity 선점, 재할당 최소화 |
| `operator[]` | 인덱스 접근 (const / non-const) |
| `begin() / end()` | 반복자 반환 |
| `size() / capacity()` | 논리 크기 / 할당 크기 조회 |

---

## 출력 결과

### TestInt()

```
Initialize elements
    Array [A]
    1 2

Deep Copy
    Array [B]
    1 2

push_back(4), push_back(5)
    Array [A]
    1 2 4 5

    Array [b]
    1 2

insert(a.begin() + 2, 3)
    Array [A]
    1 2 3 4 5

erase(a.begin() + 2)
    Array [A]
    1 2 4 5

a.resize(2)
    Array [A]
    1 2
```

### TestBool() — 비트 패킹 특수화

```
Initialize elements
    Array [A]
    true false true true false true true true

Deep Copy
    Array [b]
    true false true true false true true true

push_back(false), push_back(true), push_back(true), push_back(true)
    Array [A]
    true false true true false true true true false true true true

    Array [b]
    true false true true false true true true

insert(a.begin() + 2, true)
    Array [A]
    true false false true true false true true true false true true true

erase(a.begin() + 2)
    Array [A]
    true false true true false true true true false true true true

a.resize(15)
    Array [A]
    true false true true false true true true false true true true false false false

a.resize(2)
    Array [A]
    true false
```

---

## 구현 특이사항

### 일반 타입 (`DynamicArray<T>`)

- `resize(n)` 축소 시 capacity는 유지되고 논리 크기만 줄어듦
- `insert` / `erase` 는 raw pointer 기반으로 동작
- `reserve` 호출 시 기존 원소를 새 버퍼로 복사 후 구버퍼 해제

### bool 특수화 (`DynamicArray<bool>`)

- `uint8_t` 배열에 비트 단위 저장, **MSB 우선** (index 0 = bit 7)
- `operator[]` 는 `BoolReference` proxy 객체를 반환
- 바이트 수 계산: `GetPerByte(n) = (n + 7) / 8`
- `insert` / `erase` 는 비트 레벨에서 1칸씩 shift

---
