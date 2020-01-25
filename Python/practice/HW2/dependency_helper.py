import copy
import fnmatch
import os
from abc import abstractmethod
import ast
import unittest
from collections import OrderedDict

default_file_pattern = "*"


class DependencyHelper(object):
    def __init__(self):
        self._deps = {}

    def get_deps(self):
        return self._deps

    def add(self, indep_obj, dep_obj):
        if indep_obj in self._deps:
            # to avoid dependent values duplicates
            if dep_obj not in self.get_deps()[indep_obj]:
                self._deps[indep_obj].append(dep_obj)
        else:
            self._deps[indep_obj] = [dep_obj]

    def update(self, other):
        if isinstance(other, DependencyHelper):
            result = self.get_deps()
            for other_item in other.get_deps():
                if other_item in result:
                    result[other_item].extend(other.get_deps()[other_item])
                    result[other_item] = list(OrderedDict.fromkeys(result[other_item]))
                else:
                    result[other_item] = other.get_deps()[other_item]
            self._deps = result
        else:
            raise TypeError

    def remove(self, indep_obj, dep_obj):
        if indep_obj in self.get_deps():
            if dep_obj in self.get_deps()[indep_obj]:
                self._deps[indep_obj].remove(dep_obj)
                if not self._deps[indep_obj]:
                    del self._deps[indep_obj]

    def get_dependent(self, key):
        return self.get_deps().get(key, [])

    def __get_values_also_keys(self, key):
        values = self.get_deps()[key]
        values_also_keys = [x for x in values if x in self.get_deps().keys()]
        return values_also_keys

    def has_cycle_dependency(self):
        keys = self.get_deps().keys()
        for key in keys:
            deps = self.get_deps()[key]
            values_also_keys = self.__get_values_also_keys(key)
            while values_also_keys:
                for value_also_key in values_also_keys:
                    values_also_keys.extend(self.__get_values_also_keys(value_also_key))
                    values_also_keys.remove(value_also_key)
                    deps.extend(self.get_deps()[value_also_key])
                    if key in deps:
                        return True
        return False

    def copy(self):
        return copy.deepcopy(self)

    def __eq__(self, other):
        if isinstance(other, DependencyHelper):
            return self.get_deps() == other.get_deps()
        return NotImplemented

    def __isub__(self, tup_to_remove: tuple):
        self.remove(tup_to_remove[0], tup_to_remove[1])
        return self

    def __iadd__(self, new_tup: tuple):
        self.add(new_tup[0], new_tup[1])
        return self

    def __bool__(self):
        return not self.has_cycle_dependency()

    def __str__(self):
        str_representation = "Dependency helper: \n"
        for key in self.get_deps():
            str_representation += f"{key} : {', '.join([str(elem) for elem in self.get_deps()[key]])}\n"
        return str_representation


class PriorityHelper(DependencyHelper):
    def enumerate_priorities(self):
        if self.has_cycle_dependency():
            print('Cycle dependency found. Priorities can not be set.')
            return None

        priors = {}
        for indep_obj in self.get_deps():
            if indep_obj not in priors:
                priors[indep_obj] = 0
            values = self.get_deps()[indep_obj]
            for value in values:
                if value in priors and priors[value] > priors[indep_obj]:
                    continue
                else:
                    priors[value] = priors[indep_obj] + 1

        return priors


class Instruction(object):
    def __init__(self, name):
        self._name = name
        self._elements = []

    def set_elements(self, elements: list):
        self._elements = elements

    def get_elements(self):
        return self._elements

    def get_name(self):
        return self._name

    @abstractmethod
    def action(self, dep_helper: PriorityHelper, current_file):
        pass

    @staticmethod
    def try_parse_instruction(line):
        if len(line) > 1 and line[-1] == '(' and all(x.isupper() for x in line[:-1]):
            return line[:-1]
        else:
            return None


class PeerDirInstruction(Instruction):
    def action(self, dep_helper: PriorityHelper, current_file):
        for dir_path in self.get_elements():
            if not os.path.isabs(dir_path):
                dir_path = os.path.join(os.path.dirname(current_file), dir_path)
            for _, _, filenames in os.walk(dir_path):
                for file in filenames:
                    dep_helper.add(os.path.join(dir_path, file), current_file)


class IncludeInstruction(Instruction):
    def action(self, dep_helper: PriorityHelper, current_file):
        for file in self.get_elements():
            if not os.path.isabs(file):
                file = os.path.join(os.path.dirname(current_file), file)
            dep_helper.add(current_file, file)


class FileParser(object):
    def __init__(self, file_path, instructions, is_ast):
        self._file_path = file_path
        self._instructions = instructions
        self._is_ast = is_ast

    def parse(self, priority_helper: PriorityHelper):
        if not self._instructions and not self._is_ast:
            print("It is not an AST FileParser")
            print(f"There are no instructions passed to the FileParser, no need to parse a file '{self._file_path}'.")
            return

        with open(self._file_path, 'r', encoding='utf-8') as f:
            if self._is_ast:
                tree = ast.parse(f.read())

                analyzer = Analyzer()
                analyzer.visit(tree)
                stats = analyzer.stats
                for import_item in stats["import"]:
                    priority_helper.add(import_item, self._file_path)
                for from_item in stats["from"]:
                    priority_helper.add(from_item, self._file_path)
            else:
                active_instruction = False
                instruction_elements = []
                for line in f:
                    line = line.strip()
                    if not active_instruction:
                        my_instr = Instruction.try_parse_instruction(line)
                        if my_instr:
                            for x in self._instructions:
                                if x.get_name() == my_instr:
                                    cur_instruction = x
                                    active_instruction = True
                                    break
                    else:
                        if len(line) == 1 and line == ')':
                            cur_instruction.set_elements(instruction_elements)
                            cur_instruction.action(priority_helper, self._file_path)
                            active_instruction = False
                            instruction_elements = []
                        else:
                            instruction_elements.append(line)


