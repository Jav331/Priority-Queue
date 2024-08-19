/*
 * THis This file takes in from the pqheap.h file
 * and implements the funtionailty. This is a minHeap
 * structured array
 */
#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;    // program-wide constant
const int NONE = -1;                // used as sentinel index

/*
 * The constructor initializes all of the member variables needed for
 * an instance of the PQArray class. The allocated capacity
 * is initialized to a starting constant and a dynamic array of that
 * size is allocated. The count of filled slots is initially zero.
 */
PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated](); // new with parens will zero new memory
    _numFilled = 0;}

/*
 *The destructor is responsible for cleaning up any resources
 * used by this instance of the PQArray class. The array
 * memory that was allocated for the PQArray is deleted here.
 */
PQHeap::~PQHeap() {

    delete[] _elements;

}

/*
 * This function pairs with the enqueue funcitonality
 * and restores the structure and ordering of the minHeap
 * recursively
 */

void PQHeap::percolateUp(int index) {

    if (index == 0) {
        return;
    }

    int inputIndex = _elements[index].priority;

    int parent = getParentIndex(index);

    int parentIndex = _elements[parent].priority;


    if (inputIndex < parentIndex) {
        swapElements(index, parent);
        percolateUp(parent);
    }

}

/*
 * This function pairs with the dequeue funcitonality
 * and restores the structure and ordering of the minHeap
 * recursively. This keeps track of the current and moves it to either
 * the position of the left or right child
 */

void PQHeap::percolateDown(int index){

    int leftChild = getLeftChildIndex(index);
    int rightChild = getRightChildIndex(index);
    int current = index;

    // base case
    if (leftChild == NONE && rightChild == NONE) {
        return;
    }

    // COmpared the left first to keep the structure of the minHeap
    // grabs the index of left or right child and changes the current to the position
    if (leftChild != NONE && _elements[current].priority > _elements[leftChild].priority) {
        current = leftChild;
    }
    if (rightChild != NONE && _elements[current].priority > _elements[rightChild].priority) {
        current = rightChild;
    }



    if (current != index) {
        swapElements(index, current);
        percolateDown(current);
    }


}


/*
 * This function takes a maxed out array and doubles
 * the size
 */
void PQHeap::resizeArray() {

    DataPoint *largerArray = new DataPoint[_numAllocated * 2];

    for (int i = 0; i < _numFilled; i++) {
        largerArray[i] = _elements[i];
    }

    delete[] _elements;

    _elements = largerArray;

    _numAllocated = _numAllocated * 2;

}

/*
 * This function enqueues an element at the end of the minHeap
 * and percolates up until the ordering/structure is true to a minHeap.
 * It also resizes the array if it is full
 */
void PQHeap::enqueue(DataPoint elem) {

    if (size() >= _numAllocated) {
        resizeArray();
    }

    _elements[size()] = elem;
    _numFilled++;


    int index = size() - 1;
    percolateUp(index);
}

/*
 * The elements in the internal array are stored in decreasing order
 * of priority value. The element at index 0 is the least urgent
 * (largest priority value) and the element in the last-filled index
 * is the most urgent (minimum priority value). peek() returns the
 * frontmost elements, which is stored at the last-filled index.
 * peek() raises an error if the queue is empty.
 */
DataPoint PQHeap::peek() const {
    if (isEmpty()) {
        error("Cannot access front element of empty pqueue!");
    }
    return _elements[0];
}

/*
 * This function dequeues an element at the top of the minHeap
 * and percolates down until the ordering/structure is true to a minHeap.
 */
DataPoint PQHeap::dequeue() {
    if (isEmpty()) {
        error("Empty PQHeap");
    }


    DataPoint front = peek();

    swapElements(0, size() - 1);
    _numFilled--;
    percolateDown(0);


    return front;
}

/*
 * Returns true if the queue contains no elements, false otherwise
 */
bool PQHeap::isEmpty() const {
    return size() == 0;
}

/*
 * The count of enqueued elements is tracked in the
 * member variable _numFilled.
 */
int PQHeap::size() const {
    return _numFilled;
}

