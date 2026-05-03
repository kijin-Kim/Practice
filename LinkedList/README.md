# LinkedList 구현체

단방향(`SingleLinkedList<T>`) / 양방향(`LinkedList<T>`) 연결 리스트 구현체.

| | 단방향 | 양방향 |
|---|---|---|
| STL 대응 | `std::forward_list` | `std::list` |
| 노드 포인터 | `Next` | `Prev` + `Next` |
| Tail 추적 | X | O (`m_Tail`) |
| 역방향 순회 | X | O (`operator--`) |
| 복사 생성자 | deep copy | deep copy |

---

## API

### SingleLinkedList\<T\>

| 메서드 | 설명 |
|---|---|
| `push_front(val)` | 맨 앞에 노드 추가 |
| `pop_front()` | 맨 앞 노드 제거 |
| `insert_after(iter, val)` | iter 다음 위치에 노드 삽입 |
| `erase_after(iter)` | iter 다음 노드 제거 |
| `resize(n)` | 논리 크기 변경 (축소 시 뒤 노드 해제, 확장 시 0 초기화 노드 추가) |
| `reverse()` | Next 포인터 역전 (`O(n)`) |
| `begin() / end()` | 반복자 반환 |

### LinkedList\<T\> (양방향)

| 메서드 | 설명 |
|---|---|
| `push_front(val)` | 맨 앞에 노드 추가 (`insert(begin(), val)` 위임) |
| `pop_back()` | 맨 뒤 노드 제거 (`erase(tail)` 위임) |
| `insert(iter, val)` | iter **앞** 위치에 노드 삽입 |
| `erase(iter)` | 해당 노드 제거, 다음 Iterator 반환 |
| `reverse()` | 각 노드의 Prev/Next 교환 후 Head/Tail 스왑 |
| `begin() / end()` | 반복자 반환 |

---

## 출력 결과

### SingleLinkedList\<T\>

```
insert_after(singleLinkedList.begin(), 1)
    Singly-LinkedList [A]
    0 1 0 0

push_front(1)
    Singly-LinkedList [A]
    1 0 1 0 0

Deep Copy
    Singly-LinkedList [B]
    1 0 1 0 0

erase_after(b.begin())
    Singly-LinkedList [B]
    1 1 0 0

pop_front()
    Singly-LinkedList [B]
    1 0 0

push_front(0 ~ 9)
    Singly-LinkedList [B]
    9 8 7 6 5 4 3 2 1 0 1 0 0

resize(3)
    Singly-LinkedList [B]
    9 8 7

resize(5)
    Singly-LinkedList [B]
    9 8 7 0 0

resize(3)
    Singly-LinkedList [B]
    9 8 7

reverse()
    Singly-LinkedList [B]
    7 8 9
```

---

### LinkedList\<T\> (양방향)

```
insert(linkedList.begin(), 3), insert(linkedList.begin(), 2), push_front(1)
    Doubly-LinkedList [A]
    1 2 3 0 0 0

Deep Copy
    Doubly-LinkedList [B]
    1 2 3 0 0 0

erase(a.begin())
    Doubly-LinkedList [B]
    2 3 0 0 0

pop_back()
    Doubly-LinkedList [B]
    2 3 0 0

reverse()
    Doubly-LinkedList [B]
    0 0 3 2
```

---

## 구현 특이사항

### SingleLinkedList\<T\>

| 항목 | 내용 |
|---|---|
| `resize` 축소 | 지정 크기 이후 노드를 순회하며 해제 |
| `resize` 확장 | 꼬리에서 새 노드(`Value=0`) 추가 |
| `reverse` | `lastNode`를 들고 Next를 역전하는 in-place 방식 |

### LinkedList\<T\> (양방향)

| 항목 | 내용 |
|---|---|
| `insert` 기준 | iter **앞**에 삽입 (STL `std::list::insert`와 동일) |
| Head 갱신 | `insert`에서 `current == m_Head`이면 `m_Head` 교체 |
| `erase` 반환 | 삭제된 노드의 다음 Iterator 반환 |
| `reverse` | 각 노드 Prev↔Next 교환 후 `m_Head` / `m_Tail` 스왑 |