class Analyzer(ast.NodeVisitor):
    def __init__(self):
        self.stats = {"import": [], "from": []}

    def visit_Import(self, node):
        for alias in node.names:
            self.stats["import"].append(alias.name)
        self.generic_visit(node)

    def visit_ImportFrom(self, node):
        for alias in node.names:
            self.stats["from"].append(alias.name)
        self.generic_visit(node)


def find_files(priority_helper, instructions, is_ast=False, mask=default_file_pattern, directory=""):
    if directory == "":
        directory = os.getcwd()

    if is_ast:
        py_pattern = '*.py'
        print("AST algorithm can work only with Python files.")
        if mask == default_file_pattern:
            print(f"Default mask '{default_file_pattern}' will be discarded, pattern '{py_pattern}' will be applied.")
            mask = py_pattern
        elif len(mask) > 3 and mask[-3:] != ".py":
            print(f"Previous mask '{mask}' will be discarded, pattern '{py_pattern}' will be applied.")
            mask = py_pattern
    for root, _, files in os.walk(directory):
        for file_name in files:
            if fnmatch.fnmatch(file_name, mask):
                full_path = os.path.join(root, file_name)
                file_parser = FileParser(full_path, instructions, is_ast)
                file_parser.parse(priority_helper)


class UnitTests(unittest.TestCase):
    def test_dependency(self):
        helper = DependencyHelper()
        helper.add(1, 2)
        self.assertFalse(helper.has_cycle_dependency())
        helper.add(2, 1)
        self.assertTrue(helper.has_cycle_dependency())
        helper = DependencyHelper()
        helper += (1, 2)
        helper += (2, 3)
        helper += (3, 1)
        self.assertTrue(helper.has_cycle_dependency())
        helper = DependencyHelper()
        helper += (1, 2)
        helper += (2, 3)
        helper += (1, 3)
        self.assertFalse(helper.has_cycle_dependency())

    def test_bool(self):
        helper = DependencyHelper()
        helper.add(1, 2)
        self.assertTrue(helper)
        helper.add(2, 1)
        self.assertFalse(helper)

    def test_add(self):
        dependency_helper = DependencyHelper()
        dependency_helper.add(2, 1)
        dependency_helper += (2, 11)
        self.assertEqual(dependency_helper.get_dependent(2), [1, 11])
        dependency_helper.add(3, 1)
        self.assertEqual(dependency_helper.get_dependent(3), [1])

    def test_remove(self):
        helper = DependencyHelper()
        helper.add(1, 2)
        try:
            helper.remove(2, 1)
        except KeyError:
            pass
        try:
            helper.remove(1, 3)
        except ValueError:
            pass
        helper.add(1, 3)
        helper.add(1, 2)
        helper.add(1, 4)
        self.assertEqual(helper.get_dependent(1), [2, 3, 4])
        helper -= (1, 2)
        self.assertEqual(helper.get_dependent(1), [3, 4])
        helper.remove(1, 4)
        self.assertEqual(helper.get_dependent(1), [3])

    def test_copy(self):
        dependency_helper = DependencyHelper()
        dependency_helper.add(1, 2)
        dependency_helper += (2, 1)
        copy_dep_helper = dependency_helper.copy()
        self.assertEqual(copy_dep_helper, dependency_helper)
        dependency_helper.add(12, 4)
        self.assertNotEqual(copy_dep_helper, dependency_helper)
        copy_dep_helper.update(dependency_helper)
        self.assertEqual(copy_dep_helper, dependency_helper)

    def test_update(self):
        dependency_helper = DependencyHelper()
        dependency_helper.add(1, 2)
        dependency_helper += (2, 1)
        copy_dep_helper = dependency_helper.copy()
        dependency_helper.add(12, 4)
        copy_dep_helper.update(dependency_helper)
        self.assertEqual(copy_dep_helper, dependency_helper)

    def test_priorities(self):
        priority_helper = PriorityHelper()
        a = 'a'
        b = 'b'
        c = 'c'
        d = 'd'
        priority_helper.add(a, b)
        priority_helper.add(a, c)
        priority_helper.add(d, c)
        self.assertEqual(priority_helper.enumerate_priorities(), {'a': 0, 'b': 1, 'c': 1, 'd': 0})
        m = 'm'
        priority_helper.add(c, m)
        self.assertEqual(priority_helper.enumerate_priorities(), {'a': 0, 'b': 1, 'c': 1, 'd': 0, 'm': 2})
        k = 'k'
        priority_helper.add(m, k)
        priority_helper.add(a, k)
        self.assertEqual(priority_helper.enumerate_priorities(), {'a': 0, 'b': 1, 'c': 1, 'd': 0, 'm': 2, 'k': 3})
        priority_helper.add(k, a)
        self.assertEqual(priority_helper.enumerate_priorities(), None)


if __name__ == '__main__':
    unittest.main()
    # 3 task
    # priority_helper = PriorityHelper()
    # instructions = [PeerDirInstruction("PEERDIR"), IncludeInstruction("INCLUDE")]
    # file_mask = "make"
    # find_files(priority_helper, instructions, file_mask)
    # print(priority_helper)
    # print(priority_helper.enumerate_priorities())

    # 4 task
    # priority_helper = PriorityHelper()
    # find_files(priority_helper, [], True)
    # print(priority_helper)
    # print(priority_helper.enumerate_priorities())
