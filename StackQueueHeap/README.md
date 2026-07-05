# Stack, Queue, Heap 구현 과제

## 1. 과제 내용

배열 기반 자료구조와 자료구조 변환을 직접 구현하였습니다.

* 스택 구현
* 큐 구현
* 원형 큐 구현
* 스택을 이용한 큐 구현
* 큐를 이용한 스택 구현
* 배열 기반 힙 구현

모든 자료구조는 `template`으로 구현하여 여러 자료형에 사용할 수 있도록 구성하였습니다.

---

## 2. 파일 구성

```text
Stack/
 ├─ Stack.h
 └─ QueueBasedStack.h

Queue/
 ├─ Queue.h
 └─ StackBasedQueue.h

Heap/
 └─ Heap.h

StackQueueHeap.cpp
```

| 파일                   | 내용                      |
| -------------------- | ----------------------- |
| `Stack.h`            | 배열 기반 스택 구현             |
| `Queue.h`            | 일반 큐 및 원형 큐 구현          |
| `StackBasedQueue.h`  | 스택을 이용한 큐 구현            |
| `QueueBasedStack.h`  | 큐를 이용한 스택 구현            |
| `Heap.h`             | 배열 기반 이진 힙 구현           |
| `StackQueueHeap.cpp` | 각 자료구조의 동작을 검증하는 테스트 코드 |

---

## 3. 구현 내용

### 3-1. Stack

배열의 끝을 스택의 top으로 사용하여 LIFO(Last In, First Out) 구조를 구현하였습니다.

```cpp
Stack<int> stack;

stack.push(10);
stack.push(20);

stack.top(); // 20
stack.pop();

stack.top(); // 10
```

| 함수            | 설명                  |  시간복잡도 |
| ------------- | ------------------- | -----: |
| `push(value)` | 스택의 top에 원소를 추가합니다. | `O(1)` |
| `pop()`       | top 원소를 제거합니다.      | `O(1)` |
| `top()`       | top 원소를 반환합니다.      | `O(1)` |
| `empty()`     | 스택이 비어 있는지 확인합니다.   | `O(1)` |
| `size()`      | 현재 원소 개수를 반환합니다.    | `O(1)` |

---

### 3-2. Queue

배열의 앞쪽을 큐의 front로 사용하여 FIFO(First In, First Out) 구조를 구현하였습니다.

`dequeue()` 시 첫 번째 원소를 제거한 뒤, 남은 원소를 한 칸씩 앞으로 이동시킵니다.

```cpp
Queue<int> queue;

queue.enqueue(10);
queue.enqueue(20);
queue.enqueue(30);

queue.front(); // 10
queue.dequeue();

queue.front(); // 20
```

| 함수               | 설명                                |  시간복잡도 |
| ---------------- | --------------------------------- | -----: |
| `enqueue(value)` | 큐의 뒤에 원소를 추가합니다.                  | `O(1)` |
| `dequeue()`      | front 원소를 제거하고 남은 원소를 앞으로 이동시킵니다. | `O(N)` |
| `front()`        | 가장 먼저 들어온 원소를 반환합니다.              | `O(1)` |
| `empty()`        | 큐가 비어 있는지 확인합니다.                  | `O(1)` |
| `size()`         | 현재 원소 개수를 반환합니다.                  | `O(1)` |

---

### 3-3. CircularQueue

일반 큐에서 발생하는 원소 이동 비용을 줄이기 위해 원형 큐를 구현하였습니다.

`HeadIndex`는 다음에 제거할 원소의 위치를, `TailIndex`는 다음에 삽입할 위치를 가리킵니다. 인덱스는 `% Capacity` 연산을 사용하여 배열의 끝에 도달하면 다시 처음으로 돌아가도록 구현하였습니다.

```cpp
CircularQueue<int> queue;

queue.enqueue(10);
queue.enqueue(20);
queue.enqueue(30);

queue.dequeue(); // 10 제거
queue.enqueue(40);
```

| 함수               | 설명                       |  시간복잡도 |
| ---------------- | ------------------------ | -----: |
| `enqueue(value)` | tail 위치에 원소를 추가합니다.      | `O(1)` |
| `dequeue()`      | head 인덱스를 다음 위치로 이동시킵니다. | `O(1)` |
| `front()`        | 현재 head 원소를 반환합니다.       | `O(1)` |
| `empty()`        | 큐가 비어 있는지 확인합니다.         | `O(1)` |
| `size()`         | 현재 원소 개수를 반환합니다.         | `O(1)` |

---

### 3-4. StackBasedQueue

스택만 사용하여 큐를 구현하였습니다.

새 원소를 삽입할 때 기존 스택의 원소를 임시 스택으로 모두 옮깁니다. 이후 새 원소를 삽입하고, 임시 스택의 원소를 다시 원래 스택으로 되돌립니다.

이 과정을 거치면 가장 먼저 들어온 원소가 항상 스택의 top에 위치하므로, `front()`와 `dequeue()`를 스택의 `top()`과 `pop()`으로 처리할 수 있습니다.

```cpp
StackBasedQueue<int> queue;

queue.enqueue(10);
queue.enqueue(20);
queue.enqueue(30);

queue.front(); // 10
queue.dequeue();

queue.front(); // 20
```

