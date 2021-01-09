from ac_diskstructs import Heap


def test_Heap_impl(test_dir, non_existent_record, data):
    record_size = len(data[0])

    for i in data:
        assert len(i) == record_size, 'All records must be of the same size'

    path = test_dir.file()

    def check_Heap_record_size(heap):
        assert heap.record_size() == record_size, 'Record size must be the same'

    def check_Heap_contents(heap):
        ok = False
        prefixes = {}

        for i in data:
            assert i.encode('utf-8') == heap.get(i), 'Record must be the same'

            if len(i) < 2:
                continue

            ok = True

            for prefix in ['', i[:int(len(i) / 2)]]:
                if prefix not in prefixes:
                    prefixes[prefix] = {}

                if i not in prefixes[prefix]:
                    prefixes[prefix][i] = 0

                prefixes[prefix][i] += 1

        assert ok, 'Must have records to check prefix search'
        assert heap.get(non_existent_record) is None, 'Non-existent record must not be present'

        for prefix, items in prefixes.items():
            for i in heap.get_all(prefix):
                i = i.decode('utf-8')

                assert i in items, 'Record must be expected'

                items[i] -= 1

                if items[i] < 1:
                    del items[i]

            assert len(items) == 0, 'No records are lost, no extra records'

    def create_Heap():
        heap = Heap(path, record_size=record_size)

        check_Heap_record_size(heap)

        for i in data:
            heap.insert(i)

        heap.close()

        check_Heap_contents(heap)

        heap.delete()

    def read_Heap():
        heap = Heap(path)

        check_Heap_record_size(heap)
        check_Heap_contents(heap)

    create_Heap()
    read_Heap()


def test_Heap(test_dir):
    test_Heap_impl(
        test_dir,
        "qwer",
        [
            "abcd",
            "adbc",
            "zzzz",
            "8\2\6\2",
            "abdc",
            "acdb",
        ],
    )

    test_Heap_impl(
        test_dir,
        "\0\2\3",
        [
            "\0\0\1\2",
            "\1\6\78",
            "cccc",
            "\0\0\2\1",
        ],
    )
