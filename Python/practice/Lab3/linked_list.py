class Node(object):
    def __init__(self, value_, next_=None):
        assert isinstance(value_, int) or isinstance(value_, Node)
        self._value = value_
        self._next = next_

    def __iter__(self):
        while self is not None:
            yield self.value
            self = self.next

    def __str__(self):
        result = f'{self._value}->'
        while self.next is not None:
            self = self.next
            if isinstance(self.value, Node):
                result += f'({self.value})->'
            else:
                result += f'{self.value}->'
        result += "None"
        return result;

    @property
    def value(self):
        return self._value

    @value.setter
    def value(self, value_):
        self._value = value_

    @property
    def next(self):
        return self._next

    @next.setter
    def next(self, next_):
        self._next = next_


def flatten_linked_list(root):
    a = root
    b = root
    while a.next is not None:
        if isinstance(b.value, Node):
            after_node = a.next
            flatten_linked_list(b.value)
            a.next = b.value.next
            a.value = b.value.value
            b.value = a.value
            b.next = a.next
            while b.next is not None:
                b = b.next
            b.next = after_node
        a = a.next
        b = a
    return root


if __name__ == '__main__':
    r1 = Node(1)
    # 1 -> None - just one node
    r2 = Node(7, Node(2, Node(9)))
    # 7 -> 2 -> 9 -> None
    # r3_test = Node(Node(3, Node(4)),
    #           Node(Node(Node(19, Node(Node(25), Node(14, Node(23)))),
    #           Node(82, Node(Node(89), Node(44)))),
    #           Node(12, Node(14))))
    r3 = Node (3, Node(Node(19, Node(25)), Node(12)))
    # 3 -> (19 -> 25 -> None ) -> 12 -> None
    r3_flattenned = flatten_linked_list(r3)
    # 3 -> 19 -> 25 -> 12 -> None
    r3_expected_flattenned_collection = [3, 19, 25, 12]
    assert r3_expected_flattenned_collection == list(r3_flattenned)
