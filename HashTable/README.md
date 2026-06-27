# HashTable

체이닝(Separate Chaining) 방식으로 구현한 학습용 해시 테이블입니다.

`std::vector<std::list<T>>`를 버킷 배열로 사용하며, 같은 버킷 인덱스를 갖는 원소는 리스트에 연결해서 저장합니다.

## 구현 기능

- `insert(const T& value)`
  - 원소를 삽입합니다.
  - 이미 같은 원소가 존재하면 삽입하지 않습니다.

- `erase(const T& value)`
  - 원소를 찾아 삭제합니다.
  - 해당 원소가 없으면 아무 작업도 하지 않습니다.

- `contains(const T& value) const`
  - 원소 존재 여부를 확인합니다.

- `size() const`
  - 현재 저장된 원소 수를 반환합니다.

- `loadFactor() const`
  - 현재 load factor를 반환합니다.
  - `원소 수 / 버킷 수`로 계산합니다.

- `bucketCount() const`
  - 현재 버킷 수를 반환합니다.

---

## 충돌 처리

해시 충돌은 체이닝 방식으로 처리합니다.

```text
bucket[0] : A -> B -> C
bucket[1] : D
bucket[2] :
````

서로 다른 원소가 같은 버킷 인덱스를 가지면, 같은 버킷의 `std::list`에 함께 저장됩니다.

탐색과 삭제는 전체 테이블이 아니라 해당 버킷의 체인만 순회합니다.

---

## 해시 과정

원소는 먼저 `std::hash<T>`를 통해 `size_t` 해시값으로 변환됩니다.

```cpp
const size_t rawHash = std::hash<T>{}(value);
```

그 후 버킷 개수로 나눈 나머지를 사용하여 실제 버킷 인덱스를 결정합니다.

```cpp
const size_t index = rawHash % bucketCount;
```

즉 전체 흐름은 다음과 같습니다.

```text
value
→ std::hash<T>
→ size_t 해시값
→ hash % bucketCount
→ 버킷 인덱스
```

---

## 리사이징

삽입 후 예상 load factor가 `0.75`를 초과하면 버킷 수를 2배로 늘립니다.

```cpp
inline static constexpr float s_LoadFactorThreshold = 0.75f;
```

```text
load factor = 원소 수 / 버킷 수
```

새 원소를 삽입하기 전에 다음 값을 계산합니다.

```text
(현재 원소 수 + 1) / 현재 버킷 수
```

이 값이 임계값보다 크면 리사이징을 수행합니다.

```cpp
bool shouldResizeBeforeInsert() const
{
    return static_cast<float>(m_Size + 1) / m_BucketCount
        > s_LoadFactorThreshold;
}
```

리사이징 시에는 단순히 버킷 배열의 크기만 늘리지 않습니다.

버킷 수가 바뀌면 같은 원소라도 버킷 위치가 달라질 수 있기 때문입니다.

```text
hash(value) % oldBucketCount
≠
hash(value) % newBucketCount
```

따라서 모든 원소를 새 버킷 수 기준으로 다시 해싱합니다.

```text
기존 버킷 순회
→ 각 체인의 원소 순회
→ 새 버킷 수 기준으로 해시 계산
→ 새 버킷 배열에 삽입
→ 기존 버킷 배열 교체
```

리사이징은 모든 원소를 다시 배치하므로 한 번에 O(N)이 걸립니다.

하지만 버킷 수를 2배씩 늘리므로, 전체 삽입 과정에서는 평균적으로 분할 상환 O(1)로 볼 수 있습니다.

---

## 사용자 정의 타입 지원

템플릿 기반으로 구현되어 `int`뿐 아니라 사용자 정의 구조체도 저장할 수 있습니다.

사용자 정의 타입을 저장하려면 두 가지가 필요합니다.

1. `operator==`

   * 체인 내부에서 두 원소가 같은지 비교하기 위해 필요합니다.

2. `std::hash<T>` 특수화

   * 객체를 `size_t` 해시값으로 변환하기 위해 필요합니다.

예시:

```cpp
struct Student
{
    int id;
    std::string name;

    bool operator==(const Student& other) const
    {
        return id == other.id && name == other.name;
    }
};
```

`Student`는 `id`와 `name`이 모두 같을 때 같은 원소로 판단합니다.

```cpp
template<>
struct std::hash<Student>
{
    size_t operator()(const Student& student) const
    {
        const size_t idHash = std::hash<int>{}(student.id);
        const size_t nameHash = std::hash<std::string>{}(student.name);

        size_t seed = idHash;
        seed ^= nameHash + 0x9e3779b9 + (seed << 6) + (seed >> 2);

        return seed;
    }
};
```

`id`와 `name` 각각의 해시값을 구한 뒤 하나의 해시값으로 결합합니다.

---

## 핵심 규칙

두 원소가 같다고 판단된다면, 두 원소의 해시값도 같아야 합니다.

```text
a == b
→ hash(a) == hash(b)
```

반대로 해시값이 같다고 해서 반드시 같은 원소인 것은 아닙니다.

```text
hash(a) == hash(b)
→ a와 b가 같을 수도 있고, 다를 수도 있음
```

서로 다른 원소가 같은 해시값을 가지는 경우를 충돌이라고 합니다.

체이닝 방식에서는 충돌한 원소들을 같은 버킷의 리스트에 저장하고, `operator==`를 사용하여 실제 동일 여부를 판단합니다.

---

## 시간 복잡도

해시 함수가 원소를 비교적 고르게 분배한다고 가정하면 다음과 같습니다.

| 연산 | 평균 시간 복잡도 | 최악 시간 복잡도 |
| -- | --------- | --------- |
| 삽입 | O(1)      | O(N)      |
| 탐색 | O(1)      | O(N)      |
| 삭제 | O(1)      | O(N)      |

최악의 경우 모든 원소가 하나의 버킷에 몰릴 수 있습니다.

```text
bucket[0] : A -> B -> C -> D -> E ...
```

이 경우 체인을 처음부터 끝까지 순회해야 하므로 O(N)이 됩니다.

---

## 예제

```cpp
HashTable<Student> students(4);

students.insert({ 1, "Kim" });
students.insert({ 1, "Kim" }); // 중복이므로 삽입되지 않음
students.insert({ 1, "Lee" });
students.insert({ 2, "Kim" });

std::cout << students.size() << '\n'; // 3

std::cout << std::boolalpha;
std::cout << students.contains({ 1, "Kim" }) << '\n'; // true
std::cout << students.contains({ 3, "Kim" }) << '\n'; // false

students.erase({ 1, "Lee" });

std::cout << students.size() << '\n'; // 2
std::cout << students.contains({ 1, "Lee" }) << '\n'; // false
```

---