/*
 * Updates internal state to reflect that the queue is empty, i.e. count
 * of filled slots is reset to zero. The array memory remains allocated
 * at current capacity. The contents of the internal array do not need to
 * be cleared; the contents will be overwritten when additional values
 * are enqueued.
 */
void PQHeap::clear() {
    _numFilled = 0;}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the parent of the
 * specified child index. If this child has no parent, return
 * the sentinel value NONE.
 */
int PQHeap::getParentIndex(int child) const {
    if (child != 0) {
        return (child - 1) / 2;
    }
    return NONE;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the left child of the
 * specified parent index. If this parent has no left child, return
 * the sentinel value NONE.
 */
int PQHeap::getLeftChildIndex(int parent) const {

    int leftChild = (2 * parent) + 1;

    if (leftChild >= size()) {
        return NONE;
    }
    return leftChild;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the right child of the
 * specified parent index. If this parent has no right child, return
 * the sentinel value NONE.
 */
int PQHeap::getRightChildIndex(int parent) const {
    int rightChild = (2 * parent) + 2;

    if (rightChild >= size()) {
        return NONE;
    }
    return rightChild;
}

/*
 * We strongly recommend implementing this helper function that swaps
 * array elements. There is a swapElements member function implemented
 * in PQAarray that you start from. Be sure to include error bounds-checking
 * it is an invaluable defense against indexing mistakes!
 */
void PQHeap::swapElements(int indexA, int indexB) {
    validateIndex(indexA);
    validateIndex(indexB);
    DataPoint tmp = _elements[indexA];
    _elements[indexA] = _elements[indexB];
    _elements[indexB] = tmp;
}

void PQHeap::validateIndex(int index) const {
    if (index < 0 || index >= _numFilled) error("Invalid index " + integerToString(index));
}
/*
 * To confirm the validity of the internal array, you must check that
 * the heap property holds for all elements in the array. If elements are
 * found that violate the heap property, report an error.
 */
void PQHeap::debugConfirmInternalArray() const {
    /*
     * If there are more elements than spots in the array, we have a problem.
     */
    if (_numFilled > _numAllocated) error("Too many elements in not enough space!");

    /* Loop over internal array and compare priority of neighboring elements.
     * If element at index i has larger priority than at index i-1,
     * these two elements are out of order expected for PQArray.
     */
    for (int i = 1; i < size(); i++) {
        int parent = getParentIndex(i);
        if (parent != NONE && _elements[parent].priority > _elements[i].priority) {
            error("PQArray elements out of order: indexes " + integerToString(parent) + " and " + integerToString(i));
        }
    }}

Vector<DataPoint> PQHeap::debugGetInternalArrayContents() const {
    Vector<DataPoint> v;

    for (int i = 0; i < size(); i++) {
        v.add(_elements[i]);
    }
    return v;
}

void PQHeap::debugSetInternalArrayContents(const Vector<DataPoint>& v, int capacity) {
    if (v.size() > capacity || capacity == 0) {
        error("Invalid capacity for debugSetInternalArrayContents!");
    }
    delete[] _elements;                     // discard old memory
    _elements = new DataPoint[capacity];    // allocate new memory
    _numAllocated = capacity;
    _numFilled = v.size();
    for (int i = 0; i < v.size(); i++) {    // fill contents with copy from vector
        _elements[i] = v[i];
    }
    debugConfirmInternalArray(); }


/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Add your own custom tests here! */



STUDENT_TEST("Dequeueue on empty pq") {
    PQHeap pq;
    Vector<DataPoint> expected = {{"T", 1}, {"B", 3}, {"G", 2}, {"S", 6}, {"A", 5},
                                  {"V", 9}, {"R", 4}, {"O", 8}, {"K", 7}};
    pq.enqueue({"R", 4});
    pq.enqueue({"A", 5});
    pq.enqueue({"B", 3});
    pq.enqueue({"K", 7});
    pq.enqueue({"G", 2});
    pq.enqueue({"V", 9});
    pq.enqueue({"T", 1});
    pq.enqueue({"O", 8});
    pq.enqueue({"S", 6});

    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), expected);

    pq.dequeue();

    expected = {{"G", 2}, {"B", 3}, {"R", 4}, {"S", 6}, {"A", 5},
                {"V", 9}, {"K", 7}, {"O", 8}};

    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), expected);

    pq.dequeue();

    expected = {{"B", 3}, {"A", 5}, {"R", 4}, {"S", 6}, {"O", 8}, {"V", 9}, {"K", 7}};
    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), expected);

    pq.dequeue();
    pq.dequeue();
    pq.dequeue();
    pq.dequeue();
    pq.dequeue();
    pq.dequeue();

    expected = {{"V", 9}};

    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), expected);

    pq.dequeue();
    expected = {};
    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), expected);

    EXPECT_ERROR(pq.dequeue());
}

