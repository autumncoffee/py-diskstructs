from ac_diskstructs import RBTree


def test_RBTree_impl(test_dir, non_existent_key, data, more_data):
    path = test_dir.file()

    def check_RBTree_contents(rbt, obj):
        ok = False
        prefixes = {}

        for (key, value) in obj:
            assert value.encode('utf-8') == rbt.get(key), f'Value of {key} must be the same'

            if len(key) < 2:
                continue

            ok = True

            prefix = key[:int(len(key) / 2)]

            if prefix not in prefixes:
                prefixes[prefix] = {}

            prefixes[prefix][key] = value

        assert ok, 'Must have keys to check prefix search'
        assert rbt.get(non_existent_key) is None, 'Non-existent key must not be present'

        for prefix, items in prefixes.items():
            for (key, value) in rbt.get_all(prefix):
                key = key.decode('utf-8')

                assert value.decode('utf-8') == items.get(key), f'Value of {key} must be the same'

                del items[key]

            assert len(items) == 0, 'No keys are lost, no extra keys'

    def create_RBTree():
        rbt = RBTree(path, create=True)

        for (key, value) in data:
            rbt[key] = value

        check_RBTree_contents(rbt, data)

        rbt.delete()

    def update_RBTree():
        rbt = RBTree(path, rw=True)

        check_RBTree_contents(rbt, data)

        for (key, value) in more_data:
            rbt[key] = value

        check_RBTree_contents(rbt, more_data)
        check_RBTree_contents(rbt, data)

        rbt.delete()

    def read_RBTree():
        rbt = RBTree(path)

        check_RBTree_contents(rbt, data)
        check_RBTree_contents(rbt, more_data)

        all = {}

        for (key, value) in rbt.get_all(''):
            all[key.decode('utf-8')] = value.decode('utf-8')

        for obj in [data, more_data]:
            for (key, value) in obj:
                assert all.get(key) == value, f'Value of {key} must be the same'

                del all[key]

        assert len(all) == 0, 'No keys are lost, no extra keys'

    create_RBTree()
    update_RBTree()
    read_RBTree()


def test_RBTree(test_dir):
    test_RBTree_impl(
        test_dir,
        "qwer",
        [
            ["abcdasd", "value"],
            ["adbc", "other value"],
            ["zz\0zz", "third value"],
            ["\8\2\6\2", "\7\7\8\2\6\2"],
            ["abd--c", " "],
            ["cad", '''Lorem ipsum dolor sit amet, consectetur adipiscing
elit, sed do eiusmod tempor incididunt ut labore et
dolore magna aliqua. Ut enim ad minim veniam, quis'''],
        ],
        [
            ["\0\0\1\21233", '''nostrud exercitation ullamco laboris nisi ut aliquip
ex ea commodo consequat. Duis aute irure dolor in
reprehenderit in voluptate velit esse cillum dolore'''],
            ["\1\6\7\8\2\5\222", "test"],
            ["\0\0\2\1\8\32", "hello!"],
        ],
    )
