
class Heap:
    def __init__(self, arr = [], isMin = True):
        self.heap = []
        self.isMin = isMin
        for el in arr:
            self.insert(el)
    def parent(self, index):
        return (index - 1)//2
    def left(self, index):
        return 2*index + 1
    def right(self, index):
        return 2*index + 2
    def sift_up(self, index):
        if index < 0 or index >= len(self.heap): return
        parent = self.parent(index)
        while index and not (self.heap[parent] < self.heap[index] and self.isMin or self.heap[parent] > self.heap[index] and not self.isMin):
            self.heap[parent], self.heap[index] = self.heap[index], self.heap[parent]
            index, parent = parent, self.parent(index)
    def sift_down(self, index):
        if index < 0 or index >= len(self.heap): return
        mIndex = index
        while True:
            left, right = self.left(index), self.right(index)
            if right < len(self.heap) and (self.heap[right] < self.heap[mIndex] and self.isMin or self.heap[right] > self.heap[mIndex] and not self.isMin): mIndex = right
            if left < len(self.heap) and (self.heap[left] < self.heap[mIndex] and self.isMin or self.heap[left] > self.heap[mIndex] and not self.isMin): mIndex = left
            if mIndex == index: return
            else:
                self.heap[index], self.heap[mIndex] = self.heap[mIndex], self.heap[index]
                index = mIndex
    def pop(self):
        if not self.heap: return
        mElement = self.heap[0]
        self.heap[0] = self.heap[-1]
        del self.heap[-1]
        self.sift_down(0)
        return mElement
    def insert(self, element):
        self.heap.append(element)
        self.sift_up(len(self.heap)-1)
    def size(self):
        return len(self.heap)