STUDENT_TEST("Dequeue percolate down") {
    PQHeap pq;
    Vector<DataPoint> expected = {{"T", 1}, {"B", 3}, {"G", 2}, {"S", 6}, {"A", 5},
                                  {"V", 9}, {"R", 4}, {"O", 8}, {"K", 7}};
    pq.enqueue({"R", 4});
    pq.enqueue({"A", 5});
    pq.enqueue({"B", 3});
    pq.enqueue({"K", 7});
    pq.enqueue({"G", 2});
    pq.enqueue({"V", 9});
    pq.enqueue({"T", 1});
    pq.enqueue({"O", 8});
    pq.enqueue({"S", 6});

    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), expected);

    pq.dequeue();

    expected = {{"G", 2}, {"B", 3}, {"R", 4}, {"S", 6}, {"A", 5},
                {"V", 9}, {"K", 7}, {"O", 8}};

    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), expected);

    pq.dequeue();

    expected = {{"B", 3}, {"A", 5}, {"R", 4}, {"S", 6}, {"O", 8}, {"V", 9}, {"K", 7}};
    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), expected);

    pq.dequeue();
    pq.dequeue();
    pq.dequeue();
    pq.dequeue();
    pq.dequeue();
    pq.dequeue();

    expected = {{"V", 9}};

    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), expected);

    pq.dequeue();
    expected = {};
    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), expected);


}

STUDENT_TEST("Enqueue percolate up") {


    // {"R", 4}
    // {"A", 5}
    // {"B", 3}
    // {"K", 7}
    // {"G", 2}
    // {"V", 9}
    // {"T", 1}
    // {"O", 8}
    // {"S", 6}
    PQHeap pq;
    Vector<DataPoint> expected = {{"T", 1}, {"B", 3}, {"G", 2}, {"S", 6}, {"A", 5},
                                  {"V", 9}, {"R", 4}, {"O", 8}, {"K", 7}};

    pq.enqueue({"R", 4});
    pq.enqueue({"A", 5});
    pq.enqueue({"B", 3});
    pq.enqueue({"K", 7});
    pq.enqueue({"G", 2});
    pq.enqueue({"V", 9});
    pq.enqueue({"T", 1});
    pq.enqueue({"O", 8});
    pq.enqueue({"S", 6});



    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), expected);


    pq.enqueue({"Z", 0});
    expected = {{"Z", 0}, {"T", 1}, {"G", 2}, {"S", 6}, {"B", 3}, {"V", 9},
                {"R", 4}, {"O", 8}, {"K", 7}, {"A", 5}};


    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), expected);


}

STUDENT_TEST("PQHeap example in writeup, confirm contents of internal array") {
    PQHeap pq;
    Vector<DataPoint> input = {
                               {"R", 4}, {"A", 5}, {"B", 3}, {"K", 7}, {"G", 2},
                               {"V", 9}, {"T", 1}, {"O", 8}, {"S", 6} };

    pq.debugConfirmInternalArray();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.debugConfirmInternalArray();
    }

    /* TODO: Complete this student test */

    // What is contents of pq internal array right now?

    Vector<DataPoint> expected = {{"T", 1}, {"B", 3}, {"G", 2}, {"S", 6}, {"A", 5}, {"V", 9},{"R", 4}, {"O", 8}, {"K", 7}};

    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), expected);

    // How can your test case verify contents are as expected?
    pq.dequeue();
    pq.dequeue();

    expected = {{"B", 3}, {"A", 5}, {"R", 4}, {"S", 6}, {"O", 8}, {"V", 9}, {"K", 7}};
    // What is contents of internal array after two dequeue operations?
    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), expected);

}