| 함수               | 설명                       |  시간복잡도 |
| ---------------- | ------------------------ | -----: |
| `enqueue(value)` | 두 스택을 이용하여 삽입 순서를 유지합니다. | `O(N)` |
| `dequeue()`      | 스택의 top 원소를 제거합니다.       | `O(1)` |
| `front()`        | 스택의 top 원소를 반환합니다.       | `O(1)` |
| `empty()`        | 큐가 비어 있는지 확인합니다.         | `O(1)` |
| `size()`         | 현재 원소 개수를 반환합니다.         | `O(1)` |

---

### 3-5. QueueBasedStack

큐만 사용하여 스택을 구현하였습니다.

새 원소를 enqueue한 뒤, 기존 원소들을 다시 뒤로 enqueue하여 새 원소가 항상 큐의 front에 위치하도록 구현하였습니다. 따라서 `top()`과 `pop()`은 큐의 `front()`와 `dequeue()`로 처리할 수 있습니다.

```cpp
QueueBasedStack<int> stack;

stack.push(10);
stack.push(20);
stack.push(30);

stack.top(); // 30
stack.pop();

stack.top(); // 20
```

| 함수            | 설명                       | 논리적 시간복잡도 | 현재 구현 기준 시간복잡도 |
| ------------- | ------------------------ | --------: | -------------: |
| `push(value)` | 새 원소 삽입 후 기존 원소를 회전시킵니다. |    `O(N)` |        `O(N²)` |
| `pop()`       | 큐의 front 원소를 제거합니다.      |    `O(1)` |         `O(N)` |
| `top()`       | 큐의 front 원소를 반환합니다.      |    `O(1)` |         `O(1)` |
| `empty()`     | 스택이 비어 있는지 확인합니다.        |    `O(1)` |         `O(1)` |
| `size()`      | 현재 원소 개수를 반환합니다.         |    `O(1)` |         `O(1)` |

현재 `Queue`의 `dequeue()`는 배열 원소를 앞으로 한 칸씩 이동시키므로 `O(N)`입니다. 따라서 큐 기반 스택의 `push()`는 원소 회전 과정에서 `dequeue()`를 여러 번 호출하여, 현재 구현 기준으로는 `O(N²)`이 됩니다.

---

### 3-6. Heap

배열을 이용하여 완전 이진 트리 형태의 힙을 구현하였습니다.

부모 노드와 자식 노드의 인덱스 관계는 다음과 같습니다.

```text
부모 인덱스: (index - 1) / 2
왼쪽 자식 인덱스: index * 2 + 1
오른쪽 자식 인덱스: index * 2 + 2
```

원소 삽입 시에는 `siftUp()`을 사용하여 힙 조건을 유지하고, root 제거 시에는 마지막 원소를 root로 옮긴 뒤 `siftDown()`을 수행합니다.

```cpp
Heap<int> maxHeap;

maxHeap.push(10);
maxHeap.push(30);
maxHeap.push(20);

maxHeap.top(); // 30
maxHeap.pop();

maxHeap.top(); // 20
```

| 함수            | 설명                       |      시간복잡도 |
| ------------- | ------------------------ | ---------: |
| `push(value)` | 마지막 위치에 삽입한 뒤 위로 이동시킵니다. | `O(log N)` |
| `pop()`       | root를 제거한 뒤 아래로 이동시킵니다.  | `O(log N)` |
| `top()`       | root 원소를 반환합니다.          |     `O(1)` |
| `empty()`     | 힙이 비어 있는지 확인합니다.         |     `O(1)` |
| `size()`      | 현재 원소 개수를 반환합니다.         |     `O(1)` |

### 비교 함수 사용

기본 비교 함수는 `std::less<T>`이며, 큰 값이 우선되는 최대 힙으로 동작합니다.

```cpp
Heap<int> maxHeap;
```

`std::greater<T>`를 사용하면 작은 값이 우선되는 최소 힙으로 사용할 수 있습니다.

```cpp
Heap<int, 1000, std::greater<int>> minHeap;
```

| 비교 함수             | top에 위치하는 값 | 힙 종류 |
| ----------------- | ----------- | ---- |
| `std::less<T>`    | 가장 큰 값      | 최대 힙 |
| `std::greater<T>` | 가장 작은 값     | 최소 힙 |

---

## 4. 테스트

`StackQueueHeap.cpp`에서 `assert`를 사용하여 각 자료구조의 기본 동작을 검증하였습니다.

### 스택 테스트

* 생성 직후 `empty() == true`인지 확인합니다.
* `push()` 후 `top()`이 마지막으로 삽입한 원소를 반환하는지 확인합니다.
* `pop()` 후 이전 원소가 top이 되는지 확인합니다.
* 모든 원소를 제거한 후 다시 비어 있는지 확인합니다.

### 큐 테스트

* 생성 직후 `empty() == true`인지 확인합니다.
* `enqueue()`한 순서대로 `front()`가 유지되는지 확인합니다.
* `dequeue()` 후 다음 원소가 front가 되는지 확인합니다.
* 모든 원소를 제거한 후 다시 비어 있는지 확인합니다.

### 힙 테스트

다음 값을 순서대로 삽입하여 최대 힙과 최소 힙을 모두 검증하였습니다.

```text
10, 30, 20, 50, 40
```

최대 힙의 pop 순서는 다음과 같습니다.

```text
50, 40, 30, 20, 10
```

최소 힙의 pop 순서는 다음과 같습니다.

```text
10, 20, 30, 40, 50
```
