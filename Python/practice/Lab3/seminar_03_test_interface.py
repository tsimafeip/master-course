
import sys
import unittest
import importlib
import collections

from unittest import mock
from io import StringIO


# uncomment the line below and change the path specified
# sys.path.insert(0, r'path_to_solution_folder')


class InterfaceTestCase(unittest.TestCase):
    def setUp(self):
        self._stdout_mock = self._setup_stdout_mock()

    def _setup_stdout_mock(self):
        patcher = mock.patch('sys.stdout', new=StringIO())
        patcher.start()
        self.addCleanup(patcher.stop)
        return patcher.new

    def _check_stdout_empty(self, file_name):
        if self._stdout_mock is not None:
            self.assertFalse(self._stdout_mock.getvalue(),
                             'no prints to console are allowed in "%s"' % file_name)
                             
    def _load_task(self, task_idx, file_name):
        try:
            loaded_task = importlib.import_module(file_name)
        except ImportError:
            self.fail('cannot import task #%d solution - no file "%s"' % (task_idx, file_name))
        return loaded_task
                             
    def _load_function(self, task_idx, file_name, func_names):
        loaded_task = self._load_task(task_idx, file_name)

        func_names = (func_names, ) if isinstance(func_names, str) else func_names
        loaded_functions = list(filter(None, (getattr(loaded_task, func_name, None) for func_name in func_names)))

        self.assertEqual(1, len(loaded_functions),
                         'cannot import task #%d solution - only one of function(-s) "%s" must be in file "%s"'
                         % (task_idx, file_name, func_names))

        return loaded_functions[0]
        
    def test_transpose(self):
        f = self._load_function(0, 'iter_helpers', 'transpose')
        expected = [[1, 2], [-1, 3]]
        actual = f([[1, -1], [2, 3]])
        self.assertSequenceEqual(expected, actual)
        self._check_stdout_empty('transpose')
        
    def test_scalar_product(self):
        f = self._load_function(1, 'iter_helpers', 'scalar_product')
        
        expected = 1
        actual = f([1, '2'], [-1, 1])
        self.assertEqual(expected, actual)
        
        actual = f([1, 'xyz'], [-1, 1])
        self.assertIsNone(actual)

        self._check_stdout_empty('scalar_product')
        
    def test_profile(self):
        f = self._load_function(2, 'utils', 'profile')
        
        @f
        def some_function():
            return sum(range(1000))
        
        expected = sum(range(1000))
        actual = some_function()
        self.assertEqual(expected, actual)
        
    def test_timer(self):
        f = self._load_function(3, 'utils', 'timer')

        expected = sum(range(1000))
        
        with f():
            actual = sum(range(1000))
 
        self.assertEqual(expected, actual)
        
    def test_node(self):
        Node, flatten_linked_list = self._load_function(4, 'linked_list', ('Node', 'flatten_linked_list'))

        r1 = Node(1)  # 1 -> None - just one node
        self.assertSequenceEqual([1], list(r1))
        
        r2 = Node(7, Node(2, Node(9))) # 7 -> 2 -> 9 -> None
        self.assertSequenceEqual([7, 2, 9], list(r2))
        
        r3_expected = [3, 19, 25, 12]
        r3 = Node(3, Node(Node(19, Node(25)), Node(12)))  # 3 -> (19 -> 25 -> None) -> 12 -> None
        self.assertSequenceEqual(r3_expected, list(r3))

        r3_flattenned = flatten_linked_list(r3)  # 3 -> 19 -> 25 -> 12 -> None
        self.assertSequenceEqual(r3_expected, list(r3_flattenned))

        self._check_stdout_empty('linked_list')


if __name__ == '__main__':
    unittest.main()
